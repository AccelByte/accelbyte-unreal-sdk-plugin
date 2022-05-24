// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABWallet.h"
#include "Api/AccelByteWalletApi.h"
#include "Core/AccelByteRegistry.h"

void UABWallet::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABWallet::GetWalletInfoByCurrencyCode(
	FString const& CurrencyCode,
	FDAccelByteModelsWalletInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Wallet.GetWalletInfoByCurrencyCode(
		CurrencyCode,
		THandler<FAccelByteModelsWalletInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsWalletInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}
