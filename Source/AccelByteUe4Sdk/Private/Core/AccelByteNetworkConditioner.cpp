// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteNetworkConditioner.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
	bool FAccelByteNetworkConditioner::IsEnabled() const
	{
		return bEnabled;
	}

	void FAccelByteNetworkConditioner::SetEnabled(const bool InEnabled)
	{
		bEnabled = InEnabled;
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] %s with overall fail rate %d%%"), bEnabled? TEXT("ENABLED") : TEXT("DISABLED"), OverallFailRate);
	}

	bool FAccelByteNetworkConditioner::SetOverallFailRate(const int32 FailRate)
	{
		if(FailRate < 0 || FailRate > 100)
		{
			UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Setting overall fail rate with invalid value %d, value must be between 0-100"), FailRate);

			return false;
		}

		OverallFailRate = FailRate;
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Overall Percentage Failure Rate Set: %d%%"), OverallFailRate);
		return true;
	}

	int32 FAccelByteNetworkConditioner::GetOverallFailRate() const
	{
		return OverallFailRate;
	}

	bool FAccelByteNetworkConditioner::SetMessageFailRate(const FString& MessageType, const int32 FailRate)
	{
		if(FailRate < 0 || FailRate > 100)
		{
			UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Setting message fail rate with invalid value %d, value must be between 0-100"), FailRate);
			return false;
		}

		MessageFailRates.Emplace(MessageType, FailRate);
		
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] %s Percentage Failure Rate Set: %d%%"), *MessageType, FailRate);
		
		return true;
	}

	int32 FAccelByteNetworkConditioner::GetMessageFailRate(const FString& MessageType)
	{
		if(!MessageFailRates.Contains(MessageType))
		{
			UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Get message fail rate with message type %s not found"), *MessageType);
			
			return 0;
		}

		return MessageFailRates[MessageType];
	}

	void FAccelByteNetworkConditioner::GetAllMessageFailRate(TMap<FString, int32>& OutFailRateMap) const
	{
		OutFailRateMap = MessageFailRates;
	}

	void FAccelByteNetworkConditioner::ClearMessageFailRate()
	{
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] All Notification Percentage Failure Rates Cleared"));
		
		MessageFailRates.Empty();
	}

	bool FAccelByteNetworkConditioner::RemoveMessageFailRate(const FString& MessageType)
	{
		if(!MessageFailRates.Contains(MessageType))
		{
			UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] failed to Remove message fail rate with message type %s not found"), *MessageType);

			return false;
		}

		MessageFailRates.Remove(MessageType);
		
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] %s Percentage Failure Rate Cleared"), *MessageType);

		return true;
	}

	bool FAccelByteNetworkConditioner::CalculateFail(const FString& MessageType)
	{
#if !UE_EDITOR && (!UE_BUILD_DEVELOPMENT && !UE_BUILD_DEBUG)
		return false;
#else
		if(!bEnabled)
		{
			return false;
		}

		int32 FailRate = OverallFailRate;

		if(MessageFailRates.Contains(MessageType))
		{
			FailRate = MessageFailRates[MessageType];
		}

		const int32 RandomVal = RandStream.RandRange(1, 100);

		const bool bShouldFail = FailRate >= RandomVal;

		if(bShouldFail)
		{
			UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Killed message %s"), *MessageType);
		}
		
		return bShouldFail;
#endif
	}

	void FAccelByteNetworkConditioner::SetRandomSeed(const int32 Seed)
	{
		RandStream = FRandomStream(Seed);
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Setting random seed as %d"), RandStream.GetCurrentSeed());
	}

	int32 FAccelByteNetworkConditioner::GetCurrentRandomSeed() const
	{
		return RandStream.GetCurrentSeed();
	}

	int32 FAccelByteNetworkConditioner::GetInitialRandomSeed() const
	{
		return RandStream.GetInitialSeed();
	}

	FAccelByteNetworkConditioner::FAccelByteNetworkConditioner()
	{
		RandStream.GenerateNewSeed();
		UE_LOG(LogAccelByte, Log, TEXT("[AccelByteNetworkConditioner] Initialized AccelByte network conditioner with seed %d"), RandStream.GetCurrentSeed());
	}

	FAccelByteNetworkConditioner::~FAccelByteNetworkConditioner(){}
}
