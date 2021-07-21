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

	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateUserStatItemsSuccess, const TArray<FAccelByteModelsBulkStatItemOperationResult>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void CreateUserStatItems(const TArray<FString>& StatCodes, const FCreateUserStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAllUserStatItemsSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void GetAllUserStatItems(const FGetAllUserStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserStatItemsSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void GetUserStatItems(const TArray<FString>& StatCodes, const TArray<FString>& Tags, const FGetUserStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FBulkAddUserStatItemValueSuccess, const TArray<FAccelByteModelsBulkStatItemOperationResult>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void IncrementUserStatItems(const TArray<FAccelByteModelsBulkStatItemInc>& Data, const FBulkAddUserStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
};

