// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

class ACCELBYTEUE4SDK_API FAccelByteArrayByteFStringConverter
{
public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="Bytes"></param>
	/// <param name="bIsFromString"> Is the byte represent string? else raw byte</param>
	/// <returns></returns>
	static FString BytesToFString(const TArray<uint8>& Bytes, bool bIsFromString)
	{
		int32 Count = Bytes.Num();
		const uint8* In = &Bytes[0];

		//Taken from UE source code
		FString Result;
		Result.Empty(Count);
		while (Count)
		{
			// Put the byte into an int16 and add 1 to it, this keeps anything from being put into the string as a null terminator
			int16 Value = *In;
			Value += bIsFromString ? 0 : 1;
			Result += TCHAR(Value);
			++In;
			Count--;
		}

		return Result;
	}

	static TArray<uint8> FStringToBytes(const FString& String)
	{
		TArray<uint8> Output;
		Output.AddUninitialized(String.Len());
		StringToBytes(String, Output.GetData(), String.Len());
		return Output;
	}
};