// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteBinaryCloudSaveApi.h"
#include "Core/AccelByteError.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

constexpr int32 UserIdsRequestLimit = 20;
constexpr int32 KeysRequestLimit = 20;
static const TArray<FString> SupportedFileTypes = { "jpeg", "jpg", "png", "bmp", "gif", "mp3", "webp", "bin" };

namespace AccelByte
{
namespace Api
{

BinaryCloudSave::BinaryCloudSave(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

BinaryCloudSave::~BinaryCloudSave()
{}
	
FAccelByteTaskWPtr BinaryCloudSave::SaveUserBinaryRecord(FString const& Key
	, FString const& FileType
	, bool bIsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (SupportedFileTypes.Find(FileType) == INDEX_NONE) {
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("FileType is not supported! Supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FJsonObject JsonObj{};
	JsonObj.SetStringField("key", Key);
	JsonObj.SetBoolField("is_public", bIsPublic);
	JsonObj.SetStringField("file_type", FileType);
	auto Content = MakeShared<FJsonObject>(JsonObj);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::SaveUserBinaryRecord(FString const& Key
	, EAccelByteFileType FileType
	, bool bIsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (FileType == EAccelByteFileType::NONE)
	{
		const FString Message = TEXT("Invalid request. The 'FileType' field must be set to "
							   "(e.g., EAccelByteFileType::JPG, EAccelByteFileType::PNG, etc.). "
							   "Please set a valid 'FileType' value to fulfill the request.");
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), Message);
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FJsonObject JsonObj{};
	JsonObj.SetStringField("key", Key);
	JsonObj.SetBoolField("is_public", bIsPublic);
	JsonObj.SetStringField("file_type", FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower());
	auto Content = MakeShared<FJsonObject>(JsonObj);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::GetCurrentUserBinaryRecord(FString const& Key
	, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *Key);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::GetPublicUserBinaryRecord(FString const& Key
	, FString const& UserId
	, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/%s/public")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId
		, *Key);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::BulkGetCurrentUserBinaryRecords(const TArray<FString>& Keys
	, THandler<FAccelByteModelsListUserBinaryRecords> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (Keys.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Keys cannot be empty!"));
		return nullptr;
	}

	if (Keys.Num() > KeysRequestLimit)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), KeysRequestLimit));
		return nullptr;
	}

	const FAccelByteModelsBulkGetRecordsByKeysRequest KeyList{ Keys };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/me/binaries/bulk")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, KeyList, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::BulkGetPublicUserBinaryRecords(const TArray<FString>& Keys
	, FString const& UserId
	, THandler<FAccelByteModelsListUserBinaryRecords> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Keys.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Keys cannot be empty!"));
		return nullptr;
	}

	if (Keys.Num() > KeysRequestLimit)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), KeysRequestLimit));
		return nullptr;
	}

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FAccelByteModelsBulkGetRecordsByKeysRequest KeyList{ Keys };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/public/bulk")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, KeyList, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::BulkGetPublicUserBinaryRecords(FString const& Key
	, const TArray<FString>& UserIds
	, THandler<FAccelByteModelsListUserBinaryRecords> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (UserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserIds cannot be empty!"));
		return nullptr;
	}

	if (UserIds.Num() > UserIdsRequestLimit)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("UserIds cannot exceed %d!"), UserIdsRequestLimit));
		return nullptr;
	}

	const FListBulkUserInfoRequest UserList{ UserIds };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/bulk/binaries/%s/public")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *Key);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, UserList, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::BulkQueryCurrentUserBinaryRecords(FString const& Query
	, THandler<FAccelByteModelsPaginatedUserBinaryRecords> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/me/binaries")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("query"), *Query},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("")},
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::BulkQueryPublicUserBinaryRecords(FString const& UserId
	, THandler<FAccelByteModelsPaginatedUserBinaryRecords> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/public")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("")},
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::UpdateUserBinaryRecordFile(FString const& Key
	, FString const& FileType
	, FString const& FileLocation
	, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (FileLocation.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("File Location cannot be empty!"));
		return nullptr;
	}

	if (SupportedFileTypes.Find(FileType) == INDEX_NONE) {
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("FileType is not supported! Supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *Key);

	FJsonObject JsonObj{};
	JsonObj.SetStringField("content_type", FileType);
	JsonObj.SetStringField("file_location", FileLocation);
	auto Content = MakeShared<FJsonObject>(JsonObj);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::UpdateUserBinaryRecordFile(FString const& Key
	, EAccelByteFileType ContentType
	, FString const& FileLocation
	, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (FileLocation.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("File Location cannot be empty!"));
		return nullptr;
	}

	if (ContentType == EAccelByteFileType::NONE)
	{
		const FString Message = TEXT("Invalid request. The 'ContentType' field must be set to "
							   "(e.g., EAccelByteFileType::JPG, EAccelByteFileType::PNG, etc.). "
							   "Please set a valid 'ContentType' value to fulfill the request.");
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), Message);
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *Key);

	FJsonObject JsonObj{};
	JsonObj.SetStringField("content_type", FAccelByteUtilities::GetContentType(ContentType));
	JsonObj.SetStringField("file_location", FileLocation);
	auto Content = MakeShared<FJsonObject>(JsonObj);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::UpdateUserBinaryRecordMetadata(FString const& Key
	, bool bIsPublic
	, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/%s/metadata")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *Key);

	FJsonObject JsonObj{};
	JsonObj.SetBoolField("is_public", bIsPublic);
	auto Content = MakeShared<FJsonObject>(JsonObj);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::DeleteUserBinaryRecord(FString const& Key
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *Key);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::RequestUserBinaryRecordPresignedUrl(FString const& Key
	, FString const& FileType
	, THandler<FAccelByteModelsBinaryInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (SupportedFileTypes.Find(FileType) == INDEX_NONE) {
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("FileType is not supported! Supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/%s/presigned")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *Key);

	FJsonObject JsonObj{};
	JsonObj.SetStringField("file_type", FileType);
	auto Content = MakeShared<FJsonObject>(JsonObj);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::RequestUserBinaryRecordPresignedUrl(FString const& Key
	, EAccelByteFileType FileType
	, THandler<FAccelByteModelsBinaryInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (FileType == EAccelByteFileType::NONE)
	{
		const FString Message = TEXT("Invalid request. The 'FileType' field must be set to "
							   "(e.g., EAccelByteFileType::JPG, EAccelByteFileType::PNG, etc.). "
							   "Please set a valid 'FileType' value to fulfill the request.");
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), Message);
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/binaries/%s/presigned")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *Key);

	FJsonObject JsonObj{};
	JsonObj.SetStringField("file_type", FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower());
	auto Content = MakeShared<FJsonObject>(JsonObj);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::GetGameBinaryRecord(FString const& Key
	, THandler<FAccelByteModelsGameBinaryRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/binaries/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace()
		, *Key);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::BulkGetGameBinaryRecords(TArray<FString> const& Keys
	, THandler<FAccelByteModelsListGameBinaryRecords> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Keys.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Keys cannot be empty!"));
		return nullptr;
	}

	if (Keys.Num() > KeysRequestLimit)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), KeysRequestLimit));
		return nullptr;
	}

	const FAccelByteModelsBulkGetRecordsByKeysRequest KeyList{ Keys };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/binaries/bulk")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, KeyList, OnSuccess, OnError);
}

FAccelByteTaskWPtr BinaryCloudSave::BulkQueryGameBinaryRecords(FString const& Query
	, THandler<FAccelByteModelsPaginatedGameBinaryRecords> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (Query.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Query cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/binaries")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("query"), *Query},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("")},
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
