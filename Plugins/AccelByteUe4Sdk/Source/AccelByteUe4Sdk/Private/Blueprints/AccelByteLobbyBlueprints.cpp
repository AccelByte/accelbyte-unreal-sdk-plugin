// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteLobbyBlueprints.h"
#include "AccelByteLobbyApi.h"

using AccelByte::Api::Lobby;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsLobby::Connect()
{
	Lobby::Get().Connect();
}

void UAccelByteBlueprintsLobby::Disconnect()
{
	Lobby::Get().Disconnect();
}

bool UAccelByteBlueprintsLobby::IsConnected()
{
	return Lobby::Get().IsConnected();
}

void UAccelByteBlueprintsLobby::SendPing()
{
	Lobby::Get().SendPing();
}

void UAccelByteBlueprintsLobby::SendPrivateMessage(const FString& UserId, const FString& Message)
{
	Lobby::Get().SendPrivateMessage(UserId, Message);
}

void UAccelByteBlueprintsLobby::SendPartyMessage(const FString& Message)
{
	Lobby::Get().SendPartyMessage(Message);
}

void UAccelByteBlueprintsLobby::SendInfoPartyRequest()
{
	Lobby::Get().SendInfoPartyRequest();
}

void UAccelByteBlueprintsLobby::SendCreatePartyRequest()
{
	Lobby::Get().SendCreatePartyRequest();
}

void UAccelByteBlueprintsLobby::SendLeavePartyRequest()
{
	Lobby::Get().SendLeavePartyRequest();
}

void UAccelByteBlueprintsLobby::SendInviteToPartyRequest(const FString& UserId)
{
	Lobby::Get().SendInviteToPartyRequest(UserId);
}

void UAccelByteBlueprintsLobby::SendAcceptInvitationRequest(const FString& PartyId, const FString& InvitationToken)
{
	Lobby::Get().SendAcceptInvitationRequest(PartyId, InvitationToken);
}

void UAccelByteBlueprintsLobby::SendKickPartyMemberRequest(const FString& UserId)
{
	Lobby::Get().SendKickPartyMemberRequest(UserId);
}

void UAccelByteBlueprintsLobby::SendGetOnlineUsersRequest()
{
	Lobby::Get().SendGetOnlineUsersRequest();
}

void UAccelByteBlueprintsLobby::BindEvent(
    const FConnectSuccess& OnSuccess,
    const FBlueprintErrorHandler& OnError,
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
	Lobby::FMessageNotif OnNotificationMessageDelegate =
		Lobby::FMessageNotif::CreateLambda([OnNotificationMessage](const FAccelByteModelsNotificationMessage& Result) {
		OnNotificationMessage.ExecuteIfBound(Result);
	});

    FErrorHandler OnParsingErrorDelegate = FErrorHandler::CreateLambda([OnParsingError](int32 Code, const FString& ErrorMessage) {
        OnParsingError.ExecuteIfBound(Code, ErrorMessage);
    });
  
    Lobby::Get().SetConnectSuccessDelegate(OnSuccessDelegate);
    Lobby::Get().SetConnectFailedDelegate(OnErrorDelegate);
    Lobby::Get().SetConnectionClosedDelegate(OnConnectionCloseDelegate);
    Lobby::Get().SetPartyLeaveNotifDelegate(OnLeavePartyNoticeDelegate);
    Lobby::Get().SetPartyInviteNotifDelegate(OnInvitePartyInvitationNoticeDelegate);
    Lobby::Get().SetPartyGetInvitedNotifDelegate(OnInvitePartyGetInvitedNoticeDelegate);
    Lobby::Get().SetPartyJoinNotifDelegate(OnInvitePartyJoinNoticeDelegate);
    Lobby::Get().SetPartyKickNotifDelegate(OnInvitePartyKickedNoticeDelegate);
    Lobby::Get().SetPrivateMessageNotifDelegate(OnPrivateMessageNoticeDelegate);
    Lobby::Get().SetPartyChatNotifDelegate(OnPartyMessageNoticeDelegate);
    Lobby::Get().SetUserPresenceNotifDelegate(OnOnUserPresenceNoticeDelegate);
    Lobby::Get().SetMessageNotifDelegate(OnNotificationMessageDelegate);
    Lobby::Get().SetParsingErrorDelegate(OnParsingErrorDelegate);
}
void UAccelByteBlueprintsLobby::UnbindDelegates()
{
    Lobby::Get().UnbindEvent();
}

