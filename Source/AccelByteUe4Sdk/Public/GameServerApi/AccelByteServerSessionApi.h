// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteSessionModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{

class ACCELBYTEUE4SDK_API ServerSession
{
public:
	ServerSession(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~ServerSession();

private:
	const ServerCredentials& CredentialsRef;
	const ServerSettings& SettingsRef;
	FHttpRetryScheduler& HttpRef;

public:
	/**
	 * @brief Get details for a game session by ID
	 *
	 * @param GameSessionID The ID of the session.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetGameSessionDetails(FString const& GameSessionID, THandler<FAccelByteModelsV2GameSession> const& OnSuccess, FErrorHandler const& OnError);

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
	 * @brief Update a member of this session's status with the one provided. Requires permission
	 * 'ADMIN:NAMESPACE:{namespace}:SESSION:GAME' to be set with the 'UPDATE' action.
	 *
	 * @param GameSessionID The ID of the session.
	 * @param MemberId ID of the member to update status for
	 * @param Status Status to set for this member
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void UpdateMemberStatus(FString const& GameSessionID, FString const& MemberID, const EAccelByteV2SessionMemberStatus& Status, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

private:
	ServerSession() = delete;
	ServerSession(ServerSession const&) = delete;
	ServerSession(ServerSession&&) = delete;
};
}
}
