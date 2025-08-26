// Copyright (c) 2022 -2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "AccelByteDSMModels.h"
#include "AccelByteGeneralModels.h"
#include "Math/NumericLimits.h"
#include "AccelByteSessionModels.generated.h"

#pragma region Enum

UENUM(BlueprintType)
enum class EAccelByteV2SessionJoinability : uint8
{
	EMPTY = 0, // Used for performing a query without a join type
	CLOSED,
	INVITE_ONLY,
	OPEN,
	FRIENDS_OF_LEADER,
	FRIENDS_OF_FRIENDS,
	FRIENDS_OF_MEMBERS
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
	TERMINATED,
	CANCELLED
};

UENUM(BlueprintType)
enum class EAccelByteV2GameSessionDsStatus : uint8
{
	EMPTY = 0,
	NEED_TO_REQUEST,
	REQUESTED,
	PREPARING,
	AVAILABLE,
	FAILED_TO_REQUEST,
	ENDED,
	UNKNOWN,
	DS_ERROR,
	DS_CANCELLED
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

UENUM(BlueprintType)
enum class EAccelByteV2SessionPlatform : uint8
{
	Unknown = 0,
	Steam,
	Xbox,
	PS4,
	PS5
};
#pragma endregion

#pragma region Game Session Struct Models

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
		EAccelByteV2SessionMemberStatus PreviousStatus{EAccelByteV2SessionMemberStatus::EMPTY};
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
		int64 MinPlayers{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		int64 MaxPlayers{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		int64 InactiveTimeout{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		int64 InviteTimeout{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		FString Deployment{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		FString ClientVersion{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		TArray<FString> RequestedRegions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		bool EnableSecret{false};
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Session | Models | SessionConfiguration")
		bool TextChat{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		bool Persistent{false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		bool AutoJoin{false};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelbyteModelsV2SessionStorage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession | Storage")
	FJsonObjectWrapper Leader{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession | Storage")
	TMap<FString, FJsonObjectWrapper> Member{};
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
		int64 Version{TNumericLimits<int32>::Min()};
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Session | Models | BaseSession")
		EAccelByteV2SessionType SessionType{};
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Session | Models | BaseSession")
		FAccelbyteModelsV2SessionStorage Storage{};
	
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
		FString TeamID{}; // Not mandatory and provided by developer
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FString Code{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		TArray<FString> TicketIDs{};
	
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
		int64 MaxPlayers{TNumericLimits<int32>::Min()};

	// Optional, minimum player of session if set will override configuration from session template
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 MinPlayers{TNumericLimits<int32>::Min()};

	// Optional, timeout in seconds to kick a user if disconnected from lobby.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 InactiveTimeout{TNumericLimits<int32>::Min()};

	// Optional, invitation timeout before the invitation is automatically rejected.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 InviteTimeout{TNumericLimits<int32>::Min()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString MatchPool{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionCreateStorage
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest | Storage")
	FJsonObjectWrapper Leader;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionCreateRequest : public FAccelByteModelsV2GameSessionBaseRequest
{
	GENERATED_BODY();

	// Optional, Also create group chat attached to this game session
	TOptional<bool> TextChat{};

	// Optional, Enable auto join, the invited user will be joined directly from the backend
	TOptional<bool> AutoJoin{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
	FAccelByteModelsV2GameSessionCreateStorage Storage;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionUpdateRequest : public FAccelByteModelsV2GameSessionBaseRequest
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateRequest")
		int64 Version{TNumericLimits<int32>::Min()};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateRequest")
		bool bIncludeEmptyTeams {false};
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
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartySessionStorageReservedData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySessionStorageReservedData")
	TArray<FString> PastSessionIDs{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartySessionStorage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySessionStorage")
	TMap<FString /*UserID*/, FAccelByteModelsV2PartySessionStorageReservedData> Reserved{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionInviteRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionInviteRequest")
		FString UserID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionInvitePlatformRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionInvitePlatformRequest")
	FString PlatformID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionInvitePlatformRequest")
	FString UserID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionInvitePlatformResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionInvitePlatformResponse")
	FString PlatformUserID{};
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
		int64 MaxPlayers{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		int64 MinPlayers{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		int64 InactiveTimeout{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreateRequest")
		int64 InviteTimeout{TNumericLimits<int32>::Min()};
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
		int64 Version{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		EAccelByteV2SessionConfigurationServerType Type{EAccelByteV2SessionConfigurationServerType::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 InactiveTimeout{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 InviteTimeout{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 MinPlayers{TNumericLimits<int32>::Min()};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 MaxPlayers{TNumericLimits<int32>::Min()};
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyInvitedEvent")
		FDateTime ExpiredAt{0};
};

// It notifies the invitee about the expired party invitation
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyInviteTimeoutEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyInviteTimeoutNotification")
	FString PartyID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyCreatedEvent 
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreatedNotification")
	FString CreatedBy{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreatedNotification")
	int32 InactiveTimeout{ 0 };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreatedNotification")
	FString PartyID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreatedNotification")
	bool TextChat{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyCreatedNotification")
	FString Code{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2ImpactedUserIds
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ImpactedUserIds")
	TArray<FString> InvitedUserIDs{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ImpactedUserIds")
	TArray<FString> LeftUserIDs{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ImpactedUserIds")
	TArray<FString> KickedUserIDs{};
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyMembersChangedEvent")
		FAccelByteModelsV2ImpactedUserIds ImpactedUserIDs{};
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUserInvitedEvent")
		FDateTime ExpiredAt{ 0 };
};

// It notifies the invitee about the expired game session invitation
USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionUserInviteTimeoutEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUserInviteTimeoutEvent")
	FString SessionID{};
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionMembersChangedEvent")
		FAccelByteModelsV2ImpactedUserIds ImpactedUserIDs{};
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

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PlayerAttributesPlatform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributesPlatform")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributesPlatform")
	FString UserID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PlayerAttributes
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributes")
	FString UserID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributes")
	FString CurrentPlatform{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributes")
	FJsonObjectWrapper Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributes")
	bool CrossplayEnabled{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributes")
	TArray<FAccelByteModelsV2PlayerAttributesPlatform> Platforms{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributes")
	TArray<FString> Roles{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2StorePlayerAttributesRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributesRequest")
	FString CurrentPlatform{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributesRequest")
	FJsonObjectWrapper Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributesRequest")
	bool CrossplayEnabled{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributesRequest")
	TArray<FAccelByteModelsV2PlayerAttributesPlatform> Platforms{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PlayerAttributesRequest")
	TArray<FString> Roles{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2NativeSessionSyncNotif
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | NativeSessionSyncNotif")
	FString PlatformName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | NativeSessionSyncNotif")
	FString PlatformSessionID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | NativeSessionSyncNotif")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | NativeSessionSyncNotif")
	FString SessionID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2ServerQueryGameSessionsRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	EAccelByteV2GameSessionDsStatus Status{EAccelByteV2GameSessionDsStatus::EMPTY};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	EAccelByteV2GameSessionDsStatus StatusV2 {EAccelByteV2GameSessionDsStatus::EMPTY};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FString SessionID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FString MatchPool{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FString GameMode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	EAccelByteV2SessionJoinability Joinability{EAccelByteV2SessionJoinability::EMPTY};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FString MemberID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FString ConfigurationName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FDateTime FromTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FDateTime ToTime{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FString DsPodName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	bool IsSoftDeleted{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	bool IsPersistent{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FString OrderBy{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | ServerQueryGameSessionsRequest")
	FString Order{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionLeaderPromotionRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionLeaderPromotionRequest")
	FString LeaderID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionTeamsSetting
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionTeamsSetting")
	TArray<FAccelByteModelsV2GameSessionTeam> Teams;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionStorageChangedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2SessionStorageChangedEvent")
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2SessionStorageChangedEvent")
	FString SessionID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2SessionStorageChangedEvent")
	FString ActorUserID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2SessionStorageChangedEvent")
	bool IsLeader{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2SessionStorageChangedEvent")
	FJsonObjectWrapper StorageChanges{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionEndedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionEndedEvent")
	FString SessionID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionEndedEvent")
	bool TextChat{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionInviteCanceledEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionInviteCanceledEvent")
	TArray<FAccelByteModelsV2SessionUser> Members{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionInviteCanceledEvent")
	FString SessionID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionInviteCanceledEvent")
	TArray<FString> CancelledUserIDs{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyInviteCanceledEvent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionInviteCanceledEvent")
	TArray<FAccelByteModelsV2SessionUser> Members{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionInviteCanceledEvent")
	FString PartyID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionInviteCanceledEvent")
	TArray<FString> CancelledUserIDs{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2DSSessionReadyRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2GameSessionEndedEvent")
	bool Ready{false};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionRecentPlayer
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2RecentPlayer")
	FString Namespace {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2RecentPlayer")
	FString PlatformName {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2RecentPlayer")
	FString UserId {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2RecentPlayer")
	FDateTime LastPlayedTime {0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2RecentPlayer")
	FString MatchPool {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2RecentPlayer")
	FString SessionTemplate {};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionRecentPlayers
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | FAccelByteModelsV2SessionRecentPlayers")
	TArray<FAccelByteModelsV2SessionRecentPlayer> Data{};
};

USTRUCT(BlueprintType) 
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionJoinedSecret
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionJoinedSecret")
	FString Secret{};
};

#pragma endregion

#pragma region Game Session History Struct Models

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameSessionHistoryConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	EAccelByteV2SessionConfigurationServerType Type{ EAccelByteV2SessionConfigurationServerType::EMPTY };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	EAccelByteV2SessionJoinability Joinability{ EAccelByteV2SessionJoinability::EMPTY };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	FString Name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	int64 MinPlayers{ TNumericLimits<int32>::Min() };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	int64 MaxPlayers{ TNumericLimits<int32>::Min() };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	int64 InactiveTimeout{ TNumericLimits<int32>::Min() };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	int64 InviteTimeout{ TNumericLimits<int32>::Min() };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	FString Deployment{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	FString ClientVersion{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	TArray<FString> RequestedRegions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Game Session Histories Configuration")
	bool Persistent{ false };
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsBaseGameSessionHistoryMember
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Base Game Session History User")
	FString ID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Base Game Session History User")
	EAccelByteV2SessionMemberStatus Status{ EAccelByteV2SessionMemberStatus::EMPTY };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Base Game Session History User")
	EAccelByteV2SessionMemberStatus StatusV2{ EAccelByteV2SessionMemberStatus::EMPTY };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Base Game Session History User")
	FDateTime UpdatedAt{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Base Game Session History User")
	FString PlatformID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Base Game Session History User")
	FString PlatformUserID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameSessionHistoryPayload
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FJsonObjectWrapper Attributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FString BackfillTicketID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FAccelByteModelsGameSessionHistoryConfiguration Configuration{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FString ConfigurationName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FDateTime CreatedAt{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FString CreatedBy{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FAccelByteModelsV2GameSessionDSInformation DSInformation{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FString GameMode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FString ID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	bool IsFull{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FString LeaderID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FString MatchPool{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	TArray<FAccelByteModelsBaseGameSessionHistoryMember> Members{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	int64 ProcessingTime{ TNumericLimits<int32>::Min() };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	TArray<FAccelByteModelsV2GameSessionTeam> Teams{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	TArray<FString> TicketIDs{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	FDateTime UpdatedAt{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	int64 Version{ TNumericLimits<int32>::Min() };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Payload")
	bool IsActive{ false };
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameSessionHistoryDetail
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Detail")
	FString EventName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Detail")
	FAccelByteModelsGameSessionHistoryPayload Payload{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Detail")
	FString RequesterClientID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Detail")
	FString RequesterUserID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | Session History Detail")
	FDateTime Timestamp{ 0 };

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameSessionUpdateDSInformationRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	FString ClientVersion{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	FString CreatedRegion{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	FString Deployment{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	FString Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	FString GameMode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	FString Ip{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	int32 Port{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	FString ServerId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	FString Source{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateDSInformationRequest")
	EAccelByteV2GameSessionDsStatus Status{};
};

#pragma endregion

#pragma region Game Session Misc Models

/*
 * Not meant to be exposed to blueprint, this model utilized by OSS Session interface as an optional param when start matchmaking
 * Member of FOnlineSessionSearchAccelByte.
 */
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameSessionExcludedSession
{
	enum ExclusionType
	{
		NONE,
		N_PAST_SESSION,
		EXPLICIT_LIST,
		ALL_MEMBER_CACHED_SESSION
	};
	
	static FAccelByteModelsGameSessionExcludedSession CreateNoExclusion();
	static FAccelByteModelsGameSessionExcludedSession CreateExclusionCount(uint32 ExcludedPastSessionCount);
	static FAccelByteModelsGameSessionExcludedSession CreateExclusionList(const TSet<FString>& ExcludedGameSessionIDs);
	static FAccelByteModelsGameSessionExcludedSession CreateExclusionEntireSessionMemberPastSession();

	const ExclusionType& CurrentType;
	const uint32& ExcludedPastSessionCount;
	TArray<FString> GetExcludedGameSessionIDs() { return _ExcludedGameSessionIDs.Array(); }

	FAccelByteModelsGameSessionExcludedSession() = delete;
	FAccelByteModelsGameSessionExcludedSession& operator=(const FAccelByteModelsGameSessionExcludedSession& Copy);

private:
	FAccelByteModelsGameSessionExcludedSession(ExclusionType Type);

	ExclusionType _CurrentType = ExclusionType::NONE;
	uint32 _ExcludedPastSessionCount = 0; //ExclusionType::N_PAST_SESSION
	TSet<FString> _ExcludedGameSessionIDs{}; //ExclusionType::EXPLICIT_LIST
};

#pragma endregion
