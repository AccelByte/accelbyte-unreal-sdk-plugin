// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "CoreUObject.h"
#include "Api/AccelByteGameTelemetryApi.h"
#include "Runtime/Core/Public/Containers/Ticker.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif

FString GetDefaultAPIUrl(const FString& SpecificServerUrl, const FString& DefaultServerPath)
{
	if (SpecificServerUrl.IsEmpty()) 
	{
		return FString::Printf(TEXT("%s/%s"), *FRegistry::Settings.BaseUrl, *DefaultServerPath);
	}

	return SpecificServerUrl;
}

FString GetDefaultServerAPIUrl(const FString& SpecificServerUrl, const FString& DefaultServerPath)
{
	if (SpecificServerUrl.IsEmpty())
	{
		return FString::Printf(TEXT("%s/%s"), *FRegistry::ServerSettings.BaseUrl, *DefaultServerPath);
	}

	return SpecificServerUrl;
}

class FAccelByteUe4SdkModule : public IAccelByteUe4SdkModuleInterface
{
    virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// For registering settings in UE4 editor
	void RegisterSettings();
	void UnregisterSettings();

	bool LoadSettingsFromConfigUobject();
	bool LoadServerSettingsFromConfigUobject();
	void NullCheckConfig(FString value, FString configField);
};

void FAccelByteUe4SdkModule::StartupModule()
{
	RegisterSettings();
	LoadSettingsFromConfigUobject();
	LoadServerSettingsFromConfigUobject();

	FRegistry::HttpRetryScheduler.Startup();
	FRegistry::Credentials.Startup();
	FRegistry::GameTelemetry.Startup();
	FRegistry::ServerCredentials.Startup();
}

void FAccelByteUe4SdkModule::ShutdownModule()
{
	FRegistry::GameTelemetry.Shutdown();
	FRegistry::Credentials.Shutdown();
	FRegistry::HttpRetryScheduler.Shutdown();
	FRegistry::ServerCredentials.Shutdown();

	UnregisterSettings();
}

void FAccelByteUe4SdkModule::RegisterSettings()
{

#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(TEXT("Project"), TEXT("Plugins"), TEXT("AccelByte Unreal Engine 4 Client SDK"),
			FText::FromName(TEXT("AccelByte Unreal Engine 4 Client SDK")),
			FText::FromName(TEXT("Setup your plugin.")),
			GetMutableDefault<UAccelByteSettings>()
		);
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FAccelByteUe4SdkModule::LoadSettingsFromConfigUobject);
		}

		ISettingsSectionPtr ServerSettingsSection = SettingsModule->RegisterSettings(TEXT("Project"), TEXT("Plugins"), TEXT("AccelByte Unreal Engine 4 Server SDK"),
			FText::FromName(TEXT("AccelByte Unreal Engine 4 Server SDK")),
			FText::FromName(TEXT("Setup your plugin.")),
			GetMutableDefault<UAccelByteServerSettings>()
		);
		if (ServerSettingsSection.IsValid())
		{
			ServerSettingsSection->OnModified().BindRaw(this, &FAccelByteUe4SdkModule::LoadServerSettingsFromConfigUobject);
		}
	}
#endif
}

void FAccelByteUe4SdkModule::UnregisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings(TEXT("Project"), TEXT("Plugins"), TEXT("AccelByte Unreal Engine 4 SDK"));
	}
#endif
}

