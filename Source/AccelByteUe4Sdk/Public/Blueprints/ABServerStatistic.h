// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Models/AccelByteStatisticModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABServerStatistic.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayBulkStatItemOperationDelegate, TArray<FAccelByteModelsBulkStatItemOperationResult>, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDUserStatItemPagingSlicedDelegate, FAccelByteModelsUserStatItemPagingSlicedResult, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDGlobalStatItemDelegate, FAccelByteModelsGlobalStatItemValueResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDFetchUserStatistic, TArray<FAccelByteModelsStatItemValueResponse>, Response);

UCLASS(Blueprintable, BlueprintType)
class UABServerStatistic final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(AccelByte::FServerApiClientPtr const& NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void CreateUserStatItems(FString const & UserId, TArray<FString> const& StatCodes, FDArrayBulkStatItemOperationDelegate OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void GetAllUserStatItems(FString const& UserId, FDUserStatItemPagingSlicedDelegate OnSuccess, FDErrorHandler OnError, int32 Limit = 20, int32 Offset = 0, EAccelByteStatisticSortBy SortBy = EAccelByteStatisticSortBy::UPDATED_AT_ASC);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void GetUserStatItems(FString const& UserId, TArray<FString> const& StatCodes, TArray<FString> const& Tags, FDUserStatItemPagingSlicedDelegate OnSuccess, FDErrorHandler OnError, int32 Limit = 20, int32 Offset = 0, EAccelByteStatisticSortBy SortBy = EAccelByteStatisticSortBy::UPDATED_AT_ASC);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void BulkFetchStatItemsValue(FString const& StatCode, TArray<FString> const& UserIds, FDFetchUserStatistic OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void IncrementManyUsersStatItems(TArray<FAccelByteModelsBulkUserStatItemInc> const& Data, FDArrayBulkStatItemOperationDelegate OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void IncrementUserStatItems(FString const& UserId, TArray<FAccelByteModelsBulkStatItemInc> const& Data, FDArrayBulkStatItemOperationDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void GetGlobalStatItemsByStatCode(FString const& StatCode, FDGlobalStatItemDelegate OnSuccess, FDErrorHandler OnError);

private:
	AccelByte::FServerApiClientPtr ApiClientPtr;
};
