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
#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteUserProfileApi.h"
#include "Api/AccelByteWalletApi.h"
#include "Api/AccelByteSessionApi.h"
#include "Api/AccelByteMatchmakingV2Api.h"
#include "Core/AccelByteApiBase.h"
#include "Api/AccelByteStoreDisplayApi.h"
#include "Api/AccelBytePredefinedEventApi.h"
#include "Api/AccelByteGameStandardEventApi.h"
#include "Core/AccelByteMessagingSystem.h"

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FApiClient final
{
public:
	FApiClient();
	FApiClient(AccelByte::Credentials& Credentials, AccelByte::FHttpRetryScheduler& Http);
	~FApiClient();

#pragma region Core
	bool bUseSharedCredentials;
	FAccelByteMessagingSystem MessagingSystem{};
	FAccelByteNotificationSender NotificationSender{MessagingSystem};
	FCredentialsRef CredentialsRef;
	FHttpRetrySchedulerRef HttpRef{};
	FAccelByteTimeManager TimeManager{ *HttpRef };
	FAccelByteNetworkConditioner NetworkConditioner{};
#pragma endregion

#pragma region Access
	Api::User User{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::UserProfile UserProfile{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::GameProfile GameProfile{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Agreement Agreement{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::GDPR GDPR{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Reporting Reporting{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Miscellaneous Miscellaneous{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Configurations Configurations{*CredentialsRef, FRegistry::Settings, *HttpRef};
#pragma endregion

#pragma region Commerce
	Api::Currency Currency{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Wallet Wallet{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Category Category{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Item Item{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Order Order{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Entitlement Entitlement{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Fulfillment Fulfillment{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::StoreDisplay StoreDisplay{ *CredentialsRef, FRegistry::Settings, *HttpRef };
#pragma endregion

#pragma region Storage
	Api::CloudStorage CloudStorage{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::CloudSave CloudSave{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::BinaryCloudSave BinaryCloudSave{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::UGC UGC{ *CredentialsRef, FRegistry::Settings, *HttpRef };
#pragma endregion

#pragma region Engagement
	Api::Statistic Statistic{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Achievement Achievement{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Leaderboard Leaderboard{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Reward Reward{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::SeasonPass SeasonPass{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Group Group{ *CredentialsRef, FRegistry::Settings, *HttpRef };
#pragma endregion

#pragma region Multiplayer
	Api::QosManager QosManager{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Qos Qos{ *CredentialsRef, FRegistry::Settings, MessagingSystem };
	Api::Lobby Lobby{ *CredentialsRef, FRegistry::Settings, *HttpRef, MessagingSystem, NetworkConditioner };
	Api::Chat Chat{ *CredentialsRef, FRegistry::Settings, *HttpRef, MessagingSystem, NetworkConditioner};
	Api::SessionBrowser SessionBrowser{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::TurnManager TurnManager{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::Session Session{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::MatchmakingV2 MatchmakingV2{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::AMS Ams{ *CredentialsRef, FRegistry::Settings, *HttpRef };
#pragma endregion

#pragma region Analytics
	Api::GameTelemetry GameTelemetry{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::PresenceBroadcastEvent PresenceBroadcastEvent{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::PredefinedEvent PredefinedEvent{ *CredentialsRef, FRegistry::Settings, *HttpRef };
	Api::GameStandardEvent GameStandardEvent{ *CredentialsRef, FRegistry::Settings, *HttpRef };
#pragma endregion

	template<typename T, typename... U>
	T GetApi(U&&... Args)
	{
		static_assert(std::is_base_of<FApiBase, T>::value, "API class must be subclass of FApiBase");

		return T(*CredentialsRef, FRegistry::Settings, *HttpRef, Forward<U>(Args)...);
	}

	template<typename T, typename... U>
	TSharedPtr<T, ESPMode::ThreadSafe> GetApiPtr(U&&... Args)
	{
		static_assert(std::is_base_of<FApiBase, T>::value, "API class must be subclass of FApiBase");

		return MakeShared<T, ESPMode::ThreadSafe>(
			*CredentialsRef, FRegistry::Settings,
			*HttpRef, Forward<U>(Args)...);
	}
};

typedef TSharedRef<FApiClient, ESPMode::ThreadSafe> FApiClientRef;
typedef TSharedPtr<FApiClient, ESPMode::ThreadSafe> FApiClientPtr;

}
