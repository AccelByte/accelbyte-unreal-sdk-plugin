// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/PlatformBlueprint/RequestCurrentPlayerProfile.h"
#include "Services/JusticePlatform.h"

UAsyncRequestCurrentPlayerProfile * UAsyncRequestCurrentPlayerProfile::RequestCurrentPlayerProfile()
{
	UAsyncRequestCurrentPlayerProfile* Node = NewObject<UAsyncRequestCurrentPlayerProfile>();
	return Node;
}

void UAsyncRequestCurrentPlayerProfile::Activate()
{
	JusticePlatform::RequestCurrentPlayerProfile(FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr, UserProfileInfo UserProfile) {
		if (IsSuccess)
		{
			UUserProfileJustice* ResultUserProfile = NewObject<UUserProfileJustice>();
			ResultUserProfile->FromUserProfileInfo(UserProfile);
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(ResultUserProfile);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(NewObject<UUserProfileJustice>());
			}
		}
	}));
}