// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#include "HTTPJustice.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "OnlineSubsystemJusticeModule.h"

FString FHTTPJustice::BasicAuth(const FString& Username, const FString& Password)
{
	return TEXT("Basic " + FBase64::Encode(Username + ":" + Password));
}

