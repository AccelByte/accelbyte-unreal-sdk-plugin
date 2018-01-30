// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Modules/ModuleManager.h"
#include "Misc/ConfigCacheIni.h"

#define INVALID_INDEX -1

/** URL Prefix when using Justice socket connection */
#define JUSTICE_URL_PREFIX TEXT("Justice.")

/** pre-pended to all Justice logging */
#undef ONLINE_LOG_PREFIX
#define ONLINE_LOG_PREFIX TEXT("Justice: ")


