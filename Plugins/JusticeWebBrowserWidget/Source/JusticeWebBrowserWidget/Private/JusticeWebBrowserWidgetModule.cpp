// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "JusticeWebBrowser.h"
#include "JusticeWebBrowserWidgetModule.h"
#include "Modules/ModuleManager.h"

//////////////////////////////////////////////////////////////////////////
// FWebBrowserWidgetModule

class FJusticeWebBrowserWidgetModule : public IJusticeWebBrowserWidgetModule
{
public:
	virtual void StartupModule() override
	{
	}

	virtual void ShutdownModule() override
	{
	}
};

//////////////////////////////////////////////////////////////////////////

IMPLEMENT_MODULE(FJusticeWebBrowserWidgetModule, WebBrowserWidget);
