// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABMatchmaking.generated.h"

using namespace AccelByte;

USTRUCT(BlueprintType)
struct FStartMatchmakingRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString gameMode;
};

USTRUCT(BlueprintType)
struct FCancelMatchmakingRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString gameMode;

	UPROPERTY(BlueprintReadWrite)
	bool isTempParty{ false };
};

USTRUCT(BlueprintType)
struct FSetReadyConsentRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString matchId;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDStartMatchmakingResponse, FAccelByteModelsMatchmakingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDCancelMatchmakingResponse, FAccelByteModelsMatchmakingResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDSetReadyConsentResponse, FAccelByteModelsReadyConsentRequest, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDMatchmakingNotif, FAccelByteModelsMatchmakingNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDSetReadyConsentNotif, FAccelByteModelsReadyConsentNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDRematchmakingNotif, FAccelByteModelsRematchmakingNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDDsNotif, FAccelByteModelsDsNotice, Notif);


//Sub-API from AccelByte Lobby
UCLASS(Blueprintable, BlueprintType)
class UABMatchmaking : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	//Request-Response
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Matchmaking")
	void StartMatchmaking(
		FStartMatchmakingRequest const& Request,
		FDStartMatchmakingResponse OnResponse,
		FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Matchmaking")
	void CancelMatchmaking(
		FCancelMatchmakingRequest const& Request,
		FDCancelMatchmakingResponse OnResponse,
		FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Matchmaking")
	void SetReadyConsent(
		FSetReadyConsentRequest const& Request,
		FDSetReadyConsentResponse OnResponse,
		FDErrorHandler OnError);

	//Notification
public:
	UFUNCTION(BlueprintCallable)
	void SetOnMatchmaking(FDMatchmakingNotif OnNotif);

	UFUNCTION(BlueprintCallable)
	void SetOnSetReadyConsent(FDSetReadyConsentNotif OnNotif);

	UFUNCTION(BlueprintCallable)
	void SetOnRematchmaking(FDRematchmakingNotif OnNotif);

	UFUNCTION(BlueprintCallable)
	void SetOnDs(FDDsNotif OnNotif);

	
private:
	FApiClientPtr ApiClientPtr;
};
