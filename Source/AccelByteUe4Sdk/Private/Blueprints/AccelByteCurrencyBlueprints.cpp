// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteCurrencyBlueprints.h"
#include "Core/AccelByteSettings.h"
#include "Api/AccelByteCurrencyApi.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::Api::Currency;
using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::Settings;
using AccelByte::Credentials;

void UAccelByteBlueprintsCurrency::GetCurrencyList(const FString& Namespace, const FGetCurrencyListSuccess& OnSuccess, const FBlueprintErrorHandler& OnError,
	EAccelByteCurrencyType CurrencyType = EAccelByteCurrencyType::NONE)
{
	FRegistry::Currency.GetCurrencyList(Namespace, THandler<TArray<FAccelByteModelsCurrencyList>>::CreateLambda([OnSuccess](const TArray<FAccelByteModelsCurrencyList>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
	FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}),
	CurrencyType);
}