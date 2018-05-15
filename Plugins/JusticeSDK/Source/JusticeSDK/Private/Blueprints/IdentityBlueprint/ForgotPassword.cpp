// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/ForgotPassword.h"
#include "Services/JusticeIdentity.h"

UAsyncForgotPassword * UAsyncForgotPassword::ForgotPassword(FString LoginId)
{
	UAsyncForgotPassword* Node = NewObject<UAsyncForgotPassword>();
	Node->LoginId = LoginId;
	return Node;
}

void UAsyncForgotPassword::Activate()
{
	JusticeIdentity::ForgotPassword(this->LoginId, FForgotPasswordCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr) {
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