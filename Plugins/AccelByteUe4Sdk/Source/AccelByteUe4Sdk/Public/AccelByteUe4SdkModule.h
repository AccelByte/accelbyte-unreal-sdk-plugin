// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"

class FAccelByteUe4SdkModule : public IModuleInterface
{
public:
	static FAccelByteUe4SdkModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FAccelByteUe4SdkModule>("AccelByteUe4Sdk");
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AccelByteUe4Sdk");
	}

	void StartupModule() override;
	void ShutdownModule() override;

	// For registering settings in UE4 editor
	void RegisterSettings();
	void UnregisterSettings();

	bool LoadSettingsFromConfigUobject();
};
