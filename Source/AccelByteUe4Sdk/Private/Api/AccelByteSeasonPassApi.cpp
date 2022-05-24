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
		: HttpRef{InHttpRef}
		, CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
	{}

		SeasonPass::~SeasonPass()
		{}

		void SeasonPass::GetCurrentSeason(FString const& Language, THandler<FAccelByteModelsSeasonInfo> const& OnSuccess, FErrorHandler const& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
			FString Url = FString::Printf( TEXT("%s/public/namespaces/%s/seasons/current?language=%s"), *SettingsRef.SeasonPassServerUrl, *CredentialsRef.GetNamespace(), *Language);
			FString Verb = TEXT("GET");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			FString Content = TEXT("");

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Content);

			HttpRef.ProcessRequest(
				Request,
				CreateHttpResultHandler(
					THandler<FAccelByteModelsSeasonTierRewardAsJsonObject>::CreateLambda(
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
						}),
					OnError),
				FPlatformTime::Seconds());
		}

		void SeasonPass::GetUserSeason(FString const& SeasonId, THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess, FErrorHandler const& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
			FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/seasons/%s/data"), *SettingsRef.SeasonPassServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *SeasonId);
			FString Verb = TEXT("GET");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			FString Content = TEXT("");

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Content);

			HttpRef.ProcessRequest(
				Request,
				CreateHttpResultHandler(
					THandler<FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject>::CreateLambda(
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
						}),
					OnError),
				FPlatformTime::Seconds());
		}

		void SeasonPass::GetCurrentUserSeason(THandler<FAccelByteModelsUserSeasonInfo> const& OnSuccess, FErrorHandler const& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
			FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/seasons/current/data"), *SettingsRef.SeasonPassServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
			FString Verb = TEXT("GET");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			FString Content = TEXT("");

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Content);

			HttpRef.ProcessRequest(
				Request,
				CreateHttpResultHandler(
					THandler<FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject>::CreateLambda(
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
						}),
					OnError),
				FPlatformTime::Seconds());
		}

		void SeasonPass::ClaimRewards(FAccelByteModelsSeasonClaimRewardRequest const& RewardRequest, THandler<FAccelByteModelsSeasonClaimRewardResponse> const& OnSuccess, FErrorHandler const& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
			FString Url = FString::Printf(
				TEXT("%s/public/namespaces/%s/users/%s/seasons/current/rewards"), *SettingsRef.SeasonPassServerUrl,
				*CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
			FString Verb = TEXT("POST");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			FString Content = TEXT("");

			FJsonObjectConverter::UStructToJsonObjectString(RewardRequest, Content);

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Content);

			HttpRef.ProcessRequest(
				Request,
				CreateHttpResultHandler(
					THandler<FAccelByteModelsSeasonClaimRewardResponseJsonWrapper>::CreateLambda(
						[OnSuccess, this](
						FAccelByteModelsSeasonClaimRewardResponseJsonWrapper const& Response)
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
						}),
					OnError),
				FPlatformTime::Seconds());
		}

		void SeasonPass::BulkClaimRewards(THandler<FAccelByteModelsSeasonClaimRewardResponse> const& OnSuccess, FErrorHandler const& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
			FString Url = FString::Printf(
				TEXT("%s/public/namespaces/%s/users/%s/seasons/current/rewards/bulk"),
				*SettingsRef.SeasonPassServerUrl,
				*CredentialsRef.GetNamespace(),
				*CredentialsRef.GetUserId());
			FString Verb = TEXT("POST");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			FString Content = TEXT("");

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Content);

			HttpRef.ProcessRequest(
				Request,
				CreateHttpResultHandler(
					THandler<FAccelByteModelsSeasonClaimRewardResponseJsonWrapper>::CreateLambda(
						[OnSuccess, this](
						FAccelByteModelsSeasonClaimRewardResponseJsonWrapper const& Response)
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

							FAccelByteModelsSeasonClaimRewardResponse EndResult
							{
								ToClaimRewards,
								ClaimingRewards
							};

							OnSuccess.ExecuteIfBound(EndResult);
						}),
					OnError),
				FPlatformTime::Seconds());
		}

		TMap<FString, TArray<FString>> SeasonPass::FJsonObjectToPassRewards(TSharedPtr<FJsonObject> JsonObject)
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
