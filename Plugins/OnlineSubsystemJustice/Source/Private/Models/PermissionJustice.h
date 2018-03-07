// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "CoreMinimal.h"

class FPermissionJustice
{
public:
	FString Resource;
	int32 Action;
	FPermissionJustice(FString resource, int32 action) :Resource(resource), Action(action)
	{	}
};

