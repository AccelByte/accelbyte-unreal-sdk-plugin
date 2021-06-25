// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteUserProfileApi.h"
#include "Api/AccelByteOauth2Api.h"
#include "JsonUtilities.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{
UserProfile::UserProfile(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting){}

UserProfile::~UserProfile(){}

void UserProfile::GetUserProfile(const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles"), *Settings.BasicServerUrl, *Settings.Namespace);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GetPublicUserProfileInfo(FString UserID, const THandler<FAccelByteModelsPublicUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/public"), *Settings.BasicServerUrl, *Settings.Namespace, *UserID);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::BatchGetPublicUserProfileInfos(const FString UserIds, const THandler<TArray<FAccelByteModelsPublicUserProfileInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public"), *Settings.BasicServerUrl, *Credentials.GetNamespace());
	const FString UrlEncodedUserIds = FGenericPlatformHttp::UrlEncode(UserIds);
	const FString QueryString = FString::Printf(TEXT("userIds=%s"), *UrlEncodedUserIds);
	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	const FString ContentType = TEXT("application/json");
	const FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(FString::Printf(TEXT("%s?%s"), *Url, *QueryString));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GetCustomAttributes(const THandler<FJsonObject>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/customAttributes"), *Settings.BasicServerUrl, *Settings.Namespace, *Credentials.GetUserId());
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles"), *Settings.BasicServerUrl, *Settings.Namespace);
	FString Verb			= TEXT("PUT");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
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

void UserProfile::UpdateCustomAttributes(const FJsonObject& CustomAttributesUpdateRequest, const THandler<FJsonObject>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/customAttributes"), *Settings.BasicServerUrl, *Settings.Namespace, *Credentials.GetUserId());
	FString Verb			= TEXT("PUT");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content;
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(CustomAttributesUpdateRequest);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

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
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles"), *Settings.BasicServerUrl, *Settings.Namespace);
	FString Verb			= TEXT("POST");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content;
	TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ProfileCreateRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

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
