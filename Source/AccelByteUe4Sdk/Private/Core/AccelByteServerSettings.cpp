// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteRegistry.h"

using namespace AccelByte;

UAccelByteServerSettings::UAccelByteServerSettings()
{}

UAccelByteServerSettingsDev::UAccelByteServerSettingsDev()
{}

UAccelByteServerSettingsCert::UAccelByteServerSettingsCert()
{}

UAccelByteServerSettingsProd::UAccelByteServerSettingsProd()
{}

static const FString DefaultServerSection = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettings");

FString GetServerConfigUrlValue(const FString& SectionPath, const FString& Key, const FString& BaseUrl, const FString& DefaultPrefix)
{
	FString Value;

	if (GConfig->GetString(*SectionPath, *Key, Value, GEngineIni)) return Value;

	return FString::Printf(TEXT("%s/%s"), *BaseUrl, *DefaultPrefix);
}

void ServerSettings::LoadSettings(const FString& SectionPath)
{
#if WITH_EDITOR || UE_SERVER || UE_BUILD_DEVELOPMENT
	if (GConfig->GetString(*SectionPath, TEXT("ClientId"), ClientId, GEngineIni))
	{
		GConfig->GetString(*SectionPath, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
	else
	{
		GConfig->GetString(*DefaultServerSection, TEXT("ClientId"), ClientId, GEngineIni);
		GConfig->GetString(*DefaultServerSection, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
	LoadFallback(SectionPath, TEXT("BaseUrl"), BaseUrl);
	LoadFallback(SectionPath, TEXT("Namespace"), Namespace);
	LoadFallback(SectionPath, TEXT("PublisherNamespace"), PublisherNamespace);
	LoadFallback(SectionPath, TEXT("RedirectURI"), RedirectURI);

	IamServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("IamServerUrl"), BaseUrl, TEXT("iam"));

	DSMControllerServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("DSMControllerServerUrl"), BaseUrl, TEXT("dsmcontroller"));

	StatisticServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("StatisticServerUrl"), BaseUrl, TEXT("social"));

	PlatformServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("PlatformServerUrl"), BaseUrl, TEXT("platform"));

	QosManagerServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("QosManagerServerUrl"), BaseUrl, TEXT("qosm"));

	GameTelemetryServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("GameTelemetryServerUrl"), BaseUrl, TEXT("game-telemetry"));

	AchievementServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("AchievementServerUrl"), BaseUrl, TEXT("achievement"));

	MatchmakingServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("MatchmakingServerUrl"), BaseUrl, TEXT("matchmaking"));

	LobbyServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("LobbyServerUrl"), BaseUrl, TEXT("lobby"));

	CloudSaveServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("CloudSaveServerUrl"), BaseUrl, TEXT("cloudsave"));

	SeasonPassServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("SeasonPassServerUrl"), BaseUrl, TEXT("seasonpass"));

	SessionBrowserServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("SessionBrowserServerUrl"), BaseUrl, TEXT("sessionbrowser"));

	SessionServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("SessionServerUrl"), BaseUrl, TEXT("session"));

	ChatServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("ChatServerUrl"), BaseUrl, TEXT("chat"));

	GDPRServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("GDPRServerUrl"), BaseUrl, TEXT("gdpr"));

	const FString DSHubBaseUrl = BaseUrl.Replace(TEXT("https://"), TEXT("wss://"));
	DSHubServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("DSHubServerUrl"), DSHubBaseUrl, TEXT("dshub/"));

	MatchmakingV2ServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("MatchmakingV2ServerUrl"), BaseUrl, TEXT("match2"));

	FString QosPingTimeoutString;
	LoadFallback(SectionPath, TEXT("QosPingTimeout"), QosPingTimeoutString);
	if (QosPingTimeoutString.IsNumeric())
	{
		QosPingTimeout = FCString::Atof(*QosPingTimeoutString);
	}
	else
	{
		QosPingTimeout = .6;
	}

	FParse::Value(FCommandLine::Get(), TEXT("dsid="), DSId);
	if (!FParse::Value(FCommandLine::Get(), TEXT("watchdog_url="), AMSServerWatchdogUrl))
	{
		GConfig->GetString(*DefaultServerSection, TEXT("WatchdogUrl"), AMSServerWatchdogUrl, GEngineIni);
	}

	if (!FParse::Value(FCommandLine::Get(), TEXT("heartbeat="), AMSHeartbeatInterval))
	{
		GConfig->GetInt(*DefaultServerSection, TEXT("AMSHeartbeatInterval"), AMSHeartbeatInterval, GEngineIni);
	}

	LoadFallback(SectionPath, TEXT("StatsDUrl"), StatsDServerUrl);
	FString StatsDPortString;
	LoadFallback(SectionPath, TEXT("StatsDPort"), StatsDPortString);
	if (StatsDPortString.IsNumeric())
	{
		StatsDServerPort = FCString::Atoi(*StatsDPortString);
	}
	else
	{
		StatsDServerPort = 8125;
	}
	FString StatsDMetricIntervalString;
	LoadFallback(SectionPath, TEXT("StatsDMetricInterval"), StatsDMetricIntervalString);
	if (StatsDMetricIntervalString.IsNumeric())
	{
		StatsDMetricInterval = FCString::Atoi(*StatsDMetricIntervalString);
	}
	else
	{
		StatsDMetricInterval = 60;
	}
