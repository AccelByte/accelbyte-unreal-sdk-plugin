// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/VerifyNewPlayer.h"

UAsyncVerifyNewPlayer * UAsyncVerifyNewPlayer::VerifyNewPlayer(FString UserID, FString VerificationCode, UUserAuthTypeJustice AuthType)
{
	UAsyncVerifyNewPlayer* Node = NewObject<UAsyncVerifyNewPlayer>();
	Node->UserID = UserID;
	Node->VerificationCode = VerificationCode;	
	Node->AuthType = (FUserAuthTypeJustice)AuthType;
	return Node;
}

void UAsyncVerifyNewPlayer::Activate()
{
	JusticeIdentity::VerifyNewPlayer(JusticeGameNamespace, this->UserID, this->VerificationCode, this->AuthType, FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr) {
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