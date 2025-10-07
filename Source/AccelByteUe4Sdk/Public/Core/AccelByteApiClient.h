// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteApiUtilities.h"
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

class FAccelByteInstance;

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FApiClient final : public TSharedFromThis<FApiClient, ESPMode::ThreadSafe>
{
public:
	FApiClient();
	FApiClient(const SettingsPtr& InSettings, FAccelByteTimeManagerPtr InTimeManager, TSharedRef<FAccelByteInstance, ESPMode::ThreadSafe> InAccelByteInstance);
	FApiClient(AccelByte::Credentials& Credentials
		, AccelByte::FHttpRetryScheduler& Http
		, AccelByte::FAccelByteMessagingSystemPtr InMessagingSystemPtr = nullptr);
	~FApiClient();
	
	void Init();

#pragma region Core
	bool bUseSharedCredentials;
	TWeakPtr<FAccelByteInstance, ESPMode::ThreadSafe> AccelByteInstanceWeak;
	FAccelByteMessagingSystemPtr MessagingSystem{};
	FHttpRetrySchedulerRef HttpRef{MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>()};
	SettingsPtr Settings{};
	FCredentialsRef CredentialsRef;
	FAccelByteNotificationSender NotificationSender{ *MessagingSystem.Get() };
	FAccelByteNetworkConditioner NetworkConditioner{};
#pragma endregion

private:
	const FAccelByteTimeManagerPtr TimeManagerPtr;

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
	const AccelByte::Api::QosManagerPtr QosManagerPtr;
	const AccelByte::Api::QosPtr QosPtr;
	const AccelByte::Api::LobbyPtr LobbyPtr;
	const AccelByte::Api::ChatPtr ChatPtr;
#if 1 // MMv1 Deprecation
	const AccelByte::Api::SessionBrowserPtr SessionBrowserPtr;
#endif
	const AccelByte::Api::TurnManagerPtr TurnManagerPtr;
	const AccelByte::Api::SessionPtr SessionPtr;
	const AccelByte::Api::MatchmakingV2Ptr MatchmakingV2Ptr;
	const AccelByte::Api::AMSPtr AmsPtr;
#pragma endregion

#pragma region Analytics
	const AccelByte::Api::GameTelemetryPtr GameTelemetryPtr;
	const AccelByte::Api::PredefinedEventPtr PredefinedEventPtr;
	const AccelByte::Api::GameStandardEventPtr GameStandardEventPtr;
	const AccelByte::Api::PresenceBroadcastEventPtr PresenceBroadcastEventPtr;
#pragma endregion

	FAccelByteApiUtilitiesPtr ApiUtilitiesPtr;

public:
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
	Api::CloudStorageWPtr GetCloudStorageApi() const;
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
#if 1 // MMv1 Deprecation
	Api::SessionBrowserWPtr GetSessionBrowserApi() const;
#endif
	Api::TurnManagerWPtr GetTurnManagerApi() const;
	Api::SessionWPtr GetSessionApi() const;
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

	FAccelByteApiUtilitiesWPtr GetApiUtilities() const;

	/**
	 * @brief Utility function to get the device ID associated with this ApiClient's AccelByteInstance
	 * @return device ID string, may be empty if AccelByteInstance is invalid or Device ID not found
	 */
	FString GetDeviceId() const;

	template<typename T, typename... U>
	T GetApi(U&&... Args)
	{
		static_assert(std::is_base_of<FApiBase, T>::value, "API class must be subclass of FApiBase");

		return T(*CredentialsRef, *Settings, *HttpRef, AsShared(), Forward<U>(Args)...);
	}

	template<typename T, typename... U>
	TSharedPtr<T, ESPMode::ThreadSafe> GetApiPtr(U&&... Args)
	{
		static_assert(std::is_base_of<FApiBase, T>::value, "API class must be subclass of FApiBase");

		return MakeShared<T, ESPMode::ThreadSafe>(
			*CredentialsRef, *Settings,
			*HttpRef, AsShared(), Forward<U>(Args)...);
	}
};

typedef TSharedRef<FApiClient, ESPMode::ThreadSafe> FApiClientRef;
typedef TSharedPtr<FApiClient, ESPMode::ThreadSafe> FApiClientPtr;
typedef TWeakPtr<FApiClient, ESPMode::ThreadSafe> FApiClientWPtr;

}
