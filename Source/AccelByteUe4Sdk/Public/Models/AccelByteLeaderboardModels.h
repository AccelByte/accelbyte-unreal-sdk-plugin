// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
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

USTRUCT(BlueprintType)
struct FAccelByteModelsLeaderboardData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	bool Descending{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	FString IconUrl{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	bool IsDeleted{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	FString LeaderboardCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	FString Description{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	FString StatCode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	TArray<FString> CyleIds{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	bool AllTime{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	FDateTime CreatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | LeaderboardData")
	FDateTime DeletedAt{0};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsPaginatedLeaderboardData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | PaginatedLeaderboardData")
	TArray<FAccelByteModelsLeaderboardData> Data{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | PaginatedLeaderboardData")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsUserPointV3
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserPointV3")
	bool Hidden{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserPointV3")
	float Point{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserPointV3")
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserPointV3")
	FJsonObjectWrapper AdditionalData{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLeaderboardRankingResultV3
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | RankingResultV3")
	TArray<FAccelByteModelsUserPoint> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | RankingResultV3")
	FAccelByteModelsPaging Paging{};
};


USTRUCT(BlueprintType)
struct FAccelByteModelsUserRankingV3
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingV3")
	float Point{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingV3")
	int Rank{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingV3")
	bool Hidden{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingV3")
	FJsonObjectWrapper AdditionalData{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsCycleRank
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | CycleRank")
	FJsonObjectWrapper AdditionalData{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | CycleRank")
	bool Hidden{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | CycleRank")
	float Point{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | CycleRank")
	int Rank{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | CycleRank")
	FString CycleId{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsUserRankingDataV3
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingDataV3")
	FAccelByteModelsUserRankingV3 AllTime{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | UserRankingDataV3")
	TArray<FAccelByteModelsCycleRank> Cycles{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsBulkUserRankingDataValueV3 : public FAccelByteModelsUserRankingDataV3
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | BulkUserRankingDataValueV3")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsBulkUserRankingDataV3
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | BulkUserRankingDataV3")
	TArray<FAccelByteModelsBulkUserRankingDataValueV3> Data{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsBulkUserRankingDataRequestV3
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Leaderboard | Models | BulkUserRankingDataRequestV3")
	TArray<FString> UserIds{};
};
