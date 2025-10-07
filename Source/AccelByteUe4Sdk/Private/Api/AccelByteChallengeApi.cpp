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
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{}

Challenge::~Challenge()
{
}

FAccelByteTaskWPtr Challenge::GetChallenges(THandler<FAccelByteModelsGetChallengesResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteModelsChallengeSortBy SortBy
	, EAccelByteModelsChallengeStatus Status
	, uint64 Offset
	, uint64 Limit
	, FString Keyword
	, TArray<FString> const& Tags)
{
	FReport::Log(FString(__FUNCTION__));

	TMap<FString, FString> QueryParams = {
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) },
		{ TEXT("keyword"), Keyword.IsEmpty() ? TEXT("") : Keyword }
	};

	if (SortBy != EAccelByteModelsChallengeSortBy::NONE)
	{
		QueryParams.Emplace(TEXT("sortBy"), FAccelByteUtilities::ConvertChallengeSortByToString(SortBy));
	}

	if (Status != EAccelByteModelsChallengeStatus::NONE)
	{
		QueryParams.Emplace(TEXT("status"), FAccelByteUtilities::GetUEnumValueAsString(Status));
	}

	if (Tags.Num() > 0)
	{
		FString TagsString = CreateTagsString(Tags);
		QueryParams.Emplace(TEXT("tags"), TagsString);
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/challenges")
		, *SettingsRef.ChallengeServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::GetPreviousChallengeProgresses(FString const& ChallengeCode
	, int32 Index
	, THandler<FAccelByteModelsChallengeProgressResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& GoalCode
	, uint64 Offset
	, uint64 Limit
	, TArray<FString> const& Tags)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChallengeCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("ChallengeCode cannot be empty."));
		return nullptr;
	}

	if (Index < 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Index value must be a positive integer."));
		return nullptr;
	}

	TMap<FString, FString> QueryParams{
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) },
	};

	if (Tags.Num() > 0)
	{
		FString TagsString = CreateTagsString(Tags);
		QueryParams.Emplace(TEXT("tags"), TagsString);
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/progress/%s/index/%d")
		, *SettingsRef.ChallengeServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ChallengeCode)
		, Index);

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::GetChallengeProgress(FString const& ChallengeCode
	, FString const& GoalCode
	, THandler<FAccelByteModelsChallengeProgressResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, uint64 Offset
	, uint64 Limit
	, TArray<FString> const& Tags)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChallengeCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("ChallengeCode cannot be empty."));
		return nullptr;
	}

	return GetChallengeProgress(ChallengeCode, OnSuccess, OnError, Offset, Limit, GoalCode, Tags);
}

FAccelByteTaskWPtr Challenge::GetChallengeProgress(FString const& ChallengeCode
	, THandler<FAccelByteModelsChallengeProgressResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, uint64 Offset
	, uint64 Limit
	, FString const& GoalCode
	, TArray<FString> const& Tags)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChallengeCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("ChallengeCode cannot be empty."));
		return nullptr;
	}

	TMap<FString, FString> QueryParams{
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) },
	};

	if (!GoalCode.IsEmpty())
	{
		QueryParams.Emplace(TEXT("goalCode"), GoalCode);
	}

	if (Tags.Num() > 0)
	{
		FString TagsString = CreateTagsString(Tags);
		QueryParams.Emplace(TEXT("tags"), TagsString);
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/progress/%s")
		, *SettingsRef.ChallengeServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ChallengeCode));

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
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) },
	};

	if (SortBy != EAccelByteModelsChallengeSortBy::NONE)
	{
		QueryParams.Emplace(TEXT("sortBy"), FAccelByteUtilities::ConvertChallengeSortByToString(SortBy));
	}

	if (Status != EAccelByteModelsChallengeRewardStatus::NONE)
	{
		QueryParams.Emplace(TEXT("status"), FAccelByteUtilities::GetUEnumValueAsString(Status));
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/rewards")
		, *SettingsRef.ChallengeServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::ClaimReward(FAccelByteModelsChallengeRewardClaimRequest const& Request
	, THandler<TArray<FAccelByteModelsChallengeReward>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if(Request.RewardIDs.Num() < 1)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("RewardIDs cannot be empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/rewards/claim")
		, *SettingsRef.ChallengeServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::EvaluateChallengeProgress(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	return EvaluateChallengeProgress(FAccelByteModelsChallengeEvaluateProgressOptionalParameter(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::EvaluateChallengeProgress(FAccelByteModelsChallengeEvaluateProgressOptionalParameter const& OptionalParameter
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/progress/evaluate")
		, *SettingsRef.ChallengeServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	TMap<FString, FString> QueryParams;
	TArray<FString> CodesToEvaluate;
	CodesToEvaluate.Append(OptionalParameter.ChallengeCodesToEvaluate);
	CodesToEvaluate.Append(OptionalParameter.ChallengeCodesToEvalute);

	if (CodesToEvaluate.Num() > 0)
	{
		FString CommaSeparatedChallengeCode = FString::Join(CodesToEvaluate, TEXT(","));
		QueryParams.Emplace(TEXT("challengeCode"), CommaSeparatedChallengeCode);
	}	

	return HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::ListSchedules(FString ChallengeCode, THandler<FAccelByteModelsChallengeListSchedulesResponse> const& OnSuccess, FErrorHandler const& OnError, FDateTime const& DateTime, uint64 Offset, uint64 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChallengeCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("ChallengeCode cannot be empty."));
		return nullptr;
	}

	TMap<FString, FString> QueryParams = {
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) }
	};

	if (DateTime != FDateTime::MinValue())
	{
		QueryParams.Emplace(TEXT("dateTime"), DateTime.ToIso8601());
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/challenges/%s/schedules")
		, *SettingsRef.ChallengeServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ChallengeCode));

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Challenge::ListScheduleByGoal(FString ChallengeCode, FString GoalCode, THandler<FAccelByteModelsChallengeListScheduleByGoalResponse> const& OnSuccess, FErrorHandler const& OnError, uint64 Offset, uint64 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChallengeCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("ChallengeCode cannot be empty."));
		return nullptr;
	}

	if (GoalCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("GoalCode cannot be empty."));
		return nullptr;
	}

	TMap<FString, FString> QueryParams = {
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) }
	};

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/challenges/%s/goals/%s/schedules")
		, *SettingsRef.ChallengeServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ChallengeCode)
		, *FGenericPlatformHttp::UrlEncode(GoalCode));

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FString Challenge::CreateTagsString(const TArray<FString>& Tags)
{
	FString TagsString;

	for (int64 Index = 0; Index < Tags.Num(); Index++)
	{
		TagsString.Append(Tags[Index]);
		if (Index != Tags.Num() - 1)
		{
			TagsString.AppendChar(',');
		}
	}

	return TagsString;
}

} // Namespace Api
} // Namespace AccelByte