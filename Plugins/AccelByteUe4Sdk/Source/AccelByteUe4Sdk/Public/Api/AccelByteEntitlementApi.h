// Copyright (c) 2018-2020 AccelByte Inc. All Rights Reserved.
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
* @brief Entitlement API for checking user's ownership. User can query a list of item that belongs to him/her.
*/
class ACCELBYTEUE4SDK_API Entitlement
{
public:
	Entitlement(const Credentials& Credentials, const Settings& Settings);
	~Entitlement();
private:
	const Credentials& Credentials;
	const Settings& Settings;
public:
	/**
	* @brief Get list of user's Entitlement(s).
	*
	* @param EntitlementName The name of the entitlement (optional).
	* @param ItemId Item's id (optional).
	* @param Offset Offset of the list that has been sliced based on Limit parameter (optional, default = 0).
	* @param Limit The limit of item on page (optional).
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementPagingSlicedResult.
	* @param OnError This will be called when the operation failed.
	* @param EntitlementClass Class of the entitlement (optional).
	* @param AppType This is the type of application that entitled (optional).
	*/
	void QueryUserEntitlements(const FString& EntitlementName, const FString& ItemId, const int32& Offset, const int32& Limit, const THandler<FAccelByteModelsEntitlementPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError, EAccelByteEntitlementClass EntitlementClass, EAccelByteAppType AppType);

	/**
	* @brief Get user's Entitlement by the EntitlementId.
	*
	* @param EntitlementId The id of the entitlement.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	* @param OnError This will be called when the operation failed.
	*/
	void GetUserEntitlementById(const FString& Entitlementid, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError);
/**
	* @brief Consume a use entitlement.
	*
	* @param EntitlementId The id of the entitlement.
	* @param UseCount Number of consumed entitlement.
	* @param OnSuccess This will be called when the operation success. The result is const FAccelByteModelsEntitlementInfo.
	* @param OnError This will be called when the operation failed.
	*/
	void ConsumeUserEntitlement(const FString& EntitlementId, const int32& UseCount, const THandler<FAccelByteModelsEntitlementInfo>& OnSuccess, const FErrorHandler& OnError);

private:
	Entitlement() = delete;
	Entitlement(Entitlement const&) = delete;
	Entitlement(Entitlement&&) = delete;
};

} // Namespace Api
}
