// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteCloudSaveCommonModels.h"
#include "AccelByteCloudSaveModels.generated.h"

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

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsListGameRecords
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | ListGameRecords")
	TArray<FAccelByteModelsGameRecord> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetPlayerRecordSizeRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | GetPlayerRecordSizeRequest")
	TArray<FString> Keys{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | GetPlayerRecordSizeRequest")
	FString User_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkGetPlayerRecordSizeRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | BulkGetPlayerRecordSizeRequest")
	TArray<FAccelByteModelsGetPlayerRecordSizeRequest> Data{};

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPlayerRecordSizeResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PlayerRecordSizeResponse")
	int32 CurrentSize{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PlayerRecordSizeResponse")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PlayerRecordSizeResponse")
	int32 RemainingSize{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PlayerRecordSizeResponse")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedBulkGetPlayerRecordSizeResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PaginatedBulkGetPlayerRecordSizeResponse")
	TArray<FAccelByteModelsPlayerRecordSizeResponse> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetPublicUserRecordKeys
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PublicUserRecordKeys")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PublicUserRecordKeys")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedBulkGetPublicUserRecordKeysResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PaginatedBulkGetPlayerRecordSizeResponse")
	TArray<FAccelByteModelsGetPublicUserRecordKeys> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PaginatedBulkGetPlayerRecordSizeResponse")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAdminGameRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminGameRecord")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminGameRecord")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminGameRecord")
	FDateTime Created_At{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminGameRecord")
	FDateTime Updated_At{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminGameRecord")
	FJsonObjectWrapper Value{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAdminUserRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminUserRecord")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminUserRecord")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminUserRecord")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminUserRecord")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminUserRecord")
	FDateTime UpdatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminUserRecord")
	FJsonObjectWrapper Value{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetAdminUserRecordKeys
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminUserRecordKeys")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | AdminUserRecordKeys")
	FString User_Id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedGetAdminUserRecordKeysResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PaginatedGetAdminUserRecordSizeResponse")
	TArray<FAccelByteModelsGetAdminUserRecordKeys> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | PaginatedGetAdminUserRecordSizeResponse")
	FAccelByteModelsPaging Paging {};
};