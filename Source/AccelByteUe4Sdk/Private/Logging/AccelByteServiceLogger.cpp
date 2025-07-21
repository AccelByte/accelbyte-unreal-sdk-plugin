// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Logging/AccelByteServiceLogger.h"
#include "Api/AccelByteLobbyApi.h"

#if !UE_BUILD_SHIPPING && ACCELBYTE_SERVICE_LOGGING_ENABLED

static TAutoConsoleVariable<bool> CVarAccelbyteEnableServiceLogging(
	TEXT("accelbyte.log.EnableServiceLogging"),
	false,
	TEXT("Enable Accelbyte Service Logging, read by the Visual Debugger"),
	ECVF_Default
);

DEFINE_LOG_CATEGORY(LogAccelByteServiceLoggingEvent);

namespace AccelByte
{
namespace Logging
{

FString GenerateLoggingEventString(const FString& message_id, int type, int direction, const FString& dataStr)
{
	return FString::Printf(TEXT(R"LOG(<--{"message_id":"%s","timestamp":"%s","type":%i,"direction":%i,"data":%s}-->)LOG"), 
		*message_id, 
		*FDateTime::Now().ToIso8601(), 
		type, 
		direction, 
		*dataStr);
}

FString GenerateHttpHeaderString(const FHttpRequestPtr& Request)
{
	auto http_headers = Request->GetAllHeaders(); // return sting in Key: Value format	

	FString headerJsonStr;

	for (int i = 0; i < http_headers.Num(); i++)
	{
		FString header = http_headers[i];
		FString header_key = TEXT("");
		FString header_value = TEXT("");
		header.Split(": ", &header_key, &header_value);

		if (header_key == "Authorization")
		{
			// truncate the Authorization header
			header_key = header_key.Left(16);
		}

		headerJsonStr.Append(FString::Printf(TEXT(R"LOG(%s"%s":"%s")LOG"), (i > 0 ? TEXT(",") : TEXT("")), *header_key, *header_value));
	}

	return headerJsonStr;
}

FString GenerateHttpRequestString(FHttpRequestPtr Request)
{
	if (!Request.IsValid())
	{
		return "";
	}

	// build message for http response
	FString headerJsonStr = GenerateHttpHeaderString(Request);
	FString content_type = Request->GetContentType();
	const TArray<uint8> Payload = Request->GetContent();

	FString payloadJsonStr;

	if (Payload.Num() > 0) {
		payloadJsonStr = FAccelByteArrayByteFStringConverter::BytesToFString(Payload, true);
	}

	FString dataStr = FString::Printf(TEXT(R"LOG("{"verb":"%s","url":"%s","header":{%s},"payload":"%s"})LOG"), 
		*Request->GetVerb(), 
		*Request->GetURL(), 
		*headerJsonStr, 
		*payloadJsonStr.ReplaceQuotesWithEscapedQuotes()
	);

	FString message_id = FString::Printf(TEXT("%p"), Request.Get());

	return GenerateLoggingEventString(message_id, 1, 1, dataStr);
}

FString GenerateHttpResponseString(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bFinished)
{
	if (!Request.IsValid() || !Response.IsValid())
	{
		return "";
	}

	// build message for http response
	FString headerJsonStr = GenerateHttpHeaderString(Request);
	const TArray<uint8> Payload = Response->GetContent();
	FString payloadJsonStr = (Response == nullptr ? FAccelByteArrayByteFStringConverter::BytesToFString(Payload, true) : Response->GetContentAsString());

	payloadJsonStr = payloadJsonStr.Replace(TEXT("\r"), TEXT(""));
	payloadJsonStr = payloadJsonStr.Replace(TEXT("\n"), TEXT(""));

	FString content_type = Response->GetContentType();

	if (content_type.Contains(TEXT("application/json")) && !payloadJsonStr.IsEmpty())
	{
		if (payloadJsonStr[0] == '[') {
			payloadJsonStr = FString("{\"root\":") + payloadJsonStr + '}'; // UNREAL ENGINE JSON Deserialize Bug
		}
	}

	FString dataStr = FString::Printf(TEXT(R"LOG({"verb":"%s","url":"%s","status":%i,"content_type":"%s","header":{%s},"payload":"%s"})LOG"), 
		*Request->GetVerb(), 
		*Request->GetURL(), 
		Response->GetResponseCode(), 
		*content_type, 
		*headerJsonStr, 
		*payloadJsonStr.ReplaceQuotesWithEscapedQuotes());

	FString message_id = FString::Printf(TEXT("%p"), Request.Get());

	return GenerateLoggingEventString(message_id, 1, 2, dataStr);
}

FString GenerateWebsocketRequestString(FString payloadStr)
{
	const FString WsEnvelopeStart{ "CaSr" };
	const FString WsEnvelopeEnd{ "CaEd" };
	const FString WSEnvelopeYAML{ "type:" };

	if (payloadStr.StartsWith(WsEnvelopeStart) && payloadStr.EndsWith(WsEnvelopeEnd))
	{
		payloadStr.RemoveFromStart(WsEnvelopeStart);
		payloadStr.RemoveFromEnd(WsEnvelopeEnd);
	}
	else if (payloadStr.StartsWith(WSEnvelopeYAML))
	{
		payloadStr = AccelByte::Api::Lobby::LobbyMessageToJson(payloadStr);
	}

	static uint64 message_id_request = 1;
	FString message_id = FString::Printf(TEXT("%d"), message_id_request);
	FString dataStr = FString::Printf(TEXT(R"LOG({"payload":"%s"})LOG"), *payloadStr.ReplaceQuotesWithEscapedQuotes());

	message_id_request += 1;
	return GenerateLoggingEventString(message_id, 2, 1, dataStr);
}

FString GenerateWebsocketResponseString(FString payloadStr)
{
	const FString WsEnvelopeStart{ "CaSr" };
	const FString WsEnvelopeEnd{ "CaEd" };
	const FString WSEnvelopeYAML{ "type:" };

	if (payloadStr.StartsWith(WsEnvelopeStart) && payloadStr.EndsWith(WsEnvelopeEnd))
	{
		payloadStr.RemoveFromStart(WsEnvelopeStart);
		payloadStr.RemoveFromEnd(WsEnvelopeEnd);
	}
	else if (payloadStr.StartsWith(WSEnvelopeYAML))
	{
		payloadStr = AccelByte::Api::Lobby::LobbyMessageToJson(payloadStr);
	}

	static uint64 message_id_response = 1;
	FString message_id = FString::Printf(TEXT("%d"), message_id_response);
	FString dataStr = FString::Printf(TEXT(R"LOG({"payload":"%s"})LOG"), *payloadStr.ReplaceQuotesWithEscapedQuotes());

	message_id_response += 1;

	return GenerateLoggingEventString(message_id, 2, 2, dataStr);
}

void ServiceLogger::LogHttpRequest(const FHttpRequestPtr& Request)
{
	if (CVarAccelbyteEnableServiceLogging.GetValueOnAnyThread())
	{
		UE_LOG(LogAccelByteServiceLoggingEvent, VeryVerbose, TEXT("%s"), *GenerateHttpRequestString(Request));
	}
}

void ServiceLogger::LogHttpResponse(const FHttpRequestPtr& Request, const FHttpResponsePtr& Response, bool bFinished)
{
	if (CVarAccelbyteEnableServiceLogging.GetValueOnAnyThread())
	{
		UE_LOG(LogAccelByteServiceLoggingEvent, VeryVerbose, TEXT("%s"), *GenerateHttpResponseString(Request, Response, bFinished));
	}
}

void ServiceLogger::LogWebsocketRequest(const FString& message)
{
	if (CVarAccelbyteEnableServiceLogging.GetValueOnAnyThread())
	{
		UE_LOG(LogAccelByteServiceLoggingEvent, VeryVerbose, TEXT("%s"), *GenerateWebsocketRequestString(message));
	}
}

void ServiceLogger::LogWebsocketResponse(const FString& message)
{
	if (CVarAccelbyteEnableServiceLogging.GetValueOnAnyThread())
	{
		UE_LOG(LogAccelByteServiceLoggingEvent, VeryVerbose, TEXT("%s"), *GenerateWebsocketResponseString(message));
	}
}

} // Namespace Logging
} // Namespace AccelByte

#endif // !UE_BUILD_SHIPPING && ACCELBYTE_SERVICE_LOGGING_ENABLED
