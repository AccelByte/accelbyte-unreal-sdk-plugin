// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteSettings.h"

using AccelByte::Settings;

namespace AccelByte
{

FString Settings::ClientId;
FString Settings::ClientSecret;
FString Settings::Namespace;
FString Settings::PublisherNamespace;
FString Settings::RedirectURI;
FString Settings::BaseUrl;
FString Settings::IamServerUrl;
FString Settings::PlatformServerUrl;
FString Settings::LobbyServerUrl;
FString Settings::CloudStorageServerUrl;
FString Settings::BasicServerUrl;

}

UAccelByteSettings::UAccelByteSettings()
{
}

FString UAccelByteBlueprintsSettings::GetClientId()
{
	return Settings::ClientId;
}

FString UAccelByteBlueprintsSettings::GetClientSecret()
{
	return Settings::ClientSecret;
}

FString UAccelByteBlueprintsSettings::GetNamespace()
{
	return Settings::Namespace;
}

FString UAccelByteBlueprintsSettings::GetPublisherNamespace()
{
	return Settings::PublisherNamespace;
}

FString UAccelByteBlueprintsSettings::GetIamServerUrl()
{
	return Settings::IamServerUrl;
}

FString UAccelByteBlueprintsSettings::GetPlatformServerUrl()
{
	return Settings::PlatformServerUrl;
}

FString UAccelByteBlueprintsSettings::GetLobbyServerUrl()
{
	return Settings::LobbyServerUrl;
}

FString UAccelByteBlueprintsSettings::GetCloudStorageServerUrl()
{
    return Settings::CloudStorageServerUrl;
}

FString UAccelByteBlueprintsSettings::GetBasicServerUrl()
{
	return Settings::BasicServerUrl;
}

void UAccelByteBlueprintsSettings::SetClientId(const FString& ClientId)
{
	Settings::ClientId = ClientId;
}

void UAccelByteBlueprintsSettings::SetClientSecret(const FString& ClientSecret)
{
	Settings::ClientSecret = ClientSecret;
}

void UAccelByteBlueprintsSettings::SetNamespace(const FString& Namespace)
{
	Settings::Namespace = Namespace;
}

void UAccelByteBlueprintsSettings::SetPublisherNamespace(const FString& PublisherNamespace)
{
	Settings::PublisherNamespace = PublisherNamespace;
}

void UAccelByteBlueprintsSettings::SetIamServerUrl(const FString& IamServerUrl)
{
	Settings::IamServerUrl = IamServerUrl;
}

void UAccelByteBlueprintsSettings::SetPlatformServerUrl(const FString& PlatformServerUrl)
{
	Settings::PlatformServerUrl = PlatformServerUrl;
}

void UAccelByteBlueprintsSettings::SetLobbyServerUrl(const FString& LobbyServerUrl)
{
	Settings::LobbyServerUrl = LobbyServerUrl;
}

void UAccelByteBlueprintsSettings::SetCloudStorageServerUrl(const FString& CloudStorageServerUrl)
{
    Settings::CloudStorageServerUrl = CloudStorageServerUrl;
}

void UAccelByteBlueprintsSettings::SetBasicServerUrl(const FString& BasicServerUrl)
{
	Settings::BasicServerUrl = BasicServerUrl;
}