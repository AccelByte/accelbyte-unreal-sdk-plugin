// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteLobbyModels.h"
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
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void RequestFriend(FString UserId);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void Unfriend(FString UserId);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void ListOutgoingFriends();
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void CancelFriendRequest(FString UserId);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void ListIncomingFriends();
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void AcceptFriend(FString UserId);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void RejectFriend(FString UserId);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void LoadFriendsList();
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void GetFriendshipStatus(FString UserId);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
	static void SendStartMatchmaking(const FString& GameMode);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
	static void SendCancelMatchmaking(const FString& GameMode);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
	static void SendReadyConsentRequest(const FString& MatchId);

    // Party 
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInfoPartyResponse, const FAccelByteModelsInfoPartyResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FCreatePartyResponse, const FAccelByteModelsCreatePartyResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FLeavePartyResponse, const FAccelByteModelsLeavePartyResponse&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FLeavePartyNotice, const FAccelByteModelsLeavePartyNotice&, Result); // This delegate is DEPRECATED. better to use FPartyMemberLeaveNotice
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyResponse, const FAccelByteModelsPartyInviteResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyInvitationNotice, const FAccelByteModelsInvitationNotice&, Result);       
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyGetInvitedNotice, const FAccelByteModelsPartyGetInvitedNotice&, Result);  
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyJoinResponse, const FAccelByteModelsPartyJoinResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyJoinNotice, const FAccelByteModelsPartyJoinNotice&, Result);              
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyKickMemberResponse, const FAccelByteModelsKickPartyMemberResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FInvitePartyKickedNotice, const FAccelByteModelsGotKickedFromPartyNotice&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FPartyMemberConnectNotice, const FAccelByteModelsPartyMemberConnectionNotice&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FPartyMemberDisconnectNotice, const FAccelByteModelsPartyMemberConnectionNotice&, Result);   

    // Chat
    DECLARE_DYNAMIC_DELEGATE_OneParam(FPrivateMessageResponse, const FAccelByteModelsPersonalMessageResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FPrivateMessageNotice, const FAccelByteModelsPersonalMessageNotice&, Result);         
    DECLARE_DYNAMIC_DELEGATE_OneParam(FPartyMessageResponse, const FAccelByteModelsPartyMessageResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FPartyMessageNotice, const FAccelByteModelsPartyMessageNotice&, Result);              

    // Presence
    DECLARE_DYNAMIC_DELEGATE_OneParam(FSetUserPresenceResponse, const FAccelByteModelsSetOnlineUsersResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FUserPresenceNotice, const FAccelByteModelsUsersPresenceNotice&, Result);             
    DECLARE_DYNAMIC_DELEGATE_OneParam(FGetAllFriendsStatusResponse, const FAccelByteModelsGetOnlineUsersResponse&, Result);

	// Notification
	DECLARE_DYNAMIC_DELEGATE_OneParam(FNotificationMessage, const FAccelByteModelsNotificationMessage&, Result);

    // Matchmaking
	DECLARE_DYNAMIC_DELEGATE_OneParam(FMatchmakingResponse, const FAccelByteModelsMatchmakingResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FMatchmakingNotice, const FAccelByteModelsMatchmakingNotice&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FReadyConsentResponse, const FAccelByteModelsReadyConsentRequest&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FReadyConsentNotice, const FAccelByteModelsReadyConsentNotice&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FRematchmakingNotice, const FAccelByteModelsRematchmakingNotice&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FDsNotice, const FAccelByteModelsDsNotice&, Result);

    // Friends
    DECLARE_DYNAMIC_DELEGATE_OneParam(FRequestFriendsResponseDelegate, const FAccelByteModelsRequestFriendsResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FUnfriendResponseDelegate, const FAccelByteModelsUnfriendResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FListOutgoingFriendsResponseDelegate, const FAccelByteModelsListOutgoingFriendsResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FCancelFriendsResponseDelegate, const FAccelByteModelsCancelFriendsResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FListIncomingFriendsResponseDelegate, const FAccelByteModelsListIncomingFriendsResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FAcceptFriendsResponseDelegate, const FAccelByteModelsAcceptFriendsResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FRejectFriendsResponseDelegate, const FAccelByteModelsRejectFriendsResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FLoadFriendListResponseDelegate, const FAccelByteModelsLoadFriendListResponse&, Result);
    DECLARE_DYNAMIC_DELEGATE_OneParam(FGetFriendshipStatusResponseDelegate, const FAccelByteModelsGetFriendshipStatusResponse&, Result);

	// Friends + Notification
	DECLARE_DYNAMIC_DELEGATE_OneParam(FFriendAcceptFriendRequestNotif, const FAccelByteModelsAcceptFriendsNotif&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FIncomingFriendNotif, const FAccelByteModelsRequestFriendsNotif&, Result);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void BindEvent(
        const FConnectSuccess& OnConnectSuccess,
        const FBlueprintErrorHandler& OnConnectError,
        const FConnectionClosed& OnConnectionClosed,
        const FLeavePartyNotice& OnLeavePartyNotice, // This delegate is DEPRECATED. better to use FPartyMemberLeaveNotice
        const FInvitePartyInvitationNotice& OnInvitePartyInvitationNotice,
        const FInvitePartyGetInvitedNotice& OnInvitePartyGetInvitedNotice,
        const FInvitePartyJoinNotice& OnInvitePartyJoinNotice,
        const FInvitePartyKickedNotice& OnInvitePartyKickedNotice,
        const FPartyMemberConnectNotice& OnPartyConnectNotice,
		const FPartyMemberDisconnectNotice& OnPartyDisconnectNotice,
        const FPrivateMessageNotice& OnPrivateMessageNotice,
        const FPartyMessageNotice& OnPartyMessageNotice,
        const FUserPresenceNotice& OnUserPresenceNotice,
		const FNotificationMessage& OnNotificationMessage,
		const FMatchmakingNotice& OnMatchmakingNotice,
		const FReadyConsentNotice& OnReadyConsentNotice,
		const FRematchmakingNotice& OnRematchmakingNotice,
		const FDsNotice& OnDsNotice,
		const FFriendAcceptFriendRequestNotif& OnAcceptFriendsNotifDelegate,
		const FIncomingFriendNotif& OnRequestFriendsNotifDelegate,
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
	static void SetPresenceStatus(EAvailability State, FString Activity);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
    static void SetGetAllUserPresenceResponseDelegate(FGetAllFriendsStatusResponse OnGetAllUserPresenceResponse);

	// Notification
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void GetAllAsyncNotification();

    // Matchmaking
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
	static void SetStartMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingStart);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
	static void SetCancelMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingCancel);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
    static void SetMatchmakingNotifDelegate(FMatchmakingNotice OnMatchmakingNotice);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
	static void SetReadyConsentNotifDelegate(FReadyConsentNotice OnReadyConsentNotice);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
	static void SetReadyConsentResponseDelegate(FReadyConsentResponse OnReadyConsentResponse);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
	static void SetRematchmakingNotifDelegate(FRematchmakingNotice OnRematchmakingNotice);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Matchmaking")
	static void SetDsNotifDelegate(FDsNotice OnDsNotice);

	// Friends
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void SetRequestFriendResponseDelegate(FRequestFriendsResponseDelegate OnRequestFriendsResponseDelegate);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void SetUnfriendResponseDelegate(FUnfriendResponseDelegate OnUnfriendResponse);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void SetListOutgoingFriendsResponseDelegate(FListOutgoingFriendsResponseDelegate OnListOutgoingFriendsResponse);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void SetCancelFriendRequestResponseDelegate(FCancelFriendsResponseDelegate OnCancelFriendsResponse);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void SetListIncomingFriendsResponseDelegate(FListIncomingFriendsResponseDelegate OnListIncomingFriendsResponse);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void SetAcceptFriendResponseDelegate(FAcceptFriendsResponseDelegate OnAcceptFriendsResponse);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void SetRejectFriendResponseDelegate(FRejectFriendsResponseDelegate OnRejectFriendsResponse);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void SetLoadFriendsListResponseDelegate(FLoadFriendListResponseDelegate OnLoadFriendListResponse);
    UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api | Friends")
    static void SetGetFriendshipStatusResponseDelegate(FGetFriendshipStatusResponseDelegate OnGetFriendshipStatusResponse);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Lobby | Api")
	static void UnbindDelegates();
};

