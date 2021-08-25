// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"
#include "StatisticTestAdmin.generated.h"

USTRUCT(BlueprintType)
struct FStatCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	float defaultValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	FString description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	bool incrementOnly;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	float maximum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	float minimum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	bool setAsGlobal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	EAccelByteStatisticSetBy setBy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	FString statCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Statistic | StatCreate")
	TArray<FString> tags;
};

void AdminGetStatisticByStatCode(FString statCode, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminCreateStatistic(FStatCreateRequest body, const THandler<FAccelByteModelsStatInfo>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteStatistic(const FString& statCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteStatisticItem(const FString& userId, const FString& statCode, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
