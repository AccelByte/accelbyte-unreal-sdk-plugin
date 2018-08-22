// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IJusticeHttpRequest.h"
#include "Interfaces/IJusticeHttpResponse.h"

/**
 * Null (mock) implementation of an HTTP request
 */
class FNullHttpRequest : public IJusticeHttpRequest
{
public:

	// IJusticeHttpBase
	virtual FString GetURL() override;
	virtual FString GetURLParameter(const FString& ParameterName) override;
	virtual FString GetHeader(const FString& HeaderName) override;
	virtual TArray<FString> GetAllHeaders() override;	
	virtual FString GetContentType() override;
	virtual int32 GetContentLength() override;
	virtual const TArray<uint8>& GetContent() override;
	// IJusticeHttpRequest 
	virtual FString GetVerb() override;
	virtual void SetVerb(const FString& InVerb) override;
	virtual void SetURL(const FString& InURL) override;
	virtual void SetContent(const TArray<uint8>& ContentPayload) override;
	virtual void SetContentAsString(const FString& ContentString) override;
	virtual void SetHeader(const FString& HeaderName, const FString& HeaderValue) override;
	virtual void AppendToHeader(const FString& HeaderName, const FString& AdditionalHeaderValue) override;
	virtual bool ProcessRequest() override;
	virtual FJusticeHttpRequestCompleteDelegate& OnProcessRequestComplete() override;
	virtual FJusticeHttpRequestProgressDelegate& OnRequestProgress() override;
	virtual void CancelRequest() override;
	virtual EJusticeHttpRequestStatus::Type GetStatus() override;
	virtual const FJusticeHttpResponsePtr GetResponse() const override;
	virtual void Tick(float DeltaSeconds) override;
	virtual float GetElapsedTime() override;

	FNullHttpRequest()
		: CompletionStatus(EJusticeHttpRequestStatus::NotStarted)
		, ElapsedTime(0)
	{}
	virtual ~FNullHttpRequest() {}

private:
	void FinishedRequest();

	FString Url;
	FString Verb;
	TArray<uint8> Payload;
	FJusticeHttpRequestCompleteDelegate RequestCompleteDelegate;
	FJusticeHttpRequestProgressDelegate ReuestProgressDelegate;
	EJusticeHttpRequestStatus::Type CompletionStatus;
	TMap<FString, FString> Headers;
	float ElapsedTime;
};

/**
 * Null (mock) implementation of an HTTP request
 */
class FNullHttpResponse : public IJusitceHttpResponse
{
	// IJusticeHttpBase 
	virtual FString GetURL() override;
	virtual FString GetURLParameter(const FString& ParameterName) override;
	virtual FString GetHeader(const FString& HeaderName) override;
	virtual TArray<FString> GetAllHeaders() override;	
	virtual FString GetContentType() override;
	virtual int32 GetContentLength() override;
	virtual const TArray<uint8>& GetContent() override;
	//~ Begin IJusitceHttpResponse Interface
	virtual int32 GetResponseCode() override;
	virtual FString GetContentAsString() override;

	FNullHttpResponse() {}
	virtual ~FNullHttpResponse() {}

private:
	TArray<uint8> Payload;
};
