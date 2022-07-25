// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABUtilities.h"

FString UABUtilities::DateTimeToString(const FDateTime& DateTime)
{
	return DateTime.ToIso8601();
}

bool UABUtilities::GetJsonValueAsString(const FJsonObjectWrapper& ObjectWrapper, const FString& Key, FString& OutString)
{
	return ObjectWrapper.JsonObject->TryGetStringField(Key, OutString);
}

bool UABUtilities::GetJsonValueAsNumber(const FJsonObjectWrapper& ObjectWrapper, const FString& Key, float& OutNumber)
{
	double OutDouble;
	bool bIsSuccess = ObjectWrapper.JsonObject->TryGetNumberField(Key, OutDouble);
	if (bIsSuccess)
	{
		OutNumber = OutDouble;
	}
	return bIsSuccess;
}

bool UABUtilities::GetJsonValueAsBool(const FJsonObjectWrapper& ObjectWrapper, const FString& Key, bool& OutBool)
{
	return ObjectWrapper.JsonObject->TryGetBoolField(Key, OutBool);
}

bool UABUtilities::GetJsonValueAsArray(const FJsonObjectWrapper& ObjectWrapper, const FString& Key, TArray<FString>& OutArray)
{
	return ObjectWrapper.JsonObject->TryGetStringArrayField(Key, OutArray);
}

void UABUtilities::AddOrReplaceJsonStringValue(FJsonObjectWrapper& ObjectWrapper, const FString& Key, const FString& Value)
{
	if (!ObjectWrapper.JsonObject.IsValid())
	{
		ObjectWrapper.JsonObject = MakeShared<FJsonObject>();
	}
	ObjectWrapper.JsonObject->SetStringField(Key, Value);
}

void UABUtilities::AddOrReplaceJsonNumberValue(FJsonObjectWrapper& ObjectWrapper, const FString& Key, float Value)
{
	if (!ObjectWrapper.JsonObject.IsValid())
	{
		ObjectWrapper.JsonObject = MakeShared<FJsonObject>();
	}
	ObjectWrapper.JsonObject->SetNumberField(Key, Value);
}

void UABUtilities::AddOrReplaceJsonBoolValue(FJsonObjectWrapper& ObjectWrapper, const FString& Key, bool Value)
{
	if (!ObjectWrapper.JsonObject.IsValid())
	{
		ObjectWrapper.JsonObject = MakeShared<FJsonObject>();
	}
	ObjectWrapper.JsonObject->SetBoolField(Key, Value);
}

void UABUtilities::AddOrReplaceJsonArrayValue(FJsonObjectWrapper& ObjectWrapper, const FString& Key, const TArray<FString>& Value)
{
	
	if (!ObjectWrapper.JsonObject.IsValid())
	{
		ObjectWrapper.JsonObject = MakeShared<FJsonObject>();
	}

	TArray<TSharedPtr<FJsonValue>> Items;
	for (auto Item : Value)
	{
		Items.Add(MakeShareable(new FJsonValueString(Item)));
	}

	ObjectWrapper.JsonObject->SetArrayField(Key, Items);
}
