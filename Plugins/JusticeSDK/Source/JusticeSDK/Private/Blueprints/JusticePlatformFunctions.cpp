// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticePlatformFunctions.h"
#include "Misc/ConfigCacheIni.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "JusticePlatform.h"

void UJusticePlatformFunctions::RequestCurrentPlayerProfile(FProfileReqestCompleteDelegate OnComplete)
{
	JusticePlatform::RequestCurrentPlayerProfile(*FJusticeUserToken, FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorString, FUserProfileInfo* userInfo) {
		UUserProfileJustice* UserProfile = UUserProfileJustice::Deserialize(userInfo);
		check(UserProfile);
		OnComplete.ExecuteIfBound(IsSuccess, ErrorString, UserProfile);
	}));
}

void UJusticePlatformFunctions::UpdatePlayerProfile(FString DisplayName, FString FirstName, FString LastName, FString Country, FString AvatarURL, FString Language, FString Timezone, FUpdatePlayerProfileCompleteDynamicDelegate OnComplete)
{
	FUserProfileInfo* OldUserProfile = FJusticeSDKModule::Get().UserProfile.Get();
	UserProfileInfoUpdate NewUserProfile;

	NewUserProfile.DisplayName = !DisplayName.IsEmpty() ? DisplayName : OldUserProfile->DisplayName;
	NewUserProfile.FirstName = !FirstName.IsEmpty() ? FirstName : OldUserProfile->FirstName;
	NewUserProfile.LastName = !LastName.IsEmpty() ? LastName : OldUserProfile->LastName;
	NewUserProfile.AvatarURL = !AvatarURL.IsEmpty() ? AvatarURL : OldUserProfile->AvatarURL;
	NewUserProfile.AvatarSmallURL = !AvatarURL.IsEmpty() ? AvatarURL : OldUserProfile->AvatarSmallURL;
	NewUserProfile.AvatarLargeURL = !AvatarURL.IsEmpty() ? AvatarURL : OldUserProfile->AvatarLargeURL;
	NewUserProfile.Email = OldUserProfile->Email;
	NewUserProfile.Language = Language;
	NewUserProfile.Timezone = Timezone;

	JusticePlatform::UpdatePlayerProfile(*FJusticeUserToken, NewUserProfile, FDefaultCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorString) {
		OnComplete.ExecuteIfBound(IsSuccess, ErrorString);
	}));
}