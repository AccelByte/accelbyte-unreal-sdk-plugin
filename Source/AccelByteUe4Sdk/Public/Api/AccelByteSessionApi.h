// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteSessionModels.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{
namespace SessionTopic
{
// Temp solution while using freeform notifs
const FString UserInvited = TEXT("OnPartyInvited");
}

namespace SessionJoinType
{
const FString Open = TEXT("open");
const FString Closed = TEXT("closed");
const FString InviteOnly = TEXT("inviteOnly");
}

namespace SessionMemberStatus
{
const FString Invited  = TEXT("invited");
const FString Active   = TEXT("active");
const FString Inactive = TEXT("inactive");
const FString Timeout  = TEXT("timeout");
}

class ACCELBYTEUE4SDK_API Session
{
public:
	Session(Credentials const& CredentialsRef, Settings const& SettingsRef, FHttpRetryScheduler& HttpRef);
	~Session();

	/**
	 * @brief Create a new game session.
	 *
	 * @param CreateRequest The game session creation request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void CreateGameSession(FAccelByteModelsV2GameSessionCreateRequest const& CreateRequest, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get details for a game session by ID
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetGameSessionDetails(FString const& GameSessionID, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Query game sessions.
	 *
	 * @param QueryRequest The query object with fields to query on.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void QueryGameSessions(FAccelByteModelsV2SessionQueryRequest const& QueryRequest, THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess, FErrorHandler const& OnError, int32 const& Offset = 0, int32 const& Limit = 20);

	/**
	 * @brief Update a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param UpdateRequest The game session update request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void UpdateGameSession(FString const& GameSessionID, FAccelByteModelsV2GameSessionUpdateRequest const& UpdateRequest, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Delete a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void DeleteGameSession(FString const& GameSessionID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Join a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void JoinGameSession(FString const& GameSessionID, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Create a new party with the calling user as the sole member.
	 *
	 * @param CreateRequest The party creation request with attributes, join type, and members.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void CreateParty(FAccelByteModelsV2PartyCreateRequest const& CreateRequest, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	 * @brief Retrieve party details for the given party ID.
	 *
	 * @param PartyID The ID of the party session to retrieve.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetPartyDetails(FString const& PartyID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Update a party 
	 *
	 * @param PartyID The ID of the party session to update.
	 * @param UpdateRequest The request body to update the party with.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void UpdateParty(FString const& PartyID, FAccelByteModelsV2PartyUpdateRequest const& UpdateRequest, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	 * @brief Send a party invite to the given user.
	 *
	 * @param PartyID The ID of the party session for which the invite will be created.
	 * @param UserID The ID of the user to invite.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void SendPartyInvite(FString const& PartyID, FString const& UserID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Reject an invite to a party 
	 *
	 * @param PartyID The ID of the party session containing the invite to reject.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void RejectPartyInvite(FString const& PartyID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	 * @brief Join a party on behalf of the user.
	 *
	 * @param PartyID The ID of the party session which will be joined.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void JoinParty(FString const& PartyID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Leave a party on behalf of the user.
	 *
	 * @param PartyID The ID of the party session which will be joined.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void LeaveParty(FString const& PartyID, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Kick another user from a party.
	 *
	 * @param PartyID The ID of the party session which the user will be kicked from.
	 * @param UserID The ID of the user to kick 
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void KickUserFromParty(FString const& PartyID, FString const& UserID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get a list of parties matching the given query.
	 *
	 * @param Query The object containing fields to query on - empty fields are omitted.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void QueryParties(FAccelByteModelsV2PartyQueryRequest const& Query, THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess, FErrorHandler const& OnError, int32 const& Offset = 0, int32 const& Limit = 20);

	
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& Credentials;
	Settings const& Settings;

	Session() = delete;
	Session(Session const&) = delete;
	Session(Session&&) = delete;
};
}
}
