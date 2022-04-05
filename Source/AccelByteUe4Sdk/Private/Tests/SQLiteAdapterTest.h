// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JsonUtilities.h"
#include "SQLiteAdapterTest.generated.h"

USTRUCT(BlueprintType)
struct FTestGrandChildModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		int I = -5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		int32 I32 = -2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		int64 I64 = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		uint8 UI8 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		bool b = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		float f = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FString S = TEXT("String");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FJsonObjectWrapper JO = FJsonObjectWrapper{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<int> AI = TArray<int>{ -5, -4 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<int32> AI32 = TArray<int32>{ -2, -1 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<int64> AI64 = TArray<int64>{ -1, 0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<uint8> AUI8 = TArray < uint8>{ 0, 1 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<bool> Ab = TArray<bool>{ true, false };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<float> Af = TArray<float>{ 0.5, 0.05 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<FString> AS = TArray<FString>{ TEXT("String"), TEXT("AString") };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<FJsonObjectWrapper> AJO = TArray<FJsonObjectWrapper>{};
};

USTRUCT(BlueprintType)
struct FTestChildModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		int I = -5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		int32 I32 = -2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		int64 I64 = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		uint8 UI8 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		bool b = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		float f = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FString S = TEXT("String");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FJsonObjectWrapper JO = FJsonObjectWrapper{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<int> AI = TArray<int>{ -5, -4 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<int32> AI32 = TArray<int32>{ -2, -1 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<int64> AI64 = TArray<int64>{ -1, 0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<uint8> AUI8 = TArray < uint8>{ 0, 1 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<bool> Ab = TArray<bool>{ true, false };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<float> Af = TArray<float>{ 0.5, 0.05 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<FString> AS = TArray<FString>{ TEXT("String"), TEXT("AString") };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<FJsonObjectWrapper> AJO = TArray<FJsonObjectWrapper>{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FTestGrandChildModel CM = FTestGrandChildModel{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<FTestGrandChildModel> CAM = TArray<FTestGrandChildModel>{};
};

USTRUCT(BlueprintType)
struct FTestModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		int I = -5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		int32 I32 = -2;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		int64 I64 = -1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		uint8 UI8 = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		bool b = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		float f = 0.5;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FString S = TEXT("String");
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FJsonObjectWrapper JO = FJsonObjectWrapper{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<int> AI = TArray<int>{ -5, -4 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<int32> AI32 = TArray<int32>{ -2, -1 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<int64> AI64 = TArray<int64>{ -1, 0 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<uint8> AUI8 = TArray < uint8>{ 0, 1 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<bool> Ab = TArray<bool>{ true, false };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<float> Af = TArray<float>{ 0.5, 0.05 };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<FString> AS = TArray<FString>{ TEXT("String"), TEXT("AString") };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<FJsonObjectWrapper> AJO = TArray<FJsonObjectWrapper>{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FTestGrandChildModel CM = FTestGrandChildModel{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<FTestGrandChildModel> CAM = TArray<FTestGrandChildModel>{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FTestChildModel M = FTestChildModel{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		TArray<FTestChildModel> AM = TArray<FTestChildModel>{};
};

USTRUCT(BlueprintType)
struct FWrongTestModel
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | TestModel")
		FString WrongField;
};