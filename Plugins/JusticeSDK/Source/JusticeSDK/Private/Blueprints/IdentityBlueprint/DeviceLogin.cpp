// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/DeviceLogin.h"
#include "Services/JusticeIdentity.h"

UAsyncDeviceLogin * UAsyncDeviceLogin::DeviceLogin()
{
	UAsyncDeviceLogin* Node = NewObject<UAsyncDeviceLogin>();
	return Node;
}

void UAsyncDeviceLogin::Activate()
{
	JusticeIdentity::DeviceLogin(FUserLoginCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* Token) {
		if (IsSuccess)
		{
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(Token);
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(NewObject<UOAuthTokenJustice>());
			}
		}
	}));
}