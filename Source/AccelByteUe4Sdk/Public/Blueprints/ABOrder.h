// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABOrder.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

USTRUCT(BlueprintType)
struct FArrayModelsOrderHistoryInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | History Info")
	TArray<FAccelByteModelsOrderHistoryInfo> Content;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccelByteModelsOrderInfoResponse, FAccelByteModelsOrderInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccelByteModelsPagedOrderInfoResponse, FAccelByteModelsPagedOrderInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayModelsOrderHistoryInfoResponse, FArrayModelsOrderHistoryInfo, Response);

#pragma endregion 

UCLASS(Blueprintable, BlueprintType)
class UABOrder : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

public: 
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api")
	void CreateNewOrder(FAccelByteModelsOrderCreate const& OrderCreateRequest, FDAccelByteModelsOrderInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api")
	void CancelOrder(FString const& OrderNo, FDAccelByteModelsOrderInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api")
	void GetUserOrder(FString const& OrderNo, FDAccelByteModelsOrderInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api")
	void GetUserOrders(int32 const& Page, int32 const& Size, FDAccelByteModelsPagedOrderInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Order | Api")
	void GetUserOrderHistory(FString const& OrderNo, FDArrayModelsOrderHistoryInfoResponse OnSuccess, FDErrorHandler OnError);

private:
	FApiClientPtr ApiClientPtr;
};
