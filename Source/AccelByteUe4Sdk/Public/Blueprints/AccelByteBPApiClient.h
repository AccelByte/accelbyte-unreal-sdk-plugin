#pragma once

#include "CoreMinimal.h"
#include "ABUser.h"
#include "AccelByteBPLobby.h"
#include "ABParty.h"
#include "ABMatchmaking.h"
#include "ABFriends.h"
#include "ABCatalog.h"
#include "ABOrder.h"
#include "ABEntitlement.h"
#include "ABCurrency.h"
#include "ABWallet.h"
#include "ABFulfillment.h"
#include "ABCloudSave.h"
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
	UABUser* User;

	UPROPERTY(BlueprintReadOnly)
	ULobby* Lobby;

	UPROPERTY(BlueprintReadOnly)
	UABParty* Party;
	
	UPROPERTY(BlueprintReadOnly)
	UABMatchmaking* Matchmaking;
	
	UPROPERTY(BlueprintReadOnly)
	UABFriends* Friends;
	
	UPROPERTY(BlueprintReadOnly)
	UABCatalog* Catalog;
	
	UPROPERTY(BlueprintReadOnly)
	UABOrder* Order;
	
	UPROPERTY(BlueprintReadOnly)
	UABEntitlement* Entitlement;
	
	UPROPERTY(BlueprintReadOnly)
	UABCurrency* Currency;
	
	UPROPERTY(BlueprintReadOnly)
	UABWallet* Wallet;
	
	UPROPERTY(BlueprintReadOnly)
	UABFulfillment* Fulfillment;

	UPROPERTY(BlueprintReadOnly)
	UABCloudSave* CloudSave;

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

