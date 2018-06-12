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
#include "OrderInfo.generated.h"

class OrderInfo : public FJsonSerializable
{
public:
	OrderInfo() {}

	FString OrderNo;
	FString UserId;
	FString ItemId ;
	FString StoreId;
	bool Sandbox;
	bool Quantity;
	int Price;
	int DiscountedPrice;
	int Vat;
	int SalesTax;
	int PaymentProviderFee;
	int PaymentMethodFee;
	CurrencySummary Currency ;
	PaymentUrl PaymentUrl;
	TArray<Transaction> Transactions;
	TArray<FString> EntitlementIds;
	FString Status; // ['INIT', 'CHARGED', 'FINISHED', 'CHARGE_FAILED', 'ERROR', 'REFUNDED', 'DELETED'],
	FString StatusReason;
	FString Namespace;
	FString CreatedTime;
	FString ChargedTime;
	FString FinishedTime;
	FString RefundedTime;
	FString CreatedAt;
	FString UpdatedAt;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("orderNo", OrderNo);
		JSON_SERIALIZE("userId", UserId);
		JSON_SERIALIZE("itemId", ItemId);
		JSON_SERIALIZE("storeId", StoreId);
		JSON_SERIALIZE("sandbox", Sandbox);
		JSON_SERIALIZE("quantity", Quantity);
		JSON_SERIALIZE("price", Price);
		JSON_SERIALIZE("discountedPrice", DiscountedPrice);
		JSON_SERIALIZE("vat", Vat);
		JSON_SERIALIZE("salesTax", SalesTax);
		JSON_SERIALIZE("paymentProviderFee", PaymentProviderFee);
		JSON_SERIALIZE("paymentMethodFee", PaymentMethodFee);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("currency", Currency);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("paymentUrl", PaymentUrl);
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("transactions", Transactions, Transaction);
		JSON_SERIALIZE_ARRAY("entitlementIds", EntitlementIds);
		JSON_SERIALIZE("status", Status);
		JSON_SERIALIZE("statusReason", StatusReason);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("createdTime", CreatedTime);
		JSON_SERIALIZE("chargedTime", ChargedTime);
		JSON_SERIALIZE("finishedTime", FinishedTime);
		JSON_SERIALIZE("refundedTime", RefundedTime);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);

	END_JSON_SERIALIZER
};


UCLASS()
class UOrderInfo : public UObject, public OrderInfo
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "OrderInfo")
		FString GetItemId() { return ItemId; };

	UFUNCTION(BlueprintPure, Category = "OrderInfo")
		int GetPrice() { return Price; };

	UFUNCTION(BlueprintPure, Category = "OrderInfo")
		FString GetPaymenUrl() { return PaymentUrl.paymentUrl; };
public:
		void FromOrderInfo(OrderInfo order);
		
};
