// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABWallet.h"
#include "Api/AccelByteWalletApi.h"


using namespace AccelByte;

void UABWallet::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABWallet::GetWalletInfoByCurrencyCode(
	FString const& CurrencyCode,
	FDAccelByteModelsWalletInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	const auto WalletPtr = ApiClientPtr->GetWalletApi().Pin();
	if (WalletPtr.IsValid())
	{
		WalletPtr->GetWalletInfoByCurrencyCode(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABWallet::GetWalletInfoByCurrencyCodeV2(
	FString const& CurrencyCode,
	FDAccelByteModelsWalletInfosResponse OnSuccess,
	FDErrorHandler OnError) 
{
	const auto WalletPtr = ApiClientPtr->GetWalletApi().Pin();
	if (WalletPtr.IsValid())
	{
		WalletPtr->GetWalletInfoByCurrencyCodeV2(
		CurrencyCode,
		THandler<FAccelByteModelsWalletInfoResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsWalletInfoResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
