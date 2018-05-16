// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/VerifyNewPlayer.h"
#include "Services/JusticeIdentity.h"

UAsyncVerifyNewPlayer * UAsyncVerifyNewPlayer::VerifyNewPlayer(FString UserId, FString VerificationCode)
{
	UAsyncVerifyNewPlayer* Node = NewObject<UAsyncVerifyNewPlayer>();
	Node->UserId = UserId;
	Node->VerificationCode = VerificationCode;
	return Node;
}

void UAsyncVerifyNewPlayer::Activate()
{
	JusticeIdentity::VerifyNewPlayer(this->UserId, this->VerificationCode, FVerifyNewPlayerCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr) {
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