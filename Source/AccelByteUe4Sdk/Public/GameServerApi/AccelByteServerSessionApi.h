// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteSessionModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{

class ACCELBYTEUE4SDK_API ServerSession : public FServerApiBase
{
public:
	ServerSession(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerSession();

	/**
	 * @brief Create a new game session.
	 *
	 * @param CreateRequest The game session create request body.
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
	 * @brief Update a member of this session's status with the one provided. Requires permission
	 * 'ADMIN:NAMESPACE:{namespace}:SESSION:GAME' to be set with the 'UPDATE' action.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param MemberID ID of the member to update status for
	 * @param Status Status to set for this member
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateMemberStatus(FString const& GameSessionID
		, FString const& MemberID
		, EAccelByteV2SessionMemberStatus Status
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query for party information.
	 *
	 * @param RequestContent The query parties request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Offset Pagination offset. Default 0.
	 * @param Limit Pagination limit. Default 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryPartySessions(FAccelByteModelsV2QueryPartiesRequest const& RequestContent
		, THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess
		, FErrorHandler const& OnError
		, int64 Offset = 0
		, int64 Limit = 20);

	/**
	 * @brief [DEPRECATED] Query for party information. Please use QueryPartySessions with paginated result.
	 *
	 * @param RequestContent The query parties request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Offset Pagination offset. Default 0.
	 * @param Limit Pagination limit. Default 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryPartySessions(FAccelByteModelsV2QueryPartiesRequest const& RequestContent
		, THandler<FAccelByteModelsV2QueryPartiesResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, int64 Offset = 0
		, int64 Limit = 20);

	/**
	 * @brief Query for party information.
	 *
	 * @param PartyID ID of the party to get details from
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPartyDetails(FString const& PartyID
		, THandler<FAccelByteModelsV2PartySession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query for attributes for given users IDs.
	 *
	 * @param UserIds Array of IDs of the users that you want to query attributes for
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetPlayerAttributes(TArray<FString> const& UserIds
		, THandler<TArray<FAccelByteModelsV2PlayerAttributes>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query for information the given user by their ID.
	 *
	 * @param UserId ID of the user that you want to query attributes for
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPlayerAttributes(FString const& UserId
		, THandler<FAccelByteModelsV2PlayerAttributes> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query for game sessions for dedicated server
	 *
	 * @param RequestContent The query game sessions request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Offset Pagination offset. Default 0.
	 * @param Limit Pagination limit. Default 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryGameSessions(FAccelByteModelsV2ServerQueryGameSessionsRequest const& RequestContent
		, THandler<FAccelByteModelsV2PaginatedGameSessionQueryResult> const& OnSuccess
		, FErrorHandler const& OnError
		, int64 Offset = 0, int64 Limit = 20);

	/**
	 * @brief Promote a new game session leader by user ID
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
	 * @brief Inform session service that current DS session is ready receiving client travel.
	 * This needed if session template used for this game session enable the dsManualSetReady flag.
	 *
	 * @param GameSessionID ID of the game session that claimed the DS.
	 * @param bDSSessionReady flag indicating the DS session ready or not.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SendDSSessionReady(FString const& GameSessionID
		, bool bDSSessionReady
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query recently met users in a game session.
	 *
	 * @param UserId Target recent player User Id
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Limit Number of recent players to request, maximum value is 200.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetRecentPlayers(FString const& UserId
		, THandler<FAccelByteModelsV2SessionRecentPlayers> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20);

	/**
	 * @brief Query user's recent player who were on the same team.
	 *
	 * @param UserId Target recent player User Id
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Limit Number of recent players to request, maximum value is 200.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetRecentTeamPlayers(FString const& UserId
		, THandler<FAccelByteModelsV2SessionRecentPlayers> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Limit = 20);

	/**
	 * @brief Update the DS information associated with the specified session.
	 *
	 * @param GameSessionId ID of the session to update
	 * @param DSInformation Structure describing the DS information to update with
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateDSInformation(FString const& GameSessionId
		, FAccelByteModelsGameSessionUpdateDSInformationRequest const& DSInformation
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

private:
	ServerSession() = delete;
	ServerSession(ServerSession const&) = delete;
	ServerSession(ServerSession&&) = delete;
};

} // Namespace GameServerApi
} // Namespace AccelByte
