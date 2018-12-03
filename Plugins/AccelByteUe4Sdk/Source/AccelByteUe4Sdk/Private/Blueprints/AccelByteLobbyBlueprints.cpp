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
    AccelByte::Api::Lobby::FLeavePartyNotice OnLeavePartyNoticeDelegate =
        AccelByte::Api::Lobby::FLeavePartyNotice::CreateLambda([OnLeavePartyNotice](const FAccelByteModelsLeavePartyNotice& Result) {
        OnLeavePartyNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FInvitePartyInvitationNotice OnInvitePartyInvitationNoticeDelegate =
        AccelByte::Api::Lobby::FInvitePartyInvitationNotice::CreateLambda([OnInvitePartyInvitationNotice](const FAccelByteModelsInvitationNotice& Result) {
        OnInvitePartyInvitationNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FInvitePartyGetInvitedNotice OnInvitePartyGetInvitedNoticeDelegate =
        AccelByte::Api::Lobby::FInvitePartyGetInvitedNotice::CreateLambda([OnInvitePartyGetInvitedNotice](const FAccelByteModelsPartyGetInvitedNotice& Result) {
        OnInvitePartyGetInvitedNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FInvitePartyJoinNotice OnInvitePartyJoinNoticeDelegate =
        AccelByte::Api::Lobby::FInvitePartyJoinNotice::CreateLambda([OnInvitePartyJoinNotice](const FAccelByteModelsPartyJoinNotice& Result) {
        OnInvitePartyJoinNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FInvitePartyKickedNotice OnInvitePartyKickedNoticeDelegate =
        AccelByte::Api::Lobby::FInvitePartyKickedNotice::CreateLambda([OnInvitePartyKickedNotice](const FAccelByteModelsGotKickedFromPartyNotice& Result) {
        OnInvitePartyKickedNotice.ExecuteIfBound(Result);
    });
    
    // Chat
    AccelByte::Api::Lobby::FPrivateMessageNotice OnPrivateMessageNoticeDelegate =
        AccelByte::Api::Lobby::FPrivateMessageNotice::CreateLambda([OnPrivateMessageNotice](const FAccelByteModelsPersonalMessageNotice& Result) {
        OnPrivateMessageNotice.ExecuteIfBound(Result);
    });

    AccelByte::Api::Lobby::FPartyMessageNotice OnPartyMessageNoticeDelegate =
        AccelByte::Api::Lobby::FPartyMessageNotice::CreateLambda([OnPartyMessageNotice](const FAccelByteModelsPartyMessageNotice& Result) {
        OnPartyMessageNotice.ExecuteIfBound(Result);
    });

    // Presence
    AccelByte::Api::Lobby::FUserPresenceNotice OnOnUserPresenceNoticeDelegate =
        AccelByte::Api::Lobby::FUserPresenceNotice::CreateLambda([OnUserPresenceNotice](const FAccelByteModelsUsersPresenceNotice& Result) {
        OnUserPresenceNotice.ExecuteIfBound(Result);
    });

    FErrorHandler OnParsingErrorDelegate = FErrorHandler::CreateLambda([OnParsingError](int32 Code, const FString& ErrorMessage) {
        OnParsingError.ExecuteIfBound(Code, ErrorMessage);
    });
    
	Lobby::Get().BindEvent(
        OnSuccessDelegate,
        OnErrorDelegate,
        OnConnectionCloseDelegate,
        OnLeavePartyNoticeDelegate,
        OnInvitePartyInvitationNoticeDelegate,
        OnInvitePartyGetInvitedNoticeDelegate,
        OnInvitePartyJoinNoticeDelegate,
        OnInvitePartyKickedNoticeDelegate,
        OnPrivateMessageNoticeDelegate,
        OnPartyMessageNoticeDelegate,
        OnOnUserPresenceNoticeDelegate,
        OnParsingErrorDelegate);
}
void UAccelByteBlueprintsLobby::UnbindDelegates()
{
    Lobby::Get().UnbindEvent();
}

// Party
void UAccelByteBlueprintsLobby::SetInfoPartyResponseDelegate(FInfoPartyResponse OnInfoPartyResponse)
{
    AccelByte::Api::Lobby::FInfoPartyResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FInfoPartyResponse::CreateLambda([OnInfoPartyResponse](const FAccelByteModelsInfoPartyResponse& Result) {
        OnInfoPartyResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetInfoPartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetCreatePartyResponseDelegate(FCreatePartyResponse OnCreatePartyResponse)
{
    AccelByte::Api::Lobby::FCreatePartyResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FCreatePartyResponse::CreateLambda([OnCreatePartyResponse](const FAccelByteModelsCreatePartyResponse& Result) {
        OnCreatePartyResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetCreatePartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetLeavePartyResponseDelegate(FLeavePartyResponse OnLeavePartyResponse)
{
    AccelByte::Api::Lobby::FLeavePartyResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FLeavePartyResponse::CreateLambda([OnLeavePartyResponse](const FAccelByteModelsLeavePartyResponse& Result) {
        OnLeavePartyResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetLeavePartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetInvitePartyResponseDelegate(FInvitePartyResponse OnInvitePartyResponse)
{
    AccelByte::Api::Lobby::FInvitePartyResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FInvitePartyResponse::CreateLambda([OnInvitePartyResponse](const FAccelByteModelsPartyInviteResponse& Result) {
        OnInvitePartyResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetInvitePartyResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetInvitePartyJoinResponseDelegate(FInvitePartyJoinResponse OnInvitePartyJoinResponse)
{
    AccelByte::Api::Lobby::FInvitePartyJoinResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FInvitePartyJoinResponse::CreateLambda([OnInvitePartyJoinResponse](const FAccelByteModelsPartyJoinReponse& Result) {
        OnInvitePartyJoinResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetInvitePartyJoinResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetInvitePartyKickMemberResponseDelegate(FInvitePartyKickMemberResponse OnInvitePartyKickMemberResponse)
{
    AccelByte::Api::Lobby::FInvitePartyKickMemberResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FInvitePartyKickMemberResponse::CreateLambda([OnInvitePartyKickMemberResponse](const FAccelByteModelsKickPartyMemberResponse& Result) {
        OnInvitePartyKickMemberResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetInvitePartyKickMemberResponseDelegate(OnInfoPartyResponseDelegate);
}

// Chat
void UAccelByteBlueprintsLobby::SetPrivateMessageResponseDelegate(FPrivateMessageResponse OnPrivateMessageResponse)
{
    AccelByte::Api::Lobby::FPrivateMessageResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPrivateMessageResponse::CreateLambda([OnPrivateMessageResponse](const FAccelByteModelsPersonalMessageResponse& Result) {
        OnPrivateMessageResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetPrivateMessageResponseDelegate(OnInfoPartyResponseDelegate);
}

void UAccelByteBlueprintsLobby::SetPartyMessageResponseDelegate(FPartyMessageResponse OnPartyMessageResponse)
{
    AccelByte::Api::Lobby::FPartyMessageResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FPartyMessageResponse::CreateLambda([OnPartyMessageResponse](const FAccelByteModelsPartyMessageResponse& Result) {
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

void UAccelByteBlueprintsLobby::SetGetAllUserPresenceResponseDelegate(FGetAllUserPresenceResponse OnGetAllUserPresenceResponse)
{
    AccelByte::Api::Lobby::FGetAllUserPresenceResponse OnInfoPartyResponseDelegate =
        AccelByte::Api::Lobby::FGetAllUserPresenceResponse::CreateLambda([OnGetAllUserPresenceResponse](const FAccelByteModelsGetOnlineUsersResponse& Result) {
        OnGetAllUserPresenceResponse.ExecuteIfBound(Result);
    });
    Lobby::Get().SetGetAllUserPresenceResponseDelegate(OnInfoPartyResponseDelegate);
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