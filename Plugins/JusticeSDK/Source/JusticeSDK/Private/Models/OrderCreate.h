// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Runtime/Core/Public/Misc/Guid.h"

class OrderCreate : public FJsonSerializable
{
public:

    FString itemId;
    int price;
    FString currencyCode;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("itemId", itemId);
		JSON_SERIALIZE("price", price);
		JSON_SERIALIZE("currencyCode", currencyCode);
	END_JSON_SERIALIZER
};

