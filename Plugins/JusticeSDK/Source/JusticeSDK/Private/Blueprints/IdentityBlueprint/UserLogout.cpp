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
	JusticeIdentity::UserLogout(FUserLogoutCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr) {
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