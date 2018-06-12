// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "BalanceInfo.generated.h"

class BalanceInfo: public FJsonSerializable
{
public:
	FString ID;
	FString walletId;
	FString CurrencyCode;
	int32 balance;
	FString  balanceSource;
	FString  updatedAt;
	FString status;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("id", ID);
		JSON_SERIALIZE("walletId", walletId);
		JSON_SERIALIZE("currencyCode", CurrencyCode);
		JSON_SERIALIZE("balance", balance);
		JSON_SERIALIZE("balanceSource", balanceSource);
		JSON_SERIALIZE("updatedAt", updatedAt);
		JSON_SERIALIZE("status", status);
	END_JSON_SERIALIZER
};

UCLASS()
class UBalanceInfo : public UObject, public BalanceInfo
{
	GENERATED_BODY()
};
