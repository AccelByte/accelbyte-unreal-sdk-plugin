// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "HTTPJustice.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "Utilities/AWSXRayJustice.h"
#include "JusticeSDK.h"


FString FJusticeHTTP::BasicAuth(const FString& Username, const FString& Password)
{
	FString User = Username.IsEmpty() ? FJusticeSDKModule::Get().ClientID : Username;
	FString Pass = Password.IsEmpty() ? FJusticeSDKModule::Get().ClientSecret : Password;
	return TEXT("Basic " + FBase64::Encode(User + ":" + Pass));
}

FString FJusticeHTTP::BearerAuth(const FString& Token)
{
	return TEXT("Bearer " + Token);
}

void FJusticeHTTP::CreateRequest(FString Authorization, FString URL, FString Verb, FString ContentType, FString Accept, FString Content, FWebRequestResponseDelegate OnResponse)
{
	FString ErrorStr;
	TSharedRef<IJusticeHttpRequest> Request = FJusticeHttpModule::Get().CreateRequest();
	FString AmazonTraceID = FAWSXRayJustice::XRayTraceID();

	Request->SetURL(URL);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetHeader(TEXT("X-Amzn-TraceId"), AmazonTraceID);
	Request->SetContentAsString(Content);    

    //Request->OnRequestProgress().bind

	Request->OnProcessRequestComplete().BindLambda([=](FJusticeHttpRequestPtr Request, FJusticeHttpResponsePtr Response, bool bSuccessful) {

        TSharedPtr<JusticeHttpRequest> JusticeRequest = MakeShareable(new JusticeHttpRequest());
		JusticeRequest->Accept = Accept;
		JusticeRequest->Authorization = Authorization;
		JusticeRequest->Content = Content;
		JusticeRequest->ContentType = ContentType;
		JusticeRequest->ElapsedTime = 0;
		JusticeRequest->RetryAttempt = 0;
		JusticeRequest->URL = URL;
		JusticeRequest->Verb = Verb;

        TSharedPtr<JusticeHttpResponse, ESPMode::ThreadSafe> JusticeResponse = MakeShareable(new JusticeHttpResponse());
		JusticeResponse->AmazonTraceID = AmazonTraceID;
		JusticeResponse->JusticeRequest = JusticeRequest;


		JusticeResponse->Request = Request;
		JusticeResponse->Response = Response;
		JusticeResponse->bSuccessful = bSuccessful;

		if (!bSuccessful || !Response.IsValid())
		{
			JusticeResponse->ErrorString = TEXT("Request failed");
			OnResponse.ExecuteIfBound(JusticeResponse);
			return;
		}
		JusticeResponse->Content = Response->GetContentAsString();
		JusticeResponse->Code = Response->GetResponseCode();
		OnResponse.ExecuteIfBound(JusticeResponse);
	});


	if (!Request->ProcessRequest())
	{
        TSharedPtr<JusticeHttpResponse, ESPMode::ThreadSafe> JusticeResponse = MakeShareable(new JusticeHttpResponse());
		JusticeResponse->AmazonTraceID = AmazonTraceID;
		JusticeResponse->ErrorString = FString::Printf(TEXT("Web Request Failed. URL=%s"), *Request->GetURL());
		OnResponse.ExecuteIfBound(JusticeResponse);
	}
}

void FJusticeHTTP::CreateRequest(TSharedPtr<JusticeHttpRequest> JusticeRequest, FWebRequestResponseDelegate OnResponse)
{
	FString ErrorStr;
	TSharedRef<IJusticeHttpRequest> Request = FJusticeHttpModule::Get().CreateRequest();
	FString AmazonTraceID = FAWSXRayJustice::XRayTraceID();

	Request->SetURL(JusticeRequest->URL);
	Request->SetHeader(TEXT("Authorization"), JusticeRequest->Authorization);
	Request->SetVerb(JusticeRequest->Verb);
	Request->SetHeader(TEXT("Content-Type"), JusticeRequest->ContentType);
	Request->SetHeader(TEXT("Accept"), JusticeRequest->Accept);
	Request->SetHeader(TEXT("X-Amzn-TraceId"), AmazonTraceID);
	Request->SetContentAsString(JusticeRequest->Content);

    TSharedPtr<JusticeHttpResponse, ESPMode::ThreadSafe> JusticeResponse = MakeShareable(new JusticeHttpResponse());
	JusticeResponse->AmazonTraceID = AmazonTraceID;
	JusticeResponse->JusticeRequest = JusticeRequest;
    
	Request->OnProcessRequestComplete().BindLambda([=](FJusticeHttpRequestPtr Request, FJusticeHttpResponsePtr Response, bool bSuccessful) {
		JusticeResponse->Request = Request;
		JusticeResponse->Response = Response;
		JusticeResponse->bSuccessful = bSuccessful;

		if (!bSuccessful || !Response.IsValid())
		{
			JusticeResponse->ErrorString = TEXT("Request failed");
			OnResponse.ExecuteIfBound(JusticeResponse);
			return;
		}
		JusticeResponse->Content = Response->GetContentAsString();
		JusticeResponse->Code = Response->GetResponseCode();
		OnResponse.ExecuteIfBound(JusticeResponse);
	});
	if (!Request->ProcessRequest())
	{
		JusticeResponse->ErrorString = FString::Printf(TEXT("Web Request Failed. URL=%s"), *Request->GetURL());
		OnResponse.ExecuteIfBound(JusticeResponse);
	}
}