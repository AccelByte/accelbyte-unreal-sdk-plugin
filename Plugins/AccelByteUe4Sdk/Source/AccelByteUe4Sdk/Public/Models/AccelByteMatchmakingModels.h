// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"

#include "AccelByteDSMModels.h"
#include "AccelByteMatchmakingModels.generated.h"

UENUM(BlueprintType)
/**
 * @brief matchmaking session status enumeration.
 */
	enum class EAccelByteMatchmakingSessionStatus : uint8
{
	None = 0		UMETA(DisplayName = "none"),
	Done			UMETA(DisplayName = "done"), // when matchmaking request is done successfully
	Cancel			UMETA(DisplayName = "cancel"), // when matchmaking request is cancelled
	Timeout			UMETA(DisplayName = "timeout"), // when matchmaking request is timed out
	SessionInQueue	UMETA(DisplayName = "sessionInQueue"), // when joinable session is in queue and players from other party can join
	SessionFull		UMETA(DisplayName = "sessionFull"), // when joinable session is full, and removed from queue
	SessionTimeout	UMETA(DisplayName = "sessionTimeout") // when joinable session is timed out, and removed from queue
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		FString Channel;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		FString Client_version;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		FString Game_mode;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		bool Joinable;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		FString Match_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		TArray<FAccelByteModelsMatchingAlly> Matching_allies;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		FJsonObjectWrapper Party_attributes;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		FString Party_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		int32 Queued_at;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		FString Region;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		FString Server_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
		EAccelByteMatchmakingSessionStatus Status;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAddUserIntoSessionRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | AddUserIntoSessionRequest")
		FString User_id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | AddUserIntoSessionRequest")
		FString Party_id;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDequeueRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Server | Matchmaking | Models | DequeueRequest")
		FString Match_id;
};