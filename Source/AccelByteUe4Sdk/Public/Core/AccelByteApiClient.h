// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteNetworkConditioner.h"
#include "Core/AccelByteNotificationSender.h"
#include "Core/ServerTime/AccelByteTimeManager.h"
#include "Api/AccelByteAchievementApi.h"
#include "Api/AccelByteAgreementApi.h"
#include "Api/AccelByteAMSApi.h"
#include "Api/AccelByteBinaryCloudSaveApi.h"
#include "Api/AccelByteCategoryApi.h"
#include "Api/AccelByteChatApi.h"
#include "Api/AccelByteCloudSaveApi.h"
#include "Api/AccelByteCloudStorageApi.h"
#include "Api/AccelByteConfigurationsApi.h"
#include "Api/AccelByteCurrencyApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteFulfillmentApi.h"
#include "Api/AccelByteGameProfileApi.h"
#include "Api/AccelByteGameTelemetryApi.h"
#include "Api/AccelByteGDPRApi.h"
#include "Api/AccelByteGroupApi.h"
#include "Api/AccelByteItemApi.h"
#include "Api/AccelByteLeaderboardApi.h"
#include "Api/AccelByteLobbyApi.h"
#include "Api/AccelByteMiscellaneousApi.h"
#include "Api/AccelByteOrderApi.h"
#include "Api/AccelBytePresenceBroadcastEventApi.h"
#include "Api/AccelByteQos.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Api/AccelByteReportingApi.h"
#include "Api/AccelByteRewardApi.h"
#include "Api/AccelByteSeasonPassApi.h"
#include "Api/AccelByteSessionBrowserApi.h"
#include "Api/AccelByteStatisticApi.h"
#include "Api/AccelByteTurnManagerApi.h"
#include "Api/AccelByteUGCApi.h"
#include "Api/AccelByteInventoryApi.h"
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteUserProfileApi.h"
#include "Api/AccelByteWalletApi.h"
#include "Api/AccelByteSessionApi.h"
#include "Api/AccelByteMatchmakingV2Api.h"
#include "Core/AccelByteApiBase.h"
#include "Api/AccelByteStoreDisplayApi.h"
#include "Api/AccelBytePredefinedEventApi.h"
#include "Api/AccelByteGameStandardEventApi.h"
#include "Api/AccelByteLoginQueueApi.h"
#include "Core/AccelByteMessagingSystem.h"
#include "Api/AccelByteChallengeApi.h"
#include "Api/AccelByteSessionHistoryApi.h"

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FApiClient final
{
public:
	FApiClient();
	FApiClient(const SettingsPtr& InSettings, FAccelByteTimeManagerPtr InTimeManager);
	FApiClient(AccelByte::Credentials& Credentials
		, AccelByte::FHttpRetryScheduler& Http
		, AccelByte::FAccelByteMessagingSystemPtr InMessagingSystemPtr = nullptr);
	~FApiClient();

#pragma region Core
	bool bUseSharedCredentials;
	FAccelByteMessagingSystemPtr MessagingSystem{};
	FCredentialsRef CredentialsRef;
	FHttpRetrySchedulerRef HttpRef{};
	FAccelByteNotificationSender NotificationSender{ *MessagingSystem.Get() };
	FAccelByteNetworkConditioner NetworkConditioner{};
	SettingsPtr Settings{};
#pragma endregion

private:
	const FAccelByteTimeManagerPtr TimeManagerPtr;
#pragma region Access
	const AccelByte::Api::UserPtr UserPtr;
	const AccelByte::Api::UserProfilePtr UserProfilePtr;
	const AccelByte::Api::GameProfilePtr GameProfilePtr;
	const AccelByte::Api::AgreementPtr AgreementPtr;
	const AccelByte::Api::GDPRPtr GDPRPtr;
	const AccelByte::Api::ReportingPtr ReportingPtr;
	const AccelByte::Api::MiscellaneousPtr MiscellaneousPtr;
	const AccelByte::Api::ConfigurationsPtr ConfigurationsPtr;
	const AccelByte::Api::LoginQueuePtr LoginQueuePtr;
#pragma endregion

#pragma region Commerce
	const AccelByte::Api::CurrencyPtr CurrencyPtr;
	const AccelByte::Api::WalletPtr WalletPtr;
	const AccelByte::Api::CategoryPtr CategoryPtr;
	const AccelByte::Api::ItemPtr ItemPtr;
	const AccelByte::Api::OrderPtr OrderPtr;
	const AccelByte::Api::EntitlementPtr EntitlementPtr;
	const AccelByte::Api::FulfillmentPtr FulfillmentPtr;
	const AccelByte::Api::StoreDisplayPtr StoreDisplayPtr;
#pragma endregion

#pragma region Storage
	const AccelByte::Api::CloudStoragePtr CloudStoragePtr;
	const AccelByte::Api::CloudSavePtr CloudSavePtr;
	const AccelByte::Api::BinaryCloudSavePtr BinaryCloudSavePtr;
	const AccelByte::Api::UGCPtr UGCPtr;
	const AccelByte::Api::InventoryPtr InventoryPtr;
#pragma endregion

#pragma region Engagement
	const AccelByte::Api::StatisticPtr StatisticPtr;
	const AccelByte::Api::AchievementPtr AchievementPtr;
	const AccelByte::Api::LeaderboardPtr LeaderboardPtr;
	const AccelByte::Api::RewardPtr RewardPtr;
	const AccelByte::Api::SeasonPassPtr SeasonPassPtr;
	const AccelByte::Api::GroupPtr GroupPtr;
	const AccelByte::Api::ChallengePtr ChallengePtr;
#pragma endregion

#pragma region Multiplayer
	const AccelByte::Api::QosPtr QosPtr;
	const AccelByte::Api::QosManagerPtr QosManagerPtr;
	const AccelByte::Api::LobbyPtr LobbyPtr;
	const AccelByte::Api::ChatPtr ChatPtr;
	const AccelByte::Api::SessionBrowserPtr SessionBrowserPtr;
	const AccelByte::Api::TurnManagerPtr TurnManagerPtr;
	const AccelByte::Api::SessionPtr SessionPtr;
	const AccelByte::Api::SessionHistoryPtr SessionHistoryPtr;
	const AccelByte::Api::MatchmakingV2Ptr MatchmakingV2Ptr;
	const AccelByte::Api::AMSPtr AmsPtr;
#pragma endregion

#pragma region Analytics
	const AccelByte::Api::GameTelemetryPtr GameTelemetryPtr;
	const AccelByte::Api::PredefinedEventPtr PredefinedEventPtr;
	const AccelByte::Api::GameStandardEventPtr GameStandardEventPtr;
	const AccelByte::Api::PresenceBroadcastEventPtr PresenceBroadcastEventPtr;
#pragma endregion

public:
	/** 
	 * @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	 * THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	 * It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->GameTelemetry with ApiClient->GetGameTelemetryApi().Pin()
	 */
	FAccelByteTimeManager& TimeManager;
	
#pragma region Access
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->User with ApiClient->GetUserApi().Pin()
	*/
	Api::User& User;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->UserProfile with ApiClient->GetUserProfileApi().Pin()
	*/
	Api::UserProfile& UserProfile;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->GameProfile with ApiClient->GetGameProfileApi().Pin()
	*/
	Api::GameProfile& GameProfile;

	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Agreement with ApiClient->GetAgreementApi().Pin()
	*/
	Api::Agreement& Agreement;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->GDPR with ApiClient->GetGDPRApi().Pin()
	*/
	Api::GDPR& GDPR;

	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Reporting with ApiClient->GetReportingApi().Pin()
	*/
	Api::Reporting& Reporting;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Miscellaneous with ApiClient->GetMiscellaneousApi().Pin()
	*/
	Api::Miscellaneous& Miscellaneous;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Configurations with ApiClient->GetConfigurationsApi().Pin()
	*/
	Api::Configurations& Configurations;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->LoginQueue with ApiClient->GetLoginQueueApi().Pin()
	*/
	Api::LoginQueue& LoginQueue;
#pragma endregion

#pragma region Commerce
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Currency with ApiClient->GetCurrencyApi().Pin()
	*/
	Api::Currency& Currency;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Wallet with ApiClient->GetWalletApi().Pin()
	*/
	Api::Wallet& Wallet;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Category with ApiClient->GetCategoryApi().Pin()
	*/
	Api::Category& Category;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Item with ApiClient->GetItemApi().Pin()
	*/
	Api::Item& Item;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Order with ApiClient->GetOrderApi().Pin()
	*/
	Api::Order& Order;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Entitlement with ApiClient->GetEntitlementApi().Pin()
	*/
	Api::Entitlement& Entitlement;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Fulfillment with ApiClient->GetFulfillmentApi().Pin()
	*/
	Api::Fulfillment& Fulfillment;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->StoreDisplay with ApiClient->GetStoreDisplayApi().Pin()
	*/
	Api::StoreDisplay& StoreDisplay;
#pragma endregion

#pragma region Storage
	/**
	* @brief DEPRECATED. CloudStorage is deprecated please use BinaryCloudSave instead.
	*/
	Api::CloudStorage& CloudStorage;

	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->CloudSave with ApiClient->GetCloudSaveApi().Pin()
	*/
	Api::CloudSave& CloudSave;

	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->BinaryCloudSave with ApiClient->GetBinaryCloudSaveApi().Pin()
	*/
	Api::BinaryCloudSave& BinaryCloudSave;

	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->UGC with ApiClient->GetUGCApi().Pin()
	*/
	Api::UGC& UGC;

	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Inventory with ApiClient->GetInventoryApi().Pin()
	*/
	Api::Inventory& Inventory;
#pragma endregion

#pragma region Engagement
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Statistic with ApiClient->GetStatisticApi().Pin()
	*/
	Api::Statistic& Statistic;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Achievement with ApiClient->GetAchievementApi().Pin()
	*/
	Api::Achievement& Achievement;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Leaderboard with ApiClient->GetLeaderboardApi().Pin()
	*/
	Api::Leaderboard& Leaderboard;
	
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Reward with ApiClient->GetRewardApi().Pin()
	*/
	Api::Reward& Reward;

	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->SeasonPass with ApiClient->GetSeasonPassApi().Pin()
	*/
	Api::SeasonPass& SeasonPass;

	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Group with ApiClient->GetGroupApi().Pin()
	*/
	Api::Group& Group;

	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Challenge with ApiClient->GetChallengeApi().Pin()
	*/
	Api::Challenge& Challenge;
#pragma endregion

#pragma region Multiplayer
	/** 
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Qos with ApiClient->GetQosApi().Pin()
	*/
	Api::Qos& Qos;

	/**
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->QosManager with ApiClient->GetQosManagerApi().Pin()
	*/
	Api::QosManager& QosManager;

	/**
	 * @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	 * THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	 * It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Lobby with ApiClient->GetLobbyApi().Pin()
	 */
	Api::Lobby& Lobby;

	/**
	 * @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	 * THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	 * It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Chat with ApiClient->GetChatApi().Pin()
	 */
	Api::Chat& Chat;

	/**
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->SessionBrowser with ApiClient->GetSessionBrowserApi().Pin()
	*/
	Api::SessionBrowser& SessionBrowser;
	
	/**
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->TurnManager with ApiClient->GetTurnManagerApi().Pin()
	*/
	Api::TurnManager& TurnManager;
	
	/**
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Session with ApiClient->GetSessionApi().Pin()
	*/
	Api::Session& Session;
	
	/**
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->SessionHistory with ApiClient->GetSessionHistoryApi().Pin()
	*/
	Api::SessionHistory& SessionHistory;
	
	/**
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->MatchmakingV2 with ApiClient->GetMatchmakingV2Api().Pin()
	*/
	Api::MatchmakingV2& MatchmakingV2;
	
	/**
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->Ams with ApiClient->GetAmsApi().Pin()
	*/
	Api::AMS& Ams;
#pragma endregion

#pragma region Analytics
	/** 
	 * @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	 * THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	 * It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->GameTelemetry with ApiClient->GetGameTelemetryApi().Pin()
	 */
	Api::GameTelemetry& GameTelemetry;

	/**
	 * @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	 * THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	 * It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->PredefinedEvent with ApiClient->GetPredefinedEventApi().Pin()
	 */
	Api::PredefinedEvent& PredefinedEvent;

	/**
	 * @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	 * THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	 * It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->GameStandardEvent with ApiClient->GetGameStandardEventApi().Pin()
	 */
	Api::GameStandardEvent& GameStandardEvent;

	/**
	* @brief For continuity, this temporary member is included to avoid any build breaks introduced via improvements to the AccelByte Unreal SDK.
	* THIS MEMBER IS MARKED FOR DEPRECATION AND WILL BE REMOVED IN A FUTURE RELEASE.
	* It is recommended that you update your code as soon as possible, and replace any usage of ApiClient->PresenceBroadcastEvent with ApiClient->GetPresenceBroadcastEventApi().Pin()
	*/
	Api::PresenceBroadcastEvent& PresenceBroadcastEvent;
#pragma endregion

#pragma region Access
	Api::UserWPtr GetUserApi() const;
	Api::UserProfileWPtr GetUserProfileApi() const;
	Api::GameProfileWPtr GetGameProfileApi() const;
	Api::AgreementWPtr GetAgreementApi() const;
	Api::GDPRWPtr GetGDPRApi() const;
	Api::ReportingWPtr GetReportingApi() const;
	Api::MiscellaneousWPtr GetMiscellaneousApi() const;
	Api::ConfigurationsWPtr GetConfigurationsApi() const;
	Api::LoginQueueWPtr GetLoginQueueApi() const;
#pragma endregion

#pragma region Commerce
	Api::CurrencyWPtr GetCurrencyApi() const;
	Api::WalletWPtr GetWalletApi() const;
	Api::CategoryWPtr GetCategoryApi() const;
	Api::ItemWPtr GetItemApi() const;
	Api::OrderWPtr GetOrderApi() const;
	Api::EntitlementWPtr GetEntitlementApi() const;
	Api::FulfillmentWPtr GetFulfillmentApi() const;
	Api::StoreDisplayWPtr GetStoreDisplayApi() const;
#pragma endregion

#pragma region Storage
	Api::CloudSaveWPtr GetCloudSaveApi() const;
	Api::BinaryCloudSaveWPtr GetBinaryCloudSaveApi() const;
	Api::UGCWPtr GetUGCApi() const;
	Api::InventoryWPtr GetInventoryApi() const;
#pragma endregion

#pragma region Engagement
	Api::StatisticWPtr GetStatisticApi() const;
	Api::AchievementWPtr GetAchievementApi() const;
	Api::LeaderboardWPtr GetLeaderboardApi() const;
	Api::RewardWPtr GetRewardApi() const;
	Api::SeasonPassWPtr GetSeasonPassApi() const;
	Api::GroupWPtr GetGroupApi() const;
	Api::ChallengeWPtr GetChallengeApi() const;
#pragma endregion

#pragma region Multiplayer
	Api::QosWPtr GetQosApi() const;
	Api::QosManagerWPtr GetQosManagerApi() const;
	Api::LobbyWPtr GetLobbyApi() const;
	Api::ChatWPtr GetChatApi() const;
	Api::SessionBrowserWPtr GetSessionBrowserApi() const;
	Api::TurnManagerWPtr GetTurnManagerApi() const;
	Api::SessionWPtr GetSessionApi() const;
	Api::SessionHistoryWPtr GetSessionHistoryApi() const;
	Api::MatchmakingV2WPtr GetMatchmakingV2Api() const;
	Api::AMSWPtr GetAmsApi() const;
#pragma endregion

#pragma region Analytics
	Api::GameTelemetryWPtr GetGameTelemetryApi() const;
	Api::PredefinedEventWPtr GetPredefinedEventApi() const;
	Api::GameStandardEventWPtr GetGameStandardEventApi() const;
	Api::PresenceBroadcastEventWPtr GetPresenceBroadcastEventApi() const;
#pragma endregion

	FAccelByteTimeManagerWPtr GetTimeManager() const;

	template<typename T, typename... U>
	T GetApi(U&&... Args)
	{
		static_assert(std::is_base_of<FApiBase, T>::value, "API class must be subclass of FApiBase");

		return T(*CredentialsRef, *Settings, *HttpRef, Forward<U>(Args)...);
	}

	template<typename T, typename... U>
	TSharedPtr<T, ESPMode::ThreadSafe> GetApiPtr(U&&... Args)
	{
		static_assert(std::is_base_of<FApiBase, T>::value, "API class must be subclass of FApiBase");

		return MakeShared<T, ESPMode::ThreadSafe>(
			*CredentialsRef, *Settings,
			*HttpRef, Forward<U>(Args)...);
	}
};

typedef TSharedRef<FApiClient, ESPMode::ThreadSafe> FApiClientRef;
typedef TSharedPtr<FApiClient, ESPMode::ThreadSafe> FApiClientPtr;
typedef TWeakPtr<FApiClient, ESPMode::ThreadSafe> FApiClientWPtr;

}
