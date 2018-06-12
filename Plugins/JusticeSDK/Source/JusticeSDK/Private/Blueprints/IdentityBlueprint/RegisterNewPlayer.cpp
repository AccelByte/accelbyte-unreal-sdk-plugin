// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/RegisterNewPlayer.h"
#include "Services/JusticeIdentity.h"

UAsyncRegisterNewPlayer * UAsyncRegisterNewPlayer::RegisterNewPlayer(FString UserId, FString Password, FString DisplayName, UUserAuthTypeJustice AuthType)
{
	UAsyncRegisterNewPlayer* Node = NewObject<UAsyncRegisterNewPlayer>();
	Node->UserId = UserId;
	Node->Password = Password;
	Node->DisplayName = DisplayName;
	Node->AuthType = (FUserAuthTypeJustice)AuthType;
	return Node;
}

void UAsyncRegisterNewPlayer::Activate()
{
	JusticeIdentity::RegisterNewPlayer(this->UserId, this->Password, this->DisplayName, this->AuthType, FRegisterPlayerCompleteDelegate::CreateLambda( [&](bool IsSuccess, FString ErrorStr, UserCreateResponse* Response) {
		if (IsSuccess)
		{
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(UUserCreateResponse::Deserialize(Response));
			}
		}
		else
		{
			if (OnFailed.IsBound())
			{
				OnFailed.Broadcast(nullptr);
			}
		}
	}));
}