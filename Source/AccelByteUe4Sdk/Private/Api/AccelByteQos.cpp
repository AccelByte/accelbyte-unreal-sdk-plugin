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
static constexpr float AccelByteMaxPing = 9999.0f;

FRWLock Qos::QosServersMtx;
FRWLock Qos::LatenciesMtx;
FRWLock Qos::ResolvedAddressesMtx;
FRWLock Qos::PollLatenciesHandleMtx;
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
		LobbyConnectedDelegateHandle.Reset();
	}
	OnLobbyConnectedHandle.Unbind();

	FTickerAlias::GetCoreTicker().RemoveTicker(QosUpdateCheckerHandle);
	QosUpdateCheckerHandle.Reset();
	QosUpdateCheckerTickerDelegate.Unbind();
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
				{
					FWriteScopeLock WriteLock(Qos::QosServersMtx);
					Qos::QosServers = Result; // Cache for the session
					for (FAccelByteModelsQosServer& Server : Qos::QosServers.Servers)
					{
						QosApi->ResolveQosServerAddress(Server);
					}
				}
				if (bPingRegionsOnSuccess)
				{
					FReadScopeLock ReadLock(Qos::QosServersMtx);
					QosApi->PingRegionsSetLatencies(Qos::QosServers, OnPingRegionsSuccess, OnError);
				}
				else
				{
					FReadScopeLock ReadLock(Qos::LatenciesMtx);
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

	TSharedPtr<FRWLock, ESPMode::ThreadSafe> PingResultMtx = MakeShared<FRWLock, ESPMode::ThreadSafe>();
	TSharedPtr<TArray<TPair<FString, float>>, ESPMode::ThreadSafe> PingResult = MakeShared<TArray<TPair<FString, float>>, ESPMode::ThreadSafe>();
	TArray<FAccelByteModelsQosServer> Servers = QosServerList.Servers;
	
	Servers.RemoveAllSwap([this](const FAccelByteModelsQosServer& Server) 
		{
			bool ResolvedAddrPresent = false;
			{
				FReadScopeLock Lock(ResolvedAddressesMtx);
				ResolvedAddrPresent = Qos::ResolvedAddresses.Contains(Server.Ip);
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
					if(PingResult->Num() == ServersCount)
					{
						// When all server pinged
						{
							FWriteScopeLock WriteLock(Qos::LatenciesMtx);
							Qos::Latencies = *PingResult;
						}
						// If the lowest ping == AccelByteMaxPing, that means all ping attempts failed
						PingResult->Sort([](const auto& A, const auto& B){
							return A.Value > B.Value;
						});
						if (PingResult->Num() > 0 && PingResult->Last().Value != AccelByteMaxPing)
						{
							OnSuccess.ExecuteIfBound(*PingResult);
							auto Qos = QosWeak.Pin();
							if(Qos.IsValid())
							{
								Qos->bQosUpdated.store(true, std::memory_order_release);
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
				FReadScopeLock ReadLock(Qos::QosServersMtx);
				QosApi->PingRegionsSetLatencies(Qos::QosServers, nullptr, nullptr);
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
	FWriteScopeLock Lock(Qos::LatenciesMtx);
	Qos::Latencies.Empty();
}

bool Qos::AreLatencyPollersActive() const noexcept
{
	FReadScopeLock Lock(PollLatenciesHandleMtx);
	return PollLatenciesHandle.IsValid();
}

const TArray<TPair<FString, float>>& Qos::GetCachedLatencies()
{
	FReadScopeLock Lock(Qos::LatenciesMtx);
	return Qos::Latencies;
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
	{
		FReadScopeLock ReadLock(Qos::LatenciesMtx);
		if (Qos::Latencies.Num() <= 0)
		{
			return;
		}
		for (const auto& Latency : Qos::Latencies)
		{
			FAccelByteModelsQosRegionLatency RegionLatency;
			RegionLatency.Region = Latency.Key;
			RegionLatency.Latency = Latency.Value;

			RegionLatencies.Data.Add(RegionLatency);
		}
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
	
	{
		FWriteScopeLock WriteLock(ResolvedAddressesMtx);
		if (!Qos::ResolvedAddresses.Contains(OutServer.Ip))
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
			Qos::ResolvedAddresses.Emplace(OutServer.Ip, ServerAddress);
		}
	}
	FReadScopeLock ReadLock(ResolvedAddressesMtx);
	OutServer.ResolvedIp = Qos::ResolvedAddresses[OutServer.Ip]->ToString(false);
	
	return true;
}
} // Namespace Api
} // Namespace AccelByte
