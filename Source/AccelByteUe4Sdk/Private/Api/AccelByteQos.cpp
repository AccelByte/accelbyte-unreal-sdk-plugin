// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteQos.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteInstance.h"
#include "Icmp.h"
#include "Networking.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Core/Ping/AccelBytePing.h"

namespace AccelByte
{
namespace Api
{
static constexpr float AccelByteMaxPing = 9999.0f;

FRWLock Qos::PollLatenciesHandleMtx;
FDelegateHandleAlias Qos::PollLatenciesHandle;

Qos::Qos(Credentials& InCredentialsRef
	, const Settings& InSettingsRef
	, FAccelByteMessagingSystem& InMessagingSystemRef
	, const QosManagerWPtr InQosManagerWeak
	, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
	: CredentialsRef{InCredentialsRef.AsShared()}
	, SettingsRef{InSettingsRef}
	, QosManagerWeak{InQosManagerWeak}
#if ENGINE_MAJOR_VERSION < 5
	, MessagingSystemWPtr{InMessagingSystemRef.AsShared()}
#else
	, MessagingSystemWPtr{InMessagingSystemRef.AsWeak()}
#endif
{
	if (InApiClient.IsValid())
	{
		ExtractPlatformInfo(InApiClient->GetAccelByteInstance().Pin());
	}
}


Qos::Qos(Credentials& InCredentialsRef
	, const Settings& InSettingsRef
	, FAccelByteMessagingSystem& InMessagingSystemRef
	, const QosManagerWPtr InQosManagerWeak
	, FAccelBytePlatformPtr const& InAccelBytePlatform)
	: CredentialsRef{ InCredentialsRef.AsShared() }
	, SettingsRef{ InSettingsRef }
	, QosManagerWeak{ InQosManagerWeak }
#if ENGINE_MAJOR_VERSION < 5
	, MessagingSystemWPtr{ InMessagingSystemRef.AsShared() }
#else
	, MessagingSystemWPtr{ InMessagingSystemRef.AsWeak() }
#endif
	, AccelBytePlatformPtr(InAccelBytePlatform)
{
}

Qos::~Qos()
{
	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
	if (MessagingSystemPtr.IsValid())
	{
		MessagingSystemPtr->UnsubscribeFromTopic(EAccelByteMessagingTopic::LobbyConnected, LobbyConnectedDelegateHandle);
		LobbyConnectedDelegateHandle.Reset();
	}
	OnLobbyConnectedHandle.Unbind();

	FTickerAlias::GetCoreTicker().RemoveTicker(QosUpdateCheckerHandle);
	QosUpdateCheckerHandle.Reset();
	QosUpdateCheckerTickerDelegate.Unbind();
}

void Qos::SetApiClient(TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> InApiClient)
{
	if (InApiClient.IsValid())
	{
		ExtractPlatformInfo(InApiClient->GetAccelByteInstance().Pin());
	}
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
				QosApi->PingRegionsSetLatencies(ResolvedServerList.Servers, OnSuccess, OnError);
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
		[QosWeak, bPingRegionsOnSuccess, OnPingRegionsSuccess, OnError](FAccelByteModelsQosServerList Result)
		{
			const auto QosApi = QosWeak.Pin();
			if (QosApi.IsValid())
			{
				for (FAccelByteModelsQosServer& Server : Result.Servers)
				{
					QosApi->ResolveQosServerAddress(Server);
				}

				if (QosApi->AccelBytePlatformPtr.IsValid())
				{
					QosApi->AccelBytePlatformPtr->SetQosServers(Result);

					if (bPingRegionsOnSuccess)
					{
						auto QosServers = QosApi->AccelBytePlatformPtr->GetQosServers();
						QosApi->PingRegionsSetLatencies(QosServers, OnPingRegionsSuccess, OnError);
					}
					else
					{
						auto Latencies = QosApi->AccelBytePlatformPtr->GetLatencies();
						OnPingRegionsSuccess.ExecuteIfBound(Latencies); // Latencies == Nullable until PingRegionsSetLatencies called once+.
					}
				}
			}
		}), OnError);
}

void Qos::PingRegionsSetLatencies(const TArray<FAccelByteModelsQosServer>& QosServers
	, const THandler<TArray<TPair<FString, float>>>& OnSuccess
	, const FErrorHandler& OnError)
{
#ifdef ACCELBYTE_ACTIVATE_PROFILER
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelByteQosPing"));
#endif

	TSharedPtr<FRWLock, ESPMode::ThreadSafe> PingResultMtx = MakeShared<FRWLock, ESPMode::ThreadSafe>();
	TSharedPtr<TArray<TPair<FString, float>>, ESPMode::ThreadSafe> PingResult = MakeShared<TArray<TPair<FString, float>>, ESPMode::ThreadSafe>();
	TArray<FAccelByteModelsQosServer> Servers = QosServers;
	
	Servers.RemoveAllSwap([this](const FAccelByteModelsQosServer& Server) 
		{
			bool ResolvedAddrPresent = false;
			if (AccelBytePlatformPtr.IsValid())
			{
				ResolvedAddrPresent = AccelBytePlatformPtr->IsAddressResolved(Server.Ip);
			}
			return !ResolvedAddrPresent || Server.ResolvedIp.IsEmpty();
		});
	const int32 ServersCount = Servers.Num();

	if (ServersCount <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to ping because no QoS server"));
	}
	else
	{
		// For each server, ping them and record add to Latency TArray.
		for (const auto& Server : Servers)
		{
			// Ping -> Get the latencies on pong.
			// Must be launched from the main thread
			QosWPtr QosWeak = AsShared();
			FAccelBytePing::SendUdpPing(Server.ResolvedIp, Server.Port, SettingsRef.QosPingTimeout, FPingCompleteDelegate::CreateLambda(
				[ServersCount, PingResult, PingResultMtx, Region = Server.Region, QosWeak, OnSuccess, OnError](const FPingResult& Result)
				{
					{
						FWriteScopeLock Lock(*PingResultMtx);
						// Mutex here
						if (Result.Status == FPingResultStatus::Success)
						{
							float PingDelay = Result.AverageRoundTrip * 1000; // convert to milliseconds
							PingResult->Emplace(TPair<FString, float>(Region, PingDelay));
						}
						else
						{
							PingResult->Emplace(TPair<FString, float>(Region, AccelByteMaxPing));
						}
					}

					auto QosApi = QosWeak.Pin();
					if (QosApi.IsValid() && PingResult->Num() == ServersCount)
					{
						if (QosApi->AccelBytePlatformPtr.IsValid())
						{
							// If the lowest ping == AccelByteMaxPing, that means all ping attempts failed
							PingResult->Sort([](const auto& A, const auto& B) {
									return A.Value > B.Value;
								});

							// When all server pinged
							QosApi->AccelBytePlatformPtr->SetLatencies(*PingResult);

							if (PingResult->Num() > 0 && PingResult->Last().Value != AccelByteMaxPing)
							{
								OnSuccess.ExecuteIfBound(*PingResult);
								QosApi->bQosUpdated.store(true, std::memory_order_release);
							}
							else
							{
								OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to ping all servers"));
							}
						}
						else
						{
							OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to ping due to invalid AccelBytePlatform"));
						}
					}
				}));
		}
	}
}

void Qos::InitGetLatenciesScheduler(float LatencyPollIntervalSecs)
{
	const bool bActivateScheduler = LatencyPollIntervalSecs > 0;

	// Always try to clean up poller first
	RemovePollerFromCoreTicker();
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
	FWriteScopeLock Lock(PollLatenciesHandleMtx);
	Qos::PollLatenciesHandle = FTickerAlias::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(
		[QosWeak](float DeltaTime)
		{
			const auto QosApi = QosWeak.Pin();

			if (QosApi.IsValid())
			{
				if (QosApi->AccelBytePlatformPtr.IsValid())
				{
					auto QosServers = QosApi->AccelBytePlatformPtr->GetQosServers();
					QosApi->PingRegionsSetLatencies(QosServers, nullptr, nullptr);
				}
			}
			return true;
			
		}), AdjustedSecondsPerTick);
}

