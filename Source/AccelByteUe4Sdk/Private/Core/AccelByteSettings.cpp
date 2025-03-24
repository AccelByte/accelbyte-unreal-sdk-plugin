// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteSettings.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteUtilities.h"

using namespace AccelByte;

UAccelByteSettings::UAccelByteSettings()
{
}

UAccelByteSettingsDev::UAccelByteSettingsDev()
{
}

UAccelByteSettingsCert::UAccelByteSettingsCert()
{
}

UAccelByteSettingsProd::UAccelByteSettingsProd()
{
}

static const FString DefaultSection = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettings");

FString GetClientConfigUrlValue(const FString& SectionPath, const FString& Key, const FString& BaseUrl, const FString& DefaultPrefix)
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

void Settings::LoadSettings(const FString& SectionPath)
{
	if (!FAccelByteUtilities::LoadABConfigFallback(SectionPath, "ClientId", ClientId, DefaultSection))
	{
		ClientId = TEXT("");
	}
	if (!FAccelByteUtilities::LoadABConfigFallback(SectionPath, "ClientSecret", ClientSecret, DefaultSection))
	{
		ClientSecret = TEXT("");
	}

	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("Namespace"), Namespace, DefaultSection);
	LoadBaseUrlFallback(SectionPath, BaseUrl);
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("PublisherNamespace"), PublisherNamespace, DefaultSection);
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("RedirectURI"), RedirectURI, DefaultSection);

	IamServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("IamServerUrl"), BaseUrl, TEXT("iam"));

	PlatformServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("PlatformServerUrl"), BaseUrl, TEXT("platform"));
	
	const FString WssBaseUrl = BaseUrl.Replace(TEXT("https://"), TEXT("wss://"));
	LobbyServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("LobbyServerUrl"), WssBaseUrl, TEXT("lobby/"));

	ChatServerWsUrl = GetClientConfigUrlValue(SectionPath, TEXT("ChatServerUrl"), WssBaseUrl, TEXT("chat/"));

	ChatServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("ChatServerUrl"), BaseUrl, TEXT("chat"));
	
	BasicServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("BasicServerUrl"), BaseUrl, TEXT("basic"));

	CloudStorageServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("CloudStorageServerUrl"), BaseUrl, TEXT("social"));

	GameProfileServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("GameProfileServerUrl"), BaseUrl, TEXT("social"));

	GroupServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("GroupServerUrl"), BaseUrl, TEXT("group"));

	StatisticServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("StatisticServerUrl"), BaseUrl, TEXT("social"));

	FString ServerUseAMSString{};
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("bServerUseAMS"), ServerUseAMSString, DefaultSection);
	this->bServerUseAMS = ServerUseAMSString.IsEmpty() ? false : ServerUseAMSString.ToBool();
	FString QoSMDefaultPrefix = this->bServerUseAMS ? TEXT("ams-qosm") : TEXT("qosm");
	QosManagerServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("QosManagerServerUrl"), BaseUrl, QoSMDefaultPrefix);

	LeaderboardServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("LeaderboardServerUrl"), BaseUrl, TEXT("leaderboard"));

	GameTelemetryServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("GameTelemetryServerUrl"), BaseUrl, TEXT("game-telemetry"));

	AgreementServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("AgreementServerUrl"), BaseUrl, TEXT("agreement"));

	CloudSaveServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("CloudSaveServerUrl"), BaseUrl, TEXT("cloudsave"));

	AchievementServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("AchievementServerUrl"), BaseUrl, TEXT("achievement"));

	SessionBrowserServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("SessionBrowserServerUrl"), BaseUrl, TEXT("sessionbrowser"));

	UGCServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("UGCServerUrl"), BaseUrl, TEXT("ugc"));

	InventoryServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("InventoryServerUrl"), BaseUrl, TEXT("inventory"));
	
	SeasonPassServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("SeasonPassServerUrl"), BaseUrl, TEXT("seasonpass"));

	ReportingServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("ReportingServerUrl"), BaseUrl, TEXT("reporting"));

	SessionServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("SessionServerUrl"), BaseUrl, TEXT("session"));

	SessionHistoryServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("SessionHistoryServerUrl"), BaseUrl, TEXT("sessionhistory"));
	
	MatchmakingV2ServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("MatchmakingV2ServerUrl"), BaseUrl, TEXT("match2"));

	GDPRServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("GDPRServerUrl"), BaseUrl, TEXT("gdpr"));

	AMSServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("AMSServerUrl"), BaseUrl, TEXT("ams"));

	ChallengeServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("ChallengeServerUrl"), BaseUrl, TEXT("challenge"));

	ConfigServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("ConfigServerUrl"), BaseUrl, TEXT("config"));
	
	LoginQueueServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("LoginQueueServerUrl"), BaseUrl, TEXT("login-queue"));

	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("AppId"), AppId, DefaultSection);

	if (!FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("CustomerName"), CustomerName, DefaultSection))
	{
		CustomerName = PublisherNamespace;
	}
	
	TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField("customer", CustomerName);
	JsonObject->SetStringField("pn", PublisherNamespace);
	JsonObject->SetStringField("environment", SettingsEnvironment);
	JsonObject->SetStringField("game", Namespace);

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&HeartBeatData);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	// if QosLatencyPollIntervalSecs not set in settings, set QoS latency poll to minimum value 
	FString QosLatencyPollIntervalSecsString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("QosLatencyPollIntervalSecs"), QosLatencyPollIntervalSecsString, DefaultSection);
	if (QosLatencyPollIntervalSecsString.IsNumeric())
	{
		QosLatencyPollIntervalSecs = FCString::Atof(*QosLatencyPollIntervalSecsString);
	}
	else
	{
		QosLatencyPollIntervalSecs = MinNumSecsQosLatencyPolling;
	}

	FString QosServerLatencyPollIntervalSecsString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("QosServerLatencyPollIntervalSecs"), QosServerLatencyPollIntervalSecsString, DefaultSection);
	if (QosServerLatencyPollIntervalSecsString.IsNumeric())
	{
		QosServerLatencyPollIntervalSecs = FCString::Atof(*QosServerLatencyPollIntervalSecsString);
	}
	else
	{
		QosServerLatencyPollIntervalSecs = 0;
	}

	FString bDisableAutoGetQosLatenciesString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("DisableAutoGetQosLatencies"), bDisableAutoGetQosLatenciesString, DefaultSection);
	bDisableAutoGetQosLatencies = bDisableAutoGetQosLatenciesString.IsEmpty() ? false : bDisableAutoGetQosLatenciesString.ToBool();

	FString QosPingTimeoutString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("QosPingTimeout"), QosPingTimeoutString, DefaultSection);
	if (QosPingTimeoutString.IsNumeric())
	{
		QosPingTimeout = FCString::Atof(*QosPingTimeoutString);
	}
	else
	{
		QosPingTimeout = .6f;
	}

	//If configuration value is empty/not found, assume the caching is disabled
	bool bEnableHttpCacheString = false;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("bEnableHttpCache"), bEnableHttpCacheString, DefaultSection);
	this->bEnableHttpCache = bEnableHttpCacheString;

	FString HttpCacheTypeString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("HttpCacheType"), HttpCacheTypeString, DefaultSection);
	this->HttpCacheType = FAccelByteUtilities::GetUEnumValueFromString<EHttpCacheType>(HttpCacheTypeString);
	// Currently non-Windows platform only able to cache in Memory
	// Windows platform able to cache in both Memory & Storage
