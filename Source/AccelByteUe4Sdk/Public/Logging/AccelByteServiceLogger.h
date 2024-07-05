#pragma once

#include "CoreMinimal.h"
#include "EngineMinimal.h"
#include "Http.h"

#define ACCELBYTE_SERVICE_LOGGING_ENABLED 0

#if !UE_BUILD_SHIPPING && ACCELBYTE_SERVICE_LOGGING_ENABLED

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServiceLoggingEvent, Log, All);

namespace AccelByte
{
namespace Logging
{
		
class ACCELBYTEUE4SDK_API ServiceLogger
{
public:
	static void LogHttpRequest(const FHttpRequestPtr& request);
	static void LogHttpResponse(const FHttpRequestPtr& request, const FHttpResponsePtr& response, bool bFinished);
	static void LogWebsocketRequest(const FString& message);
	static void LogWebsocketResponse(const FString& message);
};

#define ACCELBYTE_SERVICE_LOGGING_HTTP_REQUEST(request) AccelByte::Logging::ServiceLogger::LogHttpRequest(request);
#define ACCELBYTE_SERVICE_LOGGING_HTTP_RESPONSE(request,response,bFinished) AccelByte::Logging::ServiceLogger::LogHttpResponse(request,response,bFinished);
#define ACCELBYTE_SERVICE_LOGGING_WEBSOCKET_REQUEST(message) AccelByte::Logging::ServiceLogger::LogWebsocketRequest(message);
#define ACCELBYTE_SERVICE_LOGGING_WEBSOCKET_RESPONSE(message) AccelByte::Logging::ServiceLogger::LogWebsocketRequest(message);


} // namespace Logging
} // namespace AccelByte

#else

#define ACCELBYTE_SERVICE_LOGGING_HTTP_REQUEST(request) 
#define ACCELBYTE_SERVICE_LOGGING_HTTP_RESPONSE(request,response,bFinished) 
#define ACCELBYTE_SERVICE_LOGGING_WEBSOCKET_REQUEST(message) 
#define ACCELBYTE_SERVICE_LOGGING_WEBSOCKET_RESPONSE(message)

#endif // !UE_BUILD_SHIPPING