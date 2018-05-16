// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticePlatformFunctions.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "Utilities/JusticeLog.h"
#include "Models/UserProfileInfo.h"

DECLARE_DELEGATE_ThreeParams(FReqestCurrentPlayerProfileCompleteDelegate, bool, FString, UserProfileInfo);
DECLARE_DELEGATE_TwoParams(FUpdatePlayerProfileCompleteDelegate, bool, FString);

class JUSTICESDK_API JusticePlatform
{
public:
	static void RequestCurrentPlayerProfile(FReqestCurrentPlayerProfileCompleteDelegate OnComplete);
    static void UpdatePlayerProfile(UserProfileInfo newUserProfile, FUpdatePlayerProfileCompleteDelegate OnComplete);
	static void CreateDefaultPlayerProfile(FString Email, FString DisplayName, FUpdatePlayerProfileCompleteDelegate OnComplete);
	static UserProfileInfo* GetUserProfileInfo();
private:
	static void OnRequestCurrentPlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FReqestCurrentPlayerProfileCompleteDelegate OnComplete);
	static void OnUpdatePlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUpdatePlayerProfileCompleteDelegate OnComplete, UserProfileInfo newUserProfile);
	static void OnCreateDefaultPlayerProfileComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FUpdatePlayerProfileCompleteDelegate OnComplete, FString Email, FString DisplayName);
};