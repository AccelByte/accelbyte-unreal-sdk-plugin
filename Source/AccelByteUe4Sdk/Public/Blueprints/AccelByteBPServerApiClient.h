// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABServerOauth2.h"
#include "ABServerDSM.h"
#include "ABServerLobby.h"
#include "ABServerMatchmaking.h"
#include "ABServerSessionBrowser.h"
#include "AccelByteBPServerApiClient.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;

UCLASS(Blueprintable, BlueprintType)
class UABServerCredentials final : public UObject
{
	GENERATED_BODY()

public:
	void SetServerApiClient(FServerApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable)
	FString GetAccessToken() const;

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

	UPROPERTY(BlueprintReadOnly)
	UABServerCredentials* Credentials;

	UPROPERTY(BlueprintReadOnly)
	UABServerOauth2* ServerOauth2;

	UPROPERTY(BlueprintReadOnly)
	UABServerDSM* ServerDSM;

	UPROPERTY(BlueprintReadOnly)
	UABServerLobby* ServerLobby;

	UPROPERTY(BlueprintReadOnly)
	UABServerMatchmaking* ServerMatchmaking;

	UPROPERTY(BlueprintReadOnly)
	UABServerSessionBrowser* ServerSessionBrowser;

private:
	FServerApiClientPtr ApiClientPtr;
};

UCLASS(Blueprintable, BlueprintType)
class UServerMultiRegistry final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	UServerApiClient* GetServerApiClient(FString const& Key);
};