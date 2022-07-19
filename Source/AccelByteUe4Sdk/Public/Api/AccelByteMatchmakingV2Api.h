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

	void CreateMatchTicket(
		const FString& MatchPool,
		const THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse>& OnSuccess,
		const FErrorHandler& OnError,
		const FAccelByteModelsV2MatchTicketOptionalParams& Optionals = {});
	
	void GetMatchTicketDetails(
		const FString& TicketId,
		const THandler<FAccelByteModelsV2MatchmakingGetTicketDetailsResponse>& OnSuccess,
		const FErrorHandler& OnError);
	
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
