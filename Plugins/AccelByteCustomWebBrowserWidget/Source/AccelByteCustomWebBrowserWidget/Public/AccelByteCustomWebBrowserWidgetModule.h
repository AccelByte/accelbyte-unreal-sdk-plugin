// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "ModuleManager.h"


/**
 * The public interface to this module
 */
class IAccelByteCustomWebBrowserWidgetModule : public IModuleInterface
{

public:

	/**
	 * Singleton-like access to this module's interface.  This is just for convenience!
	 * Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	 *
	 * @return Returns singleton instance, loading the module on demand if needed
	 */
	static inline IAccelByteCustomWebBrowserWidgetModule& Get()
	{
		return FModuleManager::LoadModuleChecked<IAccelByteCustomWebBrowserWidgetModule>("AccelByteCustomWebBrowserWidget" );
	}

	/**
	 * Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	 *
	 * @return True if the module is loaded and ready to use
	 */
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AccelByteCustomWebBrowserWidget" );
	}

	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};