// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Runtime/Core/Public/Misc/Guid.h"

struct FOrderCreate : public FJsonSerializable
{
    FString ItemID;
	int32	Quantity;
    int32	Price;
	int32	DiscountedPrice;
    FString CurrencyCode;
	FString ReturnURL;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("itemId", ItemID);
		JSON_SERIALIZE("quantity", Quantity);	
		JSON_SERIALIZE("price", Price);
		JSON_SERIALIZE("discountedPrice", DiscountedPrice);
		JSON_SERIALIZE("currencyCode", CurrencyCode);
		JSON_SERIALIZE("returnUrl", ReturnURL);
	END_JSON_SERIALIZER
};