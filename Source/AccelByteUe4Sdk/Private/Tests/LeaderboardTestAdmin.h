// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"
#include "LeaderboardTestAdmin.generated.h"

#pragma region LEADERBOARD_MODEL

USTRUCT(BlueprintType)
struct FLeaderboardDailyConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | DailyConfig")
		// FORMAT = hours:minutes EXAMPLE = ["01:30", "10:30", "15:30", "23:15" DEFAULT = "00:00"
		FString resetTime = "00:00";
};

USTRUCT(BlueprintType)
struct FLeaderboardMonthlyConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | MonthlyConfig")
		// RANGE = 1-31 DEFAULT = 1
		int resetDate = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | MonthlyConfig")
		// FORMAT = hours:minutes EXAMPLE = ["01:30", "10:30", "15:30", "23:15" DEFAULT = "00:00
		FString resetTime = "00:00";
};

USTRUCT(BlueprintType)
struct FLeaderboardWeeklyConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | WeeklyConfig")
		// RANGE = 0-6 DEFAULT = 0 (sunday)
		int resetDay = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | WeeklyConfig")
		// FORMAT = hours:minutes EXAMPLE = ["01:30", "10:30", "15:30", "23:15" DEFAULT = "00:00
		FString resetTime = "00:00";
};

USTRUCT(BlueprintType)
struct FLeaderboardConfigRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FLeaderboardDailyConfig daily; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FLeaderboardMonthlyConfig monthly; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FLeaderboardWeeklyConfig weekly; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		bool descending = true; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FString iconURL = ""; // OPTIONAL
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FString leaderboardCode; // lowercase and maximum length is 32 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FString name; // Max 128 characters
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		int seasonPeriod = 32; // OPTIONAL. Must be greater than 31 days. Will trigger seasonal leaderboard
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
		FString startTime; // RFC3339 e.g. "2020-10-02T15:00:00.05Z"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Leaderboard | ConfigRequest")
	FString statCode; // Statistic's service
};
using FLeaderboardConfigResponse = FLeaderboardConfigRequest;

#pragma endregion LEADERBOARD_MODEL

void AdminCreateLeaderboard(const FLeaderboardConfigRequest& request, const THandler<FLeaderboardConfigRequest>& OnSuccess, const FErrorHandler& OnError);
void AdminGetLeaderboardAll(const FString& leaderboardCode, const THandler<FLeaderboardConfigResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteLeaderboard(const FString& leaderboardCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
