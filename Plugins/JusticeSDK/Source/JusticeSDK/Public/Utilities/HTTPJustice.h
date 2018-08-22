// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "HAL/ThreadSafeCounter.h"
#include "UObject/CoreOnline.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "JusticeHttp.h"

struct JusticeHttpRequest
{
	FString Authorization;
	FString URL;
	FString Verb;
	FString ContentType;
	FString Accept;
	FString Content;
	int32 RetryAttempt;
	float ElapsedTime;
};
typedef TSharedPtr<JusticeHttpRequest> FJusticeRequestPtr;


struct JusticeHttpResponse
{
	int32 Code;
	FString Content;
	FString AmazonTraceID;
	FString ErrorString;

    FJusticeHttpRequestPtr Request;
    FJusticeHttpResponsePtr Response;
	bool bSuccessful;

    TSharedPtr<JusticeHttpRequest> JusticeRequest;
	int32 NextWait;

	void UpdateRequestForNextRetry()
	{
		this->JusticeRequest->RetryAttempt++;
		this->JusticeRequest->ElapsedTime += this->Request->GetElapsedTime();		
		// exponential wait + jitter
		UE_LOG(LogTemp, Log, TEXT("Next Retry Attempt: %d"), this->JusticeRequest->RetryAttempt);
		NextWait = 2 ^ this->JusticeRequest->RetryAttempt + FMath::RandRange(1, 7) ;
		UE_LOG(LogTemp, Log, TEXT("Next Retry in: %d second"), NextWait);
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


typedef TSharedPtr<JusticeHttpResponse, ESPMode::ThreadSafe> FJusticeResponsePtr;

DECLARE_DELEGATE_OneParam(FWebRequestResponseDelegate, FJusticeResponsePtr/* Response*/);

class JUSTICESDK_API FJusticeHTTP
{
public:	
	FJusticeHTTP()	{}
	~FJusticeHTTP()	{}
	static FString BasicAuth(const FString& Username = TEXT(""), const FString& Password = TEXT(""));
	static FString BearerAuth(const FString& Token);	
	static void CreateRequest(FString Authorization, FString URL, FString Verb, FString ContentType, FString Accept, FString Content, FWebRequestResponseDelegate OnResponse);
	static void CreateRequest(TSharedPtr<JusticeHttpRequest> JusticeRequest, FWebRequestResponseDelegate OnResponse);
};

#define TYPE_JSON TEXT("application/json")
#define TYPE_FORM TEXT("application/x-www-form-urlencoded; charset=utf-8")
#define TYPE_PLAIN TEXT("text/plain; charset=UTF-8");

#define POST TEXT("POST")
#define PUT TEXT("PUT")
#define GET TEXT("GET")