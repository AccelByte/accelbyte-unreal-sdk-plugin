// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"


class PaymentUrl : public FJsonSerializable
{
public:
    FString paymentProvider ;
    FString paymentUrl ;
    FString paymentToken  ;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("paymentProvider", paymentProvider );
		JSON_SERIALIZE("paymentUrl", paymentUrl );
        JSON_SERIALIZE("paymentToken", paymentToken );
	END_JSON_SERIALIZER
};


