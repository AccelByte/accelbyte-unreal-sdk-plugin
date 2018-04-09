// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "CurrencySummary.h"

class Transaction : public FJsonSerializable
{
public:
    FString txId ;
    int amount ;
    CurrencySummary currency ;
    FString type ;
    FString status ;
    FString provider ;
    FString paymentMethod ;
    FString merchantId ;
    FString extTxId ;
    FString extStatusCode ;
    FString extMessage ;
    FString txStartTime  ;
    FString txEndTime  ;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("txId", txId);
		JSON_SERIALIZE("amount", amount);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("currency", currency);
        JSON_SERIALIZE("status", status);
        JSON_SERIALIZE("provider", provider);
        JSON_SERIALIZE("paymentMethod", paymentMethod);
        JSON_SERIALIZE("merchantId", merchantId);
        JSON_SERIALIZE("extTxId", extTxId);
        JSON_SERIALIZE("extStatusCode", extStatusCode);
        JSON_SERIALIZE("extMessage", extMessage);
        JSON_SERIALIZE("txStartTime", txStartTime);
        JSON_SERIALIZE("txEndTime", txEndTime);
	END_JSON_SERIALIZER
};


