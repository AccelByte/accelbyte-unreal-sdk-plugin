// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerOauth2.h"

void UABServerOauth2::SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerOauth2::LoginWithClientCredentials(
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerOauth2.LoginWithClientCredentials(
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerOauth2::GetJwks(
	FDJwkSetDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerOauth2.GetJwks(
		THandler<FJwkSet>::CreateLambda(
			[OnSuccess](FJwkSet const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerOauth2::ForgetAllCredentials()
{
	ApiClientPtr->ServerOauth2.ForgetAllCredentials();
}
