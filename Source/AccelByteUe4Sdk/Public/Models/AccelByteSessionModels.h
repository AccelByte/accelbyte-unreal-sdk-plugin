// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "AccelByteDSMModels.h"
#include "AccelByteGeneralModels.h"
#include "AccelByteSessionModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteV2SessionJoinability : uint8
{
	EMPTY = 0, // Used for performing a query without a join type
	CLOSED,
	INVITE_ONLY,
	OPEN
};

UENUM(BlueprintType)
enum class EAccelByteV2SessionMemberStatus : uint8
{
	EMPTY = 0, // Used for performing a query without a status 
	INVITED,
	JOINED,
	CONNECTED,
	LEFT,
	DROPPED,
	REJECTED,
	KICKED,
	TIMEOUT,
	DISCONNECTED,
	TERMINATED
};

UENUM(BlueprintType)
enum class EAccelByteV2GameSessionDsStatus : uint8
{
	EMPTY = 0,
	NEED_TO_REQUEST,
	REQUESTED,
	AVAILABLE,
	FAILED_TO_REQUEST,
	ENDED,
	UNKNOWN
};

UENUM(BlueprintType)
enum class EAccelByteV2SessionConfigurationServerType : uint8
{
	EMPTY = 0,
	NONE,
	DS,
	P2P
};

UENUM(BlueprintType)
enum class EAccelByteV2SessionQueryComparisonOp : uint8
{
	EQUAL = 0,
	NOT_EQUAL,
	GREATER_THAN,
	GREATER_THAN_EQUAL,
	LESS_THAN,
	LESS_THAN_EQUAL,
	CONTAINS,
	NOT_CONTAINS
};

