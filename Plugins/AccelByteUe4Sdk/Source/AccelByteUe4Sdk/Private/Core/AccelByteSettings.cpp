// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteSettings.h"
#include "Core/AccelByteRegistry.h"

using namespace AccelByte;

UAccelByteSettings::UAccelByteSettings()
{
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
