// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"


struct FPaging : public FJsonSerializable
{
public:
    FString Previous;
    FString Next;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("previous", Previous);
		JSON_SERIALIZE("next", Next);
	END_JSON_SERIALIZER
};