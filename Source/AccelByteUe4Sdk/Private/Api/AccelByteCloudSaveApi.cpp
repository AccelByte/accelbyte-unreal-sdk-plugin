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

namespace AccelByte
{
namespace Api
{
CloudSave::CloudSave(
	AccelByte::Credentials const& Credentials,
	AccelByte::Settings const& Setting,
	FHttpRetryScheduler& HttpRef)
	:
	HttpRef{HttpRef},
	Credentials{Credentials},
	Settings{Setting}
{
}

CloudSave::~CloudSave(){}

void CloudSave::SaveUserRecord(const FString& Key, bool SetPublic, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FJsonObject NewRecordRequest = CreatePlayerRecordWithMetadata(ESetByMetadataRecord::Client, SetPublic, RecordRequest);
	SaveUserRecord(Key, NewRecordRequest, false, OnSuccess, OnError);
}

void CloudSave::SaveUserRecord(FString const& Key, FJsonObject RecordRequest, bool IsPublic, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s%s"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Credentials.GetUserId(), *Key, (IsPublic ? TEXT("/public") : TEXT("")));
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = TEXT("");
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request,  CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void CloudSave::GetUserRecord(FString const& Key, THandler<FAccelByteModelsUserRecord> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Credentials.GetUserId(), *Key);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(
		Request,
		CreateHttpResultHandler(
			THandler<FJsonObject>::CreateLambda(
				[OnSuccess](FJsonObject const& jsonObject)
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
					ESetByMetadataRecord SetBy = ESetByMetadataRecord::Client;
					if (SetByString.Equals(TEXT("SERVER")))
					{
						SetBy = ESetByMetadataRecord::Server;
					}
					else if (SetByString.Equals(TEXT("CLIENT")))
					{
						SetBy = ESetByMetadataRecord::Client;
					}
					userRecord.SetBy = SetBy;
					TSharedPtr<FJsonObject> const* value;
					jsonObject.TryGetObjectField("value", value);
					userRecord.Value = *value->ToSharedRef();
					OnSuccess.ExecuteIfBound(userRecord);
				}),
			OnError),
		FPlatformTime::Seconds());
}

void CloudSave::GetPublicUserRecord(FString const& Key, FString const& UserId, THandler<FAccelByteModelsUserRecord> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s/public"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *UserId, *Key);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(
		Request,
		CreateHttpResultHandler(
			THandler<FJsonObject>::CreateLambda(
				[OnSuccess](FJsonObject const& jsonObject)
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
					ESetByMetadataRecord SetBy = ESetByMetadataRecord::Client;
					if (SetByString.Equals(TEXT("SERVER")))
					{
						SetBy = ESetByMetadataRecord::Server;
					}
					else if (SetByString.Equals(TEXT("CLIENT")))
					{
						SetBy = ESetByMetadataRecord::Client;
					}
					userRecord.SetBy = SetBy;
					TSharedPtr<FJsonObject> const* value;
					jsonObject.TryGetObjectField("value", value);
					userRecord.Value = *value->ToSharedRef();
					OnSuccess.ExecuteIfBound(userRecord);
				}),
			OnError),
		FPlatformTime::Seconds());
}

void CloudSave::BulkGetPublicUserRecord(FString const& Key, const TArray<FString>& UserIds, THandler<FListAccelByteModelsUserRecord> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (UserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound((int32)ErrorCodes::InvalidRequest, TEXT("UserIds cannot be empty!"));
		return;
	}

	if (UserIds.Num() > UserIdsRequestLimit)
	{
		OnError.ExecuteIfBound((int32)ErrorCodes::InvalidRequest, FString::Printf(TEXT("UserIds cannot exceed %d!"), UserIdsRequestLimit));
		return;
	}

	const FListBulkUserInfoRequest UserList{ UserIds };

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/bulk/records/%s/public"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Key);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(UserList, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(
		Request,
		CreateHttpResultHandler(
			THandler<FJsonObject>::CreateLambda(
				[OnSuccess](FJsonObject const& jsonObject)
				{
					FListAccelByteModelsUserRecord userRecords;
					TArray<TSharedPtr<FJsonValue> > JsonArray;
					JsonArray = jsonObject.GetArrayField("data");
					for (int32 i = 0; i < JsonArray.Num(); ++i)
					{
						const auto& Value = JsonArray[i];
						TSharedPtr<FJsonObject> JsonData = MakeShareable(new FJsonObject);
						JsonData = Value->AsObject().ToSharedRef();
						FAccelByteModelsUserRecord userRecord;
						JsonData->TryGetStringField("key", userRecord.Key);
						JsonData->TryGetStringField("namespace", userRecord.Namespace);
						JsonData->TryGetStringField("user_id", userRecord.UserId);
						JsonData->TryGetBoolField("is_public", userRecord.IsPublic);
						FString CreatedAt;
						JsonData->TryGetStringField("created_at", CreatedAt);
						FDateTime::ParseIso8601(*CreatedAt, userRecord.CreatedAt);
						FString UpdatedAt;
						JsonData->TryGetStringField("updated_at", UpdatedAt);
						FDateTime::ParseIso8601(*UpdatedAt, userRecord.UpdatedAt);
						TSharedPtr<FJsonObject> const* value;
						JsonData->TryGetObjectField("value", value);
						userRecord.Value = *value->ToSharedRef();
						userRecords.Data.Add(userRecord);
					}
					OnSuccess.ExecuteIfBound(userRecords);
				}),
			OnError),
		FPlatformTime::Seconds());
}

