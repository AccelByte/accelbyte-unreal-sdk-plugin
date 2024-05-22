// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerBinaryCloudSaveApi.h"
#include "Core/AccelByteRegistry.h"
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
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
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
		TTLConfigJson->SetStringField("action", TTLConfig.Action == EAccelByteTTLConfigAction::DELETE_RECORD ? "DELETE" : "NONE");
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
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
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
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
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
		TTLConfigJson->SetStringField("action", TTLConfig.Action == EAccelByteTTLConfigAction::DELETE_RECORD ? "DELETE" : "NONE");
		TTLConfigJson->SetStringField("expires_at", TTLConfig.Expires_At.ToIso8601());
		JsonObj.SetObjectField(TEXT("ttl_config"), TTLConfigJson);
	}
	auto Content = MakeShared<FJsonObject>(JsonObj);
	
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
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
	
#pragma endregion
	
} // namespace GameServerApi
} // namespace AccelByte