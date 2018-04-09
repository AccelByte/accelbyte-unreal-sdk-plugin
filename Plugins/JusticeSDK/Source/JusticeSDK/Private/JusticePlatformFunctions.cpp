// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticePlatformFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "JusticePlatform.h"

void UJusticePlatformFunctions::RequestCurrentPlayerProfile(FProfileReqestCompleteDelegate OnComplete)
{
	JusticePlatform::RequestCurrentPlayerProfile(FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorString, UserProfileInfo userInfo) {
		UUserProfileJustice* UserProfile = NewObject<UUserProfileJustice>();
		UserProfile->FromUserProfileInfo(userInfo);
		OnComplete.Execute(IsSuccess, ErrorString, UserProfile);
	}));
}

void UJusticePlatformFunctions::UpdatePlayerProfile(FString DisplayName, FString FirstName, FString LastName, FString Country, FString AvatarUrl, FUpdatePlayerProfileCompleteDynamicDelegate OnComplete)
{
	UserProfileInfo* OldUserProfile = FJusticeSDKModule::Get().UserProfile;
	UserProfileInfo NewUserProfile;

	NewUserProfile.DisplayName = !DisplayName.IsEmpty() ? DisplayName : OldUserProfile->DisplayName;
	NewUserProfile.FirstName = !FirstName.IsEmpty() ? FirstName : OldUserProfile->FirstName;
	NewUserProfile.LastName = !LastName.IsEmpty() ? LastName : OldUserProfile->LastName;
	NewUserProfile.Country = !Country.IsEmpty() ? Country : OldUserProfile->Country;
	NewUserProfile.AvatarUrl = !AvatarUrl.IsEmpty() ? AvatarUrl : OldUserProfile->AvatarUrl;
	NewUserProfile.AvatarSmallUrl = !AvatarUrl.IsEmpty() ? AvatarUrl : OldUserProfile->AvatarSmallUrl;
	NewUserProfile.AvatarLargeUrl = !AvatarUrl.IsEmpty() ? AvatarUrl : OldUserProfile->AvatarLargeUrl;
	NewUserProfile.Namespace = OldUserProfile->Namespace;
	NewUserProfile.Email = OldUserProfile->Email;
	NewUserProfile.Status = OldUserProfile->Status;

	JusticePlatform::UpdatePlayerProfile(NewUserProfile, FUpdatePlayerProfileCompleteDelegate::CreateLambda([OnComplete](bool IsSuccess, FString ErrorString) {
		OnComplete.Execute(IsSuccess, ErrorString);
	}));

}
