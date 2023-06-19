// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCloudSaveApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{
CloudSave::CloudSave(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

CloudSave::~CloudSave()
{}

void CloudSave::SaveUserRecord(const FString& Key
	, bool bSetPublic
	, const FJsonObject& RecordRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FJsonObject NewRecordRequest = CreatePlayerRecordWithMetadata(ESetByMetadataRecord::CLIENT, bSetPublic, RecordRequest);
	SaveUserRecord(Key, NewRecordRequest, false, OnSuccess, OnError);
}

void CloudSave::SaveUserRecord(FString const& Key
	, FJsonObject RecordRequest
	, bool IsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *Key
		, (IsPublic ? TEXT("/public") : TEXT("")));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void CloudSave::GetUserRecord(FString const& Key
	, THandler<FAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *Key);

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess, this](FJsonObject const& JSONObject)
		{
			const FAccelByteModelsUserRecord UserRecord = ConvertJsonToUserRecord(JSONObject);
			OnSuccess.ExecuteIfBound(UserRecord);
		});

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

void CloudSave::GetPublicUserRecord(FString const& Key
	, FString const& UserId
	, THandler<FAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s/public")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId
		, *Key);

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess, this](FJsonObject const& JSONObject)
		{
			const FAccelByteModelsUserRecord UserRecord = ConvertJsonToUserRecord(JSONObject);
			OnSuccess.ExecuteIfBound(UserRecord);
		});

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

void CloudSave::BulkGetPublicUserRecord(FString const& Key
	, const TArray<FString>& UserIds
	, THandler<FListAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (UserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserIds cannot be empty!"));
		return;
	}

	if (UserIds.Num() > UserIdsRequestLimit)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("UserIds cannot exceed %d!"), UserIdsRequestLimit));
		return;
	}

	const FListBulkUserInfoRequest UserList{ UserIds };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/bulk/records/%s/public")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *Key);

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			FListAccelByteModelsUserRecord UserRecords;
			TArray<TSharedPtr<FJsonValue> > JSONArray = JSONObject.GetArrayField("data");

			for (int32 i = 0; i < JSONArray.Num(); ++i)
			{
				const auto& Value = JSONArray[i];
				const TSharedPtr<FJsonObject> JSONData = Value->AsObject().ToSharedRef();

				FAccelByteModelsUserRecord UserRecord = ConvertJsonToUserRecord(*JSONData);
				UserRecords.Data.Add(UserRecord);
			}

			OnSuccess.ExecuteIfBound(UserRecords);
		});

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, UserList, OnSuccessHttpClient, OnError);
}

void CloudSave::BulkGetUserRecords(const TArray<FString>& Keys
	, THandler<FListAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (Keys.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Keys cannot be empty!"));
		return;
	}

	if (Keys.Num() > MAX_BULK_KEY_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_KEY_COUNT));
		return;
	}

	const FAccelByteModelsBulkGetRecordsByKeysRequest KeyList{ Keys };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/me/records/bulk")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace());

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			FListAccelByteModelsUserRecord UserRecords;
			TArray<TSharedPtr<FJsonValue> > JSONArray = JSONObject.GetArrayField("data");

			for (int32 i = 0; i < JSONArray.Num(); ++i)
			{
				const auto& Value = JSONArray[i];
				const TSharedPtr<FJsonObject> JSONData = Value->AsObject().ToSharedRef();

				FAccelByteModelsUserRecord UserRecord = ConvertJsonToUserRecord(*JSONData);
				UserRecords.Data.Add(UserRecord);
			}

			OnSuccess.ExecuteIfBound(UserRecords);
		});

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, KeyList, OnSuccessHttpClient, OnError);
}

void CloudSave::ReplaceUserRecord(const FString& Key
	, bool bSetPublic
	, const FJsonObject& RecordRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FJsonObject NewRecordRequest = CreatePlayerRecordWithMetadata(ESetByMetadataRecord::CLIENT, bSetPublic, RecordRequest);
	ReplaceUserRecord(Key, NewRecordRequest, false, OnSuccess, OnError);
}
	
void CloudSave::ReplaceUserRecord(FString const& Key
	, FJsonObject RecordRequest
	, bool IsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *Key
		, (IsPublic ? TEXT("/public") : TEXT("")));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void CloudSave::ReplaceUserRecord(int TryAttempt
	, FString const& Key
	, FAccelByteModelsConcurrentReplaceRequest const& Data
	, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/concurrent/records/%s/public")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *Key);

	FString Content = TEXT("");
	FJsonObject DataJson;
	DataJson.SetStringField("updatedAt", Data.UpdatedAt.ToIso8601());
	DataJson.SetObjectField("value", Data.Value.JsonObject);
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(DataJson);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	const auto OnErrorHttpClient = FErrorHandler::CreateLambda(
		[this, TryAttempt, Key, Data, PayloadModifier, OnSuccess, OnError](const int32 Code, FString const& Message)
		{
			if (Code == static_cast<int32>(ErrorCodes::PlayerRecordPreconditionFailedException))
			{
				if (TryAttempt > 0)
				{
					ReplaceUserRecordCheckLatest(TryAttempt - 1
						, Key
						, Data.Value
						, PayloadModifier
						, OnSuccess
						, OnError);
				}
				else
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			}
			else
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		});

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnErrorHttpClient);
}

