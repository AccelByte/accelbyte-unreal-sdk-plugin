// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "Models/AccelByteGeneralModels.h"
#include "Models/AccelByteUserModels.h"
#include "Models/AccelByteDSMModels.h"
#include "AccelByteLobbyModels.generated.h"

/** @brief presence enumeration. */
UENUM(BlueprintType)
enum class EAvailability : uint8
{
	Offline = 0,
	Online = 1,
	Busy = 2,
	Invisible = 3,
	Away = 4
};

UENUM(BlueprintType)
enum class EHandleLobbyMessageDataType : uint8
{
	None,
	V2Matchmaking,
	Session,
	Other,
};

/** @brief Matchmaking v2 notification topics enums. */
UENUM(BlueprintType)
enum class EV2MatchmakingNotifTopic : uint8
{
	Invalid,
	OnMatchFound,
	OnMatchmakingStarted,
	OnMatchmakingTicketExpired,
	OnMatchmakingTicketCanceled,
};

/** @brief Matchmaking v2 notification topics enums. */
UENUM(BlueprintType)
enum class EV2SessionNotifTopic : uint8
{
	Invalid,
	OnPartyInvited,
	OnPartyInviteTimeout,
	OnPartyMembersChanged,
	OnPartyJoined,
	OnPartyRejected,
	OnPartyKicked,
	OnPartyCreated,
	OnSessionInvited,
	OnGameSessionInviteTimeout,
	OnSessionJoined,
	OnSessionMembersChanged,
	OnSessionKicked,
	OnSessionRejected,
	OnDSStatusChanged,
	OnPartyUpdated,
	OnGameSessionUpdated,
	OnSessionStorageChanged,
	OnSessionJoinedSecret,
	OnSessionEnded,
	OnGameSessionInviteCancelled,
	OnPartyCancelled
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FLobbySequence
{
	GENERATED_BODY()

public:
	FLobbySequence(){}

protected:
	FLobbySequence(const FLobbySequence& Other) : Value(Other.GetValue()){}

	explicit FLobbySequence(const int32 Other) : Value(Other){}

	explicit FLobbySequence(const FString& Other)
	{
		if(!Other.IsNumeric())
			return;
		
		Value = FCString::Atoi(*Other);
	}

public:
	int32 GetValue() const
	{
		return Value;
	}

	void SetValue(const int32 InValue)
	{
		Value = InValue;
	}

#pragma region Self Comparison
	bool operator==(const FLobbySequence& Other) const
	{
		return Value == Other.GetValue();
	}

	bool operator!=(const FLobbySequence& Other) const
	{
		return Value != Other.GetValue();
	}
	
	bool operator>(const FLobbySequence& Other) const
	{
		return Value > Other.GetValue();
	}

	bool operator>=(const FLobbySequence& Other) const
	{
		return Value >= Other.GetValue();
	}

	bool operator<(const FLobbySequence& Other) const
	{
		return Value < Other.GetValue();
	}

	bool operator<=(const FLobbySequence& Other) const
	{
		return Value <= Other.GetValue();
	}
#pragma endregion

#pragma region Int Comparison
	bool operator==(const int32 Other) const
	{
		return Value == Other;
	}

	bool operator!=(const int32 Other) const
	{
		return Value != Other;
	}
	
	bool operator>(const int32 Other) const
	{
		return Value > Other;
	}

	bool operator>=(const int32 Other) const
	{
		return Value >= Other;
	}

	bool operator<(const int32 Other) const
	{
		return Value < Other;
	}

	bool operator<=(const int32 Other) const
	{
		return Value <= Other;
	}
#pragma endregion

private:
	int32 Value {0};
	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FLobbySequenceID : public FLobbySequence
{
	GENERATED_BODY()
	
	FLobbySequenceID(){}
	
	FLobbySequenceID(const FLobbySequenceID& Other) : FLobbySequence(Other) {}

	explicit FLobbySequenceID(const FLobbySequence& Other) : FLobbySequence(Other) {}

	explicit FLobbySequenceID(const int32 Other) : FLobbySequence(Other) {}

	explicit FLobbySequenceID(const FString& Other) : FLobbySequence(Other)	{}
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FLobbySequenceNumber : public FLobbySequence
{
	GENERATED_BODY()

public:
	FLobbySequenceNumber(){}
	
	FLobbySequenceNumber(const FLobbySequenceNumber& Other) : FLobbySequence(Other) {}

	explicit FLobbySequenceNumber(const FLobbySequence& Other) : FLobbySequence(Other) {}

	explicit FLobbySequenceNumber(const int32 Other) : FLobbySequence(Other) {}

	explicit FLobbySequenceNumber(const FString& Other) : FLobbySequence(Other) {}
};

// Emulate namespace with long class names
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLobbyType
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString Type{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLobbyBaseResponse : public FAccelByteModelsLobbyType 
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Code{};
};

// ------------------------------------------------------------------------------------------------
// Party 
// ------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FJsonObjectWrapper Custom_Attribute{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Invitees{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Members{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	int64 UpdatedAt{};
};

// Get current party info
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsInfoPartyResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString LeaderId{}; // planned to be deprecated
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Members{};
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Invitees{};
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString InvitationToken{};
};


// Get current party data
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDataPartyResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TMap<FString, FString> custom_attribute{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString updatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString partyId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString leader{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> members{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> invitees{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString invitationToken{};
};

//Create new party
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCreatePartyResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString LeaderId{}; // planned to be deprecated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Members{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Invitees{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString InvitationToken{};
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyCode{};
};

//Leave current party
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLeavePartyResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()
};

//Party leave notice
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLeavePartyNotice
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString LeaderID{}; // planned to be deprecated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};
};

//Invite user to party
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyInviteResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString InviteeID{};
};

