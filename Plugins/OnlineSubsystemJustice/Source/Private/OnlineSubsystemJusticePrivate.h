// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

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