void CloudSave::ReplaceUserRecordCheckLatest(FString const& Key
	, FDateTime const LastUpdated
	, FJsonObjectWrapper RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	const FAccelByteModelsConcurrentReplaceRequest Request
	{
		LastUpdated,
		RecordRequest
	};

	ReplaceUserRecord(0, Key, Request, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper>(), OnSuccess, OnError);
}

void CloudSave::ReplaceUserRecordCheckLatest(int TryAttempt
	, FString const& Key
	, FJsonObjectWrapper RecordRequest
	, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (TryAttempt <= 0)
	{
		OnError.ExecuteIfBound(
			static_cast<int32>(ErrorCodes::PlayerRecordPreconditionFailedException),
			"Exhaust all retry attempt to modify game record. Please try again.");
		return;
	}

	GetUserRecord(Key
		, THandler<FAccelByteModelsUserRecord>::CreateLambda(
			[this, TryAttempt, PayloadModifier, Key, OnSuccess, OnError](FAccelByteModelsUserRecord LatestData)
			{
				if (PayloadModifier.IsBound())
				{
					LatestData.Value = PayloadModifier.Execute(LatestData.Value);
				}

				FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
				UpdateRequest.Value = LatestData.Value;
				UpdateRequest.UpdatedAt = LatestData.UpdatedAt;

				ReplaceUserRecord(TryAttempt
					, Key
					, UpdateRequest
					, PayloadModifier
					, OnSuccess
					, OnError);
			})
		, FErrorHandler::CreateLambda(
			[this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError](int32 Code, FString const& Message)
			{
				if (Code == static_cast<int32>(ErrorCodes::PlayerRecordNotFoundException))
				{
					FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
					UpdateRequest.Value = RecordRequest;
					UpdateRequest.UpdatedAt = FDateTime::Now();

					ReplaceUserRecord(TryAttempt
						, Key
						, UpdateRequest
						, PayloadModifier
						, OnSuccess
						, OnError);
				}
				else
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			}));
}

void CloudSave::DeleteUserRecord(FString const& Key
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *Key);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void CloudSave::SaveGameRecord(FString const& Key
	, FJsonObject RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *Key);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void CloudSave::GetGameRecord(FString const& Key
	, THandler<FAccelByteModelsGameRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *Key);

	const auto OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			const FAccelByteModelsGameRecord GameRecord = ConvertJsonToGameRecord(JSONObject);
			OnSuccess.ExecuteIfBound(GameRecord);
		});

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

void CloudSave::ReplaceGameRecord(FString const& Key
	, FJsonObject RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *Key);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void CloudSave::ReplaceGameRecord(int TryAttempt
	, FString const& Key
	, FAccelByteModelsConcurrentReplaceRequest const& Data
	, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/concurrent/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *Key);

	FJsonObject DataJson;
	DataJson.SetStringField("updatedAt", Data.UpdatedAt.ToIso8601());
	DataJson.SetObjectField("value", Data.Value.JsonObject);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(DataJson);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	const auto OnErrorHttpClient = FErrorHandler::CreateLambda(
		[this, TryAttempt, Key, Data, PayloadModifier, OnSuccess, OnError](const int32& Code, FString const& Message)
		{
			if (Code == static_cast<int32>(ErrorCodes::GameRecordPreconditionFailedException))
			{
				if (TryAttempt > 0)
				{
					ReplaceGameRecordCheckLatest(TryAttempt - 1
						, Key
						, Data.Value
						, PayloadModifier
						, OnSuccess
						, OnError);
				}
				else
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			}
			else
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		});

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnErrorHttpClient);
}

void CloudSave::ReplaceGameRecordCheckLatest(FString const& Key
	, FDateTime const LastUpdated
	, FJsonObjectWrapper RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	const FAccelByteModelsConcurrentReplaceRequest Request
	{
		LastUpdated,
		RecordRequest
	};

	ReplaceGameRecord(0, Key, Request, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper>(), OnSuccess, OnError);
}

