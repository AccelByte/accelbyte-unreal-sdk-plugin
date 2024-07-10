// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "GameServerApi/AccelByteServerChallengeApi.h"
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

ServerChallenge::ServerChallenge(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{
}

ServerChallenge::~ServerChallenge()
{
}

FAccelByteTaskWPtr ServerChallenge::EvaluateChallengeProgress(FAccelByteModelsChallengeServerEvaluateProgressRequest const& Request
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/progress/evaluate")
		, *ServerSettingsRef.ChallengeServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerChallenge::GetUserRewards(const FString& UserId
	, THandler<FAccelByteModelsChallengeGetRewardStatusResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteModelsChallengeRewardStatus Status
	, EAccelByteModelsChallengeSortBy SortBy
	, uint64 Offset
	, uint64 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	TMap<FString, FString> QueryParams
	{
		{ TEXT("offset"), FString::FromInt(Offset) },
		{ TEXT("limit"), FString::FromInt(Limit) },
	};

	if (SortBy != EAccelByteModelsChallengeSortBy::NONE)
	{
		QueryParams.Add(TEXT("sortBy"), FAccelByteUtilities::ConvertChallengeSortByToString(SortBy));
	}

	if (Status != EAccelByteModelsChallengeRewardStatus::NONE)
	{
		QueryParams.Add(TEXT("status"), FAccelByteUtilities::GetUEnumValueAsString(Status));
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/rewards")
		, *ServerSettingsRef.ChallengeServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerChallenge::ClaimReward( FAccelByteModelsChallengeRewardClaimUserRequest const& Request,
	THandler<TArray<FAccelByteModelsChallengeReward>> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(Request.UserID, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(Request.UserID)
			, OnError))
	{
		return nullptr;
	}

	if(Request.RewardIDs.Num() < 1)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Reward IDs array cannot be empty!"));
		return nullptr;
	}

	FAccelByteModelsChallengeRewardClaimRequest ClaimRequest;
	ClaimRequest.RewardIDs = Request.RewardIDs;
	
	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/rewards/claim")
		, *ServerSettingsRef.ChallengeServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *Request.UserID);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerChallenge::ClaimReward(TArray<FAccelByteModelsChallengeRewardClaimUserRequest> const& Request,
	THandler<TArray<FAccelByteModelsChallengeRewardClaimUserResponse>> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if(Request.Num() < 1)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Claim rewards request array cannot be empty!"));
		return nullptr;
	}

	for(auto const& RequestItem : Request)
	{
		if (!ValidateAccelByteId(RequestItem.UserID, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(RequestItem.UserID)
			, OnError))
		{
			return nullptr;
		}
	}

	FString ItemsString;
	if(!FAccelByteUtilities::TArrayUStructToJsonString(Request, ItemsString))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Claim rewards request failed, failed to serialize request to JSON!"));
		return nullptr;
	}
	
	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/rewards/claim")
		, *ServerSettingsRef.ChallengeServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ItemsString, OnSuccess, OnError);
}
}
}
