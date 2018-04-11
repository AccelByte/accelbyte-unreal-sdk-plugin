// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/JsonSerializerMacros.h"

class ResetPasswordRequest : public FJsonSerializable
{
public:
    FString Code;
    FString LoginID;
    FString NewPassword;

	BEGIN_JSON_SERIALIZER
		JSON_SERIALIZE("Code", Code);
		JSON_SERIALIZE("LoginID", LoginID);
		JSON_SERIALIZE("NewPassword", NewPassword);
	END_JSON_SERIALIZER
};


