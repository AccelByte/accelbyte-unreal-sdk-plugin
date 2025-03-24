// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerBinaryCloudSaveApi.h"

#include "Core/AccelByteReport.h" 
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h" 
#include "Core/AccelByteUtilities.h"
#include "Models/AccelByteBinaryCloudSaveModels.h"

namespace AccelByte
{
namespace GameServerApi
{
ServerBinaryCloudSave::ServerBinaryCloudSave(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InServerApiClient)
{}

ServerBinaryCloudSave::~ServerBinaryCloudSave()
{}

FAccelByteTaskWPtr ServerBinaryCloudSave::QueryGameBinaryRecords(FString const& Query
	, THandler<FAccelByteModelsPaginatedGameBinaryRecords> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Offset
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (Query.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Query cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/binaries")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("query"), *Query},
		{TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("")},
	};
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerBinaryCloudSave::CreateGameBinaryRecord(FString const& Key
	, EAccelByteFileType FileType
	, ESetByMetadataRecord SetBy
	, THandler<FAccelByteModelsBinaryInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, FTTLConfig const& TTLConfig)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (SetBy == ESetByMetadataRecord::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Set_By cannot be empty!"));
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

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/binaries")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace());
	
	FJsonObject JsonObj{};
	JsonObj.SetStringField("key", Key);
	JsonObj.SetStringField("set_by", FAccelByteUtilities::GetUEnumValueAsString(SetBy));
	JsonObj.SetStringField("file_type", FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower());
	if (TTLConfig.Action != EAccelByteTTLConfigAction::NONE)
	{
		const auto TTLConfigJson = MakeShared<FJsonObject>();
		TTLConfigJson->SetStringField(TEXT("action"),
			(TTLConfig.Action == EAccelByteTTLConfigAction::DELETE_RECORD) ? TEXT("DELETE") : TEXT("NONE"));
		TTLConfigJson->SetStringField("expires_at", TTLConfig.Expires_At.ToIso8601());
		JsonObj.SetObjectField(TEXT("ttl_config"), TTLConfigJson);
	}
	auto Content = MakeShared<FJsonObject>(JsonObj);
	
	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}	

FAccelByteTaskWPtr ServerBinaryCloudSave::GetGameBinaryRecord(FString const& Key
	, THandler<FAccelByteModelsGameBinaryRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/binaries/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess, this](FJsonObject const& JSONObject)
	{
		const FAccelByteModelsGameBinaryRecord AdminGameRecord = ConvertJsonToGameBinaryRecord(JSONObject);
		OnSuccess.ExecuteIfBound(AdminGameRecord);
	});
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerBinaryCloudSave::UpdateGameBinaryRecord(FString const& Key
	, EAccelByteFileType ContentType
	, FString const& FileLocation
	, THandler<FAccelByteModelsGameBinaryRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (ContentType == EAccelByteFileType::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("File_Type cannot be empty!"));
		return nullptr;
	}

