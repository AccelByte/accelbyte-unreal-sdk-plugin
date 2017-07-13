// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystemImpl.h"
#include "OnlineSubsystemJusticePackage.h"
#include "HAL/ThreadSafeCounter.h"

#include "UObject/CoreOnline.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "OpenTracingJustice.h"

/**
 * Justice HTTP
 * We create our own transport class so that we can extend it to add tracing and metrics.
 */
class FHTTPJustice
{
	
public:
	
	FHTTPJustice()
	{
	}
	
	~FHTTPJustice()
	{
	}

	static FString BasicAuth(const FString& Username, const FString& Password);
	static TSharedRef<IHttpRequest> CreateRequest(const FOpenTracingJustice& Parent);

private:
	
};
