// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCloudSaveApi.h"
#include "Core/AccelByteError.h"

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
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

CloudSave::~CloudSave()
{}

FAccelByteTaskWPtr CloudSave::SaveUserRecord(FString const& Key
	, bool bSetPublic
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	const FJsonObject NewRecordRequest = CreatePlayerRecordWithMetadata(ESetByMetadataRecord::CLIENT, bSetPublic, RecordRequest);
	return SaveUserRecord(Key, NewRecordRequest, false, OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudSave::SaveUserRecord(FString const& Key
	, FJsonObject RecordRequest
	, bool IsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s%s")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(Key)
		, (IsPublic ? TEXT("/public") : TEXT("")));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudSave::GetUserRecord(FString const& Key
	, THandler<FAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(Key));
	
	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			const FAccelByteModelsUserRecord UserRecord = CloudSave::ConvertJsonToUserRecord(JSONObject);
			OnSuccess.ExecuteIfBound(UserRecord);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr CloudSave::GetPublicUserRecord(FString const& Key
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

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s/public")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(Key));

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			const FAccelByteModelsUserRecord UserRecord = CloudSave::ConvertJsonToUserRecord(JSONObject);
			OnSuccess.ExecuteIfBound(UserRecord);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr CloudSave::BulkGetPublicUserRecord(FString const& Key
	, TArray<FString> const& UserIds
	, THandler<FListAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
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

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/bulk/records/%s/public")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(Key));

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			FListAccelByteModelsUserRecord UserRecords;
			TArray<TSharedPtr<FJsonValue> > JSONArray = JSONObject.GetArrayField(TEXT("data"));

			for (int32 i = 0; i < JSONArray.Num(); ++i)
			{
				const auto& Value = JSONArray[i];
				const TSharedPtr<FJsonObject> JSONData = Value->AsObject().ToSharedRef();

				FAccelByteModelsUserRecord UserRecord = ConvertJsonToUserRecord(*JSONData);
				UserRecords.Data.Add(UserRecord);
			}

			OnSuccess.ExecuteIfBound(UserRecords);
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, UserList, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr CloudSave::BulkGetUserRecords(TArray<FString> const& Keys
	, THandler<FListAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (Keys.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Keys cannot be empty!"));
		return nullptr;
	}

	if (Keys.Num() > MAX_BULK_KEY_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_KEY_COUNT));
		return nullptr;
	}

	const FAccelByteModelsBulkGetRecordsByKeysRequest KeyList{ Keys };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/me/records/bulk")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			FListAccelByteModelsUserRecord UserRecords;
			TArray<TSharedPtr<FJsonValue> > JSONArray = JSONObject.GetArrayField(TEXT("data"));

			for (int32 i = 0; i < JSONArray.Num(); ++i)
			{
				const auto& Value = JSONArray[i];
				const TSharedPtr<FJsonObject> JSONData = Value->AsObject().ToSharedRef();

				FAccelByteModelsUserRecord UserRecord = ConvertJsonToUserRecord(*JSONData);
				UserRecords.Data.Add(UserRecord);
			}

			OnSuccess.ExecuteIfBound(UserRecords);
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, KeyList, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr CloudSave::ReplaceUserRecord(FString const& Key
	, bool bSetPublic
	, FJsonObject const& RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	const FJsonObject NewRecordRequest = CreatePlayerRecordWithMetadata(ESetByMetadataRecord::CLIENT, bSetPublic, RecordRequest);
	return ReplaceUserRecord(Key, NewRecordRequest, false, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr CloudSave::ReplaceUserRecord(FString const& Key
	, FJsonObject RecordRequest
	, bool IsPublic
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s%s")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(Key)
		, (IsPublic ? TEXT("/public") : TEXT("")));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

#pragma region ReplaceUserConcurrentRecord (Without Response)

FAccelByteTaskWPtr CloudSave::ReplaceUserRecord(int TryAttempt
	, FString const& Key
	, FAccelByteModelsConcurrentReplaceRequest const& Data
	, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/concurrent/records/%s/public")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(Key));
		
	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("responseBody"), TEXT("false")}
	};

	FString Content = TEXT("");
	FJsonObject DataJson;
	DataJson.SetStringField("updatedAt", Data.UpdatedAt.ToIso8601());
	DataJson.SetObjectField("value", Data.Value.JsonObject);
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(DataJson);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	CloudSaveWPtr CloudSaveWeak = AsShared();
	const auto OnErrorHttpClient = FErrorHandler::CreateLambda(
		[CloudSaveWeak, TryAttempt, Key, Data, PayloadModifier, OnSuccess, OnError](const int32 Code, FString const& Message)
		{
			if (Code == static_cast<int32>(ErrorCodes::PlayerRecordPreconditionFailedException))
			{
				const auto CloudSavePtr = CloudSaveWeak.Pin();
				if (TryAttempt > 0 && CloudSavePtr.IsValid())
				{
					CloudSavePtr->ReplaceUserRecordCheckLatest(TryAttempt - 1
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

	return HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnErrorHttpClient);
}

FAccelByteTaskWPtr CloudSave::ReplaceUserRecordCheckLatest(FString const& Key
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

	return ReplaceUserRecord(0, Key, Request, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper>(), OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudSave::ReplaceUserRecordCheckLatest(int TryAttempt
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
		return nullptr;
	}

	CloudSaveWPtr CloudSaveWeak = AsShared();
	return GetUserRecord(Key
		, THandler<FAccelByteModelsUserRecord>::CreateLambda(
			[CloudSaveWeak, TryAttempt, PayloadModifier, Key, OnSuccess, OnError](FAccelByteModelsUserRecord LatestData)
			{
				if (PayloadModifier.IsBound())
				{
					LatestData.Value = PayloadModifier.Execute(LatestData.Value);
				}

				FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
				UpdateRequest.Value = LatestData.Value;
				UpdateRequest.UpdatedAt = LatestData.UpdatedAt;

				const auto CloudSavePtr = CloudSaveWeak.Pin();
				if (CloudSavePtr.IsValid())
				{
					CloudSavePtr->ReplaceUserRecord(TryAttempt
						, Key
						, UpdateRequest
						, PayloadModifier
						, OnSuccess
						, OnError);
				}
				else
				{
					OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
				}
			})
		, FErrorHandler::CreateLambda(
			[CloudSaveWeak, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError](int32 Code, FString const& Message)
			{
				const auto CloudSavePtr = CloudSaveWeak.Pin();
				if (Code == static_cast<int32>(ErrorCodes::PlayerRecordNotFoundException) && CloudSavePtr.IsValid())
				{
					FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
					UpdateRequest.Value = RecordRequest;
					UpdateRequest.UpdatedAt = FDateTime::Now();

					CloudSavePtr->ReplaceUserRecord(TryAttempt
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

#pragma endregion ReplaceUserConcurrentRecord (Without Response)	


#pragma region ReplaceUserConcurrentRecord (With Response)

FAccelByteTaskWPtr CloudSave::ReplaceUserRecord(int TryAttempt
	, FString const& Key
	, FAccelByteModelsConcurrentReplaceRequest const& Data
	, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier
	, THandler<FAccelByteModelsReplaceUserRecordResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Key cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/concurrent/records/%s/public")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(Key));

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("responseBody"), TEXT("true")}
	};

	FString Content = TEXT("");
	FJsonObject DataJson;
	DataJson.SetStringField("updatedAt", Data.UpdatedAt.ToIso8601());
	DataJson.SetObjectField("value", Data.Value.JsonObject);
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(DataJson);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	CloudSaveWPtr CloudSaveWeak = AsShared();
	const auto OnErrorHttpClient = FErrorHandler::CreateLambda(
		[CloudSaveWeak, TryAttempt, Key, Data, PayloadModifier, OnSuccess, OnError](const int32 Code, FString const& Message)
		{
			if (Code == static_cast<int32>(ErrorCodes::PlayerRecordPreconditionFailedException))
			{
				const auto CloudSavePtr = CloudSaveWeak.Pin();
				if (TryAttempt > 0 && CloudSavePtr.IsValid())
				{
					CloudSavePtr->ReplaceUserRecordCheckLatest(TryAttempt - 1
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

	return HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnErrorHttpClient);
}	

FAccelByteTaskWPtr CloudSave::ReplaceUserRecordCheckLatest(FString const& Key
	, FDateTime const LastUpdated
	, FJsonObjectWrapper RecordRequest
	, THandler<FAccelByteModelsReplaceUserRecordResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	const FAccelByteModelsConcurrentReplaceRequest Request
	{
		LastUpdated,
		RecordRequest
	};

	return ReplaceUserRecord(0, Key, Request, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper>(), OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudSave::ReplaceUserRecordCheckLatest(int TryAttempt
	, FString const& Key
	, FJsonObjectWrapper RecordRequest
	, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier
	, THandler<FAccelByteModelsReplaceUserRecordResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (TryAttempt <= 0)
	{
		OnError.ExecuteIfBound(
			static_cast<int32>(ErrorCodes::PlayerRecordPreconditionFailedException),
			"Exhaust all retry attempt to modify game record. Please try again.");
		return nullptr;
	}

	CloudSaveWPtr CloudSaveWeak = AsShared();
	return GetUserRecord(Key
		, THandler<FAccelByteModelsUserRecord>::CreateLambda(
			[CloudSaveWeak, TryAttempt, PayloadModifier, Key, OnSuccess, OnError](FAccelByteModelsUserRecord LatestData)
			{
				if (PayloadModifier.IsBound())
				{
					LatestData.Value = PayloadModifier.Execute(LatestData.Value);
				}

				FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
				UpdateRequest.Value = LatestData.Value;
				UpdateRequest.UpdatedAt = LatestData.UpdatedAt;

				const auto CloudSavePtr = CloudSaveWeak.Pin();
				if (CloudSavePtr.IsValid())
				{
					CloudSavePtr->ReplaceUserRecord(TryAttempt
						, Key
						, UpdateRequest
						, PayloadModifier
						, OnSuccess
						, OnError);
				}
				else
				{
					OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
				}
			})
		, FErrorHandler::CreateLambda(
			[CloudSaveWeak, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError](int32 Code, FString const& Message)
			{
				const auto CloudSavePtr = CloudSaveWeak.Pin();
				if (Code == static_cast<int32>(ErrorCodes::PlayerRecordNotFoundException) && CloudSavePtr.IsValid())
				{
					FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
					UpdateRequest.Value = RecordRequest;
					UpdateRequest.UpdatedAt = FDateTime::Now();

					CloudSavePtr->ReplaceUserRecord(TryAttempt
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

#pragma endregion ReplaceUserConcurrentRecord (With Response)		

FAccelByteTaskWPtr CloudSave::DeleteUserRecord(FString const& Key
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(Key));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudSave::SaveGameRecord(FString const& Key
	, FJsonObject RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(Key));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudSave::GetGameRecord(FString const& Key
	, THandler<FAccelByteModelsGameRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(Key));

	const auto OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			const FAccelByteModelsGameRecord GameRecord = ConvertJsonToGameRecord(JSONObject);
			OnSuccess.ExecuteIfBound(GameRecord);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr CloudSave::ReplaceGameRecord(FString const& Key
	, FJsonObject RecordRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(Key));

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(RecordRequest);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudSave::ReplaceGameRecord(int TryAttempt
	, FString const& Key
	, FAccelByteModelsConcurrentReplaceRequest const& Data
	, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/concurrent/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(Key));

	FJsonObject DataJson;
	DataJson.SetStringField("updatedAt", Data.UpdatedAt.ToIso8601());
	DataJson.SetObjectField("value", Data.Value.JsonObject);

	FString Content = TEXT("");
	const TSharedPtr<FJsonObject> JSONObject = MakeShared<FJsonObject>(DataJson);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JSONObject.ToSharedRef(), Writer);

	CloudSaveWPtr CloudSaveWeak = AsShared();
	const auto OnErrorHttpClient = FErrorHandler::CreateLambda(
		[CloudSaveWeak, TryAttempt, Key, Data, PayloadModifier, OnSuccess, OnError](const int32& Code, FString const& Message)
		{
			if (Code == static_cast<int32>(ErrorCodes::GameRecordPreconditionFailedException))
			{
				const auto CloudSavePtr = CloudSaveWeak.Pin();
				if (TryAttempt > 0 && CloudSavePtr.IsValid())
				{
					CloudSavePtr->ReplaceGameRecordCheckLatest(TryAttempt - 1
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

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnErrorHttpClient);
}

FAccelByteTaskWPtr CloudSave::ReplaceGameRecordCheckLatest(FString const& Key
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

	return ReplaceGameRecord(0, Key, Request, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper>(), OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudSave::ReplaceGameRecordCheckLatest(int TryAttempt
	, FString const& Key
	, FJsonObjectWrapper RecordRequest
	, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (TryAttempt <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::GameRecordPreconditionFailedException), "Exhaust all retry attempt to modify game record. Please try again.");
		return nullptr;
	}

	CloudSaveWPtr CloudSaveWeak = AsShared();
	return GetGameRecord(Key
		, THandler<FAccelByteModelsGameRecord>::CreateLambda(
			[CloudSaveWeak, TryAttempt, PayloadModifier, Key, OnSuccess, OnError](FAccelByteModelsGameRecord LatestData)
			{
				if (PayloadModifier.IsBound())
				{
					LatestData.Value = PayloadModifier.Execute(LatestData.Value);
				}

				FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
				UpdateRequest.Value = LatestData.Value;
				UpdateRequest.UpdatedAt = LatestData.UpdatedAt;

				const auto CloudSavePtr = CloudSaveWeak.Pin();
				if (CloudSavePtr.IsValid())
				{
					CloudSavePtr->ReplaceGameRecord(TryAttempt
						, Key
						, UpdateRequest
						, PayloadModifier
						, OnSuccess
						, OnError);
				}
				else
				{
					OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
				}
			})
		, FErrorHandler::CreateLambda(
			[CloudSaveWeak, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError](int32 Code, FString const& Message)
			{
				const auto CloudSavePtr = CloudSaveWeak.Pin();
				if (Code == static_cast<int32>(ErrorCodes::GameRecordNotFoundException) && CloudSavePtr.IsValid())
				{
					FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
					UpdateRequest.Value = RecordRequest;
					UpdateRequest.UpdatedAt = FDateTime::Now();

					CloudSavePtr->ReplaceGameRecord(TryAttempt
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

FAccelByteTaskWPtr CloudSave::DeleteGameRecord(FString const& Key
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(Key));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr CloudSave::BulkGetGameRecords(TArray<FString> const& Keys
	, THandler<FAccelByteModelsListGameRecords> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (Keys.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Keys cannot be empty!"));
		return nullptr;
	}

	if (Keys.Num() > MAX_BULK_KEY_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_KEY_COUNT));
		return nullptr;
	}

	const FAccelByteModelsBulkGetRecordsByKeysRequest KeyList{ Keys };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/bulk")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			FAccelByteModelsListGameRecords GameRecords;
			TArray<TSharedPtr<FJsonValue> > JSONArray = JSONObject.GetArrayField(TEXT("data"));

			for (int32 i = 0; i < JSONArray.Num(); ++i)
			{
				const auto& Value = JSONArray[i];
				const TSharedPtr<FJsonObject> JSONData = Value->AsObject().ToSharedRef();

				FAccelByteModelsGameRecord GameRecord = ConvertJsonToGameRecord(*JSONData);
				GameRecords.Data.Add(GameRecord);
			}

			OnSuccess.ExecuteIfBound(GameRecords);
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, KeyList, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr CloudSave::BulkGetOtherPlayerPublicRecordKeys(FString const& UserId
	, THandler<FAccelByteModelsPaginatedBulkGetPublicUserRecordKeysResponse> const& OnSuccess
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

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/public")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

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
			JsonObject.TryGetArrayField(TEXT("data"), JsonData);
			for (const TSharedPtr<FJsonValue>& JsonValue : *JsonData)
			{
				FAccelByteModelsGetPublicUserRecordKeys RecordKeys{};
				auto jsonObj = JsonValue->AsObject();
				jsonObj->TryGetStringField(TEXT("key"), RecordKeys.Key);
				jsonObj->TryGetStringField(TEXT("user_id"), RecordKeys.UserId);
				Data.Add(RecordKeys);
			}
			PlayerRecordKeys.Data = Data;

			TSharedPtr<FJsonObject> const* pagingJsonObject;
			JsonObject.TryGetObjectField(TEXT("paging"), pagingJsonObject);
			pagingJsonObject->Get()->TryGetStringField(TEXT("previous"), PlayerRecordKeys.Paging.Previous);
			pagingJsonObject->Get()->TryGetStringField(TEXT("next"), PlayerRecordKeys.Paging.Next);
			pagingJsonObject->Get()->TryGetStringField(TEXT("first"), PlayerRecordKeys.Paging.First);
			pagingJsonObject->Get()->TryGetStringField(TEXT("last"), PlayerRecordKeys.Paging.Last);

			OnSuccess.ExecuteIfBound(PlayerRecordKeys);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr CloudSave::BulkGetCurrentPlayerPublicRecordKeys(THandler<FAccelByteModelsPaginatedBulkGetPublicUserRecordKeysResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (Limit >= RESPONSE_MAX_LIMIT_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Maximum content per page cannot exceed %d!"), RESPONSE_MAX_LIMIT_COUNT));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/me/records")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

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
			JsonObject.TryGetArrayField(TEXT("data"), JsonData);
			for (const TSharedPtr<FJsonValue>& JsonValue : *JsonData)
			{
				FAccelByteModelsGetPublicUserRecordKeys RecordKeys{};
				auto jsonObj = JsonValue->AsObject();
				jsonObj->TryGetStringField(TEXT("key"), RecordKeys.Key);
				jsonObj->TryGetStringField(TEXT("user_id"), RecordKeys.UserId);
				Data.Add(RecordKeys);
			}
			PlayerRecordKeys.Data = Data;

			TSharedPtr<FJsonObject> const* pagingJsonObject;
			JsonObject.TryGetObjectField(TEXT("paging"), pagingJsonObject);
			pagingJsonObject->Get()->TryGetStringField(TEXT("previous"), PlayerRecordKeys.Paging.Previous);
			pagingJsonObject->Get()->TryGetStringField(TEXT("next"), PlayerRecordKeys.Paging.Next);
			pagingJsonObject->Get()->TryGetStringField(TEXT("first"), PlayerRecordKeys.Paging.First);
			pagingJsonObject->Get()->TryGetStringField(TEXT("last"), PlayerRecordKeys.Paging.Last);

			OnSuccess.ExecuteIfBound(PlayerRecordKeys);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr CloudSave::BulkGetOtherPlayerPublicRecords(FString const& UserId
	, TArray<FString> const& Keys
	, THandler<FListAccelByteModelsUserRecord> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	if (Keys.Num() <= 0)
	{
		OnSuccess.ExecuteIfBound(FListAccelByteModelsUserRecord{});
		return nullptr;
	}

	if (Keys.Num() > MAX_BULK_KEY_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Bulk Get Other Player Public Records Failed, Keys cannot exceed %d!"), MAX_BULK_KEY_COUNT));
		return nullptr;
	}

	const FAccelByteModelsBulkGetRecordsByKeysRequest KeyList{ Keys };

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/public/bulk")
		, *SettingsRef.CloudSaveServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	const TDelegate<void(const FJsonObject&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JSONObject)
		{
			FListAccelByteModelsUserRecord UserRecords;
			TArray<TSharedPtr<FJsonValue> > JSONArray = JSONObject.GetArrayField(TEXT("data"));

			for (const auto& JSONArrayValue : JSONArray)
			{
				const TSharedPtr<FJsonObject> JSONData = JSONArrayValue->AsObject().ToSharedRef();

				FAccelByteModelsUserRecord UserRecord = ConvertJsonToUserRecord(*JSONData);
				UserRecords.Data.Add(UserRecord);
			}

			OnSuccess.ExecuteIfBound(UserRecords);
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, KeyList, OnSuccessHttpClient, OnError);
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
	JSONObject.TryGetStringField(TEXT("key"), UserRecord.Key);
	JSONObject.TryGetStringField(TEXT("namespace"), UserRecord.Namespace);
	JSONObject.TryGetStringField(TEXT("user_id"), UserRecord.UserId);
	JSONObject.TryGetBoolField(TEXT("is_public"), UserRecord.IsPublic);
	FString CreatedAt;
	JSONObject.TryGetStringField(TEXT("created_at"), CreatedAt);
	FDateTime::ParseIso8601(*CreatedAt, UserRecord.CreatedAt);
	FString UpdatedAt;
	JSONObject.TryGetStringField(TEXT("updated_at"), UpdatedAt);
	FDateTime::ParseIso8601(*UpdatedAt, UserRecord.UpdatedAt);
	FString SetByString;
	JSONObject.TryGetStringField(TEXT("set_by"), SetByString);
	UserRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
	TSharedPtr<FJsonObject> const* Value;
	JSONObject.TryGetObjectField(TEXT("value"), Value);
	UserRecord.Value = ConvertJsonObjToJsonObjWrapper(Value);

	return UserRecord;
}

FAccelByteModelsGameRecord CloudSave::ConvertJsonToGameRecord(FJsonObject const& JSONObject)
{
	FAccelByteModelsGameRecord GameRecord;
	JSONObject.TryGetStringField(TEXT("key"), GameRecord.Key);
	JSONObject.TryGetStringField(TEXT("namespace"), GameRecord.Namespace);
	FString CreatedAt;
	JSONObject.TryGetStringField(TEXT("created_at"), CreatedAt);
	FDateTime::ParseIso8601(*CreatedAt, GameRecord.CreatedAt);
	FString UpdatedAt;
	JSONObject.TryGetStringField(TEXT("updated_at"), UpdatedAt);
	FDateTime::ParseIso8601(*UpdatedAt, GameRecord.UpdatedAt);
	FString SetByString;
	JSONObject.TryGetStringField(TEXT("set_by"), SetByString);
	GameRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
	TSharedPtr<FJsonObject> const* Value;
	JSONObject.TryGetObjectField(TEXT("value"), Value);
	GameRecord.Value = ConvertJsonObjToJsonObjWrapper(Value);
	TSharedPtr<FJsonObject> const* Ttl_Config = nullptr;
	JSONObject.TryGetObjectField(TEXT("ttl_config"), Ttl_Config);
	if (Ttl_Config)
	{
		FString Action;
		Ttl_Config->ToSharedRef()->TryGetStringField(TEXT("action"), Action);
		GameRecord.Ttl_Config.Action = (Action == TEXT("DELETE")) ? EAccelByteTTLConfigAction::DELETE_RECORD : EAccelByteTTLConfigAction::NONE;
		FString ExpiresAt;
		Ttl_Config->ToSharedRef()->TryGetStringField(TEXT("expires_at"), ExpiresAt);
		FDateTime::ParseIso8601(*ExpiresAt, GameRecord.Ttl_Config.Expires_At);
	}

	return GameRecord;
}

} // Namespace Api
} // Namespace AccelByte
