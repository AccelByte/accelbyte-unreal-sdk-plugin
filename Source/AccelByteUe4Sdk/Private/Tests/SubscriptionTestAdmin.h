// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "EcommerceTestAdmin.h"
#include "TestUtilities.h"
#include "SubscriptionTestAdmin.generated.h"

USTRUCT(BlueprintType)
struct FFreeSubscriptionRequest 
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
	FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
	int32 GrantDays;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
	FString Source;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
	FString Reason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
	FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Subscriptions | SubscriptionRequest")
	FString Language;
};

void AdminGrantSubscriptionFree(const FString& UserId, const FFreeSubscriptionRequest& BodyRequest, const THandler<FItemFullInfo>& OnSuccess, const FErrorHandler& OnError);
