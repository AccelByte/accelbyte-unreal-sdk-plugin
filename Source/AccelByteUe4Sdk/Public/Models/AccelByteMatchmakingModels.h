// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"

#include "Models/AccelByteDSMModels.h"
#include "AccelByteMatchmakingModels.generated.h"

/** @brief matchmaking session status enumeration. */
UENUM(BlueprintType)
enum class EAccelByteMatchmakingSessionStatus : uint8
{
	None = 0		UMETA(DisplayName = "none"),
	Matched	        UMETA(DisplayName = "matched"),
	Done			UMETA(DisplayName = "done"), // when matchmaking request is done successfully
	Cancel			UMETA(DisplayName = "cancel"), // when matchmaking request is cancelled
	Timeout			UMETA(DisplayName = "timeout"), // when matchmaking request is timed out
	SessionInQueue	UMETA(DisplayName = "sessionInQueue"), // when joinable session is in queue and players from other party can join
	SessionFull		UMETA(DisplayName = "sessionFull"), // when joinable session is full, and removed from queue
	SessionTimeout	UMETA(DisplayName = "sessionTimeout"), // when joinable session is timed out, and removed from queue
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	FString Channel{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	FString Client_version{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	FString Game_mode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	bool Joinable{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	FString Match_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	TArray<FAccelByteModelsMatchingAlly> Matching_allies{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	FJsonObjectWrapper Party_attributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	FString Party_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	int32 Queued_at{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	FString Server_name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchmakingResult")
	EAccelByteMatchmakingSessionStatus Status{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAddUserIntoSessionRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | AddUserIntoSessionRequest")
	FString User_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | AddUserIntoSessionRequest")
	FString Party_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDequeueRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | DequeueRequest")
	FString Match_id{};
};