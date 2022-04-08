// Copyright (c) 2018-2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteCloudStorageModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FString Checksum{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FString CustomAttribute{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FDateTime DateAccessed{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FDateTime DateCreated{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FDateTime DateModified{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FString Label{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FString MimeType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FString OriginalName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FString SlotId{};

	UPROPERTY(meta = (DeprecatedNode, DeprecationMessage = "Status member is deprecated on new cloudstorage service."))
	FString Status_DEPRECATED{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FString StoredName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	FString UserId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
	int32 Size{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUpdateMetadataRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | UpdateMetadataRequest")
	FString Label{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | UpdateMetadataRequest")
	TArray<FString> Tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | UpdateMetadataRequest")
	FString CustomAttribute{};
};
