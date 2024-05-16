// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteQos.h"
#include "Icmp.h"
#include "Networking.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/Ping/AccelBytePing.h"

namespace AccelByte
{
namespace Api
{
FAccelByteModelsQosServerList Qos::QosServers = {};
TArray<TPair<FString, float>> Qos::Latencies = {};
FDelegateHandleAlias Qos::PollLatenciesHandle;
FDelegateHandleAlias Qos::PollServerLatenciesHandle;

Qos::Qos(Credentials& InCredentialsRef
	, Settings const& InSettingsRef
	, FAccelByteMessagingSystem& InMessagingSystemRef)
	: CredentialsRef{InCredentialsRef.AsShared()}
	, SettingsRef{InSettingsRef}
	, MessagingSystem{InMessagingSystemRef}
	, bValidityFlagPtr(MakeShared<bool>(true))
{
		// Credentials is possibly destroyed before we are so we can't remove
		// from the delegate in our destructor.  This weak pointer allows us to
		// know whether `this` is valid.
		TWeakPtr<bool> ValidityFlag = bValidityFlagPtr;
		CredentialsRef->OnLoginSuccess().AddLambda([ValidityFlag, this](const FOauth2Token& Response)
			{
				if (!ValidityFlag.IsValid())
					return;
				OnLoginSuccess(Response);
			});


	OnLobbyConnectedHandle = FOnMessagingSystemReceivedMessage::CreateRaw(this, &Qos::OnLobbyConnected);
	LobbyConnectedDelegateHandle = MessagingSystem.SubscribeToTopic(EAccelByteMessagingTopic::LobbyConnected, OnLobbyConnectedHandle);

	QosUpdateCheckerTickerDelegate = FTickerDelegate::CreateRaw(this, &Qos::CheckQosUpdate);
	QosUpdateCheckerHandle = FTickerAlias::GetCoreTicker().AddTicker(QosUpdateCheckerTickerDelegate, QosUpdateCheckerIntervalSecs);
}

Qos::~Qos()
{
	MessagingSystem.UnsubscribeFromTopic(EAccelByteMessagingTopic::QosRegionLatenciesUpdated, LobbyConnectedDelegateHandle);
	OnLobbyConnectedHandle.Unbind();

	// Indicate to the OnLoginSuccess lambda that we have been destroyed and `this` is no longer valid.
	bValidityFlagPtr.Reset();

	FTickerAlias::GetCoreTicker().RemoveTicker(QosUpdateCheckerHandle);
}

void Qos::OnLoginSuccess(const FOauth2Token& Response)
{
	GetServerLatencies(nullptr, nullptr);
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
	FRegistry::QosManager.GetActiveQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda(
	[this, OnSuccess, OnError](const FAccelByteModelsQosServerList Result)
	{
		FAccelByteModelsQosServerList ResolvedServerList{Result};

		for (FAccelByteModelsQosServer& Server : ResolvedServerList.Servers)
		{
			ResolveQosServerAddress(Server);
		}

		if(Result.Servers.Num() > 0)
		{
			PingRegionsSetLatencies(ResolvedServerList, OnSuccess, OnError);
		}
	}), OnError);
}

void Qos::CallGetQosServers(const bool bPingRegionsOnSuccess
	, const THandler<TArray<TPair<FString, float>>>& OnPingRegionsSuccess
	, const FErrorHandler& OnError)
{
	FRegistry::QosManager.GetActiveQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda(
		[this, bPingRegionsOnSuccess, OnPingRegionsSuccess, OnError](const FAccelByteModelsQosServerList Result)
		{
			Qos::QosServers = Result; // Cache for the session

			for (FAccelByteModelsQosServer& Server : Qos::QosServers.Servers)
			{
				ResolveQosServerAddress(Server);
			}

			if (bPingRegionsOnSuccess)
			{
				PingRegionsSetLatencies(Qos::QosServers, OnPingRegionsSuccess, OnError);
			} 
			else
			{
				OnPingRegionsSuccess.ExecuteIfBound(Qos::Latencies); // Latencies == Nullable until PingRegionsSetLatencies called once+.
			}
		}), OnError);
}

