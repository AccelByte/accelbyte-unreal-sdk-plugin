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
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Challenge::~Challenge()
{
}

FAccelByteTaskWPtr Challenge::GetChallenges(THandler<FAccelByteModelsGetChallengesResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteModelsChallengeSortBy SortBy
	, EAccelByteModelsChallengeStatus Status
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::GetScheduledChallengeGoals(FString const& ChallengeCode
	, THandler<FAccelByteModelsGetScheduledChallengeGoalsResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, TArray<FString> Tags
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::GetChallengeProgress(FString const& ChallengeCode
	, FString const& GoalCode
	, THandler<FAccelByteModelsChallengeProgressResponse> const& OnSuccess
	, FErrorHandler const& OnError
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::GetRewards(THandler<FAccelByteModelsChallengeGetRewardStatusResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteModelsChallengeRewardStatus Status
	, EAccelByteModelsChallengeSortBy SortBy
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::ClaimReward(FAccelByteModelsChallengeRewardClaimRequest const& Request
	, THandler<TArray<FAccelByteModelsChallengeReward>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/rewards/claim")
		, *SettingsRef.ChallengeServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::EvaluateChallengeProgress(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/progress/evaluate")
		, *SettingsRef.ChallengeServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}


} // Namespace Api
} // Namespace AccelByte