#endif
}

void ServerSettings::LoadFallback(const FString& SectionPath, const FString& Key, FString& Value)
{
	if (!GConfig->GetString(*SectionPath, *Key, Value, GEngineIni))
	{
		GConfig->GetString(*DefaultServerSection, *Key, Value, GEngineIni);
	}
}

void ServerSettings::Reset(ESettingsEnvironment const Environment)
{
	FString SectionPath;
	switch (Environment)
	{
	case ESettingsEnvironment::Development:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsDev");
		break;
	case ESettingsEnvironment::Certification:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsCert");
		break;
	case ESettingsEnvironment::Production:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsProd");
		break;
	case ESettingsEnvironment::Sandbox:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsSandbox");
		break;
	case ESettingsEnvironment::Integration:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsIntegration");
		break;
	case ESettingsEnvironment::QA:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsQA");
		break;
	case ESettingsEnvironment::Default:
	default:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettings");
		break;
	}

	LoadSettings(SectionPath);
}

FString UAccelByteBlueprintsServerSettings::GetClientId()
{
	return FRegistry::ServerSettings.ClientId;
}

FString UAccelByteBlueprintsServerSettings::GetClientSecret()
{
	return FRegistry::ServerSettings.ClientSecret;
}

FString UAccelByteBlueprintsServerSettings::GetNamespace()
{
	return FRegistry::ServerSettings.Namespace;
}

FString UAccelByteBlueprintsServerSettings::GetPublisherNamespace()
{
	return FRegistry::ServerSettings.PublisherNamespace;
}

