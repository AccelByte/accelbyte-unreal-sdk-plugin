// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
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
	ACTIVE,
	INACTIVE,
	INVITED,
	TIMEOUT
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionUser
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString ID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		EAccelByteV2SessionMemberStatus Status{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FDateTime UpdatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString PlatformID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionUser")
		FString PlatformUserID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionConfigurationTemplate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfigurationTemplate")
		FString Name{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfigurationTemplate")
		FString Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfigurationTemplate")
		EAccelByteV2SessionJoinability Joinability{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfigurationTemplate")
		int32 MinPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfigurationTemplate")
		int32 MaxPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfigurationTemplate")
		int64 InviteTimeout{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfigurationTemplate")
		int64 InactiveTimeout{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfigurationTemplate")
		FString Deployment{};
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
		TArray<FAccelByteModelsV2SessionUser> Members{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FJsonObjectWrapper Attributes{};
	// This is being moved to the configuration template. Leaving it here until that is implemented on the backend.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		EAccelByteV2SessionJoinability JoinType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString CreatedBy{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FDateTime CreatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FDateTime UpdatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FAccelByteModelsV2SessionConfigurationTemplate ConfigurationTemplate{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		int64 Version{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionTeam
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionTeam")
		TArray<FString> UserIDs{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionDSInformation
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSInformation")
		FAccelByteModelsServerInfo Server{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSInformation")
		int32 MinPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSInformation")
		TArray<FString> RequestedRegions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSInformation")
		FDateTime RequestedAt{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2CustomGameSettings
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | CustomGameSettings")
		FString MapName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | CustomGameSettings")
		int32 NumBot{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | CustomGameSettings")
		int32 CurrentPlayer{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | CustomGameSettings")
		FString Attributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | CustomGameSettings")
		EAccelByteV2SessionJoinability Joinability{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | CustomGameSettings")
		int32 MinPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | CustomGameSettings")
		int32 MaxPlayers{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSession : public FAccelByteModelsV2BaseSession
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FString GameMode{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		TArray<FAccelByteModelsV2GameSessionTeam> Teams{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FAccelByteModelsV2GameSessionDSInformation DSInformation{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FString BackfillTicketID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartySession : public FAccelByteModelsV2BaseSession
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FString LeaderID{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyInviteRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyInviteRequest")
		FString UserID{};
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
		EAccelByteV2SessionJoinability JoinType{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyUpdateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
	FJsonObjectWrapper Attributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
	EAccelByteV2SessionJoinability JoinType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
	int64 Version{};
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
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionQueryRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryRequest")
		FString Key{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryRequest")
		FString Value{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryRequest")
		FString LeaderID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryRequest")
		FString MemberID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryRequest")
		EAccelByteV2SessionMemberStatus MemberStatus{EAccelByteV2SessionMemberStatus::EMPTY};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyQueryRequest")
		EAccelByteV2SessionJoinability JoinType{EAccelByteV2SessionJoinability::EMPTY};
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

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyInvitedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyInvitedEvent")
		FString SenderID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyInvitedEvent")
		FString PartyID;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyMembersChangedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyMembersChangedEvent")
		FString PartyID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyMembersChangedEvent")
		FString JoinerID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyMembersChangedEvent")
		FString LeaderID;
};

