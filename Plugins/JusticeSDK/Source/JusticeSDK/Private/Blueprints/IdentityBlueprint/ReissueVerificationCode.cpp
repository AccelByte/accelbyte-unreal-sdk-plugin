// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/ReissueVerificationCode.h"
#include "Services/JusticeIdentity.h"

UAsyncReissueVerificationCode * UAsyncReissueVerificationCode::ReissueVerificationCode(FString UserId, FString LoginId)
{
	UAsyncReissueVerificationCode* Node = NewObject<UAsyncReissueVerificationCode>();
	Node->UserId = UserId;
	Node->LoginId = LoginId;
	return Node;
}

void UAsyncReissueVerificationCode::Activate()
{
	JusticeIdentity::ReissueVerificationCode(this->UserId, this->LoginId, FVerifyNewPlayerCompleteDelegate::CreateLambda( [&](bool IsSuccess, FString ErrorStr) {
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