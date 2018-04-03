// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "ItemInfo.h"
#include "Paging.h"

class ItemPagingSlicedResult : public FJsonSerializable
{
public:
	TArray<ItemInfo> Data;
	Paging PagingInfo;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("data", Data, ItemInfo);
		JSON_SERIALIZE_SERIALIZABLE("paging", PagingInfo);
	END_JSON_SERIALIZER
};


