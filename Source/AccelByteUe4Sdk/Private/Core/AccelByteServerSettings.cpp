// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteUtilities.h"

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

	// Check AccelByte command line if exist. For example "GameExecutable -abKey=Value" and fetch the ab value of the key
	if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(Key, Value))
	{
		// If not exist, then fetch from the game default engine ini section
		if (GConfig->GetString(*SectionPath, *Key, Value, GEngineIni)) return Value;

		return FString::Printf(TEXT("%s/%s"), *BaseUrl, *DefaultPrefix);
	}

	return Value;
}

void ServerSettings::LoadSettings(const FString& SectionPath)
{
#if WITH_EDITOR || UE_SERVER || UE_BUILD_DEVELOPMENT
	if (!FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("ClientId"), ClientId, DefaultServerSection))
	{
		ClientId = TEXT("");
	}
	if (!FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("ClientSecret"), ClientSecret, DefaultServerSection))
	{
		ClientSecret = TEXT("");
	}
	
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("BaseUrl"), BaseUrl, DefaultServerSection);
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("Namespace"), Namespace, DefaultServerSection);
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("PublisherNamespace"), PublisherNamespace, DefaultServerSection);
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("RedirectURI"), RedirectURI, DefaultServerSection);

	IamServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("IamServerUrl"), BaseUrl, TEXT("iam"));

	BasicServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("BasicServerUrl"), BaseUrl, TEXT("basic"));

	DSMControllerServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("DSMControllerServerUrl"), BaseUrl, TEXT("dsmcontroller"));

	StatisticServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("StatisticServerUrl"), BaseUrl, TEXT("social"));

	PlatformServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("PlatformServerUrl"), BaseUrl, TEXT("platform"));

	QosManagerServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("QosManagerServerUrl"), BaseUrl, TEXT("qosm"));

	GameTelemetryServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("GameTelemetryServerUrl"), BaseUrl, TEXT("game-telemetry"));

	AchievementServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("AchievementServerUrl"), BaseUrl, TEXT("achievement"));

	UGCServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("UGCServerUrl"), BaseUrl, TEXT("ugc"));

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
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("QosPingTimeout"), QosPingTimeoutString, DefaultServerSection);
	if (QosPingTimeoutString.IsNumeric())
	{
		QosPingTimeout = FCString::Atof(*QosPingTimeoutString);
	}
	else
	{
		QosPingTimeout = .6;
	}

	LoadAMSSettings();

	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("StatsDUrl"), StatsDServerUrl, DefaultServerSection);
	FString StatsDPortString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("StatsDPort"), StatsDPortString, DefaultServerSection);
	if (StatsDPortString.IsNumeric())
	{
		StatsDServerPort = FCString::Atoi(*StatsDPortString);
	}
	else
	{
		StatsDServerPort = 8125;
	}
	FString StatsDMetricIntervalString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("StatsDMetricInterval"), StatsDMetricIntervalString, DefaultServerSection);
	if (StatsDMetricIntervalString.IsNumeric())
	{
		StatsDMetricInterval = FCString::Atoi(*StatsDMetricIntervalString);
	}
	else
	{
		StatsDMetricInterval = 60;
	}

	FString SendPredefinedEventString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("SendPredefinedEvent"), SendPredefinedEventString, DefaultServerSection);
	//Disabling PredefinedEventApi for the time being
	//this->bSendPredefinedEvent = SendPredefinedEventString.IsEmpty() ? false : SendPredefinedEventString.ToBool();
	this->bSendPredefinedEvent = false;
#endif

	FString DSHubReconnectTimeoutString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("DSHubReconnectTotalTimeout"), DSHubReconnectTimeoutString, DefaultServerSection);
	if (DSHubReconnectTimeoutString.IsNumeric())
	{
		DSHubReconnectTotalTimeout = FCString::Atof(*DSHubReconnectTimeoutString);
	}
	else
	{
		DSHubReconnectTotalTimeout = 60.0f;
	}

	FString AMSReconnectTimeoutString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("AMSReconnectTotalTimeout"), AMSReconnectTimeoutString, DefaultServerSection);
	if (AMSReconnectTimeoutString.IsNumeric())
	{
		AMSReconnectTotalTimeout = FCString::Atof(*AMSReconnectTimeoutString);
	}
	else
	{
		AMSReconnectTotalTimeout = 60.0f;
	}
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

bool AccelByte::ServerSettings::LoadAMSSettings()
{
	// Check if the key exist in the commandline of AccelByte format. For example "GameExecutable -abKey=Value" 
	if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(TEXT("watchdog_url"), AMSServerWatchdogUrl))
	{
		if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(TEXT("WatchdogUrl"), AMSServerWatchdogUrl))
		{
			// If not exist, then check without AccelByte format. For example "GameExecutable -Key=Value"
			if (!FAccelByteUtilities::GetValueFromCommandLineSwitch(TEXT("watchdog_url"), AMSServerWatchdogUrl))
			{
				// If not exist, then fetch the setting from game default engine ini file
				GConfig->GetString(*DefaultServerSection, TEXT("WatchdogUrl"), AMSServerWatchdogUrl, GEngineIni);
			}
		}
	}

	if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(TEXT("heartbeat"), AMSHeartbeatInterval))
	{
		if (!FAccelByteUtilities::GetValueFromCommandLineSwitch(TEXT("heartbeat"), AMSHeartbeatInterval))
		{
			GConfig->GetInt(*DefaultServerSection, TEXT("AMSHeartbeatInterval"), AMSHeartbeatInterval, GEngineIni);
		}
	}

	if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(TEXT("dsid"), DSId))
	{
		FAccelByteUtilities::GetValueFromCommandLineSwitch(TEXT("dsid"), DSId);
	}

	return true;
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

FString UAccelByteBlueprintsServerSettings::GetBasicServerUrl()
{
	return FRegistry::ServerSettings.BasicServerUrl;
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

FString UAccelByteBlueprintsServerSettings::GetUGCServerUrl()
{
	return FRegistry::ServerSettings.UGCServerUrl;
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

bool UAccelByteBlueprintsServerSettings::IsSendPredefinedEvent()
{
	return FRegistry::ServerSettings.bSendPredefinedEvent;
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

void UAccelByteBlueprintsServerSettings::SetBasicServerUrl(const FString& BasicServerUrl)
{
	FRegistry::ServerSettings.BasicServerUrl = BasicServerUrl;
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

void UAccelByteBlueprintsServerSettings::SetUGCServerUrl(const FString& UGCServerUrl)
{
	FRegistry::ServerSettings.UGCServerUrl = UGCServerUrl;
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

void UAccelByteBlueprintsServerSettings::SetSendPredefinedEvent(bool bEnabled)
{
	FRegistry::ServerSettings.bSendPredefinedEvent = bEnabled;
}

void UAccelByteBlueprintsServerSettings::ResetSettings(ESettingsEnvironment const Environment)
{
	FRegistry::ServerSettings.Reset(Environment);
	FRegistry::ServerCredentials.SetClientCredentials(FRegistry::ServerSettings.ClientId, FRegistry::ServerSettings.ClientSecret);
}
