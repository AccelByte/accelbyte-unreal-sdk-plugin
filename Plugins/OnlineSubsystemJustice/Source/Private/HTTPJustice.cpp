// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#include "HTTPJustice.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "OnlineSubsystemJusticeModule.h"

FString FHTTPJustice::BasicAuth(const FString& Username, const FString& Password)
{
	return TEXT("Basic " + FBase64::Encode(Username + ":" + Password));
}

TSharedRef<IHttpRequest> FHTTPJustice::CreateRequest(const FOpenTracingJustice& Parent=FOpenTracingJustice())
{
	FOpenTracingJustice Trace(Parent);

	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetHeader(TEXT("X-B3-TraceId"), Trace.TraceIdToString());
	Request->SetHeader(TEXT("X-B3-SpanId"), Trace.SpanIdToString());
	if (Trace.HasParent())
	{
		Request->SetHeader(TEXT("X-B3-ParentSpanId"),  Trace.ParentIdToString());
	}

	UE_LOG_ONLINE(Log, TEXT("CreateRequest(): %s"), *Trace.ToString());

	return Request;
}

