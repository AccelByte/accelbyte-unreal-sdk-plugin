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
	JusticePlatform::RequestCurrentPlayerProfile(*FJusticeUserToken, FRequestCurrentPlayerProfileCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, FUserProfileInfo* UserProfile) {
		if (bSuccessful)
		{
			UUserProfileJustice* ResultUserProfile = UUserProfileJustice::Deserialize(UserProfile);
			check(ResultUserProfile);
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(ResultUserProfile);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(nullptr);
			}
		}
	}));
}