bool FAccelByteUe4SdkModule::LoadSettingsFromConfigUobject()
{
	FRegistry::Settings.ClientId = GetDefault<UAccelByteSettings>()->ClientId;
	FRegistry::Settings.ClientSecret = GetDefault<UAccelByteSettings>()->ClientSecret;
	FRegistry::Settings.Namespace = GetDefault<UAccelByteSettings>()->Namespace;
	FRegistry::Settings.PublisherNamespace = GetDefault<UAccelByteSettings>()->PublisherNamespace;
	FRegistry::Settings.RedirectURI = GetDefault<UAccelByteSettings>()->RedirectURI;
	FRegistry::Settings.BaseUrl = GetDefault<UAccelByteSettings>()->BaseUrl;
	FRegistry::Settings.NonApiBaseUrl = GetDefault<UAccelByteSettings>()->NonApiBaseUrl;

	NullCheckConfig(*FRegistry::Settings.ClientId, "Client ID");
	NullCheckConfig(*FRegistry::Settings.Namespace, "Namespace");
	NullCheckConfig(*FRegistry::Settings.BaseUrl, "Base URL");
	NullCheckConfig(*FRegistry::Settings.NonApiBaseUrl, "Non-API Base URL");

	FRegistry::Settings.IamServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->IamServerUrl, TEXT("iam"));
	FRegistry::Settings.PlatformServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->PlatformServerUrl, TEXT("platform"));
	FRegistry::Settings.LobbyServerUrl = GetDefault<UAccelByteSettings>()->LobbyServerUrl;
	if (FRegistry::Settings.LobbyServerUrl.IsEmpty())
	{
		const FString BaseUrl = FRegistry::Settings.NonApiBaseUrl.Replace(TEXT("https://"), TEXT("wss://"));
		FRegistry::Settings.LobbyServerUrl = FString::Printf(TEXT("%s/%s"), *BaseUrl, TEXT("lobby/"));
	}
	FRegistry::Settings.BasicServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->BasicServerUrl, TEXT("basic"));
	FRegistry::Settings.CloudStorageServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->CloudStorageServerUrl, TEXT("binary-store"));
	FRegistry::Settings.GameProfileServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->GameProfileServerUrl, TEXT("soc-profile"));
	FRegistry::Settings.StatisticServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->StatisticServerUrl, TEXT("statistic"));
	FRegistry::Settings.QosManagerServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->QosManagerServerUrl, TEXT("qosm"));
	FRegistry::Settings.LeaderboardServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->LeaderboardServerUrl, TEXT("leaderboard"));
	FRegistry::Settings.GameTelemetryServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->GameTelemetryServerUrl, TEXT("game-telemetry"));
	FRegistry::Settings.AgreementServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->AgreementServerUrl, TEXT("agreement"));
	FRegistry::Settings.CloudSaveServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->CloudSaveServerUrl, TEXT("cloudsave"));
	FRegistry::Settings.AchievementServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->AchievementServerUrl, TEXT("achievement"));
	FRegistry::Settings.SessionBrowserServerUrl = GetDefaultAPIUrl(GetDefault<UAccelByteSettings>()->SessionBrowserServerUrl, TEXT("sessionbrowser"));
	FRegistry::Settings.AppId = GetDefault<UAccelByteSettings>()->AppId;
	FRegistry::Credentials.SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);
	
	return true;
}

bool FAccelByteUe4SdkModule::LoadServerSettingsFromConfigUobject()
{
#if WITH_EDITOR || UE_SERVER
	FRegistry::ServerSettings.ClientId = GetDefault<UAccelByteServerSettings>()->ClientId;
	FRegistry::ServerSettings.ClientSecret = GetDefault<UAccelByteServerSettings>()->ClientSecret;
	FRegistry::ServerSettings.Namespace = GetDefault<UAccelByteServerSettings>()->Namespace;
	FRegistry::ServerSettings.PublisherNamespace = GetDefault<UAccelByteServerSettings>()->PublisherNamespace;
	FRegistry::ServerSettings.RedirectURI = GetDefault<UAccelByteServerSettings>()->RedirectURI;
	FRegistry::ServerSettings.BaseUrl = GetDefault<UAccelByteServerSettings>()->BaseUrl;

	NullCheckConfig(*FRegistry::ServerSettings.ClientId, "Client ID");
	NullCheckConfig(*FRegistry::ServerSettings.ClientSecret, "Client Secret");

	FRegistry::ServerSettings.IamServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->IamServerUrl, TEXT("iam"));
	FRegistry::ServerSettings.DSMControllerServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->DSMControllerServerUrl, TEXT("dsmcontroller"));
	FRegistry::ServerSettings.StatisticServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->StatisticServerUrl, TEXT("social"));
	FRegistry::ServerSettings.PlatformServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->PlatformServerUrl, TEXT("platform"));
	FRegistry::ServerSettings.QosManagerServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->QosManagerServerUrl, TEXT("qosm"));
	FRegistry::ServerSettings.GameTelemetryServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->GameTelemetryServerUrl, TEXT("game-telemetry"));
	FRegistry::ServerSettings.AchievementServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->AchievementServerUrl, TEXT("achievement"));
	FRegistry::ServerSettings.MatchmakingServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->MatchmakingServerUrl, TEXT("matchmaking"));
	FRegistry::ServerSettings.LobbyServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->LobbyServerUrl, TEXT("lobby"));
	FRegistry::ServerSettings.CloudSaveServerUrl = GetDefaultServerAPIUrl(GetDefault<UAccelByteServerSettings>()->CloudSaveServerUrl, TEXT("cloudsave"));
	FRegistry::ServerCredentials.SetClientCredentials(FRegistry::ServerSettings.ClientId, FRegistry::ServerSettings.ClientSecret);

#endif
	return true;
}

void FAccelByteUe4SdkModule::NullCheckConfig(FString value, FString configField)
{
	if (value.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("\"%s\" is not configured yet.\nCheck DefaultEngine.ini or Edit/ProjectSettings/Plugins/"), *configField);
	}
}

IMPLEMENT_MODULE(FAccelByteUe4SdkModule, AccelByteUe4Sdk)
