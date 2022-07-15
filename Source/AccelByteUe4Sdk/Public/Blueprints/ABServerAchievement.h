// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABServerAchievement.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;
using namespace AccelByte::GameServerApi;

UCLASS(Blueprintable, BlueprintType)
class UABServerAchievement final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Achievement | Api")
	void UnlockAchievement(FString const& UserId, FString const& AchievementCode, FDHandler OnSuccess, FDErrorHandler OnError);

private:
	FServerApiClientPtr ApiClientPtr;
};
