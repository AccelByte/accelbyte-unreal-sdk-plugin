// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteEnvironment.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEnvironmentChangedDelegate, const ESettingsEnvironment);

namespace AccelByte
{
	class Settings;
	class ServerSettings;
}

class IAccelByteUe4SdkModuleInterface : public IModuleInterface
{
public:

	static IAccelByteUe4SdkModuleInterface& Get()
	{
		return FModuleManager::LoadModuleChecked<IAccelByteUe4SdkModuleInterface>("AccelByteUe4Sdk");
	}

	static bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("AccelByteUe4Sdk");
	}

	virtual void SetEnvironment(ESettingsEnvironment const Environment) = 0;
	virtual AccelByte::Settings const& GetClientSettings() const = 0;
	virtual AccelByte::ServerSettings const& GetServerSettings() const = 0;
	virtual ESettingsEnvironment const& GetSettingsEnvironment() const = 0;
	virtual FEnvironmentChangedDelegate& OnEnvironmentChanged() = 0;
};
