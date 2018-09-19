// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/IdentityBlueprint/ReissueVerificationCode.h"
#include "Services/JusticeIdentity.h"

UAsyncReissueVerificationCode * UAsyncReissueVerificationCode::ReissueVerificationCode(FString UserID, FString LoginID)
{
	UAsyncReissueVerificationCode* Node = NewObject<UAsyncReissueVerificationCode>();
	Node->UserID = UserID;
	Node->LoginID = LoginID;
	return Node;
}

void UAsyncReissueVerificationCode::Activate()
{
	JusticeIdentity::ReissueVerificationCode(JusticeGameNamespace, this->UserID, this->LoginID, FDefaultCompleteDelegate::CreateLambda( [&](bool bSuccessful, FString ErrorStr) {
		if (bSuccessful)
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