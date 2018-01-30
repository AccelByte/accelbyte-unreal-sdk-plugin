// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "HTTPJustice.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "OnlineSubsystemJusticeModule.h"

FString FHTTPJustice::BasicAuth(const FString& Username, const FString& Password)
{
	return TEXT("Basic " + FBase64::Encode(Username + ":" + Password));
}
