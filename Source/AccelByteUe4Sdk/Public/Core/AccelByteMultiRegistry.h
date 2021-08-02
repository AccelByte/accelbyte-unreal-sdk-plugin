// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteCredentials.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteUserProfileApi.h"
#include "Api/AccelByteCategoryApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteWalletApi.h"
#include "Api/AccelByteFulfillmentApi.h"
#include "Api/AccelByteCloudStorageApi.h"
#include "Api/AccelByteLobbyApi.h"
#include "Api/AccelByteGameProfileApi.h"
#include "Api/AccelByteStatisticApi.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Api/AccelByteQos.h"
#include "Api/AccelByteLeaderboardApi.h"
#include "Api/AccelByteCloudSaveApi.h"
#include "Api/AccelByteGameTelemetryApi.h"
#include "Api/AccelByteAgreementApi.h"
#include "Api/AccelByteAchievementApi.h"
#include "Api/AccelByteSessionBrowserApi.h"
#include "Api/AccelByteUGCApi.h"
#include "Api/AccelByteSeasonPassApi.h"

#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "GameServerApi/AccelByteServerEcommerceApi.h"
#include "GameServerApi/AccelByteServerQosManagerApi.h"
#include "GameServerApi/AccelByteServerGameTelemetryApi.h"
#include "GameServerApi/AccelByteServerAchievementApi.h"
#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "GameServerApi/AccelByteServerLobby.h"
#include "GameServerApi/AccelByteServerCloudSaveApi.h"

using namespace AccelByte;

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FApiClient
{
public:
	Credentials Credentials;
	Api::User User{ Credentials, FRegistry::Settings };
	Api::UserProfile UserProfile{ Credentials, FRegistry::Settings };
	Api::Category Category{ Credentials, FRegistry::Settings };
	Api::Entitlement Entitlement{ Credentials, FRegistry::Settings };
	Api::Order Order{ Credentials, FRegistry::Settings };
	Api::Item Item{ Credentials, FRegistry::Settings };
	Api::Wallet Wallet{ Credentials, FRegistry::Settings };
	Api::Fulfillment Fulfillment{ Credentials, FRegistry::Settings };
	Api::CloudStorage CloudStorage{ Credentials, FRegistry::Settings };
	Api::Lobby Lobby{ Credentials, FRegistry::Settings };
	Api::GameProfile GameProfile{ Credentials, FRegistry::Settings };
	Api::Statistic Statistic{ Credentials, FRegistry::Settings };
	Api::QosManager QosManager{ Credentials, FRegistry::Settings };
	Api::Qos Qos;
	Api::Leaderboard Leaderboard{ Credentials, FRegistry::Settings };
	Api::CloudSave CloudSave{ Credentials, FRegistry::Settings };
	Api::GameTelemetry GameTelemetry{ Credentials, FRegistry::Settings };
	Api::Agreement Agreement{ Credentials, FRegistry::Settings };
	Api::Achievement Achievement{ Credentials, FRegistry::Settings };
	Api::SessionBrowser SessionBrowser{ Credentials, FRegistry::Settings };
	Api::UGC UGC{ Credentials, FRegistry::Settings };
	Api::SeasonPass SeasonPass{ Credentials, FRegistry::Settings };
};

class ACCELBYTEUE4SDK_API FMultiRegistry
{
public:
	static TSharedPtr<FApiClient> GetApiClient(FString key);

private:
	static TMap<FString, TSharedPtr<FApiClient>> ApiClientInstances;

	FMultiRegistry() = delete;
	FMultiRegistry(const FMultiRegistry& other) = delete;
	FMultiRegistry& operator=(const FMultiRegistry& other) = delete;
	FMultiRegistry(FMultiRegistry&& other) = delete;
	FMultiRegistry& operator=(FMultiRegistry&& other) = delete;
	~FMultiRegistry() = delete;
};

}
