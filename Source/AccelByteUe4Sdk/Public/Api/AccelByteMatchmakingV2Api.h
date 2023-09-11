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

class ACCELBYTEUE4SDK_API MatchmakingV2 : public FApiBase
{
public:
	MatchmakingV2(Credentials const& InCredentialsRef, Settings const& InSettingRef, FHttpRetryScheduler& InHttpRef);
	~MatchmakingV2();

	/**
	 * @brief [DEPRECATED] Create a matchmaking ticket, this will start matchmaking process.
	 *
	 * @param MatchPool Name of the match pool we want to matchmake into.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Optionals optional variables we can set for matchmaking process.
	 */
	void CreateMatchTicket(const FString& MatchPool
		, const THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse>& OnSuccess
		, const FErrorHandler& OnError
		, const FAccelByteModelsV2MatchTicketOptionalParams& Optionals = {});

	/**
	 * @brief Create a matchmaking ticket, this will start matchmaking process.
	 *
	 * @param MatchPool Name of the match pool we want to matchmake into.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Optionals optional variables we can set for matchmaking process.
	 */
	void CreateMatchTicket(const FString& MatchPool
		, const THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse>& OnSuccess
		, const FCreateMatchmakingTicketErrorHandler& OnError
		, const FAccelByteModelsV2MatchTicketOptionalParams& Optionals = {});

	/**
	 * @brief Get the match ticket details, will get the status of match ticket if it's already matched
	 * and it's corresponding session ID if already matched into a session.
	 *
	 * @param TicketId ID of the matchmaking ticket.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetMatchTicketDetails(const FString& TicketId
		, const THandler<FAccelByteModelsV2MatchmakingGetTicketDetailsResponse>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Delete the match ticket. Will cancel the matchmaking process on success.
	 *
	 * @param TicketId ID of the matchmaking ticket.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void DeleteMatchTicket(const FString& TicketId
		, const FVoidHandler& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Get matchmaking matchpool's metrics.
	 *
	 * @param MatchPool Name of the matchpool.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetMatchmakingMetrics(const FString& MatchPool
		, const THandler<FAccelByteModelsV2MatchmakingMetrics>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Get my match tickets
	 * already matched tickets will be listed for a while after it's matched.
	 * 
	 * @param OnSuccess Will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param MatchPool Name of the match pool we want to query, leave empty will query all match pool.
	 * @param Limit Pagination item limit, default is 20.
	 * @param Offset Pagination item offset, default is 0.
	 */
	void GetMyMatchTickets(const THandler<FAccelByteModelsV2MatchmakingTicketStatuses>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& MatchPool = TEXT("")
		, const int32& Limit = 20
		, const int32& Offset = 0);
	
private:
	MatchmakingV2() = delete;
	MatchmakingV2(MatchmakingV2 const&) = delete;
	MatchmakingV2(MatchmakingV2&&) = delete;
	
};

}
}