	if (FileLocation.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("File Location cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/binaries/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	FJsonObject JsonObj{};
	JsonObj.SetStringField("content_type", FAccelByteUtilities::GetContentType(ContentType));
	JsonObj.SetStringField("file_location", FileLocation);
	auto Content = MakeShared<FJsonObject>(JsonObj);

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess, this](FJsonObject const& JSONObject)
	{
		const FAccelByteModelsGameBinaryRecord AdminGameRecord = ConvertJsonToGameBinaryRecord(JSONObject);
		OnSuccess.ExecuteIfBound(AdminGameRecord);
	});
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerBinaryCloudSave::DeleteGameBinaryRecord(FString const& Key
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/binaries/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);
	
	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerBinaryCloudSave::DeleteGameBinaryRecordTTLConfig(FString const& Key,
	FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/binaries/%s/ttl")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);
	
	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerBinaryCloudSave::UpdateGameBinaryRecordMetadata(FString const& Key
	, ESetByMetadataRecord SetBy
	, THandler<FAccelByteModelsGameBinaryRecord> const& OnSuccess
	, FErrorHandler const& OnError
	, FTTLConfig const& TTLConfig)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	if (SetBy == ESetByMetadataRecord::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Set_By cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/binaries/%s/metadata")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	FJsonObject JsonObj{};
	JsonObj.SetStringField("set_by", FAccelByteUtilities::GetUEnumValueAsString(SetBy));
	if (TTLConfig.Action != EAccelByteTTLConfigAction::NONE)
	{
		const auto TTLConfigJson = MakeShared<FJsonObject>();
		TTLConfigJson->SetStringField(TEXT("action"),
			(TTLConfig.Action == EAccelByteTTLConfigAction::DELETE_RECORD) ? TEXT("DELETE") : TEXT("NONE"));
		TTLConfigJson->SetStringField("expires_at", TTLConfig.Expires_At.ToIso8601());
		JsonObj.SetObjectField(TEXT("ttl_config"), TTLConfigJson);
	}
	auto Content = MakeShared<FJsonObject>(JsonObj);

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess, this](FJsonObject const& JSONObject)
	{
		const FAccelByteModelsGameBinaryRecord AdminGameRecord = ConvertJsonToGameBinaryRecord(JSONObject);
		OnSuccess.ExecuteIfBound(AdminGameRecord);
	});
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerBinaryCloudSave::RequestGameBinaryRecordPresignedUrl(FString const& Key
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

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/binaries/%s/presigned")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	FJsonObject JsonObj{};
	JsonObj.SetStringField("file_type", FAccelByteUtilities::GetUEnumValueAsString(FileType).ToLower());
	auto Content = MakeShared<FJsonObject>(JsonObj);
	
	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteModelsGameBinaryRecord ServerBinaryCloudSave::ConvertJsonToGameBinaryRecord(FJsonObject const& JsonObject)
{
	FAccelByteModelsGameBinaryRecord GameBinaryRecord;
	JsonObject.TryGetStringField(TEXT("key"), GameBinaryRecord.Key);
	JsonObject.TryGetStringField(TEXT("namespace"), GameBinaryRecord.Namespace);
	JsonObject.TryGetStringArrayField(TEXT("tags"), GameBinaryRecord.Tags);
	FString CreatedAt;
	JsonObject.TryGetStringField(TEXT("created_at"), CreatedAt);
	FDateTime::ParseIso8601(*CreatedAt, GameBinaryRecord.Created_At);
	FString UpdatedAt;
	JsonObject.TryGetStringField(TEXT("updated_at"), UpdatedAt);
	FDateTime::ParseIso8601(*UpdatedAt, GameBinaryRecord.Updated_At);
	FString SetByString;
	JsonObject.TryGetStringField(TEXT("set_by"), SetByString);
	GameBinaryRecord.Set_By = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
	TSharedPtr<FJsonObject> const* Ttl_Config = nullptr;
	JsonObject.TryGetObjectField(TEXT("ttl_config"), Ttl_Config);
	if (Ttl_Config)
	{
		FString Action;
		Ttl_Config->ToSharedRef()->TryGetStringField(TEXT("action"), Action);
		GameBinaryRecord.Ttl_Config.Action = (Action == TEXT("DELETE")) ? EAccelByteTTLConfigAction::DELETE_RECORD : EAccelByteTTLConfigAction::NONE;
		FString ExpiresAt;
		Ttl_Config->ToSharedRef()->TryGetStringField(TEXT("expires_at"), ExpiresAt);
		FDateTime::ParseIso8601(*ExpiresAt, GameBinaryRecord.Ttl_Config.Expires_At);
	}
	TSharedPtr<FJsonObject> const* Binary_Info = nullptr;
	JsonObject.TryGetObjectField(TEXT("binary_info"), Binary_Info);
	if (Binary_Info)
	{
		Binary_Info->ToSharedRef()->TryGetStringField(TEXT("content_type"), GameBinaryRecord.Binary_Info.Content_Type);
		FString BinaryCreatedAt;
		Binary_Info->ToSharedRef()->TryGetStringField(TEXT("created_at"), BinaryCreatedAt);
		FDateTime::ParseIso8601(*BinaryCreatedAt, GameBinaryRecord.Binary_Info.Created_At);
		Binary_Info->ToSharedRef()->TryGetStringField(TEXT("file_location"), GameBinaryRecord.Binary_Info.File_Location);
		FString BinaryUpdatedAt;
		Binary_Info->ToSharedRef()->TryGetStringField(TEXT("updated_at"), BinaryUpdatedAt);
		FDateTime::ParseIso8601(*BinaryUpdatedAt, GameBinaryRecord.Binary_Info.Updated_At);
		Binary_Info->ToSharedRef()->TryGetStringField(TEXT("url"), GameBinaryRecord.Binary_Info.Url);
		Binary_Info->ToSharedRef()->TryGetNumberField(TEXT("version"), GameBinaryRecord.Binary_Info.Version);
	}

	return GameBinaryRecord;
}

#pragma endregion
	
} // namespace GameServerApi
} // namespace AccelByte