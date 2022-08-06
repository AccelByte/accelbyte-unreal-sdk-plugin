// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "AccelByteDSMModels.h"
#include "AccelByteGeneralModels.h"
#include "Core/AccelByteReport.h"
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
	KICKED
};

UENUM(BlueprintType)
enum class EAccelByteV2GameSessionDsStatus : uint8
{
	INITIALIZED = 0,
	REQUESTED,
	READY,
	CREATING,
	BUSY
};

UENUM(BlueprintType)
enum class EAccelByteV2SessionConfigurationServerType : uint8
{
	NONE = 0,
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
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2SessionConfiguration
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		EAccelByteV2SessionConfigurationServerType Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionConfiguration")
		EAccelByteV2SessionJoinability Joinability{};
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
		FDateTime RequestedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSInformation")
		EAccelByteV2GameSessionDsStatus Status{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2DSStatusChangedNotif
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSStatusChangedNotif")
	FAccelByteModelsServerInfo Game_server{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSStatusChangedNotif")
	FString SessionID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionDSStatusChangedNotif")
	FString Error{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSession
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FString CreatedBy{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FDateTime CreatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FDateTime UpdatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		FAccelByteModelsV2SessionConfiguration Configuration{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | BaseSession")
		int64 Version{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		TArray<FAccelByteModelsV2GameSessionTeam> Teams{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FAccelByteModelsV2GameSessionDSInformation DSInformation{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FString BackfillTicketID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSession")
		FString MatchPool{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionCreateRequest
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FJsonObjectWrapper Attributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString BackfillTicketID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString ConfigurationName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		EAccelByteV2SessionConfigurationServerType Type{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		EAccelByteV2SessionJoinability Joinability{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString MatchPool{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		TArray<FAccelByteModelsV2GameSessionTeam> Teams{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString ClientVersion{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		FString Deployment{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		TArray<FString> RequestedRegions{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 MaxPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 MinPlayers{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 InactiveTimeout{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionCreateRequest")
		int64 InviteTimeout{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionUpdateRequest : public FAccelByteModelsV2GameSessionCreateRequest
{
	GENERATED_BODY();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionUpdateRequest")
		int64 Version{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartySession
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FString LeaderID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FString ID{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		TArray<FAccelByteModelsV2SessionUser> Members{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FJsonObjectWrapper Attributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FString CreatedBy{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FDateTime CreatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FDateTime UpdatedAt{0};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		FAccelByteModelsV2SessionConfiguration Configuration{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartySession")
		int64 Version{};
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
		EAccelByteV2SessionJoinability Joinability{};
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
		EAccelByteV2SessionConfigurationServerType Type{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2PartyUpdateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		FJsonObjectWrapper Attributes{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		EAccelByteV2SessionJoinability Joinability{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		int64 Version{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | PartyUpdateRequest")
		EAccelByteV2SessionConfigurationServerType Type{};
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

	FAccelByteModelsV2GameSessionQuery()
	{
		JsonWrapper.JsonObject = MakeShared<FJsonObject>();
	}

// Macro to reduce code duplication for setting query fields in the JSON wrapper
#define SET_SESSION_QUERY_FIELD(JsonType, Value)                                                                       \
	FString ComparisonFieldName;                                                                                       \
	const TSharedPtr<FJsonObject> ComparisonField = InitializeComparisonField(Comparison, ComparisonFieldName);        \
	ComparisonField->Set##JsonType##Field(AttributeName, Value);                                                       \
	AddToQuery(ComparisonFieldName, ComparisonField);                                                                  \

	void AddParam(
		const FString& AttributeName,
		const EAccelByteV2SessionQueryComparisonOp Comparison,
		const FString& AttributeValue) const
	{
		if(Comparison != EAccelByteV2SessionQueryComparisonOp::EQUAL &&
			Comparison != EAccelByteV2SessionQueryComparisonOp::NOT_EQUAL)
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Invalid session query operator/type combination!"));
			return;
		}

		SET_SESSION_QUERY_FIELD(String, AttributeValue);
	}

	void AddParam(
		const FString& AttributeName,
		const EAccelByteV2SessionQueryComparisonOp Comparison,
		const double AttributeValue) const
	{
		if(Comparison != EAccelByteV2SessionQueryComparisonOp::EQUAL &&
			Comparison != EAccelByteV2SessionQueryComparisonOp::NOT_EQUAL &&
			Comparison != EAccelByteV2SessionQueryComparisonOp::GREATER_THAN &&
			Comparison != EAccelByteV2SessionQueryComparisonOp::GREATER_THAN_EQUAL &&
			Comparison != EAccelByteV2SessionQueryComparisonOp::LESS_THAN &&
			Comparison != EAccelByteV2SessionQueryComparisonOp::LESS_THAN_EQUAL)
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Invalid session query operator/type combination!"));
			return;
		}

		SET_SESSION_QUERY_FIELD(Number, AttributeValue);
	}

	void AddParam(
		const FString& AttributeName,
		const EAccelByteV2SessionQueryComparisonOp Comparison,
		const TArray<FString>& AttributeValue) const
	{
		if(Comparison != EAccelByteV2SessionQueryComparisonOp::CONTAINS &&
			Comparison != EAccelByteV2SessionQueryComparisonOp::NOT_CONTAINS)
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Invalid session query operator/type combination!"));
			return;
		}

		TArray<TSharedPtr<FJsonValue>> JsonArray;
		for(const FString& Item : AttributeValue)
		{
			JsonArray.Add(MakeShared<FJsonValueString>(Item));
		}

		SET_SESSION_QUERY_FIELD(Array, JsonArray);
	}

	void AddParam(
		const FString& AttributeName,
		const EAccelByteV2SessionQueryComparisonOp Comparison,
		const TArray<double>& AttributeValue) const
	{
		if(Comparison != EAccelByteV2SessionQueryComparisonOp::CONTAINS &&
			Comparison != EAccelByteV2SessionQueryComparisonOp::NOT_CONTAINS)
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Invalid session query operator/type combination!"));
			return;
		}

		TArray<TSharedPtr<FJsonValue>> JsonArray;
		for(const double Item : AttributeValue)
		{
			JsonArray.Add(MakeShared<FJsonValueNumber>(Item));
		}

		SET_SESSION_QUERY_FIELD(Array, JsonArray);
	}

#undef SET_SESSION_QUERY_FIELD

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | SessionQuery")
		FJsonObjectWrapper JsonWrapper;

private:
	TSharedPtr<FJsonObject> InitializeComparisonField(const EAccelByteV2SessionQueryComparisonOp Op, FString& OutFieldName) const
	{
		if(!JsonWrapper.JsonObject->HasTypedField<EJson::Object>(TEXT("query")))
		{
			JsonWrapper.JsonObject->SetObjectField(TEXT("query"), MakeShared<FJsonObject>());
		}

		const TSharedPtr<FJsonObject> QueryObject = JsonWrapper.JsonObject->GetObjectField(TEXT("query"));

		switch(Op)
		{
		case EAccelByteV2SessionQueryComparisonOp::EQUAL:
			OutFieldName = TEXT("equals");
			break;
		case EAccelByteV2SessionQueryComparisonOp::NOT_EQUAL:
			OutFieldName = TEXT("notEquals");
			break;
		case EAccelByteV2SessionQueryComparisonOp::GREATER_THAN:
			OutFieldName = TEXT("greaterThan");
			break;
		case EAccelByteV2SessionQueryComparisonOp::GREATER_THAN_EQUAL:
			OutFieldName = TEXT("greaterThanEquals");
			break;
		case EAccelByteV2SessionQueryComparisonOp::LESS_THAN:
			OutFieldName = TEXT("lessThan");
			break;
		case EAccelByteV2SessionQueryComparisonOp::LESS_THAN_EQUAL:
			OutFieldName = TEXT("lessThanEquals");
			break;
		case EAccelByteV2SessionQueryComparisonOp::CONTAINS:
			OutFieldName = TEXT("in");
			break;
		case EAccelByteV2SessionQueryComparisonOp::NOT_CONTAINS:
			OutFieldName = TEXT("notIn");
			break;
		default: break;
		}

		if(!QueryObject->HasTypedField<EJson::Object>(OutFieldName))
		{
			QueryObject->SetObjectField(OutFieldName, MakeShared<FJsonObject>());
		}

		return QueryObject->GetObjectField(OutFieldName);
	}

	void AddToQuery(const FString& ComparisonFieldName, const TSharedPtr<FJsonObject>& ComparisonField) const
	{
		// #NOTE: The AddToQuery method should only ever be called after InitializeComparisonField has been called, but
		// putting this here just in case something changes in the future
		if(!JsonWrapper.JsonObject->HasTypedField<EJson::Object>(TEXT("query")))
		{
			JsonWrapper.JsonObject->SetObjectField(TEXT("query"), MakeShared<FJsonObject>());
		}

		const TSharedPtr<FJsonObject> QueryObject =	JsonWrapper.JsonObject->GetObjectField(TEXT("query"));
		QueryObject->SetObjectField(ComparisonFieldName, ComparisonField);

		JsonWrapper.JsonObject->SetObjectField(TEXT("query"), QueryObject);
	}
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
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsV2GameSessionUserKickedEvent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Session | Models | GameSessionKickedEvent")
		FString SessionID{};
};
