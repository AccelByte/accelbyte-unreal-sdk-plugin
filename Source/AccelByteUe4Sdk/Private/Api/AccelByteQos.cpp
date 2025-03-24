// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteQos.h"
#include "Icmp.h"
#include "Networking.h"
#include "Api/AccelByteQosManagerApi.h"

#include "Core/Ping/AccelBytePing.h"

namespace AccelByte
{
namespace Api
{
FAccelByteModelsQosServerList Qos::QosServers = {};
TArray<TPair<FString, float>> Qos::Latencies = {};
TMap<FString, TSharedPtr<FInternetAddr>> Qos::ResolvedAddresses = {};
FDelegateHandleAlias Qos::PollLatenciesHandle;

Qos::Qos(Credentials& InCredentialsRef
	, const Settings& InSettingsRef
	, FAccelByteMessagingSystem& InMessagingSystemRef
	, const QosManagerWPtr InQosManagerWeak
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: CredentialsRef{InCredentialsRef.AsShared()}
	, SettingsRef{InSettingsRef}
	, QosManagerWeak{InQosManagerWeak}
#if ENGINE_MAJOR_VERSION < 5
	, MessagingSystemWPtr{InMessagingSystemRef.AsShared()}
#else
	, MessagingSystemWPtr{InMessagingSystemRef.AsWeak()}
#endif
	, ApiClient(InApiClient)
{}

Qos::~Qos()
{
	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
	if (MessagingSystemPtr.IsValid())
	{
		MessagingSystemPtr->UnsubscribeFromTopic(EAccelByteMessagingTopic::LobbyConnected, LobbyConnectedDelegateHandle);
	}
	OnLobbyConnectedHandle.Unbind();

	FTickerAlias::GetCoreTicker().RemoveTicker(QosUpdateCheckerHandle);
}

void Qos::SetApiClient(TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
{
	ApiClient = InApiClient;
}

void Qos::GetServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnSuccess
                             , const FErrorHandler& OnError)
{
	StartLatencyPollers();

	bool bPingRegionsOnSuccess = true;
	CallGetQosServers(bPingRegionsOnSuccess, OnSuccess, OnError);
}

void Qos::GetActiveServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnSuccess
	, const FErrorHandler& OnError)
{
	const QosManagerPtr QosManagerPtr = QosManagerWeak.Pin();
	if(!QosManagerPtr.IsValid())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("QosManager is invalid!"));
		return;
	}
	
	QosWPtr QosWeak = AsShared();
	QosManagerPtr->GetActiveQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda(
	[QosWeak, OnSuccess, OnError](const FAccelByteModelsQosServerList Result)
	{
		const auto QosApi = QosWeak.Pin();
		if (QosApi.IsValid())
		{
			FAccelByteModelsQosServerList ResolvedServerList{ Result };
			for (FAccelByteModelsQosServer& Server : ResolvedServerList.Servers)
			{
				QosApi->ResolveQosServerAddress(Server);
			}

			if (Result.Servers.Num() > 0)
			{
				QosApi->PingRegionsSetLatencies(ResolvedServerList, OnSuccess, OnError);
			}
			else
			{
				OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidResponse), TEXT("No QoS server available!"));
			}
		}
	}), OnError);
}

void Qos::CallGetQosServers(const bool bPingRegionsOnSuccess
	, const THandler<TArray<TPair<FString, float>>>& OnPingRegionsSuccess
	, const FErrorHandler& OnError)
{
	const QosManagerPtr QosManagerPtr = QosManagerWeak.Pin();
	if(!QosManagerPtr.IsValid())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("QosManager is invalid!"));
		return;
	}
	
	QosWPtr QosWeak = AsShared();
	QosManagerPtr->GetActiveQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda(
		[QosWeak, bPingRegionsOnSuccess, OnPingRegionsSuccess, OnError](const FAccelByteModelsQosServerList Result)
		{
			const auto QosApi = QosWeak.Pin();
			if (QosApi.IsValid())
			{
				Qos::QosServers = Result; // Cache for the session
				for (FAccelByteModelsQosServer& Server : Qos::QosServers.Servers)
				{
					QosApi->ResolveQosServerAddress(Server);
				}

				if (bPingRegionsOnSuccess)
				{
					QosApi->PingRegionsSetLatencies(Qos::QosServers, OnPingRegionsSuccess, OnError);
				}
				else
				{
					OnPingRegionsSuccess.ExecuteIfBound(Qos::Latencies); // Latencies == Nullable until PingRegionsSetLatencies called once+.
				}
			}
		}), OnError);
}

