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

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsLeaderboardRankingResultResponse, FAccelByteModelsLeaderboardRankingResult, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserRankingDataResponse, FAccelByteModelsUserRankingData, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABLeaderboard : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

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

private:
	FApiClientPtr ApiClientPtr;
};