#if !PLATFORM_WINDOWS
	this->HttpCacheType = EHttpCacheType::MEMORY;
#endif

	FString PresenceBroadcastEventHeartbeatIntervalString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("PresenceBroadcastEventHeartbeatInterval"), PresenceBroadcastEventHeartbeatIntervalString, DefaultSection);
	if (PresenceBroadcastEventHeartbeatIntervalString.IsNumeric())
	{
		PresenceBroadcastEventHeartbeatInterval = FCString::Atoi(*PresenceBroadcastEventHeartbeatIntervalString);
	}

	FString PresenceBroadcastEventHeartbeatEnabledString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("PresenceBroadcastEventHeartbeatEnabled"), PresenceBroadcastEventHeartbeatEnabledString, DefaultSection);
	bEnablePresenceBroadcastEventHeartbeat = PresenceBroadcastEventHeartbeatEnabledString.IsEmpty() ? false : PresenceBroadcastEventHeartbeatEnabledString.ToBool();

	FString GameTelemetryCacheEnabledString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("GameTelemetryCacheEnabled"), GameTelemetryCacheEnabledString, DefaultSection);
	this->bEnableGameTelemetryCache = GameTelemetryCacheEnabledString.IsEmpty() ? true : GameTelemetryCacheEnabledString.ToBool();// the default is true
	
	FString SendPredefinedEventString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("SendPredefinedEvent"), SendPredefinedEventString, DefaultSection);
	//Disabling PredefinedEventApi for the time being
	//this->bSendPredefinedEvent = SendPredefinedEventString.IsEmpty() ? false : SendPredefinedEventString.ToBool();
	this->bSendPredefinedEvent = false;
}

void Settings::LoadFallback(const FString& SectionPath, const FString& Key, FString& Value)
{
	if (!FAccelByteUtilities::GetAccelByteConfigFromCommandLineSwitch(Key, Value))
	{
		if (!GConfig->GetString(*SectionPath, *Key, Value, GEngineIni))
		{
			GConfig->GetString(*DefaultSection, *Key, Value, GEngineIni);
		}
	}
}

void Settings::LoadBaseUrlFallback(const FString& SectionPath, FString& Value)
{
	FAccelByteUtilities::LoadABConfigFallback(*SectionPath, TEXT("BaseUrl"), Value, DefaultSection);

	FRegexPattern UrlRegex(TEXT("^https?:\\/\\/(?:www\\.)?[-a-zA-Z0-9@:%._\\+~#=]{1,256}\\.[a-zA-Z0-9()]{1,6}\\b(?:[-a-zA-Z0-9()@:%_\\+.~#?&\\/=]*)$"));
	FRegexMatcher Matcher(UrlRegex, Value);

	if(Matcher.FindNext())
	{
		while (Value.EndsWith(TEXT("/")))
		{
			Value.RemoveAt(Value.Len() - 1);
		}
	}
	else
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Invalid Base URL: %s"), *Value);
	}
}

void Settings::Reset(ESettingsEnvironment const Environment)
{
	FString SectionPath;
	switch (Environment)
	{
	case ESettingsEnvironment::Development :
		SettingsEnvironment = "dev";
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsDev");
		break;
	case ESettingsEnvironment::Certification:
		SettingsEnvironment = "cert";
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsCert");
		break;
	case ESettingsEnvironment::Production:
		SettingsEnvironment = "prod";
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsProd");
		break;
	case ESettingsEnvironment::Sandbox:
		SettingsEnvironment = "sandbox";
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsSandbox");
		break;
	case ESettingsEnvironment::Integration:
		SettingsEnvironment = "integration";
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsIntegration");
		break;
	case ESettingsEnvironment::QA:
		SettingsEnvironment = "qa";
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsQA");
		break;
	case ESettingsEnvironment::Default:
	default:
		SettingsEnvironment = "default";
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettings");
		break;
	}
	
	LoadSettings(SectionPath);
}