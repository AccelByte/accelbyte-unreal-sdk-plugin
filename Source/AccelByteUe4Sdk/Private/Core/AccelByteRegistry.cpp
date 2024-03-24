// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteCredentials.h"
#include "Core/ServerTime/AccelByteTimeManager.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteUserProfileApi.h"
#include "Api/AccelByteCategoryApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteGroupApi.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteWalletApi.h"
#include "Api/AccelByteFulfillmentApi.h"
#include "Api/AccelByteCloudStorageApi.h"
#include "Api/AccelByteLobbyApi.h"
#include "Api/AccelByteChatApi.h"
#include "Api/AccelByteGameProfileApi.h"
#include "Api/AccelByteStatisticApi.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Api/AccelByteQos.h"
#include "Api/AccelByteLeaderboardApi.h"
#include "Api/AccelByteCloudSaveApi.h"
#include "Api/AccelByteGameTelemetryApi.h"
#include "Api/AccelByteAgreementApi.h"
#include "Api/AccelByteAchievementApi.h"
#include "Api/AccelByteAMSApi.h"
#include "Api/AccelByteConfigurationsApi.h"
#include "Api/AccelByteSessionBrowserApi.h"
#include "Api/AccelByteTurnManagerApi.h"
#include "Api/AccelByteUGCApi.h"
#include "Api/AccelByteSeasonPassApi.h"
#include "Api/AccelByteReportingApi.h"
#include "Api/AccelByteCurrencyApi.h"
#include "Api/AccelByteGDPRApi.h"
#include "Api/AccelByteMiscellaneousApi.h"
#include "Api/AccelByteRewardApi.h"
#include "Api/AccelByteSessionApi.h"
#include "Api/AccelByteMatchmakingV2Api.h"
#include "Api/AccelByteHeartBeatApi.h"
#include "Api/AccelByteStoreDisplayApi.h"
#include "Api/AccelBytePredefinedEventApi.h"
#include "Api/AccelByteGameStandardEventApi.h"
#include "Api/AccelByteLoginQueueApi.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "GameServerApi/AccelByteServerUGCApi.h"
#include "GameServerApi/AccelByteServerEcommerceApi.h"
#include "GameServerApi/AccelByteServerQosManagerApi.h"
#include "GameServerApi/AccelByteServerGameTelemetryApi.h"
#include "GameServerApi/AccelByteServerAchievementApi.h"
#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "GameServerApi/AccelByteServerLobby.h"
#include "GameServerApi/AccelByteServerChatApi.h"
#include "GameServerApi/AccelByteServerCloudSaveApi.h"
#include "GameServerApi/AccelByteServerSeasonPassApi.h"
#include "GameServerApi/AccelByteServerSessionBrowserApi.h"
#include "GameServerApi/AccelByteServerUserApi.h"
#include "GameServerApi/AccelByteServerSessionApi.h"
#include "GameServerApi/AccelByteServerDSHubApi.h"
#include "GameServerApi/AccelByteServerMatchmakingV2Api.h"
#include "GameServerApi/AccelByteServerAMSApi.h"
#include "GameServerApi/AccelByteServerMetricExporterApi.h"
#include "GameServerApi/AccelByteServerPredefinedEventApi.h"
#include "GameServerApi/AccelByteServerGameStandardEventApi.h"

using namespace AccelByte;

#pragma region Core
FHttpRetryScheduler FRegistry::HttpRetryScheduler;
TSharedPtr<FAccelByteMessagingSystem> FRegistry::MessagingSystem = MakeShared<FAccelByteMessagingSystem>();
Settings FRegistry::Settings;
Credentials FRegistry::Credentials {*MessagingSystem.Get()};
ServerSettings FRegistry::ServerSettings;
ServerCredentials FRegistry::ServerCredentials;
FAccelByteTimeManager FRegistry::TimeManager{ FRegistry::HttpRetryScheduler };
FAccelByteNetworkConditioner FRegistry::NetworkConditioner;
FAccelByteNotificationSender FRegistry::NotificationSender{*MessagingSystem.Get()};
#pragma endregion

