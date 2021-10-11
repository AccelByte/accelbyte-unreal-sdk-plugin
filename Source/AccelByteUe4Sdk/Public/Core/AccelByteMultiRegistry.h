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
#include "Api/AccelByteTurnManagerApi.h"
#include "Api/AccelByteUGCApi.h"
#include "Api/AccelByteSeasonPassApi.h"
#include "Api/AccelByteReportingApi.h"
#include "Api/AccelByteCurrencyApi.h"
#include "Api/AccelByteMiscellaneousApi.h"
#include "Api/AccelByteRewardApi.h"

#include "GameServerApi/AccelByteServerQosManagerApi.h"

using namespace AccelByte;

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FApiBase
{

};

class ACCELBYTEUE4SDK_API FApiClient final
{
public:
	FApiClient();
	~FApiClient();

	Credentials Credentials{};
	FHttpRetryScheduler Http{};
	Api::User User{Credentials, FRegistry::Settings, Http};
	Api::UserProfile UserProfile{Credentials, FRegistry::Settings, Http};
	Api::Category Category{Credentials, FRegistry::Settings, Http};
	Api::Entitlement Entitlement{Credentials, FRegistry::Settings, Http};
	Api::Order Order{Credentials, FRegistry::Settings, Http};
	Api::Item Item{Credentials, FRegistry::Settings, Http};
	Api::Wallet Wallet{Credentials, FRegistry::Settings, Http};
	Api::Fulfillment Fulfillment{Credentials, FRegistry::Settings, Http};
	Api::CloudStorage CloudStorage{Credentials, FRegistry::Settings};
	Api::Lobby Lobby{Credentials, FRegistry::Settings, Http};
	Api::GameProfile GameProfile{Credentials, FRegistry::Settings, Http};
	Api::Statistic Statistic{Credentials, FRegistry::Settings, Http};
	Api::QosManager QosManager{Credentials, FRegistry::Settings, Http};
	Api::Qos Qos{};
	Api::Leaderboard Leaderboard{Credentials, FRegistry::Settings, Http};
	Api::CloudSave CloudSave{Credentials, FRegistry::Settings, Http};
	Api::GameTelemetry GameTelemetry{Credentials, FRegistry::Settings, Http};
	Api::Agreement Agreement{Credentials, FRegistry::Settings, Http};
	Api::Achievement Achievement{Credentials, FRegistry::Settings, Http};
	Api::SessionBrowser SessionBrowser{Credentials, FRegistry::Settings, Http};
	Api::UGC UGC{Credentials, FRegistry::Settings, Http};
	Api::SeasonPass SeasonPass{Credentials, FRegistry::Settings, Http};
	Api::Reporting Reporting{Credentials, FRegistry::Settings, Http};
	Api::Currency Currency{ Credentials, FRegistry::Settings, Http };
	Api::Miscellaneous Miscellaneous{ Credentials, FRegistry::Settings, Http };
	Api::Reward Reward{ Credentials, FRegistry::Settings, Http };
	Api::TurnManager TurnManager{ Credentials, FRegistry::Settings, Http };
	
	template<typename T, typename... U>
	T GetApi(U&&... Args)
	{
		static_assert(std::is_base_of<FApiBase, T>::value, "API class must be subclass of FApiBase");

		return T(Credentials, FRegistry::Settings, Http, Forward<U>(Args)...);
	}
};

class ACCELBYTEUE4SDK_API FMultiRegistry
{
public:
	static TSharedPtr<FApiClient> GetApiClient(const FString Key = TEXT("default"));

private:
	static TMap<FString, TSharedPtr<FApiClient>> ApiClientInstances;

	FMultiRegistry() = delete;
	FMultiRegistry(FMultiRegistry const& Other) = delete;
	FMultiRegistry& operator=(FMultiRegistry const& Other) = delete;
	FMultiRegistry(FMultiRegistry&& Other) = delete;
	FMultiRegistry& operator=(FMultiRegistry&& Other) = delete;
	~FMultiRegistry() = delete;
};

}
