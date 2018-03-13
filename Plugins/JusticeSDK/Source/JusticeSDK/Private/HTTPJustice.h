// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
//#include "OnlineSubsystemImpl.h"
//#include "OnlineSubsystemJusticePackage.h"
#include "HAL/ThreadSafeCounter.h"

#include "UObject/CoreOnline.h"
#include "Runtime/Online/HTTP/Public/Http.h"

class FHTTPJustice
{
public:	
	FHTTPJustice()	{}
	~FHTTPJustice()	{}
	static FString BasicAuth(const FString& Username, const FString& Password);
	static FString BearerAuth(const FString& Token);	
};
