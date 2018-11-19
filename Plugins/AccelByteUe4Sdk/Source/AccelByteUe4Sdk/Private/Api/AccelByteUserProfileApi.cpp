// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserProfileApi.h"
#include "AccelByteOauth2Api.h"
#include "JsonUtilities.h"
#include "AccelByteCredentials.h"
#include "AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

void UserProfile::GetUserProfile(const FGetUserProfileSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles"), *Settings::PlatformServerUrl, *Credentials::Get().GetUserNamespace(), *Credentials::Get().GetUserId());
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
	Request->OnProcessRequestComplete().BindStatic(GetUserProfileResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserProfile::UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const FUpdateUserProfileSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles"), *Settings::PlatformServerUrl, *Credentials::Get().GetUserNamespace(), *Credentials::Get().GetUserId());
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
	Request->OnProcessRequestComplete().BindStatic(UpdateUserProfileResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

void UserProfile::CreateUserProfile(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const FCreateUserProfileSuccess& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials::Get().GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles"), *Settings::PlatformServerUrl, *Credentials::Get().GetUserNamespace(), *Credentials::Get().GetUserId());
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
	Request->OnProcessRequestComplete().BindStatic(CreateUserProfileResponse, OnSuccess, OnError);
	Request->ProcessRequest();
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void UserProfile::GetUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserProfileSuccess OnSuccess, FErrorHandler OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsUserProfileInfo Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserProfile::UpdateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpdateUserProfileSuccess OnSuccess, FErrorHandler OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

void UserProfile::CreateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateUserProfileSuccess OnSuccess, FErrorHandler OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsUserProfileInfo Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	HandleHttpError(Request, Response, Code, Message);
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Api
} // Namespace AccelByte
