// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "ModuleManager.h"
#include "IHttpRequest.h"
#include "TelemetryEventDataExample.h"

DECLARE_DELEGATE_TwoParams(FDeleteUserDelegate, bool, FString);

class FIntegrationTestModule : public IModuleInterface {
public:
	static void DeleteUser(FString UserID, FDeleteUserDelegate OnComplete);
	static FString GetSteamTicket();
	static FString GetVerificationCode(FString Email);
private:
	static void OnDeleteUserComplete(FHttpResponsePtr Response, FDeleteUserDelegate OnComplete);
};
