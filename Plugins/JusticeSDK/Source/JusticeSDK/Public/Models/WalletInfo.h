// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "BalanceInfo.h"
#include "WalletInfo.generated.h"

class WalletInfo : public FJsonSerializable
{
public:
	FString ID;
	FString Namespace;
	FString userId;
	FString CurrencyCode;
	int32 balance;
	FString  createdAt;
	FString  updatedAt;
	FString status;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("id", ID);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("userId", userId);
		JSON_SERIALIZE("currencyCode", CurrencyCode);
		JSON_SERIALIZE("balance", balance);
		JSON_SERIALIZE("createdAt", createdAt);
		JSON_SERIALIZE("updatedAt", updatedAt);
		JSON_SERIALIZE("status", status);
	END_JSON_SERIALIZER
};

UCLASS()
class UWalletInfo : public UObject, public WalletInfo
{
	GENERATED_BODY()
public:
};