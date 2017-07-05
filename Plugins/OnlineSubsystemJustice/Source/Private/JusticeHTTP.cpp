// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#include "JusticeHTTP.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "OnlineSubsystemJusticeModule.h"

FString FJusticeHTTP::BasicAuth(const FString& Username, const FString& Password)
{
	return TEXT("Basic " + FBase64::Encode(Username + ":" + Password));
}
