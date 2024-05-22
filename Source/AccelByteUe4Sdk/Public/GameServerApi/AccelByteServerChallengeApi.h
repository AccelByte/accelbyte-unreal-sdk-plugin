// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"
#include "Models/AccelByteChallengeModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{
	
/**
 * @brief Server Challenge APIs to manage user challenge progress
 */
class ACCELBYTEUE4SDK_API ServerChallenge : public FServerApiBase
{
public:
	ServerChallenge(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef);
	~ServerChallenge();
	
	/**
	 * @brief Send a request to attempt to evaluate many user's challenge progress
	 *
	 * @param Request Structure containing the IDs of the users to evaluate challenge progress for
	 * @param OnSuccess Delegate executed when request succeeds
	 * @param OnError Delegate executed when request fails
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr EvaluateChallengeProgress(FAccelByteModelsChallengeServerEvaluateProgressRequest const& Request
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

private:
	ServerChallenge() = delete;
	ServerChallenge(const ServerChallenge&) = delete;
	ServerChallenge(ServerChallenge&&) = delete;
};
}
}
