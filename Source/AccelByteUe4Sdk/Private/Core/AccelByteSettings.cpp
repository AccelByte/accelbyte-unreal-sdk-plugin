// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
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

template< class T >
static bool SetEnvironment()
{
	FRegistry::Settings.ClientId = GetDefault<T>()->ClientId;
	FRegistry::Settings.ClientSecret = GetDefault<T>()->ClientSecret;
	FRegistry::Settings.Namespace = GetDefault<T>()->Namespace;
	FRegistry::Settings.PublisherNamespace = GetDefault<T>()->PublisherNamespace;
	FRegistry::Settings.RedirectURI = GetDefault<T>()->RedirectURI;
	FRegistry::Settings.BaseUrl = GetDefault<T>()->BaseUrl;
	FRegistry::Settings.IamServerUrl = GetDefaultAPIUrl(GetDefault<T>()->IamServerUrl, TEXT("iam"));
	FRegistry::Settings.PlatformServerUrl = GetDefaultAPIUrl(GetDefault<T>()->PlatformServerUrl, TEXT("platform"));
	FRegistry::Settings.LobbyServerUrl = GetDefault<T>()->LobbyServerUrl;
	if (FRegistry::Settings.LobbyServerUrl.IsEmpty())
	{
		const FString BaseUrl = FRegistry::Settings.BaseUrl.Replace(TEXT("https://"), TEXT("wss://"));
		FRegistry::Settings.LobbyServerUrl = FString::Printf(TEXT("%s/%s"), *BaseUrl, TEXT("lobby/"));
	}
	FRegistry::Settings.BasicServerUrl = GetDefaultAPIUrl(GetDefault<T>()->BasicServerUrl, TEXT("basic"));
	FRegistry::Settings.CloudStorageServerUrl = GetDefaultAPIUrl(GetDefault<T>()->CloudStorageServerUrl, TEXT("social"));
	FRegistry::Settings.GameProfileServerUrl = GetDefaultAPIUrl(GetDefault<T>()->GameProfileServerUrl, TEXT("social"));
	FRegistry::Settings.StatisticServerUrl = GetDefaultAPIUrl(GetDefault<T>()->StatisticServerUrl, TEXT("social"));
	FRegistry::Settings.QosManagerServerUrl = GetDefaultAPIUrl(GetDefault<T>()->QosManagerServerUrl, TEXT("qosm"));
	FRegistry::Settings.LeaderboardServerUrl = GetDefaultAPIUrl(GetDefault<T>()->LeaderboardServerUrl, TEXT("leaderboard"));
	FRegistry::Settings.GameTelemetryServerUrl = GetDefaultAPIUrl(GetDefault<T>()->GameTelemetryServerUrl, TEXT("game-telemetry"));
	FRegistry::Settings.AgreementServerUrl = GetDefaultAPIUrl(GetDefault<T>()->AgreementServerUrl, TEXT("agreement"));
	FRegistry::Settings.CloudSaveServerUrl = GetDefaultAPIUrl(GetDefault<T>()->CloudSaveServerUrl, TEXT("cloudsave"));
	FRegistry::Settings.AchievementServerUrl = GetDefaultAPIUrl(GetDefault<T>()->AchievementServerUrl, TEXT("achievement"));
	FRegistry::Settings.SessionBrowserServerUrl = GetDefaultAPIUrl(GetDefault<T>()->SessionBrowserServerUrl, TEXT("sessionbrowser"));
	FRegistry::Settings.UGCServerUrl = GetDefaultAPIUrl(GetDefault<T>()->UGCServerUrl, TEXT("ugc"));
	FRegistry::Settings.SeasonPassServerUrl = GetDefaultAPIUrl(GetDefault<T>()->UGCServerUrl, TEXT("seasonpass"));
	FRegistry::Settings.ReportingServerUrl = GetDefaultAPIUrl(GetDefault<T>()->ReportingServerUrl, TEXT("reporting"));
	FRegistry::Settings.AppId = GetDefault<T>()->AppId;
	FRegistry::Credentials.SetClientCredentials(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret);

	return true;
}

void Settings::Reset(const ESettingsEnvironment& Environment)
{
	switch (Environment)
	{
	case ESettingsEnvironment::Development : SetEnvironment<UAccelByteSettingsDev>();
		break;
	case ESettingsEnvironment::Certification: SetEnvironment<UAccelByteSettingsCert>();
		break;
	case ESettingsEnvironment::Production: SetEnvironment<UAccelByteSettingsProd>();
		break;
	case ESettingsEnvironment::Default:
	default:
		SetEnvironment<UAccelByteSettings>();
		break;
	}
}

static FString GetDefaultAPIUrl(const FString& SpecificServerUrl, const FString& DefaultServerPath)
{
	if (SpecificServerUrl.IsEmpty())
	{
		return FString::Printf(TEXT("%s/%s"), *FRegistry::Settings.BaseUrl, *DefaultServerPath);
	}

	return SpecificServerUrl;
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

void UAccelByteBlueprintsSettings::ResetSettings(const TEnumAsByte<ESettingsEnvironment>& Environment)
{
	FRegistry::Settings.Reset(Environment);
}