//Invitation notice
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsInvitationNotice
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString InviterID{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString InviteeID{};
};

//Being invited
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyGetInvitedNotice
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString From{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString InvitationToken{};
};

// Accept Invitation
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyJoinResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString LeaderId{}; // planned to be deprecated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Members{};
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Invitees{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString InvitationToken{};
};

// Invitation being accepted
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyJoinNotice
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString UserId{};
};

// Reject Invitation
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyRejectResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};
};

// Invitation being rejected
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyRejectNotice
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString LeaderId{};// planned to be deprecated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserId{};
};

// Kick member
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsKickPartyMemberResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString UserId{};
};

// Generate party Code
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyGenerateCodeResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyCode{};
};

// Get party Code
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyGetCodeResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyCode{};
};

// Delete party Code
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyDeleteCodeResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()
};

//Being kicked
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGotKickedFromPartyNotice
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString LeaderId{};// planned to be deprecated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};
	
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};
};

// Party member data model for party member reconnect and disconnect notification
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyMemberConnectionNotice
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};
};

//Kick notice
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsKickNotice
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString LeaderId{};// planned to be deprecated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserId{};
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyPromoteLeaderResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString LeaderId{};// planned to be deprecated

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Members{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Invitees{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString invitationToken{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartySendNotifResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartySetLimitRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Category | Models | Lobby")
	int32 Limit {0};
};

// ------------------------------------------------------------------------------------------------
// Chat 
// ------------------------------------------------------------------------------------------------
// Send personal chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPersonalMessageResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()
};

// Receive personal chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPersonalMessageNotice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString From{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString To{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Payload{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString ReceivedAt{};
};

// Send party chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyMessageResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()
};

//Receive party chat 
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyMessageNotice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString From{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString To{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Payload{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString ReceivedAt{};
};

// join default channel chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsJoinDefaultChannelResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString ChannelSlug{};
};

// send channel chat request
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChannelMessageRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString ChannelSlug{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString payload{};
};

// Receive channel chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChannelMessageNotice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString From{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString ChannelSlug{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Payload{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString SentAt{};
};

