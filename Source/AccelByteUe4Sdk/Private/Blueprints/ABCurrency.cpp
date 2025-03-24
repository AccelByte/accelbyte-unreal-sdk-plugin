// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABCurrency.h"
#include "Api/AccelByteCurrencyApi.h"

using namespace AccelByte;

void UABCurrency::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABCurrency::GetCurrencyList( 
	FDArrayModelsCurrencyListResponse OnSuccess,
	FDErrorHandler OnError,
	EAccelByteCurrencyType CurrencyType) 
{
	const auto CurrencyPtr = ApiClientPtr->GetCurrencyApi().Pin();
	if (CurrencyPtr.IsValid())
	{
		CurrencyPtr->GetCurrencyList(
			ApiClientPtr->Settings->Namespace,
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
