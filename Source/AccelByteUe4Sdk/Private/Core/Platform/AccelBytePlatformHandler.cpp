// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/Platform/AccelBytePlatformHandler.h"

namespace AccelByte
{

TMap <EAccelBytePlatformType, AccelBytePlatformWrapperPtr> FAccelBytePlatformHandler::PlatformWrappers = TMap<EAccelBytePlatformType, AccelBytePlatformWrapperPtr> {};
FAccelBytePlatformPresenceChanged FAccelBytePlatformHandler::OnPlatformPresenceChanged;
FRWLock FAccelBytePlatformHandler::WrapperLock;
FCriticalSection FAccelBytePlatformHandler::DelegateLock;

AccelBytePlatformWrapperWPtr FAccelBytePlatformHandler::GetPlatformWrapper(EAccelBytePlatformType PlatformType)
{
	FReadScopeLock ReadLock(WrapperLock);
	if (const auto& PlatformWrapper = PlatformWrappers.Find(PlatformType))
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

	FWriteScopeLock WriteLock(WrapperLock);
	PlatformWrappers.Add(PlatformType, WrapperPtr);
	return true;
}

bool FAccelBytePlatformHandler::RemovePlatformWrapper(EAccelBytePlatformType PlatformType)
{
	FWriteScopeLock WriteLock(WrapperLock);
	return (PlatformWrappers.Remove(PlatformType) > 0) ? true : false;
}

FDelegateHandle FAccelBytePlatformHandler::AddPlatformPresenceChangedDelegate(FAccelBytePlatformPresenceChangedDelegate const& InOnPlatformPresenceChanged)
{
	FScopeLock Lock(&DelegateLock);
	return OnPlatformPresenceChanged.Add(InOnPlatformPresenceChanged);
}

void FAccelBytePlatformHandler::TriggerOnPlatformPresenceChangedDelegate(EAccelBytePlatformType PlatformType, const FString& PlatformUserId, EAvailability Availability)
{
	// Snapshot the delegate under the lock, then broadcast outside the lock.
	// To avoid Re-entrancy deadlock: callbacks that call Add/Remove/Clear will try to
	// acquire DelegateLock, which must already be released before Broadcast.
	FAccelBytePlatformPresenceChanged LocalCopy;
	{
		FScopeLock Lock(&DelegateLock);
		LocalCopy = OnPlatformPresenceChanged;
	}
	if(LocalCopy.IsBound())
	{
		LocalCopy.Broadcast(PlatformType, PlatformUserId, Availability);
	}
}

bool FAccelBytePlatformHandler::RemovePlatformPresenceChangedDelegate(FDelegateHandle DelegateHandle)
{
	FScopeLock Lock(&DelegateLock);
	return OnPlatformPresenceChanged.Remove(DelegateHandle);
}

void FAccelBytePlatformHandler::ClearPlatformPresenceChangedDelegates()
{
	FScopeLock Lock(&DelegateLock);
	OnPlatformPresenceChanged.Clear();
}

}
