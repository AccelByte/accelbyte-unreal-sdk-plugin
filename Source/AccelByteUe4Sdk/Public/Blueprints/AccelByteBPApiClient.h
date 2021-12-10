#pragma once

#include "CoreMinimal.h"
#include "ABUser.h"
#include "AccelByteBPLobby.h"
#include "ABParty.h"
#include "ABMatchmaking.h"
#include "ABFriends.h"
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

	UFUNCTION(BlueprintCallable)
	FString GetAccessToken() const;

	UFUNCTION(BlueprintCallable)
	FString GetUserId() const;

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

	UPROPERTY(BlueprintReadOnly)
	UABCredentials* Credentials;

	UPROPERTY(BlueprintReadOnly)
	UABUser* User;

	UPROPERTY(BlueprintReadOnly)
	UABLobby* Lobby;

	UPROPERTY(BlueprintReadOnly)
	UABParty* Party;
	
	UPROPERTY(BlueprintReadOnly)
	UABMatchmaking* Matchmaking;
	
	UPROPERTY(BlueprintReadOnly)
	UABFriends* Friends;

private:
	FApiClientPtr ApiClientPtr;
};


UCLASS(Blueprintable, BlueprintType)
class UABMultiRegistry final : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	UABApiClient* GetApiClient(FString const& Key);
};

