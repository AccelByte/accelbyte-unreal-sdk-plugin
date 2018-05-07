// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Serialization/JsonSerializerMacros.h"

class FUserCreateResponse : public FJsonSerializable
{
public:
	FString Namespace;
	FString UserId;
	FString AuthType;
	FString DisplayName;
	FString LoginId;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("Namespace", Namespace);
		JSON_SERIALIZE("UserId", UserId);
		JSON_SERIALIZE("AuthType", AuthType);
		JSON_SERIALIZE("DisplayName", DisplayName);
		JSON_SERIALIZE("LoginId", LoginId);
	END_JSON_SERIALIZER
};