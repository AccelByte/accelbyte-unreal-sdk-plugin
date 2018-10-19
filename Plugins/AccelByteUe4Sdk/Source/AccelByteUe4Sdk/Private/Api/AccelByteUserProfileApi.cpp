// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserProfileApi.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteHttpRetrySystem.h"
#include "JsonUtilities.h"
#include "AccelByteCredentials.h"
#include "AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

void UserProfile::GetUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FGetUserProfileSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

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

void UserProfile::GetUserProfileEasy(FGetUserProfileSuccess OnSuccess, FErrorDelegate OnError)
{
	GetUserProfile(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), OnSuccess, OnError);
}

void UserProfile::UpdateUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, FUpdateUserProfileSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
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

void UserProfile::UpdateUserProfileEasy(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, FUpdateUserProfileSuccess OnSuccess, FErrorDelegate OnError)
{
	UpdateUserProfile(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), ProfileUpdateRequest, OnSuccess, OnError);
}

void UserProfile::CreateUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, FCreateUserProfileSuccess OnSuccess, FErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
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

void UserProfile::CreateUserProfileEasy(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, FCreateUserProfileSuccess OnSuccess, FErrorDelegate OnError)
{
	CreateUserProfile(Settings::ServerBaseUrl, Credentials::Get().GetUserAccessToken(), Credentials::Get().GetUserNamespace(), Credentials::Get().GetUserId(), ProfileCreateRequest, OnSuccess, OnError);
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void UserProfile::GetUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetUserProfileSuccess OnSuccess, FErrorDelegate OnError)
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

void UserProfile::UpdateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpdateUserProfileSuccess OnSuccess, FErrorDelegate OnError)
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

void UserProfile::CreateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateUserProfileSuccess OnSuccess, FErrorDelegate OnError)
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
