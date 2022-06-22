// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABCurrency.h"
#include "Api/AccelByteCurrencyApi.h"
#include "Core/AccelByteRegistry.h"

void UABCurrency::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABCurrency::GetCurrencyList( 
	FDArrayModelsCurrencyListResponse OnSuccess,
	FDErrorHandler OnError,
	EAccelByteCurrencyType CurrencyType) 
{
	ApiClientPtr->Currency.GetCurrencyList(
		FRegistry::Settings.Namespace,
		THandler<TArray<FAccelByteModelsCurrencyList>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsCurrencyList> const& Response)
			{
				FArrayModelsCurrencyList Result;
				Result.Content = Response;
				OnSuccess.ExecuteIfBound(Result);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}), CurrencyType);
}
