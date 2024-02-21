// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/Platform/AccelBytePlatformHandler.h"

namespace AccelByte
{
TMap <EAccelBytePlatformType, AccelBytePlatformWrapperPtr> FAccelBytePlatformHandler::PlatformWrappers = TMap<EAccelBytePlatformType, AccelBytePlatformWrapperPtr> {};
FAccelBytePlatformPresenceChanged FAccelBytePlatformHandler::OnPlatformPresenceChanged;

AccelBytePlatformWrapperWPtr FAccelBytePlatformHandler::GetPlatformWrapper(EAccelBytePlatformType PlatformType)
{
	if (auto PlatformWrapper = PlatformWrappers.Find(PlatformType))
	{
		return *PlatformWrapper;
	}
	return nullptr;
}

bool FAccelBytePlatformHandler::SetPlatformWrapper(EAccelBytePlatformType PlatformType, AccelBytePlatformWrapperPtr const& WrapperPtr)
{
	if (!WrapperPtr.IsValid())
	{
		return false;
	}

	PlatformWrappers.Add(PlatformType, WrapperPtr);
	return true;
}

bool FAccelBytePlatformHandler::RemovePlatformWrapper(EAccelBytePlatformType PlatformType)
{
	return (PlatformWrappers.Remove(PlatformType) > 0) ? true : false;
}

FDelegateHandle FAccelBytePlatformHandler::AddPlatformPresenceChangedDelegate(FAccelBytePlatformPresenceChangedDelegate const& InOnPlatformPresenceChanged)
{
	return OnPlatformPresenceChanged.Add(InOnPlatformPresenceChanged);
}

void FAccelBytePlatformHandler::TriggerOnPlatformPresenceChangedDelegate(EAccelBytePlatformType PlatformType, const FString& PlatformUserId, EAvailability Availability)
{
	if (OnPlatformPresenceChanged.IsBound())
	{
		OnPlatformPresenceChanged.Broadcast(PlatformType, PlatformUserId, Availability);
	}
}

bool FAccelBytePlatformHandler::RemovePlatformPresenceChangedDelegate(FDelegateHandle DelegateHandle)
{
	return OnPlatformPresenceChanged.Remove(DelegateHandle);
}

void FAccelBytePlatformHandler::ClearPlatformPresenceChangedDelegates()
{
	OnPlatformPresenceChanged.Clear();
}
}