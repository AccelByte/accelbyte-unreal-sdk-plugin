// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteApiUtilities.h"
#include "Api/AccelByteQos.h"

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteHttpClient.h"

#include "GameServerApi/AccelByteServerAchievementApi.h"
#include "GameServerApi/AccelByteServerChatApi.h"
#include "GameServerApi/AccelByteServerCloudSaveApi.h"
#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerEcommerceApi.h"
#include "GameServerApi/AccelByteServerGameTelemetryApi.h"
#include "GameServerApi/AccelByteServerLobby.h"
#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerQosManagerApi.h"
#include "GameServerApi/AccelByteServerSeasonPassApi.h"
#include "GameServerApi/AccelByteServerSessionBrowserApi.h"
#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "GameServerApi/AccelByteServerUGCApi.h"
#include "GameServerApi/AccelByteServerUserApi.h"
#include "GameServerApi/AccelByteServerSessionApi.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteServerApiBase.h"
#include "GameServerApi/AccelByteServerDSHubApi.h"
#include "GameServerApi/AccelByteServerMatchmakingV2Api.h"
#include "GameServerApi/AccelByteServerAMSApi.h"
#include "GameServerApi/AccelByteServerMetricExporterApi.h"
#include "GameServerApi/AccelByteServerPredefinedEventApi.h"
#include "GameServerApi/AccelByteServerGameStandardEventApi.h"
#include "GameServerApi/AccelByteServerBinaryCloudSaveApi.h"
#include "GameServerApi/AccelByteServerChallengeApi.h"
#include "GameServerApi/AccelByteServerInventoryApi.h"
#include "ServerTime/AccelByteTimeManager.h"

class FAccelByteInstance;

namespace AccelByte
{
class ACCELBYTEUE4SDK_API FServerApiClient final : public TSharedFromThis<FServerApiClient, ESPMode::ThreadSafe>
{
public:
	FServerApiClient(ServerSettingsPtr InServerSettings, FAccelByteTimeManagerPtr InTimeManager, TSharedRef<FAccelByteInstance, ESPMode::ThreadSafe> InAccelByteInstance);
	~FServerApiClient();

	bool bUseSharedCredentials = true;

	TWeakPtr<FAccelByteInstance, ESPMode::ThreadSafe> AccelByteInstanceWeak;

	FHttpRetrySchedulerRef HttpRef{};
	FServerCredentialsRef ServerCredentialsRef;
	ServerSettingsPtr ServerSettings{};
	const FAccelByteTimeManagerPtr ServerTimeManager{};

	GameServerApi::ServerAchievement ServerAchievement{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerCloudSave ServerCloudSave{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerDSM ServerDSM{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerEcommerce ServerEcommerce{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerGameTelemetry ServerGameTelemetry{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerLobby ServerLobby{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerChat ServerChat{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerMatchmaking ServerMatchmaking{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerOauth2 ServerOauth2{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerQosManager ServerQosManager{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerSeasonPass ServerSeasonPass{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerSessionBrowser ServerSessionBrowser{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerStatistic ServerStatistic{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerUGC ServerUGC{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerUser ServerUser{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerSession ServerSession{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerDSHub ServerDSHub{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerMatchmakingV2 ServerMatchmakingV2{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerAMS ServerAMS{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerMetricExporter ServerMetric{ *ServerSettings};
	GameServerApi::ServerPredefinedEvent ServerPredefinedEvent{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerGameStandardEvent ServerGameStandardEvent{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerBinaryCloudSave ServerBinaryCloudSave{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerChallenge ServerChallenge{ *ServerCredentialsRef, *ServerSettings, *HttpRef};
	GameServerApi::ServerInventory ServerInventory{ *ServerCredentialsRef, *ServerSettings, *HttpRef};

	void Init();
	
	/**
	 * @brief Utility function to get the device ID associated with this ServerApiClient's AccelByteInstance
	 * @return device ID string, may be empty if AccelByteInstance is invalid or Device ID not found
	 */
	FString GetDeviceId() const;

	template<typename T, typename... U>
	T GetServerApi(U&&... Args)
	{
		static_assert(std::is_base_of<FServerApiBase, T>::value, "API class must be subclass of FServerApiBase");

		return T(*ServerCredentialsRef, *ServerSettings, *HttpRef, AsShared(), Forward<U>(Args)...);
	}

	template<typename T, typename... U>
	TSharedPtr<T, ESPMode::ThreadSafe> GetServerApiPtr(U&&... Args)
	{
		static_assert(std::is_base_of<FServerApiBase, T>::value, "API class must be subclass of FServerApiBase");

		return MakeShared<T, ESPMode::ThreadSafe>(
			*ServerCredentialsRef, *ServerSettings,
			*HttpRef, AsShared(), Forward<U>(Args)...);
	}

	FAccelByteApiUtilitiesWPtr GetServerApiUtilities();

private:
	FAccelByteApiUtilitiesPtr ServerApiUtilitiesPtr;
};

typedef TSharedRef<FServerApiClient, ESPMode::ThreadSafe> FServerApiClientRef;
typedef TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> FServerApiClientPtr;
}// Namespace AccelByte
