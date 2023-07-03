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
	ServerSession(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~ServerSession();

	/**
	 * @brief Create a new game session.
	 *
	 * @param CreateRequest The game session create request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void CreateGameSession(FAccelByteModelsV2GameSessionCreateRequest const& CreateRequest
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get details for a game session by ID
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetGameSessionDetails(FString const& GameSessionID
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param UpdateRequest The game session update request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void UpdateGameSession(FString const& GameSessionID
		, FAccelByteModelsV2GameSessionUpdateRequest const& UpdateRequest
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void DeleteGameSession(FString const& GameSessionID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Send an invite to a game session by ID.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param UserID The ID of the user to invite.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void SendGameSessionInvite(FString const& GameSessionID
		, FString const& UserID
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
	 */
	void UpdateMemberStatus(FString const& GameSessionID
		, FString const& MemberID
		, const EAccelByteV2SessionMemberStatus& Status
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
	 */
	void QueryPartySessions(FAccelByteModelsV2QueryPartiesRequest const& RequestContent,  THandler<FAccelByteModelsV2PaginatedPartyQueryResult> const& OnSuccess, FErrorHandler const& OnError, int64 Offset = 0, int64 Limit = 20);

	/**
	 * @brief [DEPRECATED] Query for party information. Please use QueryPartySessions with paginated result.
	 *
	 * @param RequestContent The query parties request body.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Offset Pagination offset. Default 0.
	 * @param Limit Pagination limit. Default 20.
	 */
	void QueryPartySessions(FAccelByteModelsV2QueryPartiesRequest const& RequestContent,  THandler<FAccelByteModelsV2QueryPartiesResponse> const& OnSuccess, FErrorHandler const& OnError, int64 Offset = 0, int64 Limit = 20);

	/**
	 * @brief Query for party information.
	 *
	 * @param PartyID ID of the party to get details from
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetPartyDetails(FString const& PartyID, THandler<FAccelByteModelsV2PartySession> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Query for attributes for given users IDs.
	 *
	 * @param UserIds Array of IDs of the users that you want to query attributes for
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void BulkGetPlayerAttributes(TArray<FString> const& UserIds
		, THandler<TArray<FAccelByteModelsV2PlayerAttributes>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query for information the given user by their ID.
	 *
	 * @param UserId ID of the user that you want to query attributes for
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetPlayerAttributes(FString const& UserId
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
	 */
	void QueryGameSessions(FAccelByteModelsV2ServerQueryGameSessionsRequest RequestContent
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
	 */
	void PromoteGameSessionLeader(FString const& GameSessionID
		, FString const& NewLeaderID
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Generate a new game session code
	 *
	 * @param GameSessionID ID of the game session that a new code should be generated for.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GenerateNewGameSessionCode(FString const& GameSessionID
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Revoke a game session code
	 *
	 * @param GameSessionID ID of the game session that a code should be revoked for.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void RevokeGameSessionCode(FString const& GameSessionID
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

private:
	ServerSession() = delete;
	ServerSession(ServerSession const&) = delete;
	ServerSession(ServerSession&&) = delete;
};
}
}
