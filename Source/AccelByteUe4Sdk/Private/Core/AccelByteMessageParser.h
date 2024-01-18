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

	/**
	 * @brief Process enveloped message and join the message if fragmented.
	 *
	 * @param InMessage enveloped message to process
	 * @param InEnvelopeStart string to use to mark start of envelope
	 * @param InEnvelopeEnd string to use to mark end of envelope
	 * @param InOutEnvelopeBuffer Temporary buffer variable for message on process.
	 * @param OutMessage full joined message without envelope, will out empty string if message not whole yet.
	 * @param OutIsMessageEnd true if reached end of message and message is whole.
	 */
	static void ProcessFragmentedMessage(const FString& InMessage
		, const FString& InEnvelopeStart
		, const FString& InEnvelopeEnd
		, FString& InOutEnvelopeBuffer
		, FString& OutMessage
		, bool& OutIsMessageEnd);
};
}