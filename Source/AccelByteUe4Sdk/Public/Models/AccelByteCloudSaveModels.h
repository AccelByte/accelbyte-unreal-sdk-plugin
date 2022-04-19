// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h" 
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteCloudSaveModels.generated.h"

UENUM(BlueprintType)
enum class ESetByMetadataRecord : uint8
{
	NONE = 0,
	SERVER, 
	CLIENT,
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserRecord
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString Key{};
	
	UPROPERTY(BlueprintReadWrite)
	FString Namespace{};

	UPROPERTY(BlueprintReadWrite)
	FString UserId{};

	UPROPERTY(BlueprintReadWrite)
	bool IsPublic{};

	UPROPERTY(BlueprintReadWrite)
	FDateTime CreatedAt{};

	UPROPERTY(BlueprintReadWrite)
	FDateTime UpdatedAt{};

	UPROPERTY(BlueprintReadWrite)
	ESetByMetadataRecord SetBy{ESetByMetadataRecord::NONE};

	UPROPERTY(BlueprintReadWrite)
	FJsonObjectWrapper Value{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecord
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString Key{};

	UPROPERTY(BlueprintReadWrite)
	FString Namespace{};

	UPROPERTY(BlueprintReadWrite)
	FDateTime CreatedAt{};

	UPROPERTY(BlueprintReadWrite)
	FDateTime UpdatedAt{};

	UPROPERTY(BlueprintReadWrite)
	ESetByMetadataRecord SetBy{ESetByMetadataRecord::NONE};

	UPROPERTY(BlueprintReadWrite)
	FJsonObjectWrapper Value{};
};

struct ACCELBYTEUE4SDK_API FAccelByteModelsConcurrentReplaceRequest
{
	FDateTime UpdatedAt{};
	FJsonObjectWrapper Value{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedRecordsKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PaginatedRecordsKey")
	TArray<FString> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PaginatedRecordsKey")
	FAccelByteModelsPaging Paging{};
};

struct ACCELBYTEUE4SDK_API FListAccelByteModelsUserRecord
{
	TArray<FAccelByteModelsUserRecord> Data{};
};