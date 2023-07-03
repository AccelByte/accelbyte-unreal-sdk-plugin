// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "AccelBytePresenceBroadcastEventModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteGameState : uint8
{
	OUT_OF_GAMEPLAY,
	IN_GAMEPLAY,
	STORE
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelBytePresenceBroadcastEventPayload
{
	GENERATED_BODY()

	/** @brief Unique ID for each game start, it will be the same until the game is closed even though user has been logged out */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | PresenceBroadcastEvent | Models | Payload")
	FString Flight_id{};

	/** @brief Platform name */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | PresenceBroadcastEvent | Models | Payload")
	FString Platform_name{};

	/** @brief Game state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | PresenceBroadcastEvent | Models | Payload")
	FString Game_state{};

	/** @brief Additional information of game state */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | PresenceBroadcastEvent | Models | Payload")
	FString Game_context{};

	/** @brief Additional event */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | PresenceBroadcastEvent | Models | Payload")
	FJsonObjectWrapper Additional_data{};
};
