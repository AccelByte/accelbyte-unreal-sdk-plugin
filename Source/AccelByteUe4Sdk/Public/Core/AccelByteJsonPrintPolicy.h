// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "CoreMinimal.h"
#include "AccelByteUtilities.h"
#include "Runtime/Launch/Resources/Version.h"

template <class CharType>
struct TAccelBytePrettyJsonPrintPolicy
	: public TPrettyJsonPrintPolicy<CharType>
{
#if ENGINE_MAJOR_VERSION >= 5
	static void WriteString(FArchive* Stream, const FString& String)
	{
		FString NewString = String;
		FAccelByteUtilities::ReplaceDecimalSeparator(NewString, TEXT(","), TEXT("."));
		auto Conv = StringCast<CharType>(*NewString, NewString.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}
#if ENGINE_MINOR_VERSION >= 2
	static void WriteString(FArchive* Stream, FStringView String)
	{
		FString NewString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(NewString, TEXT(","), TEXT("."));
		FStringView NewStringView(NewString);
		auto Conv = StringCast<CharType>(NewStringView.GetData(), NewStringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}

	static void WriteString(FArchive* Stream, FAnsiStringView String)
	{
		FString NewString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(NewString, TEXT(","), TEXT("."));
		FAnsiStringView NewAnsiStringView(TCHAR_TO_ANSI(*NewString));
		auto Conv = StringCast<CharType>(NewAnsiStringView.GetData(), NewAnsiStringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}
#endif

	static void WriteFloat(FArchive* Stream, float Value)
	{
		WriteString(Stream, FString::Printf(TEXT("%g"), Value));
	}

	static void WriteDouble(FArchive* Stream, double Value)
	{
		WriteString(Stream, FString::Printf(TEXT("%.17g"), Value));
	}

#else
	static void WriteString(FArchive* Stream, const FString& String)
	{
		FString NewString = String;
		FAccelByteUtilities::ReplaceDecimalSeparator(NewString, TEXT(","), TEXT("."));

		const TCHAR* CharPtr = *NewString;

		for (int32 CharIndex = 0; CharIndex < NewString.Len(); ++CharIndex, ++CharPtr)
		{
			TJsonPrintPolicy<CharType>::WriteChar(Stream, *CharPtr);
		}
	}
#endif
};

template <class CharType>
struct TAccelByteCondensedJsonPrintPolicy
	: public TCondensedJsonPrintPolicy<CharType>
{
#if ENGINE_MAJOR_VERSION >= 5
	static void WriteString(FArchive* Stream, const FString& String)
	{
		FString NewString = String;
		FAccelByteUtilities::ReplaceDecimalSeparator(NewString, TEXT(","), TEXT("."));
		auto Conv = StringCast<CharType>(*NewString, NewString.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}

#if ENGINE_MINOR_VERSION >=2
	static void WriteString(FArchive* Stream, FStringView String)
	{
		FString NewString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(NewString, TEXT(","), TEXT("."));
		FStringView NewStringView(NewString);
		auto Conv = StringCast<CharType>(NewStringView.GetData(), NewStringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}

	static void WriteString(FArchive* Stream, FAnsiStringView String)
	{
		FString NewString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(NewString, TEXT(","), TEXT("."));
		FAnsiStringView NewAnsiStringView(TCHAR_TO_ANSI(*NewString));
		auto Conv = StringCast<CharType>(NewAnsiStringView.GetData(), NewAnsiStringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}
#endif

	static void WriteFloat(FArchive* Stream, float Value)
	{
		WriteString(Stream, FString::Printf(TEXT("%g"), Value));
	}

	static void WriteDouble(FArchive* Stream, double Value)
	{
		WriteString(Stream, FString::Printf(TEXT("%.17g"), Value));
	}

#else
	static void WriteString(FArchive* Stream, const FString& String)
	{
		FString NewString = String;
		FAccelByteUtilities::ReplaceDecimalSeparator(NewString, TEXT(","), TEXT("."));

		const TCHAR* CharPtr = *NewString;

		for (int32 CharIndex = 0; CharIndex < NewString.Len(); ++CharIndex, ++CharPtr)
		{
			TJsonPrintPolicy<CharType>::WriteChar(Stream, *CharPtr);
		}
	}
#endif
};