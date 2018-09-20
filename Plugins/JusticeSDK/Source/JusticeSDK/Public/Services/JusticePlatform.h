// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "Utilities/JusticeLog.h"
#include "Models/UserProfileInfo.h"
#include "Models/UserProfileRequest.h"

DECLARE_DELEGATE_ThreeParams(FRequestCurrentPlayerProfileCompleteDelegate, bool, FString, FUserProfileInfo*);

/**
 * @brief Justice platform service
 * User should log in before using this. See JusticeIdentity to see how to log user in.
 */
class JUSTICESDK_API JusticePlatform
{
public:
	/**
	 * @brief Get user's profile information, if it exists.
	 * 
	 * @param Token Required. User acess token.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received. The result is FUserProfileInfo, set in FRequestCurrentPlayerProfileCompleteDelegate callback.
	 */
	static void RequestCurrentPlayerProfile(FOAuthTokenJustice Token, FRequestCurrentPlayerProfileCompleteDelegate OnComplete);

	/**
	 * @brief Update user's current profile information, if it exists.
	 * 
	 * @param Token Required. Token User acess token.
	 * @param NewUserProfile Required. UserProfileInfoUpdate object.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 */
    static void UpdatePlayerProfile(FOAuthTokenJustice Token, UserProfileInfoUpdate NewUserProfile, FDefaultCompleteDelegate OnComplete);
	
	/**
	 * @brief Create empty profile information.
	 * 
	 * @param Token  Required. User acess token.
	 * @param DisplayName Required.
	 * @param OnComplete Required, but can be nullptr. This will be called when response has been received.
	 */
	static void CreateDefaultPlayerProfile(FOAuthTokenJustice Token, FString DisplayName, FDefaultCompleteDelegate OnComplete);

	static void CreateCompletePlayerProfile(FOAuthTokenJustice Token, FUserCreateRequest ProfileRequest, FRequestCurrentPlayerProfileCompleteDelegate OnComplete);
	static TSharedPtr<FUserProfileInfo> GetUserProfileInfo();
private:
	static void OnRequestCurrentPlayerProfileComplete(FJusticeResponsePtr Response, FOAuthTokenJustice Token, FRequestCurrentPlayerProfileCompleteDelegate OnComplete);
	static void OnUpdatePlayerProfileComplete(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnCreateDefaultPlayerProfileComplete(FJusticeResponsePtr Response, FDefaultCompleteDelegate OnComplete);
	static void OnCreateCompletePlayerProfile(FJusticeResponsePtr Response, FRequestCurrentPlayerProfileCompleteDelegate OnComplete);
};