// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "HAL/ThreadSafeCounter.h"

#include "UObject/CoreOnline.h"
#include "Runtime/Online/HTTP/Public/Http.h"



struct JusticeHttpRequest
{
	FString Authorization;
	FString URL;
	FString Verb;
	FString ContentType;
	FString Accept;
	FString Content;
	//FWebRequestResponseDelegate OnResponse;

	int RetryAttempt;
	float ElapsedTime;
};
typedef TSharedPtr<JusticeHttpRequest> FJusticeHttpRequestPtr;


struct JusticeHttpResponse
{
	int32 Code;
	FString Content;
	FString AmazonTraceID;
	FString ErrorString;

	FHttpRequestPtr Request;
	FHttpResponsePtr Response;
	bool bSuccessful;

	FJusticeHttpRequestPtr JusticeRequest;
	int NextWait;

	void UpdateRequestForNextRetry()
	{
		this->JusticeRequest->RetryAttempt++;
		this->JusticeRequest->ElapsedTime += this->Request->GetElapsedTime();
		
		// exponential wait + jitter
		NextWait = 2 ^ this->JusticeRequest->RetryAttempt + FMath::RandRange(1, 60) ;
	}

	float GetTotalElapsed()
	{
		return JusticeRequest->ElapsedTime;
	}

	bool TooManyRetries()
	{
		return this->JusticeRequest->RetryAttempt >= 10;
	}

	bool TakesTooLong()
	{
		return this->JusticeRequest->ElapsedTime > 60;
	}


};


typedef TSharedPtr<JusticeHttpResponse, ESPMode::ThreadSafe> FJusticeHttpResponsePtr;

DECLARE_DELEGATE_OneParam(FWebRequestResponseDelegate, FJusticeHttpResponsePtr/* Response*/);

class JUSTICESDK_API FJusticeHTTP
{
public:	
	FJusticeHTTP()	{}
	~FJusticeHTTP()	{}
	static FString BasicAuth(const FString& Username = TEXT(""), const FString& Password = TEXT(""));
	static FString BearerAuth(const FString& Token);	
	static void CreateRequest(FString Authorization, FString URL, FString Verb, FString ContentType, FString Accept, FString Content, FWebRequestResponseDelegate OnResponse);
	static void CreateRequest(FJusticeHttpRequestPtr JusticeRequest, FWebRequestResponseDelegate OnResponse);	
};

#define TYPE_JSON TEXT("application/json")
#define TYPE_FORM TEXT("application/x-www-form-urlencoded; charset=utf-8")
#define TYPE_PLAIN TEXT("text/plain; charset=UTF-8");

#define POST TEXT("POST")
#define PUT TEXT("PUT")
#define GET TEXT("GET")