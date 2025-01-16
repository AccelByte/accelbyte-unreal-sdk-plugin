// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteSettings.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteMessagingSystem.h"
#include "Core/AccelByteNetworkConditioner.h"
#include "Core/AccelByteNotificationSender.h"

namespace AccelByte
{

class FHttpRetryScheduler;
class FHttpClient;
class FAccelByteTimeManager;

namespace Api
{
	class Group;
	class User;
	class UserProfile;
	class Category;
	class Entitlement;
	class Order;
	class Item;
	class Wallet;
	class Fulfillment;
	class CloudStorage;
	class Lobby;
	class Chat;
	class GameProfile;
	class Statistic;
	class QosManager;
	class Qos;
	class Leaderboard;
	class CloudSave;
	class GameTelemetry;
	class Agreement;
	class Achievement;
	class SessionBrowser;
	class TurnManager;
	class UGC;
	class Inventory;
	class SeasonPass;
	class Reporting;
	class Currency;
	class Miscellaneous;
	class Reward;
	class Session;
	class MatchmakingV2;
	class HeartBeat;
	class StoreDisplay;
	class GDPR;
	class PredefinedEvent;
	class GameStandardEvent;
	class AMS;
	class Configurations;
	class Challenge;
	class LoginQueue;
}

namespace GameServerApi
{
	class ServerOauth2;
	class ServerDSM;
	class ServerStatistic;
	class ServerUGC;
	class ServerEcommerce;
	class ServerQosManager;
	class ServerGameTelemetry;
	class ServerAchievement;
	class ServerMatchmaking;
	class ServerLobby;
	class ServerChat;
	class ServerCloudSave;
	class ServerSeasonPass;
	class ServerSessionBrowser;
	class ServerUser;
	class ServerSession;
	class ServerDSHub;
	class ServerMatchmakingV2;
	class ServerAMS;
	class ServerMetricExporter;
	class ServerPredefinedEvent;
	class ServerGameStandardEvent;
	class ServerChallenge;
}

class ACCELBYTEUE4SDK_API FRegistry
{
public:
#pragma region Core
	static FHttpRetrySchedulerRef HttpRetrySchedulerRef;
	static FHttpRetryScheduler& HttpRetryScheduler;
	static Settings Settings;
	static FCredentialsRef CredentialsRef;
	static Credentials& Credentials;
	static ServerSettings ServerSettings;
	static FServerCredentialsRef ServerCredentialsRef;
	static ServerCredentials& ServerCredentials;
	static FAccelByteTimeManager TimeManager;
	static FAccelByteMessagingSystemPtr MessagingSystem;
	static FAccelByteNetworkConditioner NetworkConditioner;
	static FAccelByteNotificationSender NotificationSender;
	static FHttpClient HttpClient;
#pragma endregion

#pragma region GameClientAccess
	static const TSharedPtr<Api::User, ESPMode::ThreadSafe> UserPtr;
	static const TSharedPtr<Api::UserProfile, ESPMode::ThreadSafe> UserProfilePtr;
	static const TSharedPtr<Api::GameProfile, ESPMode::ThreadSafe> GameProfilePtr;
	static const TSharedPtr<Api::Agreement, ESPMode::ThreadSafe> AgreementPtr;
	static const TSharedPtr<Api::GDPR, ESPMode::ThreadSafe> GDPRPtr;
	static const TSharedPtr<Api::Reporting, ESPMode::ThreadSafe> ReportingPtr;
	static const TSharedPtr<Api::Miscellaneous, ESPMode::ThreadSafe> MiscellaneousPtr;
	static const TSharedPtr<Api::Configurations, ESPMode::ThreadSafe> ConfigurationsPtr;
	static const TSharedPtr<Api::LoginQueue, ESPMode::ThreadSafe> LoginQueuePtr;
#pragma endregion

#pragma region GameClientCommerce
	static const TSharedPtr<Api::Currency, ESPMode::ThreadSafe> CurrencyPtr;
	static const TSharedPtr<Api::Wallet, ESPMode::ThreadSafe> WalletPtr;
	static const TSharedPtr<Api::Category, ESPMode::ThreadSafe> CategoryPtr;
	static const TSharedPtr<Api::Item, ESPMode::ThreadSafe> ItemPtr;
	static const TSharedPtr<Api::Order, ESPMode::ThreadSafe> OrderPtr;
	static const TSharedPtr<Api::Entitlement, ESPMode::ThreadSafe> EntitlementPtr;
	static const TSharedPtr<Api::Fulfillment, ESPMode::ThreadSafe> FulfillmentPtr;
	static const TSharedPtr<Api::StoreDisplay, ESPMode::ThreadSafe> StoreDisplayPtr;
#pragma endregion

#pragma region GameClientStorage
	static const TSharedPtr<Api::CloudStorage, ESPMode::ThreadSafe> CloudStoragePtr;
	static const TSharedPtr<Api::CloudSave, ESPMode::ThreadSafe> CloudSavePtr;
	static const TSharedPtr<Api::UGC, ESPMode::ThreadSafe> UGCPtr;
	static const TSharedPtr<Api::Inventory, ESPMode::ThreadSafe> InventoryPtr;
#pragma endregion

#pragma region GameClientEngagement
	static const TSharedPtr<Api::Statistic, ESPMode::ThreadSafe> StatisticPtr;
	static const TSharedPtr<Api::Achievement, ESPMode::ThreadSafe> AchievementPtr;
	static const TSharedPtr<Api::Leaderboard, ESPMode::ThreadSafe> LeaderboardPtr;
	static const TSharedPtr<Api::Reward, ESPMode::ThreadSafe> RewardPtr;
	static const TSharedPtr<Api::SeasonPass, ESPMode::ThreadSafe> SeasonPassPtr;
	static const TSharedPtr<Api::Group, ESPMode::ThreadSafe> GroupPtr;
	static const TSharedPtr<Api::Challenge, ESPMode::ThreadSafe> ChallengePtr;
#pragma endregion

#pragma region GameClientMultiplayer
	static const TSharedPtr<Api::QosManager, ESPMode::ThreadSafe> QosManagerPtr;
	static const TSharedPtr<Api::Qos, ESPMode::ThreadSafe> QosPtr;
	static const TSharedPtr<Api::Lobby, ESPMode::ThreadSafe> LobbyPtr;
	static const TSharedPtr<Api::Chat, ESPMode::ThreadSafe> ChatPtr;
	static const TSharedPtr<Api::SessionBrowser, ESPMode::ThreadSafe> SessionBrowserPtr;
	static const TSharedPtr<Api::TurnManager, ESPMode::ThreadSafe> TurnManagerPtr;
	static const TSharedPtr<Api::Session, ESPMode::ThreadSafe> SessionPtr;
	static const TSharedPtr<Api::MatchmakingV2, ESPMode::ThreadSafe> MatchmakingV2Ptr;
	static const TSharedPtr<Api::AMS, ESPMode::ThreadSafe> AMSPtr;
#pragma endregion

#pragma region GameClientAnalytics
	static const TSharedPtr<Api::GameTelemetry, ESPMode::ThreadSafe> GameTelemetryPtr;
	static const TSharedPtr<Api::PredefinedEvent, ESPMode::ThreadSafe> PredefinedEventPtr;
	static const TSharedPtr<Api::GameStandardEvent, ESPMode::ThreadSafe> GameStandardEventPtr;
#pragma endregion

#pragma region Game Client Access
	static Api::User& User;
	static Api::UserProfile& UserProfile;
	static Api::GameProfile& GameProfile;
	static Api::Agreement& Agreement;
	static Api::GDPR& GDPR;
	static Api::Reporting& Reporting;
	static Api::Miscellaneous& Miscellaneous;
	static Api::Configurations& Configurations;
	static Api::LoginQueue& LoginQueue;
#pragma endregion

#pragma region Game Client Commerce
	static Api::Currency& Currency;
	static Api::Wallet& Wallet;
	static Api::Category& Category;
	static Api::Item& Item;
	static Api::Order& Order;
	static Api::Entitlement& Entitlement;
	static Api::Fulfillment& Fulfillment;
	static Api::StoreDisplay& StoreDisplay;
#pragma endregion

#pragma region Game Client Storage
	static Api::CloudStorage& CloudStorage;
	static Api::CloudSave& CloudSave;
	static Api::UGC& UGC;
	static Api::Inventory& Inventory;
#pragma endregion

#pragma region Game Client Engagement
	static Api::Statistic& Statistic;
	static Api::Achievement& Achievement;
	static Api::Leaderboard& Leaderboard;
	static Api::Reward& Reward;
	static Api::SeasonPass& SeasonPass;
	static Api::Group& Group;
	static Api::Challenge& Challenge;
#pragma endregion

#pragma region Game Client Multiplayer
	static Api::QosManager& QosManager;
	static Api::Qos& Qos;
	static Api::Lobby& Lobby;
	static Api::Chat& Chat;
	static Api::SessionBrowser& SessionBrowser;
	static Api::TurnManager& TurnManager;
	static Api::Session& Session;
	static Api::MatchmakingV2& MatchmakingV2;
	static Api::AMS& AMS;
#pragma endregion

#pragma region Game Client Analytics
	static Api::GameTelemetry& GameTelemetry;
	static Api::PredefinedEvent& PredefinedEvent;
	static Api::GameStandardEvent& GameStandardEvent;
#pragma endregion

