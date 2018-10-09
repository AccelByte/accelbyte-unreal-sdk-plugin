// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "ModuleManager.h"
#include "IHttpRequest.h"
#include "TelemetryEventDataExample.h"
#include "AccelByteError.h"

DECLARE_DELEGATE(FECommerceUserInitializationSuccess);
DECLARE_DELEGATE(FECommerceUserInitializationFail);

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteECommerceTest, Log, All);

class AccelByteIntegrationTestECommerce
{
};