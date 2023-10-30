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

void ServerCloudSave::RetrieveGameRecordsKey(const THandler<FAccelByteModelsPaginatedRecordsKey>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& Query
	, int32 Offset
	, int32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

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

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

void ServerCloudSave::SaveGameRecord(FString const& Key
	, ESetByMetadataRecord SetBy
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{ 
	FReport::Log(FString(__FUNCTION__));

	FJsonObject NewRecordRequest = CreateGameRecordWithMetadata(SetBy, RecordRequest);
	SaveGameRecord(Key, NewRecordRequest, OnSuccess, OnError );
}

void ServerCloudSave::SaveGameRecord(const FString& Key
	, const FJsonObject& RecordRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *Key);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void ServerCloudSave::GetGameRecord(const FString& Key
	, const THandler<FAccelByteModelsGameRecord>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *Key);

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient =
		THandler<FJsonObject>::CreateLambda([OnSuccess](const FJsonObject& jsonObject)
		{
			FAccelByteModelsGameRecord gameRecord;
			jsonObject.TryGetStringField("key", gameRecord.Key);
			jsonObject.TryGetStringField("namespace", gameRecord.Namespace);
			FString CreatedAt;
			jsonObject.TryGetStringField("created_at", CreatedAt);
			FDateTime::ParseIso8601(*CreatedAt, gameRecord.CreatedAt);
			FString UpdatedAt;
			jsonObject.TryGetStringField("updated_at", UpdatedAt);
			FDateTime::ParseIso8601(*UpdatedAt, gameRecord.UpdatedAt);
			FString SetByString;
			jsonObject.TryGetStringField("set_by", SetByString);
			gameRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField("value", value);
			gameRecord.Value = ConvertJsonObjToJsonObjWrapper(value);
			OnSuccess.ExecuteIfBound(gameRecord);
		});

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

void ServerCloudSave::ReplaceGameRecord(const FString& Key
	, ESetByMetadataRecord SetBy
	, const FJsonObject& RecordRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	FJsonObject NewRecordRequest = CreateGameRecordWithMetadata(SetBy, RecordRequest);
	ReplaceGameRecord(Key, NewRecordRequest, OnSuccess, OnError);
}

void ServerCloudSave::ReplaceGameRecord(const FString& Key
	, const FJsonObject& RecordRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *Key);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void ServerCloudSave::DeleteGameRecord(const FString& Key
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *Key);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerCloudSave::SaveUserRecord(const FString& Key
	, const FString& UserId
	, ESetByMetadataRecord SetBy
	, bool SetPublic
	, const FJsonObject& RecordRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	FJsonObject NewRecordRequest = CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
	SaveUserRecord(Key, UserId, NewRecordRequest, OnSuccess, OnError);
}

void ServerCloudSave::SaveUserRecord(const FString& Key
	, const FString& UserId
	, const FJsonObject& RecordRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *Key );

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void ServerCloudSave::SaveUserRecord(const FString& Key
	, const FString& UserId
	, ESetByMetadataRecord SetBy
	, bool SetPublic
	, const FJsonObject& RecordRequest
	, bool bIsPublic
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	FJsonObject NewRecordRequest = bIsPublic ? RecordRequest : CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
	SaveUserRecord(Key, UserId, NewRecordRequest, bIsPublic, OnSuccess, OnError);
}

void ServerCloudSave::SaveUserRecord(const FString& Key
	, const FString& UserId, const FJsonObject& RecordRequest
	, bool bIsPublic
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *Key
		, (bIsPublic ? TEXT("/public") : TEXT("")));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void ServerCloudSave::GetUserRecord(const FString& Key
	, const FString& UserId
	, const THandler<FAccelByteModelsUserRecord>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *Key);

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](const FJsonObject& jsonObject)
		{
			FAccelByteModelsUserRecord userRecord;
			jsonObject.TryGetStringField("key", userRecord.Key);
			jsonObject.TryGetStringField("namespace", userRecord.Namespace);
			jsonObject.TryGetStringField("user_id", userRecord.UserId);
			jsonObject.TryGetBoolField("is_public", userRecord.IsPublic);
			FString CreatedAt;
			jsonObject.TryGetStringField("created_at", CreatedAt);
			FDateTime::ParseIso8601(*CreatedAt, userRecord.CreatedAt);
			FString UpdatedAt;
			jsonObject.TryGetStringField("updated_at", UpdatedAt);
			FDateTime::ParseIso8601(*UpdatedAt, userRecord.UpdatedAt);
			FString SetByString;
			jsonObject.TryGetStringField("set_by", SetByString);
			userRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField("value", value);
			userRecord.Value = ConvertJsonObjToJsonObjWrapper(value);
			OnSuccess.ExecuteIfBound(userRecord);
		});

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

