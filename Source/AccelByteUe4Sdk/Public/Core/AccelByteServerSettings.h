// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteEnvironment.h"
#include "AccelByteServerSettings.generated.h"

namespace AccelByte
{

class ACCELBYTEUE4SDK_API ServerSettings
{
public:
	FString ClientId;
	FString ClientSecret;
	FString Namespace;
	FString PublisherNamespace;
    FString RedirectURI;
	FString BaseUrl;
	FString IamServerUrl;
	FString DSMControllerServerUrl;
	FString StatisticServerUrl;
	FString PlatformServerUrl;
	FString QosManagerServerUrl;
	FString GameTelemetryServerUrl;
	FString AchievementServerUrl;
	FString MatchmakingServerUrl;
	FString LobbyServerUrl;
	FString CloudSaveServerUrl;
	FString SeasonPassServerUrl;
	FString SessionBrowserServerUrl;
	float QosPingTimeout{.6};
	
	void Reset(ESettingsEnvironment const Environment);

	ServerSettings& operator=(ServerSettings const& Other) = default;

protected:
	void LoadSettings(const FString& SectionPath);
	void LoadFallback(const FString& SectionPath, const FString& Key, FString& Value);
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
	bool ForceEnableSettings;
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString ClientId;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString ClientSecret;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString Namespace;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString PublisherNamespace;

    UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
    FString RedirectURI;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString BaseUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString IamServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString DSMControllerServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString StatisticServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString PlatformServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString QosManagerServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString GameTelemetryServerUrl;
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString AchievementServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString MatchmakingServerUrl;
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString LobbyServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString CloudSaveServerUrl;
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString SeasonPassServerUrl;
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	FString SessionBrowserServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Server | Settings")
	float QosPingTimeout;
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
	static FString GetCloudSaveServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetSeasonPassServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetSessionBrowserServerUrl();

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
	static void SetMatchmakingServerUrl(const FString& AchievementServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetLobbyServerUrl(const FString& LobbyServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetCloudSaveServerUrl(const FString& CloudServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetSeasonPassServerUrl(const FString& CloudServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetSessionBrowserServerUrl(const FString& CloudServerUrl);

	/**
	 * @brief
	 * @param QosPingTimeout should be no less than .6 second and no more than 1 min
	 */
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetQosPingTimeout(const float& QosPingTimeout);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void ResetSettings(const ESettingsEnvironment Environment);
};