// Send channel chat
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChannelMessageResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Message{};
};

// ------------------------------------------------------------------------------------------------
// Presence 
// ------------------------------------------------------------------------------------------------
// Set user status
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSetOnlineUsersResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUsersPresenceNotice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	EAvailability Availability{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Activity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Platform{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FDateTime LastSeenAt{0};
};

// List all friends presence
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetOnlineUsersResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Code{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString type{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> friendsId{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    TArray<EAvailability> availability{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    TArray<FString> activity{};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    TArray<FString> lastSeenAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Platform{};
};

// ------------------------------------------------------------------------------------------------
// Notification 
// ------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsNotificationMessage
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString From{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString To{};
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString Topic{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Payload{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FDateTime SentAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString SequenceID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	int32 SequenceNumber{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Type{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSessionNotificationMessage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Topic{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Payload{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FDateTime SentAt{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString SequenceID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	int32 SequenceNumber{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFreeFormNotificationRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Topic{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Message{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyNotif 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Category | Models | Lobby")
	FString Sender{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Category | Models | Lobby")
	FString Topic{};
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AccelByte | Category | Models | Lobby")
	FString Payload{};
};

// ------------------------------------------------------------------------------------------------
// Matchmaking
// ------------------------------------------------------------------------------------------------
UENUM(BlueprintType)
/** @brief matchmaking status enumeration. */
enum class EAccelByteMatchmakingStatus : uint8
{
	Unknown = 0	UMETA(DisplayName = "unknown"),
	Start		UMETA(DisplayName = "start"),
	Timeout		UMETA(DisplayName = "timeout"),
	Cancel		UMETA(DisplayName = "cancel"),
	Done		UMETA(DisplayName = "done"),
	Unavailable UMETA(DisplayName = "unavailable"),
	Rejected	UMETA(DisplayName = "rejected"),
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchingAllies
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FAccelByteModelsMatchingAlly> Data{};
};

// Matchmaking notification
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsMatchmakingNotice
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    FString MatchId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
    EAccelByteMatchmakingStatus Status{EAccelByteMatchmakingStatus::Unknown};
    
   	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
   	FString GameMode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Deployment{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString ClientVersion{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	bool Joinable{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FAccelByteModelsMatchingAllies MatchingAllies{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Region {};
	
	// rejected status message
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Message{};

	// rejected status error code
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	int32 ErrorCode{};
};

// DS Notification
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDsNotice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString MatchId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PodName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Ip{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString ImageVersion{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString ServerVersion{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	int32 Port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Message{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString IsOK{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TMap<FString, int32> Ports{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString CustomAttribute{};
};

// Ready Consent Request
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsReadyConsentRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString MatchId{};
};

// Ready Consent Notice
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsReadyConsentNotice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString MatchId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserId{};
};

// Reject Consent Request
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRejectConsentRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString MatchId{};
};

// Reject Consent Notice
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRejectConsentNotice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString MatchId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserId{};
};

// Rematchmaking Notice
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRematchmakingNotice
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	int32 BanDuration{};
};

// Lobby Disconnect Notice
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsDisconnectNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | General | DisconnectNotif")
	FString Message{};
};

// ------------------------------------------------------------------------------------------------
// Friends
// ------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRequestFriendsResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | RequestFriendsResponse")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUnfriendResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | UnfriendResponse")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsListOutgoingFriendsResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | ListOutgoingFriendsResponse")
	TArray<FString> friendsId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendRequestData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | FriendRequestData")
	FString FriendId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | FriendRequestData")
	FDateTime RequestedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsListOutgoingFriendsWithTimeResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | ListOutgoingFriendsResponse")
	TArray<FAccelByteModelsFriendRequestData> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCancelFriendsResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | CancelFriendsResponse")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsListIncomingFriendsResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | ListIncomingFriendsResponse")
	TArray<FString> friendsId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsListIncomingFriendsWithTimeResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | ListOutgoingFriendsResponse")
	TArray<FAccelByteModelsFriendRequestData> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAcceptFriendsResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | AcceptFriendsResponse")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRejectFriendsResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | RejectFriendsResponse")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLoadFriendListResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | ListOfFriendsResponse")
	TArray<FString> friendsId{};
};

/** @brief Friends relation enumeration. */
UENUM(BlueprintType)
enum class ERelationshipStatusCode : uint8
{
	Friend = 3,
	Incoming = 2,
	Outgoing = 1,
	NotFriend = 0
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetFriendshipStatusResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | GetFriendshipStatusResponse")
	ERelationshipStatusCode friendshipStatus{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | GetFriendshipStatusResponse")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsGetFriendshipStatusStringResponse : public FAccelByteModelsLobbyBaseResponse
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, NotBlueprintType, Category = "AccelByte | Lobby | Models | Friends | GetFriendshipStatusStringResponse")
	FString friendshipStatus{};

	UPROPERTY(EditAnywhere, NotBlueprintType, Category = "AccelByte | Lobby | Models | Friends | GetFriendshipStatusStringResponse")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsAcceptFriendsNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | AcceptFriendsNotif")
	FString friendId{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsRequestFriendsNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | RequestFriendsNotif")
	FString friendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkFriendsRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | BulkFriendsRequest")
	TArray<FString> FriendIds{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsUnfriendNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | UnfriendNotif")
	FString friendId{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsCancelFriendsNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | CancelFriendNotif")
	FString userId{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsRejectFriendsNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | RejectFriendNotif")
	FString userId{};
};
		
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserStatusNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | UserStatusNotif")
	FString UserID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | UserStatusNotif")
	EAvailability Availability{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | UserStatusNotif")
	FString Activity{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | UserStatusNotif")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | UserStatusNotif")
	FString LastSeenAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | UserStatusNotif")
	FString Platform{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBulkUserStatusNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | BulkUserStatusNotif")
	TArray<FAccelByteModelsUserStatusNotif> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | BulkUserStatusNotif")
	int32 Online{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | BulkUserStatusNotif")
	int32 Busy{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | BulkUserStatusNotif")
	int32 Invisible{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | BulkUserStatusNotif")
	int32 Offline{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | BulkUserStatusNotif")
	int32 Away{};

	/** @brief UserIds that not processed because of the url limit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Lobby | BulkUserStatusNotif")
	TArray<FString> NotProcessed{};
};

// ------------------------------------------------------------------------------------------------
// Party Storage
// ------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyDataNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PartyId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Leader{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Members{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FString> Invitees{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FJsonObjectWrapper Custom_attribute{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UpdatedAt{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyDataUpdateRequest
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	int64 UpdatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FJsonObjectWrapper Custom_attribute{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPartyDataUpdateStringRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UpdatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FJsonObjectWrapper Custom_attribute{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsActivePartiesData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FAccelByteModelsPartyDataNotif> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FAccelByteModelsPaging Paging{};
};

// ------------------------------------------------------------------------------------------------
// Session Attribute
// ------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSetSessionAttributesResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetSessionAttributesResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Value{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetAllSessionAttributesResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FJsonObjectWrapper attributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsSetSessionAttributeRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TMap<FString, FString> Attributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetSessionAttributeResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Key{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Value{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetSessionAttributeAllResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TMap<FString, FString> Attributes{};
};

// ------------------------------------------------------------------------------------------------
// Block Players
// ------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBlockPlayerResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | BlockPlayerResponse")
	FString BlockedUserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUnblockPlayerResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | UnblockPlayerResponse")
	FString UnblockedUserId{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsBlockPlayerNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | BlockPlayerNotif")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | BlockPlayerNotif")
	FString BlockedUserId{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsUnblockPlayerNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | UnblockPlayerNotif")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends | UnblockPlayerNotif")
	FString UnblockedUserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBlockedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString BlockedUserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsListBlockedUserResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FBlockedData> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBlockerData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsListBlockerResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FBlockerData> Data{};
};

// ------------------------------------------------------------------------------------------------
// General
// ------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLobbySessionId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | General | LobbySessionId")
	FString LobbySessionID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | General | LobbySessionId")

	FString SequenceID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | General | LobbySessionId")
	int32 SequenceNumber{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FLobbyMessages
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="AccelByte | Lobby | Models | General | LobbyError")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="AccelByte | Lobby | Models | General | LobbyError")
	FString CodeName{};
};

// ------------------------------------------------------------------------------------------------
// Ban
// ------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserBannedNotification
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	EBanType Ban{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString EndDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	EBanReason Reason{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	bool Enable{};
};

// ------------------------------------------------------------------------------------------------
// Third Party Friend Sync
// ------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FAccelByteModelsSyncThirdPartyFriendInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	bool IsLogin{false};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PlatformId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PlatformToken{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PsnEnv{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsSyncThirdPartyFriendsRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FAccelByteModelsSyncThirdPartyFriendInfo> FriendSyncDetails;
};

USTRUCT(BlueprintType)
struct FAccelByteModelsSyncThirdPartyFriendsResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Detail{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PlatformId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Status{};
};

// ------------------------------------------------------------------------------------------------
// Third Party Block List Sync
// ------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct FAccelByteModelsSyncThirdPartyBlockListInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PlatformId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PsnEnv{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsSyncThirdPartyBlockListRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	TArray<FAccelByteModelsSyncThirdPartyBlockListInfo> BlockListSyncDetails{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsSyncThirdPartyBlockListResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Detail{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString PlatformId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby")
	FString Status{};
};

// ------------------------------------------------------------------------------------------------
// Refresh Token
// ------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRefreshTokenResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()
};

// ------------------------------------------------------------------------------------------------
// Metrics
// ------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsChangeUserRegionResponse : public FAccelByteModelsLobbyBaseResponse
{
	GENERATED_BODY()
};

// ------------------------------------------------------------------------------------------------
// Lobby Notification via REST API
// ------------------------------------------------------------------------------------------------
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserNotification
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	FString From{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	FString Id{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	FString LobbySessionID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	FString LoginType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	FString Payload{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	int32 ReconnectFromCode{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	FDateTime SentAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	int32 SequenceID{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	int32 SequenceNumber{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	FString To{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	FString Topic{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	FString Type{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGetUserNotificationsResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Category | Models | Lobby | UserNotification")
	TArray<FAccelByteModelsUserNotification> Notifications{};
};

// ------------------------------------------------------------------------------------------------
// Friends REST API
// ------------------------------------------------------------------------------------------------

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendPlatformInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString PlatformId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsQueryFriendListResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	TArray<FString> FriendIds{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	TArray<FAccelByteModelsFriendPlatformInfo> Friends{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString FriendID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FDateTime RequestedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsIncomingFriendRequests
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	TArray<FAccelByteModelsFriendRequest> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOutgoingFriendRequests
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	TArray<FAccelByteModelsFriendRequest> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendRequestByUserId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendRequestByPublicId
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString FriendPublicId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsCancelFriendRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsAcceptFriendRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsRejectFriendRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsFriendshipStatusResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	int32 Code{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString Name{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUnfriendRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString FriendId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBlockUserRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString BlockedUserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUnlockUserRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Friends")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOneTimeCodeLinked 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | MessageNotif")
	FString InitiatorGameUserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | MessageNotif")
	FString InitiatorNamespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | MessageNotif")
	FString InitiatorUserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | MessageNotif")
	FString LinkPlatformId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | MessageNotif")
	FString UserId{};
};

class FAccelByteKey
{};

namespace AccelByte
{
namespace MatchExtraAttributes
{
/** @brief attribute to start role based matchmaking */
const FString Role = TEXT("role");
}

namespace SessionAttributeKeys
{
const FString Role = TEXT("role");
}
}