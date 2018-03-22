// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticePlatformFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeSDK.h"
#include "JusticeLog.h"


void UJusticePlatformFunctions::RequestCurrentPlayerProfile(FProfileReqestCompleteDelegate OnComplete)
{
	FString ErrorStr;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());

	FString BaseURL = FJusticeSDKModule::Get().BaseURL;
	FString Namespace = FJusticeSDKModule::Get().Namespace;

	//string loginUrl = baseUrl + "/platform/public/profiles/namespaces/" + UserAccount.Token.Namespace + "/users/" + UserAccount.Token.UserId;
	Request->SetURL(BaseURL + TEXT("/platform/public/profiles/namespaces/") + FJusticeSDKModule::Get().Namespace + TEXT("/users/") + FJusticeSDKModule::Get().UserToken->UserId);
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(FJusticeSDKModule::Get().UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	UE_LOG(LogJustice, VeryVerbose, TEXT("Attemp to call Player Profile: %s"), *Request->GetURL());

	//Request->OnProcessRequestComplete().BindUObject(this, &UFJusticeComponent::TokenPlayerProfileComplete, JusticeUserId, RequestTrace);
	Request->OnProcessRequestComplete().BindLambda([OnComplete](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful, FString UserId, TSharedRef<FAWSXRayJustice> RequestTrace) {
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
				UUserProfileJustice* pUserProfileJustice = NewObject<UUserProfileJustice>();
				TSharedPtr<FJsonObject> JsonObject;
				TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseStr);
				if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
				{
					if (!pUserProfileJustice->FromJson(JsonObject))
					{
						ErrorStr = TEXT("unable to deserlize response from json object");
					}
					OnComplete.Execute(true, ErrorStr, pUserProfileJustice);
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
			UE_LOG(LogJustice, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
			OnComplete.Execute(false, ErrorStr, nullptr);
			return;
		}

	}, FJusticeSDKModule::Get().UserToken->UserId, RequestTrace);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Warning, TEXT("UFJusticeComponent::RequestCurrentPlayerProfile failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr, nullptr);
	}
}