#pragma region Game Client Access
Api::User FRegistry::User{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::UserProfile FRegistry::UserProfile{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::GameProfile FRegistry::GameProfile{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Agreement FRegistry::Agreement{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::GDPR FRegistry::GDPR{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Reporting FRegistry::Reporting{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Miscellaneous FRegistry::Miscellaneous{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Configurations FRegistry::Configurations{FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler};
Api::LoginQueue FRegistry::LoginQueue{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Commerce
Api::Currency FRegistry::Currency{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Wallet FRegistry::Wallet{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Category FRegistry::Category{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Item FRegistry::Item{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Order FRegistry::Order{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Entitlement FRegistry::Entitlement{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Fulfillment FRegistry::Fulfillment{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::StoreDisplay FRegistry::StoreDisplay{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Storage
Api::CloudStorage FRegistry::CloudStorage{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::CloudSave FRegistry::CloudSave{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::UGC FRegistry::UGC{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Engagement
Api::Statistic FRegistry::Statistic{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Achievement FRegistry::Achievement{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Leaderboard FRegistry::Leaderboard{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Reward FRegistry::Reward{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::SeasonPass FRegistry::SeasonPass{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Group FRegistry::Group{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Multiplayer
Api::QosManager FRegistry::QosManager{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Qos FRegistry::Qos{ FRegistry::Credentials, FRegistry::Settings,  *FRegistry::MessagingSystem.Get() };
Api::Lobby FRegistry::Lobby{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler, *FRegistry::MessagingSystem.Get(), FRegistry::NetworkConditioner};
Api::Chat FRegistry::Chat{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler,  *FRegistry::MessagingSystem.Get(), FRegistry::NetworkConditioner};
Api::SessionBrowser FRegistry::SessionBrowser{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::TurnManager FRegistry::TurnManager{ FRegistry::Credentials, FRegistry::Settings, HttpRetryScheduler };
Api::Session FRegistry::Session{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::MatchmakingV2 FRegistry::MatchmakingV2{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::AMS FRegistry::AMS{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Analytics
Api::GameTelemetry FRegistry::GameTelemetry{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::PredefinedEvent FRegistry::PredefinedEvent{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::GameStandardEvent FRegistry::GameStandardEvent{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

Api::HeartBeat FRegistry::HeartBeat{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };

#pragma region Dedicated Server Access
GameServerApi::ServerOauth2 FRegistry::ServerOauth2{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerUser FRegistry::ServerUser{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Commerce
GameServerApi::ServerEcommerce FRegistry::ServerEcommerce{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Storage
GameServerApi::ServerCloudSave FRegistry::ServerCloudSave{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerUGC FRegistry::ServerUGC{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Engagement
GameServerApi::ServerStatistic FRegistry::ServerStatistic{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerAchievement FRegistry::ServerAchievement{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerSeasonPass FRegistry::ServerSeasonPass{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Multiplayer
GameServerApi::ServerLobby FRegistry::ServerLobby{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerChat FRegistry::ServerChat{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerSessionBrowser FRegistry::ServerSessionBrowser{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerMatchmaking FRegistry::ServerMatchmaking{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerDSHub FRegistry::ServerDSHub{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerSession FRegistry::ServerSession{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerMatchmakingV2 FRegistry::ServerMatchmakingV2{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Analytics
GameServerApi::ServerGameTelemetry FRegistry::ServerGameTelemetry{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerPredefinedEvent FRegistry::ServerPredefinedEvent{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerGameStandardEvent FRegistry::ServerGameStandardEvent{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Management
GameServerApi::ServerDSM FRegistry::ServerDSM{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerAMS FRegistry::ServerAMS{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerMetricExporter FRegistry::ServerMetricExporter{ FRegistry::ServerSettings };
GameServerApi::ServerQosManager FRegistry::ServerQosManager{ FRegistry::ServerCredentials, FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion
