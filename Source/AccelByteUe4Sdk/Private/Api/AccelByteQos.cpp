// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteQos.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Core/AccelByteRegistry.h"

namespace AccelByte
{
namespace Api
{

	Qos::Qos(Credentials& NewCredentialsRef, const Settings& NewSettingsRef)
		: CredentialsRef{NewCredentialsRef}
		, SettingsRef{NewSettingsRef}
	{
		CredentialsRef.OnLoginSuccess().AddRaw(this, &Qos::OnLoginSuccess);
	}

	Qos::~Qos()
	{
	}
	
	void Qos::OnLoginSuccess(const FOauth2Token& Response)
	{
		GetServerLatencies();
	}

	void Qos::GetServerLatencies()
	{
		Qos::GetServerLatencies(THandler<TArray<TPair<FString, float>>>(), FErrorHandler());
	}

	void Qos::GetServerLatencies(
		const THandler<TArray<TPair<FString, float>>>& OnSuccess,
		const FErrorHandler& OnError)
	{
		// Should we schedule this to repeat?
		const bool bAlreadyPolling = PollLatenciesHandle.IsValid();
		const bool bPlanningToPoll =  SettingsRef.QosLatencyPollIntervalSecs > 0;
		
		if (bPlanningToPoll && !bAlreadyPolling)
		{
			// Setup polling schedulers that will consistently refresh Latencies.
			InitGetLatenciesScheduler(OnSuccess, OnError);
			InitGetServerLatenciesScheduler(OnSuccess, OnError);
		}
		else
		{
			// Call just once.
			constexpr bool bPingRegionsOnSuccess = true;
			CallGetQosServers(bPingRegionsOnSuccess, OnSuccess, OnError);
		}
	}

	void Qos::GetActiveServerLatencies(
		const THandler<TArray<TPair<FString, float>>>& OnSuccess,
		const FErrorHandler& OnError)
	{
		FRegistry::QosManager.GetActiveQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda(
		[this, OnSuccess](const FAccelByteModelsQosServerList Result)
		{
			if(Result.Servers.Num() > 0)
			{
				PingRegionsSetLatencies(Result, OnSuccess);
			}
		}), OnError);
	}

	void Qos::CallGetQosServers(
		const bool bPingRegionsOnSuccess,
		const THandler<TArray<TPair<FString, float>>>& OnPingRegionsSuccess,
		const FErrorHandler& OnError)
	{
		FRegistry::QosManager.GetQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda(
		[this, bPingRegionsOnSuccess, OnPingRegionsSuccess](const FAccelByteModelsQosServerList Result)
		{
			QosServers = Result; // Cache for the session

			if (bPingRegionsOnSuccess)
				PingRegionsSetLatencies(QosServers, OnPingRegionsSuccess);
			else
				OnPingRegionsSuccess.ExecuteIfBound(Latencies); // Latencies == Nullable until PingRegionsSetLatencies called once+.
			
		}), OnError);
	}
	
	void Qos::PingRegionsSetLatencies(
		const FAccelByteModelsQosServerList& QosServerList,
		const THandler<TArray<TPair<FString, float>>>& OnSuccess)
	{
		Latencies.Empty();
		
		// For each server, ping them and record add to Latency TArray.
		for (int i = 0; i < QosServerList.Servers.Num(); i++)
		{
			auto Server = QosServerList.Servers[i];
			int32 Count = QosServerList.Servers.Num();
			FString IpPort = FString::Printf(TEXT("%s:%d"), *Server.Ip, Server.Port);

			// Ping -> Get the latencies on pong.
			FUDPPing::UDPEcho(IpPort, 10.00, FIcmpEchoResultDelegate::CreateLambda(
				[this, Server, OnSuccess, Count](FIcmpEchoResult& PingResult)
				{
					// Add <Region, PingSeconds>
					Latencies.Add(TPair<FString, float>(Server.Region, PingResult.Time * 1000));
					if (Count == Latencies.Num())
						OnSuccess.ExecuteIfBound(Latencies);
				}));
		}
	}
	
	void Qos::InitGetLatenciesScheduler(
		const THandler<TArray<TPair<FString, float>>>& OnTick,
		const FErrorHandler& OnError)
	{
		const float SecondsPerTick = SettingsRef.QosServerLatencyPollIntervalSecs;
		const bool bActivateScheduler = SecondsPerTick > 0;
		
		if (!bActivateScheduler)
		{
			RemoveFromTicker(PollLatenciesHandle);
			return;
		}

		// Active (>0): ensure min value to prevent flooding
		const float AdjustedSecondsPerTick = SecondsPerTick < Settings::MinNumSecsQosLatencyPolling
			? Settings::MinNumSecsQosLatencyPolling
			: SecondsPerTick;

		// -----------------------------------
		// Schedule a Latencies refresh poller
		auto& Ticker = FTicker::GetCoreTicker();

		// Loop infinitely, every x seconds, until we tell the delegate to stop via RemoveFromTicker()
		PollLatenciesHandle = Ticker.AddTicker(FTickerDelegate::CreateLambda(
			[this, OnTick](float DeltaTime)
		{
			PingRegionsSetLatencies(QosServers, OnTick);
			return true;
				
		}), AdjustedSecondsPerTick);
	}
	
	void Qos::InitGetServerLatenciesScheduler(
		const THandler<TArray<TPair<FString, float>>>& OnTick,
		const FErrorHandler& OnError)
	{
		const float SecondsPerTick = SettingsRef.QosLatencyPollIntervalSecs;
		
		if (SecondsPerTick <= 0)
		{
			RemoveFromTicker(PollServerLatenciesHandle);
			return;
		}

		// Active (>0): ensure min value to prevent flooding
		const float AdjustedSecondsPerTick = SettingsRef.QosLatencyPollIntervalSecs < Settings::MinNumSecsQosLatencyPolling
			? Settings::MinNumSecsQosLatencyPolling
			: SecondsPerTick;

		// -----------------------------------
		// Schedule a Latencies refresh poller
		auto& Ticker = FTicker::GetCoreTicker();

		// Loop infinitely, every x seconds, until we tell the delegate to stop via RemoveFromTicker()
		PollServerLatenciesHandle = Ticker.AddTicker(FTickerDelegate::CreateLambda(
			[this, OnTick, OnError](float DeltaTime)
		{
			constexpr bool bPingRegionsOnSuccess = true;
			CallGetQosServers(bPingRegionsOnSuccess, OnTick, OnError);
				
			return true;
		}), AdjustedSecondsPerTick);
	}

	void Qos::RemoveFromTicker(FDelegateHandle& Handle)
	{
		if (!Handle.IsValid())
			return;
		
		FTicker::GetCoreTicker().RemoveTicker(Handle);
		Handle.Reset();
	}

	void Qos::RemoveLatencyPollers()
	{
		RemoveFromTicker(PollLatenciesHandle);
		RemoveFromTicker(PollServerLatenciesHandle);
	}
	
	const TArray<TPair<FString, float>>& Qos::GetCachedLatencies() const
	{
		return Latencies;
	}

	void Qos::Shutdown()
	{
		RemoveLatencyPollers();
	}
	
} // Namespace Api
} // Namespace AccelByte
