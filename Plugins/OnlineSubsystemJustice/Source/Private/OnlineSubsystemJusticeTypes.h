// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "OnlineJsonSerializer.h"
#include "IPAddress.h"

class FOnlineSubsystemJustice;

/** 
 * Implementation of session information
 */
class FOnlineSessionInfoJustice : public FOnlineSessionInfo
{
protected:
	
	/** Hidden on purpose */
	FOnlineSessionInfoJustice(const FOnlineSessionInfoJustice& Src)
	{
	}

	/** Hidden on purpose */
	FOnlineSessionInfoJustice& operator=(const FOnlineSessionInfoJustice& Src)
	{
		return *this;
	}

PACKAGE_SCOPE:

	/** Constructor */
	FOnlineSessionInfoJustice();

	/** 
	 * Initialize a Justice session info with the address of this machine
	 * and an id for the session
	 */
	void Init(const FOnlineSubsystemJustice& Subsystem);

	/** The ip & port that the host is listening on (valid for LAN/GameServer) */
	TSharedPtr<class FInternetAddr> HostAddr;
	/** Unique Id for this session */
	FUniqueNetIdString SessionId;

public:

	virtual ~FOnlineSessionInfoJustice() {}

 	bool operator==(const FOnlineSessionInfoJustice& Other) const
 	{
 		return false;
 	}

	virtual const uint8* GetBytes() const override
	{
		return NULL;
	}

	virtual int32 GetSize() const override
	{
		return sizeof(uint64) + sizeof(TSharedPtr<class FInternetAddr>);
	}

	virtual bool IsValid() const override
	{
		// LAN case
		return HostAddr.IsValid() && HostAddr->IsValid();
	}

	virtual FString ToString() const override
	{
		return SessionId.ToString();
	}

	virtual FString ToDebugString() const override
	{
		return FString::Printf(TEXT("HostIP: %s SessionId: %s"), 
			HostAddr.IsValid() ? *HostAddr->ToString(true) : TEXT("INVALID"), 
			*SessionId.ToDebugString());
	}

	virtual const FUniqueNetId& GetSessionId() const override
	{
		return SessionId;
	}
};


class FOAuthTokenJustice :
	public FOnlineJsonSerializable
{
	
public:
	
	bool ShouldRefresh() { return NextTokenRefreshUtc >= FDateTime::UtcNow(); };
	
	FString AccesToken;
	FString RefreshToken;
	FString TokenType;
	int32   ExpiresIn;
	
	/* todo: add these
	 TArray Roles;
	 TArray Permissions;
	 */
	
	FString UserId;
	FString DisplayName;
	FString Namespace;
	
	// Tracking when to refresh the token
	FDateTime LastTokenRefreshUtc;
	FDateTime NextTokenRefreshUtc;
	
	BEGIN_ONLINE_JSON_SERIALIZER
	ONLINE_JSON_SERIALIZE("access_token", AccesToken);
	ONLINE_JSON_SERIALIZE("refresh_token", RefreshToken);
	ONLINE_JSON_SERIALIZE("token_type", TokenType);
	ONLINE_JSON_SERIALIZE("expires_in", ExpiresIn);
	ONLINE_JSON_SERIALIZE("user_id", UserId);
	ONLINE_JSON_SERIALIZE("display_name", DisplayName);
	ONLINE_JSON_SERIALIZE("namespace", Namespace);
	END_ONLINE_JSON_SERIALIZER
};







