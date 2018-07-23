// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/UserLogout.h"
#include "Services/JusticeIdentity.h"

UAsyncUserLogout * UAsyncUserLogout::UserLogout()
{
	UAsyncUserLogout* Node = NewObject<UAsyncUserLogout>();
	return Node;
}

void UAsyncUserLogout::Activate()
{
	JusticeIdentity::UserLogout(FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr) {
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
				OnFailed.Broadcast(ErrorStr);
			}
		}		
	}));
}