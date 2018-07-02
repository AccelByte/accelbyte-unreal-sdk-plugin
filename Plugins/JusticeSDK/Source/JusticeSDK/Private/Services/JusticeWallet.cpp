// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeWallet.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "Services/JusticeIdentity.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "Models/WalletInfo.h"

void JusticeWallet::GetWalletBalance(FString CurrencyCode, FGetWalletBalanceCompleteDelegate OnComplete)
{
	FString Authorization	= FJusticeHTTP::BearerAuth(FJusticeUserToken->AccessToken);
	FString URL				= FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/wallets/%s"), *FJusticeBaseURL, *FJusticeNamespace, *FJusticeUserID, *CurrencyCode);
	FString Verb			= GET;
	FString ContentType		= TYPE_JSON;
	FString Accept			= TYPE_JSON;
	FString Payload			= TEXT("");

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeWallet::OnGetWalletBalanceResponse, OnComplete));

}
void JusticeWallet::OnGetWalletBalanceResponse(FJusticeHttpResponsePtr Response, FGetWalletBalanceCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get wallet balance Failed. Error Message: %s"), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, 0);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::Ok:
	{
		WalletInfo* wallet = new WalletInfo();
		if (wallet->FromJson(Response->Content))
		{
			OnComplete.ExecuteIfBound(true, wallet->balance);
		}
		else
		{
			ErrorStr = TEXT("unable to deserialize response from server");
		}
		break;
	}
	case EHttpResponseCodes::Denied:
		JusticeIdentity::UserRefreshToken(
			FUserLoginCompleteDelegate::CreateLambda([&](bool bSuccessful, FString InnerErrorStr, OAuthTokenJustice* Token) {
			if (bSuccessful)
			{
				if (Token->Bans.Num() > 0)
				{
					FString bansList = FString::Join(Token->Bans, TEXT(","));
					ErrorStr = FString::Printf(TEXT("You got banned, Ban List=%s"), *bansList);
					OnComplete.ExecuteIfBound(false, 0);
				}
				else
				{
					if (Response->TooManyRetries() || Response->TakesTooLong())
					{
						OnComplete.ExecuteIfBound(false, 0);
						return;
					}
					Response->UpdateRequestForNextRetry();
					FJusticeRetryManager->AddQueue(Response->JusticeRequest,
						Response->NextWait,
						FWebRequestResponseDelegate::CreateStatic(JusticeWallet::OnGetWalletBalanceResponse, OnComplete));
					return;
				}
			}
			else
			{
				ErrorStr = FString::Printf(TEXT("Your token is expired, but we cannot refresh your token. Error: %s"), *InnerErrorStr);
				OnComplete.ExecuteIfBound(false, 0);
				return;
			}
		}));
		return;
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
		UE_LOG(LogJustice, Log, TEXT("Get Wallet Balance Error, Attempt Retry : %s"), *ErrorStr);
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			OnComplete.ExecuteIfBound(false, 0);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeWallet::OnGetWalletBalanceResponse, OnComplete));
		return;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected Response Code: %d, Content: %s"), Response->Code, *Response->Content);
	}
	
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Get Wallet Balance Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, 0);
		return;
	}
}