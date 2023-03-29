// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteBaseSettings.h"
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
	FString WatchdogServerUrl{};
	FString StatsDServerUrl{};
	int32 StatsDServerPort;
	int32 StatsDMetricInterval;
	int WatchdogHeartbeatInterval;

	virtual void Reset(ESettingsEnvironment const Environment) override;

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
	FString WatchdogServerUrl{};

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	int WatchdogHeartbeatInterval{15};
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

/**
 * @brief Get or update settings via blueprint.
 */
UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsServerSettings : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetClientId();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetClientSecret();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetNamespace();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetPublisherNamespace();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetIamServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetDSMControllerServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetStatisticServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetPlatformServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetQosManagerServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetGameTelemetryServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetAchievementServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetMatchmakingServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetLobbyServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetChatServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetCloudSaveServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetSeasonPassServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetSessionBrowserServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetSessionServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetDSHubServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetMatchmakingV2ServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetWatchdogServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static float GetQosPingTimeout();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetClientId(const FString& ClientId);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetClientSecret(const FString& ClientSecret);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetNamespace(const FString& Namespace);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetPublisherNamespace(const FString& PublisherNamespace);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetIamServerUrl(const FString& IamServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetDSMControllerServerUrl(const FString& DSMControllerServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetStatisticServerUrl(const FString& StatisticServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetPlatformServerUrl(const FString& PlatformServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetQosManagerServerUrl(const FString& PlatformServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetGameTelemetryServerUrl(const FString& GameTelemetryServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetAchievementServerUrl(const FString& AchievementServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetMatchmakingServerUrl(const FString& MatchmakingServerurl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetLobbyServerUrl(const FString& LobbyServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetChatServerUrl(const FString& ChatServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetCloudSaveServerUrl(const FString& CloudServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetSeasonPassServerUrl(const FString& SessionPassServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetSessionBrowserServerUrl(const FString& SessionBrowserServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetSessionServerUrl(const FString& SessionServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetDSHubServerUrl(const FString& DSHubServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetMatchmakingV2ServerUrl(const FString& MatchmakingV2ServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetWatchdogServerUrl(const FString& WatchdogServerUrl);

	/**
	 * @brief
	 * @param QosPingTimeout should be no less than .6 second and no more than 1 min
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetQosPingTimeout(const float& QosPingTimeout);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void ResetSettings(const ESettingsEnvironment Environment);
};
