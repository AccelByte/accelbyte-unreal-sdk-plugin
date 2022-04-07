// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteHttpClient.h"

#include "GameServerApi/AccelByteServerAchievementApi.h"
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
#include "GameServerApi/AccelByteServerUserApi.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"

using namespace AccelByte;

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

	GameServerApi::ServerAchievement ServerAchievement{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerCloudSave ServerCloudSave{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerDSM ServerDSM{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerEcommerce ServerEcommerce{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerGameTelemetry ServerGameTelemetry{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerLobby ServerLobby{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerMatchmaking ServerMatchmaking{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerOauth2 ServerOauth2{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerQosManager ServerQosManager{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerSeasonPass ServerSeasonPass{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerSessionBrowser ServerSessionBrowser{ *ServerCredentialsRef, FRegistry::ServerSettings, *HttpRef };
	GameServerApi::ServerStatistic ServerStatistic{ *ServerCredentialsRef, FRegistry::ServerSettings };
	GameServerApi::ServerUser ServerUser{ *ServerCredentialsRef, FRegistry::ServerSettings };
};

typedef TSharedRef<FServerApiClient, ESPMode::ThreadSafe> FServerApiClientRef;
typedef TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> FServerApiClientPtr;
}// Namespace AccelByte
