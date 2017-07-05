// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemImpl.h"
#include "OnlineSubsystemJusticePackage.h"
#include "HAL/ThreadSafeCounter.h"
#include "UObject/CoreOnline.h"
#include "JusticeHTTP.h"


class FJusticeAccessToken
{
	
public:
	FString AccessToken;
	FString RefreshToken;
	FString TokenType;
	int32 ExpiresIn;
};


/**
 * Justice IAM Client
 */
class FJusticeIamClient
{

public:
	
	FJusticeIamClient();
	
	~FJusticeIamClient()
	{
	}

	void ClientCredentialsTokenGrant();
	void SetBaseURL(const FString URL)   { BaseURL = URL; };

	
private:

	FString BaseURL;
	FString ClientId, ClientSecret;
	FJusticeAccessToken ClientToken;

	void ClientCredentialsTokenGrantComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded);
 
};
