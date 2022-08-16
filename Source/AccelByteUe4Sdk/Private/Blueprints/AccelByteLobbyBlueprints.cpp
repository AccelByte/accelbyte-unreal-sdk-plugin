// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteLobbyBlueprints.h"
#include "Api/AccelByteLobbyApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::FErrorHandler;

void UAccelByteBlueprintsLobby::Connect()
{
	FRegistry::Lobby.Connect();
}

void UAccelByteBlueprintsLobby::Disconnect()
{
	FRegistry::Lobby.Disconnect();
}

bool UAccelByteBlueprintsLobby::IsConnected()
{
	return FRegistry::Lobby.IsConnected();
}

void UAccelByteBlueprintsLobby::SendPing()
{
	FRegistry::Lobby.SendPing();
}

void UAccelByteBlueprintsLobby::SendPrivateMessage(const FString& UserId, const FString& Message)
{
	FRegistry::Lobby.SendPrivateMessage(UserId, Message);
}

void UAccelByteBlueprintsLobby::SendPartyMessage(const FString& Message)
{
	FRegistry::Lobby.SendPartyMessage(Message);
}

void UAccelByteBlueprintsLobby::SendInfoPartyRequest()
{
	FRegistry::Lobby.SendInfoPartyRequest();
}

void UAccelByteBlueprintsLobby::SendCreatePartyRequest()
{
	FRegistry::Lobby.SendCreatePartyRequest();
}

void UAccelByteBlueprintsLobby::SendLeavePartyRequest()
{
	FRegistry::Lobby.SendLeavePartyRequest();
}

void UAccelByteBlueprintsLobby::SendInviteToPartyRequest(const FString& UserId)
{
	FRegistry::Lobby.SendInviteToPartyRequest(UserId);
}

void UAccelByteBlueprintsLobby::SendAcceptInvitationRequest(const FString& PartyId, const FString& InvitationToken)
{
	FRegistry::Lobby.SendAcceptInvitationRequest(PartyId, InvitationToken);
}

void UAccelByteBlueprintsLobby::SendKickPartyMemberRequest(const FString& UserId)
{
	FRegistry::Lobby.SendKickPartyMemberRequest(UserId);
}

void UAccelByteBlueprintsLobby::SendGetOnlineUsersRequest()
{
	FRegistry::Lobby.SendGetOnlineUsersRequest();
}

// Matchmaking
void UAccelByteBlueprintsLobby::SendStartMatchmaking(const FString& GameMode)
{
	FRegistry::Lobby.SendStartMatchmaking(GameMode);
}

void UAccelByteBlueprintsLobby::SendCancelMatchmaking(const FString& GameMode)
{
	FRegistry::Lobby.SendCancelMatchmaking(GameMode);
}

void UAccelByteBlueprintsLobby::SendReadyConsentRequest(const FString& MatchId)
{
	FRegistry::Lobby.SendReadyConsentRequest(MatchId);
}


// Friends
void UAccelByteBlueprintsLobby::RequestFriend(FString UserId)
{
	FRegistry::Lobby.RequestFriend(UserId);
}

void UAccelByteBlueprintsLobby::Unfriend(FString UserId)
{
	FRegistry::Lobby.Unfriend(UserId);
}

void UAccelByteBlueprintsLobby::ListOutgoingFriends()
{
	FRegistry::Lobby.ListOutgoingFriends();
}

void UAccelByteBlueprintsLobby::CancelFriendRequest(FString UserId)
{
	FRegistry::Lobby.CancelFriendRequest(UserId);
}

void UAccelByteBlueprintsLobby::ListIncomingFriends()
{
	FRegistry::Lobby.ListIncomingFriends();
}

void UAccelByteBlueprintsLobby::AcceptFriend(FString UserId)
{
	FRegistry::Lobby.AcceptFriend(UserId);
}

void UAccelByteBlueprintsLobby::RejectFriend(FString UserId)
{
	FRegistry::Lobby.RejectFriend(UserId);
}

void UAccelByteBlueprintsLobby::LoadFriendsList()
{
	FRegistry::Lobby.LoadFriendsList();
}

void UAccelByteBlueprintsLobby::GetFriendshipStatus(FString UserId)
{
	FRegistry::Lobby.GetFriendshipStatus(UserId);
}

