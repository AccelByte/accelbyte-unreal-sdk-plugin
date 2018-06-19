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
	JusticeIdentity::DeviceLogin(FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, OAuthTokenJustice* Token) {
		if (bSuccessful)
		{
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(UOAuthTokenJustice::Deserialize(Token), TEXT(""));
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(nullptr, ErrorStr);
			}
		}
	}));
}