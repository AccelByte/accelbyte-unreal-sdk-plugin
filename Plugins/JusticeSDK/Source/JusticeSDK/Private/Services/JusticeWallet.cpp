// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeWallet.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "Utilities/AWSXRayJustice.h"
#include "Utilities/HTTPJustice.h"
#include "Models/WalletInfo.h"

void JusticeWallet::GetWalletBalance(FString CurrencyCode, FGetWalletBalanceCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;
	FString UserID = FJusticeSDKModule::Get().UserToken->UserId;

	Request->SetURL(FString::Printf(TEXT("%s/platform/public/namespaces/%s/users/%s/wallets/%s"), *BaseURL, *Namespace, *UserID, *CurrencyCode));	
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	UE_LOG(LogJustice, Log, TEXT("Attemp to call GetWalletBalance: %s"), *Request->GetURL());

	Request->OnProcessRequestComplete().BindStatic(JusticeWallet::OnGetWalletBalanceComplete, RequestTrace, OnComplete, CurrencyCode);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeWallet::GetWalletBalance failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.ExecuteIfBound(false, 0);
	}
}
void JusticeWallet::OnGetWalletBalanceComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetWalletBalanceCompleteDelegate OnComplete, FString CurrencyCode)
{
	FString ErrorStr;
	if (!bSuccessful || !Response.IsValid())
	{
		ErrorStr = TEXT("request failed");
	}
	else
	{
		switch (Response->GetResponseCode())
		{
		case EHttpResponseCodes::Ok:
		{
			FString ResponseStr = Response->GetContentAsString();
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
			{				
				WalletInfo* wallet = new WalletInfo();				
				if (wallet->FromJson(JsonObject))
				{
					OnComplete.ExecuteIfBound(true, wallet->balance);					
				}
				else
				{
					ErrorStr = TEXT("unable to deserlize response from server");
				}
			}
			else
			{
				ErrorStr = TEXT("unable to deserlize response from server");
			}
			break;
		}
		case EHttpResponseCodes::Denied:
		case EHttpResponseCodes::RequestTimeout:
		case EHttpResponseCodes::ServerError:
		case EHttpResponseCodes::ServiceUnavail:
		case EHttpResponseCodes::GatewayTimeout:
		{
			JusticeWallet::GetWalletBalance(CurrencyCode, OnComplete);
			return;
		}
		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d, response content=%s"), Response->GetResponseCode(), *Response->GetContentAsString());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("GetJusticeWallet::GetWalletBalance Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, 0);
		return;
	}
}