void Qos::PingRegionsSetLatencies(const FAccelByteModelsQosServerList& QosServerList
	, const THandler<TArray<TPair<FString, float>>>& OnSuccess
	, const FErrorHandler& OnError)
{
#ifdef ACCELBYTE_ACTIVATE_PROFILER
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelByteQosPing"));
#endif
	TSharedRef<TArray<TPair<FString, float>>> SuccessLatencies = MakeShared<TArray<TPair<FString, float>>>();
	TSharedRef<TArray<FString>> FailedLatencies = MakeShared<TArray<FString>>();

	TArray<FAccelByteModelsQosServer> Servers = QosServerList.Servers;
	
	Servers.RemoveAllSwap([](const FAccelByteModelsQosServer& Server) 
		{
			return !ResolvedAddresses.Contains(Server.Ip) || Server.ResolvedIp.IsEmpty();
		});
	const int32 Count = Servers.Num();

	if (Count > 0)
	{
		// For each server, ping them and record add to Latency TArray.
		for (auto& Server : Servers)
		{
			FString Region = Server.Region;

			// Ping -> Get the latencies on pong.
			QosWPtr QosWeak = AsShared();
			FAccelBytePing::SendUdpPing(Server.ResolvedIp, Server.Port, SettingsRef.QosPingTimeout, FPingCompleteDelegate::CreateLambda(
				[Count, SuccessLatencies, FailedLatencies, Region, OnSuccess, OnError, QosWeak](const FPingResult& PingResult)
				{
					if (PingResult.Status == FPingResultStatus::Success)
					{
						float PingDelay = PingResult.AverageRoundTrip * 1000; // convert to milliseconds
						SuccessLatencies->Add(TPair<FString, float>(Region, PingDelay));
					}
					else
					{
						FailedLatencies->Add(Region);
					}

					int TotalLatencies = SuccessLatencies->Num() + FailedLatencies->Num();
					if (Count == TotalLatencies)
					{
						TSharedRef<TArray<TPair<FString, float>>> ResultLatencies = MakeShared<TArray<TPair<FString, float>>>();
						ResultLatencies->Append(*SuccessLatencies);
						for (const FString& FailedLatency : *FailedLatencies)
						{
							ResultLatencies->Emplace(FailedLatency, ACCELBYTE_MAX_PING);
						}
						
						Qos::Latencies.Empty();
						Qos::Latencies.Append(*ResultLatencies);

						if (ResultLatencies->Num() > 0)
						{
							OnSuccess.ExecuteIfBound(*ResultLatencies);
							const auto QosApi = QosWeak.Pin();
							if (QosApi.IsValid())
							{
								QosApi->bQosUpdated = true;
							}
						}
						else
						{
							OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to ping all servers"));
						}
					}
				}));
		}
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to ping because no QoS server"));
	}
}

void Qos::InitGetLatenciesScheduler(float LatencyPollIntervalSecs)
{
	const bool bActivateScheduler = LatencyPollIntervalSecs > 0;

	if (Qos::PollLatenciesHandle.IsValid())
	{
		RemoveFromTicker(Qos::PollLatenciesHandle);
	}

	if (!bActivateScheduler)
	{
		return;
	}

	// Active (>0): ensure min value to prevent flooding
	const float AdjustedSecondsPerTick = LatencyPollIntervalSecs < Settings::MinNumSecsQosLatencyPolling
		? Settings::MinNumSecsQosLatencyPolling
		: LatencyPollIntervalSecs;

	// -----------------------------------
	// Schedule a Latencies refresh poller
	// Loop infinitely, every x seconds, until we tell the delegate to stop via RemoveFromTicker()
	QosWPtr QosWeak = AsShared();
	Qos::PollLatenciesHandle = FTickerAlias::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(
		[QosWeak](float DeltaTime)
		{
			const auto QosApi = QosWeak.Pin();
			if (QosApi.IsValid())
			{
				QosApi->PingRegionsSetLatencies(Qos::QosServers, nullptr, nullptr);
			}
			return true;
			
		}), AdjustedSecondsPerTick);
}

