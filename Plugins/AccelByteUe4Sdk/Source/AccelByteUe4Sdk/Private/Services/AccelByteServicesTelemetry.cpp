// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteServicesTelemetry.h"
#if 0
namespace AccelByte
{
namespace Services
{

void Telemetry::SendTelemetryEvent(FAccelByteModelsTelemetryEvent Telemetry, FDefaultCompleteDelegate OnComplete)
{
	FString Authorization = TEXT("");
	FString Url = FString::Printf(TEXT("%s/telemetry/public/namespaces/%s/events/gameclient/%d/%d/%d/%d"), *ServerBaseUrl, *JusticeGameNamespace, Telemetry.AppId, Telemetry.EventType, Telemetry.EventLevel, Telemetry.EventId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("*/*");
	FString Content = Telemetry.ToJson();

	FAccelByteHttp::CreateRequest(
		Authorization,
		Url,
		Verb,
		ContentType,
		Accept,
		Content,
		FWebRequestResponseDelegate::CreateStatic(Telemetry::SendTelemetryEventResponse, OnComplete));
}

void Telemetry::SendTelemetryEventResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, FDefaultCompleteDelegate OnComplete)
{
	FString ErrorStr;
	if (!Response->ErrorString.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Send Telemetry Event Failed. Error Message: %s."), *Response->ErrorString);
		OnComplete.ExecuteIfBound(false, Response->ErrorString);
		return;
	}
	switch (Response->GetResponseCode())
	{
	case EHttpResponseCodes::Ok:
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
			ErrorStr = FString::Printf(TEXT("Retry Error, Response Code: %d, Content: %s"), Response->GetResponseCode(), *Response->GetContentAsString());
			OnComplete.ExecuteIfBound(false, ErrorStr);
			return;
		}
		Response->UpdateRequestForNextRetry();
		FJusticeRetryManager->AddQueue(Response->JusticeRequest,
			Response->NextWait,
			FWebRequestResponseDelegate::CreateStatic(Telemetry::SendTelemetryEventResponse, OnComplete));
		break;
	}
	default:
		ErrorStr = FString::Printf(TEXT("Unexpected Response Code=%d"), Response->GetResponseCode());
	}

	if (!ErrorStr.IsEmpty())
	{
		UE_LOG(LogJustice, Error, TEXT("Send Telemetry Event Error : %s"), *ErrorStr);
		OnComplete.ExecuteIfBound(false, ErrorStr);
		return;
	}
}

} // Namespace Services
} // Namespace AccelByte
#endif