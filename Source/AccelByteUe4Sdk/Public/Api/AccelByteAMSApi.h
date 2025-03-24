// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "CoreMinimal.h"
#include "Core/AccelByteApiBase.h"
#include "Models/AccelByteAMSModels.h"

namespace AccelByte
{
class Credentials;
class Settings;

namespace Api
{

class ACCELBYTEUE4SDK_API AMS : public FApiBase, public TSharedFromThis<AMS, ESPMode::ThreadSafe>
{
public:
	AMS(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);
	~AMS();

	/**
	 * @brief Get the account associated with the namespace.
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetAccount(THandler<FAccelByteModelsAMSGetAccountResponse> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	AMS() = delete;
	AMS(AMS const&) = delete;
	AMS(AMS&&) = delete;
};

typedef TSharedRef<AMS, ESPMode::ThreadSafe> AMSRef;
typedef TSharedPtr<AMS, ESPMode::ThreadSafe> AMSPtr;
typedef TWeakPtr<AMS, ESPMode::ThreadSafe> AMSWPtr;

}
}
