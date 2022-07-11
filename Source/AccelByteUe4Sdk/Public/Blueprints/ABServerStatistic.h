// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Models/AccelByteStatisticModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABServerStatistic.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;
using namespace AccelByte::GameServerApi;

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayBulkStatItemOperationDelegate, TArray<FAccelByteModelsBulkStatItemOperationResult>, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDUserStatItemPagingSlicedDelegate, FAccelByteModelsUserStatItemPagingSlicedResult, Response);

UCLASS(Blueprintable, BlueprintType)
class UABServerStatistic final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void CreateUserStatItems(FString const & UserId, TArray<FString> const& StatCodes, FDArrayBulkStatItemOperationDelegate OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void GetAllUserStatItems(FString const& UserId, FDUserStatItemPagingSlicedDelegate OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void GetUserStatItems(FString const& UserId, TArray<FString> const& StatCodes, TArray<FString> const& Tags, FDUserStatItemPagingSlicedDelegate OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void IncrementManyUsersStatItems(TArray<FAccelByteModelsBulkUserStatItemInc> const& Data, FDArrayBulkStatItemOperationDelegate OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Statistic | Api")
	void IncrementUserStatItems(FString const& UserId, TArray<FAccelByteModelsBulkStatItemInc> const& Data, FDArrayBulkStatItemOperationDelegate OnSuccess, FDErrorHandler OnError);

private:
	FServerApiClientPtr ApiClientPtr;
};
