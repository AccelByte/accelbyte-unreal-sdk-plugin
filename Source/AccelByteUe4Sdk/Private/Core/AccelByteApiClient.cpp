// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteApiClient.h"

#include "Core/AccelByteApiUtilities.h"
#include "Core/AccelByteInstance.h"

namespace AccelByte
{

FApiClient::FApiClient(const SettingsPtr& InSettings, FAccelByteTimeManagerPtr InTimeManager, TSharedRef<FAccelByteInstance, ESPMode::ThreadSafe> InAccelByteInstance)
	: bUseSharedCredentials(false)
	, AccelByteInstanceWeak(InAccelByteInstance)
	, MessagingSystem(MakeShared<FAccelByteMessagingSystem, ESPMode::ThreadSafe>())
	, HttpRef(MakeShared<AccelByte::FHttpRetryScheduler, ESPMode::ThreadSafe>())
	, Settings(InSettings)
	, CredentialsRef(MakeShared<AccelByte::Credentials, ESPMode::ThreadSafe>(*HttpRef, *MessagingSystem.Get(), InSettings->IamServerUrl))
	, TimeManagerPtr(InTimeManager)
	, CurrencyPtr(MakeShared<AccelByte::Api::Currency, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, WalletPtr(MakeShared<AccelByte::Api::Wallet, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, CategoryPtr(MakeShared<AccelByte::Api::Category, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, ItemPtr(MakeShared<AccelByte::Api::Item, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, OrderPtr(MakeShared<AccelByte::Api::Order, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, EntitlementPtr(MakeShared<AccelByte::Api::Entitlement, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, FulfillmentPtr(MakeShared<AccelByte::Api::Fulfillment, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, StoreDisplayPtr(MakeShared<AccelByte::Api::StoreDisplay, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, UserPtr(MakeShared<AccelByte::Api::User, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef, EntitlementPtr, ItemPtr))
	, UserProfilePtr(MakeShared<AccelByte::Api::UserProfile, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, GameProfilePtr(MakeShared<AccelByte::Api::GameProfile, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, AgreementPtr(MakeShared<AccelByte::Api::Agreement, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, GDPRPtr(MakeShared<AccelByte::Api::GDPR, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, ReportingPtr(MakeShared<AccelByte::Api::Reporting, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, MiscellaneousPtr(MakeShared<AccelByte::Api::Miscellaneous, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, ConfigurationsPtr(MakeShared<AccelByte::Api::Configurations, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, LoginQueuePtr(MakeShared<AccelByte::Api::LoginQueue, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, CloudStoragePtr(MakeShared<AccelByte::Api::CloudStorage, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, CloudSavePtr(MakeShared<AccelByte::Api::CloudSave, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, BinaryCloudSavePtr(MakeShared<AccelByte::Api::BinaryCloudSave, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, UGCPtr(MakeShared<AccelByte::Api::UGC, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, InventoryPtr(MakeShared<AccelByte::Api::Inventory, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, StatisticPtr(MakeShared<AccelByte::Api::Statistic, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, AchievementPtr(MakeShared<AccelByte::Api::Achievement, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, LeaderboardPtr(MakeShared<AccelByte::Api::Leaderboard, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, RewardPtr(MakeShared<AccelByte::Api::Reward, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, SeasonPassPtr(MakeShared<AccelByte::Api::SeasonPass, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, GroupPtr(MakeShared<AccelByte::Api::Group, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, ChallengePtr(MakeShared<AccelByte::Api::Challenge, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, QosManagerPtr(MakeShared<AccelByte::Api::QosManager, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, QosPtr(MakeShared<Api::Qos, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *MessagingSystem.Get(), QosManagerPtr))
	, LobbyPtr(MakeShared<AccelByte::Api::Lobby, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef, *MessagingSystem.Get(), NetworkConditioner))
	, ChatPtr(MakeShared<AccelByte::Api::Chat, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef, *MessagingSystem.Get(), NetworkConditioner))
	, SessionBrowserPtr(MakeShared<AccelByte::Api::SessionBrowser, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, TurnManagerPtr(MakeShared<AccelByte::Api::TurnManager, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, SessionPtr(MakeShared<AccelByte::Api::Session, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, MatchmakingV2Ptr(MakeShared<AccelByte::Api::MatchmakingV2, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, AmsPtr(MakeShared<AccelByte::Api::AMS, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, GameTelemetryPtr(MakeShared<AccelByte::Api::GameTelemetry, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, PredefinedEventPtr( MakeShared<AccelByte::Api::PredefinedEvent, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, GameStandardEventPtr(MakeShared<AccelByte::Api::GameStandardEvent, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, PresenceBroadcastEventPtr(MakeShared<AccelByte::Api::PresenceBroadcastEvent, ESPMode::ThreadSafe>(*CredentialsRef, *Settings, *HttpRef))
	, ApiUtilitiesPtr(MakeShared<FAccelByteApiUtilities, ESPMode::ThreadSafe>(CredentialsRef, Settings.ToSharedRef(), HttpRef, TimeManagerPtr))
{
	HttpRef->Startup();
	CredentialsRef->Startup();
	QosPtr->Startup();
	LobbyPtr->Startup();
	GameTelemetryPtr->Startup();
	PredefinedEventPtr->Startup();
	GameStandardEventPtr->Startup();
	PresenceBroadcastEventPtr->Startup();
}

FApiClient::~FApiClient()
{
	GameTelemetryPtr->Shutdown();
	PredefinedEventPtr->Shutdown();
	GameStandardEventPtr->Shutdown();
	PresenceBroadcastEventPtr->Shutdown();
	MessagingSystem.Reset();
	
	if (!bUseSharedCredentials)
	{
		const bool bShouldExecuteImmediately = IsInGameThread() || !FTaskGraphInterface::IsRunning();
		if (bShouldExecuteImmediately)
		{
			CredentialsRef->Shutdown();
			HttpRef->Shutdown();
		}
		else
		{
			FFunctionGraphTask::CreateAndDispatchWhenReady(
					[Credentials = CredentialsRef, HttpScheduler = HttpRef]()
					{
						Credentials->Shutdown();
						HttpScheduler->Shutdown();
					}
				, TStatId()
				, nullptr
				, ENamedThreads::GameThread);
		}
	}
}

void FApiClient::Init()
{
	CurrencyPtr->SetApiClient(AsShared());
	WalletPtr->SetApiClient(AsShared());
	CategoryPtr->SetApiClient(AsShared());
	ItemPtr->SetApiClient(AsShared());
	OrderPtr->SetApiClient(AsShared());
	EntitlementPtr->SetApiClient(AsShared());
	FulfillmentPtr->SetApiClient(AsShared());
	StoreDisplayPtr->SetApiClient(AsShared());
	UserPtr->SetApiClient(AsShared());
	UserProfilePtr->SetApiClient(AsShared());
	GameProfilePtr->SetApiClient(AsShared());
	AgreementPtr->SetApiClient(AsShared());
	GDPRPtr->SetApiClient(AsShared());
	ReportingPtr->SetApiClient(AsShared());
	MiscellaneousPtr->SetApiClient(AsShared());
	ConfigurationsPtr->SetApiClient(AsShared());
	LoginQueuePtr->SetApiClient(AsShared());
	CloudStoragePtr->SetApiClient(AsShared());
	CloudSavePtr->SetApiClient(AsShared());
	BinaryCloudSavePtr->SetApiClient(AsShared());
	UGCPtr->SetApiClient(AsShared());
	InventoryPtr->SetApiClient(AsShared());
	StatisticPtr->SetApiClient(AsShared());
	AchievementPtr->SetApiClient(AsShared());
	LeaderboardPtr->SetApiClient(AsShared());
	RewardPtr->SetApiClient(AsShared());
	SeasonPassPtr->SetApiClient(AsShared());
	GroupPtr->SetApiClient(AsShared());
	ChallengePtr->SetApiClient(AsShared());
	QosManagerPtr->SetApiClient(AsShared());
	QosPtr->SetApiClient(AsShared());
	LobbyPtr->SetApiClient(AsShared());
	ChatPtr->SetApiClient(AsShared());
	SessionBrowserPtr->SetApiClient(AsShared());
	TurnManagerPtr->SetApiClient(AsShared());
	SessionPtr->SetApiClient(AsShared());
	MatchmakingV2Ptr->SetApiClient(AsShared());
	AmsPtr->SetApiClient(AsShared());
	GameTelemetryPtr->SetApiClient(AsShared());
	PredefinedEventPtr->SetApiClient(AsShared());
	GameStandardEventPtr->SetApiClient(AsShared());
	PresenceBroadcastEventPtr->SetApiClient(AsShared());
}

#pragma region Access
Api::UserWPtr FApiClient::GetUserApi() const
{
	return UserPtr;
}

Api::UserProfileWPtr FApiClient::GetUserProfileApi() const
{
	return UserProfilePtr;
}

Api::GameProfileWPtr FApiClient::GetGameProfileApi() const
{
	return GameProfilePtr;
}

Api::AgreementWPtr FApiClient::GetAgreementApi() const
{
	return AgreementPtr;
}

Api::GDPRWPtr FApiClient::GetGDPRApi() const
{
	return GDPRPtr;
}

Api::ReportingWPtr FApiClient::GetReportingApi() const
{
	return ReportingPtr;
}

Api::MiscellaneousWPtr FApiClient::GetMiscellaneousApi() const
{
	return MiscellaneousPtr;
}

Api::ConfigurationsWPtr FApiClient::GetConfigurationsApi() const
{
	return ConfigurationsPtr;
}

Api::LoginQueueWPtr FApiClient::GetLoginQueueApi() const
{
	return LoginQueuePtr;
}
#pragma endregion

#pragma region Multiplayer
Api::LobbyWPtr FApiClient::GetLobbyApi() const
{
	return LobbyPtr;
}

Api::ChatWPtr FApiClient::GetChatApi() const
{
	return ChatPtr;
}

TWeakPtr<Api::Qos, ESPMode::ThreadSafe> FApiClient::GetQosApi() const
{
	return QosPtr;
}

Api::QosManagerWPtr FApiClient::GetQosManagerApi() const
{
	return QosManagerPtr;
}

Api::SessionBrowserWPtr FApiClient::GetSessionBrowserApi() const
{
	return SessionBrowserPtr;
}

Api::TurnManagerWPtr FApiClient::GetTurnManagerApi() const
{
	return TurnManagerPtr;
}

Api::SessionWPtr FApiClient::GetSessionApi() const
{
	return SessionPtr;
}

Api::MatchmakingV2WPtr FApiClient::GetMatchmakingV2Api() const
{
	return MatchmakingV2Ptr;
}

Api::AMSWPtr FApiClient::GetAmsApi() const
{
	return AmsPtr;
}
#pragma endregion

#pragma region Analytics
Api::GameTelemetryWPtr FApiClient::GetGameTelemetryApi() const
{
	return GameTelemetryPtr;
}

Api::PredefinedEventWPtr FApiClient::GetPredefinedEventApi() const
{
	return PredefinedEventPtr;
}

Api::GameStandardEventWPtr FApiClient::GetGameStandardEventApi() const
{
	return GameStandardEventPtr;
}

Api::PresenceBroadcastEventWPtr FApiClient::GetPresenceBroadcastEventApi() const
{
	return PresenceBroadcastEventPtr;
}
#pragma endregion

#pragma region Commerces
Api::CurrencyWPtr FApiClient::GetCurrencyApi() const
{
	return CurrencyPtr;
}

Api::WalletWPtr FApiClient::GetWalletApi() const
{
	return WalletPtr;
}

Api::CategoryWPtr FApiClient::GetCategoryApi() const
{
	return CategoryPtr;
}

Api::ItemWPtr FApiClient::GetItemApi() const
{
	return ItemPtr;
}

Api::OrderWPtr FApiClient::GetOrderApi() const
{
	return OrderPtr;
}

Api::EntitlementWPtr FApiClient::GetEntitlementApi() const
{
	return EntitlementPtr;
}

Api::FulfillmentWPtr FApiClient::GetFulfillmentApi() const
{
	return FulfillmentPtr;
}

Api::StoreDisplayWPtr FApiClient::GetStoreDisplayApi() const
{
	return StoreDisplayPtr;
}
#pragma endregion

#pragma region Storage
Api::CloudSaveWPtr FApiClient::GetCloudSaveApi() const
{
	return CloudSavePtr;
}

Api::BinaryCloudSaveWPtr FApiClient::GetBinaryCloudSaveApi() const
{
	return BinaryCloudSavePtr;
}

Api::UGCWPtr FApiClient::GetUGCApi() const
{
	return UGCPtr;
}

Api::CloudStorageWPtr FApiClient::GetCloudStorageApi() const
{
	return CloudStoragePtr;
}

Api::InventoryWPtr FApiClient::GetInventoryApi() const
{
	return InventoryPtr;
}
#pragma endregion

#pragma region Engagement
Api::StatisticWPtr FApiClient::GetStatisticApi() const
{
	return StatisticPtr;
}

Api::AchievementWPtr FApiClient::GetAchievementApi() const
{
	return AchievementPtr;
}

Api::LeaderboardWPtr FApiClient::GetLeaderboardApi() const
{
	return LeaderboardPtr;
}

Api::RewardWPtr FApiClient::GetRewardApi() const
{
	return RewardPtr;
}

Api::SeasonPassWPtr FApiClient::GetSeasonPassApi() const
{
	return SeasonPassPtr;
}

Api::GroupWPtr FApiClient::GetGroupApi() const
{
	return GroupPtr;
}

Api::ChallengeWPtr FApiClient::GetChallengeApi() const
{
	return ChallengePtr;
}
#pragma endregion

FAccelByteTimeManagerWPtr FApiClient::GetTimeManager() const
{
	return TimeManagerPtr;
}

FAccelByteApiUtilitiesWPtr FApiClient::GetApiUtilities() const
{
	return ApiUtilitiesPtr;
}

FString FApiClient::GetDeviceId() const
{
	const FAccelByteInstancePtr AccelByteInstancePtr = AccelByteInstanceWeak.Pin();
	if(AccelByteInstancePtr.IsValid())
	{
		return AccelByteInstancePtr->GetDeviceId();
	}

	return TEXT("");
}
}
