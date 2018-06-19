// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/UnlinkPlatform.h"
#include "Services/JusticeIdentity.h"

UAsyncUnlinkPlatform * UAsyncUnlinkPlatform::UnlinkPlatform(FString PlatformID)
{
	UAsyncUnlinkPlatform* Node = NewObject<UAsyncUnlinkPlatform>();
	Node->PlatformID = PlatformID;
	return Node;
}

void UAsyncUnlinkPlatform::Activate()
{
	JusticeIdentity::UnlinkPlatform(this->PlatformID, FUnlinkPlatformCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr) {
		if (bSuccessful)
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