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

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchTicketOptionalParams
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchTicketOptionalParams")
	FString SessionId{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | MatchTicketOptionalParams")
	FJsonObjectWrapper Attributes{};
	
	// Cannot expose this field as UPROPERTY as TPair is not a blueprint type
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
	EAccelByteMatchmakingSessionStatus Status{EAccelByteMatchmakingSessionStatus::None};
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
	TMap<FString, int32> Latencies{};

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
	bool MatchFound{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | Models | GetTicketDetailsResponse")
	FString SessionId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2Ticket
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | Ticket")
	FString TicketId{};
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
	FDateTime CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchFoundNotif")
	FString MatchPool{};

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
	FString TicketID{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | StartMatchmakingNotif")
	FString PartyID{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | StartMatchmakingNotif")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | StartMatchmakingNotif")
	FDateTime CreatedAt{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | StartMatchmakingNotif")
	FString MatchPool{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchmakingExpiredNotif
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingExpiredNotif")
	FString TicketID{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingExpiredNotif")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingExpiredNotif")
	FDateTime CreatedAt{0};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingExpiredNotif")
	FString MatchPool{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchmakingCanceledNotif
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingCanceledNotif")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingCanceledNotif")
	TArray<FString> UserIDs{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchmakingTicketPlayerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchTicketPlayerData")
	FString PlayerID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchTicketPlayerData")
	FJsonObjectWrapper Attributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchmakingTicket
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchTicket")
	FString TicketID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchTicket")
	FString MatchPool{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchTicket")
	FDateTime CreatedAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchTicket")
	TMap<FString, int> Latencies{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchTicket")
	TArray<FAccelByteModelsV2MatchmakingTicketPlayerData> Players{}; 
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2MatchmakingBackfillProposalNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingBackfillProposalNotif")
	FString BackfillTicketID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingBackfillProposalNotif")
	FString ProposalID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingBackfillProposalNotif")
	FString MatchPool{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingBackfillProposalNotif")
	FString MatchSessionID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingBackfillProposalNotif")
	TArray<FAccelByteModelsV2GameSessionTeam> ProposedTeams{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Server | Matchmaking | v2 | Models | MatchmakingBackfillProposalNotif")
	TArray<FAccelByteModelsV2MatchmakingTicket> AddedTickets{};
};
