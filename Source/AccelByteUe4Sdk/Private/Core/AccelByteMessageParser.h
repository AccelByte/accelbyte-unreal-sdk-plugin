// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

namespace AccelByte
{
class ACCELBYTEUE4SDK_API MessageParser
{
public:
	static bool MoveToEndQuote(const TCHAR*& Cursor);
	static bool MoveToEndObject(const TCHAR*& Cursor);
	// escape quote on string
	static FString EscapeString(const FString& InString, bool bIsEnclosedWithQuote = false);
	// parse and append string into OutJsonString, unescaped quote will be escaped
	static bool ParseString(const TCHAR*& Cursor, FString& OutJsonString);
	// parse and append object into OutJsonString
	static bool ParseObject(const TCHAR*& Cursor, FString& OutJsonString);
	// parse and append array into OutJsonString
	static bool ParseArrayOfObject(const TCHAR*& Cursor, FString& OutJsonString);
	// parse and append array into OutJsonString
	static bool ParseArrayOfString(const TCHAR*& Cursor, FString& OutJsonString);
};
}