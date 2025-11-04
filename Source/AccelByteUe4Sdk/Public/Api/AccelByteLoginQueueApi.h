// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteApiBase.h"
#include "Models/AccelByteUserModels.h"

namespace AccelByte
{

namespace Api
{

/**
 * @brief Login Queue API for Login Ticket Management.
 */
class ACCELBYTEUE4SDK_API LoginQueue 
	: public FApiBase
	, public TSharedFromThis<LoginQueue, ESPMode::ThreadSafe>
{
public:
	LoginQueue(Credentials& Credentials
		, Settings& Settings
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	LoginQueue(Credentials& Credentials
		, Settings& Settings
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~LoginQueue();
private:
	TSharedRef<Credentials, ESPMode::ThreadSafe> UserCredentialsRef;
public:

	/**
	 * @brief Refresh Login Ticket.
	 *
	 * @param Ticket Login Ticket to refresh.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Namespace Namespace of the game
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RefreshTicket(FString const& Ticket
		, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& Namespace = TEXT(""));

	/**
	 * @brief Cancel Login Ticket.
	 *
	 * @param Ticket Login Ticket to cancel.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Namespace Namespace of the game
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CancelTicket(FString const& Ticket
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& Namespace = TEXT(""));

private:
	LoginQueue() = delete;
	LoginQueue(LoginQueue const&) = delete;
	LoginQueue(LoginQueue&&) = delete;
	 
};

using LoginQueueRef = TSharedRef<LoginQueue, ESPMode::ThreadSafe>;
using LoginQueuePtr = TSharedPtr<LoginQueue, ESPMode::ThreadSafe>;
using LoginQueueWPtr = TWeakPtr<LoginQueue, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
