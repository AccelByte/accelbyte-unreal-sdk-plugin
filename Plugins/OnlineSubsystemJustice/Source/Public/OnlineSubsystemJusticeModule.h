// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleInterface.h"

/**
 * Online subsystem module class  (Justice Implementation)
 * Code related to the loading of the Justice module
 */
class FOnlineSubsystemJusticeModule : public IModuleInterface
{
private:

	/** Class responsible for creating instance(s) of the subsystem */
	class FOnlineFactoryJustice* JusticeFactory;

public:

	FOnlineSubsystemJusticeModule() : 
		JusticeFactory(nullptr)
	{}

	virtual ~FOnlineSubsystemJusticeModule() {}

	// IModuleInterface

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual bool SupportsDynamicReloading() override
	{
		return false;
	}

	virtual bool SupportsAutomaticShutdown() override
	{
		return false;
	}
};
