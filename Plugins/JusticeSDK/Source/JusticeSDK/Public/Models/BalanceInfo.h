// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "BalanceInfo.generated.h"

struct FBalanceInfo: public FJsonSerializable
{
public:
	FString	ID;
	FString	WalletID;
	FString	CurrencyCode;
	int32	Balance;
	FString	BalanceSource;
	FString	UpdatedAt;
	FString	Status;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("id", ID);
		JSON_SERIALIZE("walletId", WalletID);
		JSON_SERIALIZE("currencyCode", CurrencyCode);
		JSON_SERIALIZE("balance", Balance);
		JSON_SERIALIZE("balanceSource", BalanceSource);
		JSON_SERIALIZE("updatedAt", UpdatedAt);
		JSON_SERIALIZE("status", Status);
	END_JSON_SERIALIZER
};

UCLASS()
class UBalanceInfo : public UObject, public FBalanceInfo
{
	GENERATED_BODY()
};