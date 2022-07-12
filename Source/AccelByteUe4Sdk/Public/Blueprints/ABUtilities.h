// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "JsonObjectWrapper.h"
#include "ABUtilities.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UABUtilities : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
		static FString DateTimeToString(const FDateTime& DateTime);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
	static bool GetJsonValueAsString(const FJsonObjectWrapper& ObjectWrapper, const FString& Key, FString& OutString);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
		static bool GetJsonValueAsNumber(const FJsonObjectWrapper& ObjectWrapper, const FString& Key, float& OutNumber);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
		static bool GetJsonValueAsBool(const FJsonObjectWrapper& ObjectWrapper, const FString& Key, bool& OutBool);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
		static bool GetJsonValueAsArray(const FJsonObjectWrapper& ObjectWrapper, const FString& Key, TArray<FString>& OutArray);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
		static void AddOrReplaceJsonStringValue(FJsonObjectWrapper& ObjectWrapper, const FString& Key, const FString& Value);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
		static void AddOrReplaceJsonNumberValue(FJsonObjectWrapper& ObjectWrapper, const FString& Key, float Value);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
		static void AddOrReplaceJsonBoolValue(FJsonObjectWrapper& ObjectWrapper, const FString& Key, bool Value);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Utilities")
		static void AddOrReplaceJsonArrayValue(FJsonObjectWrapper& ObjectWrapper, const FString& Key, const TArray<FString>& Value);
};
