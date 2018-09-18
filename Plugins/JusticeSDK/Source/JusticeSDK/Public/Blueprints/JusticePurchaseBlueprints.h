// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "Models/OrderInfo.h"
#include "JusticePurchaseBlueprints.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCreateNewOrderOutputPin, const UOrderInfo*, OrderInformation);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UCreateNewOrder : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FCreateNewOrderOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FCreateNewOrderOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Purchase"))
		static UCreateNewOrder* CreateNewOrder(int32 Quantity, FString ItemID, int32 Price, int32 DiscountedPrice, FString Currency);
	virtual void Activate() override;
private:
	int32 Quantity;
	FString ItemID;
	int32 Price;
	int32 DiscountedPrice;
	FString Currency;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFulfillOrderOutputPin, const UOrderInfo*, OrderInformation);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UFulfillOrder : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FFulfillOrderOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FFulfillOrderOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Purchase"))
		static UFulfillOrder* FulfillOrder(FString OrderNo);
	virtual void Activate() override;
private:
	FString OrderNo;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetUserOrderOutputPin, const UOrderInfo*, OrderInformation);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UGetUserOrder : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FGetUserOrderOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetUserOrderOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Purchase"))
		static UGetUserOrder* GetUserOrder(FString OrderNo);
	virtual void Activate() override;
private:
	FString OrderNo;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetUserOrderHistoryOutputPin, const TArray<UOrderHistoryInfo*>&, OrderHistory);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UGetUserOrderHistory : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FGetUserOrderHistoryOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetUserOrderHistoryOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Purchase"))
		static UGetUserOrderHistory* GetUserOrderHistory(FString OrderNo);
	virtual void Activate() override;
private:
	FString OrderNo;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGetUserOrdersOutputPin, const TArray<UOrderInfo*>&, OrdersInformation);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UGetUserOrders : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FGetUserOrdersOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetUserOrdersOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | Purchase"))
		static UGetUserOrders* GetUserOrders(int32 Page, int32 Size);
	virtual void Activate() override;
private:
	int32 Page;
	int32 Size;
};
