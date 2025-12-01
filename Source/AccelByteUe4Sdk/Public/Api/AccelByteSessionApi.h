// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#pragma once

#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteSessionModels.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace GameServerApi
{
class ServerSession;
}

namespace Api
{

class ServerSessionApi;

class ACCELBYTEUE4SDK_API Session 
	: public FApiBase
	, public TSharedFromThis<Session, ESPMode::ThreadSafe>
{
public:
	Session(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	Session(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~Session();

	/**
	 * @brief Create a new game session.
	 *
	 * @param CreateRequest The game session creation request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateGameSession(FAccelByteModelsV2GameSessionCreateRequest const& CreateRequest
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get details for a game session by ID
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetGameSessionDetails(FString const& GameSessionID
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query game sessions.
	 *
	 * @param QueryObject Query object containing the parameters to query on.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Offset Pagination offset.
	 * @param Limit Pagination limit.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryGameSessions(FAccelByteModelsV2GameSessionQuery const& QueryObject
		, THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Offset = 0
		, int32 Limit = 20);

	/**
	 * @brief Update a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param UpdateRequest The game session update request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateGameSession(FString const& GameSessionID
		, FAccelByteModelsV2GameSessionUpdateRequest const& UpdateRequest
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteGameSession(FString const& GameSessionID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send an invite to a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param UserID The ID of the user to invite.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SendGameSessionInvite(FString const& GameSessionID
		, FString const& UserID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send an invite to a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param UserID The ID of the user to invite.
	 * @param Platform The platform of the user
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	FAccelByteTaskWPtr SendGameSessionInvitePlatform(FString const& GameSessionID
		, FString const& UserID
		, EAccelByteV2SessionPlatform const& Platform
		, THandler<FAccelByteModelsV2SessionInvitePlatformResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Reject an invite to a game session by game session ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RejectGameSessionInvite(FString const& GameSessionID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Join a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr JoinGameSession(FString const& GameSessionID
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Join a game session by Code.
	 *
	 * @param Code The session's code.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr JoinGameSessionByCode(FString const& Code
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Generate a new game session code
	 *
	 * @param GameSessionID ID of the game session that a new code should be generated for.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GenerateNewGameSessionCode(FString const& GameSessionID
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Revoke a game session code
	 *
	 * @param GameSessionID ID of the game session that a code should be revoked for.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RevokeGameSessionCode(FString const& GameSessionID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Leave a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr LeaveGameSession(FString const& GameSessionID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a list of the logged in user's game sessions.
	 *
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Status Optional membership status to query for - either active or invited.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetMyGameSessions(THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteV2SessionMemberStatus Status = EAccelByteV2SessionMemberStatus::EMPTY);

	/**
	 * @brief Promote a new game session leader by user ID (only applicable for current game session leader)
	 *
	 * @param GameSessionID The ID of the game session to update.
	 * @param NewLeaderID The user ID of the new leader to promote.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PromoteGameSessionLeader(FString const& GameSessionID
		, FString const& NewLeaderID
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Cancel outgoing game session invitation.
	 *
	 * @param GameSessionID The ID of the game session.
	 * @param UserID The user ID of invitee.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CancelGameSessionInvitation(FString const& GameSessionID
		, FString const& UserID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Kick user from a game session.
	 *
	 * @param GameSessionID The ID of the game session which the user will be kicked from.
	 * @param UserID The ID of the user to kick 
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr KickUserFromGameSession(FString const& GameSessionID
		, FString const& UserID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Create a new party with the calling user as the sole member.
	 *
	 * @param CreateRequest The party creation request with attributes, join type, and members.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateParty(FAccelByteModelsV2PartyCreateRequest const& CreateRequest
		, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Retrieve party details for the given party ID.
	 *
	 * @param PartyID The ID of the party session to retrieve.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPartyDetails(FString const& PartyID
		, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update a party 
	 *
	 * @param PartyID The ID of the party session to update.
	 * @param UpdateRequest The request body to update the party with.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateParty(FString const& PartyID
		, FAccelByteModelsV2PartyUpdateRequest const& UpdateRequest
		, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Promote a new party leader by user ID
	 *
	 * @param PartyID The ID of the party session to update.
	 * @param NewLeaderID The user ID of the new leader to promote.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr PromotePartyLeader(FString const& PartyID
		, FString const& NewLeaderID
		, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Cancel outgoing party invitation.
	 *
	 * @param PartyID The ID of the party.
	 * @param UserID The user ID of invitee.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CancelPartyInvitation(FString const& PartyID
		, FString const& UserID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send a party invite to the given user.
	 *
	 * @param PartyID The ID of the party session for which the invite will be created.
	 * @param UserID The ID of the user to invite.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SendPartyInvite(FString const& PartyID
		, FString const& UserID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send a party invite to the given user on native platform.
	 *
	 * @param PartyID The ID of the party session for which the invite will be created.
	 * @param UserID The ID of the user to invite.
	 * @param Platform The platform of the user
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	FAccelByteTaskWPtr SendPartyInvitePlatform(FString const& PartyID
		, FString const& UserID
		, EAccelByteV2SessionPlatform const& Platform
		, THandler<FAccelByteModelsV2SessionInvitePlatformResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Reject an invite to a party 
	 *
	 * @param PartyID The ID of the party session containing the invite to reject.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RejectPartyInvite(FString const& PartyID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Join a party on behalf of the user.
	 *
	 * @param PartyID The ID of the party session which will be joined.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr JoinParty(FString const& PartyID
		, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Leave a party on behalf of the user.
	 *
	 * @param PartyID The ID of the party session which will be joined.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr LeaveParty(FString const& PartyID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Join a party using a party code.
	 *
	 * @param PartyCode Code used to try and join a party.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr JoinPartyByCode(FString const& PartyCode
		, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Generate a new party code for the party
	 *
	 * @param PartyID ID of the party that a new code should be generated for.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GenerateNewPartyCode(FString const& PartyID
		, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Revoke a party code associated with a party
	 *
	 * @param PartyID ID of the party that a code should be revoked for.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RevokePartyCode(FString const& PartyID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Kick another user from a party.
	 *
	 * @param PartyID The ID of the party session which the user will be kicked from.
	 * @param UserID The ID of the user to kick 
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr KickUserFromParty(FString const& PartyID
		, FString const& UserID
		, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a list of parties matching the given query.
	 *
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Status Optional membership status to query for - either active or invited.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetMyParties(THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess
		, FErrorHandler const& OnError
		, EAccelByteV2SessionMemberStatus Status = EAccelByteV2SessionMemberStatus::EMPTY);

	/**
	 * @brief Get stored attributes for the current player.
	 *
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPlayerAttributes(THandler<FAccelByteModelsV2PlayerAttributes> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get current user's party session storage.
	 *
	 * @param PartySessionID Current player's party ID.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPartySessionStorage(FString const& PartySessionID
		, THandler<FAccelByteModelsV2PartySessionStorage> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update stored attributes for the current player.
	 *
	 * @param AttributesRequest Request model for updating attributes for this player.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr StorePlayerAttributes(FAccelByteModelsV2StorePlayerAttributesRequest const& AttributesRequest
		, THandler<FAccelByteModelsV2PlayerAttributes> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Store a data by current player to its party reserved personal storage.
	 * WARNING: Overwrite operation! 
	 * This function will be used by AccelByte OSS internally and won't be exposed as a function.
	 *
	 * @param PartySessionID Current player's party ID.
	 * @param Data Player's data to be stored.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr StorePersonalDataToReservedPartySessionStorage(FString const& PartySessionID
		, FAccelByteModelsV2PartySessionStorageReservedData const& Data
		, THandler<FAccelByteModelsV2PartySessionStorageReservedData> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete stored attributes for the current player.
	 *
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeletePlayerAttributes(FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update leader's session storage data, can only be updated by current session leader.
	 * this will overwrite leader storage data, if updating also provide latest leader storage.
	 * to clear current leader storage data update with empty jsonObject.
	 *
	 * @param Data Data to update leader storage.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateLeaderStorage(FString const& SessionID
		, FJsonObjectWrapper const& Data
		, THandler<FJsonObjectWrapper> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update current user's session member storage data.
	 * this will overwrite this user's storage data, if updating also provide latest user's storage.
	 * to clear current user's storage data update with empty jsonObject.
	 *
	 * @param Data Data to update member storage.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateMemberStorage(FString const& SessionID
		, FJsonObjectWrapper const& Data
		, THandler<FJsonObjectWrapper> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update current user's party session member storage data.
	 * This allows users to update or insert their own party storage (non-immutable).
	 * This will overwrite this user's party storage data. Can store generic JSON.
	 *
	 * @param PartyID The ID of the party session.
	 * @param Data Generic JSON data to update party storage.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 *
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdatePartyStorage(FString const& PartyID
		, FJsonObjectWrapper const& Data
		, THandler<FJsonObjectWrapper> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query recently met users in a game session.
	 *
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Limit Number of recent players to request, maximum value is 200.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetRecentPlayers(THandler<FAccelByteModelsV2SessionRecentPlayers> const& OnSuccess
		, FErrorHandler const& OnError
		, const int32 Limit = 20);

	/**
	 * @brief Query user's recent player who were on the same team.
	 *
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Limit Number of recent players to request, maximum value is 200.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetRecentTeamPlayers(THandler<FAccelByteModelsV2SessionRecentPlayers> const& OnSuccess
		, FErrorHandler const& OnError
		, const int32 Limit = 20);

	/**
	 * @brief  Used by game client to get a session secret that is saved in session service.
	 * The secret will only return the secret value when the Enable Secret Validation option is set true (in the Admin Portal). 
	 *
	 * @param SessionID The ID of the game session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetSessionSecret(FString const& SessionID
		, THandler<FAccelByteModelsV2SessionJoinedSecret> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	Session() = delete;
	Session(Session const&) = delete;
	Session(Session&&) = delete;

	// The server session API class needs access to `SerializeAndRemoveEmptyValues`
	friend class GameServerApi::ServerSession;

	static void RemoveEmptyEnumValue(TSharedPtr<FJsonObject> & JsonObjectPtr
		, FString const& EnumFieldName);
	
	static void RemoveEmptyEnumValuesFromChildren(TSharedPtr<FJsonObject> & JsonObjectPtr
		, FString const& FieldName
		, FString const& EnumFieldName);

	template <typename DataStruct>
	static void SerializeAndRemoveEmptyValues(DataStruct const& Model
		, FString& OutputString
		, bool bIncludeTeams = true)
	{
		TSharedPtr<FJsonObject> JsonObjectPtr;
		SerializeAndRemoveEmptyValues(Model, JsonObjectPtr, bIncludeTeams);

		auto Writer = TJsonWriterFactory<>::Create(&OutputString);
		FJsonSerializer::Serialize(JsonObjectPtr.ToSharedRef(), Writer);
	}

	template <typename DataStruct>
	static void SerializeAndRemoveEmptyValues(DataStruct const& Model
		, TSharedPtr<FJsonObject>& OutJsonObjectPtr
		, bool bIncludeTeams = true)
	{
		auto JsonObjectPtr = FJsonObjectConverter::UStructToJsonObject(Model);

		RemoveEmptyEnumValue(JsonObjectPtr, TEXT("joinability"));
		RemoveEmptyEnumValue(JsonObjectPtr, TEXT("type"));
		RemoveEmptyEnumValuesFromChildren(JsonObjectPtr, TEXT("members"), TEXT("status"));

		TArray<FString> ExcludedFieldNamesForRemoval;
		if(bIncludeTeams)
		{
			ExcludedFieldNamesForRemoval.Add(TEXT("teams"));
		}

		FAccelByteUtilities::RemoveEmptyFieldsFromJson(JsonObjectPtr, FAccelByteUtilities::FieldRemovalFlagAll, ExcludedFieldNamesForRemoval);

		OutJsonObjectPtr = JsonObjectPtr;
	}
};

using SessionRef = TSharedRef<Session, ESPMode::ThreadSafe>;
using SessionPtr = TSharedPtr<Session, ESPMode::ThreadSafe>;
using SessionWPtr = TWeakPtr<Session, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
