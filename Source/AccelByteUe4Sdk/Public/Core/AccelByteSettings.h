// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteBaseSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "AccelByteSettings.generated.h" 

namespace AccelByte
{
class ACCELBYTEUE4SDK_API Settings : public BaseSettings
{
public:
	FString ChatServerWsUrl{};
	FString ChatServerUrl{};
	FString BasicServerUrl{};
	FString CloudStorageServerUrl{};
	FString GameProfileServerUrl{};
	FString GroupServerUrl{};
	FString LeaderboardServerUrl{};
	FString AgreementServerUrl{};
	FString TurnManagerServerUrl{};
	FString UGCServerUrl{};
	FString InventoryServerUrl{};
	FString ReportingServerUrl{};
	FString LoginQueueServerUrl{};
	FString AppId{};
	FString CustomerName{};
	FString SettingsEnvironment{};
	FString HeartBeatData{};
	float QosLatencyPollIntervalSecs{.0f};
	float QosServerLatencyPollIntervalSecs{.0f};
	int64 PresenceBroadcastEventHeartbeatInterval{600};
	bool bEnablePresenceBroadcastEventHeartbeat;
	bool bEnableHttpCache{false};
	bool bServerUseAMS{false};//Affect QoS Manager BaseURL
	bool bDisableAutoGetQosLatencies{false};
	
	/* @brief false will disable cache on game telemetry, except for what listed on critical event list */
	bool bEnableGameTelemetryCache{ true };
	
	EHttpCacheType HttpCacheType {EHttpCacheType::STORAGE};
	
	/** @brief Ensure a minimum # secs for Qos Latency polling */
	constexpr static float MinNumSecsQosLatencyPolling = {60*10}; // 10m
	
	virtual void Reset(ESettingsEnvironment const Environment) override;

	Settings& operator=(Settings const& Other) = default;

protected:
	virtual void LoadSettings(const FString& SectionPath) override;
	virtual void LoadFallback(const FString& SectionPath, const FString& Key, FString& Value) override;
};

typedef TSharedRef<Settings, ESPMode::ThreadSafe> SettingsRef;
typedef TSharedPtr<Settings, ESPMode::ThreadSafe> SettingsPtr;
typedef TWeakPtr<Settings, ESPMode::ThreadSafe> SettingsWPtr;

} // Namespace AccelByte

/**
 * @brief UObject for storing settings into configuration file.
 */
UCLASS(Config = Engine, defaultConfig)
class ACCELBYTEUE4SDK_API UAccelByteSettings : public UObject
{
	GENERATED_BODY()
public:
	UAccelByteSettings();

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString ClientId{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString ClientSecret{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString Namespace{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString PublisherNamespace{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString RedirectURI{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString BaseUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString IamServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString PlatformServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString LobbyServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString CloudStorageServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString BasicServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString GameProfileServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString GroupServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString StatisticServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString QosManagerServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString ChatServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString LeaderboardServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString CloudSaveServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString GameTelemetryServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString AgreementServerUrl{};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString AchievementServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString SessionBrowserServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString TurnManagerServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString UGCServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString InventoryServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString ReportingServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString SessionServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString MatchmakingV2ServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString ChallengeServerUrl{};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString LoginQueueServerUrl{};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString AppId{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString CustomerName{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString SettingsEnvironment{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString HeartBeatData{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	float QosLatencyPollIntervalSecs{.0f};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	float QosServerLatencyPollIntervalSecs{.0f};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	float QosPingTimeout{.6f};
	
	// Enable token for client side http caching
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	bool bEnableHttpCache{false};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	EHttpCacheType HttpCacheType { EHttpCacheType::STORAGE};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	bool bSendPredefinedEvent{ false };

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	bool bServerUseAMS{ false };

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	bool bEnableGameTelemetryCache{ true };
};

UCLASS(Config = Engine)
class ACCELBYTEUE4SDK_API UAccelByteSettingsDev : public UAccelByteSettings
{
	GENERATED_BODY()
public:
	UAccelByteSettingsDev();
};

UCLASS(Config = Engine)
class ACCELBYTEUE4SDK_API UAccelByteSettingsCert : public UAccelByteSettings
{
	GENERATED_BODY()
public:
	UAccelByteSettingsCert();
};

UCLASS(Config = Engine)
class ACCELBYTEUE4SDK_API UAccelByteSettingsProd : public UAccelByteSettings
{
	GENERATED_BODY()
public:
	UAccelByteSettingsProd();
};