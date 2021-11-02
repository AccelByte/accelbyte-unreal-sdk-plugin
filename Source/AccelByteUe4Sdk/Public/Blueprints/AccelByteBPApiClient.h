#pragma once

#include "CoreMinimal.h"
#include "AccelByteBPUser.h"
#include "AccelByteBPLobby.h"
#include "ABParty.h"
#include "ABMatchmaking.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Core/AccelByteMultiRegistry.h"
#include "AccelByteBPApiClient.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;

UCLASS(Blueprintable, BlueprintType)
class UCredentials final : public UObject
{
	GENERATED_BODY()

public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable)
	FString GetAccessToken() const;

	UFUNCTION(BlueprintCallable)
	FString GetUserId() const;

private:
	FApiClientPtr ApiClientPtr;
};


UCLASS(Blueprintable, BlueprintType)
class UApiClient final : public UObject
{
	GENERATED_BODY()
public:
	UApiClient();

	void SetApiClient(FApiClientPtr NewApiClientPtr);

	UPROPERTY(BlueprintReadOnly)
	UCredentials* Credentials;

	UPROPERTY(BlueprintReadOnly)
	UUser* User;

	UPROPERTY(BlueprintReadOnly)
	ULobby* Lobby;

	UPROPERTY(BlueprintReadOnly)
	UABParty* Party;
	
	UPROPERTY(BlueprintReadOnly)
	UABMatchmaking* Matchmaking;

private:
	FApiClientPtr ApiClientPtr;
};


UCLASS(Blueprintable, BlueprintType)
class UMultiRegistry final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	UApiClient* GetApiClient(FString const& Key);
};