FString UAccelByteBlueprintsServerSettings::GetIamServerUrl()
{
	return FRegistry::ServerSettings.IamServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetDSMControllerServerUrl()
{
	return FRegistry::ServerSettings.DSMControllerServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetStatisticServerUrl()
{
	return FRegistry::ServerSettings.StatisticServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetPlatformServerUrl()
{
	return FRegistry::ServerSettings.PlatformServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetQosManagerServerUrl()
{
	return FRegistry::ServerSettings.QosManagerServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetGameTelemetryServerUrl()
{
	return FRegistry::ServerSettings.GameTelemetryServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetAchievementServerUrl()
{
	return FRegistry::ServerSettings.AchievementServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetMatchmakingServerUrl()
{
	return FRegistry::ServerSettings.MatchmakingServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetLobbyServerUrl()
{
	return FRegistry::ServerSettings.LobbyServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetChatServerUrl()
{
	return FRegistry::ServerSettings.ChatServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetCloudSaveServerUrl()
{
	return FRegistry::ServerSettings.CloudSaveServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetSeasonPassServerUrl()
{
	return FRegistry::ServerSettings.SeasonPassServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetSessionBrowserServerUrl()
{
	return FRegistry::ServerSettings.SessionBrowserServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetSessionServerUrl()
{
	return FRegistry::ServerSettings.SessionServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetDSHubServerUrl()
{
	return FRegistry::ServerSettings.DSHubServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetMatchmakingV2ServerUrl()
{
	return FRegistry::ServerSettings.MatchmakingV2ServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetAMSServerWatchdogUrl()
{
	return FRegistry::ServerSettings.AMSServerWatchdogUrl;
}

FString UAccelByteBlueprintsServerSettings::GetGDPRServerUrl()
{
	return FRegistry::ServerSettings.GDPRServerUrl;
}

float UAccelByteBlueprintsServerSettings::GetQosPingTimeout()
{
	return FRegistry::ServerSettings.QosPingTimeout;
}

void UAccelByteBlueprintsServerSettings::SetClientId(const FString& ClientId)
{
	FRegistry::ServerSettings.ClientId = ClientId;
}

void UAccelByteBlueprintsServerSettings::SetClientSecret(const FString& ClientSecret)
{
	FRegistry::ServerSettings.ClientSecret = ClientSecret;
}

void UAccelByteBlueprintsServerSettings::SetNamespace(const FString& Namespace)
{
	FRegistry::ServerSettings.Namespace = Namespace;
}

void UAccelByteBlueprintsServerSettings::SetPublisherNamespace(const FString& PublisherNamespace)
{
	FRegistry::ServerSettings.PublisherNamespace = PublisherNamespace;
}

void UAccelByteBlueprintsServerSettings::SetIamServerUrl(const FString& IamServerUrl)
{
	FRegistry::ServerSettings.IamServerUrl = IamServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetDSMControllerServerUrl(const FString& DSMControllerServerUrl)
{
	FRegistry::ServerSettings.DSMControllerServerUrl = DSMControllerServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetStatisticServerUrl(const FString& StatisticServerUrl)
{
	FRegistry::ServerSettings.StatisticServerUrl = StatisticServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetPlatformServerUrl(const FString& PlatformServerUrl)
{
	FRegistry::ServerSettings.PlatformServerUrl = PlatformServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetQosManagerServerUrl(const FString& QosManagerServerUrl)
{
	FRegistry::ServerSettings.QosManagerServerUrl = QosManagerServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetGameTelemetryServerUrl(const FString& GameTelemetryServerUrl)
{
	FRegistry::ServerSettings.GameTelemetryServerUrl = GameTelemetryServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetAchievementServerUrl(const FString& AchievementServerUrl)
{
	FRegistry::ServerSettings.AchievementServerUrl = AchievementServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetMatchmakingServerUrl(const FString& MatchmakingServerUrl)
{
	FRegistry::ServerSettings.MatchmakingServerUrl = MatchmakingServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetLobbyServerUrl(const FString & LobbyServerUrl)
{
	FRegistry::ServerSettings.LobbyServerUrl = LobbyServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetChatServerUrl(const FString& ChatServerUrl)
{
	FRegistry::ServerSettings.ChatServerUrl = ChatServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetCloudSaveServerUrl(const FString & CloudSaveServerUrl)
{
	FRegistry::ServerSettings.CloudSaveServerUrl = CloudSaveServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetSeasonPassServerUrl(const FString & SeasonPassServerUrl)
{
	FRegistry::ServerSettings.SeasonPassServerUrl = SeasonPassServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetSessionBrowserServerUrl(const FString & SessionBrowserUrl)
{
	FRegistry::ServerSettings.SessionBrowserServerUrl = SessionBrowserUrl;
}

void UAccelByteBlueprintsServerSettings::SetSessionServerUrl(const FString& SessionUrl)
{
	FRegistry::ServerSettings.SessionServerUrl = SessionUrl;
}

void UAccelByteBlueprintsServerSettings::SetDSHubServerUrl(const FString& DSHubServerUrl)
{
	FRegistry::ServerSettings.DSHubServerUrl = DSHubServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetMatchmakingV2ServerUrl(const FString& MatchmakingV2ServerUrl)
{
	FRegistry::ServerSettings.MatchmakingV2ServerUrl = MatchmakingV2ServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetAMSServerWatchdogUrl(const FString& AMSServerWatchdogUrl)
{
	FRegistry::ServerSettings.AMSServerWatchdogUrl = AMSServerWatchdogUrl;
}

void UAccelByteBlueprintsServerSettings::SetGDPRServerUrl(const FString& GDPRServerUrl)
{
	FRegistry::ServerSettings.GDPRServerUrl = GDPRServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetQosPingTimeout(const float& QosPingTimeout)
{
	if (QosPingTimeout < .6)
	{
		FRegistry::ServerSettings.QosPingTimeout = .6;
	}
	else if (QosPingTimeout > 60)
	{
		FRegistry::ServerSettings.QosPingTimeout = 60;
	}
	else
	{
		FRegistry::ServerSettings.QosPingTimeout = QosPingTimeout;
	}
}

void UAccelByteBlueprintsServerSettings::ResetSettings(ESettingsEnvironment const Environment)
{
	FRegistry::ServerSettings.Reset(Environment);
	FRegistry::ServerCredentials.SetClientCredentials(FRegistry::ServerSettings.ClientId, FRegistry::ServerSettings.ClientSecret);
}
