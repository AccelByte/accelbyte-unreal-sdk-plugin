// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteSettings.h"
#include "Core/AccelByteRegistry.h"

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

	if (GConfig->GetString(*SectionPath, *Key, Value, GEngineIni)) return Value;

	return FString::Printf(TEXT("%s/%s"), *BaseUrl, *DefaultPrefix);
}

void Settings::LoadSettings(const FString& SectionPath)
{
	if (GConfig->GetString(*SectionPath, TEXT("ClientId"), ClientId, GEngineIni))
	{
		GConfig->GetString(*SectionPath, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
	else
	{
		GConfig->GetString(*DefaultSection, TEXT("ClientId"), ClientId, GEngineIni);
		GConfig->GetString(*DefaultSection, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
	LoadFallback(SectionPath, TEXT("Namespace"), Namespace);
	LoadFallback(SectionPath, TEXT("BaseUrl"), BaseUrl);
	LoadFallback(SectionPath, TEXT("PublisherNamespace"), PublisherNamespace);
	LoadFallback(SectionPath, TEXT("RedirectURI"), RedirectURI);

	IamServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("IamServerUrl"), BaseUrl, TEXT("iam"));

	PlatformServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("PlatformServerUrl"), BaseUrl, TEXT("platform"));
	
	const FString LobbyBaseUrl = BaseUrl.Replace(TEXT("https://"), TEXT("wss://"));
	LobbyServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("LobbyServerUrl"), LobbyBaseUrl, TEXT("lobby/"));
	
	BasicServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("BasicServerUrl"), BaseUrl, TEXT("basic"));

	CloudStorageServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("CloudStorageServerUrl"), BaseUrl, TEXT("social"));

	GameProfileServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("GameProfileServerUrl"), BaseUrl, TEXT("social"));

	GroupServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("GroupServerUrl"), BaseUrl, TEXT("group"));

	StatisticServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("StatisticServerUrl"), BaseUrl, TEXT("social"));

	QosManagerServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("QosManagerServerUrl"), BaseUrl, TEXT("qosm"));

	LeaderboardServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("LeaderboardServerUrl"), BaseUrl, TEXT("leaderboard"));

	GameTelemetryServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("GameTelemetryServerUrl"), BaseUrl, TEXT("game-telemetry"));

	AgreementServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("AgreementServerUrl"), BaseUrl, TEXT("agreement"));

	CloudSaveServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("CloudSaveServerUrl"), BaseUrl, TEXT("cloudsave"));

	AchievementServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("AchievementServerUrl"), BaseUrl, TEXT("achievement"));

	SessionBrowserServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("SessionBrowserServerUrl"), BaseUrl, TEXT("sessionbrowser"));

	UGCServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("UGCServerUrl"), BaseUrl, TEXT("ugc"));
	
	SeasonPassServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("SeasonPassServerUrl"), BaseUrl, TEXT("seasonpass"));

	ReportingServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("ReportingServerUrl"), BaseUrl, TEXT("reporting"));

	SessionServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("SessionServerUrl"), BaseUrl, TEXT("session"));
	
	MatchmakingV2ServerUrl = GetClientConfigUrlValue(SectionPath, TEXT("MatchmakingV2ServerUrl"), BaseUrl, TEXT("match2"));

	LoadFallback(SectionPath, TEXT("AppId"), AppId);

	FString QosLatencyPollIntervalSecsString;
	LoadFallback(SectionPath, TEXT("QosLatencyPollIntervalSecs"), QosLatencyPollIntervalSecsString);
	if (QosLatencyPollIntervalSecsString.IsNumeric())
	{
		QosLatencyPollIntervalSecs = FCString::Atof(*QosLatencyPollIntervalSecsString);
	}
	else
	{
		QosLatencyPollIntervalSecs = 0;
	}

	FString QosServerLatencyPollIntervalSecsString;
	LoadFallback(SectionPath, TEXT("QosServerLatencyPollIntervalSecs"), QosServerLatencyPollIntervalSecsString);
	if (QosServerLatencyPollIntervalSecsString.IsNumeric())
	{
		QosServerLatencyPollIntervalSecs = FCString::Atof(*QosServerLatencyPollIntervalSecsString);
	}
	else
	{
		QosServerLatencyPollIntervalSecs = 0;
	}

	FString QosPingTimeoutString;
	LoadFallback(SectionPath, TEXT("QosPingTimeout"), QosPingTimeoutString);
	if (QosPingTimeoutString.IsNumeric())
	{
		QosPingTimeout = FCString::Atof(*QosPingTimeoutString);
	}
	else
	{
		QosPingTimeout = .6f;
	}
}

void Settings::LoadFallback(const FString& SectionPath, const FString& Key, FString& Value)
{
	if (!GConfig->GetString(*SectionPath, *Key, Value, GEngineIni))
	{
		GConfig->GetString(*DefaultSection, *Key, Value, GEngineIni);
	}
}

void Settings::Reset(ESettingsEnvironment const Environment)
{
	FString SectionPath;
	switch (Environment)
	{
	case ESettingsEnvironment::Development :
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsDev");
		break;
	case ESettingsEnvironment::Certification:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsCert");
		break;
	case ESettingsEnvironment::Production:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsProd");
		break;
	case ESettingsEnvironment::Default:
	default:
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

FString UAccelByteBlueprintsSettings::GetAppId()
{
	return FRegistry::Settings.AppId;
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

void UAccelByteBlueprintsSettings::ResetSettings(const ESettingsEnvironment Environment)
{
	FRegistry::Settings.Reset(Environment);
	FRegistry::Credentials.SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);
}
