// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/PlatformBlueprint/UpdatePlayerProfile.h"
#include "Services/JusticePlatform.h"

UAsyncUpdatePlayerProfile * UAsyncUpdatePlayerProfile::UpdatePlayerProfile(FString DisplayName, FString FirstName, FString LastName, FString Country, FString AvatarUrl)
{
	UAsyncUpdatePlayerProfile* Node = NewObject<UAsyncUpdatePlayerProfile>();
	Node->DisplayName = DisplayName;
	Node->FirstName = FirstName;
	Node->LastName = LastName;
	Node->Country = Country;
	Node->AvatarUrl = AvatarUrl;
	return Node;
}

void UAsyncUpdatePlayerProfile::Activate()
{
	UserProfileInfo* OldUserProfile = FJusticeSDKModule::Get().UserProfile;
	UserProfileInfo NewUserProfile;

	NewUserProfile.DisplayName = !DisplayName.IsEmpty() ? this->DisplayName : OldUserProfile->DisplayName;
	NewUserProfile.FirstName = !FirstName.IsEmpty() ? this->FirstName : OldUserProfile->FirstName;
	NewUserProfile.LastName = !LastName.IsEmpty() ? this->LastName : OldUserProfile->LastName;
	NewUserProfile.Country = !Country.IsEmpty() ? this->Country : OldUserProfile->Country;
	NewUserProfile.AvatarUrl = !AvatarUrl.IsEmpty() ? this->AvatarUrl : OldUserProfile->AvatarUrl;
	NewUserProfile.AvatarSmallUrl = !AvatarUrl.IsEmpty() ? this->AvatarUrl : OldUserProfile->AvatarSmallUrl;
	NewUserProfile.AvatarLargeUrl = !AvatarUrl.IsEmpty() ? this->AvatarUrl : OldUserProfile->AvatarLargeUrl;
	NewUserProfile.Namespace = OldUserProfile->Namespace;
	NewUserProfile.Email = OldUserProfile->Email;
	NewUserProfile.Status = OldUserProfile->Status;

	JusticePlatform::UpdatePlayerProfile(NewUserProfile, FUpdatePlayerProfileCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorString) {
		if (IsSuccess)
		{
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast();
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast();
			}
		}
	}));
}