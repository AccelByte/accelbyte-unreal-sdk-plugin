// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Transaction.h"
#include "PaymentUrl.h"
#include "CurrencySummary.h"
#include "JusticeBaseModel.h"
#include "OrderInfo.generated.h"

struct FOrderInfo : public FJsonSerializable
{
public:
	FOrderInfo() {}

	FString					OrderNo;
	FString					UserID;
	FString					ItemID ;
	bool					bSandbox;
	int32					Quantity;
	int32					Price;
	int32					DiscountedPrice;
	int32					Vat;
	int32					SalesTax;
	int32					PaymentProviderFee;
	int32					PaymentMethodFee;
	FCurrencySummary		Currency ;
	FPaymentURL				PaymentUrl;
	FString					PaymentStationUrl;
	TArray<FTransaction>	Transactions;
	TArray<FString>			EntitlementIds;
	FString					Status; // ['INIT', 'CHARGED', 'FINISHED', 'CHARGE_FAILED', 'ERROR', 'REFUNDED', 'DELETED'],
	FString					StatusReason;
	FString					Namespace;
	FString					CreatedTime;
	FString					ChargedTime;
	FString					FulfilledTime;
	FString					RefundedTime;
	FString					CreatedAt;
	FString					UpdatedAt;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("orderNo", OrderNo);
		JSON_SERIALIZE("userId", UserID);
		JSON_SERIALIZE("itemId", ItemID);
		JSON_SERIALIZE("sandbox", bSandbox);
		JSON_SERIALIZE("quantity", Quantity);
		JSON_SERIALIZE("price", Price);
		JSON_SERIALIZE("discountedPrice", DiscountedPrice);
		JSON_SERIALIZE("vat", Vat);
		JSON_SERIALIZE("salesTax", SalesTax);
		JSON_SERIALIZE("paymentProviderFee", PaymentProviderFee);
		JSON_SERIALIZE("paymentMethodFee", PaymentMethodFee);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("currency", Currency);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("paymentUrl", PaymentUrl);
		JSON_SERIALIZE("paymentStationUrl", PaymentStationUrl);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("transactions", Transactions, FTransaction);
		JSON_SERIALIZE_ARRAY("entitlementIds", EntitlementIds);
		JSON_SERIALIZE("status", Status);
		JSON_SERIALIZE("statusReason", StatusReason);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("createdTime", CreatedTime);
		JSON_SERIALIZE("chargedTime", ChargedTime);
		JSON_SERIALIZE("fulfilledTime", FulfilledTime);
		JSON_SERIALIZE("refundedTime", RefundedTime);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);

	END_JSON_SERIALIZER
};


UCLASS()
class UOrderInfo : public UObject, public FOrderInfo, public FBaseModelJustice<UOrderInfo, FOrderInfo>
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "OrderInfo")
		FString GetItemId() { return ItemID; };

	UFUNCTION(BlueprintPure, Category = "OrderInfo")
		int32 GetPrice() { return Price; };

	UFUNCTION(BlueprintPure, Category = "OrderInfo")
		FString GetPaymentUrl() { return PaymentUrl.PaymentURL; };		

	UFUNCTION(BlueprintPure, Category = "OrderInfo")
		FString GetPaymentStationURL() { return PaymentStationUrl; };		

	UFUNCTION(BlueprintPure, Category = "OrderInfo")
		FString GetOrderNo() { return OrderNo; };
};