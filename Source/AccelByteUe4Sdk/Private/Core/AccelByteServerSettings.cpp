// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteReport.h"

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

	InventoryServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("InventoryServerUrl"), BaseUrl, TEXT("inventory"));

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

	ChallengeServerUrl = GetServerConfigUrlValue(SectionPath, TEXT("ChallengeServerUrl"), BaseUrl, TEXT("challenge"));

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

	FString ServerUseAMSString{};
	// even though this is ServerSettings it's intended to read from previously available bServerUseAMS in AccelByteSettings
	if (FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("bServerUseAMS"), ServerUseAMSString, TEXT("/Script/AccelByteUe4Sdk.AccelByteSettings")))
	{
		this->bServerUseAMS = ServerUseAMSString.IsEmpty() ? true : ServerUseAMSString.ToBool();
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
	bool bIsLoadSuccess = true;
	// Check if the key exist in the commandline of AccelByte format. For example "GameExecutable -abKey=Value" 
	if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(TEXT("watchdog_url"), AMSServerWatchdogUrl))
	{
		if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(TEXT("WatchdogUrl"), AMSServerWatchdogUrl))
		{
			// If not exist, then check without AccelByte format. For example "GameExecutable -Key=Value"
			if (!FAccelByteUtilities::GetValueFromCommandLineSwitch(TEXT("watchdog_url"), AMSServerWatchdogUrl))
			{
				// If not exist, then fetch the setting from game default engine ini file
				if (!GConfig->GetString(*DefaultServerSection, TEXT("WatchdogUrl"), AMSServerWatchdogUrl, GEngineIni))
				{
					UE_LOG(LogAccelByte, Log, TEXT("WatchdogUrl is not defined, default value will be assigned."));
				}
			}
		}
	}

	if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(TEXT("heartbeat"), AMSHeartbeatInterval))
	{
		if (!FAccelByteUtilities::GetValueFromCommandLineSwitch(TEXT("heartbeat"), AMSHeartbeatInterval))
		{
			if (!GConfig->GetInt(*DefaultServerSection, TEXT("AMSHeartbeatInterval"), AMSHeartbeatInterval, GEngineIni))
			{
				// Assigning default AMS heartbeat; the default value is 15 seconds.
				UE_LOG(LogAccelByte, Log, TEXT("Heartbeat is not defined, default value will be assigned."));
				AMSHeartbeatInterval = 15;
			}
		}
	}

	if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(TEXT("dsid"), DSId))
	{
		if (!FAccelByteUtilities::GetValueFromCommandLineSwitch(TEXT("dsid"), DSId))
		{
			UE_LOG(LogAccelByte, Log, TEXT("DS Id is not defined, please add -dsid=${dsid} or -ABDsId ${dsid} in the command line arguments."));

			bIsLoadSuccess = false;
		}
	}

	return bIsLoadSuccess;
}