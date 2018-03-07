// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemTypes.h"
#include "Online.h"
#include "OnlineJsonSerializer.h"
#include "Runtime/Core/Public/Misc/Guid.h"

class FUserCreateRequest : public FOnlineJsonSerializable
{

public:
	FUserCreateRequest()
	{

	}

	FString AuthType;
	FString DisplayName;
	FString LoginId;
	FString Password;

	BEGIN_ONLINE_JSON_SERIALIZER
		ONLINE_JSON_SERIALIZE("AuthType", AuthType);
		ONLINE_JSON_SERIALIZE("DisplayName", DisplayName);
		ONLINE_JSON_SERIALIZE("LoginId", LoginId);
		ONLINE_JSON_SERIALIZE("Password", Password);
	END_ONLINE_JSON_SERIALIZER

};

