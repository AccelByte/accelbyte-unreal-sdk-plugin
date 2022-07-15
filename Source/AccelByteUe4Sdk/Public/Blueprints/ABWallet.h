// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABWallet.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccelByteModelsWalletInfoResponse, FAccelByteModelsWalletInfo, Response);

#pragma endregion 

UCLASS(Blueprintable, BlueprintType)
class UABWallet : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

public: 
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Wallet | Api")
	void GetWalletInfoByCurrencyCode(FString const& CurrencyCode, FDAccelByteModelsWalletInfoResponse OnSuccess, FDErrorHandler OnError);

private:
	FApiClientPtr ApiClientPtr;
};
