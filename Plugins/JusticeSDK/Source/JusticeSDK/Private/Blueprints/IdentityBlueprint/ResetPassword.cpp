// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/ResetPassword.h"
#include "Services/JusticeIdentity.h"

UAsyncResetPassword * UAsyncResetPassword::ResetPassword(FString UserID, FString VerificationCode, FString NewPassword)
{
	UAsyncResetPassword* Node = NewObject<UAsyncResetPassword>();
	Node->UserID = UserID;
	Node->VerificationCode = VerificationCode;
	Node->NewPassword = NewPassword;
	return Node;
}

void UAsyncResetPassword::Activate()
{
	JusticeIdentity::ResetPassword(this->UserID, this->VerificationCode, this->NewPassword, FResetPasswordCompleteDelegate::CreateLambda([&](bool bSuccessful, FString ErrorStr) {
		if (bSuccessful)
		{
			if (OnSuccess.IsBound())
			{
				OnSuccess.Broadcast(TEXT(""));
			}
		}
		else
		{
			if (OnSuccess.IsBound())
			{
				OnFailed.Broadcast(ErrorStr);
			}
		}
	}));
}