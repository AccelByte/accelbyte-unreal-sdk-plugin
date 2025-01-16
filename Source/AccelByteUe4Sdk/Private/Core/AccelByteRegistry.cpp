// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteHttpClient.h"
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
#include "Api/AccelByteInventoryApi.h"
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
FHttpRetrySchedulerRef FRegistry::HttpRetrySchedulerRef{ MakeShared<FHttpRetryScheduler, ESPMode::ThreadSafe>() };
FHttpRetryScheduler& FRegistry::HttpRetryScheduler{ FRegistry::HttpRetrySchedulerRef.Get() };
FAccelByteMessagingSystemPtr FRegistry::MessagingSystem = MakeShared<FAccelByteMessagingSystem, ESPMode::ThreadSafe>();
Settings FRegistry::Settings;
FCredentialsRef FRegistry::CredentialsRef { MakeShared<AccelByte::Credentials, ESPMode::ThreadSafe>(*MessagingSystem.Get()) };
Credentials& FRegistry::Credentials { FRegistry::CredentialsRef.Get() };
ServerSettings FRegistry::ServerSettings;
FServerCredentialsRef FRegistry::ServerCredentialsRef { MakeShared<AccelByte::ServerCredentials, ESPMode::ThreadSafe>() };
ServerCredentials& FRegistry::ServerCredentials { FRegistry::ServerCredentialsRef.Get() };
FAccelByteTimeManager FRegistry::TimeManager{ FRegistry::HttpRetryScheduler };
FAccelByteNetworkConditioner FRegistry::NetworkConditioner;
FAccelByteNotificationSender FRegistry::NotificationSender{*MessagingSystem.Get()};
FHttpClient FRegistry::HttpClient{ Credentials, Settings, HttpRetryScheduler };
#pragma endregion

