// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "Blueprints/ABSeasonPass.h"

void UABSeasonPass::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABSeasonPass::GetCurrentSeason(
	FString const& Language,
	FDModelsSeasonInfo const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->SeasonPass.GetCurrentSeason(
		Language,
		THandler<FAccelByteModelsSeasonInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsSeasonInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABSeasonPass::GetUserSeason(
	FString const& SeasonId,
	FDModelsUserSeasonInfo const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->SeasonPass.GetUserSeason(
		SeasonId,
		THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserSeasonInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABSeasonPass::GetCurrentUserSeason(
	FDModelsUserSeasonInfo const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->SeasonPass.GetCurrentUserSeason(
		THandler<FAccelByteModelsUserSeasonInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserSeasonInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABSeasonPass::ClaimRewards(
	FAccelByteModelsSeasonClaimRewardRequest const& RewardRequest,
	FDModelsSeasonClaimRewardResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->SeasonPass.ClaimRewards(
		RewardRequest,
		THandler<FAccelByteModelsSeasonClaimRewardResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsSeasonClaimRewardResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABSeasonPass::BulkClaimRewards(
	FDModelsSeasonClaimRewardResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->SeasonPass.BulkClaimRewards(
		THandler<FAccelByteModelsSeasonClaimRewardResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsSeasonClaimRewardResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}