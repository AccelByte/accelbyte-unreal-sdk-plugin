// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "OrderInfo.h"
#include "Paging.h"

struct FOrderInfoPaging : public FJsonSerializable
{
	TArray<FOrderInfo>	Data;
	FPaging				Paging;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("data", Data, FOrderInfo);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("paging", Paging);
	END_JSON_SERIALIZER
};


