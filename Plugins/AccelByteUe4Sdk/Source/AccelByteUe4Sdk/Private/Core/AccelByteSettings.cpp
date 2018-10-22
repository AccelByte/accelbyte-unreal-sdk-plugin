// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteSettings.h"

using AccelByte::Settings;

namespace AccelByte
{

FString Settings::ClientId;
FString Settings::ClientSecret;
FString Settings::GameId;
FString Settings::PublisherId;
FString Settings::IamServerUrl;
FString Settings::PlatformServerUrl;
FString Settings::LobbyServerUrl;

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

FString UAccelByteBlueprintsSettings::GetGameId()
{
	return Settings::GameId;
}

FString UAccelByteBlueprintsSettings::GetPublisherId()
{
	return Settings::PublisherId;
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

void UAccelByteBlueprintsSettings::SetClientId(const FString& ClientId)
{
	Settings::ClientId = ClientId;
}

void UAccelByteBlueprintsSettings::SetClientSecret(const FString& ClientSecret)
{
	Settings::ClientSecret = ClientSecret;
}

void UAccelByteBlueprintsSettings::SetGameId(const FString& GameId)
{
	Settings::GameId = GameId;
}

void UAccelByteBlueprintsSettings::SetPublisherId(const FString& PublisherId)
{
	Settings::PublisherId = PublisherId;
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
