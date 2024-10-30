// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Containers/Map.h"
#include "Misc/Timespan.h"

namespace AccelByte
{
	
enum class EWebsocketClosureCodeForSpecificRetry;
	
/*
* Reconnection behavios when the websocket connection has been lost
* It is optional and can be adjusted by the developer's need.
*/
enum class EReconnectionStrategyType
{
	// Default strategy. It tries to reconnect several times until the timeout has been reached.
	BALANCED = 0,

	// Recommended stragey. It tries to reconnect indefinitely until the websocket connection can be established again.
	LIMITLESS = 1,

	// Similar behavior with the BALANCED strategy, but it has a rapid interval & shorter timeout duration.
	AGGRESSIVE = 2
};

enum class EConfigurableWebsocketServiceType
{
	LOBBY_GAMECLIENT,
	CHAT_GAMECLIENT,
	DS_HUB_GAMESERVER,
	AMS_GAMESERVER
};

class ACCELBYTEUE4SDK_API FReconnectionStrategy
{
public:
	class ACCELBYTEUE4SDK_API FMassiveOutageDuration : public FTimespan {
	public:
		explicit FMassiveOutageDuration(FTimespan Input) :FTimespan(Input) {}
	};
	class ACCELBYTEUE4SDK_API FTotalTimeoutDuration : public FTimespan {
	public:
		explicit FTotalTimeoutDuration(FTimespan Input) :FTimespan(Input) {}
	};
	class ACCELBYTEUE4SDK_API FInitialBackoffDelay : public FTimespan {
	public:
		explicit FInitialBackoffDelay(FTimespan Input) :FTimespan(Input) {}
	};
	class ACCELBYTEUE4SDK_API FBalancedMaxRetryInterval : public FTimespan {
	public:
		explicit FBalancedMaxRetryInterval(FTimespan Input) :FTimespan(Input) {}
	};
	class ACCELBYTEUE4SDK_API FAggressiveRetryInterval : public FTimespan {
	public:
		explicit FAggressiveRetryInterval(FTimespan Input) :FTimespan(Input) {}
	};

#define DEFAULT_TOTAL_TIMEOUT_GIVE_UP_RETRY FTotalTimeoutDuration(FTimespan::FromMinutes(1))
#define DEFAULT_INITIAL_BACKOFF_DELAY FInitialBackoffDelay(FTimespan::FromSeconds(1.0f))
#define DEFAULT_AGGRESSIVE_RETRY_INTERVAL FAggressiveRetryInterval(FTimespan::FromSeconds(3.0f))
#define DEFAULT_BALANCED_MAX_RETRY_INTERVAL FBalancedMaxRetryInterval(FTimespan::FromSeconds(30.0f))

public:
	FMassiveOutageDuration GetMassiveOutageDuration() { return Advanced_MassiveOutageDuration; }
	float GetBaseFactorBackoff() { return Advanced_RetryBackoffBaseFactor; }

	/*
	 * Change the behavior of the backoff retry
	 * N-th Retry Interval = BASE_FACTOR ^ (N-1)
	 */
	void SetBackoffMechanism(float BASE_FACTOR)
	{
		Advanced_RetryBackoffBaseFactor = BASE_FACTOR;
	}

	/*
	 * A specified disconnected duration will be considered as a massive outage if exceeded.
	 */
	void SetMassiveOutageDuration(FTimespan NewMassiveOutageDuration)
	{
		Advanced_MassiveOutageDuration = FReconnectionStrategy::FMassiveOutageDuration(NewMassiveOutageDuration);
	}

	FReconnectionStrategy& operator=(const FReconnectionStrategy& Copy);

	static FReconnectionStrategy CreateBalancedStrategy(
		FBalancedMaxRetryInterval BalancedMaxRetryInterval = DEFAULT_BALANCED_MAX_RETRY_INTERVAL,
		FTotalTimeoutDuration TotalTimeoutToGiveUpRetry = DEFAULT_TOTAL_TIMEOUT_GIVE_UP_RETRY,
		FInitialBackoffDelay InitialBackoffDelay = DEFAULT_INITIAL_BACKOFF_DELAY)
	{
		auto ClampedRetryInterval = FMath::Clamp(
			BalancedMaxRetryInterval.GetTotalSeconds(),
			(double)CAP_MINIMUM_RETRY_INTERVAL_SECONDS,
			(double)CAP_LIMITLESS_RETRY_INTERVAL_SECONDS);

		FReconnectionStrategy Output(
			EReconnectionStrategyType::BALANCED,
			TotalTimeoutToGiveUpRetry,
			InitialBackoffDelay,
			DEFAULT_AGGRESSIVE_RETRY_INTERVAL, //No aggressive retry interval
			FBalancedMaxRetryInterval(FTimespan::FromSeconds(ClampedRetryInterval))
		);
		return Output;
	}

