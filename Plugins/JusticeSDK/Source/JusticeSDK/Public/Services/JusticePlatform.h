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

DECLARE_DELEGATE_ThreeParams(FRequestCurrentPlayerProfileCompleteDelegate, bool, FString, FUserProfileInfo*);

class JUSTICESDK_API JusticePlatform
{
public:
	static void RequestCurrentPlayerProfile(FOAuthTokenJustice Token, FRequestCurrentPlayerProfileCompleteDelegate OnComplete);
    static void UpdatePlayerProfile(FOAuthTokenJustice Token, UserProfileInfoUpdate NewUserProfile, FDefaultCompleteDelegate OnComplete);
	static void CreateDefaultPlayerProfile(FOAuthTokenJustice Token, FString DisplayName, FDefaultCompleteDelegate OnComplete);
	static TSharedPtr<FUserProfileInfo> GetUserProfileInfo();
private:
	static void OnRequestCurrentPlayerProfileComplete(FJusticeResponsePtr Response, FOAuthTokenJustice Token, FRequestCurrentPlayerProfileCompleteDelegate OnComplete);
	static void OnUpdatePlayerProfileComplete(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnCreateDefaultPlayerProfileComplete(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
};