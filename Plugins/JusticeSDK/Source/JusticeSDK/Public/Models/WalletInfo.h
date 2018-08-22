// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "BalanceInfo.h"
#include "WalletInfo.generated.h"

struct FWalletInfo : public FJsonSerializable
{
	FString ID;
	FString Namespace;
	FString UserID;
	FString CurrencyCode;
	FString CurrencySymbol;
	int32	Balance;
	FString CreatedAt;
	FString UpdatedAt;
	FString Status;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("id", ID);
		JSON_SERIALIZE("namespace", Namespace);
		JSON_SERIALIZE("userId", UserID);
		JSON_SERIALIZE("currencyCode", CurrencyCode);
		JSON_SERIALIZE("currencySymbol", CurrencySymbol);
		JSON_SERIALIZE("balance", Balance);
		JSON_SERIALIZE("createdAt", CreatedAt);
		JSON_SERIALIZE("updatedAt", UpdatedAt);
		JSON_SERIALIZE("status", Status);
	END_JSON_SERIALIZER
};

UCLASS()
class UWalletInfo : public UObject, public FWalletInfo
{
	GENERATED_BODY()
public:
};