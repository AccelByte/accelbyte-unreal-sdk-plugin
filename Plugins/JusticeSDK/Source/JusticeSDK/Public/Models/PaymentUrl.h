// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"

struct FPaymentURL : public FJsonSerializable
{
    FString PaymentProvider ;
    FString PaymentURL ;
    FString PaymentToken  ;
	FString ReturnURL;
	FString PaymentType;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("paymentProvider", PaymentProvider );
		JSON_SERIALIZE("paymentUrl", PaymentURL );
        JSON_SERIALIZE("paymentToken", PaymentToken );
		JSON_SERIALIZE("returnUrl", ReturnURL);
		JSON_SERIALIZE("paymentType", PaymentType);
	END_JSON_SERIALIZER
};