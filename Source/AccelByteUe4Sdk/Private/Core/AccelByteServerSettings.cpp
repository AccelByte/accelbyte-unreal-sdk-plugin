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

static FString GetDefaultServerAPIUrl(FString const& SpecificServerUrl, FString const& BaseUrl, FString const& DefaultServerPath)
{
	if (SpecificServerUrl.IsEmpty())
	{
		return FString::Printf(TEXT("%s/%s"), *BaseUrl, *DefaultServerPath);
	}

	return SpecificServerUrl;
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

	GConfig->GetString(*SectionPath, TEXT("IamServerUrl"), IamServerUrl, GEngineIni);
	IamServerUrl = GetDefaultServerAPIUrl(IamServerUrl, BaseUrl, TEXT("iam"));

	GConfig->GetString(*SectionPath, TEXT("DSMControllerServerUrl"), DSMControllerServerUrl, GEngineIni);
	DSMControllerServerUrl = GetDefaultServerAPIUrl(DSMControllerServerUrl, BaseUrl, TEXT("dsmcontroller"));

	GConfig->GetString(*SectionPath, TEXT("StatisticServerUrl"), StatisticServerUrl, GEngineIni);
	StatisticServerUrl = GetDefaultServerAPIUrl(StatisticServerUrl, BaseUrl, TEXT("social"));

	GConfig->GetString(*SectionPath, TEXT("PlatformServerUrl"), PlatformServerUrl, GEngineIni);
	PlatformServerUrl = GetDefaultServerAPIUrl(PlatformServerUrl, BaseUrl, TEXT("platform"));

	GConfig->GetString(*SectionPath, TEXT("QosManagerServerUrl"), QosManagerServerUrl, GEngineIni);
	QosManagerServerUrl = GetDefaultServerAPIUrl(QosManagerServerUrl, BaseUrl, TEXT("qosm"));

	GConfig->GetString(*SectionPath, TEXT("GameTelemetryServerUrl"), GameTelemetryServerUrl, GEngineIni);
	GameTelemetryServerUrl = GetDefaultServerAPIUrl(GameTelemetryServerUrl, BaseUrl, TEXT("game-telemetry"));

	GConfig->GetString(*SectionPath, TEXT("AchievementServerUrl"), AchievementServerUrl, GEngineIni);
	AchievementServerUrl = GetDefaultServerAPIUrl(AchievementServerUrl, BaseUrl, TEXT("achievement"));

	GConfig->GetString(*SectionPath, TEXT("MatchmakingServerUrl"), MatchmakingServerUrl, GEngineIni);
	MatchmakingServerUrl = GetDefaultServerAPIUrl(MatchmakingServerUrl, BaseUrl, TEXT("matchmaking"));

	GConfig->GetString(*SectionPath, TEXT("LobbyServerUrl"), LobbyServerUrl, GEngineIni);
	LobbyServerUrl = GetDefaultServerAPIUrl(LobbyServerUrl, BaseUrl, TEXT("lobby"));

	GConfig->GetString(*SectionPath, TEXT("CloudSaveServerUrl"), CloudSaveServerUrl, GEngineIni);
	CloudSaveServerUrl = GetDefaultServerAPIUrl(CloudSaveServerUrl, BaseUrl, TEXT("cloudsave"));

	GConfig->GetString(*SectionPath, TEXT("SeasonPassServerUrl"), SeasonPassServerUrl, GEngineIni);
	SeasonPassServerUrl = GetDefaultServerAPIUrl(SeasonPassServerUrl, BaseUrl, TEXT("seasonpass"));

	GConfig->GetString(*SectionPath, TEXT("SessionBrowserServerUrl"), SessionBrowserServerUrl, GEngineIni);
	SessionBrowserServerUrl = GetDefaultServerAPIUrl(SessionBrowserServerUrl, BaseUrl, TEXT("sessionbrowser"));

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
