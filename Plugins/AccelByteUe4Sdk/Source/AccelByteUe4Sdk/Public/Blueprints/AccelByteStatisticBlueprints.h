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

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAllProfileStatItemsSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
	static void GetAllProfileStatItems(const FString& ProfileId, const FGetAllProfileStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetProfileStatItemsByStatCodesSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
	static void GetProfileStatItemsByStatCodes(const FString& ProfileId, const TArray<FString>& StatCodes, const FGetProfileStatItemsByStatCodesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAllUserStatItemsSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
        static void GetAllUserStatItems(const FGetAllUserStatItemsSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserStatItemsByStatCodesSuccess, const FAccelByteModelsUserStatItemPagingSlicedResult&, Result);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
        static void GetUserStatItemsByStatCodes(const TArray<FString>& StatCodes, const FGetUserStatItemsByStatCodesSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FBulkAddStatItemValueSuccess, const TArray<FAccelByteModelsBulkStatItemIncResult>&, Result);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
        static void BulkAddStatItemValue(const TArray<FAccelByteModelsBulkUserStatItemInc>& data, const FBulkAddStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FBulkAddProfileStatItemValueSuccess, const TArray<FAccelByteModelsBulkStatItemIncResult>&, Result);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
        static void BulkAddProfileStatItemValue(const FString& ProfileId, const TArray<FAccelByteModelsBulkStatItemInc>& data, const FBulkAddProfileStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FAddProfileStatItemValueSuccess, const FAccelByteModelsStatItemIncResult&, Result);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
        static void AddProfileStatItemValue(const FString& ProfileId, const FString& statCode, const float& inc, const FAddProfileStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FBulkAddUserStatItemValueSuccess, const TArray<FAccelByteModelsBulkStatItemIncResult>&, Result);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
        static void BulkAddUserStatItemValue(const TArray<FAccelByteModelsBulkStatItemInc>& data, const FBulkAddUserStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

    DECLARE_DYNAMIC_DELEGATE_OneParam(FAddUserStatItemValueSuccess, const FAccelByteModelsStatItemIncResult&, Result);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
        static void AddUserStatItemValue(const FString& statCode, const float& inc, const FAddUserStatItemValueSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);
};