void CloudSave::ReplaceGameRecordCheckLatest(int TryAttempt
	, FString const& Key
	, FJsonObjectWrapper RecordRequest
	, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (TryAttempt <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::GameRecordPreconditionFailedException), "Exhaust all retry attempt to modify game record. Please try again.");
		return;
	}

	GetGameRecord(Key
		, THandler<FAccelByteModelsGameRecord>::CreateLambda(
			[this, TryAttempt, PayloadModifier, Key, OnSuccess, OnError](FAccelByteModelsGameRecord LatestData)
			{
				if (PayloadModifier.IsBound())
				{
					LatestData.Value = PayloadModifier.Execute(LatestData.Value);
				}

				FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
				UpdateRequest.Value = LatestData.Value;
				UpdateRequest.UpdatedAt = LatestData.UpdatedAt;

				ReplaceGameRecord(TryAttempt
					, Key
					, UpdateRequest
					, PayloadModifier
					, OnSuccess
					, OnError);
			})
		, FErrorHandler::CreateLambda(
			[this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError](int32 Code, FString const& Message)
			{
				if (Code == static_cast<int32>(ErrorCodes::GameRecordNotFoundException))
				{
					FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
					UpdateRequest.Value = RecordRequest;
					UpdateRequest.UpdatedAt = FDateTime::Now();

					ReplaceGameRecord(TryAttempt
						, Key
						, UpdateRequest
						, PayloadModifier
						, OnSuccess
						, OnError);
				}
				else
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			}));
}

void CloudSave::DeleteGameRecord(FString const& Key
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *Key);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void CloudSave::BulkGetGameRecords(TArray<FString> const& Keys
	, THandler<FAccelByteModelsListGameRecords> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (Keys.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Keys cannot be empty!"));
		return;
	}

	if (Keys.Num() > MAX_BULK_KEY_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_KEY_COUNT));
		return;
	}

	const FAccelByteModelsBulkGetRecordsByKeysRequest KeyList{ Keys };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/bulk")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace());

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			FAccelByteModelsListGameRecords GameRecords;
			TArray<TSharedPtr<FJsonValue> > JSONArray = JSONObject.GetArrayField("data");

			for (int32 i = 0; i < JSONArray.Num(); ++i)
			{
				const auto& Value = JSONArray[i];
				const TSharedPtr<FJsonObject> JSONData = Value->AsObject().ToSharedRef();

				FAccelByteModelsGameRecord GameRecord = ConvertJsonToGameRecord(*JSONData);
				GameRecords.Data.Add(GameRecord);
			}

			OnSuccess.ExecuteIfBound(GameRecords);
		});

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, KeyList, OnSuccessHttpClient, OnError);
}

void CloudSave::BulkGetOtherPlayerPublicRecordKeys(FString const& UserId
	, THandler<FAccelByteModelsPaginatedBulkGetPublicUserRecordKeysResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Bulk Get Other Player Public Record Keys Failed, UserId cannot be empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/public")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("")}
	};

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JsonObject)
		{
			FAccelByteModelsPaginatedBulkGetPublicUserRecordKeysResponse PlayerRecordKeys;
			TArray<FAccelByteModelsGetPublicUserRecordKeys> Data{};

			const TArray<TSharedPtr<FJsonValue>>* JsonData;
			JsonObject.TryGetArrayField("data", JsonData);
			for (const TSharedPtr<FJsonValue>& JsonValue : *JsonData)
			{
				FAccelByteModelsGetPublicUserRecordKeys RecordKeys{};
				auto jsonObj = JsonValue->AsObject();
				jsonObj->TryGetStringField("key", RecordKeys.Key);
				jsonObj->TryGetStringField("user_id", RecordKeys.UserId);
				Data.Add(RecordKeys);
			}
			PlayerRecordKeys.Data = Data;

			TSharedPtr<FJsonObject> const* pagingJsonObject;
			JsonObject.TryGetObjectField("paging", pagingJsonObject);
			pagingJsonObject->Get()->TryGetStringField(TEXT("previous"), PlayerRecordKeys.Paging.Previous);
			pagingJsonObject->Get()->TryGetStringField(TEXT("next"), PlayerRecordKeys.Paging.Next);
			pagingJsonObject->Get()->TryGetStringField(TEXT("first"), PlayerRecordKeys.Paging.First);
			pagingJsonObject->Get()->TryGetStringField(TEXT("last"), PlayerRecordKeys.Paging.Last);

			OnSuccess.ExecuteIfBound(PlayerRecordKeys);
		});

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccessHttpClient, OnError);
}

