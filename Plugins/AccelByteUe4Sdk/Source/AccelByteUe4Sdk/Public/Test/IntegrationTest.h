// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "ModuleManager.h"
#include "IHttpRequest.h"
#include "TelemetryEventDataExample.h"
#include "AccelByteError.h"

DECLARE_DELEGATE(FDeleteUserByIdSuccess);
DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteTest, Log, All);

class FIntegrationTestModule : public IModuleInterface {
public:
	static void DeleteUser(FString UserID, FDeleteUserByIdSuccess OnSuccess, AccelByte::FErrorDelegate OnError);
	static FString GetSteamTicket();
	static FString GetVerificationCode(FString Email);
};
