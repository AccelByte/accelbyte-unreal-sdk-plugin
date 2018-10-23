// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteLobbyModels.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteLobbyBlueprints.generated.h"


// Testing
UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsLobby : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_ThreeParams(FConnectionClosed, int32, StatusCode, const FString&, Reason, bool, WasClean);
	DECLARE_DYNAMIC_DELEGATE(FConnectSuccess);
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void Connect(const FConnectSuccess& OnSuccess, const FBlueprintErrorHandler& OnError, const FConnectionClosed& OnConnectionClosed);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void Disconnect();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static bool IsConnected();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendPrivateMessage(const FString& UserId, const FString& Message);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendPartyMessage(const FString& Message);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendInfoPartyRequest();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendCreatePartyRequest();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendLeavePartyRequest();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendInviteToPartyRequest(const FString& UserId);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendAcceptInvitationRequest(const FString& PartyId, FString InvitationToken);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendGetOnlineUsersRequest();

	DECLARE_DYNAMIC_DELEGATE_OneParam(FPrivateMessageNotice, const FAccelByteModelsPrivateMessageNotice&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FPartyMessageNotice, const FAccelByteModelsPartyMessageNotice&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FInfoPartyResponse, const FAccelByteModelsInfoPartyResponse&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreatePartyResponse, const FAccelByteModelsCreatePartyResponse&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FLeavePartyResponse, const FAccelByteModelsLeavePartyResponse&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FInviteToPartyResponse, const FAccelByteModelsInviteToPartyResponse&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FPartyInvitationNotice, const FAccelByteModelsPartyInvitationNotice&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FAcceptInvitationResponse, const FAccelByteModelsAcceptInvitationReponse&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FPartyInvitationAcceptanceNotice, const FAccelByteModelsPartyInvitationAcceptanceNotice&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetOnlineUsersResponse, const FAccelByteModelsGetOnlineUsersResponse&, Result);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void BindDelegates(
		const FPrivateMessageNotice& PrivateMessageNotice,
		const FPartyMessageNotice& PartyMessageNotice,
		const FInfoPartyResponse& InfoPartyResponse,
		const FCreatePartyResponse& CreatePartyResponse,
		const FLeavePartyResponse& LeavePartyResponse,
		const FInviteToPartyResponse& InviteToPartyResponse,
		const FPartyInvitationNotice& PartyInvitationNotice,
		const FAcceptInvitationResponse& AcceptInvitationResponse,
		const FPartyInvitationAcceptanceNotice& PartyInvitationAcceptanceNotice,
		const FGetOnlineUsersResponse& GetOnlineUsersResponse
	);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void UnbindDelegates();

};

