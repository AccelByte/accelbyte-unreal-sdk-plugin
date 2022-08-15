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

static FString GetDefaultClientAPIUrl(FString const& SpecificServerUrl, FString const& BaseUrl, FString const& DefaultServerPath)
{
	if (SpecificServerUrl.IsEmpty())
	{
		return FString::Printf(TEXT("%s/%s"), *BaseUrl, *DefaultServerPath);
	}

	return SpecificServerUrl;
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

	GConfig->GetString(*SectionPath, TEXT("IamServerUrl"), IamServerUrl, GEngineIni);
	IamServerUrl = GetDefaultClientAPIUrl(IamServerUrl, BaseUrl, TEXT("iam"));

	GConfig->GetString(*SectionPath, TEXT("PlatformServerUrl"), PlatformServerUrl, GEngineIni);
	PlatformServerUrl = GetDefaultClientAPIUrl(PlatformServerUrl, BaseUrl, TEXT("platform"));
	
	GConfig->GetString(*SectionPath, TEXT("LobbyServerUrl"), LobbyServerUrl, GEngineIni);
	if (LobbyServerUrl.IsEmpty())
	{
		const FString LobbyBaseUrl = BaseUrl.Replace(TEXT("https://"), TEXT("wss://"));
		LobbyServerUrl = FString::Printf(TEXT("%s/%s"), *LobbyBaseUrl, TEXT("lobby/"));
	}

	GConfig->GetString(*SectionPath, TEXT("BasicServerUrl"), BasicServerUrl, GEngineIni);
	BasicServerUrl = GetDefaultClientAPIUrl(BasicServerUrl, BaseUrl, TEXT("basic"));

	GConfig->GetString(*SectionPath, TEXT("CloudStorageServerUrl"), CloudStorageServerUrl, GEngineIni);
	CloudStorageServerUrl = GetDefaultClientAPIUrl(CloudStorageServerUrl, BaseUrl, TEXT("social"));

	GConfig->GetString(*SectionPath, TEXT("GameProfileServerUrl"), GameProfileServerUrl, GEngineIni);
	GameProfileServerUrl = GetDefaultClientAPIUrl(GameProfileServerUrl, BaseUrl, TEXT("social"));

	GConfig->GetString(*SectionPath, TEXT("GroupServerUrl"), GroupServerUrl, GEngineIni);
	GroupServerUrl = GetDefaultClientAPIUrl(GroupServerUrl, BaseUrl, TEXT("group"));

	GConfig->GetString(*SectionPath, TEXT("StatisticServerUrl"), StatisticServerUrl, GEngineIni);
	StatisticServerUrl = GetDefaultClientAPIUrl(StatisticServerUrl, BaseUrl, TEXT("social"));

	GConfig->GetString(*SectionPath, TEXT("QosManagerServerUrl"), QosManagerServerUrl, GEngineIni);
	QosManagerServerUrl = GetDefaultClientAPIUrl(QosManagerServerUrl, BaseUrl, TEXT("qosm"));

	GConfig->GetString(*SectionPath, TEXT("LeaderboardServerUrl"), LeaderboardServerUrl, GEngineIni);
	LeaderboardServerUrl = GetDefaultClientAPIUrl(LeaderboardServerUrl, BaseUrl, TEXT("leaderboard"));

	GConfig->GetString(*SectionPath, TEXT("GameTelemetryServerUrl"), GameTelemetryServerUrl, GEngineIni);
	GameTelemetryServerUrl = GetDefaultClientAPIUrl(GameTelemetryServerUrl, BaseUrl, TEXT("game-telemetry"));

	GConfig->GetString(*SectionPath, TEXT("AgreementServerUrl"), AgreementServerUrl, GEngineIni);
	AgreementServerUrl = GetDefaultClientAPIUrl(AgreementServerUrl, BaseUrl, TEXT("agreement"));

	GConfig->GetString(*SectionPath, TEXT("CloudSaveServerUrl"), CloudSaveServerUrl, GEngineIni);
	CloudSaveServerUrl = GetDefaultClientAPIUrl(CloudSaveServerUrl, BaseUrl, TEXT("cloudsave"));

	GConfig->GetString(*SectionPath, TEXT("AchievementServerUrl"), AchievementServerUrl, GEngineIni);
	AchievementServerUrl = GetDefaultClientAPIUrl(AchievementServerUrl, BaseUrl, TEXT("achievement"));

	GConfig->GetString(*SectionPath, TEXT("SessionBrowserServerUrl"), SessionBrowserServerUrl, GEngineIni);
	SessionBrowserServerUrl = GetDefaultClientAPIUrl(SessionBrowserServerUrl, BaseUrl, TEXT("sessionbrowser"));

	GConfig->GetString(*SectionPath, TEXT("UGCServerUrl"), UGCServerUrl, GEngineIni);
	UGCServerUrl = GetDefaultClientAPIUrl(UGCServerUrl, BaseUrl, TEXT("ugc"));
	
	GConfig->GetString(*SectionPath, TEXT("SeasonPassServerUrl"), SeasonPassServerUrl, GEngineIni);
	SeasonPassServerUrl = GetDefaultClientAPIUrl(SeasonPassServerUrl, BaseUrl, TEXT("seasonpass"));

	GConfig->GetString(*SectionPath, TEXT("ReportingServerUrl"), ReportingServerUrl, GEngineIni);
	ReportingServerUrl = GetDefaultClientAPIUrl(ReportingServerUrl, BaseUrl, TEXT("reporting"));

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
		QosPingTimeout = .6;
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
