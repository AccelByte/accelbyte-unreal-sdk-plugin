// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"
#include "ItemInfo.h"
#include "Paging.h"

struct FItemPagingSlicedResult : public FJsonSerializable
{
	TArray<FItemInfoJustice>	Data;
	FPaging						Paging;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE_ARRAY_SERIALIZABLE("data", Data, FItemInfoJustice);
		JSON_SERIALIZE_OBJECT_SERIALIZABLE("paging", Paging);
	END_JSON_SERIALIZER
};


