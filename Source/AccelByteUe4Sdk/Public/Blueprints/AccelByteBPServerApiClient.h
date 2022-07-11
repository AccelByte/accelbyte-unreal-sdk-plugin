// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABServerOauth2.h"
#include "ABServerDSM.h"
#include "ABServerLobby.h"
#include "ABServerMatchmaking.h"
#include "ABServerSessionBrowser.h"
#include "ABServerAchievement.h"
#include "ABServerStatistic.h"
#include "AccelByteBPServerApiClient.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;

UCLASS(Blueprintable, BlueprintType)
class UABServerCredentials final : public UObject
{
	GENERATED_BODY()

public:
	void SetServerApiClient(FServerApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Credentials")
	FString GetAccessToken();

private:
	FServerApiClientPtr ApiClientPtr;
};


UCLASS(Blueprintable, BlueprintType)
class UServerApiClient final : public UObject
{
	GENERATED_BODY()
public:
	UServerApiClient();

	void SetServerApiClient(FServerApiClientPtr const& NewApiClientPtr);

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Server | ApiClient")
	UABServerCredentials* Credentials;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Server | ApiClient")
	UABServerOauth2* ServerOauth2;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Server | ApiClient")
	UABServerDSM* ServerDSM;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Server | ApiClient")
	UABServerLobby* ServerLobby;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Server | ApiClient")
	UABServerMatchmaking* ServerMatchmaking;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Server | ApiClient")
	UABServerSessionBrowser* ServerSessionBrowser;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Server | ApiClient")
	UABServerAchievement* ServerAchievement;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | Server | ApiClient")
	UABServerStatistic* ServerStatistic;
private:
	FServerApiClientPtr ApiClientPtr;
};

UCLASS(Blueprintable, BlueprintType)
class UServerMultiRegistry final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | MultiRegistry | Api")
	static UServerApiClient* GetServerApiClient(FString const& Key);
};