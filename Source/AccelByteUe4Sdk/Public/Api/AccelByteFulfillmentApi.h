// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "Core/AccelByteError.h"
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
class ACCELBYTEUE4SDK_API Fulfillment
{
public:
	Fulfillment(const Credentials& Credentials, const Settings& Settings);
	~Fulfillment();
private:
	const Credentials& Credentials;
	const Settings& Settings;
public:
	/**
	* @brief Redeem Campaign Code to Receive In Game Item.
	*
	* @param Code The campaign code to redeem.
	* @param Region Region of the item. If not set, the region from the access token will be used.
	* @param Language Display language.null If not set, the language from the access token will be used.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsFulfillmentResult.
	* @param OnError This will be called when the operation failed.
	*/
	void RedeemCode(const FString& Code, const FString& Region, const FString& Language, THandler<FAccelByteModelsFulfillmentResult> OnSuccess, FErrorHandler OnError);

private:
	Fulfillment() = delete;
	Fulfillment(Fulfillment const&) = delete;
	Fulfillment(Fulfillment&&) = delete;
};

} // Namespace Api
}
