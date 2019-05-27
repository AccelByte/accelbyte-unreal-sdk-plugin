// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Models/AccelByteStatisticModels.h"
#include "AccelByteStatisticBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsStatistic : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAllStatItemsSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
	static void GetAllStatItems(const FString& ProfileId, const FGetAllStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetStatItemsByStatCodesSuccess, const TArray<FAccelByteModelsUserStatItemInfo>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
	static void GetStatItemsByStatCodes(const FString& ProfileId, TArray<FString> StatCodes, const FGetStatItemsByStatCodesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

};

