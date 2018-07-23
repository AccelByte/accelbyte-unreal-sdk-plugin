// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/PlatformBlueprint/UpdatePlayerProfile.h"
#include "Services/JusticePlatform.h"

UAsyncUpdatePlayerProfile * UAsyncUpdatePlayerProfile::UpdatePlayerProfile(FString DisplayName, FString FirstName, FString LastName, FString Country, FString AvatarUrl, FString Language, FString Timezone, FString DateOfBirth, TMap<FString, FString> CustomAttributes)
{
	UAsyncUpdatePlayerProfile* Node = NewObject<UAsyncUpdatePlayerProfile>();
	Node->DisplayName = DisplayName;
	Node->FirstName = FirstName;
	Node->LastName = LastName;
	Node->AvatarUrl = AvatarUrl;
	Node->Language = Language;
	Node->Timezone = Timezone;
	Node->DateOfBirth = DateOfBirth;
	Node->CustomAttributes = CustomAttributes;
	return Node;
}

void UAsyncUpdatePlayerProfile::Activate()
{
	UserProfileInfo* OldUserProfile = FJusticeSDKModule::Get().UserProfile;
	UserProfileInfoUpdate NewUserProfile;

	NewUserProfile.DisplayName = !DisplayName.IsEmpty() ? this->DisplayName : OldUserProfile->DisplayName;
	NewUserProfile.FirstName = !FirstName.IsEmpty() ? this->FirstName : OldUserProfile->FirstName;
	NewUserProfile.LastName = !LastName.IsEmpty() ? this->LastName : OldUserProfile->LastName;
	NewUserProfile.AvatarUrl = !AvatarUrl.IsEmpty() ? this->AvatarUrl : OldUserProfile->AvatarUrl;
	NewUserProfile.AvatarSmallUrl = !AvatarUrl.IsEmpty() ? this->AvatarUrl : OldUserProfile->AvatarSmallUrl;
	NewUserProfile.AvatarLargeUrl = !AvatarUrl.IsEmpty() ? this->AvatarUrl : OldUserProfile->AvatarLargeUrl;
	NewUserProfile.Email = OldUserProfile->Email;
	NewUserProfile.Status = OldUserProfile->Status;
	NewUserProfile.Language = !Language.IsEmpty() ? this->Language : OldUserProfile->Language;
	NewUserProfile.Timezone = !Timezone.IsEmpty() ? this->Timezone : OldUserProfile->Timezone;
	NewUserProfile.DateOfBirth = !DateOfBirth.IsEmpty() ? this->DateOfBirth : OldUserProfile->DateOfBirth;
	NewUserProfile.CustomAttributes = CustomAttributes.Num()!=0 ? this->CustomAttributes : OldUserProfile->CustomAttributes;

	JusticePlatform::UpdatePlayerProfile(NewUserProfile, FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorString) {
		if (bSuccessful)
		{
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(TEXT(""));
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(ErrorString);
			}
		}
	}));
}