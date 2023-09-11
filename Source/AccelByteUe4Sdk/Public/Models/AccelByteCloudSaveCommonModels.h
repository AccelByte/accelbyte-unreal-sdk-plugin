// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Dom/JsonObject.h" 
#include "JsonObjectWrapper.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteCloudSaveCommonModels.generated.h"

UENUM(BlueprintType)
enum class ESetByMetadataRecord : uint8
{
	NONE = 0,
	SERVER,
	CLIENT,
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkGetRecordsByKeysRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Accelbyte | CloudSave | Models | ListRecordRequest")
	TArray<FString> Keys{};
};