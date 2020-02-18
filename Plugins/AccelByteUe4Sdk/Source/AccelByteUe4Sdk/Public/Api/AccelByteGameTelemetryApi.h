// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Models/AccelByteGameTelemetryModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief 
 */
class ACCELBYTEUE4SDK_API GameTelemetry
{
public:
	GameTelemetry(const Credentials& Credentials, const Settings& Settings);
	~GameTelemetry();

	/**
	 * @brief Send single authorized telemetry data. User must be logged in.
	 * 
	 * @param EventName Telemetry request with arbitrary payload
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void SendProtectedEvent(FString EventName, FJsonObject Payload, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Send multiple authorized telemetry data. User must be logged in.
	 *
	 * @param Events Map of telemetry payload with event name as its key
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void SendProtectedEvents(TMap<FString, TSharedPtr<FJsonObject>> Events, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

private:
	GameTelemetry() = delete;
	GameTelemetry(GameTelemetry const&) = delete;
	GameTelemetry(GameTelemetry&&) = delete;

	const Credentials& Credentials;
	const Settings& Settings;
};

} // Namespace Api
} // Namespace AccelByte
