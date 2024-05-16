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
class ACCELBYTEUE4SDK_API LoginQueue : public FApiBase
{
public:
	LoginQueue(Credentials& Credentials, Settings& Settings, FHttpRetryScheduler& InHttpRef);
	~LoginQueue();
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
	 */
	void RefreshTicket(const FString& Ticket
		, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess
		, const FErrorHandler& OnError
		, const FString& Namespace = TEXT(""));

	/**
	 * @brief Cancel Login Ticket.
	 *
	 * @param Ticket Login Ticket to cancel.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param Namespace Namespace of the game
	 */
	void CancelTicket(const FString& Ticket
		, const FVoidHandler& OnSuccess
		, const FErrorHandler& OnError
		, const FString& Namespace = TEXT(""));

private:
	LoginQueue() = delete;
	LoginQueue(LoginQueue const&) = delete;
	LoginQueue(LoginQueue&&) = delete;
	 
};

} // Namespace Api
} // Namespace AccelByte
