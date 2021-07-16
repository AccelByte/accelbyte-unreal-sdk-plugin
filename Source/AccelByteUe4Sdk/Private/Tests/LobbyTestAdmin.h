// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteError.h"

#include "LobbyTestAdmin.generated.h"

using AccelByte::FErrorHandler;
using AccelByte::THandler;
using AccelByte::FVoidHandler;

USTRUCT(BlueprintType)
struct FLobbyModelConfig
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	bool AutoKickOnDisconnect{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	int32 ChatRateLimitBurst;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	int32 ChatRateLimitDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	bool EnableChat{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	bool EntitlementCheck{ false };
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	FString EntitlementItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	int32 GeneralRateLimitBurst;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	int32 GeneralRateLimitDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	int32 MaxPartyMember;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Lobby | Config")
	bool ProfanityFilter{ false };
};

USTRUCT(BlueprintType)
struct FPodConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Cpu_limit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Mem_limit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Params;
};

USTRUCT(BlueprintType)
struct FDeploymentConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Buffer_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Configuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Game_version;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Max_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Min_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TArray<FString> Regions;
};

USTRUCT(BlueprintType)
struct FDeploymentWithOverride
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	bool Allow_version_override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Buffer_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Configuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Game_version;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Max_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Min_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FDeploymentConfig> Overrides;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TArray<FString> Regions;
};

USTRUCT(BlueprintType)
struct FDsmConfig
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	bool Allow_version_override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Buffer_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Claim_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FPodConfig> Configurations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Cpu_limit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Mem_limit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Params;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Creation_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Default_version;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FDeploymentWithOverride> Deployments;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Heartbeat_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FString> Image_version_mapping;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Max_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Min_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, FDeploymentConfig> Overrides;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Port;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, int32> Ports;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	FString Protocol;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TArray<FString> Providers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Session_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	int32 Unreachable_timeout;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | DSM | DSMConfig")
	TMap<FString, int32> Version_image_size_mapping;
};

void LobbyGetConfig(const THandler<FLobbyModelConfig>& OnSuccess, const FErrorHandler& OnError);
void LobbySetConfig(const FLobbyModelConfig& Config, const THandler<FLobbyModelConfig>& OnSuccess, const FErrorHandler& OnError);
void DSMDeleteServer(const FString& podName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
void DSMGetConfig(const THandler<FDsmConfig> & OnSuccess, const FErrorHandler & OnError);
void DSMSetConfig(const FDsmConfig& Body, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);