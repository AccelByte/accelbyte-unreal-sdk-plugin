// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/UserLogin.h"
#include "Services/JusticeIdentity.h"

UAsyncUserLogin * UAsyncUserLogin::UserLogin(FString UserId, FString Password)
{
	UAsyncUserLogin* Node = NewObject<UAsyncUserLogin>();
	Node->UserId = UserId;
	Node->Password = Password;
	return Node;
}

void UAsyncUserLogin::Activate()
{
	JusticeIdentity::UserLogin(this->UserId, this->Password, FUserLoginCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* Token) {
		if (IsSuccess)
		{
			OnSuccess.Broadcast(Token);
		}
		else
		{
			OnFailed.Broadcast(NewObject<UOAuthTokenJustice>());
		}		
	}));
}