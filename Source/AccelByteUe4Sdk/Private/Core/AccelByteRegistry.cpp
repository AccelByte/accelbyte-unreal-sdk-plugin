// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteServerCredentials.h"
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
#include "Api/AccelByteChallengeApi.h"
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
FCredentialsRef FRegistry::CredentialsRef { MakeShared<AccelByte::Credentials, ESPMode::ThreadSafe>(*MessagingSystem.Get()) };
Credentials& FRegistry::Credentials { FRegistry::CredentialsRef.Get() };
ServerSettings FRegistry::ServerSettings;
FServerCredentialsRef FRegistry::ServerCredentialsRef { MakeShared<AccelByte::ServerCredentials, ESPMode::ThreadSafe>() };
ServerCredentials& FRegistry::ServerCredentials { FRegistry::ServerCredentialsRef.Get() };
FAccelByteTimeManager FRegistry::TimeManager{ FRegistry::HttpRetryScheduler };
FAccelByteNetworkConditioner FRegistry::NetworkConditioner;
FAccelByteNotificationSender FRegistry::NotificationSender{*MessagingSystem.Get()};
#pragma endregion

#pragma region Game Client Access
Api::User FRegistry::User{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::UserProfile FRegistry::UserProfile{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::GameProfile FRegistry::GameProfile{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Agreement FRegistry::Agreement{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::GDPR FRegistry::GDPR{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Reporting FRegistry::Reporting{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Miscellaneous FRegistry::Miscellaneous{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Configurations FRegistry::Configurations{FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler};
Api::LoginQueue FRegistry::LoginQueue{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Commerce
Api::Currency FRegistry::Currency{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Wallet FRegistry::Wallet{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Category FRegistry::Category{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Item FRegistry::Item{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Order FRegistry::Order{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Entitlement FRegistry::Entitlement{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Fulfillment FRegistry::Fulfillment{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::StoreDisplay FRegistry::StoreDisplay{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Storage
Api::CloudStorage FRegistry::CloudStorage{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::CloudSave FRegistry::CloudSave{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::UGC FRegistry::UGC{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Engagement
Api::Statistic FRegistry::Statistic{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Achievement FRegistry::Achievement{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Leaderboard FRegistry::Leaderboard{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Reward FRegistry::Reward{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::SeasonPass FRegistry::SeasonPass{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Group FRegistry::Group{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Challenge FRegistry::Challenge{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Multiplayer
Api::QosManager FRegistry::QosManager{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::Qos FRegistry::Qos{ FRegistry::CredentialsRef.Get(), FRegistry::Settings,  *FRegistry::MessagingSystem.Get() };
Api::Lobby FRegistry::Lobby{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler, *FRegistry::MessagingSystem.Get(), FRegistry::NetworkConditioner};
Api::Chat FRegistry::Chat{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler,  *FRegistry::MessagingSystem.Get(), FRegistry::NetworkConditioner};
Api::SessionBrowser FRegistry::SessionBrowser{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::TurnManager FRegistry::TurnManager{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, HttpRetryScheduler };
Api::Session FRegistry::Session{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::MatchmakingV2 FRegistry::MatchmakingV2{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::AMS FRegistry::AMS{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Game Client Analytics
Api::GameTelemetry FRegistry::GameTelemetry{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::PredefinedEvent FRegistry::PredefinedEvent{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
Api::GameStandardEvent FRegistry::GameStandardEvent{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };
#pragma endregion

Api::HeartBeat FRegistry::HeartBeat{ FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler };

#pragma region Dedicated Server Access
GameServerApi::ServerOauth2 FRegistry::ServerOauth2{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerUser FRegistry::ServerUser{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Commerce
GameServerApi::ServerEcommerce FRegistry::ServerEcommerce{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Storage
GameServerApi::ServerCloudSave FRegistry::ServerCloudSave{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerUGC FRegistry::ServerUGC{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Engagement
GameServerApi::ServerStatistic FRegistry::ServerStatistic{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerAchievement FRegistry::ServerAchievement{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerSeasonPass FRegistry::ServerSeasonPass{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Multiplayer
GameServerApi::ServerLobby FRegistry::ServerLobby{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerChat FRegistry::ServerChat{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerSessionBrowser FRegistry::ServerSessionBrowser{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerMatchmaking FRegistry::ServerMatchmaking{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerDSHub FRegistry::ServerDSHub{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerSession FRegistry::ServerSession{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerMatchmakingV2 FRegistry::ServerMatchmakingV2{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Analytics
GameServerApi::ServerGameTelemetry FRegistry::ServerGameTelemetry{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerPredefinedEvent FRegistry::ServerPredefinedEvent{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerGameStandardEvent FRegistry::ServerGameStandardEvent{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion

#pragma region Dedicated Server Management
GameServerApi::ServerDSM FRegistry::ServerDSM{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerAMS FRegistry::ServerAMS{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
GameServerApi::ServerMetricExporter FRegistry::ServerMetricExporter{ FRegistry::ServerSettings };
GameServerApi::ServerQosManager FRegistry::ServerQosManager{ FRegistry::ServerCredentialsRef.Get(), FRegistry::ServerSettings, FRegistry::HttpRetryScheduler };
#pragma endregion
