// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteEnvironment.h"

#include "AccelByteSettings.generated.h"

namespace AccelByte
{

class ACCELBYTEUE4SDK_API Settings
{
public:
	FString ClientId;
	FString ClientSecret;
	FString Namespace;
	FString PublisherNamespace;
	FString RedirectURI;
	FString BaseUrl;
	FString IamServerUrl;
	FString PlatformServerUrl;
	FString LobbyServerUrl;
	FString BasicServerUrl;
	FString CloudStorageServerUrl;
	FString GameProfileServerUrl;
	FString StatisticServerUrl;
	FString QosManagerServerUrl;
	FString LeaderboardServerUrl;
	FString CloudSaveServerUrl;
	FString GameTelemetryServerUrl;
	FString AgreementServerUrl;
	FString AchievementServerUrl;
	FString SessionBrowserServerUrl;
	FString TurnManagerServerUrl;
	FString UGCServerUrl;
	FString SeasonPassServerUrl;
	FString ReportingServerUrl;
	FString AppId;

	void Reset(ESettingsEnvironment const Environment);

	Settings& operator=(Settings const& Other) = default;
};

typedef TSharedRef<Settings, ESPMode::ThreadSafe> SettingsRef;
typedef TSharedPtr<Settings, ESPMode::ThreadSafe> SettingsPtr;

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
	FString ClientId;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString ClientSecret;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString Namespace;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString PublisherNamespace;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString RedirectURI;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString BaseUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString IamServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString PlatformServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString LobbyServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString CloudStorageServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString BasicServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString GameProfileServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString StatisticServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString QosManagerServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString LeaderboardServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString CloudSaveServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString GameTelemetryServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString AgreementServerUrl;
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString AchievementServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString SessionBrowserServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString TurnManagerServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString UGCServerUrl;

	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString ReportingServerUrl;
	
	UPROPERTY(EditAnywhere, Config, Category = "AccelByte Client | Settings")
	FString AppId;
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

/**
 * @brief Get or update settings via blueprint.
 */
UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsSettings : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetClientId();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetClientSecret();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetNamespace();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetPublisherNamespace();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetIamServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetPlatformServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetLobbyServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetCloudStorageServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetBasicServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetGameProfileServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetStatisticServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetQosManagerServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetLeaderboardServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetCloudSaveServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetGameTelemetryServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetAchievementServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetSessionBrowserServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetTurnManagerServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetUGCServerUrl();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetReportingServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static FString GetAppId();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetClientId(const FString& ClientId);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetClientSecret(const FString& ClientSecret);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetNamespace(const FString& Namespace);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetPublisherNamespace(const FString& PublisherNamespace);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetIamServerUrl(const FString& IamServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetPlatformServerUrl(const FString& PlatformServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetLobbyServerUrl(const FString& LobbyServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetCloudStorageServerUrl(const FString& CloudStorageServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetBasicServerUrl(const FString& BasicServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetGameProfileServerUrl(const FString& GameProfileServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetStatisticServerUrl(const FString& StatisticServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetQosManagerServerUrl(const FString& QosManagerServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetLeaderboardServerUrl(const FString& LeaderboardServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetCloudSaveServerUrl(const FString& CloudSaveServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetGameTelemetryServerUrl(const FString& GameTelemetryServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetAchievementServerUrl(const FString& CloudSaveServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetSessionBrowserServerUrl(const FString& SessionBrowserServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetTurnManagerServerUrl(const FString& TurnManagerServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetUGCServerUrl(const FString& UGCServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetReportingServerUrl(const FString& ReportingServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetSeasonPassServerUrl(const FString& SeasonPassServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void SetAppId(const FString& AppId);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Client | Settings")
	static void ResetSettings(const ESettingsEnvironment Environment);
};

