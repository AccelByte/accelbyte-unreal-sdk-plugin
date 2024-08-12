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

FAccelByteTaskWPtr UserProfile::GetUserProfile(THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::GetPublicUserProfileInfo(FString const& UserID
	, THandler<FAccelByteModelsPublicUserProfileInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/public")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserID);

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("GET"), Url, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::BatchGetPublicUserProfileInfos(FString const& UserIds
	, THandler<TArray<FAccelByteModelsPublicUserProfileInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__)
		, TEXT("The API might be removed without notice, please use BulkGetPublicUserProfileInfos instead!!"));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("userIds"), UserIds}
	};

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::BulkGetPublicUserProfileInfos(TArray<FString> const& UserIds
	, THandler<TArray<FAccelByteModelsPublicUserProfileInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("userIds"), FString::Join(UserIds, TEXT(","))}
	};

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::BulkGetPublicUserProfileInfosV2(TArray<FString> const& UserIds,
	THandler<FAccelByteModelsPublicUserProfileInfoV2> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace());
	
	if (UserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserIds cannot be empty!"));
		return nullptr;
	}

	TArray<FString> ProcessedUserIds{};
	TArray<FString> NotProcessedUserIds{};

	if (UserIds.Num() > MaximumQueryLimit)
	{
		ProcessedUserIds.Append(UserIds.GetData(), MaximumQueryLimit);
		NotProcessedUserIds.Append(UserIds.GetData() + MaximumQueryLimit, UserIds.Num() - MaximumQueryLimit);
	}
	else
	{
		ProcessedUserIds = UserIds;
	}

	TArray<TSharedPtr<FJsonValue>> UserIdsJsonValues;
	for (auto UserId : ProcessedUserIds)
	{
		UserIdsJsonValues.Add(MakeShareable(new FJsonValueString(UserId)));
	}
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetArrayField("userIds", UserIdsJsonValues);

	FString Content = TEXT("");
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("POST"), Url, Content, Headers,
		THandler<TArray<FAccelByteModelsPublicUserProfileInfo>>::CreateLambda([OnSuccess, NotProcessedUserIds](const TArray<FAccelByteModelsPublicUserProfileInfo>& Result)
		{
			FAccelByteModelsPublicUserProfileInfoV2 FinalResult;
			FinalResult.UserProfileInfos = Result;
			FinalResult.NotProcessed = NotProcessedUserIds;
			OnSuccess.ExecuteIfBound(FinalResult);
		}), OnError);
}

FAccelByteTaskWPtr UserProfile::GetCustomAttributes(THandler<FJsonObject> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/customAttributes")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::GetPublicCustomAttributes(FString const& UserId
	, THandler<FJsonObject> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), "This will no longer able to use since will give security hole issue for other player/user, "
	 "please use GetPublicUserProfileInfo instead");

	OnError.ExecuteIfBound(static_cast<int>(ErrorCodes::Deprecated),
		TEXT("This will no longer able to use since will give security hole issue for other player/user, use GetPublicUserProfileInfo instead"));
	return nullptr;
}

FAccelByteTaskWPtr UserProfile::UpdateUserProfile(FAccelByteModelsUserProfileUpdateRequest const& ProfileUpdateRequest
	, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, ProfileUpdateRequest, OnSuccess, OnError, true);
}

FAccelByteTaskWPtr UserProfile::UpdateCustomAttributes(FJsonObject const& CustomAttributesUpdateRequest
	, THandler<FJsonObject> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles/customAttributes")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FString Content;
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(CustomAttributesUpdateRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::CreateUserProfile(FAccelByteModelsUserProfileCreateRequest const& ProfileCreateRequest
	, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace());

	FString Content;
	const TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ProfileCreateRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::GetUserProfilePublicInfoByPublicId(FString const& PublicId
	, THandler<FAccelByteModelsPublicUserProfileInfo> const& OnSuccess
	, FCustomErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/profiles/public/byPublicId")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("publicId"), *PublicId}
	};

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::CreateUserProfile(FString const& UserId
	, FAccelByteModelsUserProfileCreateRequest const& ProfileCreateRequest
	, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	FString Content;
	const TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(ProfileCreateRequest);
	FAccelByteUtilities::RemoveEmptyStrings(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr UserProfile::UpdateUserProfile(FString const& UserId
	, FAccelByteModelsUserProfileUpdateRequest const& ProfileUpdateRequest
	, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, ProfileUpdateRequest, OnSuccess, OnError, true);
}

FAccelByteTaskWPtr UserProfile::GetUserProfile(FString const& UserId
	, THandler<FAccelByteModelsUserProfileInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/profiles")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::GenerateUploadURL(FString const& Folder
	, EAccelByteFileType FileType
	, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/folders/%s/files")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace()
		, *Folder);

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("fileType"), FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower()}
	};

	return HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::GenerateUploadURLForUserContent(FString const& UserId
	, EAccelByteFileType FileType
	, THandler<FAccelByteModelsUserProfileUploadURLResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteUploadCategory Category)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/files")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("fileType"), FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower()},
		{ TEXT("category"), FAccelByteUtilities::GetUEnumValueAsString(Category).ToLower()}
	};

	return HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::GetPrivateCustomAttributes(THandler<FJsonObjectWrapper> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles/privateCustomAttributes")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UserProfile::UpdatePrivateCustomAttributes(FJsonObject const& PrivateCustomAttributesUpdateRequest
	, THandler<FJsonObjectWrapper> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/profiles/privateCustomAttributes")
		, *SettingsRef.BasicServerUrl
		, *CredentialsRef->GetNamespace());

	FString Content;
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(PrivateCustomAttributesUpdateRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
  
} // Namespace Api
} // Namespace AccelByte
