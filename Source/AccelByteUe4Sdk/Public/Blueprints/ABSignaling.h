// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABSignaling.generated.h"

using namespace AccelByte;

USTRUCT(BlueprintType)
struct FSendSignalingMessageRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Signaling | Models | Message")
	FString UserID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Signaling | Models | Message")
	FString Message;
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FDSignalingP2PNotif, FString, UserID, FString, Message);

//Sub-API from AccelByte Lobby
UCLASS(Blueprintable, BlueprintType)
class UABSignaling : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

//Request-Response
public: 
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Signaling | Api")
	FString SendSignalingMessage(FSendSignalingMessageRequest const& Request);

//Notification
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Signaling | Api")
	void SetSignalingP2PNotif(FDSignalingP2PNotif OnNotif);

private:
	FApiClientPtr ApiClientPtr;
};
