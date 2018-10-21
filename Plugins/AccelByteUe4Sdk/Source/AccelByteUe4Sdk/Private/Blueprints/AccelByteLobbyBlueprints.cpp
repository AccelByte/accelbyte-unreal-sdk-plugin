// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
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

void UAccelByteBlueprintsLobby::SendAcceptInvitationRequest(const FString& PartyId, FString InvitationToken)
{
	Lobby::Get().SendAcceptInvitationRequest(PartyId, InvitationToken);
}

void UAccelByteBlueprintsLobby::BindDelegates(
	const FPrivateMessageNotice& PrivateMessageNotice,
	const FPartyMessageNotice& PartyMessageNotice,
	const FInfoPartyResponse& InfoPartyResponse,
	const FCreatePartyResponse& CreatePartyResponse,
	const FLeavePartyResponse& LeavePartyResponse,
	const FInviteToPartyResponse& InviteToPartyResponse,
	const FPartyInvitationNotice& PartyInvitationNotice,
	const FAcceptInvitationResponse& AcceptInvitationResponse,
	const FPartyInvitationAcceptanceNotice& PartyInvitationAcceptanceNotice)
{
	Lobby::Get().BindDelegates(Lobby::FPrivateMessageNotice::CreateLambda([PrivateMessageNotice](const FAccelByteModelsPrivateMessageNotice& Result)
	{
		PrivateMessageNotice.ExecuteIfBound(Result);
	}),
	Lobby::FPartyMessageNotice::CreateLambda([PartyMessageNotice](const FAccelByteModelsPartyMessageNotice& Result)
	{
		PartyMessageNotice.ExecuteIfBound(Result);
	}),
	Lobby::FInfoPartyResponse::CreateLambda([InfoPartyResponse](const FAccelByteModelsInfoPartyResponse& Result)
	{
		InfoPartyResponse.ExecuteIfBound(Result);
	}),
	Lobby::FCreatePartyResponse::CreateLambda([CreatePartyResponse](const FAccelByteModelsCreatePartyResponse& Result)
	{
		CreatePartyResponse.ExecuteIfBound(Result);
	}),
	Lobby::FLeavePartyResponse::CreateLambda([LeavePartyResponse](const FAccelByteModelsLeavePartyResponse& Result)
	{
		LeavePartyResponse.ExecuteIfBound(Result);
	}),
	Lobby::FInviteToPartyResponse::CreateLambda([InviteToPartyResponse](const FAccelByteModelsInviteToPartyResponse& Result)
	{
		InviteToPartyResponse.ExecuteIfBound(Result);
	}),
	Lobby::FPartyInvitationNotice::CreateLambda([PartyInvitationNotice](const FAccelByteModelsPartyInvitationNotice& Result)
	{
		PartyInvitationNotice.ExecuteIfBound(Result);
	}),
	Lobby::FAcceptInvitationResponse::CreateLambda([AcceptInvitationResponse](const FAccelByteModelsAcceptInvitationReponse& Result)
	{
		AcceptInvitationResponse.ExecuteIfBound(Result);
	}),
	Lobby::FPartyInvitationAcceptanceNotice::CreateLambda([PartyInvitationAcceptanceNotice](const FAccelByteModelsPartyInvitationAcceptanceNotice& Result)
	{
		PartyInvitationAcceptanceNotice.ExecuteIfBound(Result);
	}));
}

void UAccelByteBlueprintsLobby::UnbindDelegates()
{
	Lobby::Get().UnbindDelegates();
}
