// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerCloudSaveApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h" 
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h" 
#include "Core/AccelByteUtilities.h"
#include "Models/AccelByteCloudSaveModels.h" 

namespace AccelByte
{
namespace GameServerApi
{
ServerCloudSave::ServerCloudSave(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

ServerCloudSave::~ServerCloudSave()
{}

FAccelByteTaskWPtr ServerCloudSave::RetrieveGameRecordsKey(THandler<FAccelByteModelsPaginatedRecordsKey> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& Query
	, int32 Offset
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	TMultiMap<FString, FString> QueryParams;
	if(!Query.IsEmpty())
	{
		QueryParams.Add(TEXT("query=%s"), *Query);
	}
	if (Offset >= 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit >= 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::SaveGameRecord(FString const& Key
	, ESetByMetadataRecord SetBy
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FTTLConfig const& TTLConfig)
{ 
	FReport::Log(FString(__FUNCTION__));

	FJsonObject NewRecordRequest = CreateGameRecordWithMetadata(SetBy, TTLConfig, RecordRequest);
	return SaveGameRecord(Key, NewRecordRequest, OnSuccess, OnError );
}

FAccelByteTaskWPtr ServerCloudSave::SaveGameRecord(FString const& Key
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::GetGameRecord(FString const& Key
	, THandler<FAccelByteModelsGameRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient =
		THandler<FJsonObject>::CreateLambda([OnSuccess](FJsonObject const& jsonObject)
		{
			FAccelByteModelsGameRecord gameRecord;
			jsonObject.TryGetStringField(TEXT("key"), gameRecord.Key);
			jsonObject.TryGetStringField(TEXT("namespace"), gameRecord.Namespace);
			FString CreatedAt;
			jsonObject.TryGetStringField(TEXT("created_at"), CreatedAt);
			FDateTime::ParseIso8601(*CreatedAt, gameRecord.CreatedAt);
			FString UpdatedAt;
			jsonObject.TryGetStringField(TEXT("updated_at"), UpdatedAt);
			FDateTime::ParseIso8601(*UpdatedAt, gameRecord.UpdatedAt);
			FString SetByString;
			jsonObject.TryGetStringField(TEXT("set_by"), SetByString);
			gameRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField(TEXT("value"), value);
			gameRecord.Value = ConvertJsonObjToJsonObjWrapper(value);
			OnSuccess.ExecuteIfBound(gameRecord);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::ReplaceGameRecord(FString const& Key
	, ESetByMetadataRecord SetBy
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FTTLConfig const& TTLConfig)
{
	FReport::Log(FString(__FUNCTION__)); 

	FJsonObject NewRecordRequest = CreateGameRecordWithMetadata(SetBy, TTLConfig, RecordRequest);
	return ReplaceGameRecord(Key, NewRecordRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::ReplaceGameRecord(FString const& Key
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::DeleteGameRecord(FString const& Key
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::SaveUserRecord(FString const& Key
	, FString const& UserId
	, ESetByMetadataRecord SetBy
	, bool SetPublic
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	FJsonObject NewRecordRequest = CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
	return SaveUserRecord(Key, UserId, NewRecordRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::SaveUserRecord(FString const& Key
	, FString const& UserId
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key );

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::SaveUserRecord(FString const& Key
	, FString const& UserId
	, ESetByMetadataRecord SetBy
	, bool SetPublic
	, FJsonObject const& RecordRequest
	, bool bIsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	FJsonObject NewRecordRequest = bIsPublic ? RecordRequest : CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
	return SaveUserRecord(Key, UserId, NewRecordRequest, bIsPublic, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::SaveUserRecord(FString const& Key
	, FString const& UserId, FJsonObject const& RecordRequest
	, bool bIsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key
		, (bIsPublic ? TEXT("/public") : TEXT("")));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::GetUserRecord(FString const& Key
	, FString const& UserId
	, THandler<FAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key);

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& jsonObject)
		{
			FAccelByteModelsUserRecord userRecord;
			jsonObject.TryGetStringField(TEXT("key"), userRecord.Key);
			jsonObject.TryGetStringField(TEXT("namespace"), userRecord.Namespace);
			jsonObject.TryGetStringField(TEXT("user_id"), userRecord.UserId);
			jsonObject.TryGetBoolField(TEXT("is_public"), userRecord.IsPublic);
			FString CreatedAt;
			jsonObject.TryGetStringField(TEXT("created_at"), CreatedAt);
			FDateTime::ParseIso8601(*CreatedAt, userRecord.CreatedAt);
			FString UpdatedAt;
			jsonObject.TryGetStringField(TEXT("updated_at"), UpdatedAt);
			FDateTime::ParseIso8601(*UpdatedAt, userRecord.UpdatedAt);
			FString SetByString;
			jsonObject.TryGetStringField(TEXT("set_by"), SetByString);
			userRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField(TEXT("value"), value);
			userRecord.Value = ConvertJsonObjToJsonObjWrapper(value);
			OnSuccess.ExecuteIfBound(userRecord);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::GetPublicUserRecord(FString const& Key
	, FString const& UserId
	, THandler<FAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s/public")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key);

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& jsonObject)
		{
			FAccelByteModelsUserRecord userRecord;
			jsonObject.TryGetStringField(TEXT("key"), userRecord.Key);
			jsonObject.TryGetStringField(TEXT("namespace"), userRecord.Namespace);
			jsonObject.TryGetStringField(TEXT("user_id"), userRecord.UserId);
			jsonObject.TryGetBoolField(TEXT("is_public"), userRecord.IsPublic);
			FString CreatedAt;
			jsonObject.TryGetStringField(TEXT("created_at"), CreatedAt);
			FDateTime::ParseIso8601(*CreatedAt, userRecord.CreatedAt);
			FString UpdatedAt;
			jsonObject.TryGetStringField(TEXT("updated_at"), UpdatedAt);
			FDateTime::ParseIso8601(*UpdatedAt, userRecord.UpdatedAt);
			FString SetByString;
			jsonObject.TryGetStringField(TEXT("set_by"), SetByString);
			userRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField(TEXT("value"), value);
			userRecord.Value = ConvertJsonObjToJsonObjWrapper(value);
			OnSuccess.ExecuteIfBound(userRecord);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::ReplaceUserRecord(FString const& Key
	, ESetByMetadataRecord SetBy
	, bool SetPublic
	, FString const& UserId
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	FJsonObject const& NewRecordRequest = CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
	return ReplaceUserRecord(Key, UserId, NewRecordRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::ReplaceUserRecord(FString const& Key
	, FString const& UserId
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::ReplaceUserRecord(FString const& Key
	, FString const& UserId
	, ESetByMetadataRecord SetBy
	, bool SetPublic
	, FJsonObject const& RecordRequest
	, bool bIsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	FJsonObject NewRecordRequest = bIsPublic ? RecordRequest : CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
	return ReplaceUserRecord(Key, UserId, NewRecordRequest, bIsPublic, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::ReplaceUserRecord(FString const& Key
	, FString const& UserId
	, FJsonObject const& RecordRequest
	, bool bIsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key
		, (bIsPublic ? TEXT("/public") : TEXT("")));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::DeleteUserRecord(FString const& Key
	, FString const& UserId
	, bool bIsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key
		, (bIsPublic ? TEXT("/public") : TEXT("")));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::BulkGetPlayerRecordSize(FAccelByteModelsBulkGetPlayerRecordSizeRequest const& GetPlayerRecordSizeRequest
	, THandler<FAccelByteModelsPaginatedBulkGetPlayerRecordSizeResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/bulk/records/size")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JsonObject)
		{
			FAccelByteModelsPaginatedBulkGetPlayerRecordSizeResponse PlayerRecordSizes;
			TArray<FAccelByteModelsPlayerRecordSizeResponse> Data{};

			const TArray<TSharedPtr<FJsonValue>>* JsonData;
			JsonObject.TryGetArrayField(TEXT("data"), JsonData);
			for (const TSharedPtr<FJsonValue>& JsonValue : *JsonData)
			{
				FAccelByteModelsPlayerRecordSizeResponse UserRecordData{};
				auto jsonObj = JsonValue->AsObject();
				jsonObj->TryGetNumberField(TEXT("current_size"), UserRecordData.CurrentSize);
				jsonObj->TryGetStringField(TEXT("key"), UserRecordData.Key);
				jsonObj->TryGetNumberField(TEXT("remaining_size"), UserRecordData.RemainingSize);
				jsonObj->TryGetStringField(TEXT("user_id"), UserRecordData.UserId);
				Data.Add(UserRecordData);
			}
			PlayerRecordSizes.Data = Data;

			OnSuccess.ExecuteIfBound(PlayerRecordSizes);
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, GetPlayerRecordSizeRequest, OnSuccessHttpClient, OnError);
}

#pragma region AdminRecord

FAccelByteTaskWPtr ServerCloudSave::CreateAdminGameRecord(FString const& Key
	, FJsonObject const& RecordRequest
	, THandler<FAccelByteModelsAdminGameRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/adminrecords/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	TSharedRef<FJsonObject> Content = MakeShared<FJsonObject>(RecordRequest);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::QueryAdminGameRecordsByKey(FString const& Key
	, THandler<FAccelByteModelsAdminGameRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/adminrecords/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	return HttpClient.ApiRequest(TEXT("GET"), Url, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::QueryAdminGameRecordKeys(int Limit
	, int Offset
	, THandler<FAccelByteModelsPaginatedRecordsKey> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	TMultiMap<FString, FString> QueryParams;
	if (Offset >= 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit >= 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/adminrecords")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::ReplaceAdminGameRecord(FString const& Key
	, FJsonObject const& RecordRequest
	, THandler<FAccelByteModelsAdminGameRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/adminrecords/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	TSharedRef<FJsonObject> Content = MakeShared<FJsonObject>(RecordRequest);
	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::DeleteAdminGameRecord(FString const& Key
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/adminrecords/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Key);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::CreateAdminUserRecord(FString const& Key
	, FString const& UserId
	, FJsonObject const& RecordRequest
	, THandler<FAccelByteModelsAdminUserRecord> const& OnSuccess
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

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/adminrecords/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key);

	TSharedRef<FJsonObject> Content = MakeShared<FJsonObject>(RecordRequest);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::QueryAdminUserRecordsByKey(FString const& Key
	, FString const& UserId
	, THandler<FAccelByteModelsAdminUserRecord> const& OnSuccess
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

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/adminrecords/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key);

	return HttpClient.ApiRequest(TEXT("GET"), Url, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::QueryAdminUserRecordKeys(FString const& UserId
	, int Limit
	, int Offset
	,  THandler<FAccelByteModelsPaginatedGetAdminUserRecordKeysResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("User id cannot be empty!"));
		return nullptr;
	}

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	TMultiMap<FString, FString> QueryParams;
	if (Offset >= 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit >= 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/adminrecords")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::ReplaceAdminUserRecord(FString const& Key
	, FString const& UserId
	, FJsonObject const& RecordRequest
	, THandler<FAccelByteModelsAdminUserRecord> const& OnSuccess
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

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/adminrecords/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key);

	TSharedRef<FJsonObject> Content = MakeShared<FJsonObject>(RecordRequest);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerCloudSave::DeleteAdminUserRecord(FString const& Key
	, FString const& UserId
	, FVoidHandler const& OnSuccess
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

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/adminrecords/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *Key);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

#pragma endregion

FJsonObject ServerCloudSave::CreatePlayerRecordWithMetadata(ESetByMetadataRecord SetBy
	, bool SetPublic
	, FJsonObject const& RecordRequest)
{
	FJsonObject NewRecordRequest = RecordRequest;

	const auto MetadataJson = MakeShared<FJsonObject>();
	FString SetByString = FAccelByteUtilities::GetUEnumValueAsString(SetBy);
	MetadataJson->SetStringField(TEXT("set_by"), SetByString);
	MetadataJson->SetBoolField(TEXT("is_public"), SetPublic);
	NewRecordRequest.SetObjectField(TEXT("__META"), MetadataJson);

	return NewRecordRequest;
}

FJsonObject ServerCloudSave::CreateGameRecordWithMetadata(ESetByMetadataRecord SetBy
	, FTTLConfig const& TTLConfig
	, FJsonObject const& RecordRequest)
{
	FJsonObject NewRecordRequest = RecordRequest;

	const auto MetadataJson = MakeShared<FJsonObject>();
	FString SetByString = FAccelByteUtilities::GetUEnumValueAsString(SetBy);
	MetadataJson->SetStringField(TEXT("set_by"), SetByString);
	if (TTLConfig.Action != EAccelByteTTLConfigAction::NONE)
	{
		const auto TTLConfigJson = MakeShared<FJsonObject>();
		TTLConfigJson->SetStringField(TEXT("action"), TTLConfig.Action == EAccelByteTTLConfigAction::DELETE_RECORD ? TEXT("DELETE") : TEXT("NONE"));
		TTLConfigJson->SetStringField(TEXT("expires_at"), TTLConfig.Expires_At.ToIso8601());
		MetadataJson->SetObjectField(TEXT("ttl_config"), TTLConfigJson);
	}
	NewRecordRequest.SetObjectField(TEXT("__META"), MetadataJson);

	return NewRecordRequest;
}

FJsonObjectWrapper ServerCloudSave::ConvertJsonObjToJsonObjWrapper(TSharedPtr<FJsonObject> const*& value)
{
	FJsonObjectWrapper jsonObjWrapper{};
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(value->ToSharedRef(), Writer);					
	jsonObjWrapper.JsonObjectFromString(OutputString);

	return jsonObjWrapper;
}
	
} // namespace GameServerApi
} // namespace AccelByte