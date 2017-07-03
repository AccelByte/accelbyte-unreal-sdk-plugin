// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#include "IamClient.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "OnlineSubsystemJusticeModule.h"


void FIamClient::ClientLogin(const FString& IamApiUrl, const FString& InClientId, const FString& InSecret)
{
    ClientCredentialsTokenGrant(InClientId, InSecret);
}

void FIamClient::ClientCredentialsTokenGrant(const FString& InClientId, const FString& InSecret)
{

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

    Request->OnProcessRequestComplete().BindLambda(
                                                   [](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
    {
//        UE_LOG_ONLINE(Warning, TEXT("FIamClient::ClientCredentialsTokenGrant(): %s"), *Response->GetContentAsString());
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            FString token = JsonObject->GetStringField("access_token");
            UE_LOG_ONLINE(Warning, TEXT("FIamClient::ClientCredentialsTokenGrant(): Access Token=%s"), *token);
        }
    });

    Request->SetURL("https://api.alpha.accelbyte.net/iam/oauth/token");
    Request->SetVerb("POST");
    FString auth = FBase64::Encode("c60af7f2ce0f40b59ff32c737253c695:testtest");
    Request->SetHeader("Authorization", "Basic " + auth);
    Request->SetHeader("Content-Type", TEXT("application/x-www-form-urlencoded"));
    Request->SetHeader("Accept", TEXT("application/json"));
    Request->SetContentAsString("grant_type=client_credentials");
    Request->ProcessRequest();
    UE_LOG_ONLINE(Display, TEXT("FIamClient::ClientCredentialsTokenGrant()"));
    
}
