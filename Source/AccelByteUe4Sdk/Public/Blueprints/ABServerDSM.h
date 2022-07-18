// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Models/AccelByteDSMModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABServerDSM.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;
using namespace AccelByte::GameServerApi;

UENUM(BlueprintType)
enum class UEnumServerType : uint8
{
	NONE = 0,
	CLOUDSERVER = (uint8)EServerType::CLOUDSERVER,
	LOCALSERVER = (uint8)EServerType::LOCALSERVER
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDServerSessionResponseDelegate, FAccelByteModelsServerSessionResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDServerInfoDelegate, FAccelByteModelsServerInfo, Response);

UCLASS(Blueprintable, BlueprintType)
class UABServerDSM final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void RegisterServerToDSM(int32 const& Port, FDHandler OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void RegisterLocalServerToDSM(FString const& IPAddress, int32 const& Port, FString const& ServerName, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void SendShutdownToDSM(bool const& KillMe, FString const& MatchId, FDHandler OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void DeregisterLocalServerFromDSM(FString const& ServerName, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void GetSessionId(FDServerSessionResponseDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void GetServerInfo(FDServerInfoDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void ConfigureAutoShutdown(int32 TickSeconds = 5, int CountdownStart = 30);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void SetOnAutoShutdownResponse(FDHandler OnAutoShutdown);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void SetOnAutoShutdownErrorDelegate(FDErrorHandler OnShutdownError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void SetServerName(const FString Name);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	void SetServerType(UEnumServerType Type);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | DSMC | Api")
	int32 GetPlayerNum();

private:
	FServerApiClientPtr ApiClientPtr;
};
