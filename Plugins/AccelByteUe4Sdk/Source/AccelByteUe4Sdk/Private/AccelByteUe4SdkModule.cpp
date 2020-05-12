// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "CoreUObject.h"
#include "Runtime/Core/Public/Containers/Ticker.h"

#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif


class FAccelByteUe4SdkModule : public IAccelByteUe4SdkModuleInterface
{
    virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	// For registering settings in UE4 editor
	void RegisterSettings();
	void UnregisterSettings();

	bool LoadSettingsFromConfigUobject();
	bool LoadServerSettingsFromConfigUobject();
};

void FAccelByteUe4SdkModule::StartupModule()
{
	RegisterSettings();
	LoadSettingsFromConfigUobject();
	LoadServerSettingsFromConfigUobject();
	FTicker& Ticker = FTicker::GetCoreTicker();

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([](float DeltaTime)
		{
			FRegistry::HttpRetryScheduler.PollRetry(FPlatformTime::Seconds(), FRegistry::Credentials);

			return true;
		}),
		0.2f);

	Ticker.AddTicker(
		FTickerDelegate::CreateLambda([](float DeltaTime)
		{
			FRegistry::Credentials.PollRefreshToken(FPlatformTime::Seconds());

			return true;
		}),
		0.2f);
}

void FAccelByteUe4SdkModule::ShutdownModule()
{
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
	FRegistry::Settings.IamServerUrl = GetDefault<UAccelByteSettings>()->IamServerUrl;
	FRegistry::Settings.PlatformServerUrl = GetDefault<UAccelByteSettings>()->PlatformServerUrl;
	FRegistry::Settings.LobbyServerUrl = GetDefault<UAccelByteSettings>()->LobbyServerUrl;
	FRegistry::Settings.BasicServerUrl = GetDefault<UAccelByteSettings>()->BasicServerUrl;
	FRegistry::Settings.CloudStorageServerUrl = GetDefault<UAccelByteSettings>()->CloudStorageServerUrl;
	FRegistry::Settings.GameProfileServerUrl = GetDefault<UAccelByteSettings>()->GameProfileServerUrl;
	FRegistry::Settings.StatisticServerUrl = GetDefault<UAccelByteSettings>()->StatisticServerUrl;
	FRegistry::Settings.QosManagerServerUrl = GetDefault<UAccelByteSettings>()->QosManagerServerUrl;
	FRegistry::Settings.LeaderboardServerUrl = GetDefault<UAccelByteSettings>()->LeaderboardServerUrl;
	FRegistry::Settings.GameTelemetryServerUrl = GetDefault<UAccelByteSettings>()->GameTelemetryServerUrl;
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
	FRegistry::ServerSettings.IamServerUrl = GetDefault<UAccelByteServerSettings>()->IamServerUrl;
	FRegistry::ServerSettings.DSMControllerServerUrl = GetDefault<UAccelByteServerSettings>()->DSMControllerServerUrl;
	FRegistry::ServerSettings.StatisticServerUrl = GetDefault<UAccelByteServerSettings>()->StatisticServerUrl;
	FRegistry::ServerSettings.PlatformServerUrl = GetDefault<UAccelByteServerSettings>()->PlatformServerUrl;
	FRegistry::ServerSettings.QosManagerServerUrl = GetDefault<UAccelByteSettings>()->QosManagerServerUrl;
	FRegistry::ServerSettings.LeaderboardServerUrl = GetDefault<UAccelByteSettings>()->LeaderboardServerUrl;
	FRegistry::ServerSettings.GameTelemetryServerUrl = GetDefault<UAccelByteServerSettings>()->GameTelemetryServerUrl;
	FRegistry::ServerCredentials.SetClientCredentials(FRegistry::ServerSettings.ClientId, FRegistry::ServerSettings.ClientSecret);
#endif
	return true;
}

IMPLEMENT_MODULE(FAccelByteUe4SdkModule, AccelByteUe4Sdk)
