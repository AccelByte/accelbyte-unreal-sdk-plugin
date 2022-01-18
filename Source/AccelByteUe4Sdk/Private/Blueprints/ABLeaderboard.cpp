// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABLeaderboard.h"

void UABLeaderboard::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABLeaderboard::GetRankings(
	FString const& LeaderboardCode,
	EAccelByteLeaderboardTimeFrame const& TimeFrame,
	int32 Offset,
	int32 Limit,
	FDModelsLeaderboardRankingResultResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->Leaderboard.GetRankings(
		LeaderboardCode,
		TimeFrame,
		Offset,
		Limit,
		THandler<FAccelByteModelsLeaderboardRankingResult>::CreateLambda(
			[OnSuccess](FAccelByteModelsLeaderboardRankingResult const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABLeaderboard::GetUserRanking(
	FString const& UserId,
	FString const& LeaderboardCode,
	FDModelsUserRankingDataResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->Leaderboard.GetUserRanking(
		UserId,
		LeaderboardCode,
		THandler<FAccelByteModelsUserRankingData>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserRankingData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}