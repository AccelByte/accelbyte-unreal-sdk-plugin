// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteAchievementApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Achievement::Achievement(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Achievement::~Achievement()
{}

FString Achievement::ConvertAchievementSortByToString(const EAccelByteAchievementListSortBy& SortBy)
{
	switch (SortBy)
	{
	case EAccelByteAchievementListSortBy::LISTORDER:
		return TEXT("listOrder");
	case EAccelByteAchievementListSortBy::LISTORDER_ASC:
		return TEXT("listOrder:asc");
	case EAccelByteAchievementListSortBy::LISTORDER_DESC:
		return TEXT("listOrder:desc");
	case EAccelByteAchievementListSortBy::CREATED_AT:
		return TEXT("createdAt");
	case EAccelByteAchievementListSortBy::CREATED_AT_ASC:
		return TEXT("createdAt:asc");
	case EAccelByteAchievementListSortBy::CREATED_AT_DESC:
		return TEXT("createdAt:desc");
	case EAccelByteAchievementListSortBy::UPDATED_AT:
		return TEXT("updatedAt");
	case EAccelByteAchievementListSortBy::UPDATED_AT_ASC:
		return TEXT("updatedAt:asc");
	case EAccelByteAchievementListSortBy::UPDATED_AT_DESC:
		return TEXT("updatedAt:desc");
	default:
		return TEXT("");
	}
}

void Achievement::QueryAchievements(
	const FString& Language,
	const EAccelByteAchievementListSortBy& SortBy,
	const THandler<FAccelByteModelsPaginatedPublicAchievement>& OnSuccess,
	const FErrorHandler& OnError,
	const int32& Offset, const int32& Limit, const FString& TagQuery)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/achievements")
		, *SettingsRef.AchievementServerUrl
		, *CredentialsRef.GetNamespace());

	const TMap<FString, FString> QueryParams = {
		{TEXT("language"), Language.IsEmpty() ? TEXT("") : Language},
		{TEXT("sortBy"), SortBy == EAccelByteAchievementListSortBy::NONE ? TEXT(""): ConvertAchievementSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("tags"), TagQuery.IsEmpty() ? TEXT("") : *TagQuery}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Achievement::GetAchievement(const FString& AchievementCode
	, const THandler<FAccelByteModelsMultiLanguageAchievement>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Achievement Code is empty."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/achievements/%s")
		, *SettingsRef.AchievementServerUrl
		, *CredentialsRef.GetNamespace()
		, *AchievementCode);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Achievement::QueryUserAchievements(
	const EAccelByteAchievementListSortBy& SortBy,
	const THandler<FAccelByteModelsPaginatedUserAchievement>& OnSuccess,
	const FErrorHandler& OnError,
	const int32& Offset, const int32& Limit, bool PreferUnlocked, const FString& TagQuery)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/achievements")
		, *SettingsRef.AchievementServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	const TMap<FString, FString> QueryParams = {
		{TEXT("sortBy"), SortBy == EAccelByteAchievementListSortBy::NONE ? TEXT(""): ConvertAchievementSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("preferUnlocked"), PreferUnlocked ? TEXT("true") : TEXT("false")},
		{TEXT("tags"), TagQuery.IsEmpty() ? TEXT("") : *TagQuery}

	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Achievement::UnlockAchievement(const FString& AchievementCode
	, const FVoidHandler OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Achievement Code is empty."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/achievements/%s/unlock")
		, *SettingsRef.AchievementServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *AchievementCode);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}
	
void Achievement::GetTags(FString const& Name, EAccelByteAchievementListSortBy const& SortBy, THandler<FAccelByteModelsPaginatedPublicTag> const& OnSuccess, FErrorHandler const& OnError, int32 const& Offset, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/tags"), *SettingsRef.AchievementServerUrl, *CredentialsRef.GetNamespace());
	const FString Verb = TEXT("GET");

	const TMap<FString, FString> QueryParams = {
		{TEXT("sortBy"), SortBy == EAccelByteAchievementListSortBy::NONE ? TEXT(""): ConvertAchievementSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
	};
	
	HttpClient.ApiRequest(Verb,Url,QueryParams,OnSuccess,OnError);
}

} // Namespace Api
} // Namespace AccelByte
