// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"

#include "Models/AccelByteDSMModels.h"
#include "Models/AccelByteSessionModels.h"
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

struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchTicketOptionalParams
{
	FString SessionId{};
	FJsonObjectWrapper Attributes{};
	TArray<TPair<FString, float>> Latencies{};	
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

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchmakingCreateTicketRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | CreateTicketRequest")
	FJsonObjectWrapper Attributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | CreateTicketRequest")
	TMap<FString, float> Latencies{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | CreateTicketRequest")
	FString MatchPool{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | CreateTicketRequest")
	FString SessionId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchmakingCreateTicketResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | CreateTicketResponse")
	FString MatchTicketId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchmakingGetTicketDetailsResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | GetTicketDetailsResponse")
	bool MatchPool{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | GetTicketDetailsResponse")
	FString SessionId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2Ticket
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchFoundNotif")
	FString Ticket_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchFoundNotif
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchFoundNotif")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchFoundNotif")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchFoundNotif")
	FDateTime Created_at{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchFoundNotif")
	FString Match_pool{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchFoundNotif")
	TArray<FAccelByteModelsV2GameSessionTeam> Teams{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchFoundNotif")
	TArray<FAccelByteModelsV2Ticket> Tickets{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2StartMatchmakingNotif
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | StartMatchmakingNotif")
	FString Ticket_id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | StartMatchmakingNotif")
	FString Party_id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | StartMatchmakingNotif")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | StartMatchmakingNotif")
	FDateTime Created_at{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | StartMatchmakingNotif")
	FString Match_pool{};
};