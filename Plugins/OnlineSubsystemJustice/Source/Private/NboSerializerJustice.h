// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemJusticeTypes.h"
#include "NboSerializer.h"

/**
 * Serializes data in network byte order form into a buffer
 */
class FNboSerializeToBufferJustice : public FNboSerializeToBuffer
{
public:
	/** Default constructor zeros num bytes*/
	FNboSerializeToBufferJustice() :
		FNboSerializeToBuffer(512)
	{
	}

	/** Constructor specifying the size to use */
	FNboSerializeToBufferJustice(uint32 Size) :
		FNboSerializeToBuffer(Size)
	{
	}

	/**
	 * Adds Justice session info to the buffer
	 */
 	friend inline FNboSerializeToBufferJustice& operator<<(FNboSerializeToBufferJustice& Ar, const FOnlineSessionInfoJustice& SessionInfo)
 	{
		check(SessionInfo.HostAddr.IsValid());
		// Skip SessionType (assigned at creation)
		Ar << SessionInfo.SessionId;
		Ar << *SessionInfo.HostAddr;
		return Ar;
 	}

	/**
	 * Adds Justice Unique Id to the buffer
	 */
	friend inline FNboSerializeToBufferJustice& operator<<(FNboSerializeToBufferJustice& Ar, const FUniqueNetIdString& UniqueId)
	{
		Ar << UniqueId.UniqueNetIdStr;
		return Ar;
	}
};

/**
 * Class used to write data into packets for sending via system link
 */
class FNboSerializeFromBufferJustice : public FNboSerializeFromBuffer
{
public:
	/**
	 * Initializes the buffer, size, and zeros the read offset
	 */
	FNboSerializeFromBufferJustice(uint8* Packet,int32 Length) :
		FNboSerializeFromBuffer(Packet,Length)
	{
	}

	/**
	 * Reads Justice session info from the buffer
	 */
 	friend inline FNboSerializeFromBufferJustice& operator>>(FNboSerializeFromBufferJustice& Ar, FOnlineSessionInfoJustice& SessionInfo)
 	{
		check(SessionInfo.HostAddr.IsValid());
		// Skip SessionType (assigned at creation)
		Ar >> SessionInfo.SessionId; 
		Ar >> *SessionInfo.HostAddr;
		return Ar;
 	}

	/**
	 * Reads Justice Unique Id from the buffer
	 */
	friend inline FNboSerializeFromBufferJustice& operator>>(FNboSerializeFromBufferJustice& Ar, FUniqueNetIdString& UniqueId)
	{
		Ar >> UniqueId.UniqueNetIdStr;
		return Ar;
	}
};
