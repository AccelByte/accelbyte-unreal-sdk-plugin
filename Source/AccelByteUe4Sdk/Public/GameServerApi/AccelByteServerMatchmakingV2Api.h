// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

// includes
#include "CoreMinimal.h"
#include "Core/AccelByteServerApiBase.h"
#include "Models/AccelByteMatchmakingModels.h"

namespace AccelByte
{

class ServerCredentials;
class ServerSettings;
class FHttpRetryScheduler;

namespace GameServerApi
{

class ACCELBYTEUE4SDK_API ServerMatchmakingV2 : public FServerApiBase
{
public:
	ServerMatchmakingV2(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerMatchmakingV2();

	/**
	 * @brief [DEPRECATED] Accept a proposal from the matchmaking V2 service to backfill a session.
	 * 
	 * @param BackfillTicketId ID of the backfill ticket that the proposal is for
	 * @param ProposalId ID of the proposal that you are accepting
	 * @param bStopBackfilling Whether or not to signal to the matchmaking service that you no longer want backfill
	 * @param OnSuccess Delegate fired when the accept request has gone through
	 * @param OnError Delegate fired when the accept request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr AcceptBackfillProposal(FString const& BackfillTicketId
		, FString const& ProposalId
		, bool bStopBackfilling
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Accept a proposal from the matchmaking V2 service to backfill a session.
	 *
	 * @param BackfillTicketId ID of the backfill ticket that the proposal is for
	 * @param ProposalId ID of the proposal that you are accepting
	 * @param bStopBackfilling Whether or not to signal to the matchmaking service that you no longer want backfill
	 * @param OnSuccess Delegate fired when the accept request has gone through
	 * @param OnError Delegate fired when the accept request fails
	 * @param OptionalParameter An extra field to modify the acceptance behavior.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr AcceptBackfillProposal(FString const& BackfillTicketId
		, FString const& ProposalId
		, bool bStopBackfilling
		, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
		, FErrorHandler const& OnError
		, FAccelByteModelsV2MatchmakingBackfillAcceptanceOptionalParam const& OptionalParameter = {}
	);
	
	/**
	 * @brief Reject a proposal from the matchmaking V2 service to backfill a session.
	 *
	 * @param BackfillTicketId ID of the backfill ticket that the proposal is for
	 * @param ProposalId ID of the proposal that you are rejecting
	 * @param bStopBackfilling Whether or not to signal to the matchmaking service that you no longer want backfill
	 * @param OnSuccess Delegate fired when the reject request has gone through
	 * @param OnError Delegate fired when the reject request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RejectBackfillProposal(FString const& BackfillTicketId
		, FString const& ProposalId
		, bool bStopBackfilling
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Create a ticket to enable backfill on this session.
	 *
	 * @param MatchPool Pool that the backfill ticket should be queued in
	 * @param SessionId ID of the session that backfill will apply to
	 * @param OnSuccess Delegate fired when the reject request has gone through
	 * @param OnError Delegate fired when the reject request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateBackfillTicket(FString const& MatchPool
		, FString const& SessionId
		, THandler<FAccelByteModelsV2MatchmakingCreateBackfillTicketResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Remove a backfill ticket from the queue, effectively disabling backfill.
	 *
	 * @param BackfillTicketId ID of the backfill ticket that we wish to remove
	 * @param OnSuccess Delegate fired when the reject request has gone through
	 * @param OnError Delegate fired when the reject request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteBackfillTicket(FString const& BackfillTicketId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

private:
	ServerMatchmakingV2() = delete;
	ServerMatchmakingV2(ServerMatchmakingV2 const&) = delete;
	ServerMatchmakingV2(ServerMatchmakingV2&&) = delete;
	
};
}
}
