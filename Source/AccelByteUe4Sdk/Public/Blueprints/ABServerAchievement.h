// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteServerApiClient.h"
#include "ABServerAchievement.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UABServerAchievement final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(AccelByte::FServerApiClientPtr const& NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Achievement | Api")
	void UnlockAchievement(FString const& UserId, FString const& AchievementCode, FDHandler OnSuccess, FDErrorHandler OnError);

private:
	AccelByte::FServerApiClientPtr ApiClientPtr;
};
