// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

// includes
#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteDefines.h"
#include "Models/AccelByteMatchmakingModels.h"

namespace AccelByte
{
namespace Api
{

class ACCELBYTEUE4SDK_API MatchmakingV2 
	: public FApiBase
	, public TSharedFromThis<MatchmakingV2, ESPMode::ThreadSafe>
{
public:
	MatchmakingV2(Credentials const& InCredentialsRef
		, Settings const& InSettingRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	MatchmakingV2(Credentials const& InCredentialsRef
		, Settings const& InSettingRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~MatchmakingV2();

	/**
	 * @brief [DEPRECATED] Create a matchmaking ticket, this will start matchmaking process.
	 *
	 * @param MatchPool Name of the match pool we want to matchmake into.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Optionals optional variables we can set for matchmaking process.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateMatchTicket(FString const& MatchPool
		, THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse> const& OnSuccess
		, FErrorHandler const& OnError
		, FAccelByteModelsV2MatchTicketOptionalParams const& Optionals = {});

	/**
	 * @brief Create a matchmaking ticket, this will start matchmaking process.
	 *
	 * @param MatchPool Name of the match pool we want to matchmake into.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Optionals optional variables we can set for matchmaking process.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateMatchTicket(FString const& MatchPool
		, THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse> const& OnSuccess
		, FCreateMatchmakingTicketErrorHandler const& OnError
		, FAccelByteModelsV2MatchTicketOptionalParams const& Optionals = {});

	/**
	 * @brief Get the match ticket details, will get the status of match ticket if it's already matched
	 * and it's corresponding session ID if already matched into a session.
	 *
	 * @param TicketId ID of the matchmaking ticket.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetMatchTicketDetails(FString const& TicketId
		, THandler<FAccelByteModelsV2MatchmakingGetTicketDetailsResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete the match ticket. Will cancel the matchmaking process on success.
	 *
	 * @param TicketId ID of the matchmaking ticket.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteMatchTicket(FString const& TicketId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get matchmaking matchpool's metrics.
	 *
	 * @param MatchPool Name of the matchpool.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetMatchmakingMetrics(FString const& MatchPool
		, THandler<FAccelByteModelsV2MatchmakingMetrics> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get my match tickets
	 * already matched tickets will be listed for a while after it's matched.
	 * 
	 * @param OnSuccess Will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param MatchPool Name of the match pool we want to query, leave empty will query all match pool.
	 * @param Limit Pagination item limit, default is 20.
	 * @param Offset Pagination item offset, default is 0.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetMyMatchTickets(THandler<FAccelByteModelsV2MatchmakingTicketStatuses> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& MatchPool = TEXT("")
		, int32 Limit = 20
		, int32 Offset = 0);
	
private:
	MatchmakingV2() = delete;
	MatchmakingV2(MatchmakingV2 const&) = delete;
	MatchmakingV2(MatchmakingV2&&) = delete;
	
};

using MatchmakingV2Ref = TSharedRef<MatchmakingV2, ESPMode::ThreadSafe>;
using MatchmakingV2Ptr = TSharedPtr<MatchmakingV2, ESPMode::ThreadSafe>;
using MatchmakingV2WPtr = TWeakPtr<MatchmakingV2, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
