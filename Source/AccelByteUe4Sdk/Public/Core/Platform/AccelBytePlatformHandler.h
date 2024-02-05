// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "CoreMinimal.h"
#include "Models/AccelByteLobbyModels.h"
#pragma once

namespace AccelByte
{
	class ACCELBYTEUE4SDK_API IAccelBytePlatformWrapper
	{
	public:
		virtual void FetchPlatformToken(TDelegate<void(FString)> Delegate) = 0;
	};

	class ACCELBYTEUE4SDK_API FAccelBytePlatformHandler
	{
	public:
		DECLARE_MULTICAST_DELEGATE_ThreeParams(FAccelBytePlatformPresenceChanged, EAccelBytePlatformType, const FString&, EAvailability);
		typedef FAccelBytePlatformPresenceChanged::FDelegate FAccelBytePlatformPresenceChangedDelegate;

		TSharedPtr<IAccelBytePlatformWrapper> GetPlatformWrapper(EAccelBytePlatformType PlatformType);

		void SetPlatformWrapper(EAccelBytePlatformType PlatformType, TSharedPtr<IAccelBytePlatformWrapper> Wrapper);

		bool RemovePlatformWrapper(EAccelBytePlatformType PlatformType);

		FDelegateHandle AddOnPlatformPresenceChangedDelegate(const FAccelBytePlatformPresenceChangedDelegate& InOnPlatformPresenceChanged);

		void TriggerOnPlatformPresenceChangedDelegate(EAccelBytePlatformType PlatformType, const FString& PlatformUserId, EAvailability Availability);

		bool RemoveOnPlatformPresenceChangedDelegate(FDelegateHandle DelegateHandle);

		void ClearOnPlatformPresenceChangedDelegate();
	private:
		static TMap<EAccelBytePlatformType, TSharedPtr<IAccelBytePlatformWrapper>> PlatformWrappers;
		static FAccelBytePlatformPresenceChanged OnPlatformPresenceChanged;
	};
}