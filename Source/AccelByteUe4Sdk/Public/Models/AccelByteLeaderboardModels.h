// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteLeaderboardModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteLeaderboardTimeFrame : uint8
{
	ALL_TIME,
	CURRENT_SEASON,
	CURRENT_MONTH,
	CURRENT_WEEK,
	TODAY
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserPoint")
	float point{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserPoint")
	FString userId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLeaderboardRankingResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | RankingResult")
	TArray<FAccelByteModelsUserPoint> data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | RankingResult")
	FAccelByteModelsPaging paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserRanking
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRanking")
	float point{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRanking")
	int rank{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserRankingData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingData")
	FAccelByteModelsUserRanking allTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingData")
	FAccelByteModelsUserRanking current{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingData")
	FAccelByteModelsUserRanking daily{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingData")
	FAccelByteModelsUserRanking monthly{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingData")
	FAccelByteModelsUserRanking weekly{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingData")
	FString userId{};
};