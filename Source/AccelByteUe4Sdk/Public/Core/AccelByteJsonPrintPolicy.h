// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "CoreMinimal.h"
#include "AccelByteUtilities.h"

template <class CharType>
struct TAccelBytePrettyJsonPrintPolicy
	: public TPrettyJsonPrintPolicy<CharType>
{
	static void WriteString(FArchive* Stream, const FString& String)
	{
		FString NewString = String;
		if (FAccelByteUtilities::IsNumericString(NewString))
		{
			NewString = NewString.Replace(TEXT(","), TEXT("."));
		}

		const TCHAR* CharPtr = *NewString;

		for (int32 CharIndex = 0; CharIndex < NewString.Len(); ++CharIndex, ++CharPtr)
		{
			TJsonPrintPolicy<CharType>::WriteChar(Stream, *CharPtr);
		}
	}
};

template <class CharType>
struct TAccelByteCondensedJsonPrintPolicy
	: public TCondensedJsonPrintPolicy<CharType>
{
	static void WriteString(FArchive* Stream, const FString& String)
	{
		FString NewString = String;
		if (FAccelByteUtilities::IsNumericString(NewString))
		{
			NewString = NewString.Replace(TEXT(","), TEXT("."));
		}

		const TCHAR* CharPtr = *NewString;

		for (int32 CharIndex = 0; CharIndex < NewString.Len(); ++CharIndex, ++CharPtr)
		{
			TJsonPrintPolicy<CharType>::WriteChar(Stream, *CharPtr);
		}
	}
};