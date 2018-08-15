// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/UserLogin.h"
#include "Services/JusticeIdentity.h"

UAsyncUserLogin * UAsyncUserLogin::UserLogin(FString UserID, FString Password)
{
	UAsyncUserLogin* Node = NewObject<UAsyncUserLogin>();
	Node->UserID = UserID;
	Node->Password = Password;
	return Node;
}

void UAsyncUserLogin::Activate()
{
	JusticeIdentity::UserLogin(this->UserID, this->Password, FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr, FOAuthTokenJustice* Token) {
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