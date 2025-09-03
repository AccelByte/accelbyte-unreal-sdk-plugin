#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/IAccelByteTokenGenerator.h"
#include "AccelByteBPLobby.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

USTRUCT(BlueprintType)
struct FRefreshTokenRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Token")
	FString AccessToken{};
};

USTRUCT(BlueprintType)
struct FSetRetryParametersRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Retry Parameters")
	int32 NewTotalTimeout = 60000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Retry Parameters")
	int32 NewBackoffDelay = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Retry Parameters")
	int32 NewMaxDelay = 30000;
};

USTRUCT(BlueprintType)
struct FRequestDSModel
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Request DS")
	FString SessionID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Request DS")
	FString GameMode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Request DS")
	FString ClientVersion{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Request DS")
	FString Region{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Request DS")
	FString Deployment{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Lobby | Models | Request DS")
	FString ServerName = TEXT("");
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDRefreshTokenResponseDelegate, FAccelByteModelsRefreshTokenResponse, Response);

#pragma endregion

#pragma region MODEL_AND_DELEGATE_FOR_NOTIFICATION

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDConnectionClosedDelegate, int32, StatusCode, FString, Reason, bool, bWasClean);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDDisconnectNotifDelegate, FAccelByteModelsDisconnectNotif, Notif);

#pragma endregion

UCLASS(Blueprintable, BlueprintType)
class UABLobby : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	bool IsConnected();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	void Connect();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	void Disconnect();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	FString RefreshToken(FRefreshTokenRequest const& Request, FDRefreshTokenResponseDelegate OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	void SetRetryParameters(FSetRetryParametersRequest const& Request);

	// Can not test this through lua yet, removing the UFUNCTION for now
	void SetTokenGenerator(TSharedPtr<AccelByte::IAccelByteTokenGenerator> TokenGenerator);

#if 1 // MMv1 Deprecation
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	FString RequestDS(FRequestDSModel const& Request);
#endif
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	void UnbindEvent();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Delegate")
	void SetOnConnected(FDHandler OnConnected);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Delegate")
	void SetOnConnectFailed(const FDErrorHandler& OnConnectError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Delegate")
	void SetOnDisconnect(const FDDisconnectNotifDelegate& OnDisconnected);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Delegate")
	void SetOnConnectionClosed(FDConnectionClosedDelegate OnConnectionClosed);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Delegate")
	void SetOnErrorNotification(FDErrorHandler OnErrorNotification);

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};
