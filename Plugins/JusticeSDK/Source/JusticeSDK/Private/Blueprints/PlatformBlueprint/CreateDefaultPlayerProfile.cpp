// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/PlatformBlueprint/CreateDefaultPlayerProfile.h"
#include "Services/JusticePlatform.h"

UAsyncCreateDefaultPlayerProfile * UAsyncCreateDefaultPlayerProfile::CreateDefaultPlayerProfile(FString Email, FString DisplayName)
{
	UAsyncCreateDefaultPlayerProfile* Node = NewObject<UAsyncCreateDefaultPlayerProfile>();
	Node->Email = Email;
	Node->DisplayName = DisplayName;
	return Node;
}

void UAsyncCreateDefaultPlayerProfile::Activate()
{
	JusticePlatform::CreateDefaultPlayerProfile(this->Email, this->DisplayName, FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr) {
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