	static Api::HeartBeat HeartBeat;

#pragma region Dedicated Server Access
	static GameServerApi::ServerOauth2 ServerOauth2;
	static GameServerApi::ServerUser ServerUser;
#pragma endregion

#pragma region Dedicated Server Commerce
	static GameServerApi::ServerEcommerce ServerEcommerce;
#pragma endregion

#pragma region Dedicated Server Storage
	static GameServerApi::ServerCloudSave ServerCloudSave;
	static GameServerApi::ServerUGC ServerUGC;
#pragma endregion

#pragma region Dedicated Server Engagement
	static GameServerApi::ServerStatistic ServerStatistic;
	static GameServerApi::ServerAchievement ServerAchievement;
	static GameServerApi::ServerSeasonPass ServerSeasonPass;
	static GameServerApi::ServerChallenge ServerChallenge;
#pragma endregion

#pragma region Dedicated Server Multiplayer
	static GameServerApi::ServerLobby ServerLobby;
	static GameServerApi::ServerChat ServerChat;
	static GameServerApi::ServerSessionBrowser ServerSessionBrowser;
	static GameServerApi::ServerMatchmaking ServerMatchmaking;
	static GameServerApi::ServerDSHub ServerDSHub;
	static GameServerApi::ServerSession ServerSession;
	static GameServerApi::ServerMatchmakingV2 ServerMatchmakingV2;
#pragma endregion

#pragma region DedicatedServerAnalytics
	static GameServerApi::ServerGameTelemetry ServerGameTelemetry;
	static GameServerApi::ServerPredefinedEvent ServerPredefinedEvent;
	static GameServerApi::ServerGameStandardEvent ServerGameStandardEvent;
#pragma endregion

#pragma region Dedicated Server Management
	static GameServerApi::ServerDSM ServerDSM;
	static GameServerApi::ServerAMS ServerAMS;
	static GameServerApi::ServerMetricExporter ServerMetricExporter;
	static GameServerApi::ServerQosManager ServerQosManager;
#pragma endregion

	//Static class doesn't have constructors or destructor
	FRegistry() = delete;
	FRegistry(const FRegistry& other) = delete;
	FRegistry& operator=(const FRegistry& other) = delete;
	FRegistry(FRegistry&& other) = delete;
	FRegistry& operator=(FRegistry&& other) = delete;
	~FRegistry() = delete;
};

}
