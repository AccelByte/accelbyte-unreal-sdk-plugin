// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "CoreMinimal.h"
#include "Models/AccelByteLobbyModels.h"
#pragma once

namespace AccelByte
{

DECLARE_MULTICAST_DELEGATE_ThreeParams(FAccelBytePlatformPresenceChanged, EAccelBytePlatformType, FString const&, EAvailability);
typedef FAccelBytePlatformPresenceChanged::FDelegate FAccelBytePlatformPresenceChangedDelegate;

/**
 * @brief Platform Wrapper Interface to implement specific functionalities of given Native Platform.
 */
class ACCELBYTEUE4SDK_API IAccelBytePlatformWrapper
{
public:
	/**
	 * @brief Retrieve App Id of the game on specific Native Platform.
	 * 
	 * @return AppId of the game.
	 */
	virtual FString GetAppId() = 0;

	/**
	 * @brief Retrieve User Id that was playing the game on the Native Platform.
	 * 
	 * @param ExtraAttributes Key-Value pairs to determine which player info that were trying to retrieve e.g. LocalUserIndex. (optional)
	 * @return Specific Platform User Id.
	 */
	virtual FString GetPlatformUserId(TMap<FString, FString> const& ExtraAttributes = {}) = 0;

	/**
	 * @brief Fetch Platform Token or Auth Code on specific Native Platform.
	 * 
	 * @param Delegate A callback delegate that will be triggered to return the Platform Token value whether success or not.
	 * @param ExtraAttributes Key-Value pairs for additional information needed to get the Platform Token e.g. service identity in Steam. (optional)
	 */
	virtual void FetchPlatformToken(TDelegate<void(FString const&)> Delegate, TMap<FString, FString> const& ExtraAttributes = {}) = 0;
};

typedef TSharedPtr<IAccelBytePlatformWrapper, ESPMode::ThreadSafe> AccelBytePlatformWrapperPtr;
typedef TWeakPtr<IAccelBytePlatformWrapper, ESPMode::ThreadSafe> AccelBytePlatformWrapperWPtr;

class ACCELBYTEUE4SDK_API FAccelBytePlatformHandler
{
public:
	/**
	 * @brief Retrieve stored Platform Wrapper from a specified Native Platform.
	 * 
	 * @param PlatformType Specified Native Platform Type.
	 * @return WeakPtr to the Platform Wrapper object that was stored inside the Platform Handler.
	 */
	AccelBytePlatformWrapperWPtr GetPlatformWrapper(EAccelBytePlatformType PlatformType);

	/**
	 * @brief Store Platform Wrapper for a specific Native Platform.
	 * 
	 * @param PlatformType Specified Native Platform Type.
	 * @param WrapperPtr SharedPtr to the Platform Wrapper object.
	 * @return Boolean status whether or not the operation is successful.
	 */
	bool SetPlatformWrapper(EAccelBytePlatformType PlatformType, AccelBytePlatformWrapperPtr const& WrapperPtr);

	/**
	 * @brief Remove stored Platform Wrapper for a specific Native Platform.
	 * 
	 * @param PlatformType Specified Native Platform Type.
	 */
	bool RemovePlatformWrapper(EAccelBytePlatformType PlatformType);

	/**
	 * @brief Add delegate to listen at Presence changed notification on Native Platform.
	 * 
	 * @param InPlatformPresenceChanged Presence changed delegate.
	 * @return DelegateHandle for given Presence changed delegate.
	 */
	FDelegateHandle AddPlatformPresenceChangedDelegate(FAccelBytePlatformPresenceChangedDelegate const& InPlatformPresenceChanged);

	/**
	 * @brief Trigger stored Presence changed delegate.
	 *
	 * @param PlatformType Specified Native Platform Type.
	 * @param PlatformUserId Specified User Id.
	 * @param Availability Availability status changed of the given User Id.
	 */
	void TriggerOnPlatformPresenceChangedDelegate(EAccelBytePlatformType PlatformType, FString const& PlatformUserId, EAvailability Availability);

	/**
	 * @brief Remove registered Platform Presence changed delegate.
	 * 
	 * @param DelegateHandle DelegateHandle for given Presence changed delegate.
	 * @return Boolean status whether or not the removal is successful.
	 */
	bool RemovePlatformPresenceChangedDelegate(FDelegateHandle DelegateHandle);

	/**
	 * @brief Clear all registered Platform Presence changed delegate.
	 */
	void ClearPlatformPresenceChangedDelegates();

private:

	static TMap<EAccelBytePlatformType, AccelBytePlatformWrapperPtr> PlatformWrappers;
	static FAccelBytePlatformPresenceChanged OnPlatformPresenceChanged;
};
}