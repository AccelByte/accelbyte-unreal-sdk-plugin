// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Online/ICMP/Public/Icmp.h"
#include "Networking.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteQosModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
* @brief QosManager API to manage Qos Server(s).
*/
class ACCELBYTEUE4SDK_API ServerQosManager
{
public:
	ServerQosManager(const ServerCredentials& Credentials, const ServerSettings& Settings);
	~ServerQosManager();

	/**
	* @brief Get List of Qos Server(s).
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsQosServerList.
	* @param OnError This will be called when the operation failed.
	*/
	void GetQosServers(const THandler<FAccelByteModelsQosServerList>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Get List of Qos Server(s) Latencies.
	*
	* @param OnSuccess This will be called when the operation succeeded. The result is an array of TMap with FString as key and int32 as value.
	* @param OnError This will be called when the operation failed.
	*/
	void GetServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnSuccess, const FErrorHandler& OnError);

	TArray<TPair<FString, float>> Latencies;
private:
	const ServerCredentials& Credentials;
	const ServerSettings& Settings;

	ServerQosManager() = delete;
	ServerQosManager(ServerQosManager const&) = delete;
	ServerQosManager(ServerQosManager&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
