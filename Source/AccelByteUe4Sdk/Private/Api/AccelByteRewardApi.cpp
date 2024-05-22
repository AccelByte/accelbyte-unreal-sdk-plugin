// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteRewardApi.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{
Reward::Reward(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Reward::~Reward()
{}
	
FString Reward::ConvertRewardSortByToString(EAccelByteRewardListSortBy SortBy)
{
	switch (SortBy)
	{
		case EAccelByteRewardListSortBy::NAMESPACE:
			return TEXT("namespace");
		case EAccelByteRewardListSortBy::NAMESPACE_ASC:
			return TEXT("namespace:asc");
		case EAccelByteRewardListSortBy::NAMESPACE_DESC:
			return TEXT("namespace:desc");
		case EAccelByteRewardListSortBy::REWARDCODE:
			return TEXT("rewardcode");
		case EAccelByteRewardListSortBy::REWARDCODE_ASC:
			return TEXT("rewardcode:asc");
		case EAccelByteRewardListSortBy::REWARDCODE_DESC:
			return TEXT("rewardcode:desc");
		default:
			return TEXT("");
	}
}

FAccelByteTaskWPtr Reward::GetRewardByRewardCode(FString const& RewardCode
	, THandler<FAccelByteModelsRewardInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/rewards/byCode")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("rewardCode"), RewardCode }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Reward::GetRewardByRewardId(FString const& RewardId
	, THandler<FAccelByteModelsRewardInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/rewards/%s")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace()
		, *RewardId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Reward::QueryRewards(FString const& EventTopic
	, int32 Offset
	, int32 Limit
	, EAccelByteRewardListSortBy const& SortBy
	, THandler<FAccelByteModelsQueryReward> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/rewards/byCriteria")
		, *SettingsRef.PlatformServerUrl
		, *CredentialsRef->GetNamespace());

	TMultiMap<FString, FString> QueryParams;
	if (!EventTopic.IsEmpty())
	{
		QueryParams.Add(TEXT("eventTopic"), *EventTopic);
	}
	if (Offset >= 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit >= 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}
	if (SortBy != EAccelByteRewardListSortBy::NONE)
	{
		QueryParams.Add(TEXT("sortBy"), ConvertRewardSortByToString(SortBy));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte