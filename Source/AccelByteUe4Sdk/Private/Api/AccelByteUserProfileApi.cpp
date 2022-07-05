// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteUserProfileApi.h"
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

UserProfile::UserProfile(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{
}

UserProfile::~UserProfile(){}

void UserProfile::GetUserProfile(const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace());
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GetPublicUserProfileInfo(FString UserID, const THandler<FAccelByteModelsPublicUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/public"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace(), *UserID);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::BatchGetPublicUserProfileInfos(const FString UserIds, const THandler<TArray<FAccelByteModelsPublicUserProfileInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace());
	const FString UrlEncodedUserIds = FGenericPlatformHttp::UrlEncode(UserIds);
	const FString QueryString = FString::Printf(TEXT("userIds=%s"), *UrlEncodedUserIds);
	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	const FString ContentType = TEXT("application/json");
	const FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(FString::Printf(TEXT("%s?%s"), *Url, *QueryString));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GetCustomAttributes(const THandler<FJsonObject>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/customAttributes"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GetPublicCustomAttributes(const FString& UserId, const THandler<FJsonObject>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/customAttributes"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace(), *UserId);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace());
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

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::UpdateCustomAttributes(const FJsonObject& CustomAttributesUpdateRequest, const THandler<FJsonObject>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/customAttributes"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
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

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::CreateUserProfile(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace());
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

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GetUserProfilePublicInfoByPublicId(const FString& PublicId, const THandler<FAccelByteModelsPublicUserProfileInfo>& OnSuccess, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public/byPublicId"), *SettingsRef.BasicServerUrl, *SettingsRef.Namespace);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	Url.Append(FString::Printf(TEXT("?publicId=%s"), *PublicId));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::CreateUserProfile(const FString& UserId, FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace(), *UserId);
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

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void UserProfile::UpdateUserProfile(const FString& UserId, const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace(), *UserId);
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

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GetUserProfile(const FString& UserId, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace(), *UserId);
	FString Verb			= TEXT("GET");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GenerateUploadURL(const FString& Folder, EAccelByteFileType FileType, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/folders/%s/files"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace(), *Folder);
	FString Verb			= TEXT("POST");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content			= TEXT("");
	
	FString QueryParam = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("fileType"), FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower()},
	});
	Url.Append(QueryParam);
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UserProfile::GenerateUploadURLForUserContent(const FString& UserId, EAccelByteFileType FileType, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess, FErrorHandler const& OnError,
	EAccelByteUploadCategory Category)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization	= FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url				= FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/files"), *SettingsRef.BasicServerUrl, *CredentialsRef.GetNamespace(), *UserId);
	FString Verb			= TEXT("POST");
	FString ContentType		= TEXT("application/json");
	FString Accept			= TEXT("application/json");
	FString Content			= TEXT("");
	
	FString QueryParam = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("fileType"), FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower()},
		{ TEXT("category"), FAccelByteUtilities::GetUEnumValueAsString(Category).ToLower()}, 
	});
	Url.Append(QueryParam); 
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
  
} // Namespace Api
} // Namespace AccelByte
