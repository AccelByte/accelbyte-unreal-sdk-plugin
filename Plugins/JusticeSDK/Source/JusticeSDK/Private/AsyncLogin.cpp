// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AsyncLogin.h"
#include "Misc/ConfigCacheIni.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformMisc.h"
#include "JusticeLog.h"
#include "Runtime/JsonUtilities/Public/JsonObjectConverter.h"
#include "Private/Models/OrderCreate.h"
#include "JusticeIdentity.h"
#include "JusticeSDK.h"

UAsyncLogin * UAsyncLogin::Login(FString UserId, FString Password)
{
	UAsyncLogin* Node = NewObject<UAsyncLogin>();
	Node->UserId = UserId;
	Node->Password = Password;
	return Node;
}

void UAsyncLogin::Activate() 
{
	JusticeIdentity::Login(this->UserId, this->Password, FGrantTypeJustice::PasswordGrant, FUserLoginCompleteDelegate::CreateLambda([&](bool IsSuccess, FString ErrorStr, UOAuthTokenJustice* Token) {
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