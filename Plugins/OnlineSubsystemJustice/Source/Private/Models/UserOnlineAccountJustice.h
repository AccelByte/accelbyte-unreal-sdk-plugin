// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "OnlineSubsystemTypes.h"
#include "OAuthTokenJustice.h"

class FUserOnlineAccountJustice :
	public FUserOnlineAccount
{

public:

	// FOnlineUser

	virtual TSharedRef<const FUniqueNetId> GetUserId() const override { return UserIdPtr; }
	virtual FString GetUserIdStr() const { return UserIdPtr->ToString(); }
	virtual FString GetRealName() const override { return TEXT("DummyRealName"); }
	virtual FString GetDisplayName(const FString& Platform = FString()) const override { return TEXT("DummyDisplayName"); }
	virtual bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const override;
	virtual bool SetUserAttribute(const FString& AttrName, const FString& AttrValue) override;

	// FUserOnlineAccount

	virtual FString GetAccessToken() const override { return TEXT("DummyAuthTicket"); }
	virtual bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const override;

	// FUserOnlineAccountJustice
	FUserOnlineAccountJustice(const FString& LoginId = TEXT(""), int32 LocalUserNumber = 0)
		:LocalUserNum(LocalUserNumber), LoginId(LoginId), UserIdPtr(new FUniqueNetIdString(TEXT("")))
	{
	}

	virtual ~FUserOnlineAccountJustice()
	{
	}

	void SetUserId(FString InUserId) { UserIdPtr = TSharedRef<const FUniqueNetIdString>(new FUniqueNetIdString(InUserId)); }
	FString GetLoginId() { return LoginId; }

	FString LoginId;

	/** User Id represented as a FUniqueNetId */
	TSharedRef<const FUniqueNetId> UserIdPtr;

	/** Additional key/value pair data related to auth */
	TMap<FString, FString> AdditionalAuthData;
	/** Additional key/value pair data related to user attribution */
	TMap<FString, FString> UserAttributes;

	// save local user num
	int32 LocalUserNum;

	/** Credentials */
	UOAuthTokenJustice* Token;
};
