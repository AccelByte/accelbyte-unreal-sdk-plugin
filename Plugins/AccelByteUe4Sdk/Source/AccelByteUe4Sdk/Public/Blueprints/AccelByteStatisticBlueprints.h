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

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAllUserStatItemsSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void GetAllUserStatItems(const FGetAllUserStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserStatItemsByStatCodesSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void GetUserStatItemsByStatCodes(const TArray<FString>& StatCodes, const FGetUserStatItemsByStatCodesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserStatItemsByTagsSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void GetUserStatItemsByTags(const TArray<FString>& Tags, const FGetUserStatItemsByTagsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FBulkAddStatItemValueSuccess, const TArray<FAccelByteModelsBulkStatItemOperationResult>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void BulkAddStatItemValue(const TArray<FAccelByteModelsBulkUserStatItemInc>& data, const FBulkAddStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FBulkAddUserStatItemValueSuccess, const TArray<FAccelByteModelsBulkStatItemOperationResult>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void BulkAddUserStatItemValue(const TArray<FAccelByteModelsBulkStatItemInc>& data, const FBulkAddUserStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FAddUserStatItemValueSuccess, const FAccelByteModelsStatItemIncResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
		static void AddUserStatItemValue(const FString& statCode, const float& inc, const FAddUserStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
};

