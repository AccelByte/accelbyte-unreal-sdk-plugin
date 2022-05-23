// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteDefines.h"
#include "Models/AccelByteMatchmakingModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
	* @brief Matchmaking API to manage user's in-game profiles's matchmaking.
	*/
class ACCELBYTEUE4SDK_API ServerMatchmaking
{
public:
	ServerMatchmaking(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~ServerMatchmaking();
private:
	ServerCredentials const& CredentialsRef;
	ServerSettings const& SettingsRef;
	FHttpRetryScheduler& HttpRef;

public:

	/*
	 * @brief Enqueue Game Server to Joinable Session Queue. This will make this server joinable by other parties while already in a session.
	 *
	 * @param MatchmakingResult The session's data (get this data from QuerySessionStatus)
	 * @param OnSuccess This will be called when the operation succeeded. Will return model FAccelByteModelsMatchmakingResult.
	 * @param OnError This will be called when the operation failed.
	 */
	void EnqueueJoinableSession(const FAccelByteModelsMatchmakingResult& MatchmakingResult, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);


	/*
	 * @brief Dequeue Game Server from Joinable Session Queue. This will make this server not joinable to other parties while already in a session.
	 *
	 * @note If you're implementing the player count in the Server, call this once you treat the server as full already and don't want it to be used as matchmaking target for DSM.
	 *
	 * @param MatchId The match/session ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void DequeueJoinableSession(const FString& MatchId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/*
	 * @brief Add a user to session data.
	 *
	 * @param ChannelName The Channel's Name.
	 * @param MatchId match/session ID.
	 * @param UserId user ID to be added.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param PartyId optional, the party ID of the user to be added. if not listed user will be added to a new party.
	 */
	void AddUserToSession(const FString& ChannelName, const FString& MatchId, const FString& UserId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError, const FString& PartyId = TEXT(""));

	/*
	 * @brief Remove a user from session data.
	 *
	 * @param ChannelName The Channel's Name.
	 * @param MatchId match/session ID.
	 * @param UserId user ID to be removed.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Body optional, the session's data
	 */
	void RemoveUserFromSession(const FString& ChannelName, const FString& MatchId, const FString& UserId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError, const FAccelByteModelsMatchmakingResult& Body = FAccelByteModelsMatchmakingResult());

	/*
	 * @brief Get the session's data status
	 *
	 * @param MatchId the match/session ID
	 * @param OnSuccess This will be called when the operation succeeded. Will return model FAccelByteModelsMatchmakingResult.
	 * @param OnError This will be called when the operation failed.
	 */
	void QuerySessionStatus(const FString MatchId, const THandler<FAccelByteModelsMatchmakingResult>& OnSuccess, const FErrorHandler& OnError);

	/*
	 * @brief Activate session data polling in a certain time interval.
	 *
	 * @param MatchId The match/session ID.
	 * @param OnSuccess This will be called when the operation succeeded. Will return model FAccelByteModelsMatchmakingResult.
	 * @param OnError This will be called when the operation failed.
	 * @param IntervalSec the interval of every session data get call in second.
	 */
	void ActivateSessionStatusPolling(const FString& MatchId, const THandler<FAccelByteModelsMatchmakingResult>& OnSuccess, const FErrorHandler& OnError, uint32 IntervalSec = 5);

	/*
	 * @brief Deactivate session data polling.
	 */
	void DeactivateStatusPolling();
private:
	ServerMatchmaking() = delete;
	ServerMatchmaking(ServerMatchmaking const&) = delete;
	ServerMatchmaking(ServerMatchmaking&&) = delete;

	bool StatusPollingTick(float DeltaTime);
	bool bStatusPollingActive;
	FString StatusPollingMatchId;
	FHttpRequestCompleteDelegate OnStatusPollingResponse;
	THandler<FAccelByteModelsMatchmakingResult> OnStatusPollingResponseSuccess;
	FErrorHandler OnStatusPollingResponseError;
	FTickerDelegate StatusPollingDelegate;
	FDelegateHandleAlias StatusPollingDelegateHandle;
};

} // Namespace Api
} // Namespace AccelByte
