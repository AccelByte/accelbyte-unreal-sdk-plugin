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

static FString GetDefaultServerAPIUrl(FString const& SpecificServerUrl, FString const& BaseUrl, FString const& DefaultServerPath)
{
	if (SpecificServerUrl.IsEmpty())
	{
		return FString::Printf(TEXT("%s/%s"), *BaseUrl, *DefaultServerPath);
	}

	return SpecificServerUrl;
}

template<class T>
static bool SetServerEnvironment(ServerSettings * InSettings)
{
#if WITH_EDITOR || UE_SERVER || UE_BUILD_DEVELOPMENT
	InSettings->ClientId = GetDefault<T>()->ClientId;
	InSettings->ClientSecret = GetDefault<T>()->ClientSecret;
	InSettings->Namespace = GetDefault<T>()->Namespace;
	InSettings->PublisherNamespace = GetDefault<T>()->PublisherNamespace;
	InSettings->RedirectURI = GetDefault<T>()->RedirectURI;
	InSettings->BaseUrl = GetDefault<T>()->BaseUrl;
	InSettings->IamServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->IamServerUrl, InSettings->BaseUrl, TEXT("iam"));
	InSettings->DSMControllerServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->DSMControllerServerUrl, InSettings->BaseUrl, TEXT("dsmcontroller"));
	InSettings->StatisticServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->StatisticServerUrl, InSettings->BaseUrl, TEXT("social"));
	InSettings->PlatformServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->PlatformServerUrl, InSettings->BaseUrl, TEXT("platform"));
	InSettings->QosManagerServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->QosManagerServerUrl, InSettings->BaseUrl, TEXT("qosm"));
	InSettings->GameTelemetryServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->GameTelemetryServerUrl, InSettings->BaseUrl, TEXT("game-telemetry"));
	InSettings->AchievementServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->AchievementServerUrl, InSettings->BaseUrl, TEXT("achievement"));
	InSettings->MatchmakingServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->MatchmakingServerUrl, InSettings->BaseUrl, TEXT("matchmaking"));
	InSettings->LobbyServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->LobbyServerUrl, InSettings->BaseUrl, TEXT("lobby"));
	InSettings->CloudSaveServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->CloudSaveServerUrl, InSettings->BaseUrl, TEXT("cloudsave"));
	InSettings->SeasonPassServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->SeasonPassServerUrl, InSettings->BaseUrl, TEXT("seasonpass"));
	InSettings->SessionBrowserServerUrl = GetDefaultServerAPIUrl(GetDefault<T>()->SessionBrowserServerUrl, InSettings->BaseUrl, TEXT("sessionbrowser"));
#endif

	return true;
}

void ServerSettings::Reset(ESettingsEnvironment const Environment)
{
	switch (Environment)
	{
	case ESettingsEnvironment::Development:
		SetServerEnvironment<UAccelByteServerSettingsDev>(this);
		break;
	case ESettingsEnvironment::Certification:
		SetServerEnvironment<UAccelByteServerSettingsCert>(this);
		break;
	case ESettingsEnvironment::Production:
		SetServerEnvironment<UAccelByteServerSettingsProd>(this);
		break;
	case ESettingsEnvironment::Default:
	default:
		SetServerEnvironment<UAccelByteServerSettings>(this);
		break;
	}
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

void UAccelByteBlueprintsServerSettings::ResetSettings(ESettingsEnvironment const Environment)
{
	FRegistry::ServerSettings.Reset(Environment);
	FRegistry::ServerCredentials.SetClientCredentials(FRegistry::ServerSettings.ClientId, FRegistry::ServerSettings.ClientSecret);
}
