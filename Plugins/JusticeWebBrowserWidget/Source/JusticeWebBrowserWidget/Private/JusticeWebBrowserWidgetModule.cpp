// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

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
