// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteApiUserProfile.h"
#include "AccelByteApiIdentity.h"
#include "AccelByteHttpRetrySystem.h"
#include "JsonUtilities.h"

namespace AccelByte
{
namespace Api
{

void UserProfile::GetUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FGetUserProfileSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		GetUserProfileResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void UserProfile::UpdateUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsUserProfileInfoUpdate& NewUserProfile, FUpdateUserProfileSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(NewUserProfile, Content);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		UpdateUserProfileResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void UserProfile::CreateDefaultUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, FString DisplayName, FCreateDefaultUserProfileSuccess OnSuccess, ErrorDelegate OnError)
{
	FString DefaultLocale;
	#if PLATFORM_IOS
	DefaultLocale =  FIOSPlatformMisc::GetDefaultLocale();
	#elif PLATFORM_ANDROID
	DefaultLocale =  FAndroidMisc::GetDefaultLocale();
	#else
	DefaultLocale = FGenericPlatformMisc::GetDefaultLocale();
	#endif
	TArray<FString> RegionId;
	DefaultLocale.ParseIntoArray(RegionId, TEXT("_"), true);

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{\"displayName\": \"%s\", \"country\": \"%s\"}"), *DisplayName, *RegionId[1]);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		CreateDefaultUserProfileResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

void UserProfile::CreateCompleteUserProfile(FString ServerBaseUrl, FString AccessToken, FString Namespace, FString UserId, const FAccelByteModelsUserProfileRequest& ProfileRequest, FCreateCompleteUserProfileSuccess OnSuccess, ErrorDelegate OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *AccessToken);
	FString Url = FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/profiles"), *ServerBaseUrl, *Namespace, *UserId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(ProfileRequest, Content);

	FHttpRequestPtr Request = RetrySystem.Manager.CreateRequest(RetrySystem.RetryLimitCount, RetrySystem.RetryTimeoutRelativeSeconds, RetrySystem.RetryResponseCodes);
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	Request->OnProcessRequestComplete().BindLambda([OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		CreateCompleteUserProfileResponse(Request, Response, OnSuccess, OnError);
	});
	Request->ProcessRequest();
}

// =============================================================================================================================
// ========================================================= Responses =========================================================
// =============================================================================================================================

void UserProfile::GetUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FGetUserProfileSuccess OnSuccess, ErrorDelegate OnError)
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
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void UserProfile::UpdateUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FUpdateUserProfileSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnSuccess.ExecuteIfBound();
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void UserProfile::CreateDefaultUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateDefaultUserProfileSuccess OnSuccess, ErrorDelegate OnError)
{
	int32 Code;
	FString Message;
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		FAccelByteModelsUserProfileInfo Result;
		FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0 ,0);
		OnSuccess.ExecuteIfBound(Result);
		return;
	}
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

void UserProfile::CreateCompleteUserProfileResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FCreateCompleteUserProfileSuccess OnSuccess, ErrorDelegate OnError)
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
	else
	{
		HandleHttpError(Request, Response, Code, Message);
	}
	OnError.ExecuteIfBound(Code, Message);
}

} // Namespace Api
} // Namespace AccelByte