void ServerCloudSave::GetPublicUserRecord(const FString& Key
	, const FString& UserId
	, const THandler<FAccelByteModelsUserRecord>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s/public")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *Key);

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](const FJsonObject& jsonObject)
		{
			FAccelByteModelsUserRecord userRecord;
			jsonObject.TryGetStringField("key", userRecord.Key);
			jsonObject.TryGetStringField("namespace", userRecord.Namespace);
			jsonObject.TryGetStringField("user_id", userRecord.UserId);
			jsonObject.TryGetBoolField("is_public", userRecord.IsPublic);
			FString CreatedAt;
			jsonObject.TryGetStringField("created_at", CreatedAt);
			FDateTime::ParseIso8601(*CreatedAt, userRecord.CreatedAt);
			FString UpdatedAt;
			jsonObject.TryGetStringField("updated_at", UpdatedAt);
			FDateTime::ParseIso8601(*UpdatedAt, userRecord.UpdatedAt);
			FString SetByString;
			jsonObject.TryGetStringField("set_by", SetByString);
			userRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField("value", value);
			userRecord.Value = ConvertJsonObjToJsonObjWrapper(value);
			OnSuccess.ExecuteIfBound(userRecord);
		});

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

void ServerCloudSave::ReplaceUserRecord(const FString& Key
	, ESetByMetadataRecord SetBy
	, bool SetPublic
	, const FString& UserId
	, const FJsonObject& RecordRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FJsonObject& NewRecordRequest = CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
	ReplaceUserRecord(Key, UserId, NewRecordRequest, OnSuccess, OnError);
}

void ServerCloudSave::ReplaceUserRecord(const FString& Key
	, const FString& UserId
	, const FJsonObject& RecordRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *Key);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void ServerCloudSave::ReplaceUserRecord(const FString& Key
	, const FString& UserId
	, ESetByMetadataRecord SetBy
	, bool SetPublic
	, const FJsonObject& RecordRequest
	, bool bIsPublic
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	FJsonObject NewRecordRequest = bIsPublic ? RecordRequest : CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
	ReplaceUserRecord(Key, UserId, NewRecordRequest, bIsPublic, OnSuccess, OnError);
}

void ServerCloudSave::ReplaceUserRecord(const FString& Key
	, const FString& UserId
	, const FJsonObject& RecordRequest
	, bool bIsPublic
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *Key
		, (bIsPublic ? TEXT("/public") : TEXT("")));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void ServerCloudSave::DeleteUserRecord(const FString& Key
	, const FString& UserId
	, bool bIsPublic
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *Key
		, (bIsPublic ? TEXT("/public") : TEXT("")));

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerCloudSave::BulkGetPlayerRecordSize(const FAccelByteModelsBulkGetPlayerRecordSizeRequest& GetPlayerRecordSizeRequest
	, const THandler<FAccelByteModelsPaginatedBulkGetPlayerRecordSizeResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/bulk/records/size")
		, *ServerSettingsRef.CloudSaveServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JsonObject)
		{
			FAccelByteModelsPaginatedBulkGetPlayerRecordSizeResponse PlayerRecordSizes;
			TArray<FAccelByteModelsPlayerRecordSizeResponse> Data{};

			const TArray<TSharedPtr<FJsonValue>>* JsonData;
			JsonObject.TryGetArrayField("data", JsonData);
			for (const TSharedPtr<FJsonValue>& JsonValue : *JsonData)
			{
				FAccelByteModelsPlayerRecordSizeResponse UserRecordData{};
				auto jsonObj = JsonValue->AsObject();
				jsonObj->TryGetNumberField("current_size", UserRecordData.CurrentSize);
				jsonObj->TryGetStringField("key", UserRecordData.Key);
				jsonObj->TryGetNumberField("remaining_size", UserRecordData.RemainingSize);
				jsonObj->TryGetStringField("user_id", UserRecordData.UserId);
				Data.Add(UserRecordData);
			}
			PlayerRecordSizes.Data = Data;

			OnSuccess.ExecuteIfBound(PlayerRecordSizes);
		});

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, GetPlayerRecordSizeRequest, OnSuccessHttpClient, OnError);
}

FJsonObject ServerCloudSave::CreatePlayerRecordWithMetadata(ESetByMetadataRecord SetBy
	, bool SetPublic
	, FJsonObject const& RecordRequest)
{
	FJsonObject NewRecordRequest = RecordRequest;

	const auto MetadataJson = MakeShared<FJsonObject>();
	FString SetByString = FAccelByteUtilities::GetUEnumValueAsString(SetBy);
	MetadataJson->SetStringField(TEXT("set_by"), SetByString);
	MetadataJson->SetBoolField(TEXT("is_public"), SetPublic);
	NewRecordRequest.SetObjectField("__META", MetadataJson);

	return NewRecordRequest;
}

FJsonObject ServerCloudSave::CreateGameRecordWithMetadata(ESetByMetadataRecord SetBy
	, FJsonObject const& RecordRequest)
{
	FJsonObject NewRecordRequest = RecordRequest;

	const auto MetadataJson = MakeShared<FJsonObject>();
	FString SetByString = FAccelByteUtilities::GetUEnumValueAsString(SetBy);
	MetadataJson->SetStringField(TEXT("set_by"), SetByString);
	NewRecordRequest.SetObjectField("__META", MetadataJson);

	return NewRecordRequest;
}

FJsonObjectWrapper ServerCloudSave::ConvertJsonObjToJsonObjWrapper(const TSharedPtr<FJsonObject> *& value)
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