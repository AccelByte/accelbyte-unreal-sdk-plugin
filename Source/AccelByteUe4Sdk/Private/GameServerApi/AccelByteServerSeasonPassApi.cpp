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
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
}

ServerSeasonPass::~ServerSeasonPass()
{
}

void ServerSeasonPass::GrantExpToUser(const FString& UserId
	, int32 Exp
	, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess
	, const FErrorHandler& OnError
	, EAccelByteSeasonPassSource Source
	, const TArray<FString>& Tags)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/exp")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

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
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void ServerSeasonPass::GrantTierToUser(const FString& UserId
	, int32 Count
	, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess
	, const FErrorHandler& OnError
	, EAccelByteSeasonPassSource Source
	, const TArray<FString>& Tags)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/tiers")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

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
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void ServerSeasonPass::GetCurrentUserSeasonProgression(const FString& UserId
	, const THandler<FAccelByteModelsUserSeasonInfoWithoutReward>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/progression")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerSeasonPass::GetUserSeasonData(const FString& UserId
	, FString const& SeasonId
	, const THandler<FAccelByteModelsUserSeasonInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/%s/data")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *SeasonId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerSeasonPass::GetCurrentUserSeasonHistory(const FString& UserId
	, const THandler<FAccelByteModelsUserSeasonExpHistory>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/exp/history")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JsonObject)
		{
			FAccelByteModelsUserSeasonExpHistory SessionExpHistory;
			TArray<FAccelByteModelsUserSeasonData> Data{};

			const TArray<TSharedPtr<FJsonValue>>* JsonData;
			JsonObject.TryGetArrayField("data", JsonData);
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
			JsonObject.TryGetObjectField("paging", pagingJsonObject);
			pagingJsonObject->Get()->TryGetStringField(TEXT("previous"), SessionExpHistory.Paging.Previous);
			pagingJsonObject->Get()->TryGetStringField(TEXT("next"), SessionExpHistory.Paging.Next);

			JsonObject.TryGetNumberField("total", SessionExpHistory.Total);

			OnSuccess.ExecuteIfBound(SessionExpHistory);
		});

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

void ServerSeasonPass::QueryUserSeasonExp(const FString& UserId
	, const THandler<FAccelByteModelsQueryUserSeasonExp>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/exp/history/tags")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerSeasonPass::BulkGetUserSessionProgression(const TArray<FString>& UserIds
	, const THandler<TArray<FAccelByteModelsUserSeasonInfo>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/seasons/current/users/bulk/progression")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	FString Content{};
	FJsonObject DataJson; 
	TArray<TSharedPtr<FJsonValue>> UserIdArray{};
	for (FString UserId : UserIds)
	{
		UserIdArray.Add(MakeShareable(new FJsonValueString(UserId)));
	}
	DataJson.SetArrayField("userIds", UserIdArray);
	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(DataJson);
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

}
}
