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
class ACCELBYTEUE4SDK_API MatchmakingV2 : FApiBase
{
public:
	MatchmakingV2(Credentials const& InCredentialsRef, Settings const& InSettingRef, FHttpRetryScheduler& InHttpRef);
	~MatchmakingV2();

	/**
	 * @brief Create a matchmaking ticket, this will start matchmaking process.
	 *
	 * @param MatchPool Name of the match pool we want to matchmake into.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 * @param Optionals optional variables we can set for matchmaking process.
	 */
	void CreateMatchTicket(
		const FString& MatchPool,
		const THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse>& OnSuccess,
		const FErrorHandler& OnError,
		const FAccelByteModelsV2MatchTicketOptionalParams& Optionals = {});

	/**
	 * @brief Get the match ticket details, will get the status of match ticket if it's already matched
	 * and it's corresponding session ID if already matched into a session.
	 *
	 * @param TicketId ID of the matchmaking ticket.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void GetMatchTicketDetails(
		const FString& TicketId,
		const THandler<FAccelByteModelsV2MatchmakingGetTicketDetailsResponse>& OnSuccess,
		const FErrorHandler& OnError);

	/**
	 * @brief Delete the match ticket. Will cancel the matchmaking process on success.
	 *
	 * @param TicketId ID of the matchmaking ticket.
	 * @param OnSuccess This will be called if the operation succeeded.
	 * @param OnError This will be called if the operation failed.
	 */
	void DeleteMatchTicket(
	const FString& TicketId,
	const FVoidHandler& OnSuccess,
	const FErrorHandler& OnError);
	
private:
	MatchmakingV2() = delete;
	MatchmakingV2(MatchmakingV2 const&) = delete;
	MatchmakingV2(MatchmakingV2&&) = delete;
	
};
}
}
