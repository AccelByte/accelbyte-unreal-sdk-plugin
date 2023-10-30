#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABCurrency.generated.h"

USTRUCT(BlueprintType)
struct FArrayModelsCurrencyList
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Currency | Models | List Currency")
	TArray<FAccelByteModelsCurrencyList> Content;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDArrayModelsCurrencyListResponse, FArrayModelsCurrencyList, Response);

UCLASS(Blueprintable, BlueprintType)
class UABCurrency : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Currency | Api")
	void GetCurrencyList(FDArrayModelsCurrencyListResponse OnSuccess, FDErrorHandler OnError,
		EAccelByteCurrencyType CurrencyType = EAccelByteCurrencyType::NONE);

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};