#pragma region GameClientAccess
const Api::UserPtr FRegistry::UserPtr{ MakeShared<Api::User, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::UserProfilePtr FRegistry::UserProfilePtr{ MakeShared<Api::UserProfile, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::GameProfilePtr FRegistry::GameProfilePtr{ MakeShared<Api::GameProfile, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::AgreementPtr FRegistry::AgreementPtr{ MakeShared<Api::Agreement, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::GDPRPtr FRegistry::GDPRPtr{ MakeShared<Api::GDPR, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::ReportingPtr FRegistry::ReportingPtr{ MakeShared<Api::Reporting, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::MiscellaneousPtr FRegistry::MiscellaneousPtr{ MakeShared<Api::Miscellaneous, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::ConfigurationsPtr FRegistry::ConfigurationsPtr{MakeShared<Api::Configurations, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler)};
const Api::LoginQueuePtr FRegistry::LoginQueuePtr{ MakeShared<Api::LoginQueue, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
#pragma endregion

#pragma region GameClientCommerce
const Api::CurrencyPtr FRegistry::CurrencyPtr{ MakeShared<Api::Currency, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::WalletPtr FRegistry::WalletPtr{ MakeShared<Api::Wallet, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::CategoryPtr FRegistry::CategoryPtr{ MakeShared<Api::Category, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::ItemPtr FRegistry::ItemPtr{ MakeShared<Api::Item, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::OrderPtr FRegistry::OrderPtr{ MakeShared<Api::Order, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::EntitlementPtr FRegistry::EntitlementPtr{ MakeShared<Api::Entitlement, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::FulfillmentPtr FRegistry::FulfillmentPtr{ MakeShared<Api::Fulfillment, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::StoreDisplayPtr FRegistry::StoreDisplayPtr{ MakeShared<Api::StoreDisplay, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
#pragma endregion

#pragma region GameClientStorage
const Api::CloudStoragePtr FRegistry::CloudStoragePtr{ MakeShared<Api::CloudStorage, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::CloudSavePtr FRegistry::CloudSavePtr{ MakeShared<Api::CloudSave, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::UGCPtr FRegistry::UGCPtr{ MakeShared<Api::UGC, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::InventoryPtr FRegistry::InventoryPtr{MakeShared<Api::Inventory, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler)};
#pragma endregion

#pragma region GameClientEngagement
const Api::StatisticPtr FRegistry::StatisticPtr{ MakeShared<Api::Statistic, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::AchievementPtr FRegistry::AchievementPtr{ MakeShared<Api::Achievement, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::LeaderboardPtr FRegistry::LeaderboardPtr{ MakeShared<Api::Leaderboard, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::RewardPtr FRegistry::RewardPtr{ MakeShared<Api::Reward, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::SeasonPassPtr FRegistry::SeasonPassPtr{ MakeShared<Api::SeasonPass, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::GroupPtr FRegistry::GroupPtr{ MakeShared<Api::Group, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::ChallengePtr FRegistry::ChallengePtr{ MakeShared<Api::Challenge, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
#pragma endregion

#pragma region GameClientMultiplayer
const Api::QosManagerPtr FRegistry::QosManagerPtr{ MakeShared<Api::QosManager, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::QosPtr FRegistry::QosPtr{ MakeShared<Api::Qos, ESPMode::ThreadSafe>( FRegistry::CredentialsRef.Get(), FRegistry::Settings,  *FRegistry::MessagingSystem.Get()) };
const Api::LobbyPtr FRegistry::LobbyPtr{ MakeShared<Api::Lobby, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler, *FRegistry::MessagingSystem.Get(), FRegistry::NetworkConditioner) };
const Api::ChatPtr FRegistry::ChatPtr{ MakeShared<Api::Chat, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler,  *FRegistry::MessagingSystem.Get(), FRegistry::NetworkConditioner) };
const Api::SessionBrowserPtr FRegistry::SessionBrowserPtr{ MakeShared<Api::SessionBrowser, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::TurnManagerPtr FRegistry::TurnManagerPtr{ MakeShared<Api::TurnManager, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::SessionPtr FRegistry::SessionPtr{ MakeShared<Api::Session, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::MatchmakingV2Ptr FRegistry::MatchmakingV2Ptr{ MakeShared<Api::MatchmakingV2, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::AMSPtr FRegistry::AMSPtr{ MakeShared<Api::AMS, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
#pragma endregion

#pragma region GameClientAnalytics
const Api::GameTelemetryPtr FRegistry::GameTelemetryPtr{ MakeShared<Api::GameTelemetry, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::PredefinedEventPtr FRegistry::PredefinedEventPtr{ MakeShared<Api::PredefinedEvent, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
const Api::GameStandardEventPtr FRegistry::GameStandardEventPtr{ MakeShared<Api::GameStandardEvent, ESPMode::ThreadSafe>(FRegistry::CredentialsRef.Get(), FRegistry::Settings, FRegistry::HttpRetryScheduler) };
#pragma endregion

#pragma region Game Client Access
Api::User& FRegistry::User{ *FRegistry::UserPtr.Get()};
Api::UserProfile& FRegistry::UserProfile{ *FRegistry::UserProfilePtr.Get()};
Api::GameProfile& FRegistry::GameProfile{ *FRegistry::GameProfilePtr.Get() };
Api::Agreement& FRegistry::Agreement{ *FRegistry::AgreementPtr.Get() };
Api::GDPR& FRegistry::GDPR{ *FRegistry::GDPRPtr.Get() };
Api::Reporting& FRegistry::Reporting{ *FRegistry::ReportingPtr.Get() };
Api::Miscellaneous& FRegistry::Miscellaneous{ *FRegistry::MiscellaneousPtr.Get() };
Api::Configurations& FRegistry::Configurations{*FRegistry::ConfigurationsPtr.Get() };
Api::LoginQueue& FRegistry::LoginQueue{ *FRegistry::LoginQueuePtr.Get() };
#pragma endregion

#pragma region Game Client Commerce
Api::Currency& FRegistry::Currency{ *FRegistry::CurrencyPtr.Get() };
Api::Wallet& FRegistry::Wallet{ *FRegistry::WalletPtr.Get() };
Api::Category& FRegistry::Category{ *FRegistry::CategoryPtr.Get() };
Api::Item& FRegistry::Item{ *FRegistry::ItemPtr.Get() };
Api::Order& FRegistry::Order{ *FRegistry::OrderPtr.Get() };
Api::Entitlement& FRegistry::Entitlement{ *FRegistry::EntitlementPtr.Get() };
Api::Fulfillment& FRegistry::Fulfillment{ *FRegistry::FulfillmentPtr.Get() };
Api::StoreDisplay& FRegistry::StoreDisplay{ *FRegistry::StoreDisplayPtr.Get() };
#pragma endregion

#pragma region Game Client Storage
Api::CloudStorage& FRegistry::CloudStorage{ *FRegistry::CloudStoragePtr.Get()};
Api::CloudSave& FRegistry::CloudSave{ *FRegistry::CloudSavePtr.Get()};
Api::UGC& FRegistry::UGC{ *FRegistry::UGCPtr.Get()};
Api::Inventory& FRegistry::Inventory{ *FRegistry::InventoryPtr.Get()};
#pragma endregion

#pragma region Game Client Engagement
Api::Statistic& FRegistry::Statistic{ *FRegistry::StatisticPtr.Get() };
Api::Achievement& FRegistry::Achievement{ *FRegistry::AchievementPtr.Get() };
Api::Leaderboard& FRegistry::Leaderboard{ *FRegistry::LeaderboardPtr.Get() };
Api::Reward& FRegistry::Reward{ *FRegistry::RewardPtr.Get() };
Api::SeasonPass& FRegistry::SeasonPass{ *FRegistry::SeasonPassPtr.Get() };
Api::Group& FRegistry::Group{ *FRegistry::GroupPtr.Get() };
Api::Challenge& FRegistry::Challenge{ *FRegistry::ChallengePtr.Get() };
#pragma endregion

#pragma region Game Client Multiplayer
Api::QosManager& FRegistry::QosManager{ *FRegistry::QosManagerPtr.Get()};
Api::Qos& FRegistry::Qos{ *FRegistry::QosPtr.Get()};
Api::Lobby& FRegistry::Lobby{ *FRegistry::LobbyPtr.Get() };
Api::Chat& FRegistry::Chat{ *FRegistry::ChatPtr.Get() };
Api::SessionBrowser& FRegistry::SessionBrowser{ *FRegistry::SessionBrowserPtr.Get()};
Api::TurnManager& FRegistry::TurnManager{ *FRegistry::TurnManagerPtr.Get()};
Api::Session& FRegistry::Session{ *FRegistry::SessionPtr.Get()};
Api::MatchmakingV2& FRegistry::MatchmakingV2{ *FRegistry::MatchmakingV2Ptr.Get()};
Api::AMS& FRegistry::AMS{ *FRegistry::AMSPtr.Get()};
#pragma endregion

#pragma region Game Client Analytics
Api::GameTelemetry& FRegistry::GameTelemetry{ *FRegistry::GameTelemetryPtr.Get() };
Api::PredefinedEvent& FRegistry::PredefinedEvent{ *FRegistry::PredefinedEventPtr.Get()};
Api::GameStandardEvent& FRegistry::GameStandardEvent{ *FRegistry::GameStandardEventPtr.Get()};
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