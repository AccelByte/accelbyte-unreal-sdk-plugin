// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#include "Blueprints/ABLeaderboard.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLeaderboard, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteLeaderboard);

using namespace AccelByte;

void UABLeaderboard::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABLeaderboard::GetRankings(FString const& LeaderboardCode
	, EAccelByteLeaderboardTimeFrame const& TimeFrame
	, int32 Offset
	, int32 Limit
	, FDModelsLeaderboardRankingResultResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	auto Leaderboard = ApiClientPtr->GetLeaderboardApi().Pin();
	if (Leaderboard.IsValid())
	{
		Leaderboard->GetRankings(LeaderboardCode
			, TimeFrame
			, Offset
			, Limit
			, THandler<FAccelByteModelsLeaderboardRankingResult>::CreateLambda(
				[OnSuccess](FAccelByteModelsLeaderboardRankingResult const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		UE_LOG(LogAccelByteLeaderboard, Warning, TEXT("Invalid Leaderboard API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Leaderboard API already destroyed!"));
	}
}

void UABLeaderboard::GetUserRanking(FString const& UserId
	, FString const& LeaderboardCode
	, FDModelsUserRankingDataResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	auto Leaderboard = ApiClientPtr->GetLeaderboardApi().Pin();
	if (Leaderboard.IsValid())
	{
		Leaderboard->GetUserRanking(UserId
			, LeaderboardCode
			, THandler<FAccelByteModelsUserRankingData>::CreateLambda(
				[OnSuccess](FAccelByteModelsUserRankingData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		UE_LOG(LogAccelByteLeaderboard, Warning, TEXT("Invalid Leaderboard API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Leaderboard API already destroyed!"));
	}
}

void UABLeaderboard::GetLeaderboards(int32 Offset
	, int32 Limit
	, FDModelsPaginatedLeaderboardData const& OnSuccess
	, FDErrorHandler const& OnError)
{
	GetLeaderboardsV3(OnSuccess, OnError, Limit, Offset);
}

void UABLeaderboard::GetLeaderboardsV3(FDModelsPaginatedLeaderboardData const& OnSuccess
	, FDErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	auto Leaderboard = ApiClientPtr->GetLeaderboardApi().Pin();
	if (Leaderboard.IsValid())
	{
		Leaderboard->GetLeaderboardsV3(THandler<FAccelByteModelsPaginatedLeaderboardData>::CreateLambda([OnSuccess](FAccelByteModelsPaginatedLeaderboardData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				})
			, Limit
			, Offset);
	}
	else
	{
		UE_LOG(LogAccelByteLeaderboard, Warning, TEXT("Invalid Leaderboard API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Leaderboard API already destroyed!"));
	}
}

void UABLeaderboard::GetLeaderboardV3(FString const& LeaderboardCode
	, FDModelsBaseLeaderboardData const& OnSuccess
	, FDErrorHandler const& OnError)
{
	auto Leaderboard = ApiClientPtr->GetLeaderboardApi().Pin();
	if (Leaderboard.IsValid())
	{
		Leaderboard->GetLeaderboardV3(LeaderboardCode
			, THandler<FAccelByteModelsBaseLeaderboardData>::CreateLambda([OnSuccess](FAccelByteModelsBaseLeaderboardData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		UE_LOG(LogAccelByteLeaderboard, Warning, TEXT("Invalid Leaderboard API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Leaderboard API already destroyed!"));
	}
}

void UABLeaderboard::GetRankingsV3(FString const& LeaderboardCode
	, int32 Offset
	, int32 Limit
	, FDModelsLeaderboardRankingResultV3 const& OnSuccess
	, FDErrorHandler const& OnError)
{
	auto Leaderboard = ApiClientPtr->GetLeaderboardApi().Pin();
	if (Leaderboard.IsValid())
	{
		Leaderboard->GetRankingsV3(LeaderboardCode
			, Offset
			, Limit
			, THandler<FAccelByteModelsLeaderboardRankingResultV3>::CreateLambda([OnSuccess](FAccelByteModelsLeaderboardRankingResultV3 const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		UE_LOG(LogAccelByteLeaderboard, Warning, TEXT("Invalid Leaderboard API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Leaderboard API already destroyed!"));
	}
}

void UABLeaderboard::GetRankingByCycle(FString const& LeaderboardCode
	, FString const& CycleId
	, int32 Offset
	, int32 Limit
	, FDModelsLeaderboardRankingResultV3 const& OnSuccess
	, FDErrorHandler const& OnError)
{
	GetRankingByCycleV3(LeaderboardCode, CycleId, OnSuccess, OnError, Limit, Offset);
}

void UABLeaderboard::GetRankingByCycleV3(FString const& LeaderboardCode
	, FString const& CycleId
	, FDModelsLeaderboardRankingResultV3 const& OnSuccess
	, FDErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	auto Leaderboard = ApiClientPtr->GetLeaderboardApi().Pin();
	if (Leaderboard.IsValid())
	{
		Leaderboard->GetRankingByCycleV3(LeaderboardCode
			, CycleId
			, THandler<FAccelByteModelsLeaderboardRankingResultV3>::CreateLambda([OnSuccess](FAccelByteModelsLeaderboardRankingResultV3 const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				})
			, Limit
			, Offset);
	}
	else
	{
		UE_LOG(LogAccelByteLeaderboard, Warning, TEXT("Invalid Leaderboard API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Leaderboard API already destroyed!"));
	}
}

void UABLeaderboard::GetUserRankingV3(FString const& UserId
	, FString const& LeaderboardCode
	, FDModelsUserRankingDataV3 const& OnSuccess
	, FDErrorHandler const& OnError)
{
	auto Leaderboard = ApiClientPtr->GetLeaderboardApi().Pin();
	if (Leaderboard.IsValid())
	{
		Leaderboard->GetUserRankingV3(UserId
			, LeaderboardCode
			, THandler<FAccelByteModelsUserRankingDataV3>::CreateLambda([OnSuccess](FAccelByteModelsUserRankingDataV3 const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		UE_LOG(LogAccelByteLeaderboard, Warning, TEXT("Invalid Leaderboard API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Leaderboard API already destroyed!"));
	}
}

void UABLeaderboard::GetBulkUserRankingV3(TArray<FString> const& UserIds
	, FString const& LeaderboardCode
	, FDModelsBulkUserRankingDataV3 const& OnSuccess
	, FDErrorHandler const& OnError)
{
	auto Leaderboard = ApiClientPtr->GetLeaderboardApi().Pin();
	if (Leaderboard.IsValid())
	{
		Leaderboard->GetBulkUserRankingV3(UserIds
			, LeaderboardCode
			, THandler<FAccelByteModelsBulkUserRankingDataV3>::CreateLambda([OnSuccess](FAccelByteModelsBulkUserRankingDataV3 const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		UE_LOG(LogAccelByteLeaderboard, Warning, TEXT("Invalid Leaderboard API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Leaderboard API already destroyed!"));
	}
}


