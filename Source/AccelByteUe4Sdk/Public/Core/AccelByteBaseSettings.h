// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteEnvironment.h"

UENUM(BlueprintType)
enum class EHttpCacheType : uint8
{
	UNKNOWN = 0,
	STORAGE = 1,
	MEMORY = 2
};

namespace AccelByte
{
class ACCELBYTEUE4SDK_API BaseSettings
{
public:
	virtual ~BaseSettings() = default;

	FString ClientId{};
	FString ClientSecret{};
	FString Namespace{};
	FString PublisherNamespace{};
	FString RedirectURI{};
	FString BaseUrl{};
	FString IamServerUrl{};
	FString PlatformServerUrl{};
	FString LobbyServerUrl{};
	FString StatisticServerUrl{};
	FString QosManagerServerUrl{};
	FString CloudSaveServerUrl{};
	FString GameTelemetryServerUrl{};
	FString AchievementServerUrl{};
	FString SessionBrowserServerUrl{};
	FString SeasonPassServerUrl{};
	FString MatchmakingV2ServerUrl{};
	FString SessionServerUrl{};
	
	float QosPingTimeout{.6f};
	EHttpCacheType HttpCacheType = EHttpCacheType::STORAGE;

	virtual void Reset(ESettingsEnvironment const Environment) = 0;

	BaseSettings& operator=(BaseSettings const& Other) = default;

protected:
	virtual void LoadSettings(const FString& SectionPath) = 0;
	virtual void LoadFallback(const FString& SectionPath, const FString& Key, FString& Value) = 0;
};

typedef TSharedRef<BaseSettings, ESPMode::ThreadSafe> BaseSettingsRef;
typedef TSharedPtr<BaseSettings, ESPMode::ThreadSafe> BaseSettingsPtr;

} // Namespace AccelByte


