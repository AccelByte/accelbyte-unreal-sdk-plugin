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
	ServerMatchmakingV2(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingRef, FHttpRetryScheduler& InHttpRef);
	~ServerMatchmakingV2();

	/**
	 * @brief Accept a proposal from the matchmaking V2 service to backfill a session.
	 * 
	 * @param BackfillTicketId ID of the backfill ticket that the proposal is for
	 * @param ProposalId ID of the proposal that you are accepting
	 * @param bStopBackfilling Whether or not to signal to the matchmaking service that you no longer want backfill
	 * @param OnSuccess Delegate fired when the accept request has gone through
	 * @param OnError Delegate fired when the accept request fails
	 */
	void AcceptBackfillProposal(const FString& BackfillTicketId
		, const FString& ProposalId
		, bool bStopBackfilling
		, const FVoidHandler& OnSuccess
		, const FErrorHandler& OnError);
	
	/**
	 * @brief Reject a proposal from the matchmaking V2 service to backfill a session.
	 *
	 * @param BackfillTicketId ID of the backfill ticket that the proposal is for
	 * @param ProposalId ID of the proposal that you are rejecting
	 * @param bStopBackfilling Whether or not to signal to the matchmaking service that you no longer want backfill
	 * @param OnSuccess Delegate fired when the reject request has gone through
	 * @param OnError Delegate fired when the reject request fails
	 */
	void RejectBackfillProposal(const FString& BackfillTicketId
		, const FString& ProposalId
		, bool bStopBackfilling
		, const FVoidHandler& OnSuccess
		, const FErrorHandler& OnError);

private:
	ServerMatchmakingV2() = delete;
	ServerMatchmakingV2(ServerMatchmakingV2 const&) = delete;
	ServerMatchmakingV2(ServerMatchmakingV2&&) = delete;
	
};
}
}
