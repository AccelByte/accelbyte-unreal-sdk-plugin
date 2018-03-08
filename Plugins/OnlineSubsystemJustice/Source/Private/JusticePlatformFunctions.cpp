// Fill out your copyright notice in the Description page of Project Settings.

#include "JusticePlatformFunctions.h"
#include "AWSXRayJustice.h"
#include "HTTPJustice.h"
#include "FJusticeComponent.h"
#include "Misc/ConfigCacheIni.h"


void UJusticePlatformFunctions::RequestCurrentPlayerProfile(FProfileReqestCompleteDelegate OnComplete)
{
	FString ErrorStr;
	FString BaseURL;
	FString Namespace;
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	TSharedRef<FAWSXRayJustice> RequestTrace = MakeShareable(new FAWSXRayJustice());
	UFJusticeComponent* JusticeComponent = UFJusticeComponent::GetInstance();

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("Namespace"), Namespace, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing Namespace= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	

	Request->SetURL(BaseURL + TEXT("/platform/namespace/") + JusticeComponent->UserToken->Namespace + TEXT("/players/") + JusticeComponent->UserToken->UserId);
	Request->SetHeader(TEXT("Authorization"), FHTTPJustice::BearerAuth(JusticeComponent->UserToken->AccessToken));
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("X-Amzn-TraceId"), RequestTrace->XRayTraceID());

	UE_LOG(LogOnline, VeryVerbose, TEXT("Attemp to call Player Profile: %s"), *Request->GetURL());

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
			UE_LOG(LogOnline, Error, TEXT("Get Player Profile Error : %s"), *ErrorStr);
			OnComplete.Execute(false, ErrorStr, nullptr);
			return;
		}

	}, JusticeComponent->UserToken->UserId, RequestTrace);
	if (!Request->ProcessRequest())
	{
		ErrorStr = FString::Printf(TEXT("request failed. URL=%s"), *Request->GetURL());
	}
	if (!ErrorStr.IsEmpty())
	{
		UE_LOG_ONLINE(Warning, TEXT("UFJusticeComponent::RequestCurrentPlayerProfile failed. Error=%s XrayID=%s ReqTime=%.3f"), *ErrorStr, *RequestTrace->ToString(), Request->GetElapsedTime());
		OnComplete.Execute(false, ErrorStr, nullptr);
	}
}
