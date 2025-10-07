// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#if 1 // MMv1 Deprecation
#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteDefines.h"
#include "Core/AccelByteServerApiBase.h"
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
class ACCELBYTEUE4SDK_API ServerMatchmaking : public FServerApiBase
{
public:
	ServerMatchmaking(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerMatchmaking();

	/**
	 * @brief Enqueue Game Server to Joinable Session Queue. This will make this server joinable by other parties while already in a session.
	 *
	 * @param MatchmakingResult The session's data (get this data from QuerySessionStatus)
	 * @param OnSuccess This will be called when the operation succeeded. Will return model FAccelByteModelsMatchmakingResult.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr EnqueueJoinableSession(FAccelByteModelsMatchmakingResult const& MatchmakingResult
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Dequeue Game Server from Joinable Session Queue. This will make this server not joinable to other parties while already in a session.
	 *
	 * @note If you're implementing the player count in the Server, call this once you treat the server as full already and don't want it to be used as matchmaking target for DSM.
	 *
	 * @param MatchId The match/session ID.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DequeueJoinableSession(FString const& MatchId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Add a user to session data.
	 *
	 * @param ChannelName The Channel's Name.
	 * @param MatchId match/session ID.
	 * @param UserId user ID to be added.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param PartyId optional, the party ID of the user to be added. if not listed user will be added to a new party.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr AddUserToSession(FString const& ChannelName
		, FString const& MatchId
		, FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& PartyId = TEXT(""));

	/**
	 * @brief Remove a user from session data.
	 *
	 * @param ChannelName The Channel's Name.
	 * @param MatchId match/session ID.
	 * @param UserId user ID to be removed.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Body optional, the session's data
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RemoveUserFromSession(FString const& ChannelName
		, FString const& MatchId
		, FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FAccelByteModelsMatchmakingResult const& Body = FAccelByteModelsMatchmakingResult());

	/**
	 * @brief Get the session's data status
	 *
	 * @param MatchId the match/session ID
	 * @param OnSuccess This will be called when the operation succeeded. Will return model FAccelByteModelsMatchmakingResult.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QuerySessionStatus(FString const& MatchId
		, THandler<FAccelByteModelsMatchmakingResult> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Rebalance matchmaking based on MMR
	 *
	 * @param MatchId The match/session ID.
	 * @param OnSuccess This will be called when the operation succeeded. Will return model FAccelByteModelsMatchmakingResult.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RebalanceMatchmakingBasedOnMMR(FString const& MatchId
		, THandler<FAccelByteModelsMatchmakingResult> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Activate session data polling in a certain time interval.
	 *
	 * @param MatchId The match/session ID.
	 * @param OnSuccess This will be called when the operation succeeded. Will return model FAccelByteModelsMatchmakingResult.
	 * @param OnError This will be called when the operation failed.
	 * @param IntervalSec the interval of every session data get call in second.
	 */
	void ActivateSessionStatusPolling(FString const& MatchId
		, THandler<FAccelByteModelsMatchmakingResult> const& OnSuccess
		, FErrorHandler const& OnError
		, uint32 IntervalSec = 5);

	/**
	 * @brief Deactivate session data polling.
	 */
	void DeactivateStatusPolling();
	
private:
	ServerMatchmaking() = delete;
	ServerMatchmaking(ServerMatchmaking const&) = delete;
	ServerMatchmaking(ServerMatchmaking&&) = delete;

	bool StatusPollingTick(float DeltaTime);
	
	FString StatusPollingMatchId;
	FHttpRequestCompleteDelegate OnStatusPollingResponse;
	THandler<FAccelByteModelsMatchmakingResult> OnStatusPollingResponseSuccess;
	FErrorHandler OnStatusPollingResponseError;
	FTickerDelegate StatusPollingDelegate;
	FDelegateHandleAlias StatusPollingDelegateHandle;
	bool bStatusPollingActive;
};

} // Namespace Api
} // Namespace AccelByte
#endif