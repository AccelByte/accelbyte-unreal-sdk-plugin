// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLeaderboardModels.h"
#include "ABLeaderboard.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsLeaderboardRankingResultResponse, FAccelByteModelsLeaderboardRankingResult, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserRankingDataResponse, FAccelByteModelsUserRankingData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedLeaderboardData, FAccelByteModelsPaginatedLeaderboardData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsLeaderboardRankingResultV3, FAccelByteModelsLeaderboardRankingResultV3, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserRankingDataV3, FAccelByteModelsUserRankingDataV3, Response);
#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABLeaderboard : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Leaderboard | Api")
	void GetRankings(
		FString const& LeaderboardCode,
		EAccelByteLeaderboardTimeFrame const& TimeFrame,
		int32 Offset,
		int32 Limit,
		FDModelsLeaderboardRankingResultResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Leaderboard | Api")
	void GetUserRanking(
		FString const& UserId,
		FString const& LeaderboardCode,
		FDModelsUserRankingDataResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

	UFUNCTION(BlueprintCallable, Category= "AccelByte | Leaderboard | Api")
	void GetLeaderboards(
		int32 Offset,
		int32 Limit,
		FDModelsPaginatedLeaderboardData const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category= "AccelByte | Leaderboard | Api")
	void GetRankingsV3(FString const& LeaderboardCode,
		int32 Offset,
		int32 Limit,
		FDModelsLeaderboardRankingResultV3 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category= "AccelByte | Leaderboard | Api")
	void GetRankingByCycle(FString const& LeaderboardCode,
		FString const& CycleId,
		int32 Offset,
		int32 Limit,
		FDModelsLeaderboardRankingResultV3 const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category= "AccelByte | Leaderboard | Api")
	void GetUserRankingV3(FString const& UserId,
		FString const& LeaderboardCode,
		FDModelsUserRankingDataV3 const& OnSuccess,
		FDErrorHandler const& OnError);

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};
