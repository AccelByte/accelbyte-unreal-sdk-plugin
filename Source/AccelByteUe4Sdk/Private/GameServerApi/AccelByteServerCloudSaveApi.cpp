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
		: CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
		, HttpRef{InHttpRef}
	{}

	ServerCloudSave::~ServerCloudSave()
	{}

	void ServerCloudSave::RetrieveGameRecordsKey(const THandler<FAccelByteModelsPaginatedRecordsKey>& OnSuccess, const FErrorHandler& OnError, const FString& Query, int32 Offset, int32 Limit)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace());
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content = TEXT("");
		FString QueryField = TEXT("");

		if(!Query.IsEmpty())
		{
			QueryField.Append(QueryField.IsEmpty() ? TEXT("") : TEXT("&"));
			QueryField.Append(FString::Printf(TEXT("query=%s"), *Query));
		}
		if (Offset >= 0)
		{
			QueryField.Append(QueryField.IsEmpty() ? TEXT("") : TEXT("&"));
			QueryField.Append(FString::Printf(TEXT("offset=%d"), Offset));
		}
		if (Limit >= 0)
		{
			QueryField.Append(QueryField.IsEmpty() ? TEXT("") : TEXT("&"));
			QueryField.Append(FString::Printf(TEXT("limit=%d"), Limit));
		}
		Url.Append(QueryField.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"), *QueryField));

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerCloudSave::SaveGameRecord(FString const& Key, ESetByMetadataRecord SetBy, FJsonObject const& RecordRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
	{ 
		FReport::Log(FString(__FUNCTION__));

		FJsonObject NewRecordRequest = CreateGameRecordWithMetadata(SetBy, RecordRequest);
		SaveGameRecord(Key, NewRecordRequest, OnSuccess, OnError );
	}
	
	void ServerCloudSave::SaveGameRecord(const FString& Key, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *Key);
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

	void ServerCloudSave::GetGameRecord(const FString& Key, const THandler<FAccelByteModelsGameRecord>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *Key);
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
			FString SetByString;
			jsonObject.TryGetStringField("set_by", SetByString); 
			gameRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField("value", value);
			gameRecord.Value = ConvertJsonObjToJsonObjWrapper(value);
			OnSuccess.ExecuteIfBound(gameRecord);
		}), OnError),
			FPlatformTime::Seconds());
	}

	void ServerCloudSave::ReplaceGameRecord(const FString& Key, ESetByMetadataRecord SetBy, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__)); 

		FJsonObject NewRecordRequest = CreateGameRecordWithMetadata(SetBy, RecordRequest);
		ReplaceGameRecord(Key, NewRecordRequest, OnSuccess, OnError);
	}

	void ServerCloudSave::ReplaceGameRecord(const FString& Key, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *Key);
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

	void ServerCloudSave::DeleteGameRecord(const FString& Key, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/records/%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *Key);
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

	void ServerCloudSave::SaveUserRecord(const FString& Key, const FString& UserId, ESetByMetadataRecord SetBy, bool SetPublic, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FJsonObject NewRecordRequest = CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
		SaveUserRecord(Key, UserId, NewRecordRequest, OnSuccess, OnError);
	}

	void ServerCloudSave::SaveUserRecord(const FString& Key, const FString& UserId, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url             = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *Key );
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

	void ServerCloudSave::SaveUserRecord(const FString& Key, const FString& UserId, ESetByMetadataRecord SetBy, bool SetPublic, const FJsonObject& RecordRequest, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FJsonObject NewRecordRequest = bIsPublic ? RecordRequest : CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
		SaveUserRecord(Key, UserId, NewRecordRequest, bIsPublic, OnSuccess, OnError);
	}
	
	void ServerCloudSave::SaveUserRecord(const FString& Key, const FString& UserId, const FJsonObject& RecordRequest, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url             = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *Key, (bIsPublic ? TEXT("/public") : TEXT("")));
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

	void ServerCloudSave::GetUserRecord(const FString& Key, const FString& UserId, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization   = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *Key);
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
				FString SetByString;
				jsonObject.TryGetStringField("set_by", SetByString); 
				userRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);
				const TSharedPtr<FJsonObject> *value;
				jsonObject.TryGetObjectField("value", value);
				userRecord.Value = ConvertJsonObjToJsonObjWrapper(value);
				OnSuccess.ExecuteIfBound(userRecord);
			}), OnError),
			FPlatformTime::Seconds());
	}

	void ServerCloudSave::GetPublicUserRecord(const FString& Key, const FString& UserId, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s/public"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *Key);
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
			FString SetByString;
			jsonObject.TryGetStringField("set_by", SetByString); 
			userRecord.SetBy = FAccelByteUtilities::GetUEnumValueFromString<ESetByMetadataRecord>(SetByString);			
			const TSharedPtr<FJsonObject> *value;
			jsonObject.TryGetObjectField("value", value);
			userRecord.Value = ConvertJsonObjToJsonObjWrapper(value);
			OnSuccess.ExecuteIfBound(userRecord);
		}), OnError),
			FPlatformTime::Seconds());
	}

	void ServerCloudSave::ReplaceUserRecord(const FString& Key, ESetByMetadataRecord SetBy, bool SetPublic, const FString& UserId, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		const FJsonObject& NewRecordRequest = CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
		ReplaceUserRecord(Key, UserId, NewRecordRequest, OnSuccess, OnError);
	}

	void ServerCloudSave::ReplaceUserRecord(const FString& Key, const FString& UserId, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *Key);
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
	
	void ServerCloudSave::ReplaceUserRecord(const FString& Key, const FString& UserId, ESetByMetadataRecord SetBy, bool SetPublic, const FJsonObject& RecordRequest, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FJsonObject NewRecordRequest = bIsPublic ? RecordRequest : CreatePlayerRecordWithMetadata(SetBy, SetPublic, RecordRequest);
		ReplaceUserRecord(Key, UserId, NewRecordRequest, bIsPublic, OnSuccess, OnError);
	}

	void ServerCloudSave::ReplaceUserRecord(const FString& Key, const FString& UserId, const FJsonObject& RecordRequest, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *Key, (bIsPublic ? TEXT("/public") : TEXT("")));
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

	void ServerCloudSave::DeleteUserRecord(const FString& Key, const FString& UserId, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/records/%s%s"), *SettingsRef.CloudSaveServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *Key, (bIsPublic ? TEXT("/public") : TEXT("")));
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

	FJsonObject ServerCloudSave::CreatePlayerRecordWithMetadata(ESetByMetadataRecord SetBy, bool SetPublic, FJsonObject const& RecordRequest)
	{
		FJsonObject NewRecordRequest = RecordRequest;

		const auto MetadataJson = MakeShared<FJsonObject>();
		FString SetByString = FAccelByteUtilities::GetUEnumValueAsString(SetBy);
		MetadataJson->SetStringField(TEXT("set_by"), SetByString);
		MetadataJson->SetBoolField(TEXT("is_public"), SetPublic);
		NewRecordRequest.SetObjectField("__META", MetadataJson);

		return NewRecordRequest;
	}

	FJsonObject ServerCloudSave::CreateGameRecordWithMetadata(ESetByMetadataRecord SetBy, FJsonObject const& RecordRequest)
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