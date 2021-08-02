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
		SeasonPass::SeasonPass(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting)
		{
		}

		SeasonPass::~SeasonPass()
		{
		}

		void SeasonPass::GetCurrentSeason(const FString& Language, const THandler<FAccelByteModelsSeasonInfo>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
			FString Url = FString::Printf( TEXT("%s/public/namespaces/%s/seasons/current?language=%s"), *Settings.SeasonPassServerUrl, *Credentials.GetNamespace(), *Language);
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

			FRegistry::HttpRetryScheduler.ProcessRequest(
				Request,
				CreateHttpResultHandler(THandler<FAccelByteModelsSeasonTierRewardAsJsonObject>::CreateLambda(
				[OnSuccess, this](const FAccelByteModelsSeasonTierRewardAsJsonObject& SeasonResult)
				{
					// Convert the Tier JsonObject to TMap<FString, TArray<FString>>
					TArray<FAccelByteModelsSeasonPassTier> Tiers;
					for (const FString& PassCode : SeasonResult.PassCodes)
					{
						for (const FAccelByteModelsSeasonPassTierJsonObject& Tier : SeasonResult.Tiers)
						{
							FAccelByteModelsSeasonPassTier TierRes;
							TierRes.Id = Tier.Id;
							TierRes.RequiredExp = Tier.RequiredExp;
							TierRes.Rewards = FJsonObjectToPassRewards(Tier.Rewards.JsonObject);
							Tiers.Add(TierRes);
						}
					}
					FAccelByteModelsSeasonInfo EndResult{
						SeasonResult.Title, SeasonResult.Description, SeasonResult.Id,
						SeasonResult.Namespace, SeasonResult.Name, SeasonResult.Start,
						SeasonResult.End, SeasonResult.TierItemId, SeasonResult.AutoClaim,
						SeasonResult.Images, SeasonResult.PassCodes,
						SeasonResult.Status, SeasonResult.PublishedAt,
						SeasonResult.Language, SeasonResult.CreatedAt,
						SeasonResult.UpdatedAt, SeasonResult.Passes, SeasonResult.Rewards,
						Tiers
					};
					OnSuccess.ExecuteIfBound(EndResult);
				}), OnError),
				FPlatformTime::Seconds());
		}

		void SeasonPass::GetUserSeason(const FString& SeasonId, const THandler<FAccelByteModelsUserSeasonInfo>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
			FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/seasons/%s/data"), *Settings.SeasonPassServerUrl, *Credentials.GetNamespace(), *Credentials.GetUserId(), *SeasonId);
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

			FRegistry::HttpRetryScheduler.ProcessRequest(
				Request,
				CreateHttpResultHandler(THandler<FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject>::CreateLambda(
				[OnSuccess, this](const FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject& Response)
				{
					TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards;
					for (const TTuple<int, FJsonObjectWrapper>& ClaimReward : Response.ToClaimRewards)
					{
						ToClaimRewards.Add(ClaimReward.Key, FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
					}

					TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards;
					for (const TTuple<int, FJsonObjectWrapper>& ClaimReward : Response.ClaimingRewards)
					{
						ClaimingRewards.Add(ClaimReward.Key, FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
					}
					FAccelByteModelsUserSeasonInfo EndResult{
						Response.Id, Response.Namespace, Response.UserId,
						Response.SeasonId, Response.EnrolledAt,
						Response.EnrolledPasses,
						Response.CurrentTierIndex, Response.LastTierIndex,
						Response.RequiredExp, Response.CurrentExp,
						Response.Cleared, Response.Season,
						ToClaimRewards, ClaimingRewards, Response.CreatedAt,
						Response.UpdatedAt
					};
					OnSuccess.ExecuteIfBound(EndResult);
				}), OnError),
				FPlatformTime::Seconds());
		}

		void SeasonPass::GetCurrentUserSeason(const THandler<FAccelByteModelsUserSeasonInfo>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
			FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/seasons/current/data"), *Settings.SeasonPassServerUrl, *Credentials.GetNamespace(), *Credentials.GetUserId());
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

			FRegistry::HttpRetryScheduler.ProcessRequest(
				Request,
				CreateHttpResultHandler(THandler<FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject>::CreateLambda(
				[OnSuccess, this](const FAccelByteModelsUserSeasonInfoClaimRewardAsJsonObject& Response)
				{
					TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards;
					for (const TTuple<int, FJsonObjectWrapper>& ClaimReward : Response.ToClaimRewards)
					{
						ToClaimRewards.Add(ClaimReward.Key, FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
					}

					TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards;
					for (const TTuple<int, FJsonObjectWrapper>& ClaimReward : Response.ClaimingRewards)
					{
						ClaimingRewards.Add(ClaimReward.Key, FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
					}
					FAccelByteModelsUserSeasonInfo EndResult{
						Response.Id, Response.Namespace, Response.UserId,
						Response.SeasonId, Response.EnrolledAt,
						Response.EnrolledPasses,
						Response.CurrentTierIndex, Response.LastTierIndex,
						Response.RequiredExp, Response.CurrentExp,
						Response.Cleared, Response.Season,
						ToClaimRewards, ClaimingRewards, Response.CreatedAt,
						Response.UpdatedAt
					};
					OnSuccess.ExecuteIfBound(EndResult);
				}), OnError),
				FPlatformTime::Seconds());
		}

		void SeasonPass::ClaimRewards(const FAccelByteModelsSeasonClaimRewardRequest& RewardRequest, const THandler<FAccelByteModelsSeasonClaimRewardResponse>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
			FString Url = FString::Printf(
				TEXT("%s/public/namespaces/%s/users/%s/seasons/current/rewards"), *Settings.SeasonPassServerUrl,
				*Credentials.GetNamespace(), *Credentials.GetUserId());
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

			FRegistry::HttpRetryScheduler.ProcessRequest(
				Request,
				CreateHttpResultHandler(THandler<FAccelByteModelsSeasonClaimRewardResponseJsonWrapper>::CreateLambda(
					[OnSuccess, this](
					const FAccelByteModelsSeasonClaimRewardResponseJsonWrapper& Response)
					{
						TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards;
						for (const TTuple<int, FJsonObjectWrapper>& ClaimReward : Response.ToClaimRewards)
						{
							ToClaimRewards.Add(ClaimReward.Key, FJsonObjectToPassRewards( ClaimReward.Value.JsonObject));
						}

						TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards;
						for (const TTuple<int, FJsonObjectWrapper>& ClaimReward : Response.ClaimingRewards)
						{
							ClaimingRewards.Add( ClaimReward.Key, FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
						}

						FAccelByteModelsSeasonClaimRewardResponse EndResult{
							ToClaimRewards, ClaimingRewards
						};
						OnSuccess.ExecuteIfBound(EndResult);
					}), OnError),
				FPlatformTime::Seconds());
		}

		void SeasonPass::BulkClaimRewards(const THandler<FAccelByteModelsSeasonClaimRewardResponse>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
			FString Url = FString::Printf(
				TEXT("%s/public/namespaces/%s/users/%s/seasons/current/rewards/bulk"), *Settings.SeasonPassServerUrl,
				*Credentials.GetNamespace(), *Credentials.GetUserId());
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

			FRegistry::HttpRetryScheduler.ProcessRequest(
				Request,
				CreateHttpResultHandler(THandler<FAccelByteModelsSeasonClaimRewardResponseJsonWrapper>::CreateLambda(
					[OnSuccess, this](
					const FAccelByteModelsSeasonClaimRewardResponseJsonWrapper& Response)
					{
						TMap<int32, TMap<FString, TArray<FString>>> ToClaimRewards;
						for (const TTuple<int, FJsonObjectWrapper>& ClaimReward : Response.ToClaimRewards)
						{
							ToClaimRewards.Add(ClaimReward.Key, FJsonObjectToPassRewards( ClaimReward.Value.JsonObject));
						}

						TMap<int32, TMap<FString, TArray<FString>>> ClaimingRewards;
						for (const TTuple<int, FJsonObjectWrapper>& ClaimReward : Response.ClaimingRewards)
						{
							ClaimingRewards.Add( ClaimReward.Key, FJsonObjectToPassRewards(ClaimReward.Value.JsonObject));
						}

						FAccelByteModelsSeasonClaimRewardResponse EndResult{
							ToClaimRewards, ClaimingRewards
						};
						OnSuccess.ExecuteIfBound(EndResult);
					}), OnError),
				FPlatformTime::Seconds());
		}

		TMap<FString, TArray<FString>> SeasonPass::FJsonObjectToPassRewards(TSharedPtr<FJsonObject> JsonObject)
		{
			// Convert the Tier JsonObject to TMap<FString, TArray<FString>>
			TMap<FString, TArray<FString>> PassAndRewardCode;
			for (const TTuple<FString, TSharedPtr<FJsonValue, ESPMode::Fast>>& PassReward : JsonObject->Values)
			{
				TArray<FString> ClaimRewardCode;
				for (const TSharedPtr<FJsonValue>& RewardCode : PassReward.Value->AsArray())
				{
					ClaimRewardCode.Add(RewardCode->AsString());
				}
				PassAndRewardCode.Add(PassReward.Key, ClaimRewardCode);
			}

			return PassAndRewardCode;
		}
	}
}
