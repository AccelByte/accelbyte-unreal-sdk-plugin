// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteRegistry.h"

using namespace AccelByte;

UAccelByteServerSettings::UAccelByteServerSettings()
{
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