UENUM(BlueprintType)
enum class EAccelByteV2SessionType : uint8
{
	Unknown = 0,
	GameSession,
	PartySession
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionUser
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString ID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		EAccelByteV2SessionMemberStatus Status{EAccelByteV2SessionMemberStatus::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		EAccelByteV2SessionMemberStatus StatusV2 {EAccelByteV2SessionMemberStatus::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FDateTime UpdatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString PlatformID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString PlatformUserID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionConfiguration
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		EAccelByteV2SessionConfigurationServerType Type{EAccelByteV2SessionConfigurationServerType::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		EAccelByteV2SessionJoinability Joinability{EAccelByteV2SessionJoinability::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		int64 MinPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		int64 MaxPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		int64 InactiveTimeout{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		int64 InviteTimeout{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		FString Deployment{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		FString ClientVersion{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		TArray<FString> RequestedRegions{};
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Session | Models | SessionConfiguration")
		bool TextChat{false};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2BaseSession
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString ID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		bool IsActive{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FJsonObjectWrapper Attributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		TArray<FAccelByteModelsV2SessionUser> Members{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString CreatedBy{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString LeaderID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FDateTime CreatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FDateTime UpdatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FAccelByteModelsV2SessionConfiguration Configuration{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		int64 Version{};
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Session | Models | BaseSession")
		EAccelByteV2SessionType SessionType{};
	
	FAccelByteModelsV2BaseSession(EAccelByteV2SessionType Type) : SessionType(Type) {}
	FAccelByteModelsV2BaseSession() : SessionType(EAccelByteV2SessionType::Unknown) {}
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionTeamParties
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionTeamParties")
		FString PartyID {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionTeamParties")
		TArray<FString> UserIDs{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionTeam
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionTeam")
		TArray<FString> UserIDs{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionTeam")
		TArray<FAccelByteModelsV2GameSessionTeamParties> Parties{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionDSInformation
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSInformation")
		FAccelByteModelsServerInfo Server{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSInformation")
		FDateTime RequestedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSInformation")
		EAccelByteV2GameSessionDsStatus Status{EAccelByteV2GameSessionDsStatus::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSInformation")
		EAccelByteV2GameSessionDsStatus StatusV2 {EAccelByteV2GameSessionDsStatus::EMPTY};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSession : public FAccelByteModelsV2BaseSession
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		TArray<FAccelByteModelsV2GameSessionTeam> Teams{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FAccelByteModelsV2GameSessionDSInformation DSInformation{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FString BackfillTicketID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FString MatchPool{};
	FAccelByteModelsV2GameSession() : FAccelByteModelsV2BaseSession(EAccelByteV2SessionType::GameSession) {}
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2DSStatusChangedNotif
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSStatusChangedNotif")
		FAccelByteModelsServerInfo GameServer{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSStatusChangedNotif")
		FString SessionID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSStatusChangedNotif")
		FString Error{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSStatusChangedNotif")
		FAccelByteModelsV2GameSession Session{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionBaseRequest
{
	GENERATED_BODY();
	
	// Mandatory, Configuration template name to be used for this game session.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString ConfigurationName{};
	
	// Optional, specify the game session's attribute to be used when query.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FJsonObjectWrapper Attributes{};

	// Optional, if not empty will override the type defined in configuration template.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		EAccelByteV2SessionConfigurationServerType Type{EAccelByteV2SessionConfigurationServerType::EMPTY};

	// Optional, if not empty will override the type defined in configuration template.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		EAccelByteV2SessionJoinability Joinability{EAccelByteV2SessionJoinability::EMPTY};

	// Optional, if empty will be filled with the requester's Id.
	// Users added in this array will get an invitation to join the session.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		TArray<FAccelByteModelsV2GameSessionTeam> Teams{};

	// Optional, DS client version to be used if type is DS.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString ClientVersion{};

	// Optional, if session type is DS and this is not empty will request a local server with this name. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString ServerName{};

	// Optional, if session type is DS this will request DS from a specific deployment.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString Deployment{};

	// Optional, if session type is DS, this will request DS only from the specified regions.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		TArray<FString> RequestedRegions{};

	// Optional, maximum player of the session if set will override configuration from session template
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 MaxPlayers{};

	// Optional, minimum player of session if set will override configuration from session template
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 MinPlayers{};

	// Optional, timeout in seconds to kick a user if disconnected from lobby.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 InactiveTimeout{};

	// Optional, invitation timeout before the invitation is automatically rejected.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 InviteTimeout{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString MatchPool{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionCreateRequest : public FAccelByteModelsV2GameSessionBaseRequest
{
	GENERATED_BODY();

	// Optional, Also create group chat attached to this game session
	TOptional<bool> TextChat{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionUpdateRequest : public FAccelByteModelsV2GameSessionBaseRequest
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateRequest")
		int64 Version{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartySession : public FAccelByteModelsV2BaseSession
{
	GENERATED_BODY()
	
	FAccelByteModelsV2PartySession() : FAccelByteModelsV2BaseSession(EAccelByteV2SessionType::PartySession) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
	FString Code{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionInviteRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionInviteRequest")
		FString UserID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyLeaderPromotionRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyLeaderPromotionRequest")
	FString LeaderID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		TArray<FAccelByteModelsV2SessionUser> Members{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		FJsonObjectWrapper Attributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		EAccelByteV2SessionJoinability Joinability{EAccelByteV2SessionJoinability::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		FString ConfigurationName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		int64 MaxPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		int64 MinPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		int64 InactiveTimeout{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		int64 InviteTimeout{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		EAccelByteV2SessionConfigurationServerType Type{EAccelByteV2SessionConfigurationServerType::EMPTY};

		TOptional<bool> TextChat{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyUpdateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		FJsonObjectWrapper Attributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		EAccelByteV2SessionJoinability Joinability{EAccelByteV2SessionJoinability::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 Version{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		EAccelByteV2SessionConfigurationServerType Type{EAccelByteV2SessionConfigurationServerType::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 InactiveTimeout{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 InviteTimeout{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 MinPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 MaxPlayers{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyUserInvitedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserInvitedEvent")
		FString SenderID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserInvitedEvent")
		FString PartyID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PaginatedGameSessionQueryResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PaginatedGameSessionQueryResult")
		TArray<FAccelByteModelsV2GameSession> Data{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PaginatedGameSessionQueryResult")
		FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PaginatedPartyQueryResult
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PaginatedPartyQueryResult")
		TArray<FAccelByteModelsV2PartySession> Data{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PaginatedPartyQueryResult")
		FAccelByteModelsPaging Paging{};
};

// NOTE: This model will be used for more complex operations when the backend supports numeric range queries
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionQuery
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionQuery")
		FJsonObjectWrapper JsonWrapper;

	FAccelByteModelsV2GameSessionQuery();
	
	void AddParam(
		const FString& AttributeName,
		const EAccelByteV2SessionQueryComparisonOp Comparison,
		const FString& AttributeValue) const;

	void AddParam(
		const FString& AttributeName,
		const EAccelByteV2SessionQueryComparisonOp Comparison,
		const double AttributeValue) const;

	void AddParam(
		const FString& AttributeName,
		const EAccelByteV2SessionQueryComparisonOp Comparison,
		const TArray<FString>& AttributeValue) const;

	void AddParam(
		const FString& AttributeName,
		const EAccelByteV2SessionQueryComparisonOp Comparison,
		const TArray<double>& AttributeValue) const;

private:
	TSharedPtr<FJsonObject> InitializeComparisonField(const EAccelByteV2SessionQueryComparisonOp Op, FString& OutFieldName) const;

	void AddToQuery(const FString& ComparisonFieldName, const TSharedPtr<FJsonObject>& ComparisonField) const;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyInvitedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyInvitedEvent")
		FString SenderID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyInvitedEvent")
		FString PartyID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyMembersChangedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyMembersChangedEvent")
		FString PartyID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyMembersChangedEvent")
		FString JoinerID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyMembersChangedEvent")
		FString LeaderID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyMembersChangedEvent")
		TArray<FAccelByteModelsV2SessionUser> Members{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyMembersChangedEvent")
		FAccelByteModelsV2PartySession Session{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyUserRejectedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserRejectedEvent")
		FString PartyID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserRejectedEvent")
		FString RejectedID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserRejectedEvent")
		TArray<FAccelByteModelsV2SessionUser> Members{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyUserJoinedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserJoinedEvent")
		FString PartyID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserJoinedEvent")
		TArray<FAccelByteModelsV2SessionUser> Members{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyUserKickedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUserJoinedEvent")
		FString PartyID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionUserInvitedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUserInvitedEvent")
		FString SessionID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUserInvitedEvent")
		FString SenderID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionUserJoinedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUserJoinedEvent")
		FString SessionID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUserJoinedEvent")
		TArray<FAccelByteModelsV2SessionUser> Members{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionMembersChangedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionMembersChangedEvent")
		FString SessionID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionMembersChangedEvent")
		TArray<FAccelByteModelsV2SessionUser> Members{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionMembersChangedEvent")
		FString JoinerID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionMembersChangedEvent")
		FString LeaderID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionMembersChangedEvent")
		FAccelByteModelsV2GameSession Session{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionUserKickedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionKickedEvent")
		FString SessionID{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsV2GameSessionUserRejectedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUserRejectedEvent")
		FString SessionID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUserRejectedEvent")
		TArray<FAccelByteModelsV2SessionUser> Members{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUserRejectedEvent")
		FString RejectedID{};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsV2QueryPartiesRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryPartiesRequest")
		FString AttributeKey {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryPartiesRequest")
		FString AttributeValue {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryPartiesRequest")
		FString LeaderID {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryPartiesRequest")
		FString MemberID {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryPartiesRequest")
		EAccelByteV2SessionMemberStatus MemberStatus{EAccelByteV2SessionMemberStatus::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryPartiesRequest")
		EAccelByteV2SessionJoinability JoinType{EAccelByteV2SessionJoinability::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryPartiesRequest")
		FString OrderBy {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryPartiesRequest")
		FString Order {};
};

USTRUCT(BlueprintType)
struct FAccelByteModelsV2QueryPartiesResponse
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryPartiesResponse")
		TArray<FAccelByteModelsV2PartySession> Data{};
};
