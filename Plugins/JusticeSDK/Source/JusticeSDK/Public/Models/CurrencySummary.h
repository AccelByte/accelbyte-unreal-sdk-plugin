// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"

struct FCurrencySummary : public FJsonSerializable
{
    FString CurrencyCode;
    FString CurrencyType;
	FString CurrencySymbol;
    FString Namespace;
    int32	Decimals;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("currencyCode", CurrencyCode);
		JSON_SERIALIZE("currencyType", CurrencyType);
		JSON_SERIALIZE("currencySymbol", CurrencySymbol);
        JSON_SERIALIZE("namespace", Namespace);
        JSON_SERIALIZE("decimals", Decimals);
	END_JSON_SERIALIZER
};