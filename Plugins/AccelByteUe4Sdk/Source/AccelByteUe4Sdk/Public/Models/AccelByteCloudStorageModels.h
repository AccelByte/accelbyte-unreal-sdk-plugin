// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
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
		FString Checksum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		FString CustomAttribute;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		float DateAccessed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		float DateCreated;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		float DateModified;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		FString Label;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		FString MimeType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		FString OriginalName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		FString SlotId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		FString Status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		FString StoredName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		TArray<FString> Tags;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | CloudStorage | Models | CreateSlotResponse")
		FString UserId;
};
