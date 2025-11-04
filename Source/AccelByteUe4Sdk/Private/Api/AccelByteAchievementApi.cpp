// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteAchievementApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteInstance.h"
#include "Core/AccelByteApiClient.h"

namespace AccelByte
{
namespace Api
{

Achievement::Achievement(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{
}

Achievement::Achievement(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, FAccelBytePlatformPtr const& InAccelBytePlatform)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InAccelBytePlatform)
{
}

Achievement::~Achievement()
{
}

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

FString Achievement::ConvertGlobalAchievementSortByToString(EAccelByteGlobalAchievementListSortBy const& SortBy)
{
	switch (SortBy)
	{
	case EAccelByteGlobalAchievementListSortBy::ACHIEVED_AT:
		return TEXT("achievedAt");
	case EAccelByteGlobalAchievementListSortBy::ACHIEVED_AT_ASC:
		return TEXT("achievedAt:asc");
	case EAccelByteGlobalAchievementListSortBy::ACHIEVED_AT_DESC:
		return TEXT("achievedAt:desc");
	case EAccelByteGlobalAchievementListSortBy::CREATED_AT:
		return TEXT("createdAt");
	case EAccelByteGlobalAchievementListSortBy::CREATED_AT_ASC:
		return TEXT("createdAt:asc");
	case EAccelByteGlobalAchievementListSortBy::CREATED_AT_DESC:
		return TEXT("createdAt:desc");
	default:
		return TEXT("");
	}
}

FString Achievement::ConvertGlobalAchievementTypeToString(EAccelByteGlobalAchievementStatus const& Status)
{
	switch (Status)
	{
	case EAccelByteGlobalAchievementStatus::IN_PROGRESS:
		return TEXT("in_progress");
	case EAccelByteGlobalAchievementStatus::UNLOCKED:
		return TEXT("unlocked");
	default:
		return TEXT("");
	}
}

FString Achievement::ConvertGlobalAchievementContributosSortByToString(EAccelByteGlobalAchievementContributorsSortBy const& SortBy)
{
	switch (SortBy)
	{
	case EAccelByteGlobalAchievementContributorsSortBy::CONTRIBUTED_VALUE:
		return TEXT("contributedValue");
	case EAccelByteGlobalAchievementContributorsSortBy::CONTRIBUTED_VALUE_ASC:
		return TEXT("contributedValue:asc");
	case EAccelByteGlobalAchievementContributorsSortBy::CONTRIBUTED_VALUE_DESC:
		return TEXT("contributedValue:desc");
	default:
		return TEXT("");
	}
}

FAccelByteTaskWPtr Achievement::QueryAchievements(const FString& Language
	, const EAccelByteAchievementListSortBy& SortBy
	, const THandler<FAccelByteModelsPaginatedPublicAchievement>& OnSuccess
	, const FErrorHandler& OnError
	, const int32& Offset
	, const int32& Limit
	, const FString& TagQuery
	, bool bGlobal)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/achievements")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("language"), Language.IsEmpty() ? TEXT("en-US") : Language},
		{TEXT("sortBy"), SortBy == EAccelByteAchievementListSortBy::NONE ? TEXT(""): ConvertAchievementSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("global"), bGlobal ? TEXT("true") : TEXT("false")},
		{TEXT("tags"), TagQuery.IsEmpty() ? TEXT("") : *TagQuery}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Achievement::GetAchievement(const FString& AchievementCode
	, const THandler<FAccelByteModelsMultiLanguageAchievement>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. Achievement Code is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/achievements/%s")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(AchievementCode));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Achievement::QueryUserAchievements(const EAccelByteAchievementListSortBy& SortBy
	, const THandler<FAccelByteModelsPaginatedUserAchievement>& OnSuccess
	, const FErrorHandler& OnError
	, const int32& Offset
	, const int32& Limit
	, bool PreferUnlocked
	, const FString& TagQuery)
{
	FReport::Log(FString(__FUNCTION__));

	if (SortBy == EAccelByteAchievementListSortBy::LISTORDER ||
		SortBy == EAccelByteAchievementListSortBy::LISTORDER_ASC ||
		SortBy == EAccelByteAchievementListSortBy::LISTORDER_DESC ||
		SortBy == EAccelByteAchievementListSortBy::UPDATED_AT ||
		SortBy == EAccelByteAchievementListSortBy::UPDATED_AT_ASC ||
		SortBy == EAccelByteAchievementListSortBy::UPDATED_AT_DESC)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("SortBy %s is not supported."), *FAccelByteUtilities::GetUEnumValueAsString(SortBy)));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/achievements")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId()));

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("sortBy"), SortBy == EAccelByteAchievementListSortBy::NONE ? TEXT(""): ConvertAchievementSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("preferUnlocked"), PreferUnlocked ? TEXT("true") : TEXT("false")},
		{TEXT("tags"), TagQuery.IsEmpty() ? TEXT("") : *TagQuery}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Achievement::QueryUserAchievements(EAccelByteGlobalAchievementListSortBy const& SortBy
	, THandler<FAccelByteModelsPaginatedUserAchievement> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit
	, bool PreferUnlocked
	, FString const& TagQuery)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/achievements")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId()));

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("sortBy"), SortBy == EAccelByteGlobalAchievementListSortBy::NONE ? TEXT(""): ConvertGlobalAchievementSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("preferUnlocked"), PreferUnlocked ? TEXT("true") : TEXT("false")},
		{TEXT("tags"), TagQuery.IsEmpty() ? TEXT("") : *TagQuery}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Achievement::QueryUserAchievementsByUserId(FString const& UserId,
	EAccelByteAchievementListSortBy const& SortBy,
	THandler<FAccelByteModelsPaginatedUserAchievement> const& OnSuccess,
	FErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit,
	bool PreferUnlocked,
	FString const& TagQuery)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/achievements")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("sortBy"), SortBy == EAccelByteAchievementListSortBy::NONE ? TEXT(""): ConvertAchievementSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("preferUnlocked"), PreferUnlocked ? TEXT("true") : TEXT("false")},
		{TEXT("tags"), TagQuery.IsEmpty() ? TEXT("") : *TagQuery}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Achievement::UnlockAchievement(const FString& AchievementCode
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Url is invalid. Achievement Code is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/achievements/%s/unlock")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(AchievementCode));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Achievement::BulkUnlockAchievement(FAccelByteModelsAchievementBulkUnlockRequest const& AchievementsToUnlock
	, THandler<TArray<FAccelByteModelsAchievementBulkUnlockRespone>> const& UnlockResponses
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (AchievementsToUnlock.AchievementCodes.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Request is invalid due to provided AchievementCodes is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/achievements/bulkUnlock")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId()));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, AchievementsToUnlock, UnlockResponses, OnError);
}

FAccelByteTaskWPtr Achievement::QueryGlobalAchievements(FString const& AchievementCode
	, EAccelByteGlobalAchievementStatus const& AchievementStatus
	, EAccelByteGlobalAchievementListSortBy const& SortBy
	, THandler<FAccelByteModelsPaginatedUserGlobalAchievement> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit
	, FString const& TagQuery)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/global/achievements")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("sortBy"), SortBy == EAccelByteGlobalAchievementListSortBy::NONE ? TEXT("") : ConvertGlobalAchievementSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("status"), AchievementStatus == EAccelByteGlobalAchievementStatus::NONE ? TEXT("") : ConvertGlobalAchievementTypeToString(AchievementStatus)},
		{TEXT("achievementCode"), AchievementCode.IsEmpty() ? TEXT("") : AchievementCode},
		{TEXT("tags"), TagQuery.IsEmpty() ? TEXT("") : TagQuery}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Achievement::QueryGlobalAchievementContributors(FString const& AchievementCode
	, EAccelByteGlobalAchievementContributorsSortBy const& SortBy
	, THandler<FAccelByteModelsPaginatedGlobalAchievementContributors> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, AchievementCode is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/global/achievements/%s/contributors")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(AchievementCode));

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("sortBy"), SortBy == EAccelByteGlobalAchievementContributorsSortBy::NONE ? TEXT("") : ConvertGlobalAchievementContributosSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Achievement::QueryGlobalAchievementUserContributed(FString const& AchievementCode
	, EAccelByteGlobalAchievementContributorsSortBy const& SortBy
	, THandler<FAccelByteModelsPaginatedGlobalAchievementUserContributed> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/global/achievements")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId()));

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("sortBy"), SortBy == EAccelByteGlobalAchievementContributorsSortBy::NONE ? TEXT("") : ConvertGlobalAchievementContributosSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("achievementCode"), AchievementCode.IsEmpty() ? TEXT("") : AchievementCode}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Achievement::ClaimGlobalAchievements(FString const& AchievementCode
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (AchievementCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, AchievementCode is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/global/achievements/%s/claim")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(AchievementCode));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr Achievement::GetTags(FString const& Name
	, EAccelByteAchievementListSortBy const& SortBy
	, THandler<FAccelByteModelsPaginatedPublicTag> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/tags")
		, *SettingsRef.AchievementServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));
	const FString Verb = TEXT("GET");

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("sortBy"), SortBy == EAccelByteAchievementListSortBy::NONE ? TEXT(""): ConvertAchievementSortByToString(SortBy)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
	};
	
	return HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
