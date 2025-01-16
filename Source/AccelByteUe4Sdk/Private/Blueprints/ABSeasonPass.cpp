// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "Blueprints/ABSeasonPass.h"

using namespace AccelByte;

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
	const auto SeasonPassPtr = ApiClientPtr->GetSeasonPassApi().Pin();
	if (SeasonPassPtr.IsValid())
	{
		SeasonPassPtr->GetCurrentSeason(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABSeasonPass::GetUserSeason(
	FString const& SeasonId,
	FDModelsUserSeasonInfo const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto SeasonPassPtr = ApiClientPtr->GetSeasonPassApi().Pin();
	if (SeasonPassPtr.IsValid())
	{
		SeasonPassPtr->GetUserSeason(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABSeasonPass::GetCurrentUserSeason(
	FDModelsUserSeasonInfo const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto SeasonPassPtr = ApiClientPtr->GetSeasonPassApi().Pin();
	if (SeasonPassPtr.IsValid())
	{
		SeasonPassPtr->GetCurrentUserSeason(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABSeasonPass::ClaimRewards(
	FAccelByteModelsSeasonClaimRewardRequest const& RewardRequest,
	FDModelsSeasonClaimRewardResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto SeasonPassPtr = ApiClientPtr->GetSeasonPassApi().Pin();
	if (SeasonPassPtr.IsValid())
	{
		SeasonPassPtr->ClaimRewards(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABSeasonPass::BulkClaimRewards(
	FDModelsSeasonClaimRewardResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto SeasonPassPtr = ApiClientPtr->GetSeasonPassApi().Pin();
	if (SeasonPassPtr.IsValid())
	{
		SeasonPassPtr->BulkClaimRewards(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}