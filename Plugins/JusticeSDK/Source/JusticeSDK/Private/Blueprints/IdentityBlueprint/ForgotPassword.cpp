// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/ForgotPassword.h"
#include "Services/JusticeIdentity.h"

UAsyncForgotPassword * UAsyncForgotPassword::ForgotPassword(FString LoginID)
{
	UAsyncForgotPassword* Node = NewObject<UAsyncForgotPassword>();
	Node->LoginID = LoginID;
	return Node;
}

void UAsyncForgotPassword::Activate()
{
	JusticeIdentity::ForgotPassword(JusticeGameNamespace, this->LoginID, FDefaultCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr) {
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