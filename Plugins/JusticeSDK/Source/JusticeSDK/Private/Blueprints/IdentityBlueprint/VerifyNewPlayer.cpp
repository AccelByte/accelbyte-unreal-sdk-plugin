// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/VerifyNewPlayer.h"

UAsyncVerifyNewPlayer * UAsyncVerifyNewPlayer::VerifyNewPlayer(FString UserId, FString VerificationCode, UUserAuthTypeJustice AuthType)
{
	UAsyncVerifyNewPlayer* Node = NewObject<UAsyncVerifyNewPlayer>();
	Node->UserId = UserId;
	Node->VerificationCode = VerificationCode;	
	Node->AuthType = (FUserAuthTypeJustice)AuthType;
	return Node;
}

void UAsyncVerifyNewPlayer::Activate()
{
	JusticeIdentity::VerifyNewPlayer(this->UserId, this->VerificationCode, this->AuthType, FVerifyNewPlayerCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr) {
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