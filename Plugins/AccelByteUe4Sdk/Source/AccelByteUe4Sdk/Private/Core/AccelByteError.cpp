// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteError.h"

namespace AccelByte
{

const std::unordered_map<int32, FString> ErrorMessages::Default
{
	{ static_cast<int32>(ErrorCodes::ErrorCodeNotSet), TEXT("Error code is not set (0).") },
	{ static_cast<int32>(ErrorCodes::UnknownError), TEXT("Unknown error.") },
	{ static_cast<int32>(ErrorCodes::JsonDeserializationFailed), TEXT("JSON deserialization failed. ") },
	{ static_cast<int32>(ErrorCodes::HttpOther), TEXT("Other HTTP error.") },
	{ static_cast<int32>(ErrorCodes::HttpBadRequest), TEXT("HTTP 400 Bad Request.") },
	{ static_cast<int32>(ErrorCodes::HttpUnauthorized), TEXT("HTTP 401 Unauthorized.") },
	{ static_cast<int32>(ErrorCodes::HttpPaymentRequired), TEXT("HTTP 402 Payment Required.") },
	{ static_cast<int32>(ErrorCodes::HttpForbidden), TEXT("HTTP 403 Forbidden.") },
	{ static_cast<int32>(ErrorCodes::HttpNotFound), TEXT("HTTP 404 Not Found.") },
	{ static_cast<int32>(ErrorCodes::HttpMethodNotAllowed), TEXT("HTTP 405 Method Not Allowed.") },
	{ static_cast<int32>(ErrorCodes::HttpNotAcceptable), TEXT("HTTP 406 Not Acceptable.") },
	{ static_cast<int32>(ErrorCodes::HttpProxyAuthenticationRequired), TEXT("HTTP 407 Proxy Authentication Required.") },
	{ static_cast<int32>(ErrorCodes::HttpRequestTimeout), TEXT("HTTP 408 Request Timeout.") },
	{ static_cast<int32>(ErrorCodes::HttpConflict), TEXT("HTTP 409 Conflict.") },
	{ static_cast<int32>(ErrorCodes::HttpGone), TEXT("HTTP 410 Gone.") },

};

} // Namespace AccelByte

