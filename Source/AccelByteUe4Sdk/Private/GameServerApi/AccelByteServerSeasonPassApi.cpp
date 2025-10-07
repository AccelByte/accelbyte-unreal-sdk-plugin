// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "GameServerApi/AccelByteServerSeasonPassApi.h"

#include <string>

#include "Core/AccelByteHttpRetryScheduler.h"

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
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InServerApiClient)
{
}

ServerSeasonPass::~ServerSeasonPass()
{
}

FAccelByteTaskWPtr ServerSeasonPass::GrantExpToUser(FString const& UserId
	, int32 Exp
	, THandler<FAccelByteModelsUserSeasonInfoWithoutReward> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteSeasonPassSource Source
	, TArray<FString> const& Tags)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/exp")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

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

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSeasonPass::GrantTierToUser(FString const& UserId
	, int32 Count
	, THandler<FAccelByteModelsUserSeasonInfoWithoutReward> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteSeasonPassSource Source
	, TArray<FString> const& Tags)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/tiers")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

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

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSeasonPass::GetCurrentUserSeasonProgression(FString const& UserId
	, THandler<FAccelByteModelsUserSeasonInfoWithoutReward> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}
	
	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/current/progression")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSeasonPass::GetUserSeasonData(FString const& UserId
	, FString const& SeasonId
	, THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/%s/data")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(SeasonId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSeasonPass::GetCurrentUserSeasonHistory(FString const& UserId
	, THandler<FAccelByteModelsUserSeasonExpHistory> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/exp/history")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	const TDelegate<void(FJsonObject const&)> OnSuccessHttpClient = THandler<FJsonObject>::CreateLambda(
		[OnSuccess](FJsonObject const& JsonObject)
		{
			FAccelByteModelsUserSeasonExpHistory SessionExpHistory;
			TArray<FAccelByteModelsUserSeasonData> Data{};

			const TArray<TSharedPtr<FJsonValue>>* JsonData;
			JsonObject.TryGetArrayField(TEXT("data"), JsonData);
			for (const TSharedPtr<FJsonValue>& JsonValue : *JsonData)
			{
				FAccelByteModelsUserSeasonData UserSeasonData{};
				auto jsonObj = JsonValue->AsObject();
				jsonObj->TryGetStringField(TEXT("id"), UserSeasonData.Id);
				jsonObj->TryGetStringField(TEXT("namespace"), UserSeasonData.Namespace);
				jsonObj->TryGetStringField(TEXT("seasonId"), UserSeasonData.SeasonId);
				jsonObj->TryGetStringField(TEXT("userId"), UserSeasonData.UserId);
				jsonObj->TryGetNumberField(TEXT("grantExp"), UserSeasonData.GrantExp);
				FString Source;
				jsonObj->TryGetStringField(TEXT("source"), Source);
				UserSeasonData.Source = FAccelByteUtilities::GetUEnumValueFromString<EAccelByteSeasonPassSource>(Source);
				jsonObj->TryGetStringArrayField(TEXT("tags"), UserSeasonData.Tags);
				FString CreatedAt;
				jsonObj->TryGetStringField(TEXT("createdAt"), CreatedAt);
				FDateTime::ParseIso8601(*CreatedAt, UserSeasonData.CreatedAt);
				Data.Add(UserSeasonData);
			}
			SessionExpHistory.Data = Data;

			TSharedPtr<FJsonObject> const* pagingJsonObject;
			JsonObject.TryGetObjectField(TEXT("paging"), pagingJsonObject);
			pagingJsonObject->Get()->TryGetStringField(TEXT("previous"), SessionExpHistory.Paging.Previous);
			pagingJsonObject->Get()->TryGetStringField(TEXT("next"), SessionExpHistory.Paging.Next);

			JsonObject.TryGetNumberField(TEXT("total"), SessionExpHistory.Total);

			OnSuccess.ExecuteIfBound(SessionExpHistory);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr ServerSeasonPass::QueryUserSeasonExp(FString const& UserId
	, THandler<FAccelByteModelsQueryUserSeasonExp> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/users/%s/seasons/exp/history/tags")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerSeasonPass::BulkGetUserSessionProgression(TArray<FString> const& UserIds
	, THandler<TArray<FAccelByteModelsUserSeasonInfo>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/admin/namespaces/%s/seasons/current/users/bulk/progression")
		, *ServerSettingsRef.SeasonPassServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetClientNamespace()));

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

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte
