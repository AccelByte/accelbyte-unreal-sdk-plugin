// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteEcommerceModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief Fulfillment API for user to fulfill an item to their entitlements.
 */
class ACCELBYTEUE4SDK_API Fulfillment : public FApiBase, public TSharedFromThis<Fulfillment, ESPMode::ThreadSafe>
{
public:
	Fulfillment(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef, TSharedPtr<FApiClient, ESPMode::ThreadSafe> InApiClient = nullptr);
	~Fulfillment();

	/**
	 * @brief Redeem Campaign Code to Receive In Game Item.
	 *
	 * @param Code The campaign code to redeem.
	 * @param Region Region of the item. If not set, the region from the access token will be used.
	 * @param Language Display language.null If not set, the language from the access token will be used.
	 * @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsFulfillmentResult.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RedeemCode(FString const& Code
		, FString const& Region
		, FString const& Language
		, THandler<FAccelByteModelsFulfillmentResult> const& OnSuccess
		, FErrorHandler const& OnError);

private:
	Fulfillment() = delete;
	Fulfillment(Fulfillment const&) = delete;
	Fulfillment(Fulfillment&&) = delete;
};

typedef TSharedRef<Fulfillment, ESPMode::ThreadSafe> FulfillmentRef;
typedef TSharedPtr<Fulfillment, ESPMode::ThreadSafe> FulfillmentPtr;
typedef TWeakPtr<Fulfillment, ESPMode::ThreadSafe> FulfillmentWPtr;

} // Namespace Api
}
