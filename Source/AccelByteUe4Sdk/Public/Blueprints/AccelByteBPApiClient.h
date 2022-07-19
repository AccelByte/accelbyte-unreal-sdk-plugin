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
#include "ABAgreement.h"
#include "ABReward.h"
#include "ABSeasonPass.h"
#include "ABStatistic.h"
#include "ABLeaderboard.h"
#include "ABAchievement.h"
#include "ABSessionBrowser.h"
#include "ABUGC.h"
#include "ABPresence.h"
#include "ABBlock.h"
#include "ABMessage.h"
#include "ABSessionAttribute.h"
#include "ABSignaling.h"
#include "ABGroup.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Core/AccelByteMultiRegistry.h"
#include "AccelByteBPApiClient.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;

UCLASS(Blueprintable, BlueprintType)
class UABCredentials final : public UObject
{
	GENERATED_BODY()

public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	FString GetAccessToken();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	FString GetUserId();

private:
	FApiClientPtr ApiClientPtr;
};


UCLASS(Blueprintable, BlueprintType)
class UABApiClient final : public UObject
{
	GENERATED_BODY()

public:
	UABApiClient();

	void SetApiClient(FApiClientPtr NewApiClientPtr);

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABCredentials* Credentials;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABUser* User;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABLobby* Lobby;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABParty* Party;
	
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABMatchmaking* Matchmaking;
	
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABFriends* Friends;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABCatalog* Catalog;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABOrder* Order;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABEntitlement* Entitlement;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABCurrency* Currency;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABWallet* Wallet;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABFulfillment* Fulfillment;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABCloudSave* CloudSave;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABAgreement* Agreement;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABReward* Reward;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABSeasonPass* SeasonPass;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABStatistic* Statistic;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABLeaderboard* Leaderboard;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABAchievement* Achievement;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABSessionBrowser* SessionBrowser;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABUGC* UGC;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABPresence* Presence;
	
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABBlock* Block;
	
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABMessage* Message;
	
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABSessionAttribute* SessionAttribute;
	
	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABSignaling* Signaling;

	UPROPERTY(BlueprintReadOnly, Category = "AccelByte | ApiClient")
	UABGroup* Group;

private:
	FApiClientPtr ApiClientPtr;
};


UCLASS(Blueprintable, BlueprintType)
class UABMultiRegistry final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "AccelByte | MultiRegistry | Api")
	static UABApiClient* GetApiClient(FString const& Key);
};

