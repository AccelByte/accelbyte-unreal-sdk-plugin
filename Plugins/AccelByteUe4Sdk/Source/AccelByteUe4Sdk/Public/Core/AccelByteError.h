// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"

#include <unordered_map>

namespace AccelByte
{

DECLARE_DELEGATE_TwoParams(ErrorDelegate, int32, FString);

enum class ErrorCodes : int32
{
	DefaultError = 0,
	UnknownError = 1000,
	JsonDeserializationFailed = 1001,
	HttpOther = 8000,
	HttpBadRequest = 8400,
	HttpUnauthorized = 8401,
	HttpPaymentRequired = 8402,
	HttpForbidden = 8403,
	HttpNotFound = 8404,
	HttpMethodNotAllowed = 8405,
	HttpNotAcceptable = 8406,
	HttpProxyAuthenticationRequired = 8407,
	HttpRequestTimeout = 8408,
	HttpConflict = 8409,
	HttpGone = 8410,
};

class ErrorMessages
{
public:
	/**
	 * @brief Contains dictionary for default error messages (English).
	 */
	const static std::unordered_map<int32, FString> Default;
};

inline void HandleHttpError(FHttpRequestPtr Request, FHttpResponsePtr Response, int& OutCode, FString& OutMessage)
{
	int32 Code = OutCode + 8000;
	if (Response->GetResponseCode() < 1000)
	{
		Code = static_cast<int32>(ErrorCodes::HttpOther) + Response->GetResponseCode();
	}
	else
	{
		Code = static_cast<int32>(ErrorCodes::HttpOther);
	}
	OutCode = Code;
	// if the ErrorCode exist in the map, use that error string
	auto it = ErrorMessages::Default.find(Code);
	if (it != ErrorMessages::Default.cend())
	{
		OutMessage = it->second;
		return;
	}
	OutMessage = ErrorMessages::Default.at(static_cast<int32>(ErrorCodes::HttpOther));
}

} // Namespace AccelByte