void Qos::RemovePollerFromCoreTicker()
{
	// Put lock here, we cannot have dangling tickers
	FWriteScopeLock Lock(PollLatenciesHandleMtx);
	FTickerAlias::GetCoreTicker().RemoveTicker(Qos::PollLatenciesHandle);
	Qos::PollLatenciesHandle.Reset();
}

void Qos::StartLatencyPollers()
{
	// Setup polling schedulers that will consistently refresh Latencies.
	InitGetLatenciesScheduler(SettingsRef.QosLatencyPollIntervalSecs);
}

void Qos::StopLatencyPollers()
{
	RemovePollerFromCoreTicker();
	if (AccelBytePlatformPtr.IsValid())
	{
		AccelBytePlatformPtr->ClearLatencies();
	}
}

bool Qos::AreLatencyPollersActive() const noexcept
{
	FReadScopeLock Lock(PollLatenciesHandleMtx);
	return PollLatenciesHandle.IsValid();
}

TArray<TPair<FString, float>> Qos::GetCachedLatencies()
{
	TArray<TPair<FString, float>> Latencies;
	if (AccelBytePlatformPtr.IsValid())
	{
		Latencies = AccelBytePlatformPtr->GetLatencies();
	}
	return Latencies;
}

void Qos::Startup()
{
	FScopeLock Lock(&StartupMtx);
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
	FAccelByteModelsQosRegionLatencies RegionLatencies;
	auto Latencies = GetCachedLatencies();
	if (Latencies.Num() <= 0)
	{
		return;
	}
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
	if (bQosUpdated.load(std::memory_order_acquire))
	{
		SendQosLatenciesMessage();
		bQosUpdated.store(false, std::memory_order_release);
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

	if (AccelBytePlatformPtr.IsValid())
	{
		if (!AccelBytePlatformPtr->IsAddressResolved(OutServer.Ip))
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
			AccelBytePlatformPtr->SetResolvedAddress(OutServer.Ip, ServerAddress);
		}

		OutServer.ResolvedIp = AccelBytePlatformPtr->GetResolvedAddress(OutServer.Ip)->ToString(false);
		return true;
	}
	
	return false;
}

void Qos::ExtractPlatformInfo(TSharedPtr<FAccelByteInstance, ESPMode::ThreadSafe> const& InAccelByteInstance)
{
	if (InAccelByteInstance.IsValid())
	{
		AccelBytePlatformPtr = InAccelByteInstance->GetAccelBytePlatform();
	}
}
} // Namespace Api
} // Namespace AccelByte
