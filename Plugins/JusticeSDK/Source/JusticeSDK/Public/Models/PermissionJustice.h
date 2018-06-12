// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "CoreMinimal.h"
#include "Serialization/JsonSerializerMacros.h"

class PermissionJustice : public FJsonSerializable
{
public:
	FString Resource;
	int32 Action;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("Resource", Resource);
		JSON_SERIALIZE("Action", Action);
	END_JSON_SERIALIZER

};

