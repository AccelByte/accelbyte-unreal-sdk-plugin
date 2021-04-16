// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerCloudSaveApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"
#include "Models/AccelByteCloudSaveModels.h"

namespace AccelByte
{
namespace GameServerApi
{
	ServerCloudSave::ServerCloudSave(const ServerCredentials& Credentials, const ServerSettings& Settings) : Credentials(Credentials), Settings(Settings)
	{}

	ServerCloudSave::~ServerCloudSave()
	{}

	void ServerCloudSave::RetrieveGameRecordsKey(const THandler<FAccelByteModelsPaginatedRecordsKey>& OnSuccess, const FErrorHandler& OnError, int32 Offset, int32 Limit)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace());
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content = TEXT("");
		FString Query = TEXT("");

		if (Offset >= 0)
		{
			Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
			Query.Append(FString::Printf(TEXT("offset=%d"), Offset));
		}
		if (Limit >= 0)
		{
			Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
			Query.Append(FString::Printf(TEXT("limit=%d"), Limit));
		}
		Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"), *Query));

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);

		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerCloudSave::SaveGameRecord(const FString& Key, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace(), *Key);
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

	void ServerCloudSave::GetGameRecord(const FString& Key, const THandler<FAccelByteModelsGameRecord>& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace(), *Key);
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

	void ServerCloudSave::ReplaceGameRecord(const FString& Key, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace(), *Key);
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

	void ServerCloudSave::DeleteGameRecord(const FString& Key, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace(), *Key);
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
	void ServerCloudSave::SaveUserRecord(const FString& Key, const FString& UserId, const FJsonObject& RecordRequest, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url             = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace(), *UserId, *Key, (bIsPublic ? TEXT("/public") : TEXT("")));
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

	void ServerCloudSave::GetUserRecord(const FString& Key, const FString& UserId, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace(), *UserId, *Key);
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

	void ServerCloudSave::GetPublicUserRecord(const FString& Key, const FString& UserId, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s/public"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace(), *UserId, *Key);
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

	void ServerCloudSave::ReplaceUserRecord(const FString& Key, const FString& UserId, const FJsonObject& RecordRequest, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace(), *UserId, *Key, (bIsPublic ? TEXT("/public") : TEXT("")));
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

	void ServerCloudSave::DeleteUserRecord(const FString& Key, const FString& UserId, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s"), *Settings.CloudSaveServerUrl, *Credentials.GetClientNamespace(), *UserId, *Key, (bIsPublic ? TEXT("/public") : TEXT("")));
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

} // namespace GameServerApi
} // namespace AccelByte