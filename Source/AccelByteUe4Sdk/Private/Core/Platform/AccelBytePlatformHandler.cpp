// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/Platform/AccelBytePlatformHandler.h"

TMap <EAccelBytePlatformType,TSharedPtr<AccelByte::IAccelBytePlatformWrapper>> AccelByte::FAccelBytePlatformHandler::PlatformWrappers = TMap < EAccelBytePlatformType, TSharedPtr<AccelByte::IAccelBytePlatformWrapper>>{};
AccelByte::FAccelBytePlatformHandler::FAccelBytePlatformPresenceChanged AccelByte::FAccelBytePlatformHandler::OnPlatformPresenceChanged;

TSharedPtr<AccelByte::IAccelBytePlatformWrapper> AccelByte::FAccelBytePlatformHandler::GetPlatformWrapper(EAccelBytePlatformType PlatformType)
{
	if (auto PlatformWrapper = PlatformWrappers.Find(PlatformType))
	{
		return *PlatformWrapper;
	}
	return nullptr;
}

void AccelByte::FAccelBytePlatformHandler::SetPlatformWrapper(EAccelBytePlatformType PlatformType, TSharedPtr<IAccelBytePlatformWrapper> Wrapper)
{
	PlatformWrappers.Add(PlatformType, Wrapper);
}

bool AccelByte::FAccelBytePlatformHandler::RemovePlatformWrapper(EAccelBytePlatformType PlatformType)
{
	return (PlatformWrappers.Remove(PlatformType) > 0) ? true : false;
}

FDelegateHandle AccelByte::FAccelBytePlatformHandler::AddOnPlatformPresenceChangedDelegate(const FAccelBytePlatformPresenceChangedDelegate& InOnPlatformPresenceChanged)
{
	return OnPlatformPresenceChanged.Add(InOnPlatformPresenceChanged);
}

void AccelByte::FAccelBytePlatformHandler::TriggerOnPlatformPresenceChangedDelegate(EAccelBytePlatformType PlatformType, const FString& PlatformUserId, EAvailability Availability)
{
	if (OnPlatformPresenceChanged.IsBound())
	{
		OnPlatformPresenceChanged.Broadcast(PlatformType, PlatformUserId, Availability);
	}
}

bool AccelByte::FAccelBytePlatformHandler::RemoveOnPlatformPresenceChangedDelegate(FDelegateHandle DelegateHandle)
{
	return OnPlatformPresenceChanged.Remove(DelegateHandle);
}

void AccelByte::FAccelBytePlatformHandler::ClearOnPlatformPresenceChangedDelegate()
{
	OnPlatformPresenceChanged.Clear();
}
