// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Models/AccelByteStatisticModels.h"
#include "Core/AccelByteError.h"
#include "ABStatistic.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

USTRUCT(BlueprintType)
struct FArrayModelsBulkStatItemOperationResultResponse
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Statistic | Api | Models | StatItem")
	TArray<FAccelByteModelsBulkStatItemOperationResult> Content;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserStatItemPagingSlicedResultResponse, FAccelByteModelsUserStatItemPagingSlicedResult, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayModelsBulkStatItemOperationResultResponse, FArrayModelsBulkStatItemOperationResultResponse, Response);

#pragma endregion

UCLASS(Blueprintable, BlueprintType)
class UABStatistic : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
	void CreateUserStatItems(
		TArray<FString> const& StatCodes,
		FDArrayModelsBulkStatItemOperationResultResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
	void GetAllUserStatItems(
		FDModelsUserStatItemPagingSlicedResultResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
	void GetUserStatItems(
		TArray<FString> const& StatCodes,
		TArray<FString> const& Tags,
		FDModelsUserStatItemPagingSlicedResultResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Statistic | Api")
	void IncrementUserStatItems(
		TArray<FAccelByteModelsBulkStatItemInc> const& Data,
		FDArrayModelsBulkStatItemOperationResultResponse const& OnSuccess,
		FDErrorHandler const& OnError
	);

private:
	FApiClientPtr ApiClientPtr;
};
