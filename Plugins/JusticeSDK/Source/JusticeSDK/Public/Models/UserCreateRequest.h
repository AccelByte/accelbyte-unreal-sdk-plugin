// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Serialization/JsonSerializerMacros.h"
#include "Runtime/Core/Public/Misc/Guid.h"

struct FUserCreateRequest : public FJsonSerializable
{
public:
	FUserCreateRequest(){}

	FString AuthType;
	FString DisplayName;
	FString LoginID;
	FString Password;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("AuthType", AuthType);
		JSON_SERIALIZE("DisplayName", DisplayName);
		JSON_SERIALIZE("LoginId", LoginID);
		JSON_SERIALIZE("Password", Password);
	END_JSON_SERIALIZER
};