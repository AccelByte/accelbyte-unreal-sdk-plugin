// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "IAccelBytePlatformWrapper.h"
#include "Models/AccelByteLobbyModels.h"

namespace AccelByte
{

DECLARE_MULTICAST_DELEGATE_ThreeParams(FAccelBytePlatformPresenceChanged, EAccelBytePlatformType, FString const&, EAvailability);
typedef FAccelBytePlatformPresenceChanged::FDelegate FAccelBytePlatformPresenceChangedDelegate;

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
	 * @return true if set platform wrapper successful.
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
	 * @return true if the removal is successful.
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