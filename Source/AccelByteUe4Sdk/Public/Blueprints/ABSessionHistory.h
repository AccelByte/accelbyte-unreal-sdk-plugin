// Copyright (c) 2023 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteSessionModels.h"
#include "ABSessionHistory.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsV2SessionHistoriesResult, FAccelByteModelsGameSessionHistoriesResult, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABSessionHistory : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

#pragma region V1 SessionHistory

	UFUNCTION(BlueprintCallable, Category = "AccelByte | SessionHistory | Api")
	void QueryGameSessionHistory(
		FDModelsV2SessionHistoriesResult const& OnSuccess,
		FDErrorHandler const& OnError,
		EAccelByteGeneralSortBy const& SortBy = EAccelByteGeneralSortBy::DESC,
		int32 Offset = 0,
		int32 Limit = 20
	);

#pragma endregion

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};