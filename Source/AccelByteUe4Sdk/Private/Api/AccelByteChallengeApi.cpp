// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteChallengeApi.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
namespace Api
{

Challenge::Challenge(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef
		, InSettingsRef
		, InHttpRef)
{}

Challenge::~Challenge()
{
}

void Challenge::GetChallenges(const THandler<FAccelByteModelsGetChallengesResponse>& OnSuccess
	, const FErrorHandler& OnError
	, const EAccelByteModelsChallengeSortBy& SortBy
	, const EAccelByteModelsChallengeStatus& Status
	, uint64 Offset
	, uint64 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const TMap<FString, FString> QueryParams = {
		{ TEXT("sortBy"), FAccelByteUtilities::ConvertChallengeSortByToString(SortBy) },
		{ TEXT("status"), (Status != EAccelByteModelsChallengeStatus::NONE) ? FAccelByteUtilities::GetUEnumValueAsString(Status) : FString() },
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) },
	};

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/challenges")
		, *SettingsRef.ChallengeServerUrl
		, *CredentialsRef->GetNamespace());

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Challenge::GetScheduledChallengeGoals(const FString& ChallengeCode
	, const THandler<FAccelByteModelsGetScheduledChallengeGoalsResponse>& OnSuccess
	, const FErrorHandler& OnError
	, const TArray<FString>& Tags
	, uint64 Offset
	, uint64 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	FString TagsString{};
	for (int64 Index = 0; Index < Tags.Num(); Index++)
	{
		TagsString.Append(Tags[Index]);
		if (Index != Tags.Num() - 1)
		{
			TagsString.AppendChar(',');
		}
	}

	const TMap<FString, FString> QueryParams{
		{ TEXT("tags"), FGenericPlatformHttp::UrlEncode(TagsString) },
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) },
	};

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/challenges/%s/goals")
		, *SettingsRef.ChallengeServerUrl
		, *CredentialsRef->GetNamespace()
		, *ChallengeCode);

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Challenge::GetChallengeProgress(const FString& ChallengeCode
	, const FString& GoalCode
	, const THandler<FAccelByteModelsChallengeProgressResponse>& OnSuccess
	, const FErrorHandler& OnError
	, uint64 Offset
	, uint64 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const TMap<FString, FString> QueryParams{
		{ TEXT("goalCode"), GoalCode },
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) },
	};

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/progress/%s")
		, *SettingsRef.ChallengeServerUrl
		, *CredentialsRef->GetNamespace()
		, *ChallengeCode);

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Challenge::GetRewards(const THandler<FAccelByteModelsChallengeGetRewardStatusResponse>& OnSuccess
	, const FErrorHandler& OnError
	, const EAccelByteModelsChallengeRewardStatus& Status
	, const EAccelByteModelsChallengeSortBy& SortBy
	, uint64 Offset
	, uint64 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	TMap<FString, FString> QueryParams{
		{ TEXT("sortBy"), FAccelByteUtilities::ConvertChallengeSortByToString(SortBy) },
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) },
	};

	if (Status != EAccelByteModelsChallengeRewardStatus::NONE)
	{
		QueryParams.Add(TEXT("status"), FAccelByteUtilities::GetUEnumValueAsString(Status));
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/rewards")
		, *SettingsRef.ChallengeServerUrl
		, *CredentialsRef->GetNamespace());

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Challenge::ClaimReward(const FAccelByteModelsChallengeRewardClaimRequest& Request
	, const THandler<TArray<FAccelByteModelsChallengeReward>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/rewards/claim")
		, *SettingsRef.ChallengeServerUrl
		, *CredentialsRef->GetNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

void Challenge::EvaluateChallengeProgress(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/progress/evaluate")
		, *SettingsRef.ChallengeServerUrl
		, *CredentialsRef->GetNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}


} // Namespace Api
} // Namespace AccelByte