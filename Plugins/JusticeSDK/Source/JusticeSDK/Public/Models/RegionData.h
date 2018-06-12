// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "RegionData.generated.h"

class RegionData : public FJsonSerializable
{
public:

    int Price;
    int DiscountPercentage;
    int DiscountAmount;
    int DiscountedPrice;
    FString CurrencyCode;
    FString CurrencyType;
    FString CurrencyNamespace;
    FString PurchaseAt;
    FString ExpireAt;
    int TotalNum;
    int TotalNumPerAccount;
    FString DiscountPurchaseAt;
    FString DiscountExpireAt;
    int DiscountTotalNum;
    int DiscountTotalNumPerAccount;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("price", Price);
        JSON_SERIALIZE("discountPercentage", DiscountPercentage);
        JSON_SERIALIZE("discountAmount", DiscountAmount);
        JSON_SERIALIZE("discountedPrice", DiscountedPrice);
        JSON_SERIALIZE("currencyCode", CurrencyCode);
        JSON_SERIALIZE("currencyType", CurrencyType);
        JSON_SERIALIZE("currencyNamespace", CurrencyNamespace);
        JSON_SERIALIZE("purchaseAt", PurchaseAt);
        JSON_SERIALIZE("expireAt", ExpireAt);
        JSON_SERIALIZE("totalNum", TotalNum);
        JSON_SERIALIZE("totalNumPerAccount", TotalNumPerAccount);
        JSON_SERIALIZE("discountPurchaseAt", DiscountPurchaseAt);
        JSON_SERIALIZE("discountExpireAt", DiscountExpireAt);
        JSON_SERIALIZE("discountTotalNum", DiscountTotalNum);
        JSON_SERIALIZE("discountTotalNumPerAccount", DiscountTotalNumPerAccount);

	END_JSON_SERIALIZER
};


UCLASS()
class URegionData : public UObject, public RegionData
{
	GENERATED_BODY()	

	// UFUNCTION(BlueprintPure, Category = "Price")
	// 	FString GetCurrency() { return CurrencyCode; };

	// UFUNCTION(BlueprintPure, Category = "Price")
	// 	int GetValue() { return Value; };

public:
	void FromRegionData(RegionData regionData);

};