// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteLobbyBlueprints.h"
#include "AccelByteLobbyApi.h"

using AccelByte::Api::Lobby;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsLobby::Connect(const FConnectSuccess& OnSuccess, const FBlueprintErrorHandler& OnError, const FConnectionClosed& OnConnectionClosed)
{
	Lobby::Get().Connect(Lobby::FConnectSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}),
		Lobby::FConnectionClosed::CreateLambda([OnConnectionClosed](int32 StatusCode, const FString& Reason, bool WasClean)
	{
		OnConnectionClosed.ExecuteIfBound(StatusCode, Reason, WasClean);
	}));
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

void UAccelByteBlueprintsLobby::BindDelegates(
	const FPrivateMessageNotice& OnPrivateMessageNotice,
	const FPartyMessageNotice& OnPartyMessageNotice,
	const FInfoPartyResponse& OnInfoPartyResponse,
	const FCreatePartyResponse& OnCreatePartyResponse,
	const FLeavePartyResponse& OnLeavePartyResponse,
	const FInviteToPartyResponse& OnInviteToPartyResponse,
	const FPartyInvitationNotice& OnPartyInvitationNotice,
	const FAcceptInvitationResponse& OnAcceptInvitationResponse,
	const FPartyInvitationAcceptanceNotice& OnPartyInvitationAcceptanceNotice,
	const FKickPartyMemberResponse& OnKickPartyMemberResponse,
	const FGotKickedNoticeFromParty& OnGotKickedNoticeFromParty,
	const FGetOnlineUsersResponse& OnGetOnlineUsersResponse)
{
	Lobby::Get().BindDelegates(Lobby::FPrivateMessageNotice::CreateLambda([OnPrivateMessageNotice](const FAccelByteModelsPrivateMessageNotice& Result)
	{
		OnPrivateMessageNotice.ExecuteIfBound(Result);
	}),
	Lobby::FPartyMessageNotice::CreateLambda([OnPartyMessageNotice](const FAccelByteModelsPartyMessageNotice& Result)
	{
		OnPartyMessageNotice.ExecuteIfBound(Result);
	}),
	Lobby::FInfoPartyResponse::CreateLambda([OnInfoPartyResponse](const FAccelByteModelsInfoPartyResponse& Result)
	{
		OnInfoPartyResponse.ExecuteIfBound(Result);
	}),
	Lobby::FCreatePartyResponse::CreateLambda([OnCreatePartyResponse](const FAccelByteModelsCreatePartyResponse& Result)
	{
		OnCreatePartyResponse.ExecuteIfBound(Result);
	}),
	Lobby::FLeavePartyResponse::CreateLambda([OnLeavePartyResponse](const FAccelByteModelsLeavePartyResponse& Result)
	{
		OnLeavePartyResponse.ExecuteIfBound(Result);
	}),
	Lobby::FInviteToPartyResponse::CreateLambda([OnInviteToPartyResponse](const FAccelByteModelsInviteToPartyResponse& Result)
	{
		OnInviteToPartyResponse.ExecuteIfBound(Result);
	}),
	Lobby::FPartyInvitationNotice::CreateLambda([OnPartyInvitationNotice](const FAccelByteModelsPartyInvitationNotice& Result)
	{
		OnPartyInvitationNotice.ExecuteIfBound(Result);
	}),
	Lobby::FAcceptInvitationResponse::CreateLambda([OnAcceptInvitationResponse](const FAccelByteModelsAcceptInvitationReponse& Result)
	{
		OnAcceptInvitationResponse.ExecuteIfBound(Result);
	}),
	Lobby::FPartyInvitationAcceptanceNotice::CreateLambda([OnPartyInvitationAcceptanceNotice](const FAccelByteModelsPartyInvitationAcceptanceNotice& Result)
	{
		OnPartyInvitationAcceptanceNotice.ExecuteIfBound(Result);
	}),
	Lobby::FKickPartyMemberResponse::CreateLambda([OnKickPartyMemberResponse](const FAccelByteModelsKickPartyMemberResponse& Result)
	{
		OnKickPartyMemberResponse.ExecuteIfBound(Result);
	}),
	Lobby::FGotKickedNoticeFromParty::CreateLambda([OnGotKickedNoticeFromParty](const FAccelByteModelsGotKickedFromPartyNotice& Result)
	{
		OnGotKickedNoticeFromParty.ExecuteIfBound(Result);
	}),
	Lobby::FGetOnlineUsersResponse::CreateLambda([OnGetOnlineUsersResponse](const FAccelByteModelsGetOnlineUsersResponse& Result)
	{
		OnGetOnlineUsersResponse.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::UnbindDelegates()
{
	Lobby::Get().UnbindDelegates();
}
