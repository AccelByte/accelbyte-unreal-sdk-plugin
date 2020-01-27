// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
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

FString UAccelByteBlueprintsServerSettings::GetDSMServerUrl()
{
	return FRegistry::ServerSettings.DSMServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetStatisticServerUrl()
{
	return FRegistry::ServerSettings.StatisticServerUrl;
}

FString UAccelByteBlueprintsServerSettings::GetPlatformServerUrl()
{
	return FRegistry::ServerSettings.PlatformServerUrl;
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

void UAccelByteBlueprintsServerSettings::SetDSMServerUrl(const FString& DSMServerUrl)
{
	FRegistry::ServerSettings.DSMServerUrl = DSMServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetStatisticServerUrl(const FString& StatisticServerUrl)
{
	FRegistry::ServerSettings.StatisticServerUrl = StatisticServerUrl;
}

void UAccelByteBlueprintsServerSettings::SetPlatformServerUrl(const FString& PlatformServerUrl)
{
	FRegistry::ServerSettings.PlatformServerUrl = PlatformServerUrl;
}
