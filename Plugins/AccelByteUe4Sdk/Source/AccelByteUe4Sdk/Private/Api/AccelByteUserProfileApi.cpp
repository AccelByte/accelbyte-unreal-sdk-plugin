// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserProfileApi.h"
#include "AccelByteOauth2Api.h"
#include "JsonUtilities.h"
#include "AccelByteRegistry.h"
#include "AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
namespace Api
{

void UserProfile::GetUserProfile(const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/profiles"), *FRegistry::Settings.BasicServerUrl, *FRegistry::Credentials.GetUserNamespace());
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GetPublicUserProfileInfo(FString UserID, const THandler<FAccelByteModelsPublicUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
    FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
    FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/public"), *FRegistry::Settings.BasicServerUrl, *FRegistry::Credentials.GetUserNamespace(), *UserID);
    FString Verb = TEXT("GET");
    FString ContentType = TEXT("application/json");
    FString Accept = TEXT("application/json");
    FString Content;

    FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
    Request->SetURL(Url);
    Request->SetHeader(TEXT("Authorization"), Authorization);
    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), ContentType);
    Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}


void UserProfile::UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/profiles"), *FRegistry::Settings.BasicServerUrl, *FRegistry::Credentials.GetUserNamespace());
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ProfileUpdateRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::CreateUserProfile(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/me/profiles"), *FRegistry::Settings.BasicServerUrl, *FRegistry::Credentials.GetUserNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ProfileCreateRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
