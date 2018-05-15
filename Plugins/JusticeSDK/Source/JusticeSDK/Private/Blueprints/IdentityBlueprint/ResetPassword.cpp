// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/ResetPassword.h"
#include "Services/JusticeIdentity.h"

UAsyncResetPassword * UAsyncResetPassword::ResetPassword(FString UserId, FString VerificationCode, FString NewPassword)
{
	UAsyncResetPassword* Node = NewObject<UAsyncResetPassword>();
	Node->UserId = UserId;
	Node->VerificationCode = VerificationCode;
	Node->NewPassword = NewPassword;
	return Node;
}

void UAsyncResetPassword::Activate()
{
	JusticeIdentity::ResetPassword(this->UserId, this->VerificationCode, this->NewPassword, FResetPasswordCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr) {
		if (IsSuccess)
		{
			OnSuccess.Broadcast();
		}
		else
		{
			OnFailed.Broadcast();
		}
	}));
}