// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "Api/AccelByteSeasonPassApi.h"

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
namespace Api
{

SeasonPass::SeasonPass(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

SeasonPass::~SeasonPass()
{}

FAccelByteTaskWPtr SeasonPass::GetCurrentSeason(FString const& Language
	, THandler<FAccelByteModelsSeasonInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/seasons/current")
		, *SettingsRef.SeasonPassServerUrl
		, *CredentialsRef->GetNamespace());

	const TMultiMap<FString, FString> QueryParams = {
		{TEXT("language"), *Language}
	};

	auto OnSuccessHttpClient = THandler<FAccelByteModelsSeasonTierRewardAsJsonObject>::CreateLambda(
		[OnSuccess, this](FAccelByteModelsSeasonTierRewardAsJsonObject const& SeasonResult)
		{
			// Convert the Tier JsonObject to TMap<FString, TArray<FString>>
			TArray<FAccelByteModelsSeasonPassTier> Tiers;

			for (FAccelByteModelsSeasonPassTierJsonObject const& Tier : SeasonResult.Tiers)
			{
				FAccelByteModelsSeasonPassTier TierRes;
				TierRes.Id = Tier.Id;
				TierRes.RequiredExp = Tier.RequiredExp;
				TierRes.Rewards = FJsonObjectToPassRewards(Tier.Rewards.JsonObject);
				Tiers.Add(TierRes);
			}

			FAccelByteModelsSeasonInfo const EndResult
			{
				SeasonResult.Title,
				SeasonResult.Description,
				SeasonResult.Id,
				SeasonResult.Namespace,
				SeasonResult.Name,
				SeasonResult.Start,
				SeasonResult.End,
				SeasonResult.TierItemId,
				SeasonResult.AutoClaim,
				SeasonResult.Images,
				SeasonResult.PassCodes,
				SeasonResult.Status,
				SeasonResult.PublishedAt,
				SeasonResult.Language,
				SeasonResult.CreatedAt,
				SeasonResult.UpdatedAt,
				SeasonResult.Passes,
				SeasonResult.Rewards,
				Tiers
			};

			OnSuccess.ExecuteIfBound(EndResult);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr SeasonPass::GetUserSeason(FString const& SeasonId
	, THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/seasons/%s/data")
		, *SettingsRef.SeasonPassServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *SeasonId);

	auto OnSuccessHttpClient = THandler<FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject>::CreateLambda(
		[OnSuccess, this](FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject const& Response)
		{
			TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards;
			for (TTuple<int, FJsonObjectWrapper> const& ClaimReward : Response.ToClaimRewards)
			{
				ToClaimRewards.Add(
					ClaimReward.Key,
					FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
			}

			TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards;
			for (TTuple<int, FJsonObjectWrapper> const& ClaimReward : Response.ClaimingRewards)
			{
				ClaimingRewards.Add(
					ClaimReward.Key,
					FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
			}

			FAccelByteModelsUserSeasonInfo const EndResult
			{
				Response.Id,
				Response.Namespace,
				Response.UserId,
				Response.SeasonId,
				Response.EnrolledAt,
				Response.EnrolledPasses,
				Response.CurrentTierIndex,
				Response.LastTierIndex,
				Response.RequiredExp,
				Response.CurrentExp,
				Response.Cleared,
				Response.Season,
				ToClaimRewards,
				ClaimingRewards,
				Response.CreatedAt,
				Response.UpdatedAt
			};

			OnSuccess.ExecuteIfBound(EndResult);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr SeasonPass::GetCurrentUserSeason(THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/seasons/current/data")
		, *SettingsRef.SeasonPassServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	auto OnSuccessHttpClient = THandler<FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject>::CreateLambda(
		[OnSuccess, this](FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject const& Response)
		{
			TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards;
			for (TTuple<int, FJsonObjectWrapper> const& ClaimReward : Response.ToClaimRewards)
			{
				ToClaimRewards.Add(
					ClaimReward.Key,
					FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
			}

			TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards;
			for (TTuple<int, FJsonObjectWrapper> const& ClaimReward : Response.ClaimingRewards)
			{
				ClaimingRewards.Add(
					ClaimReward.Key,
					FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
			}

			FAccelByteModelsUserSeasonInfo const EndResult
			{
				Response.Id,
				Response.Namespace,
				Response.UserId,
				Response.SeasonId,
				Response.EnrolledAt,
				Response.EnrolledPasses,
				Response.CurrentTierIndex,
				Response.LastTierIndex,
				Response.RequiredExp,
				Response.CurrentExp,
				Response.Cleared,
				Response.Season,
				ToClaimRewards,
				ClaimingRewards,
				Response.CreatedAt,
				Response.UpdatedAt
			};

			OnSuccess.ExecuteIfBound(EndResult);
		});

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr SeasonPass::ClaimRewards(FAccelByteModelsSeasonClaimRewardRequest const& RewardRequest
	, THandler<FAccelByteModelsSeasonClaimRewardResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/seasons/current/rewards")
		, *SettingsRef.SeasonPassServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	auto OnSuccessHttpClient = THandler<FAccelByteModelsSeasonClaimRewardResponseJsonWrapper>::CreateLambda(
		[OnSuccess, this](FAccelByteModelsSeasonClaimRewardResponseJsonWrapper const& Response)
		{
			TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards;
			for (TTuple<int, FJsonObjectWrapper> const& ClaimReward : Response.ToClaimRewards)
			{
				ToClaimRewards.Add(
					ClaimReward.Key,
					FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
			}

			TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards;
			for (TTuple<int, FJsonObjectWrapper> const& ClaimReward : Response.ClaimingRewards)
			{
				ClaimingRewards.Add(
					ClaimReward.Key,
					FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
			}

			FAccelByteModelsSeasonClaimRewardResponse const EndResult
			{
				ToClaimRewards,
				ClaimingRewards
			};

			OnSuccess.ExecuteIfBound(EndResult);
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, RewardRequest, OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr SeasonPass::BulkClaimRewards(THandler<FAccelByteModelsSeasonClaimRewardResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/seasons/current/rewards/bulk")
		, *SettingsRef.SeasonPassServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	auto OnSuccessHttpClient = THandler<FAccelByteModelsSeasonClaimRewardResponseJsonWrapper>::CreateLambda(
		[OnSuccess, this](FAccelByteModelsSeasonClaimRewardResponseJsonWrapper const& Response)
		{
			TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards;
			for (TTuple<int, FJsonObjectWrapper> const& ClaimReward : Response.ToClaimRewards)
			{
				ToClaimRewards.Add(
					ClaimReward.Key,
					FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
			}

			TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards;
			for (TTuple<int, FJsonObjectWrapper> const& ClaimReward : Response.ClaimingRewards)
			{
				ClaimingRewards.Add(
					ClaimReward.Key,
					FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
			}

			const FAccelByteModelsSeasonClaimRewardResponse EndResult
			{
				ToClaimRewards,
				ClaimingRewards
			};

			OnSuccess.ExecuteIfBound(EndResult);
		});

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

TMap<FString, TArray<FString>> SeasonPass::FJsonObjectToPassRewards(TSharedPtr<FJsonObject> const& JsonObject)
{
	TMap<FString, TArray<FString>> PassAndRewardCode;
	for (TTuple<FString, TSharedPtr<FJsonValue>> const& PassReward : JsonObject->Values)
	{
		TArray<FString> ClaimRewardCode;

		for (TSharedPtr<FJsonValue> const& RewardCode : PassReward.Value->AsArray())
		{
			ClaimRewardCode.Add(RewardCode->AsString());
		}

		PassAndRewardCode.Add(PassReward.Key, ClaimRewardCode);
	}

	return PassAndRewardCode;
}
}
}
