// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteBaseSettings.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteServerSettings.generated.h"

namespace AccelByte
{

class ACCELBYTEUE4SDK_API ServerSettings : public BaseSettings
{
public:
	FString DSMControllerServerUrl{};
	FString MatchmakingServerUrl{};
	FString ChatServerUrl{};
	FString DSHubServerUrl{};
	FString DSId{};
	FString AMSServerWatchdogUrl{};
	FString StatsDServerUrl{};
	int32 StatsDServerPort{0};
	int32 StatsDMetricInterval{0};
	int AMSHeartbeatInterval{0};
	float DSHubReconnectTotalTimeout{0};
	float AMSReconnectTotalTimeout{0};
	bool bServerUseAMS{true};

	virtual void Reset(ESettingsEnvironment const Environment) override;

	bool LoadAMSSettings();

	ServerSettings& operator=(ServerSettings const& Other) = default;

protected:
	virtual void LoadSettings(const FString& SectionPath) override;
	virtual void LoadFallback(const FString& SectionPath, const FString& Key, FString& Value) override;
};

typedef TSharedRef<ServerSettings, ESPMode::ThreadSafe> ServerSettingsRef;
typedef TSharedPtr<ServerSettings, ESPMode::ThreadSafe> ServerSettingsPtr;

} // Namespace AccelByte

/**
 * @brief UObject for storing settings into configuration file.
 */
UCLASS(Config = Engine, defaultConfig)
class ACCELBYTEUE4SDK_API UAccelByteServerSettings : public UObject
{
	GENERATED_BODY()
public:
	UAccelByteServerSettings();

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	bool ForceEnableSettings{false};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString ClientId{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString ClientSecret{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString Namespace{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString PublisherNamespace{};

    UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
    FString RedirectURI{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString BaseUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString IamServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString BasicServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString DSMControllerServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString StatisticServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString PlatformServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString QosManagerServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString GameTelemetryServerUrl{};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString AchievementServerUrl{};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString UGCServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString MatchmakingServerUrl{};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString LobbyServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString CloudSaveServerUrl{};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString SeasonPassServerUrl{};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString SessionBrowserServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString SessionServerUrl{};

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
	FString ChatServerUrl{};

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
	float QosPingTimeout{.6f};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString DSHubServerUrl{};
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString MatchmakingV2ServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString AMSServerWatchdogUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	int AMSHeartbeatInterval{15};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	bool bSendPredefinedEvent{ false };
};

UCLASS(Config = Engine)
class ACCELBYTEUE4SDK_API UAccelByteServerSettingsDev : public UAccelByteServerSettings
{
	GENERATED_BODY()
public:
	UAccelByteServerSettingsDev();
};

UCLASS(Config = Engine)
class ACCELBYTEUE4SDK_API UAccelByteServerSettingsCert : public UAccelByteServerSettings
{
	GENERATED_BODY()
public:
	UAccelByteServerSettingsCert();
};

UCLASS(Config = Engine)
class ACCELBYTEUE4SDK_API UAccelByteServerSettingsProd : public UAccelByteServerSettings
{
	GENERATED_BODY()
public:
	UAccelByteServerSettingsProd();
};