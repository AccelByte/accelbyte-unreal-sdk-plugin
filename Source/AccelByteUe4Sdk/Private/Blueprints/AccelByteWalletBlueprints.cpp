// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteWalletBlueprints.h"
#include "Core/AccelByteSettings.h"
#include "Api/AccelByteWalletApi.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::Wallet;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsWallet::GetWalletInfoByCurrencyCode(const FString& CurrencyCode, const FGetWalletByCurrencyCodeSuccess& OnSuccess, const FBlueprintErrorHandler& OnError)
{
	FRegistry::Wallet.GetWalletInfoByCurrencyCode(CurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda([OnSuccess](const FAccelByteModelsWalletInfo& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
