// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABBlock.generated.h"

using namespace AccelByte;

USTRUCT(BlueprintType)
struct FBlockPlayerRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString UserID;
};

USTRUCT(BlueprintType)
struct FUnblockPlayerRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString UserID;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDBlockPlayerResponse, FAccelByteModelsBlockPlayerResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDUnblockPlayerResponse, FAccelByteModelsUnblockPlayerResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDListOfBlockedUserResponse, FAccelByteModelsListBlockedUserResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDListOfBlockersResponse, FAccelByteModelsListBlockerResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDBlockPlayerNotif, FAccelByteModelsBlockPlayerNotif, Notif);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDUnblockPlayerNotif, FAccelByteModelsUnblockPlayerNotif, Notif);

//Sub-API from AccelByte Lobby
UCLASS(Blueprintable, BlueprintType)
class UABBlock : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

//Request-Response
public: 

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Block")
	void BlockPlayer(
		FBlockPlayerRequest const& Request,
		FDBlockPlayerResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Block")
	void UnblockPlayer(
		FUnblockPlayerRequest const& Request,
		FDUnblockPlayerResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Block")
	void GetListOfBlockedUsers(
		FDListOfBlockedUserResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Block")
	void GetListOfBlockers(
		FDListOfBlockersResponse OnResponse,
		FDErrorHandler OnError) const;

//Notification
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Block")
	void SetBlockPlayerNotifDelegate(FDBlockPlayerNotif OnNotif);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Block")
	void SetUnblockPlayerNotifDelegate(FDUnblockPlayerNotif OnNotif);

private:
	FApiClientPtr ApiClientPtr;
};

