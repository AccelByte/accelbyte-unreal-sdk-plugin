// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteSettings.h"
#include "Core/AccelByteRegistry.h"
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

	FString QosLatencyPollIntervalSecsString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("QosLatencyPollIntervalSecs"), QosLatencyPollIntervalSecsString, DefaultSection);
	if (QosLatencyPollIntervalSecsString.IsNumeric())
	{
		QosLatencyPollIntervalSecs = FCString::Atof(*QosLatencyPollIntervalSecsString);
	}
	else
	{
		QosLatencyPollIntervalSecs = 0;
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
	FString bEnableHttpCacheString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("bEnableHttpCache"), bEnableHttpCacheString, DefaultSection);
	this->bEnableHttpCache = bEnableHttpCacheString.IsEmpty() ? false : bEnableHttpCacheString.ToBool();

	FString HttpCacheTypeString;
	FAccelByteUtilities::LoadABConfigFallback(SectionPath, TEXT("HttpCacheType"), HttpCacheTypeString, DefaultSection);
	this->HttpCacheType = FAccelByteUtilities::GetUEnumValueFromString<EHttpCacheType>(HttpCacheTypeString);

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

FString UAccelByteBlueprintsSettings::GetClientId()
{
	return FRegistry::Settings.ClientId;
}

FString UAccelByteBlueprintsSettings::GetClientSecret()
{
	return FRegistry::Settings.ClientSecret;
}

FString UAccelByteBlueprintsSettings::GetNamespace()
{
	return FRegistry::Settings.Namespace;
}

FString UAccelByteBlueprintsSettings::GetPublisherNamespace()
{
	return FRegistry::Settings.PublisherNamespace;
}

FString UAccelByteBlueprintsSettings::GetIamServerUrl()
{
	return FRegistry::Settings.IamServerUrl;
}

FString UAccelByteBlueprintsSettings::GetPlatformServerUrl()
{
	return FRegistry::Settings.PlatformServerUrl;
}

FString UAccelByteBlueprintsSettings::GetLobbyServerUrl()
{
	return FRegistry::Settings.LobbyServerUrl;
}

FString UAccelByteBlueprintsSettings::GetChatServerUrl()
{
	return FRegistry::Settings.ChatServerWsUrl;
}

FString UAccelByteBlueprintsSettings::GetCloudStorageServerUrl()
{
    return FRegistry::Settings.CloudStorageServerUrl;
}

FString UAccelByteBlueprintsSettings::GetBasicServerUrl()
{
	return FRegistry::Settings.BasicServerUrl;
}

FString UAccelByteBlueprintsSettings::GetGameProfileServerUrl()
{
	return FRegistry::Settings.GameProfileServerUrl;
}

FString UAccelByteBlueprintsSettings::GetGroupServerUrl()
{
	return FRegistry::Settings.GroupServerUrl;
}

FString UAccelByteBlueprintsSettings::GetStatisticServerUrl()
{
	return FRegistry::Settings.StatisticServerUrl;
}

FString UAccelByteBlueprintsSettings::GetQosManagerServerUrl()
{
	return FRegistry::Settings.QosManagerServerUrl;
}

FString UAccelByteBlueprintsSettings::GetLeaderboardServerUrl()
{
	return FRegistry::Settings.LeaderboardServerUrl;
}

FString UAccelByteBlueprintsSettings::GetCloudSaveServerUrl()
{
	return FRegistry::Settings.CloudSaveServerUrl;
}

FString UAccelByteBlueprintsSettings::GetGameTelemetryServerUrl()
{
	return FRegistry::Settings.GameTelemetryServerUrl;
}

FString UAccelByteBlueprintsSettings::GetAchievementServerUrl()
{
	return FRegistry::Settings.AchievementServerUrl;
}

FString UAccelByteBlueprintsSettings::GetSessionBrowserServerUrl()
{
	return FRegistry::Settings.SessionBrowserServerUrl;
}

FString UAccelByteBlueprintsSettings::GetTurnManagerServerUrl()
{
	return FRegistry::Settings.TurnManagerServerUrl;
}

FString UAccelByteBlueprintsSettings::GetUGCServerUrl()
{
	return FRegistry::Settings.UGCServerUrl;
}

FString UAccelByteBlueprintsSettings::GetInventoryServerUrl()
{
	return FRegistry::Settings.InventoryServerUrl;
}

FString UAccelByteBlueprintsSettings::GetReportingServerUrl()
{
	return FRegistry::Settings.ReportingServerUrl;
}

FString UAccelByteBlueprintsSettings::GetSessionServerUrl()
{
	return FRegistry::Settings.SessionServerUrl;
}

FString UAccelByteBlueprintsSettings::GetMatchmakingV2ServerUrl()
{
	return FRegistry::Settings.MatchmakingV2ServerUrl;
}

FString UAccelByteBlueprintsSettings::GetGDPRServerUrl()
{
	return FRegistry::Settings.GDPRServerUrl;
}

