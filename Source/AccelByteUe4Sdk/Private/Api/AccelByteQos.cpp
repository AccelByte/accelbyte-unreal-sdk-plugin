// Copyright (c) 2020-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteQos.h"
#include "Icmp.h"
#include "Networking.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Core/AccelByteRegistry.h"

namespace AccelByte
{
namespace Api
{
	FAccelByteModelsQosServerList Qos::QosServers = {};
	TArray<TPair<FString, float>> Qos::Latencies = {};
	FDelegateHandleAlias Qos::PollLatenciesHandle;
	FDelegateHandleAlias Qos::PollServerLatenciesHandle;

	Qos::Qos(Credentials& InCredentialsRef
		, Settings const& InSettingsRef)
		: CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
	{
		CredentialsRef.OnLoginSuccess().AddRaw(this, &Qos::OnLoginSuccess);
	}

	Qos::~Qos()
	{
	}
	
	void Qos::OnLoginSuccess(const FOauth2Token& Response)
	{
		GetServerLatencies(THandler<TArray<TPair<FString, float>>>(), FErrorHandler());
	}

	void Qos::GetServerLatencies(
		const THandler<TArray<TPair<FString, float>>>& OnSuccess,
		const FErrorHandler& OnError)
	{
		StartLatencyPollers();

		bool bPingRegionsOnSuccess = true;
		CallGetQosServers(bPingRegionsOnSuccess, OnSuccess, OnError);
	}

	void Qos::GetActiveServerLatencies(
		const THandler<TArray<TPair<FString, float>>>& OnSuccess,
		const FErrorHandler& OnError)
	{
		FRegistry::QosManager.GetActiveQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda(
		[this, OnSuccess, OnError](const FAccelByteModelsQosServerList Result)
		{
			if(Result.Servers.Num() > 0)
			{
				PingRegionsSetLatencies(Result, OnSuccess, OnError);
			}
		}), OnError);
	}

	void Qos::CallGetQosServers(
		const bool bPingRegionsOnSuccess,
		const THandler<TArray<TPair<FString, float>>>& OnPingRegionsSuccess,
		const FErrorHandler& OnError)
	{
		FRegistry::QosManager.GetQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda(
			[this, bPingRegionsOnSuccess, OnPingRegionsSuccess, OnError](const FAccelByteModelsQosServerList Result)
			{
				Qos::QosServers = Result; // Cache for the session

				if (bPingRegionsOnSuccess)
				{
					PingRegionsSetLatencies(Qos::QosServers, OnPingRegionsSuccess, OnError);
				} 
				else
				{
					OnPingRegionsSuccess.ExecuteIfBound(Qos::Latencies); // Latencies == Nullable until PingRegionsSetLatecnies called once+.
				}
			}), OnError);
	}
	
	void Qos::PingRegionsSetLatencies(
		const FAccelByteModelsQosServerList& QosServerList,
		const THandler<TArray<TPair<FString, float>>>& OnSuccess,
		const FErrorHandler& OnError)
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
				FString IpPort = FString::Printf(TEXT("%s:%d"), *Server.Ip, Server.Port);
				FString Region = Server.Region;

				// Ping -> Get the latencies on pong.
				FUDPPing::UDPEcho(IpPort, FRegistry::Settings.QosPingTimeout, FIcmpEchoResultDelegate::CreateLambda(
					[Count, SuccessLatencies, FailedLatencies, Region, OnSuccess, OnError](FIcmpEchoResult& PingResult)
					{
						// Add <Region, PingSeconds>
						if (PingResult.Status == EIcmpResponseStatus::Success)
						{
							float PingDelay = PingResult.Time * 1000;
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
				PingRegionsSetLatencies(Qos::QosServers, nullptr,  nullptr);
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
	
} // Namespace Api
} // Namespace AccelByte
