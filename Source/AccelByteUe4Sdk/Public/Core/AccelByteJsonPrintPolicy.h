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

	static void WriteFloat(FArchive* Stream, float Value)
	{
		WriteString(Stream, FString::Printf(TEXT("%g"), Value));
	}

	static void WriteDouble(FArchive* Stream, double Value)
	{
		WriteString(Stream, FString::Printf(TEXT("%.17g"), Value));
	}


#if ENGINE_MINOR_VERSION >= 2
	static void WriteString(FArchive* Stream, FStringView String)
	{
		TempString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(TempString, TEXT(","), TEXT("."));
		TempStringView = TempString;
		auto Conv = StringCast<CharType>(TempStringView.GetData(), TempStringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}

	static void WriteString(FArchive* Stream, FAnsiStringView String)
	{
		TempString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(TempString, TEXT(","), TEXT("."));
		TempAnsiStringView = StringCast<ANSICHAR>(*TempString);
		auto Conv = StringCast<CharType>(TempAnsiStringView.GetData(), TempAnsiStringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}

	static void WriteString(FArchive* Stream, FUtf8StringView String)
	{
		TempString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(TempString, TEXT(","), TEXT("."));
		TempUtf8StringView = StringCast<UTF8CHAR>(*TempString);
		auto Conv = StringCast<CharType>(TempUtf8StringView.GetData(), TempUtf8StringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}

private:
	inline static FString TempString{};
	inline static FStringView TempStringView{};
	inline static FAnsiStringView TempAnsiStringView{};
	inline static FUtf8StringView TempUtf8StringView{};

#endif
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

	static void WriteFloat(FArchive* Stream, float Value)
	{
		WriteString(Stream, FString::Printf(TEXT("%g"), Value));
	}

	static void WriteDouble(FArchive* Stream, double Value)
	{
		WriteString(Stream, FString::Printf(TEXT("%.17g"), Value));
	}


#if ENGINE_MINOR_VERSION >= 2
	static void WriteString(FArchive* Stream, FStringView String)
	{
		TempString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(TempString, TEXT(","), TEXT("."));
		TempStringView = TempString;
		auto Conv = StringCast<CharType>(TempStringView.GetData(), TempStringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}

	static void WriteString(FArchive* Stream, FAnsiStringView String)
	{
		TempString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(TempString, TEXT(","), TEXT("."));
		TempAnsiStringView = StringCast<ANSICHAR>(*TempString);
		auto Conv = StringCast<CharType>(TempAnsiStringView.GetData(), TempAnsiStringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}

	static void WriteString(FArchive* Stream, FUtf8StringView String)
	{
		TempString = String.GetData();
		FAccelByteUtilities::ReplaceDecimalSeparator(TempString, TEXT(","), TEXT("."));
		TempUtf8StringView = StringCast<UTF8CHAR>(*TempString);
		auto Conv = StringCast<CharType>(TempUtf8StringView.GetData(), TempUtf8StringView.Len());
		Stream->Serialize((void*)Conv.Get(), Conv.Length() * sizeof(CharType));
	}

private:
	inline static FString TempString{};
	inline static FStringView TempStringView{};
	inline static FAnsiStringView TempAnsiStringView{};
	inline static FUtf8StringView TempUtf8StringView{};

#endif
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