FString UAccelByteBlueprintsSettings::GetChallengeServerUrl()
{
	return FRegistry::Settings.ChallengeServerUrl;
}

FString UAccelByteBlueprintsSettings::GetLoginQueueServerUrl()
{
	return FRegistry::Settings.LoginQueueServerUrl;
}

FString UAccelByteBlueprintsSettings::GetAppId()
{
	return FRegistry::Settings.AppId;
}

FString UAccelByteBlueprintsSettings::GetCustomerName()
{
	return FRegistry::Settings.CustomerName;
}

FString UAccelByteBlueprintsSettings::GetHeartBeatData()
{
	return FRegistry::Settings.HeartBeatData;
}

float UAccelByteBlueprintsSettings::GetQosLatencyPollIntervalSecs()
{
	return FRegistry::Settings.QosLatencyPollIntervalSecs;
}

float UAccelByteBlueprintsSettings::GetQosServerLatencyPollIntervalSecs()
{
	return FRegistry::Settings.QosServerLatencyPollIntervalSecs;
}

float UAccelByteBlueprintsSettings::GetQosPingTimeout()
{
	return FRegistry::Settings.QosPingTimeout;
}

bool UAccelByteBlueprintsSettings::IsHttpCacheEnabled()
{
	return FRegistry::Settings.bEnableHttpCache;
}

EHttpCacheType UAccelByteBlueprintsSettings::GetHttpCacheType()
{
	return FRegistry::Settings.HttpCacheType;
}

bool UAccelByteBlueprintsSettings::IsSendPredefinedEvent()
{
	return FRegistry::Settings.bSendPredefinedEvent;
}

bool UAccelByteBlueprintsSettings::IsServerUseAMS()
{
	return FRegistry::Settings.bServerUseAMS;
}

bool UAccelByteBlueprintsSettings::IsGameTelemetryCacheEnabled()
{
	return FRegistry::Settings.bEnableGameTelemetryCache;
}

void UAccelByteBlueprintsSettings::SetClientId(const FString& ClientId)
{
	FRegistry::Settings.ClientId = ClientId;
}

void UAccelByteBlueprintsSettings::SetClientSecret(const FString& ClientSecret)
{
	FRegistry::Settings.ClientSecret = ClientSecret;
}

void UAccelByteBlueprintsSettings::SetNamespace(const FString& Namespace)
{
	FRegistry::Settings.Namespace = Namespace;
}

void UAccelByteBlueprintsSettings::SetPublisherNamespace(const FString& PublisherNamespace)
{
	FRegistry::Settings.PublisherNamespace = PublisherNamespace;
}

void UAccelByteBlueprintsSettings::SetIamServerUrl(const FString& IamServerUrl)
{
	FRegistry::Settings.IamServerUrl = IamServerUrl;
}

void UAccelByteBlueprintsSettings::SetPlatformServerUrl(const FString& PlatformServerUrl)
{
	FRegistry::Settings.PlatformServerUrl = PlatformServerUrl;
}

void UAccelByteBlueprintsSettings::SetLobbyServerUrl(const FString& LobbyServerUrl)
{
	FRegistry::Settings.LobbyServerUrl = LobbyServerUrl;
}

void UAccelByteBlueprintsSettings::SetChatServerUrl(const FString& ChatServerUrl)
{
	FRegistry::Settings.ChatServerWsUrl = ChatServerUrl;
}

void UAccelByteBlueprintsSettings::SetCloudStorageServerUrl(const FString& CloudStorageServerUrl)
{
	FRegistry::Settings.CloudStorageServerUrl = CloudStorageServerUrl;
}

void UAccelByteBlueprintsSettings::SetBasicServerUrl(const FString& BasicServerUrl)
{
	FRegistry::Settings.BasicServerUrl = BasicServerUrl;
}

void UAccelByteBlueprintsSettings::SetGameProfileServerUrl(const FString& GameProfileServerUrl)
{
	FRegistry::Settings.GameProfileServerUrl = GameProfileServerUrl;
}

void UAccelByteBlueprintsSettings::SetGroupServerUrl(const FString& GroupServerUrl)
{
	FRegistry::Settings.GroupServerUrl = GroupServerUrl;
}

void UAccelByteBlueprintsSettings::SetStatisticServerUrl(const FString& StatisticServerUrl)
{
	FRegistry::Settings.StatisticServerUrl = StatisticServerUrl;
}

void UAccelByteBlueprintsSettings::SetQosManagerServerUrl(const FString& QosManagerServerUrl)
{
	FRegistry::Settings.QosManagerServerUrl = QosManagerServerUrl;
}

void UAccelByteBlueprintsSettings::SetLeaderboardServerUrl(const FString& LeaderboardServerUrl)
{
	FRegistry::Settings.LeaderboardServerUrl = LeaderboardServerUrl;
}

void UAccelByteBlueprintsSettings::SetCloudSaveServerUrl(const FString& CloudSaveServerUrl)
{
	FRegistry::Settings.CloudSaveServerUrl = CloudSaveServerUrl;
}

