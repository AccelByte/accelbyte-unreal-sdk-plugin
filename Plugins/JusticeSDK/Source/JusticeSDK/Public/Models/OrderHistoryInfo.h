// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "JusticeBaseModel.h"
#include "OrderHistoryInfo.generated.h"

class OrderHistoryInfo : public FJsonSerializable
{
public:
	OrderHistoryInfo() {}

	FString OrderNo;
	FString Operator;
	FString Action;
	FString Reason;
	FString UserID;
	FString CreatedAt;
	FString UpdatedAt;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("orderNo", OrderNo);
		JSON_SERIALIZE("operator", Operator);
		JSON_SERIALIZE("action", Action);
		JSON_SERIALIZE("reason", Reason);
		JSON_SERIALIZE("userId", UserID);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);
	END_JSON_SERIALIZER
};


UCLASS()
class UOrderHistoryInfo : public UObject, public OrderHistoryInfo, public JusticeBaseModel<UOrderHistoryInfo, OrderHistoryInfo>
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, Category = "OrderHistoryInfo")
		FString GetOrderNo() { return OrderNo; };

	UFUNCTION(BlueprintPure, Category = "OrderHistoryInfo")
		FString GetOperator() { return Operator; };

	UFUNCTION(BlueprintPure, Category = "OrderHistoryInfo")
		FString GetAction() { return Action; };

	UFUNCTION(BlueprintPure, Category = "OrderHistoryInfo")
		FString GetReason() { return Reason; };

	UFUNCTION(BlueprintPure, Category = "OrderHistoryInfo")
		FString GetUserID() { return UserID; };

	UFUNCTION(BlueprintPure, Category = "OrderHistoryInfo")
		FString GetCreatedAt() { return CreatedAt; };

	UFUNCTION(BlueprintPure, Category = "OrderHistoryInfo")
		FString GetUpdatedAt() { return UpdatedAt; };
};
