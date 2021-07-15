// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h" 
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteCloudSaveModels.generated.h"

struct ACCELBYTEUE4SDK_API FAccelByteModelsUserRecord
{
	FString Key;
	FString Namespace;
	FString UserId;
	bool IsPublic;
	FDateTime CreatedAt;
	FDateTime UpdatedAt;
	FJsonObject Value;
};

struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecord
{
	FString Key;
	FString Namespace;
	FDateTime CreatedAt;
	FDateTime UpdatedAt;
	FJsonObject Value;
};

struct ACCELBYTEUE4SDK_API FAccelByteModelsConcurrentReplaceRequest
{
	FDateTime UpdatedAt;
	FJsonObject Value;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedRecordsKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PaginatedRecordsKey")
	TArray<FString> Data;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PaginatedRecordsKey")
	FAccelByteModelsPaging Paging;
};