void CloudSave::ReplaceUserRecord(const FString& Key, bool SetPublic, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FJsonObject NewRecordRequest = CreatePlayerRecordWithMetadata(ESetByMetadataRecord::Client, SetPublic, RecordRequest);
	ReplaceUserRecord(Key, NewRecordRequest, false, OnSuccess, OnError);
}
	
void CloudSave::ReplaceUserRecord(FString const& Key, FJsonObject RecordRequest, bool IsPublic, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s%s"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Credentials.GetUserId(), *Key, (IsPublic ? TEXT("/public") : TEXT("")));
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
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

void CloudSave::ReplaceUserRecord(int TryAttempt, FString const& Key, FAccelByteModelsConcurrentReplaceRequest const& Data, THandlerPayloadModifier<FJsonObject, FJsonObject> const& PayloadModifier, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/concurrent/records/%s/public"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Credentials.GetUserId(), *Key);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObject DataJson;
	DataJson.SetStringField("updatedAt", Data.UpdatedAt.ToIso8601());
	DataJson.SetObjectField("value", MakeShared<FJsonObject>(Data.Value));
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(
		Request,
		CreateHttpResultHandler(
			OnSuccess,
			FErrorHandler::CreateLambda(
				[this, TryAttempt, Key, Data, PayloadModifier, OnSuccess, OnError](int32 Code, FString const& Message)
				{
					if (Code == (int32)ErrorCodes::PlayerRecordPreconditionFailedException)
					{
						if (TryAttempt > 0)
						{
							CloudSave::ReplaceUserRecordCheckLatest(
								TryAttempt - 1,
								Key,
								Data.Value,
								PayloadModifier,
								OnSuccess,
								OnError);
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
				})),
		FPlatformTime::Seconds());
}

void CloudSave::ReplaceUserRecordCheckLatest(FString const& Key, FDateTime const LastUpdated, FJsonObject RecordRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FAccelByteModelsConcurrentReplaceRequest Request
	{
		LastUpdated,
		RecordRequest
	};

	CloudSave::ReplaceUserRecord(0, Key, Request, THandlerPayloadModifier<FJsonObject, FJsonObject>(), OnSuccess, OnError);
}

void CloudSave::ReplaceUserRecordCheckLatest(int TryAttempt, FString const& Key, FJsonObject RecordRequest, THandlerPayloadModifier<FJsonObject, FJsonObject> const& PayloadModifier, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	if (TryAttempt <= 0)
	{
		OnError.ExecuteIfBound(
			(int32)ErrorCodes::PlayerRecordPreconditionFailedException,
			"Exhaust all retry attempt to modify game record. Please try again.");
		return;
	}

	GetUserRecord(
		Key,
		THandler<FAccelByteModelsUserRecord>::CreateLambda(
			[this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError]
			(FAccelByteModelsUserRecord LatestData)
			{
				if (PayloadModifier.IsBound())
				{
					LatestData.Value = PayloadModifier.Execute(LatestData.Value);
				}

				FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
				UpdateRequest.Value = LatestData.Value;
				UpdateRequest.UpdatedAt = LatestData.UpdatedAt;

				CloudSave::ReplaceUserRecord(
					TryAttempt,
					Key,
					UpdateRequest,
					PayloadModifier,
					OnSuccess,
					OnError);
			}),
		FErrorHandler::CreateLambda(
			[this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError]
			(int32 Code, FString const& Message)
			{
				if (Code == (int32)ErrorCodes::PlayerRecordNotFoundException)
				{
					FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
					UpdateRequest.Value = RecordRequest;
					UpdateRequest.UpdatedAt = FDateTime::Now();

					CloudSave::ReplaceUserRecord(
						TryAttempt,
						Key,
						UpdateRequest,
						PayloadModifier,
						OnSuccess,
						OnError);
				}
				else
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			}));
}

void CloudSave::DeleteUserRecord(FString const& Key, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Credentials.GetUserId(), *Key);
	FString Verb = TEXT("DELETE");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void CloudSave::SaveGameRecord(FString const& Key, FJsonObject RecordRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Key);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
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

void CloudSave::GetGameRecord(FString const& Key, THandler<FAccelByteModelsGameRecord> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Key);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(
		Request,
		CreateHttpResultHandler(
			THandler<FJsonObject>::CreateLambda(
				[OnSuccess](FJsonObject const& JSONObject)
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
					TSharedPtr<FJsonObject> const* value;
					JSONObject.TryGetObjectField("value", value);
					GameRecord.Value = *value->ToSharedRef();
					OnSuccess.ExecuteIfBound(GameRecord);
				}),
			OnError),
		FPlatformTime::Seconds());
}

void CloudSave::ReplaceGameRecord(FString const& Key, FJsonObject RecordRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Key);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(RecordRequest);
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

void CloudSave::ReplaceGameRecord(int TryAttempt, FString const& Key, FAccelByteModelsConcurrentReplaceRequest const& Data, THandlerPayloadModifier<FJsonObject, FJsonObject> const& PayloadModifier, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/concurrent/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Key);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObject DataJson;
	DataJson.SetStringField("updatedAt", Data.UpdatedAt.ToIso8601());
	DataJson.SetObjectField("value", MakeShared<FJsonObject>(Data.Value));
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(
		Request,
		CreateHttpResultHandler(
			OnSuccess,
			FErrorHandler::CreateLambda(
				[this, TryAttempt, Key, Data, PayloadModifier, OnSuccess, OnError]
				(int32 Code, FString const& Message)
				{
					if (Code == (int32)ErrorCodes::GameRecordPreconditionFailedException)
					{
						if (TryAttempt > 0)
						{
							CloudSave::ReplaceGameRecordCheckLatest(
								TryAttempt - 1,
								Key,
								Data.Value,
								PayloadModifier,
								OnSuccess,
								OnError);
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
				})),
		FPlatformTime::Seconds());
}

void CloudSave::ReplaceGameRecordCheckLatest(FString const& Key, FDateTime const LastUpdated, FJsonObject RecordRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FAccelByteModelsConcurrentReplaceRequest Request
	{
		LastUpdated,
		RecordRequest
	};

	CloudSave::ReplaceGameRecord(0, Key, Request, THandlerPayloadModifier<FJsonObject, FJsonObject>(), OnSuccess, OnError);
}

void CloudSave::ReplaceGameRecordCheckLatest(int TryAttempt, FString const& Key, FJsonObject RecordRequest, THandlerPayloadModifier<FJsonObject, FJsonObject> const& PayloadModifier, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	if (TryAttempt <= 0)
	{
		OnError.ExecuteIfBound((int32)ErrorCodes::GameRecordPreconditionFailedException, "Exhaust all retry attempt to modify game record. Please try again.");
		return;
	}

	GetGameRecord(
		Key,
		THandler<FAccelByteModelsGameRecord>::CreateLambda(
			[this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError]
			(FAccelByteModelsGameRecord LatestData)
			{
				if (PayloadModifier.IsBound())
				{
					LatestData.Value = PayloadModifier.Execute(LatestData.Value);
				}

				FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
				UpdateRequest.Value = LatestData.Value;
				UpdateRequest.UpdatedAt = LatestData.UpdatedAt;

				ReplaceGameRecord(
					TryAttempt,
					Key,
					UpdateRequest,
					PayloadModifier,
					OnSuccess,
					OnError);
			}),
		FErrorHandler::CreateLambda(
			[this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError]
			(int32 Code, FString const& Message)
			{
				if (Code == (int32)ErrorCodes::GameRecordNotFoundException)
				{
					FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
					UpdateRequest.Value = RecordRequest;
					UpdateRequest.UpdatedAt = FDateTime::Now();

					ReplaceGameRecord(
						TryAttempt,
						Key,
						UpdateRequest,
						PayloadModifier,
						OnSuccess,
						OnError);
				}
				else
				{
					OnError.ExecuteIfBound(Code, Message);
				}
			}));
}

void CloudSave::DeleteGameRecord(FString const& Key, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetNamespace(), *Key);
	FString Verb = TEXT("DELETE");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FJsonObject CloudSave::CreatePlayerRecordWithMetadata(ESetByMetadataRecord SetBy, bool SetPublic, FJsonObject const& RecordRequest)
{
	FJsonObject NewRecordRequest = RecordRequest;

	const auto MetadataJson = MakeShared<FJsonObject>();
	FString SetByString = TEXT("");
	switch (SetBy)
	{
	case ESetByMetadataRecord::Server:
		SetByString = TEXT("SERVER");
		break;
	case ESetByMetadataRecord::Client:
		SetByString = TEXT("CLIENT");
		break;
	}
	MetadataJson->SetStringField(TEXT("set_by"), SetByString);
	MetadataJson->SetBoolField(TEXT("is_public"), SetPublic);
	NewRecordRequest.SetObjectField("__META", MetadataJson);

	return NewRecordRequest;
}

FJsonObject CloudSave::CreateGameRecordWithMetadata(ESetByMetadataRecord SetBy, FJsonObject const& RecordRequest)
{
	FJsonObject NewRecordRequest = RecordRequest;

	const auto MetadataJson = MakeShared<FJsonObject>();
	FString SetByString = TEXT("");
	switch (SetBy)
	{
	case ESetByMetadataRecord::Server:
		SetByString = TEXT("SERVER");
		break;
	case ESetByMetadataRecord::Client:
		SetByString = TEXT("CLIENT");
		break;
	}
	MetadataJson->SetStringField(TEXT("set_by"), SetByString);
	NewRecordRequest.SetObjectField("__META", MetadataJson);

	return NewRecordRequest;
}

} // Namespace Api
} // Namespace AccelByte
