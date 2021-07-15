// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteQosModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
* @brief QosManager API to manage Qos Server(s).
*/
class ACCELBYTEUE4SDK_API QosManager
{
public:
	QosManager(const Credentials& Credentials, const Settings& Settings);
	~QosManager();

	/**
	* @brief Get List of Qos Server(s).
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsQosServerList.
	* @param OnError This will be called when the operation failed.
	*/
	void GetQosServers(const THandler<FAccelByteModelsQosServerList>& OnSuccess, const FErrorHandler& OnError);
private:
	const Credentials& Credentials;
	const Settings& Settings;

	QosManager() = delete;
	QosManager(QosManager const&) = delete;
	QosManager(QosManager&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
