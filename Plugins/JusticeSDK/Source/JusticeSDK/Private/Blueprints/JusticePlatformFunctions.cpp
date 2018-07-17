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
	JusticePlatform::RequestCurrentPlayerProfile(FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorString, UserProfileInfo userInfo) {
		UUserProfileJustice* UserProfile = UUserProfileJustice::Deserialize(userInfo);
		check(UserProfile);
		OnComplete.ExecuteIfBound(IsSuccess, ErrorString, UserProfile);
	}));
}

void UJusticePlatformFunctions::UpdatePlayerProfile(FString DisplayName, FString FirstName, FString LastName, FString Country, FString AvatarUrl, FString Language, FString Timezone, FUpdatePlayerProfileCompleteDynamicDelegate OnComplete)
{
	UserProfileInfo* OldUserProfile = FJusticeSDKModule::Get().UserProfile;
	UserProfileInfoUpdate NewUserProfile;

	NewUserProfile.DisplayName = !DisplayName.IsEmpty() ? DisplayName : OldUserProfile->DisplayName;
	NewUserProfile.FirstName = !FirstName.IsEmpty() ? FirstName : OldUserProfile->FirstName;
	NewUserProfile.LastName = !LastName.IsEmpty() ? LastName : OldUserProfile->LastName;
	NewUserProfile.AvatarUrl = !AvatarUrl.IsEmpty() ? AvatarUrl : OldUserProfile->AvatarUrl;
	NewUserProfile.AvatarSmallUrl = !AvatarUrl.IsEmpty() ? AvatarUrl : OldUserProfile->AvatarSmallUrl;
	NewUserProfile.AvatarLargeUrl = !AvatarUrl.IsEmpty() ? AvatarUrl : OldUserProfile->AvatarLargeUrl;
	NewUserProfile.Email = OldUserProfile->Email;
	NewUserProfile.Language = Language;
	NewUserProfile.Timezone = Timezone;

	JusticePlatform::UpdatePlayerProfile(NewUserProfile, FUpdatePlayerProfileCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorString) {
		OnComplete.ExecuteIfBound(IsSuccess, ErrorString);
	}));
}