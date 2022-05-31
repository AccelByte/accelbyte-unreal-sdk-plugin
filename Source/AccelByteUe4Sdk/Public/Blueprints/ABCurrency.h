#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABCurrency.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;

USTRUCT(BlueprintType)
struct FArrayModelsCurrencyList
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TArray<FAccelByteModelsCurrencyList> Content;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayModelsCurrencyListResponse, FArrayModelsCurrencyList, Response);

UCLASS(Blueprintable, BlueprintType)
class UABCurrency : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Currency")
	void GetCurrencyList(FDArrayModelsCurrencyListResponse OnSuccess, FDErrorHandler OnError);

private:
	FApiClientPtr ApiClientPtr;
};