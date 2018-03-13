// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/CoreOnline.h"
#include "OAuthTokenJustice.h"

class FUserOnlineAccountJustice
{

public:
	FUserOnlineAccountJustice(const FString& LoginId = TEXT(""), int32 LocalUserNumber = 0)
		:LocalUserNum(LocalUserNumber), LoginId(LoginId){}
	virtual ~FUserOnlineAccountJustice(){}

	FString GetUserId() { return Token->UserId; }
	FString GetDisplayName(const FString& Platform = FString()) { return Token->DisplayName; }
	bool GetUserAttribute(const FString& AttrName, FString& OutAttrValue) const;
	bool SetUserAttribute(const FString& AttrName, const FString& AttrValue);
	FString GetAccessToken() { return Token->AccessToken; }
	bool GetAuthAttribute(const FString& AttrName, FString& OutAttrValue) const;
	FString GetLoginId() { return LoginId; }

	FString LoginId;
	//TSharedRef<const FUniqueNetId> UserIdPtr;
	TMap<FString, FString> AdditionalAuthData;
	TMap<FString, FString> UserAttributes;
	int32 LocalUserNum;
	UOAuthTokenJustice* Token;
};
