// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"

class IAccelByteUe4SdkModuleInterface : public IModuleInterface
{
public:
	static IAccelByteUe4SdkModuleInterface& Get()
	{
		return FModuleManager::LoadModuleChecked<IAccelByteUe4SdkModuleInterface>("AccelByteUe4Sdk");
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AccelByteUe4Sdk");
	}
};
