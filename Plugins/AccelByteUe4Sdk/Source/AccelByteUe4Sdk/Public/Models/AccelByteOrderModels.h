// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteOrderModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfoPaymentUrl
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString PaymentProvider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString PaymentUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString PaymentToken;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString ReturnUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | PaymentUrl")
		FString PaymentType;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfoCurrencySummary
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		FString CurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		FString CurrencyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		FString CurrencySymbol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | CurrencySummary")
		int32 Decimals;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfoTransaction
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString TransactionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 Amount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 Vat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 SalesTax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FAccelByteModelsOrderInfoCurrencySummary Currency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString Type; //['CHARGE', 'REFUND'],
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString Status; // ['INIT', 'FINISHED', 'FAILED'],
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString Provider; //['XSOLLA', 'WALLET'],
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 PaymentProviderFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString PaymentMethod;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		int32 PaymentMethodFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString MerchantId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString ExternalTransactionId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString ExternalStatusCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString ExternalMessage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString TransactionStartTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo | Transaction")
		FString TransactionEndTime;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString OrderNo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	bool bSandbox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 DiscountedPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 Vat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 SalesTax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 PaymentProviderFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	int32 PaymentMethodFee;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FAccelByteModelsOrderInfoCurrencySummary Currency;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FAccelByteModelsOrderInfoPaymentUrl PaymentUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString PaymentStationUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	TArray<FAccelByteModelsOrderInfoTransaction> Transactions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	TArray<FString> EntitlementIds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString Status; // ['INIT', 'CHARGED', 'FINISHED', 'CHARGE_FAILED', 'ERROR', 'REFUNDED', 'DELETED'],
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString StatusReason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString CreatedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString ChargedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString FulfilledTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString RefundedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
	FString UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderInfoPaging
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString Previous;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderInfo")
		FString Next;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderHistoryInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString OrderNo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString Operator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString Reason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderHistoryInfo")
		FString UpdatedAt;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOrderCreate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		FString ItemId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		int32 Quantity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		int32 Price;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		int32 DiscountedPrice;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		FString CurrencyCode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Order | Models | OrderCreate")
		FString ReturnUrl;
};
