// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;
namespace GameServerApi
{

/**
 * @brief Send telemetry data securely and server should be logged in first.
 */
class ACCELBYTEUE4SDK_API ServerGameTelemetry
{
public:
	ServerGameTelemetry(const ServerCredentials& Credentials, const ServerSettings& Settings);
	~ServerGameTelemetry();

	/**
	 * @brief Send single authorized telemetry data. Server must be logged in.
	 * 
	 * @param EventName Telemetry request with arbitrary payload
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void SendProtectedEvent(FString EventName, FJsonObject Payload, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Send multiple authorized telemetry data. Server must be logged in.
	 *
	 * @param Events Map of telemetry payload with event name as its key
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void SendProtectedEvents(TMap<FString, TSharedPtr<FJsonObject>> Events, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

private:
	ServerGameTelemetry() = delete;
	ServerGameTelemetry(ServerGameTelemetry const&) = delete;
	ServerGameTelemetry(ServerGameTelemetry&&) = delete;

	const ServerCredentials& Credentials;
	const ServerSettings& Settings;
};

} // Namespace Api
} // Namespace AccelByte
