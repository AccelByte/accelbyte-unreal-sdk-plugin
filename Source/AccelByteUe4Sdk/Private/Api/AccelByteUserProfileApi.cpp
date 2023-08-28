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
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
}

UserProfile::~UserProfile(){}

void UserProfile::GetUserProfile(const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UserProfile::GetPublicUserProfileInfo(FString UserID
	, const THandler<FAccelByteModelsPublicUserProfileInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsAccelByteIDValid(*UserID))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, User Id format is invalid"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/public")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserID);

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("GET"), Url, Headers, OnSuccess, OnError);
}

void UserProfile::BatchGetPublicUserProfileInfos(const FString& UserIds
	, const THandler<TArray<FAccelByteModelsPublicUserProfileInfo>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__)
		, TEXT("The API might be removed without notice, please use BulkGetPublicUserProfileInfos instead!!"));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("userIds"), UserIds}
	};

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("GET"), Url, QueryParams, Headers, OnSuccess, OnError);
}

void UserProfile::BulkGetPublicUserProfileInfos(const TArray<FString>& UserIds
	, const THandler<TArray<FAccelByteModelsPublicUserProfileInfo>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("userIds"), FString::Join(UserIds, TEXT(","))}
	};

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("GET"), Url, QueryParams, Headers, OnSuccess, OnError);
}

void UserProfile::GetCustomAttributes(const THandler<FJsonObject>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/customAttributes")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UserProfile::GetPublicCustomAttributes(const FString& UserId
	, const THandler<FJsonObject>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This will no longer able to use since will give security hole issue for other player/user, "
	 "please use GetPublicUserProfileInfo instead");

	OnError.ExecuteIfBound(static_cast<int>(ErrorCodes::Deprecated),
		TEXT("This will no longer able to use since will give security hole issue for other player/user, use GetPublicUserProfileInfo instead"));

}

void UserProfile::UpdateUserProfile(const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest
	, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, ProfileUpdateRequest, OnSuccess, OnError, true);
}

void UserProfile::UpdateCustomAttributes(const FJsonObject& CustomAttributesUpdateRequest
	, const THandler<FJsonObject>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/customAttributes")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	FString Content;
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(CustomAttributesUpdateRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void UserProfile::CreateUserProfile(const FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest
	, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace());

	FString Content;
	const TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ProfileCreateRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void UserProfile::GetUserProfilePublicInfoByPublicId(const FString& PublicId
	, const THandler<FAccelByteModelsPublicUserProfileInfo>& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public/byPublicId")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("publicId"), *PublicId}
	};

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("GET"), Url, QueryParams, Headers, OnSuccess, OnError);
}

void UserProfile::CreateUserProfile(const FString& UserId
	, FAccelByteModelsUserProfileCreateRequest& ProfileCreateRequest
	, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsAccelByteIDValid(*UserId))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, User Id format is invalid"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	FString Content;
	const TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ProfileCreateRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}
	
void UserProfile::UpdateUserProfile(const FString& UserId
	, const FAccelByteModelsUserProfileUpdateRequest& ProfileUpdateRequest
	, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsAccelByteIDValid(*UserId))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, User Id format is invalid"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, ProfileUpdateRequest, OnSuccess, OnError, true);
}

void UserProfile::GetUserProfile(const FString& UserId
	, const THandler<FAccelByteModelsUserProfileInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsAccelByteIDValid(*UserId))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, User Id format is invalid"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UserProfile::GenerateUploadURL(const FString& Folder
	, EAccelByteFileType FileType
	, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/folders/%s/files")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace()
		, *Folder);

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("fileType"), FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower()}
	};

	HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void UserProfile::GenerateUploadURLForUserContent(const FString& UserId
	, EAccelByteFileType FileType
	, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteUploadCategory Category)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsAccelByteIDValid(*UserId))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, User Id format is invalid"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/files")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("fileType"), FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower()},
		{ TEXT("category"), FAccelByteUtilities::GetUEnumValueAsString(Category).ToLower()}
	};

	HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void UserProfile::GetPrivateCustomAttributes(const THandler<FJsonObjectWrapper>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles/privateCustomAttributes")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UserProfile::UpdatePrivateCustomAttributes(const FJsonObject& PrivateCustomAttributesUpdateRequest
	, const THandler<FJsonObjectWrapper>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles/privateCustomAttributes")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef.GetNamespace());

	FString Content;
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(PrivateCustomAttributesUpdateRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
  
} // Namespace Api
} // Namespace AccelByte