void UAccelByteBlueprintsSettings::SetGameTelemetryServerUrl(const FString& GameTelemetryServerUrl)
{
	FRegistry::Settings.GameTelemetryServerUrl = GameTelemetryServerUrl;
}

void UAccelByteBlueprintsSettings::SetAchievementServerUrl(const FString& AchievementServerUrl)
{
	FRegistry::Settings.AchievementServerUrl = AchievementServerUrl;
}

void UAccelByteBlueprintsSettings::SetSessionBrowserServerUrl(const FString& SessionBrowserServerUrl)
{
	FRegistry::Settings.SessionBrowserServerUrl = SessionBrowserServerUrl;
}

void UAccelByteBlueprintsSettings::SetTurnManagerServerUrl(const FString& TurnManagerServerUrl)
{
	FRegistry::Settings.TurnManagerServerUrl = TurnManagerServerUrl;
}

void UAccelByteBlueprintsSettings::SetUGCServerUrl(const FString& UGCServerUrl)
{
	FRegistry::Settings.UGCServerUrl = UGCServerUrl;
}

void UAccelByteBlueprintsSettings::SetInventoryServerUrl(const FString& InventoryServerUrl)
{
	FRegistry::Settings.InventoryServerUrl = InventoryServerUrl;
}

void UAccelByteBlueprintsSettings::SetSeasonPassServerUrl(const FString& SeasonPassServerUrl)
{
	FRegistry::Settings.SeasonPassServerUrl = SeasonPassServerUrl;
}

void UAccelByteBlueprintsSettings::SetReportingServerUrl(const FString& ReportingServerUrl)
{
	FRegistry::Settings.ReportingServerUrl = ReportingServerUrl;
}

void UAccelByteBlueprintsSettings::SetSessionServerUrl(const FString& SessionServerUrl)
{
	FRegistry::Settings.SessionServerUrl = SessionServerUrl;
}

void UAccelByteBlueprintsSettings::SetMatchmakingV2ServerUrl(const FString& MatchmakingV2ServerUrl)
{
	FRegistry::Settings.MatchmakingV2ServerUrl = MatchmakingV2ServerUrl;
}

void UAccelByteBlueprintsSettings::SetGDPRServerUrl(const FString& GDPRServerUrl)
{
	FRegistry::Settings.GDPRServerUrl = GDPRServerUrl;
}

void UAccelByteBlueprintsSettings::SetChallengeServerUrl(const FString& ChallengeServerUrl)
{
	FRegistry::Settings.ChallengeServerUrl = ChallengeServerUrl;
}

void UAccelByteBlueprintsSettings::SetLoginQueueServerUrl(const FString& LoginQueueServerUrl)
{
	FRegistry::Settings.LoginQueueServerUrl = LoginQueueServerUrl;
}

void UAccelByteBlueprintsSettings::SetAppId(const FString& AppId)
{
	FRegistry::Settings.AppId = AppId;
}

void UAccelByteBlueprintsSettings::SetQosLatencyPollIntervalSecs(const float& QosLatencyPollIntervalSecs)
{
	FRegistry::Settings.QosLatencyPollIntervalSecs = QosLatencyPollIntervalSecs;
}

void UAccelByteBlueprintsSettings::SetServerQosLatencyPollIntervalSecs(const float& QosServerLatencyPollIntervalSecs)
{
	FRegistry::Settings.QosServerLatencyPollIntervalSecs = QosServerLatencyPollIntervalSecs;
}
	
void UAccelByteBlueprintsSettings::SetIsHttpCacheEnabled(bool bEnable)
{
	FRegistry::Settings.bEnableHttpCache = bEnable;
}

void UAccelByteBlueprintsSettings::SetQosPingTimeout(const float& QosPingTimeout)
{
	if (QosPingTimeout < .6)
	{
		FRegistry::Settings.QosPingTimeout = .6;
	}
	else if (QosPingTimeout > 60)
	{
		FRegistry::Settings.QosPingTimeout = 60;
	}
	else
	{
		FRegistry::Settings.QosPingTimeout = QosPingTimeout;
	}
}

void UAccelByteBlueprintsSettings::SetHttpCacheType(EHttpCacheType Type)
{
	FRegistry::Settings.HttpCacheType = Type;
}

void UAccelByteBlueprintsSettings::SetServerUseAMS(bool bEnable)
{
	FRegistry::Settings.bServerUseAMS = bEnable;
}

void UAccelByteBlueprintsSettings::SetSendPredefinedEvent(bool bEnable)
{
	FRegistry::Settings.bSendPredefinedEvent = bEnable;
}

void UAccelByteBlueprintsSettings::SetEnableGameTelemetryCache(bool bEnable)
{
	FRegistry::Settings.bEnableGameTelemetryCache = bEnable;
}

void UAccelByteBlueprintsSettings::ResetSettings(const ESettingsEnvironment Environment)
{
	FRegistry::Settings.Reset(Environment);
	FRegistry::CredentialsRef->SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);
}
