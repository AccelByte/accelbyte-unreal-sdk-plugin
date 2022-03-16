// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ABMessage.generated.h"

using namespace AccelByte;

USTRUCT(BlueprintType)
struct FSendPrivateMessageRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString UserID;

	UPROPERTY(BlueprintReadWrite)
	FString Message;
};

USTRUCT(BlueprintType)
struct FSendPartyMessageRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Message;
};

USTRUCT(BlueprintType)
struct FSendChannelMessageRequest
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Message;
};

//Response Delegate
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPersonalChatResponse, FAccelByteModelsPersonalMessageResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPartyChatResponse, FAccelByteModelsPartyMessageResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDJoinDefaultChannelChatResponse, FAccelByteModelsJoinDefaultChannelResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDChannelChatResponse, FAccelByteModelsChannelMessageResponse, Response);

//Notif Delegate
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPersonalChatNotif, FAccelByteModelsPersonalMessageNotice, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDMessageNotif, FAccelByteModelsNotificationMessage, Notif);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDChannelChatNotif, FAccelByteModelsChannelMessageNotice, Notif);


//Sub-API from AccelByte Lobby
UCLASS(Blueprintable, BlueprintType)
class UABMessage : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

//Request-Response
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Message")
	void SendPing() const;
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Message")
	FString SendPrivateMessage(
		FSendPrivateMessageRequest const& Request,
		FDPersonalChatResponse OnResponse,
		FDErrorHandler OnError) const;
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Message")
	FString SendPartyMessage(
		FSendPartyMessageRequest const& Request,
		FDPartyChatResponse OnResponse,
		FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Message")
	FString SendJoinDefaultChannelChatRequest(
		FDJoinDefaultChannelChatResponse OnResponse,
		FDErrorHandler OnError) const;
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Message")
	FString SendChannelMessage(
		FSendChannelMessageRequest const& Request,
		FDChannelChatResponse OnResponse,
		FDErrorHandler OnError) const;

//Notification
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Message")
	void SetPrivateMessageNotifDelegate(FDPersonalChatNotif OnNotif) const;
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Message")
	void SetMessageNotifDelegate(FDMessageNotif OnNotif) const;
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Message")
	void SetChannelMessageNotifDelegate(FDChannelChatNotif OnNotif) const;

private:
	FApiClientPtr ApiClientPtr;
};
