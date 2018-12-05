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
	static void Connect();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void Disconnect();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static bool IsConnected();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendPing();

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
	static void SendAcceptInvitationRequest(const FString& PartyId, const FString& InvitationToken);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendKickPartyMemberRequest(const FString& UserId);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void SendGetOnlineUsersRequest();

    // Party 
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInfoPartyResponse, const FAccelByteModelsInfoPartyResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FCreatePartyResponse, const FAccelByteModelsCreatePartyResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FLeavePartyResponse, const FAccelByteModelsLeavePartyResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FLeavePartyNotice, const FAccelByteModelsLeavePartyNotice&, Result);                  
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyResponse, const FAccelByteModelsPartyInviteResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyInvitationNotice, const FAccelByteModelsInvitationNotice&, Result);       
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyGetInvitedNotice, const FAccelByteModelsPartyGetInvitedNotice&, Result);  
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyJoinResponse, const FAccelByteModelsPartyJoinReponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyJoinNotice, const FAccelByteModelsPartyJoinNotice&, Result);              
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyKickMemberResponse, const FAccelByteModelsKickPartyMemberResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyKickedNotice, const FAccelByteModelsGotKickedFromPartyNotice&, Result);   

    // Chat
    DECLARE_DYNAMIC_DELEGATE_OneParam(FPrivateMessageResponse, const FAccelByteModelsPersonalMessageResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FPrivateMessageNotice, const FAccelByteModelsPersonalMessageNotice&, Result);         
    DECLARE_DYNAMIC_DELEGATE_OneParam(FPartyMessageResponse, const FAccelByteModelsPartyMessageResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FPartyMessageNotice, const FAccelByteModelsPartyMessageNotice&, Result);              

    // Presence
    DECLARE_DYNAMIC_DELEGATE_OneParam(FSetUserPresenceResponse, const FAccelByteModelsSetOnlineUsersResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FUserPresenceNotice, const FAccelByteModelsUsersPresenceNotice&, Result);             
    DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAllUserPresenceResponse, const FAccelByteModelsGetOnlineUsersResponse&, Result);

	// Notification
	DECLARE_DYNAMIC_DELEGATE_OneParam(FNotificationMessage, const FAccelByteModelsNotificationMessage&, Result);

    // Matchmaking
    DECLARE_DYNAMIC_DELEGATE_OneParam(FMatchmakingResponse, const FAccelByteModelsMatchmakingResponse&, Result);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void BindEvent(
        const FConnectSuccess& OnConnectSuccess,
        const FBlueprintErrorHandler& OnConnectError,
        const FConnectionClosed& OnConnectionClosed,
        const FLeavePartyNotice& OnLeavePartyNotice,
        const FInvitePartyInvitationNotice& OnInvitePartyInvitationNotice,
        const FInvitePartyGetInvitedNotice& OnInvitePartyGetInvitedNotice,
        const FInvitePartyJoinNotice& OnInvitePartyJoinNotice,
        const FInvitePartyKickedNotice& OnInvitePartyKickedNotice,
        const FPrivateMessageNotice& OnPrivateMessageNotice,
        const FPartyMessageNotice& OnPartyMessageNotice,
        const FUserPresenceNotice& OnUserPresenceNotice,
		const FNotificationMessage& OnNotificationMessage,
        const FBlueprintErrorHandler& OnParsingError
	);


    // Party
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetInfoPartyResponseDelegate(FInfoPartyResponse OnInfoPartyResponse);

    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetCreatePartyResponseDelegate(FCreatePartyResponse OnCreatePartyResponse);

    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetLeavePartyResponseDelegate(FLeavePartyResponse OnLeavePartyResponse);

    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetInvitePartyResponseDelegate(FInvitePartyResponse OnInvitePartyResponse);

    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetInvitePartyJoinResponseDelegate(FInvitePartyJoinResponse OnInvitePartyJoinResponse);

    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetInvitePartyKickMemberResponseDelegate(FInvitePartyKickMemberResponse OnInvitePartyKickMemberResponse);



    // Chat
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetPrivateMessageResponseDelegate(FPrivateMessageResponse OnPrivateMessageResponse);

    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetPartyMessageResponseDelegate(FPartyMessageResponse OnPartyMessageResponse);


    // Presence
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetUserPresenceResponseDelegate(FSetUserPresenceResponse OnUserPresenceResponse);


    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetGetAllUserPresenceResponseDelegate(FGetAllUserPresenceResponse OnGetAllUserPresenceResponse);

	// Notification
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void GetAllAsyncNotification();

    // Matchmaking
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingResponse);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void UnbindDelegates();
};