// Party
void UAccelByteBlueprintsLobby::SetInfoPartyResponseDelegate(FInfoPartyResponse OnInfoPartyResponse)
{
    AccelByte::Api::Lobby::FPartyInfoResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyInfoResponse::CreateLambda([OnInfoPartyResponse](const FAccelByteModelsInfoPartyResponse& Result) {
        OnInfoPartyResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetInfoPartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetCreatePartyResponseDelegate(FCreatePartyResponse OnCreatePartyResponse)
{
    AccelByte::Api::Lobby::FPartyCreateResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyCreateResponse::CreateLambda([OnCreatePartyResponse](const FAccelByteModelsCreatePartyResponse& Result) {
        OnCreatePartyResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetCreatePartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetLeavePartyResponseDelegate(FLeavePartyResponse OnLeavePartyResponse)
{
    AccelByte::Api::Lobby::FPartyLeaveResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyLeaveResponse::CreateLambda([OnLeavePartyResponse](const FAccelByteModelsLeavePartyResponse& Result) {
        OnLeavePartyResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetLeavePartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetInvitePartyResponseDelegate(FInvitePartyResponse OnInvitePartyResponse)
{
    AccelByte::Api::Lobby::FPartyInviteResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyInviteResponse::CreateLambda([OnInvitePartyResponse](const FAccelByteModelsPartyInviteResponse& Result) {
        OnInvitePartyResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetInvitePartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetInvitePartyJoinResponseDelegate(FInvitePartyJoinResponse OnInvitePartyJoinResponse)
{
    AccelByte::Api::Lobby::FPartyJoinResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyJoinResponse::CreateLambda([OnInvitePartyJoinResponse](const FAccelByteModelsPartyJoinReponse& Result) {
        OnInvitePartyJoinResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetInvitePartyJoinResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetInvitePartyKickMemberResponseDelegate(FInvitePartyKickMemberResponse OnInvitePartyKickMemberResponse)
{
    AccelByte::Api::Lobby::FPartyKickResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyKickResponse::CreateLambda([OnInvitePartyKickMemberResponse](const FAccelByteModelsKickPartyMemberResponse& Result) {
        OnInvitePartyKickMemberResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetInvitePartyKickMemberResponseDelegate(OnInfoPartyResponseDelegate);
}

// Chat
void UAccelByteBlueprintsLobby::SetPrivateMessageResponseDelegate(FPrivateMessageResponse OnPrivateMessageResponse)
{
    AccelByte::Api::Lobby::FPersonalChatResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPersonalChatResponse::CreateLambda([OnPrivateMessageResponse](const FAccelByteModelsPersonalMessageResponse& Result) {
        OnPrivateMessageResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetPrivateMessageResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetPartyMessageResponseDelegate(FPartyMessageResponse OnPartyMessageResponse)
{
    AccelByte::Api::Lobby::FPartyChatResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyChatResponse::CreateLambda([OnPartyMessageResponse](const FAccelByteModelsPartyMessageResponse& Result) {
        OnPartyMessageResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetPartyMessageResponseDelegate(OnInfoPartyResponseDelegate);
}

// Presence
void UAccelByteBlueprintsLobby::SetUserPresenceResponseDelegate(FSetUserPresenceResponse OnUserPresenceResponse)
{
    AccelByte::Api::Lobby::FSetUserPresenceResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FSetUserPresenceResponse::CreateLambda([OnUserPresenceResponse](const FAccelByteModelsSetOnlineUsersResponse& Result) {
        OnUserPresenceResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetUserPresenceResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetGetAllUserPresenceResponseDelegate(FGetAllFriendsStatusResponse OnGetAllUserPresenceResponse)
{
    AccelByte::Api::Lobby::FGetAllFriendsStatusResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FGetAllFriendsStatusResponse::CreateLambda([OnGetAllUserPresenceResponse](const FAccelByteModelsGetOnlineUsersResponse& Result) {
        OnGetAllUserPresenceResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetGetAllUserPresenceResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetPresenceStatus(Availability State, FString GameName)
{
	Lobby::Get().SendSetPresenceStatus(State, GameName);
}

// Notification
void UAccelByteBlueprintsLobby::GetAllAsyncNotification()
{
	Lobby::Get().GetAllAsyncNotification();
}

// Matchmaking
void UAccelByteBlueprintsLobby::SetMatchmakingResponseDelegate(FMatchmakingResponse OnMatchmakingResponse)
{
    AccelByte::Api::Lobby::FMatchmakingResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FMatchmakingResponse::CreateLambda([OnMatchmakingResponse](const FAccelByteModelsMatchmakingResponse& Result) {
        OnMatchmakingResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetMatchmakingResponseDelegate(OnInfoPartyResponseDelegate);
}