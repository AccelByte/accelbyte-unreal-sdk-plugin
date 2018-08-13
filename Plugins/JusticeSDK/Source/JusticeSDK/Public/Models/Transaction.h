// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "CurrencySummary.h"

struct FTransaction : public FJsonSerializable
{
public:
    FString				TransactionID;
    int32				Amount;
	int32				Vat;
	int32				SalesTax;
    FCurrencySummary	Currency;
    FString				Type; //['CHARGE', 'REFUND'],
    FString				Status; // ['INIT', 'FINISHED', 'FAILED'],
    FString				Provider; //['XSOLLA', 'WALLET'],
	int32				PaymentProviderFee;
    FString				PaymentMethod;
	int32				PaymentMethodFee;
    FString				MerchantID;
    FString				ExternalTransactionID;
    FString				ExternalStatusCode;
    FString				ExternalMessage;
    FString				TransactionStartTime;
    FString				TransactionEndTime;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("txId", TransactionID);
		JSON_SERIALIZE("amount", Amount);
		JSON_SERIALIZE("vat", Vat);
		JSON_SERIALIZE("salesTax", SalesTax);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("currency", Currency);
        JSON_SERIALIZE("status", Status);
        JSON_SERIALIZE("provider", Provider);
		JSON_SERIALIZE("paymentProviderFee", PaymentProviderFee);
        JSON_SERIALIZE("paymentMethod", PaymentMethod);
		JSON_SERIALIZE("paymentMethodFee", PaymentMethodFee);
        JSON_SERIALIZE("merchantId", MerchantID);
        JSON_SERIALIZE("extTxId", ExternalTransactionID);
        JSON_SERIALIZE("extStatusCode", ExternalStatusCode);
        JSON_SERIALIZE("extMessage", ExternalMessage);
        JSON_SERIALIZE("txStartTime", TransactionStartTime);
        JSON_SERIALIZE("txEndTime", TransactionEndTime);
	END_JSON_SERIALIZER
};