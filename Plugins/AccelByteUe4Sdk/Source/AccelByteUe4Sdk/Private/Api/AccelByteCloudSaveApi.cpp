// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteCloudSaveApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
CloudSave::CloudSave(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting){}

CloudSave::~CloudSave(){}

void CloudSave::SaveUserRecord(const FString& Key, FJsonObject RecordRequest, bool IsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s%s"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *Key, (IsPublic ? TEXT("/public") : TEXT("")));
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request,  CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void CloudSave::GetUserRecord(const FString& Key, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *Key);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(
		Request,
		CreateHttpResultHandler(THandler<FJsonObject>::CreateLambda([OnSuccess](const FJsonObject& jsonObject)
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
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField("value", value);
			userRecord.Value = *value->ToSharedRef();
			OnSuccess.ExecuteIfBound(userRecord);
		}), OnError),
		FPlatformTime::Seconds());
}

void CloudSave::GetPublicUserRecord(const FString& Key, const FString& UserId, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s/public"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *UserId, *Key);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(
		Request,
		CreateHttpResultHandler(THandler<FJsonObject>::CreateLambda([OnSuccess](const FJsonObject& jsonObject)
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
		const TSharedPtr<FJsonObject> *value;
		jsonObject.TryGetObjectField("value", value);
		userRecord.Value = *value->ToSharedRef();
		OnSuccess.ExecuteIfBound(userRecord);
	}), OnError),
		FPlatformTime::Seconds());
}

void CloudSave::ReplaceUserRecord(const FString& Key, FJsonObject RecordRequest, bool IsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s%s"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *Key, (IsPublic ? TEXT("/public") : TEXT("")));
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void CloudSave::ReplaceUserRecord(int TryAttempt, const FString& Key, const FAccelByteModelsConcurrentReplaceRequest& Data, const THandlerPayloadModifier<FJsonObject, FJsonObject>& PayloadModifier, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/concurrent/records/%s/public"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *Key);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, FErrorHandler::CreateLambda([this, TryAttempt, Key, Data, PayloadModifier, OnSuccess, OnError](int32 Code, const FString& Message)
	{
		if (Code == (int32)ErrorCodes::PlayerRecordPreconditionFailedException)
		{
			if (TryAttempt > 0)
			{
				CloudSave::ReplaceUserRecordCheckLatest(TryAttempt - 1, Key, Data.Value, PayloadModifier, OnSuccess, OnError);
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
	})), FPlatformTime::Seconds());
}

void CloudSave::ReplaceUserRecordCheckLatest(const FString& Key, const FDateTime LastUpdated, FJsonObject RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FAccelByteModelsConcurrentReplaceRequest Request
	{
		LastUpdated,
		RecordRequest
	};

	CloudSave::ReplaceUserRecord(0, Key, Request, THandlerPayloadModifier<FJsonObject, FJsonObject>(), OnSuccess, OnError);
}

void CloudSave::ReplaceUserRecordCheckLatest(int TryAttempt, const FString& Key, FJsonObject RecordRequest, const THandlerPayloadModifier<FJsonObject, FJsonObject>& PayloadModifier, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	if (TryAttempt <= 0)
	{
		OnError.ExecuteIfBound((int32)ErrorCodes::PlayerRecordPreconditionFailedException, "Exhaust all retry attempt to modify game record. Please try again.");
		return;
	}

	GetUserRecord(Key, THandler<FAccelByteModelsUserRecord>::CreateLambda([this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError](FAccelByteModelsUserRecord LatestData)
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
	}), FErrorHandler::CreateLambda([this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError](int32 Code, const FString& Message)
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

void CloudSave::DeleteUserRecord(const FString& Key, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/users/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *Key);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void CloudSave::SaveGameRecord(const FString& Key, FJsonObject RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Key);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void CloudSave::GetGameRecord(const FString& Key, const THandler<FAccelByteModelsGameRecord>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Key);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(
		Request,
		CreateHttpResultHandler(THandler<FJsonObject>::CreateLambda([OnSuccess](const FJsonObject& jsonObject)
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
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField("value", value);
			gameRecord.Value = *value->ToSharedRef();
			OnSuccess.ExecuteIfBound(gameRecord);
		}), OnError),
		FPlatformTime::Seconds());
}

void CloudSave::ReplaceGameRecord(const FString& Key, FJsonObject RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Key);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void CloudSave::ReplaceGameRecord(int TryAttempt, const FString& Key, const FAccelByteModelsConcurrentReplaceRequest& Data, const THandlerPayloadModifier<FJsonObject, FJsonObject>& PayloadModifier, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/concurrent/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Key);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, FErrorHandler::CreateLambda([this, TryAttempt, Key, Data, PayloadModifier, OnSuccess, OnError](int32 Code, const FString& Message)
	{
		if (Code == (int32)ErrorCodes::GameRecordPreconditionFailedException)
		{
			if (TryAttempt > 0)
			{
				CloudSave::ReplaceGameRecordCheckLatest(TryAttempt - 1, Key, Data.Value, PayloadModifier, OnSuccess, OnError);
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
	})), FPlatformTime::Seconds());
}

void CloudSave::ReplaceGameRecordCheckLatest(const FString& Key, const FDateTime LastUpdated, FJsonObject RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FAccelByteModelsConcurrentReplaceRequest Request
	{
		LastUpdated,
		RecordRequest
	};

	CloudSave::ReplaceGameRecord(0, Key, Request, THandlerPayloadModifier<FJsonObject, FJsonObject>(), OnSuccess, OnError);
}

void CloudSave::ReplaceGameRecordCheckLatest(int TryAttempt, const FString& Key, FJsonObject RecordRequest, const THandlerPayloadModifier<FJsonObject, FJsonObject>& PayloadModifier, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	if (TryAttempt <= 0)
	{
		OnError.ExecuteIfBound((int32)ErrorCodes::GameRecordPreconditionFailedException, "Exhaust all retry attempt to modify game record. Please try again.");
		return;
	}

	GetGameRecord(Key, THandler<FAccelByteModelsGameRecord>::CreateLambda([this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError](FAccelByteModelsGameRecord LatestData)
	{
		if (PayloadModifier.IsBound())
		{
			LatestData.Value = PayloadModifier.Execute(LatestData.Value);
		}

		FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
		UpdateRequest.Value = LatestData.Value;
		UpdateRequest.UpdatedAt = LatestData.UpdatedAt;

		CloudSave::ReplaceGameRecord(
			TryAttempt,
			Key,
			UpdateRequest,
			PayloadModifier,
			OnSuccess,
			OnError);
	}), FErrorHandler::CreateLambda([this, TryAttempt, PayloadModifier, Key, RecordRequest, OnSuccess, OnError](int32 Code, const FString& Message)
	{
		if (Code == (int32)ErrorCodes::GameRecordNotFoundException)
		{
			FAccelByteModelsConcurrentReplaceRequest UpdateRequest;
			UpdateRequest.Value = RecordRequest;
			UpdateRequest.UpdatedAt = FDateTime::Now();

			CloudSave::ReplaceGameRecord(
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

void CloudSave::DeleteGameRecord(const FString& Key, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetUserNamespace(), *Key);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
