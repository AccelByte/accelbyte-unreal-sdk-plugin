// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteCloudSaveCommonModels.h"
#include "AccelByteBinaryCloudSaveModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBinaryInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudSave | Binary | Models | BinaryInfo")
	FString Content_Type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudSave | Binary | Models | BinaryInfo")
	FString File_Location{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudSave | Binary | Models | BinaryInfo")
	FString Url{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudSave | Binary | Models | BinaryInfo")
	int32 Version{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBinaryInfoWithTime : public FAccelByteModelsBinaryInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | BinaryInfo")
	FDateTime Created_At{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | BinaryInfo")
	FDateTime Updated_At{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserBinaryRecord
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | UserBinaryRecord")
	FAccelByteModelsBinaryInfo Binary_Info{};
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | UserBinaryRecord")
	FDateTime Created_At{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | UserBinaryRecord")
	bool Is_Public{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | UserBinaryRecord")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | UserBinaryRecord")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | UserBinaryRecord")
	ESetByMetadataRecord Set_By{ESetByMetadataRecord::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | UserBinaryRecord")
	FDateTime Updated_At{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | UserBinaryRecord")
	FString User_Id{};

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameBinaryRecord
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | GameBinaryRecord")
	FAccelByteModelsBinaryInfoWithTime Binary_Info{};
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | GameBinaryRecord")
	FDateTime Created_At{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | GameBinaryRecord")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | GameBinaryRecord")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | GameBinaryRecord")
	ESetByMetadataRecord Set_By{ESetByMetadataRecord::NONE};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | GameBinaryRecord")
	FDateTime Updated_At{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | GameBinaryRecord")
	FTTLConfig Ttl_Config{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | GameBinaryRecord")
	TArray<FString> Tags{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsListUserBinaryRecords
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | ListUserBinaryRecords")
	TArray<FAccelByteModelsUserBinaryRecord> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsListGameBinaryRecords
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | ListGameBinaryRecords")
	TArray<FAccelByteModelsGameBinaryRecord> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedUserBinaryRecords
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | PaginatedUserBinaryRecords")
	TArray<FAccelByteModelsUserBinaryRecord> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | PaginatedUserBinaryRecords")
	FAccelByteModelsPaging Paging{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPaginatedGameBinaryRecords
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | PaginatedGameBinaryRecords")
	TArray<FAccelByteModelsGameBinaryRecord> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Binary | Models | PaginatedGameBinaryRecords")
	FAccelByteModelsPaging Paging{};
};
