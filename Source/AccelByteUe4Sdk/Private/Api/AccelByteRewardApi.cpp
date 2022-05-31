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
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

Reward::~Reward()
{}
	
FString Reward::ConvertRewardSortByToString(EAccelByteRewardListSortBy const& SortBy)
{
	switch (SortBy)
	{
		case EAccelByteRewardListSortBy::NAMESPACE:
			return TEXT("namespace");
		case EAccelByteRewardListSortBy::NAMESPACE_ASC:
			return TEXT("namespace%3Aasc");
		case EAccelByteRewardListSortBy::NAMESPACE_DESC:
			return TEXT("namespace%3Adesc");
		case EAccelByteRewardListSortBy::REWARDCODE:
			return TEXT("rewardcode");
		case EAccelByteRewardListSortBy::REWARDCODE_ASC:
			return TEXT("rewardcode%3Aasc");
		case EAccelByteRewardListSortBy::REWARDCODE_DESC:
			return TEXT("rewardcode%3Adesc");
	}
	return TEXT("");
}

void Reward::GetRewardByRewardCode(FString const& RewardCode, THandler<FAccelByteModelsRewardInfo> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/rewards/byCode"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace());
	Url.Append(FString::Printf(TEXT("?rewardCode=%s"), *RewardCode));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Reward::GetRewardByRewardId(FString const& RewardId, THandler<FAccelByteModelsRewardInfo> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/rewards/%s"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace(), *RewardId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Reward::QueryRewards(FString const& EventTopic, int32 Offset, int32 Limit, EAccelByteRewardListSortBy const& SortBy, THandler<FAccelByteModelsQueryReward> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/rewards/byCriteria"), *SettingsRef.PlatformServerUrl, *CredentialsRef.GetNamespace());
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Query = TEXT("");

	if (!EventTopic.IsEmpty())
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("eventTopic=%s"), *EventTopic));
	}
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
	if (SortBy != EAccelByteRewardListSortBy::NONE)
	{
		Query.Append(Query.IsEmpty() ? TEXT("") : TEXT("&"));
		Query.Append(FString::Printf(TEXT("sortBy=%s"), *ConvertRewardSortByToString(SortBy)));
	}
	Url.Append(Query.IsEmpty() ? TEXT("") : FString::Printf(TEXT("?%s"), *Query));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
} // Namespace Api
} // Namespace AccelByte