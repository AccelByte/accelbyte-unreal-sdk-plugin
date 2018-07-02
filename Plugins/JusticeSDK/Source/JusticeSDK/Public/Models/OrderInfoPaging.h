// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "OrderInfo.h"
#include "Paging.h"

class OrderInfoPaging : public FJsonSerializable
{
public:
	TArray<OrderInfo> Data;
	Paging PagingInfo;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("data", Data, OrderInfo);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("paging", PagingInfo);
	END_JSON_SERIALIZER
};


