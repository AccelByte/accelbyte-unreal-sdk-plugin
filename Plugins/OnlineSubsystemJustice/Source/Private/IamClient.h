// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemImpl.h"
#include "OnlineSubsystemJusticePackage.h"
#include "HAL/ThreadSafeCounter.h"

#include "UObject/CoreOnline.h"
#include "Runtime/Online/HTTP/Public/Http.h"

/**
 * Justice IAM Client
 */
class FIamClient
{


public:
    
    FIamClient()
    : ClientAccessToken(nullptr),
      ApiUrl(nullptr)
    {
    }

    ~FIamClient()
    {
    }

    void ClientLogin(const FString& IamApiUrl, const FString& InClientId, const FString& InSecret);

    
private:
    
    FString ClientAccessToken;
    FString ApiUrl;

    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
    void ClientCredentialsTokenGrant(const FString& InClientId, const FString& InSecret);
    

    
};
