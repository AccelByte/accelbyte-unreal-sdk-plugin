// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeWallet.h"
#include "JusticePlatformFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"
#include "Private/Models/WalletInfo.h"


void JusticeWallet::GetWalletBalance(FGetWalletBalanceCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	// {{justice_url}}/platform/public/namespaces/{{namespace}}/users/{{user_id}}/wallets/DOGECOIN
	Request->SetURL(BaseURL + TEXT("/platform/public/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/users/") + FJusticeSDKModule::Get().UserToken->UserId + TEXT("/wallets/DOGECOIN"));
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	//Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());
	UE_LOG(LogJustice, Log, TEXT("Attemp to call GetWalletBalance: %s"), *Request->GetURL());

	Request->OnProcessRequestComplete().BindStatic(JusticeWallet::OnGetWalletBalanceComplete, RequestTrace, OnComplete);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("JusticeWallet::GetWalletBalance failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, 0);
	}
}

void JusticeWallet::OnGetWalletBalanceComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, TSharedRef<FAWSXRayJustice> RequestTrace, FGetWalletBalanceCompleteDelegate OnComplete)
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
					OnComplete.Execute(true, wallet->balances[0].balance);					
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
		}
		break;

		default:
			ErrorStr = FString::Printf(TEXT("unexpcted response Code=%d"), Response->GetResponseCode());
		}
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("GetJusticeWallet::GetWalletBalance Error : %s"), *ErrorStr);
		OnComplete.Execute(false, 0);
		return;
	}

}
