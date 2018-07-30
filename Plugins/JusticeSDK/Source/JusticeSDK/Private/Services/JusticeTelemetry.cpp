// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "JusticeTelemetry.h"
#include "Misc/ConfigCacheIni.h"
#include "JusticeLog.h"
#include "Async.h"
#include "AsyncTaskManagerJustice.h"

void JusticeTelemetry::SendTelemetryEvent(TelemetryEvent Telemetry, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization = TEXT("");
	FString URL = FString::Printf(TEXT("%s/telemetry/public/namespaces/%s/events/gameclient/%d/%d/%d/%d"), *FJusticeBaseURL, *FJusticeNamespace, Telemetry.AppID, Telemetry.EventType, Telemetry.EventLevel, Telemetry.EventID);
	FString Verb = POST;
	FString ContentType = TYPE_JSON;
	FString Accept = TEXT("*/*");
	FString Payload = Telemetry.ToJson();

	FJusticeHTTP::CreateRequest(
		Authorization,
		URL,
		Verb,
		ContentType,
		Accept,
		Payload,
		FWebRequestResponseDelegate::CreateStatic(JusticeTelemetry::OnSendTelemetryEventResponse, OnComplete));
}

void JusticeTelemetry::OnSendTelemetryEventResponse(FJusticeHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Send Telemetry Event Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}
	switch (Response->Code)
	{
	case EHttpResponseCodes::NoContent:
	{
		UE_LOG(LogJustice, VeryVerbose, TEXT("OnSendTelemetryEventComplete: Operation succeeded"));
		OnComplete.ExecuteIfBound(true, TEXT(""));
		break;
	}
	case EHttpResponseCodes::RequestTimeout:
		ErrorStr = TEXT("Request Timeout");
		break;
	case EHttpResponseCodes::Forbidden:
		ErrorStr = TEXT("Forbidden");
		break;
	case EHttpResponseCodes::ServerError:
	case EHttpResponseCodes::ServiceUnavail:
	case EHttpResponseCodes::GatewayTimeout:
	{
		if (Response->TooManyRetries() || Response->TakesTooLong())
		{
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->Code, *Response->Content);
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(JusticeTelemetry::OnSendTelemetryEventResponse, OnComplete));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected Response Code=%d"), Response->Code);
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Send Telemetry Event Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

