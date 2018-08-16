// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/WebLoginRefresh.h"
#include "Services/JusticeIdentity.h"

UWebLoginRefresh* UWebLoginRefresh::WebLoginRefresh(FString UserRefreshToken)
{
	UWebLoginRefresh* Node = NewObject<UWebLoginRefresh>();
	Node->UserRefreshToken = UserRefreshToken;
	return Node;
}

void UWebLoginRefresh::Activate()
{
	JusticeIdentity::SetRefreshToken(this->UserRefreshToken);
	JusticeIdentity::UserRefreshToken(FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, FOAuthTokenJustice* Token) {
	
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