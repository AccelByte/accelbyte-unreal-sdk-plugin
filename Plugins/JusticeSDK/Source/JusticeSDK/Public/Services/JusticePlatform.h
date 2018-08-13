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

DECLARE_DELEGATE_ThreeParams(FReqestCurrentPlayerProfileCompleteDelegate, bool, FString, FUserProfileInfo);

class JUSTICESDK_API JusticePlatform
{
public:
	static void RequestCurrentPlayerProfile(FReqestCurrentPlayerProfileCompleteDelegate OnComplete);
    static void UpdatePlayerProfile(UserProfileInfoUpdate newUserProfile, FDefaultCompleteDelegate OnComplete);
	static void CreateDefaultPlayerProfile(FString Email, FString DisplayName, FDefaultCompleteDelegate OnComplete);
	static FUserProfileInfo* GetUserProfileInfo();
private:
	static void OnRequestCurrentPlayerProfileComplete(FJusticeHttpResponsePtr Response, FReqestCurrentPlayerProfileCompleteDelegate OnComplete);
	static void OnUpdatePlayerProfileComplete(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnCreateDefaultPlayerProfileComplete(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete);
};