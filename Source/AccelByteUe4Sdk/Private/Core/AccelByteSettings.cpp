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

static FString GetDefaultClientAPIUrl(FString const& SpecificServerUrl, FString const& BaseUrl, FString const& DefaultServerPath)
{
	if (SpecificServerUrl.IsEmpty())
	{
		return FString::Printf(TEXT("%s/%s"), *BaseUrl, *DefaultServerPath);
	}

	return SpecificServerUrl;
}

template< class T >
static bool SetClientEnvironment(Settings * InSettings)
{
	InSettings->ClientId = GetDefault<T>()->ClientId;
	InSettings->ClientSecret = GetDefault<T>()->ClientSecret;
	InSettings->Namespace = GetDefault<T>()->Namespace;
	InSettings->PublisherNamespace = GetDefault<T>()->PublisherNamespace;
	InSettings->RedirectURI = GetDefault<T>()->RedirectURI;
	InSettings->BaseUrl = GetDefault<T>()->BaseUrl;
	InSettings->IamServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->IamServerUrl, InSettings->BaseUrl, TEXT("iam"));
	InSettings->PlatformServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->PlatformServerUrl, InSettings->BaseUrl, TEXT("platform"));
	InSettings->LobbyServerUrl = GetDefault<T>()->LobbyServerUrl;
	if (InSettings->LobbyServerUrl.IsEmpty())
	{
		const FString BaseUrl = InSettings->BaseUrl.Replace(TEXT("https://"), TEXT("wss://"));
		InSettings->LobbyServerUrl = FString::Printf(TEXT("%s/%s"), *BaseUrl, TEXT("lobby/"));
	}
	InSettings->BasicServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->BasicServerUrl, InSettings->BaseUrl, TEXT("basic"));
	InSettings->CloudStorageServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->CloudStorageServerUrl, InSettings->BaseUrl, TEXT("social"));
	InSettings->GameProfileServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->GameProfileServerUrl, InSettings->BaseUrl, TEXT("social"));
	InSettings->GroupServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->GroupServerUrl, InSettings->BaseUrl, TEXT("group"));
	InSettings->StatisticServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->StatisticServerUrl, InSettings->BaseUrl, TEXT("social"));
	InSettings->QosManagerServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->QosManagerServerUrl, InSettings->BaseUrl, TEXT("qosm"));
	InSettings->LeaderboardServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->LeaderboardServerUrl, InSettings->BaseUrl, TEXT("leaderboard"));
	InSettings->GameTelemetryServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->GameTelemetryServerUrl, InSettings->BaseUrl, TEXT("game-telemetry"));
	InSettings->AgreementServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->AgreementServerUrl, InSettings->BaseUrl, TEXT("agreement"));
	InSettings->CloudSaveServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->CloudSaveServerUrl, InSettings->BaseUrl, TEXT("cloudsave"));
	InSettings->AchievementServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->AchievementServerUrl, InSettings->BaseUrl, TEXT("achievement"));
	InSettings->SessionBrowserServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->SessionBrowserServerUrl, InSettings->BaseUrl, TEXT("sessionbrowser"));
	InSettings->UGCServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->UGCServerUrl, InSettings->BaseUrl, TEXT("ugc"));
	InSettings->SeasonPassServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->UGCServerUrl, InSettings->BaseUrl, TEXT("seasonpass"));
	InSettings->ReportingServerUrl = GetDefaultClientAPIUrl(GetDefault<T>()->ReportingServerUrl, InSettings->BaseUrl, TEXT("reporting"));
	InSettings->AppId = GetDefault<T>()->AppId;

	return true;
}

void Settings::Reset(ESettingsEnvironment const Environment)
{
	switch (Environment)
	{
	case ESettingsEnvironment::Development :
		SetClientEnvironment<UAccelByteSettingsDev>(this);
		break;
	case ESettingsEnvironment::Certification:
		SetClientEnvironment<UAccelByteSettingsCert>(this);
		break;
	case ESettingsEnvironment::Production:
		SetClientEnvironment<UAccelByteSettingsProd>(this);
		break;
	case ESettingsEnvironment::Default:
	default:
		SetClientEnvironment<UAccelByteSettings>(this);
		break;
	}
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

void UAccelByteBlueprintsSettings::SetIsHttpCacheEnabled(bool bEnable)
{
	FRegistry::Settings.bEnableHttpCache = bEnable;
}

void UAccelByteBlueprintsSettings::ResetSettings(const ESettingsEnvironment Environment)
{
	FRegistry::Settings.Reset(Environment);
	FRegistry::Credentials.SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);
}
