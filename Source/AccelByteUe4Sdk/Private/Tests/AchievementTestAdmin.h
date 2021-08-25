// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"
#include "Models/AccelByteAchievementModels.h"
#include "AchievementTestAdmin.generated.h"

USTRUCT(BlueprintType)
struct FAchievementRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		FString AchievementCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		FString DefaultLanguage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TMap<FString, FString> Name; // "Language" : "Name"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TMap<FString, FString> Description; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TArray<FAccelByteModelsAchievementIcon> LockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TArray<FAccelByteModelsAchievementIcon> UnlockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		bool Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		bool Incremental;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		float GoalValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementRequest")
		FString StatCode;
};

USTRUCT(BlueprintType)
struct FAchievementResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString AchievementCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString DefaultLanguage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TMap<FString, FString> Name; // "Language" : "Name"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TMap<FString, FString> Description; // "Language" : "Description"
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TArray<FAccelByteModelsAchievementIcon> LockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TArray<FAccelByteModelsAchievementIcon> UnlockedIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		bool Hidden;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		int32 ListOrder;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		bool Incremental;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		float GoalValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString StatCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Achievements | AchievementResponse")
		FString UpdatedAt;
};

void AdminCreateAchievement(const FAchievementRequest& AchievementRequest, const THandler<FAchievementResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteAchievement(const FString& AchievementCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