void UAccelByteBlueprintsLobby::BindEvent(
    const FConnectSuccess& OnSuccess,
    const FBlueprintErrorHandler& OnError,
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
	const FFriendAcceptFriendRequestNotif& OnAcceptFriendsNotif,
	const FIncomingFriendNotif& OnRequestFriendsNotif,
	const FBlueprintErrorHandler& OnParsingError)
{
    FSimpleDelegate OnSuccessDelegate = FSimpleDelegate::CreateLambda([OnSuccess]() {
        OnSuccess.ExecuteIfBound();
    });

    FErrorHandler OnErrorDelegate = FErrorHandler::CreateLambda([OnError](int32 Code, const FString& ErrorMessage) {
        OnError.ExecuteIfBound(Code, ErrorMessage);
    });

    AccelByte::Api::Lobby::FConnectionClosed OnConnectionCloseDelegate =
        AccelByte::Api::Lobby::FConnectionClosed::CreateLambda([OnConnectionClosed](int32  StatusCode, const FString& Reason, bool  WasClean) {
        OnConnectionClosed.ExecuteIfBound(StatusCode, Reason, WasClean);
    });
	
    // Party
	// This delegate is DEPRECATED. better to use FPartyMemberLeaveNotice
	AccelByte::Api::Lobby::FPartyLeaveNotif OnLeavePartyNoticeDelegate =
        AccelByte::Api::Lobby::FPartyLeaveNotif::CreateLambda([OnLeavePartyNotice](const FAccelByteModelsLeavePartyNotice& Result) {
        OnLeavePartyNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FPartyInviteNotif OnInvitePartyInvitationNoticeDelegate =
        AccelByte::Api::Lobby::FPartyInviteNotif::CreateLambda([OnInvitePartyInvitationNotice](const FAccelByteModelsInvitationNotice& Result) {
        OnInvitePartyInvitationNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FPartyGetInvitedNotif OnInvitePartyGetInvitedNoticeDelegate =
        AccelByte::Api::Lobby::FPartyGetInvitedNotif::CreateLambda([OnInvitePartyGetInvitedNotice](const FAccelByteModelsPartyGetInvitedNotice& Result) {
        OnInvitePartyGetInvitedNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FPartyJoinNotif OnInvitePartyJoinNoticeDelegate =
        AccelByte::Api::Lobby::FPartyJoinNotif::CreateLambda([OnInvitePartyJoinNotice](const FAccelByteModelsPartyJoinNotice& Result) {
        OnInvitePartyJoinNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FPartyKickNotif OnInvitePartyKickedNoticeDelegate =
        AccelByte::Api::Lobby::FPartyKickNotif::CreateLambda([OnInvitePartyKickedNotice](const FAccelByteModelsGotKickedFromPartyNotice& Result) {
        OnInvitePartyKickedNotice.ExecuteIfBound(Result);
    });

	AccelByte::Api::Lobby::FPartyMemberConnectNotif OnPartyConnectNoticeDelegate =
		AccelByte::Api::Lobby::FPartyMemberConnectNotif::CreateLambda([OnPartyConnectNotice](const FAccelByteModelsPartyMemberConnectionNotice& Result) {
		OnPartyConnectNotice.ExecuteIfBound(Result);
	});

	AccelByte::Api::Lobby::FPartyMemberDisconnectNotif OnPartyDisconnectNoticeDelegate =
		AccelByte::Api::Lobby::FPartyMemberDisconnectNotif::CreateLambda([OnPartyDisconnectNotice](const FAccelByteModelsPartyMemberConnectionNotice& Result) {
		OnPartyDisconnectNotice.ExecuteIfBound(Result);
	});
    
    // Chat
    AccelByte::Api::Lobby::FPersonalChatNotif OnPrivateMessageNoticeDelegate =
        AccelByte::Api::Lobby::FPersonalChatNotif::CreateLambda([OnPrivateMessageNotice](const FAccelByteModelsPersonalMessageNotice& Result) {
        OnPrivateMessageNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FPartyChatNotif OnPartyMessageNoticeDelegate =
        AccelByte::Api::Lobby::FPartyChatNotif::CreateLambda([OnPartyMessageNotice](const FAccelByteModelsPartyMessageNotice& Result) {
        OnPartyMessageNotice.ExecuteIfBound(Result);
    });

    // Presence
    AccelByte::Api::Lobby::FFriendStatusNotif OnOnUserPresenceNoticeDelegate =
        AccelByte::Api::Lobby::FFriendStatusNotif::CreateLambda([OnUserPresenceNotice](const FAccelByteModelsUsersPresenceNotice& Result) {
        OnUserPresenceNotice.ExecuteIfBound(Result);
    });

	// Notification
	AccelByte::Api::Lobby::FMessageNotif OnNotificationMessageDelegate =
		AccelByte::Api::Lobby::FMessageNotif::CreateLambda([OnNotificationMessage](const FAccelByteModelsNotificationMessage& Result) {
		OnNotificationMessage.ExecuteIfBound(Result);
	});

	// Matchmaking
	AccelByte::Api::Lobby::FMatchmakingNotif OnMatchmakingNoticeDelegate =
		AccelByte::Api::Lobby::FMatchmakingNotif::CreateLambda([OnMatchmakingNotice](const FAccelByteModelsMatchmakingNotice& Result) {
		OnMatchmakingNotice.ExecuteIfBound(Result);
	});

	AccelByte::Api::Lobby::FReadyConsentNotif OnReadyConsentNoticeDelegate =
		AccelByte::Api::Lobby::FReadyConsentNotif::CreateLambda([OnReadyConsentNotice](const FAccelByteModelsReadyConsentNotice& Result) {
		OnReadyConsentNotice.ExecuteIfBound(Result);
	});

	AccelByte::Api::Lobby::FRematchmakingNotif OnRematchmakingNoticeDelegate =
		AccelByte::Api::Lobby::FRematchmakingNotif::CreateLambda([OnRematchmakingNotice](const FAccelByteModelsRematchmakingNotice& Result) {
		OnRematchmakingNotice.ExecuteIfBound(Result);
	});

	AccelByte::Api::Lobby::FDsNotif OnDsNoticeDelegate =
		AccelByte::Api::Lobby::FDsNotif::CreateLambda([OnDsNotice](const FAccelByteModelsDsNotice& Result){
		OnDsNotice.ExecuteIfBound(Result);
	});

	// Friends + Notification
	AccelByte::Api::Lobby::FAcceptFriendsNotif OnAcceptFriendsNotifDelegate =
		AccelByte::Api::Lobby::FAcceptFriendsNotif::CreateLambda([OnAcceptFriendsNotif](const FAccelByteModelsAcceptFriendsNotif& Result) {
		OnAcceptFriendsNotif.Execute(Result);
	});

	AccelByte::Api::Lobby::FRequestFriendsNotif OnRequestFriendsNotifDelegate =
		AccelByte::Api::Lobby::FRequestFriendsNotif::CreateLambda([OnRequestFriendsNotif](const FAccelByteModelsRequestFriendsNotif& Result) {
		OnRequestFriendsNotif.Execute(Result);
	});

    FErrorHandler OnParsingErrorDelegate = FErrorHandler::CreateLambda([OnParsingError](int32 Code, const FString& ErrorMessage) {
        OnParsingError.ExecuteIfBound(Code, ErrorMessage);
    });
  
    FRegistry::Lobby.SetConnectSuccessDelegate(OnSuccessDelegate);
    FRegistry::Lobby.SetConnectFailedDelegate(OnErrorDelegate);
    FRegistry::Lobby.SetConnectionClosedDelegate(OnConnectionCloseDelegate);
    FRegistry::Lobby.SetPartyLeaveNotifDelegate(OnLeavePartyNoticeDelegate); // This delegate is DEPRECATED. better to use SetPartyMemberLeaveNotifDelegate().
    FRegistry::Lobby.SetPartyInviteNotifDelegate(OnInvitePartyInvitationNoticeDelegate);
    FRegistry::Lobby.SetPartyGetInvitedNotifDelegate(OnInvitePartyGetInvitedNoticeDelegate);
    FRegistry::Lobby.SetPartyJoinNotifDelegate(OnInvitePartyJoinNoticeDelegate);
    FRegistry::Lobby.SetPartyKickNotifDelegate(OnInvitePartyKickedNoticeDelegate);
	FRegistry::Lobby.SetPartyMemberConnectNotifDelegate(OnPartyConnectNoticeDelegate);
	FRegistry::Lobby.SetPartyMemberDisconnectNotifDelegate(OnPartyDisconnectNoticeDelegate);
    FRegistry::Lobby.SetPrivateMessageNotifDelegate(OnPrivateMessageNoticeDelegate);
    FRegistry::Lobby.SetPartyChatNotifDelegate(OnPartyMessageNoticeDelegate);
    FRegistry::Lobby.SetUserPresenceNotifDelegate(OnOnUserPresenceNoticeDelegate);
    FRegistry::Lobby.SetMessageNotifDelegate(OnNotificationMessageDelegate);
	FRegistry::Lobby.SetMatchmakingNotifDelegate(OnMatchmakingNoticeDelegate);
	FRegistry::Lobby.SetOnFriendRequestAcceptedNotifDelegate(OnAcceptFriendsNotifDelegate);
	FRegistry::Lobby.SetOnIncomingRequestFriendsNotifDelegate(OnRequestFriendsNotifDelegate);
    FRegistry::Lobby.SetParsingErrorDelegate(OnParsingErrorDelegate);
	FRegistry::Lobby.SetReadyConsentNotifDelegate(OnReadyConsentNoticeDelegate);
	FRegistry::Lobby.SetRematchmakingNotifDelegate(OnRematchmakingNoticeDelegate);
	FRegistry::Lobby.SetDsNotifDelegate(OnDsNoticeDelegate);
}
void UAccelByteBlueprintsLobby::UnbindDelegates()
{
    FRegistry::Lobby.UnbindEvent();
}

// Party
void UAccelByteBlueprintsLobby::SetInfoPartyResponseDelegate(FInfoPartyResponse OnInfoPartyResponse)
{
    AccelByte::Api::Lobby::FPartyInfoResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyInfoResponse::CreateLambda([OnInfoPartyResponse](const FAccelByteModelsInfoPartyResponse& Result) {
        OnInfoPartyResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetInfoPartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetCreatePartyResponseDelegate(FCreatePartyResponse OnCreatePartyResponse)
{
    AccelByte::Api::Lobby::FPartyCreateResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyCreateResponse::CreateLambda([OnCreatePartyResponse](const FAccelByteModelsCreatePartyResponse& Result) {
        OnCreatePartyResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetCreatePartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetLeavePartyResponseDelegate(FLeavePartyResponse OnLeavePartyResponse)
{
    AccelByte::Api::Lobby::FPartyLeaveResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyLeaveResponse::CreateLambda([OnLeavePartyResponse](const FAccelByteModelsLeavePartyResponse& Result) {
        OnLeavePartyResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetLeavePartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetInvitePartyResponseDelegate(FInvitePartyResponse OnInvitePartyResponse)
{
    AccelByte::Api::Lobby::FPartyInviteResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyInviteResponse::CreateLambda([OnInvitePartyResponse](const FAccelByteModelsPartyInviteResponse& Result) {
        OnInvitePartyResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetInvitePartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetInvitePartyJoinResponseDelegate(FInvitePartyJoinResponse OnInvitePartyJoinResponse)
{
    AccelByte::Api::Lobby::FPartyJoinResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyJoinResponse::CreateLambda([OnInvitePartyJoinResponse](const FAccelByteModelsPartyJoinResponse& Result) {
        OnInvitePartyJoinResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetInvitePartyJoinResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetInvitePartyKickMemberResponseDelegate(FInvitePartyKickMemberResponse OnInvitePartyKickMemberResponse)
{
    AccelByte::Api::Lobby::FPartyKickResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyKickResponse::CreateLambda([OnInvitePartyKickMemberResponse](const FAccelByteModelsKickPartyMemberResponse& Result) {
        OnInvitePartyKickMemberResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetInvitePartyKickMemberResponseDelegate(OnInfoPartyResponseDelegate);
}

// Chat
void UAccelByteBlueprintsLobby::SetPrivateMessageResponseDelegate(FPrivateMessageResponse OnPrivateMessageResponse)
{
    AccelByte::Api::Lobby::FPersonalChatResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPersonalChatResponse::CreateLambda([OnPrivateMessageResponse](const FAccelByteModelsPersonalMessageResponse& Result) {
        OnPrivateMessageResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetPrivateMessageResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetPartyMessageResponseDelegate(FPartyMessageResponse OnPartyMessageResponse)
{
    AccelByte::Api::Lobby::FPartyChatResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyChatResponse::CreateLambda([OnPartyMessageResponse](const FAccelByteModelsPartyMessageResponse& Result) {
        OnPartyMessageResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetPartyMessageResponseDelegate(OnInfoPartyResponseDelegate);
}

// Presence
void UAccelByteBlueprintsLobby::SetUserPresenceResponseDelegate(FSetUserPresenceResponse OnUserPresenceResponse)
{
    AccelByte::Api::Lobby::FSetUserPresenceResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FSetUserPresenceResponse::CreateLambda([OnUserPresenceResponse](const FAccelByteModelsSetOnlineUsersResponse& Result) {
        OnUserPresenceResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetUserPresenceResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetGetAllUserPresenceResponseDelegate(FGetAllFriendsStatusResponse OnGetAllUserPresenceResponse)
{
    AccelByte::Api::Lobby::FGetAllFriendsStatusResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FGetAllFriendsStatusResponse::CreateLambda([OnGetAllUserPresenceResponse](const FAccelByteModelsGetOnlineUsersResponse& Result) {
        OnGetAllUserPresenceResponse.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetGetAllUserPresenceResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetPresenceStatus(EAvailability State, FString GameName)
{
	FRegistry::Lobby.SendSetPresenceStatus(State, GameName);
}

// Notification
void UAccelByteBlueprintsLobby::GetAllAsyncNotification()
{
	FRegistry::Lobby.GetAllAsyncNotification();
}

// Matchmaking
void UAccelByteBlueprintsLobby::SetStartMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingStart)
{
	AccelByte::Api::Lobby::FMatchmakingResponse OnMatchmakingStartDelegate =
		AccelByte::Api::Lobby::FMatchmakingResponse::CreateLambda([OnMatchmakingStart](const FAccelByteModelsMatchmakingResponse& Result) {
		OnMatchmakingStart.ExecuteIfBound(Result);
	});
	FRegistry::Lobby.SetStartMatchmakingResponseDelegate(OnMatchmakingStartDelegate);
}

void UAccelByteBlueprintsLobby::SetCancelMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingCancel)
{
	AccelByte::Api::Lobby::FMatchmakingResponse OnMatchmakingCancelDelegate =
		AccelByte::Api::Lobby::FMatchmakingResponse::CreateLambda([OnMatchmakingCancel](const FAccelByteModelsMatchmakingResponse& Result) {
		OnMatchmakingCancel.ExecuteIfBound(Result);
	});
	FRegistry::Lobby.SetCancelMatchmakingResponseDelegate(OnMatchmakingCancelDelegate);
}

void UAccelByteBlueprintsLobby::SetReadyConsentResponseDelegate(FReadyConsentResponse OnReadyConsentResponse)
{
	AccelByte::Api::Lobby::FReadyConsentResponse OnReadyConsentResponseDelegate =
		AccelByte::Api::Lobby::FReadyConsentResponse::CreateLambda([OnReadyConsentResponse](const FAccelByteModelsReadyConsentRequest& Result){
		OnReadyConsentResponse.ExecuteIfBound(Result);
	});
	FRegistry::Lobby.SetReadyConsentResponseDelegate(OnReadyConsentResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetMatchmakingNotifDelegate(FMatchmakingNotice OnMatchmakingNotice)
{
    AccelByte::Api::Lobby::FMatchmakingNotif OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FMatchmakingNotif::CreateLambda([OnMatchmakingNotice](const FAccelByteModelsMatchmakingNotice& Result) {
        OnMatchmakingNotice.ExecuteIfBound(Result);
    });
    FRegistry::Lobby.SetMatchmakingNotifDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetReadyConsentNotifDelegate(FReadyConsentNotice OnReadyConsentNotice)
{
	AccelByte::Api::Lobby::FReadyConsentNotif OnReadyConsentNotifDelegate =
		AccelByte::Api::Lobby::FReadyConsentNotif::CreateLambda([OnReadyConsentNotice](const FAccelByteModelsReadyConsentNotice& Result) {
		OnReadyConsentNotice.ExecuteIfBound(Result);
	});
	FRegistry::Lobby.SetReadyConsentNotifDelegate(OnReadyConsentNotifDelegate);
}

void UAccelByteBlueprintsLobby::SetRematchmakingNotifDelegate(FRematchmakingNotice OnRematchmakingNotice)
{
	AccelByte::Api::Lobby::FRematchmakingNotif OnRematchmakingNotifDelegate =
		AccelByte::Api::Lobby::FRematchmakingNotif::CreateLambda([OnRematchmakingNotice](const FAccelByteModelsRematchmakingNotice& Result) {
		OnRematchmakingNotice.ExecuteIfBound(Result);
	});
	FRegistry::Lobby.SetRematchmakingNotifDelegate(OnRematchmakingNotifDelegate);
}

void UAccelByteBlueprintsLobby::SetDsNotifDelegate(FDsNotice OnDsNotice)
{
	AccelByte::Api::Lobby::FDsNotif OnDsNotifDelegate =
		AccelByte::Api::Lobby::FDsNotif::CreateLambda([OnDsNotice](const FAccelByteModelsDsNotice& Result) {
		OnDsNotice.ExecuteIfBound(Result);
	});
	FRegistry::Lobby.SetDsNotifDelegate(OnDsNotifDelegate);
}

// Friends
void UAccelByteBlueprintsLobby::SetRequestFriendResponseDelegate(FRequestFriendsResponseDelegate OnResponse)
{
	FRegistry::Lobby.SetRequestFriendsResponseDelegate(AccelByte::Api::Lobby::FRequestFriendsResponse::CreateLambda([OnResponse](const FAccelByteModelsRequestFriendsResponse& Result)
	{
		OnResponse.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::SetUnfriendResponseDelegate(FUnfriendResponseDelegate OnResponse)
{
	FRegistry::Lobby.SetUnfriendResponseDelegate(AccelByte::Api::Lobby::FUnfriendResponse::CreateLambda([OnResponse](const FAccelByteModelsUnfriendResponse& Result)
	{
		OnResponse.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::SetListOutgoingFriendsResponseDelegate(FListOutgoingFriendsResponseDelegate OnResponse)
{
	FRegistry::Lobby.SetListOutgoingFriendsResponseDelegate(AccelByte::Api::Lobby::FListOutgoingFriendsResponse::CreateLambda([OnResponse](const FAccelByteModelsListOutgoingFriendsResponse& Result)
	{
		OnResponse.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::SetCancelFriendRequestResponseDelegate(FCancelFriendsResponseDelegate OnResponse)
{
	FRegistry::Lobby.SetCancelFriendsResponseDelegate(AccelByte::Api::Lobby::FCancelFriendsResponse::CreateLambda([OnResponse](const FAccelByteModelsCancelFriendsResponse& Result)
	{
		OnResponse.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::SetListIncomingFriendsResponseDelegate(FListIncomingFriendsResponseDelegate OnResponse)
{
	FRegistry::Lobby.SetListIncomingFriendsResponseDelegate(AccelByte::Api::Lobby::FListIncomingFriendsResponse::CreateLambda([OnResponse](const FAccelByteModelsListIncomingFriendsResponse& Result)
	{
		OnResponse.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::SetAcceptFriendResponseDelegate(FAcceptFriendsResponseDelegate OnResponse)
{
	FRegistry::Lobby.SetAcceptFriendsResponseDelegate(AccelByte::Api::Lobby::FAcceptFriendsResponse::CreateLambda([OnResponse](const FAccelByteModelsAcceptFriendsResponse& Result)
	{
		OnResponse.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::SetRejectFriendResponseDelegate(FRejectFriendsResponseDelegate OnResponse)
{
	FRegistry::Lobby.SetRejectFriendsResponseDelegate(AccelByte::Api::Lobby::FRejectFriendsResponse::CreateLambda([OnResponse](const FAccelByteModelsRejectFriendsResponse& Result)
	{
		OnResponse.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::SetLoadFriendsListResponseDelegate(FLoadFriendListResponseDelegate OnResponse)
{
	FRegistry::Lobby.SetLoadFriendListResponseDelegate(AccelByte::Api::Lobby::FLoadFriendListResponse::CreateLambda([OnResponse](const FAccelByteModelsLoadFriendListResponse& Result)
	{
		OnResponse.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::SetGetFriendshipStatusResponseDelegate(FGetFriendshipStatusResponseDelegate OnResponse)
{
	FRegistry::Lobby.SetGetFriendshipStatusResponseDelegate(AccelByte::Api::Lobby::FGetFriendshipStatusResponse::CreateLambda([OnResponse](const FAccelByteModelsGetFriendshipStatusResponse& Result)
	{
		OnResponse.ExecuteIfBound(Result);
	}));
}