void Qos::RemoveFromTicker(FDelegateHandleAlias& Handle)
{
	if (!Handle.IsValid())
		return;
	
	FTickerAlias::GetCoreTicker().RemoveTicker(Handle);
	Handle.Reset();
}

void Qos::StartLatencyPollers()
{
	// Setup polling schedulers that will consistently refresh Latencies.
	InitGetLatenciesScheduler(SettingsRef.QosLatencyPollIntervalSecs);
}

void Qos::StopLatencyPollers()
{
	RemoveFromTicker(Qos::PollLatenciesHandle);
	Qos::Latencies.Empty();
}

bool Qos::AreLatencyPollersActive()
{
	return PollLatenciesHandle.IsValid();
}

const TArray<TPair<FString, float>>& Qos::GetCachedLatencies()
{
	return Qos::Latencies;
}

void Qos::Startup()
{
	if (!bIsStarted)
	{
		OnLobbyConnectedHandle = FOnMessagingSystemReceivedMessage::CreateThreadSafeSP(AsShared(), &Qos::OnLobbyConnected);
		auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
		if (MessagingSystemPtr.IsValid())
		{
			LobbyConnectedDelegateHandle = MessagingSystemPtr->SubscribeToTopic(EAccelByteMessagingTopic::LobbyConnected, OnLobbyConnectedHandle);
		}

		QosUpdateCheckerTickerDelegate = FTickerDelegate::CreateThreadSafeSP(AsShared(), &Qos::CheckQosUpdate);
		QosUpdateCheckerHandle = FTickerAlias::GetCoreTicker().AddTicker(QosUpdateCheckerTickerDelegate, QosUpdateCheckerIntervalSecs);
		bIsStarted = true;
	}
}

void Qos::SendQosLatenciesMessage()
{
	if (Latencies.Num() <= 0)
	{
		return;
	}

	FAccelByteModelsQosRegionLatencies RegionLatencies;

	for (const auto& Latency : Latencies)
	{
		FAccelByteModelsQosRegionLatency RegionLatency;
		RegionLatency.Region = Latency.Key;
		RegionLatency.Latency = Latency.Value;

		RegionLatencies.Data.Add(RegionLatency);
	}

	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
	if (MessagingSystemPtr.IsValid())
	{
		MessagingSystemPtr->SendMessage<FAccelByteModelsQosRegionLatencies>(EAccelByteMessagingTopic::QosRegionLatenciesUpdated, RegionLatencies);
	}
}

void Qos::OnLobbyConnected(const FString& Payload)
{
	SendQosLatenciesMessage();
}

bool Qos::CheckQosUpdate(float DeltaTime)
{
	if (bQosUpdated)
	{
		SendQosLatenciesMessage();
		bQosUpdated = false;
	}

	return true;
}

bool Qos::ResolveQosServerAddress(FAccelByteModelsQosServer& OutServer)
{
	if (OutServer.Ip.IsEmpty())
	{
		return false;
	}

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SocketSubsystem)
	{
		return false;
	}
	
	if (!ResolvedAddresses.Contains(OutServer.Ip))
	{
		TSharedPtr<FInternetAddr> ServerAddress = SocketSubsystem->CreateInternetAddr();
		if (!ServerAddress.IsValid())
		{
			return false;
		}

		bool bIsAddressValid = false;
		ServerAddress->SetIp(*OutServer.Ip, bIsAddressValid);
		if (!bIsAddressValid)
		{
			FAddressInfoResult AddressInfo = SocketSubsystem->GetAddressInfo(*OutServer.Ip // 'Ip' field is actually the domain name in AMS
				, nullptr
				, EAddressInfoFlags::Default
				, NAME_None);

			if (AddressInfo.Results.Num() < 1)
			{
				return false;
			}

			// Retrieve the IP string from the address result and return success
			FString ResolvedIp = AddressInfo.Results[0].Address->ToString(false);
			if (ResolvedIp.IsEmpty())
			{
				return false;
			}

			ServerAddress->SetIp(*ResolvedIp, bIsAddressValid);
			if (!bIsAddressValid)
			{
				return false;
			}
		}
		ServerAddress->SetPort(OutServer.Port);
		ResolvedAddresses.Emplace(OutServer.Ip, ServerAddress);
	}
	OutServer.ResolvedIp = ResolvedAddresses[OutServer.Ip]->ToString(false);
	
	return true;
}
} // Namespace Api
} // Namespace AccelByte
