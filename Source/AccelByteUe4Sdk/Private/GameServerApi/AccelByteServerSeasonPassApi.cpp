// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "GameServerApi/AccelByteServerSeasonPassApi.h"

#include <string>

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
	namespace GameServerApi
	{
		ServerSeasonPass::ServerSeasonPass(ServerCredentials const& InCredentialsRef
			, ServerSettings const& InSettingsRef
			, FHttpRetryScheduler& InHttpRef)
			: CredentialsRef{InCredentialsRef}
			, SettingsRef{InSettingsRef}
			, HttpRef{InHttpRef}
		{
		}

		ServerSeasonPass::~ServerSeasonPass()
		{
		}

		void ServerSeasonPass::GrantExpToUser(const FString& UserId, int32 Exp, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess, const FErrorHandler& OnError,
			EAccelByteSeasonPassSource Source,  const TArray<FString>& Tags)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/exp"), *SettingsRef.SeasonPassServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
			FString Verb = TEXT("POST");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");

			FString Content{};
			FJsonObject DataJson;
			DataJson.SetNumberField("exp", Exp);
			DataJson.SetStringField("source", FAccelByteUtilities::GetUEnumValueAsString(Source));
			TArray<TSharedPtr<FJsonValue>> TagArray{};
			for (FString Tag : Tags)
			{
				TagArray.Add(MakeShareable(new FJsonValueString(Tag)));
			}
			DataJson.SetArrayField("tags", TagArray);
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

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}

		void ServerSeasonPass::GrantTierToUser(const FString& UserId, int32 Count, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess, const FErrorHandler& OnError,
			EAccelByteSeasonPassSource Source, const TArray<FString>& Tags)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/tiers"), *SettingsRef.SeasonPassServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
			FString Verb = TEXT("POST");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");

			FString Content{};
			FJsonObject DataJson;
			DataJson.SetNumberField("count", Count);
			DataJson.SetStringField("source", FAccelByteUtilities::GetUEnumValueAsString(Source));
			TArray<TSharedPtr<FJsonValue>> TagArray{};
			for (FString Tag : Tags)
			{
				TagArray.Add(MakeShareable(new FJsonValueString(Tag)));
			}
			DataJson.SetArrayField("tags", TagArray);
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

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}

		void ServerSeasonPass::GetCurrentUserSeasonProgression(const FString& UserId, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			
			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/progression"), *SettingsRef.SeasonPassServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
			FString Verb = TEXT("GET");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}

		void ServerSeasonPass::GetUserSeasonData(const FString& UserId, FString const& SeasonId, const THandler<FAccelByteModelsUserSeasonInfo>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/%s/data"), *SettingsRef.SeasonPassServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, *SeasonId);
			FString Verb = TEXT("GET");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}

		void ServerSeasonPass::GetCurrentUserSeasonHistory(const FString& UserId, const THandler<FAccelByteModelsUserSeasonExpHistory>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
			
			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/exp/history"), *SettingsRef.SeasonPassServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
			FString Verb = TEXT("GET");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(
				THandler<FJsonObject>::CreateLambda(
					[OnSuccess](FJsonObject const& jsonObject)
					{
						FAccelByteModelsUserSeasonExpHistory SessionExpHistory;
						TArray<FAccelByteModelsUserSeasonData> Data{};

						const TArray<TSharedPtr<FJsonValue>>* JsonData;
						jsonObject.TryGetArrayField("data", JsonData);
						for (const TSharedPtr<FJsonValue>& JsonValue : *JsonData)
						{
							FAccelByteModelsUserSeasonData UserSeasonData{};
							auto jsonObj = JsonValue->AsObject();
							jsonObj->TryGetStringField("id", UserSeasonData.Id);
							jsonObj->TryGetStringField("namespace", UserSeasonData.Namespace);
							jsonObj->TryGetStringField("seasonId", UserSeasonData.SeasonId);
							jsonObj->TryGetStringField("userId", UserSeasonData.UserId);
							jsonObj->TryGetNumberField("grantExp", UserSeasonData.GrantExp);
							FString Source;
							jsonObj->TryGetStringField("source", Source);
							UserSeasonData.Source = FAccelByteUtilities::GetUEnumValueFromString<EAccelByteSeasonPassSource>(Source);
							jsonObj->TryGetStringArrayField("tags", UserSeasonData.Tags);
							FString CreatedAt;
							jsonObj->TryGetStringField("createdAt", CreatedAt);
							FDateTime::ParseIso8601(*CreatedAt, UserSeasonData.CreatedAt);
							Data.Add(UserSeasonData);
						}
						SessionExpHistory.Data = Data;

						TSharedPtr<FJsonObject> const* pagingJsonObject;
						jsonObject.TryGetObjectField("paging", pagingJsonObject);
						pagingJsonObject->Get()->TryGetStringField(TEXT("previous"), SessionExpHistory.Paging.Previous);
						pagingJsonObject->Get()->TryGetStringField(TEXT("next"), SessionExpHistory.Paging.Next);

						jsonObject.TryGetNumberField("total", SessionExpHistory.Total);

						OnSuccess.ExecuteIfBound(SessionExpHistory);
					}),
				OnError), FPlatformTime::Seconds());
		}

		void ServerSeasonPass::QueryUserSeasonExp(const FString& UserId, const THandler<FAccelByteModelsQueryUserSeasonExp>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/exp/history/tags"), *SettingsRef.SeasonPassServerUrl, *CredentialsRef.GetClientNamespace(), *UserId);
			FString Verb = TEXT("GET");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);

			HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}
		
	}
}
