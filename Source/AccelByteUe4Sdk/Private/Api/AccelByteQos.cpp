// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteQos.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"

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
		CredentialsRef.OnLoginSuccess().RemoveAll(this);
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
			InitGetLatenciesScheduler(SettingsRef.QosLatencyPollIntervalSecs, OnSuccess, OnError);
			InitGetServerLatenciesScheduler(SettingsRef.QosServerLatencyPollIntervalSecs, OnSuccess, OnError);
		}
		else
		{
			// Call just once.
			constexpr bool bPingRegionsOnSuccess = true;
			CallGetQosServers(bPingRegionsOnSuccess, OnSuccess, OnError);
		}
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
					{
						OnSuccess.ExecuteIfBound(Latencies);
					}
				}));
		}
	}
	
	void Qos::InitGetLatenciesScheduler(
		const float& SecondsPerTick,
		const THandler<TArray<TPair<FString, float>>>& OnTick,
		const FErrorHandler& OnError)
	{
		#pragma region PREFERED_DS_REGION TODO (?)
		// // @todo This example was found @ LobbyTest.cpp: Should we check for preferredDSRegion?
		// // Ensure we have regions set to get latencies from
		// const FString PreferredDSRegion = Environment::GetEnvironmentVariable(
		// 	"PREFERED_DS_REGION", 1000); // @todo "PREFERED" should be spelled "PREFERRED" (don't change the typo, for now).

		// // @todo If we check for the preferred region, do we abandon the scheduler completely?
		// if (PreferredDSRegion.IsEmpty())
		// {
		// 	OnError.ExecuteIfBound(0, "!PREFERED_DS_REGION (Env Var)");
		// 	return;
		// }
		#pragma endregion /PREFERED_DS_REGION TODO (?)
		
		if (SecondsPerTick <= 0)
		{
			RemoveFromTicker(PollLatenciesHandle);
			return;
		}

		// -----------------------------------
		// Schedule a Latencies refresh poller
		auto& Ticker = FTicker::GetCoreTicker();

		// Loop infinitely, every x seconds, until we tell the delegate to stop via RemoveFromTicker()
		PollLatenciesHandle = Ticker.AddTicker(FTickerDelegate::CreateLambda(
			[this, OnTick](float DeltaTime)
		{
			PingRegionsSetLatencies(QosServers, OnTick);
			return true;
				
		}), SecondsPerTick);
	}
	
	void Qos::InitGetServerLatenciesScheduler(
		const float& SecondsPerTick,
		const THandler<TArray<TPair<FString, float>>>& OnTick,
		const FErrorHandler& OnError)
	{
		#pragma region PREFERED_DS_REGION TODO (?)
		// // @todo This example was found @ LobbyTest.cpp: Should we check for preferredDSRegion?
		// // Ensure we have regions set to get latencies from
		// const FString PreferredDSRegion = Environment::GetEnvironmentVariable(
		// 	"PREFERED_DS_REGION", 1000); // @todo "PREFERED" should be spelled "PREFERRED" (don't change the typo, for now).

		// // @todo If we check for the preferred region, do we abandon the scheduler completely?
		// if (PreferredDSRegion.IsEmpty())
		// {
		// 	OnError.ExecuteIfBound(0, "!PREFERED_DS_REGION (Env Var)");
		// 	return;
		// }
		#pragma endregion /PREFERED_DS_REGION TODO (?)
		
		if (SecondsPerTick <= 0)
		{
			RemoveFromTicker(PollServerLatenciesHandle);
			return;
		}

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
		}), SecondsPerTick);
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
