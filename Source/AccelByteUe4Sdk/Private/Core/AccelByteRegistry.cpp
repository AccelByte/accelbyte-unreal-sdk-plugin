// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

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
#include "Api/AccelByteReportingApi.h"
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
#include "GameServerApi/AccelByteServerSeasonPassApi.h"

using namespace AccelByte;

Settings FRegistry::Settings;
FHttpRetryScheduler FRegistry::HttpRetryScheduler;
Credentials FRegistry::Credentials;
ServerSettings FRegistry::ServerSettings;
ServerCredentials FRegistry::ServerCredentials;
Api::User FRegistry::User{FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler};
Api::UserProfile FRegistry::UserProfile{FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler};
Api::Category FRegistry::Category(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::Entitlement FRegistry::Entitlement(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::Order FRegistry::Order(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::Item FRegistry::Item(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::Wallet FRegistry::Wallet(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::Fulfillment FRegistry::Fulfillment(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::CloudStorage FRegistry::CloudStorage(FRegistry::Credentials, FRegistry::Settings);
Api::Lobby FRegistry::Lobby(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::GameProfile FRegistry::GameProfile(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::Statistic FRegistry::Statistic(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::QosManager FRegistry::QosManager(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::CloudSave FRegistry::CloudSave(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::Qos FRegistry::Qos;
Api::Leaderboard FRegistry::Leaderboard(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::GameTelemetry FRegistry::GameTelemetry(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::Agreement FRegistry::Agreement{FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler};
Api::Achievement FRegistry::Achievement(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::SessionBrowser FRegistry::SessionBrowser(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::UGC FRegistry::UGC(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::SeasonPass FRegistry::SeasonPass(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
Api::Reporting FRegistry::Reporting(FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
GameServerApi::ServerOauth2 FRegistry::ServerOauth2(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerDSM FRegistry::ServerDSM(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerStatistic FRegistry::ServerStatistic(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerEcommerce FRegistry::ServerEcommerce(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerQosManager FRegistry::ServerQosManager(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerGameTelemetry FRegistry::ServerGameTelemetry(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerAchievement FRegistry::ServerAchievement(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerMatchmaking FRegistry::ServerMatchmaking(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerLobby FRegistry::ServerLobby(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerCloudSave FRegistry::ServerCloudSave(FRegistry::ServerCredentials, FRegistry::ServerSettings);
GameServerApi::ServerSeasonPass FRegistry::ServerSeasonPass(FRegistry::ServerCredentials, FRegistry::ServerSettings);
