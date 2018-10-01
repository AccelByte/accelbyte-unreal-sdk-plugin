// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"

class AccelByteUe4SdkModule : public IModuleInterface
{
public:
	bool IsInitialized;
	static AccelByteUe4SdkModule& Get()
	{
		AccelByteUe4SdkModule& ModuleInstance = FModuleManager::LoadModuleChecked<AccelByteUe4SdkModule>("AccelByteUe4Sdk");
		verify(ModuleInstance.IsInitialized);
		return ModuleInstance;
	}
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AccelByteUe4Sdk");
	}

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};
