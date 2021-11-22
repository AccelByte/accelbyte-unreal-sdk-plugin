#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "AccelByteBPLobby.generated.h"

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
	bool isTempParty{false};
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


UCLASS(Blueprintable, BlueprintType)
class ULobby : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable)
	bool IsConnected() const;

	UFUNCTION(BlueprintCallable)
	void Connect() const;

	UFUNCTION(BlueprintCallable)
	void Disconnect() const;

	UFUNCTION(BlueprintCallable)
	void UnbindEvent() const;

	UFUNCTION(BlueprintCallable)
	void SetOnConnected(FDHandler OnConnected) const;

	UFUNCTION(BlueprintCallable)
	void StartMatchmaking(
		FStartMatchmakingRequest const& Request,
		FDStartMatchmakingResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void CancelMatchmaking(
		FCancelMatchmakingRequest const& Request,
		FDCancelMatchmakingResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void SetReadyConsent(
		FSetReadyConsentRequest const& Request,
		FDSetReadyConsentResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable)
	void SetOnMatchmaking(FDMatchmakingNotif OnNotif);

	UFUNCTION(BlueprintCallable)
	void SetOnSetReadyConsent(FDSetReadyConsentNotif OnNotif);

	UFUNCTION(BlueprintCallable)
	void SetOnRematchmaking(FDRematchmakingNotif OnNotif);
private:
	bool bConnected{false};
	FApiClientPtr ApiClientPtr;
};
