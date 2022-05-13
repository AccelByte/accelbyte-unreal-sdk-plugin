// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h" 
#include "JsonObjectWrapper.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | UserRecord")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | UserRecord")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | UserRecord")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | UserRecord")
	bool IsPublic{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | UserRecord")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | UserRecord")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | UserRecord")
	ESetByMetadataRecord SetBy{ESetByMetadataRecord::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | UserRecord")
	FJsonObjectWrapper Value{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | GameRecord")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | GameRecord")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | GameRecord")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | GameRecord")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | GameRecord")
	ESetByMetadataRecord SetBy{ESetByMetadataRecord::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | GameRecord")
	FJsonObjectWrapper Value{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsConcurrentReplaceRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | ConcurrentReplaceRequest")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | ConcurrentReplaceRequest")
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

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FListAccelByteModelsUserRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | UserRecordList")
	TArray<FAccelByteModelsUserRecord> Data{};
};