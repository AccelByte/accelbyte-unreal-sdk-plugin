// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Price.generated.h"

class Price : public FJsonSerializable
{
public:
	int Value;
	FString CurrencyCode;
	FString CurrencyType;
	FString Namespace;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("value", Value);
		JSON_SERIALIZE("currencyCode", CurrencyCode);
		JSON_SERIALIZE("currencyType", CurrencyType);
		JSON_SERIALIZE("namespace", Namespace);
	END_JSON_SERIALIZER
};


UCLASS()
class UPrice : public UObject, public Price
{
	GENERATED_BODY()	

	UFUNCTION(BlueprintPure, Category = "Price")
		FString GetCurrency() { return CurrencyCode; };

	UFUNCTION(BlueprintPure, Category = "Price")
		int GetValue() { return Value; };

public:
	void FromPrice(Price price);

};