void Qos::PingRegionsSetLatencies(const FAccelByteModelsQosServerList& QosServerList
	, const THandler<TArray<TPair<FString, float>>>& OnSuccess
	, const FErrorHandler& OnError)
{
	TSharedRef<TArray<TPair<FString, float>>> SuccessLatencies = MakeShared<TArray<TPair<FString, float>>>();
	TSharedRef<TArray<FString>> FailedLatencies = MakeShared<TArray<FString>>();

	int32 Count = QosServerList.Servers.Num();
	
	if (Count > 0)
	{
		// For each server, ping them and record add to Latency TArray.
		for (int count = 0; count < QosServerList.Servers.Num(); count++)
		{
			auto Server = QosServerList.Servers[count];
			FString Region = Server.Region;

			// Ping -> Get the latencies on pong.
			FAccelBytePing::SendUdpPing(Server.ResolvedIp, Server.Port, FRegistry::Settings.QosPingTimeout, FPingCompleteDelegate::CreateLambda(
				[Count, SuccessLatencies, FailedLatencies, Region, OnSuccess, OnError, this](const FPingResult& PingResult)
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
						Qos::Latencies.Empty();
						Qos::Latencies.Append(*SuccessLatencies);

						if (SuccessLatencies->Num() > 0)
						{
							OnSuccess.ExecuteIfBound(*SuccessLatencies);
							bQosUpdated = true;
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
	Qos::PollLatenciesHandle = FTickerAlias::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(
		[this](float DeltaTime)
		{
			PingRegionsSetLatencies(Qos::QosServers, nullptr, nullptr);
			return true;
			
		}), AdjustedSecondsPerTick);
}

void Qos::InitGetServerLatenciesScheduler(float QosServerPollIntervalSecs)
{
	const bool bActivateScheduler = QosServerPollIntervalSecs > 0;
	
	if (Qos::PollServerLatenciesHandle.IsValid())
	{
		RemoveFromTicker(Qos::PollServerLatenciesHandle);
	}

	if (!bActivateScheduler)
	{
		return;
	}

	// Active (>0): ensure min value to prevent flooding
	const float AdjustedSecondsPerTick = QosServerPollIntervalSecs < Settings::MinNumSecsQosLatencyPolling
		? Settings::MinNumSecsQosLatencyPolling
		: QosServerPollIntervalSecs;

	// -----------------------------------
	// Schedule a Latencies refresh poller
	// Loop infinitely, every x seconds, until we tell the delegate to stop via RemoveFromTicker()
	Qos::PollServerLatenciesHandle = FTickerAlias::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda(
		[this](float DeltaTime)
		{
			constexpr bool bPingRegionsOnSuccess = false;
			CallGetQosServers(bPingRegionsOnSuccess, nullptr, nullptr);
			
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
	InitGetLatenciesScheduler(FRegistry::Settings.QosLatencyPollIntervalSecs);
	InitGetServerLatenciesScheduler(FRegistry::Settings.QosServerLatencyPollIntervalSecs);
}

void Qos::StopLatencyPollers()
{
	RemoveFromTicker(Qos::PollLatenciesHandle);
	RemoveFromTicker(Qos::PollServerLatenciesHandle);
	Qos::Latencies.Empty();
}

bool Qos::AreLatencyPollersActive()
{
	return PollLatenciesHandle.IsValid() || PollServerLatenciesHandle.IsValid();
}

const TArray<TPair<FString, float>>& Qos::GetCachedLatencies()
{
	return Qos::Latencies;
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

	MessagingSystem.SendMessage<FAccelByteModelsQosRegionLatencies>(EAccelByteMessagingTopic::QosRegionLatenciesUpdated, RegionLatencies);
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
	if (!SettingsRef.bServerUseAMS)
	{
		// Armada does not use domain names in place of raw IPs, so just route the resolved IP to the reported Qos IP
		OutServer.ResolvedIp = OutServer.Ip;
		return true;
	}

	ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM);
	if (!SocketSubsystem)
	{
		return false;
	}

	FAddressInfoResult AddressInfo = SocketSubsystem->GetAddressInfo(*OutServer.Ip // 'Ip' field is actually the domain name in AMS
		, nullptr
		, EAddressInfoFlags::Default
		, NAME_None);

	if (AddressInfo.Results.Num() < 1)
	{
		return false;
	}

	// Retrieve the IP string from the address result and return success
	OutServer.ResolvedIp = AddressInfo.Results[0].Address->ToString(false);
	return true;
}
} // Namespace Api
} // Namespace AccelByte
