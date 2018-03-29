// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Price.generated.h"

class Price : public FJsonSerializable
{
public:
	int value;
	FString currency;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("value", value);
		JSON_SERIALIZE("currency", currency);
	END_JSON_SERIALIZER
};


UCLASS()
class UPrice : public UObject, public Price
{
	GENERATED_BODY()	
};