void CloudSave::BulkGetOtherPlayerPublicRecords(FString const& UserId
	, TArray<FString> const& Keys
	, THandler<FListAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Bulk Get Other Player Public Records Failed, UserId cannot be empty!"));
		return;
	}

	if (Keys.Num() <= 0)
	{
		OnSuccess.ExecuteIfBound(FListAccelByteModelsUserRecord{});
		return;
	}

	if (Keys.Num() > MAX_BULK_KEY_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Bulk Get Other Player Public Records Failed, Keys cannot exceed %d!"), MAX_BULK_KEY_COUNT));
		return;
	}

	const FAccelByteModelsBulkGetRecordsByKeysRequest KeyList{ Keys };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/public/bulk")
		, *SettingsRef.CloudSaveServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			FListAccelByteModelsUserRecord UserRecords;
			TArray<TSharedPtr<FJsonValue> > JSONArray = JSONObject.GetArrayField("data");

			for (const auto& JSONArrayValue : JSONArray)
			{
				const TSharedPtr<FJsonObject> JSONData = JSONArrayValue->AsObject().ToSharedRef();

				FAccelByteModelsUserRecord UserRecord = ConvertJsonToUserRecord(*JSONData);
				UserRecords.Data.Add(UserRecord);
			}

			OnSuccess.ExecuteIfBound(UserRecords);
		});

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, KeyList, OnSuccessHttpClient, OnError);
}

FJsonObject CloudSave::CreatePlayerRecordWithMetadata(ESetByMetadataRecord SetBy
	, bool bSetPublic
	, FJsonObject const& RecordRequest)
{
	FJsonObject NewRecordRequest = RecordRequest;

	const auto MetadataJson = MakeShared<FJsonObject>();
	const FString SetByString = FAccelByteUtilities::GetUEnumValueAsString(SetBy);
	
	MetadataJson->SetStringField(TEXT("set_by"), SetByString);
	MetadataJson->SetBoolField(TEXT("is_public"), bSetPublic);
	NewRecordRequest.SetObjectField("__META", MetadataJson);

	return NewRecordRequest;
}

FJsonObject CloudSave::CreateGameRecordWithMetadata(ESetByMetadataRecord SetBy
	, FJsonObject const& RecordRequest)
{
	FJsonObject NewRecordRequest = RecordRequest;

	const auto MetadataJson = MakeShared<FJsonObject>();
	const FString SetByString = FAccelByteUtilities::GetUEnumValueAsString(SetBy);
	MetadataJson->SetStringField(TEXT("set_by"), SetByString);
	NewRecordRequest.SetObjectField("__META", MetadataJson);

	return NewRecordRequest;
}

FJsonObjectWrapper CloudSave::ConvertJsonObjToJsonObjWrapper(const TSharedPtr<FJsonObject> *& value)
{
	FJsonObjectWrapper jsonObjWrapper{};
	FString OutputString;
	const TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(value->ToSharedRef(), Writer);					
	jsonObjWrapper.JsonObjectFromString(OutputString);
	
	return jsonObjWrapper;
}

FAccelByteModelsUserRecord CloudSave::ConvertJsonToUserRecord(FJsonObject const& JSONObject)
{
	FAccelByteModelsUserRecord UserRecord;
	JSONObject.TryGetStringField("key", UserRecord.Key);
	JSONObject.TryGetStringField("namespace", UserRecord.Namespace);
	JSONObject.TryGetStringField("user_id", UserRecord.UserId);
	JSONObject.TryGetBoolField("is_public", UserRecord.IsPublic);
	FString CreatedAt;
	JSONObject.TryGetStringField("created_at", CreatedAt);
	FDateTime::ParseIso8601(*CreatedAt, UserRecord.CreatedAt);
	FString UpdatedAt;
	JSONObject.TryGetStringField("updated_at", UpdatedAt);
	FDateTime::ParseIso8601(*UpdatedAt, UserRecord.UpdatedAt);
	FString SetByString;
	JSONObject.TryGetStringField("set_by", SetByString);
	UserRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
	TSharedPtr<FJsonObject> const* Value;
	JSONObject.TryGetObjectField("value", Value);
	UserRecord.Value = ConvertJsonObjToJsonObjWrapper(Value);

	return UserRecord;
}

FAccelByteModelsGameRecord CloudSave::ConvertJsonToGameRecord(FJsonObject const& JSONObject)
{
	FAccelByteModelsGameRecord GameRecord;
	JSONObject.TryGetStringField("key", GameRecord.Key);
	JSONObject.TryGetStringField("namespace", GameRecord.Namespace);
	FString CreatedAt;
	JSONObject.TryGetStringField("created_at", CreatedAt);
	FDateTime::ParseIso8601(*CreatedAt, GameRecord.CreatedAt);
	FString UpdatedAt;
	JSONObject.TryGetStringField("updated_at", UpdatedAt);
	FDateTime::ParseIso8601(*UpdatedAt, GameRecord.UpdatedAt);
	FString SetByString;
	JSONObject.TryGetStringField("set_by", SetByString);
	GameRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
	TSharedPtr<FJsonObject> const* Value;
	JSONObject.TryGetObjectField("value", Value);
	GameRecord.Value = ConvertJsonObjToJsonObjWrapper(Value);

	return GameRecord;
}

} // Namespace Api
} // Namespace AccelByte
