// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "HttpPackage.h"
#include "Math/RandomStream.h"

namespace AccelByte
{
// will only work on Editor / Development builds
class ACCELBYTEUE4SDK_API FAccelByteNetworkConditioner
{
public:
	/**
	* @brief Get enabled status of network conditioner.
	*
	* @return true if network conditioner is enabled.
	*/
	bool IsEnabled() const;

	/**
	* @brief Set enabled status of network conditioner.
	*
	* @param InEnabled Network conditioner enabled state.
	*/
	void SetEnabled(const bool InEnabled);

	/**
	* @brief Set overall fail rate percentage.
	* this will be used if a particular MessageType is not set.
	* 
	* @param FailRate Percentage fail rate to use, valid value is between 0 (always pass) to 100 (always fail)
	* @return true if overall fail rate set successfully.
	*/
	bool SetOverallFailRate(const int32 FailRate);

	/**
	* @brief Get overall fail rate percentage.
	* this will be used if a particular MessageType is not set.
	* 
	* @return overall fail rate value.
	*/
	int32 GetOverallFailRate() const;

	/**
	* @brief Set fail rate percentage of a particular message type
	* override the overall fail rate value for this message type.
	*
	* @param MessageType The message type name to override.
	* @param FailRate Percentage fail rate to use, valid value is between 0 (always pass) to 100 (always fail)
	* @return true if fail rate set successfully.
	*/
	bool SetMessageFailRate(const FString& MessageType, const int32 FailRate);

	/**
	* @brief Get fail rate percentage of a particular message type
	* override the overall fail rate value for this message type.
	*
	* @param MessageType The message type name.
	* @return Fail rate percentage value for that message type, return 0 if the message type is not set.
	*/
	int32 GetMessageFailRate(const FString& MessageType);

	/**
	* @brief Get map of fail rate percentage of all message type that is set
	*
	* @param OutFailRateMap map of fail rate percentage.
	*/
	void GetAllMessageFailRate(TMap<FString, int32>& OutFailRateMap) const;

	/**
	* @brief Clear all message fail rate that was set.
	*/
	void ClearMessageFailRate();

	/**
	* @brief Remove fail rate of a message type
	*
	* @param MessageType Name of message type to remove.
	* @return true if successfully removed a message type.
	*/
	bool RemoveMessageFailRate(const FString& MessageType);

	/**
	* @brief Set random seed to be used in calculation,
	* this will set new initial random seed.
	*
	* @param Seed random seed to use.
	*/
	void SetRandomSeed(const int32 Seed);

	/**
	* @brief Get current random seed used.
	*
	* @return random seed used.
	*/
	int32 GetCurrentRandomSeed() const;

	/**
	* @brief Get initial random seed used.
	*
	* @return initial random seed used.
	*/
	int32 GetInitialRandomSeed() const;
	
PACKAGE_SCOPE:
	FAccelByteNetworkConditioner();
	~FAccelByteNetworkConditioner();

	/**
	* @brief Calculate with random if a MessageType should be fail or pass.
	*
	* @param MessageType Name of message type.
	* @return true should fail, false is pass.
	*/
	bool CalculateFail(const FString& MessageType);

private:
	bool bEnabled{false};
	int32 OverallFailRate{0};
	TMap<FString, int32> MessageFailRates;
	FRandomStream RandStream;
};
}
