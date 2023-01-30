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

class ACCELBYTEUE4SDK_API FAccelByteJsonConverter
{
public:
	template<typename OutStructType>
	static bool JsonObjectStringToUStruct(const FString& JsonString, OutStructType* OutStruct)
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(JsonString);
		if (!FJsonSerializer::Deserialize(JsonReader, JsonObject) || !JsonObject.IsValid())
		{
			UE_LOG(LogJson, Warning, TEXT("JsonObjectStringToUStruct - Unable to parse json=[%s]"), *JsonString);
			return false;
		}
		HandleUnidentifiedEnum(JsonObject, OutStructType::StaticStruct());
		bool bSuccess = FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), OutStruct, 0, 0);
		if (!bSuccess)
		{
			UE_LOG(LogJson, Warning, TEXT("JsonObjectStringToUStruct - Unable to deserialize. json=[%s]"), *JsonString);
		}
		return bSuccess;
	}

	template<typename OutStructType>
	static bool JsonArrayStringToUStruct(const FString& JsonString, TArray<OutStructType>* OutStructArray)
	{
		TArray<TSharedPtr<FJsonValue> > JsonArray;
		TSharedRef<TJsonReader<> > JsonReader = TJsonReaderFactory<>::Create(JsonString);
		if (!FJsonSerializer::Deserialize(JsonReader, JsonArray))
		{
			UE_LOG(LogJson, Warning, TEXT("JsonArrayStringToUStruct - Unable to parse. json=[%s]"), *JsonString);
			return false;
		}
		for (const auto& Value : JsonArray)
		{
			HandleUnidentifiedEnum(Value->AsObject(), OutStructType::StaticStruct());
		}
		if (!FJsonObjectConverter::JsonArrayToUStruct(JsonArray, OutStructArray, 0, 0))
		{
			UE_LOG(LogJson, Warning, TEXT("JsonArrayStringToUStruct - Error parsing one of the elements. json=[%s]"), *JsonString);
			return false;
		}
		return true;
	}

	static void HandleUnidentifiedEnum(const TSharedPtr<FJsonObject>& JsonObject, const UStruct* Definition)
	{
		const TMap< FString, TSharedPtr<FJsonValue> > JsonAttributes = JsonObject->Values;
		for (TFieldIterator<FProperty> PropertyIt(Definition); PropertyIt; ++PropertyIt)
		{
			FProperty* Property = *PropertyIt;
			const TSharedPtr<FJsonValue>* JsonValue = JsonAttributes.Find(Property->GetName());
			if (!JsonValue)
			{
				continue;
			}

			if (FEnumProperty* EnumProperty = CastField<FEnumProperty>(Property))
			{
				int64 IntValue = 0;
				const UEnum* Enum = EnumProperty->GetEnum();
				if (Enum == nullptr)
				{
					continue;
				}
				if (JsonValue->IsValid() && !(*JsonValue)->IsNull())
				{
					IntValue = EnumJsonValueToInt64(Enum, *JsonValue);
				}
				FString EnumPropertyName = FJsonObjectConverter::StandardizeCase(Property->GetName());
				FString ValueAsString = Enum->GetNameByValue(IntValue).ToString();
				JsonObject->SetStringField(EnumPropertyName, ValueAsString);
			}
			else if (FArrayProperty* ArrayProperty = CastField<FArrayProperty>(Property))
			{
				if ((*JsonValue)->Type != EJson::Array)
				{
					continue;
				}
				if (FEnumProperty* ArrayEnumProperty = CastField<FEnumProperty>(ArrayProperty->Inner))
				{
					const TArray<TSharedPtr<FJsonValue>> ArrayValues = (*JsonValue)->AsArray();
					int ArrayItem = ArrayValues.Num();
					TArray<TSharedPtr<FJsonValue>> NewArrayValues;
					for (int Index = 0; Index != ArrayItem; ++Index)
					{
						const TSharedPtr<FJsonValue> Value = ArrayValues[Index];
						int64 IntValue = 0;
						const UEnum* Enum = ArrayEnumProperty->GetEnum();
						if (Enum == nullptr)
						{
							continue;
						}
						if (!Value->IsNull())
						{
							IntValue = EnumJsonValueToInt64(Enum, Value);
						}
						FString ValueAsString = Enum->GetNameByValue(IntValue).ToString();
						NewArrayValues.Add(MakeShareable(new FJsonValueString(ValueAsString)));

					}
					JsonObject->SetArrayField(Property->GetName(), NewArrayValues);
				}

			}
			else if (JsonValue->IsValid() && !(*JsonValue)->IsNull() && (*JsonValue)->Type == EJson::Object)
			{
				const TSharedPtr<FJsonObject> Object = (*JsonValue)->AsObject();
				HandleUnidentifiedEnum(Object, Property->GetOwnerStruct());
			}
		}
	}

	static int64 EnumJsonValueToInt64(const UEnum* Enum, const TSharedPtr<FJsonValue>& Value)
	{
		if (Enum == nullptr)
		{
			return 0;
		}

		int64 ValueInt = 0;
		if (Value->Type == EJson::String)
		{
			FString ValueAsString = Value->AsString();
			ValueInt = Enum->GetValueByName(FName(*ValueAsString));
			if (ValueInt == INDEX_NONE)
			{
				UE_LOG(LogJson, Warning, TEXT("EnumJsonValueToInt64 - Unknown Enum value %s"), *ValueAsString);
				ValueInt = 0;
			}
		}
		else if (Value->Type == EJson::Number)
		{
			ValueInt = (int64)Value->AsNumber();
			if (Enum->GetNameByValue(ValueInt) == NAME_None)
			{
				UE_LOG(LogJson, Warning, TEXT("EnumJsonValueToInt64 - Unknown Enum value %d"), ValueInt);
				ValueInt = 0;
			}
		}
		return ValueInt;
	}
};