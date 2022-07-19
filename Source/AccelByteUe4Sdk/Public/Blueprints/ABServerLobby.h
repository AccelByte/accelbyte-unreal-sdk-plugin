// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABServerLobby.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;
using namespace AccelByte::GameServerApi;

DECLARE_DYNAMIC_DELEGATE_OneParam(FDDataPartyResponseDelegate, FAccelByteModelsDataPartyResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyDataNotifDelegate, FAccelByteModelsPartyDataNotif, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPayloadModifier, FJsonObjectWrapper, Payload);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDSessionAttributeAllResponseDelegate, FAccelByteModelsGetSessionAttributeAllResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDSessionAttributeResponseDelegate, FAccelByteModelsGetSessionAttributeResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDListBlockerResponseDelegate, FAccelByteModelsListBlockerResponse, Response);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDListBlockedUserResponseDelegate, FAccelByteModelsListBlockedUserResponse, Response);

UCLASS(Blueprintable, BlueprintType)
class UABServerLobby final : public UObject
{
	GENERATED_BODY()
public:
	void SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr);

public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Lobby | Api")
	void GetPartyDataByUserId(FString const& UserId, FDDataPartyResponseDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Lobby | Api")
	void WritePartyStorage(FString const& PartyId, FDPayloadModifier PayloadModifier, int const& RetryAttempt, FDPartyDataNotifDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Lobby | Api")
	void GetPartyStorage(FString const& PartyId, FDPartyDataNotifDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Lobby | Api")
	void GetSessionAttributeAll(FString const& UserId, FDSessionAttributeAllResponseDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Lobby | Api")
	void GetSessionAttribute(FString const& UserId, FString const& Key, FDSessionAttributeResponseDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Lobby | Api")
	void SetSessionAttribute(FString const& UserId, TMap<FString, FString> const& Attributes, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Lobby | Api")
	void SetSessionAttributeString(FString const& UserId, FString const& Key, FString const& Value, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Lobby | Api")
	void GetListOfBlockers(FString const& UserId, FDListBlockerResponseDelegate OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Lobby | Api")
	void GetListOfBlockedUsers(FString const& UserId, FDListBlockedUserResponseDelegate OnSuccess, FDErrorHandler OnError);

private:
	FServerApiClientPtr ApiClientPtr;
};