	static FReconnectionStrategy CreateAggressiveStrategy(
		FAggressiveRetryInterval AggressiveRetryInterval = DEFAULT_AGGRESSIVE_RETRY_INTERVAL,
		FTotalTimeoutDuration TotalTimeoutToGiveUpRetry = FTotalTimeoutDuration(FTimespan::FromSeconds(20.0f)),
		FInitialBackoffDelay InitialBackoffDelay = DEFAULT_INITIAL_BACKOFF_DELAY)
	{
		auto ClampedAggressiveRetryIntervalSeconds = FMath::Clamp(
			AggressiveRetryInterval.GetTotalSeconds(),
			(double)CAP_MINIMUM_RETRY_INTERVAL_SECONDS,
			(double)CAP_LIMITLESS_RETRY_INTERVAL_SECONDS);

		FReconnectionStrategy Output(
			EReconnectionStrategyType::AGGRESSIVE,
			TotalTimeoutToGiveUpRetry,
			InitialBackoffDelay,
			FAggressiveRetryInterval(FTimespan::FromSeconds(ClampedAggressiveRetryIntervalSeconds)),
			DEFAULT_BALANCED_MAX_RETRY_INTERVAL
		);
		return Output;
	}

	static FReconnectionStrategy CreateLimitlessStrategy(
		FInitialBackoffDelay InitialBackoffDelay = DEFAULT_INITIAL_BACKOFF_DELAY)
	{
		FReconnectionStrategy Output(
			EReconnectionStrategyType::LIMITLESS,
			FTotalTimeoutDuration(FTimespan::MaxValue()), //Unlimited, never expired
			InitialBackoffDelay,
			DEFAULT_AGGRESSIVE_RETRY_INTERVAL,
			DEFAULT_BALANCED_MAX_RETRY_INTERVAL
		);
		return Output;
	}

public: // Non-configurable & absolute public member
	const static int CAP_MINIMUM_RETRY_INTERVAL_SECONDS = 1; //Non-negotiable
	const static int CAP_LIMITLESS_RETRY_INTERVAL_SECONDS = 120; //To cap the value of the Limitless retry

private: //private value member
	EReconnectionStrategyType _StrategyType = EReconnectionStrategyType::BALANCED;
	FTotalTimeoutDuration _TotalTimeout = DEFAULT_TOTAL_TIMEOUT_GIVE_UP_RETRY;
	FInitialBackoffDelay _InitialBackoffDelay = DEFAULT_INITIAL_BACKOFF_DELAY;
	FAggressiveRetryInterval _AggressiveRetryInterval = DEFAULT_AGGRESSIVE_RETRY_INTERVAL;
	FBalancedMaxRetryInterval _BalancedMaxRetryInterval = DEFAULT_BALANCED_MAX_RETRY_INTERVAL;
public: //public face member
	const EReconnectionStrategyType& StrategyType;
	const FTotalTimeoutDuration& TotalTimeout;
	const FInitialBackoffDelay& InitialBackoffDelay;
	const FAggressiveRetryInterval& AggressiveRetryInterval;
	const FBalancedMaxRetryInterval& BalancedMaxRetryInterval;

private:
	FMassiveOutageDuration Advanced_MassiveOutageDuration = FMassiveOutageDuration(FTimespan::FromMinutes(5));
	float Advanced_RetryBackoffBaseFactor = 2.0f;

private: // Avoid direct constructor & the Create() is mandatory
	FReconnectionStrategy() = delete;
	FReconnectionStrategy(
		EReconnectionStrategyType InStrategyType,
		FTotalTimeoutDuration InTotalTimeoutToGiveUp,
		FInitialBackoffDelay InInitialBackoffDelay,
		FAggressiveRetryInterval InAggressiveRetryInterval,
		FBalancedMaxRetryInterval InBalancedMaxRetryInterval
	);
};

//Interface that need to be implemented by the child of Websocket adopter
class ACCELBYTEUE4SDK_API IWebsocketConfigurableReconnectStrategy
{
public:
	IWebsocketConfigurableReconnectStrategy() = default;
	~IWebsocketConfigurableReconnectStrategy() = default;

	/*
	 * Set reconnect strategy when the websocket connection is closed
	 */
	void SetDefaultReconnectionStrategy(const FReconnectionStrategy& Strategy);

	/*
	 * Get the default reconnection strategy
	 */
	FReconnectionStrategy& GetDefaultReconnectionStrategy();

	/*
	 * Get the reconnection strategy for specified code
	 */
	TMap<int32, FReconnectionStrategy>& GetReconnectionStrategyDictionary();

	/*
	 * Set reconnect strategy when specific connection close is received
	 */
	void SetSpecificReconnectionStrategy(EWebsocketClosureCodeForSpecificRetry WebsocketDisconnectionCode, const FReconnectionStrategy& Strategy);

	/*
	 * Set reconnect strategy when specific connection close is received
	 */
	void SetSpecificReconnectionStrategy(int32 RawWebsocketConnectionCode, const FReconnectionStrategy& Strategy);

private:
	// Default Reconnection Strategy
	FReconnectionStrategy DefaultReconnectionStrategy = FReconnectionStrategy::CreateBalancedStrategy();

	// Specific reconnection strategy for a specific code
	TMap<int32, FReconnectionStrategy> ReconnectionStrategyDictionary = TMap<int32, FReconnectionStrategy>();
};

}
