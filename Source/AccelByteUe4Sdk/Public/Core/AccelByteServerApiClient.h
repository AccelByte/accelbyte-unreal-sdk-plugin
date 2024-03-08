// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteRegistry.h"
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

namespace AccelByte
{
class ACCELBYTEUE4SDK_API FServerApiClient final
{
public:
	FServerApiClient();
	FServerApiClient(AccelByte::ServerCredentials& Credentials, AccelByte::FHttpRetryScheduler& Http);
	~FServerApiClient();

	bool bUseSharedCredentials = true;

	FServerCredentialsRef ServerCredentialsRef{};
	FHttpRetrySchedulerRef HttpRef{};

	GameServerApi::ServerAchievement ServerAchievement{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerCloudSave ServerCloudSave{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerDSM ServerDSM{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerEcommerce ServerEcommerce{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerGameTelemetry ServerGameTelemetry{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerLobby ServerLobby{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerChat ServerChat{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerMatchmaking ServerMatchmaking{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerOauth2 ServerOauth2{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerQosManager ServerQosManager{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerSeasonPass ServerSeasonPass{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerSessionBrowser ServerSessionBrowser{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerStatistic ServerStatistic{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerUGC ServerUGC{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerUser ServerUser{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerSession ServerSession{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerDSHub ServerDSHub{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerMatchmakingV2 ServerMatchmakingV2{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerAMS ServerAMS{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerMetricExporter ServerMetric{ FRegistry::ServerSettings };
	GameServerApi::ServerPredefinedEvent ServerPredefinedEvent{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerGameStandardEvent ServerGameStandardEvent{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerBinaryCloudSave ServerBinaryCloudSave{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerChallenge ServerChallenge{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };

	template<typename T, typename... U>
	T GetServerApi(U&&... Args)
	{
		static_assert(std::is_base_of<FServerApiBase, T>::value, "API class must be subclass of FServerApiBase");

		return T(*ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef, Forward<U>(Args)...);
	}

	template<typename T, typename... U>
	TSharedPtr<T, ESPMode::ThreadSafe> GetServerApiPtr(U&&... Args)
	{
		static_assert(std::is_base_of<FServerApiBase, T>::value, "API class must be subclass of FServerApiBase");

		return MakeShared<T, ESPMode::ThreadSafe>(
			*ServerCredentialsRef, FRegistry::ServerSettings,
			*HttpRef, Forward<U>(Args)...);
	}
};

typedef TSharedRef<FServerApiClient, ESPMode::ThreadSafe> FServerApiClientRef;
typedef TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> FServerApiClientPtr;
}// Namespace AccelByte
