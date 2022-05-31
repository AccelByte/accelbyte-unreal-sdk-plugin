#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/IAccelByteTokenGenerator.h"
#include "AccelByteBPLobby.generated.h"

using namespace AccelByte;

USTRUCT(BlueprintType)
struct FRefreshTokenRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString AccessToken;
};

USTRUCT(BlueprintType)
struct FSetRetryParametersRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 NewTotalTimeout = 60000;

	UPROPERTY(BlueprintReadWrite)
	int32 NewBackoffDelay = 1000;

	UPROPERTY(BlueprintReadWrite)
	int32 NewMaxDelay = 30000;
};

USTRUCT(BlueprintType)
struct FRequestDSModel
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString SessionID;

	UPROPERTY(BlueprintReadWrite)
	FString GameMode;

	UPROPERTY(BlueprintReadWrite)
	FString ClientVersion;

	UPROPERTY(BlueprintReadWrite)
	FString Region;

	UPROPERTY(BlueprintReadWrite)
	FString Deployment;

	UPROPERTY(BlueprintReadWrite)
	FString ServerName = TEXT("");
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDRefreshTokenResponseDelegate, FAccelByteModelsRefreshTokenResponse, Response);

UCLASS(Blueprintable, BlueprintType)
class UABLobby : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable)
	bool IsConnected();

	UFUNCTION(BlueprintCallable)
	void Connect();

	UFUNCTION(BlueprintCallable)
	void Disconnect();

	UFUNCTION(BlueprintCallable)
	void UnbindEvent();

	UFUNCTION(BlueprintCallable)
	void SetOnConnected(FDHandler OnConnected);

	UFUNCTION(BlueprintCallable)
	void SetOnErrorNotification(FDErrorHandler OnErrorNotification);

	UFUNCTION(BlueprintCallable)
	FString RefreshToken(FRefreshTokenRequest const& Request, FDRefreshTokenResponseDelegate OnResponse, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable)
	void SetRetryParameters(FSetRetryParametersRequest const& Request);

	//Can not test this through lua yet, removing the UFUNCTION for now
	void SetTokenGenerator(TSharedPtr<IAccelByteTokenGenerator> TokenGenerator);

	UFUNCTION(BlueprintCallable)
	FString RequestDS(FRequestDSModel const& Request);

private:
	bool bConnected{false};
	FApiClientPtr ApiClientPtr;
};
