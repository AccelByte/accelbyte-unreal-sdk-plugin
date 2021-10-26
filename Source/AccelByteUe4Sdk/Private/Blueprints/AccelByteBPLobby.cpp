#include "Blueprints/AccelByteBPLobby.h"

void ULobby::Connect() const
{
	ApiClientPtr->Lobby.Connect();
}

void ULobby::Disconnect() const
{
	return ApiClientPtr->Lobby.Disconnect();
}

void ULobby::UnbindEvent() const
{
	ApiClientPtr->Lobby.UnbindEvent();
}

void ULobby::SetConnectSuccessDelegate(FDHandler OnConnectSuccess) const
{
	ApiClientPtr->Lobby.SetConnectSuccessDelegate(
		FVoidHandler::CreateLambda(
			[OnConnectSuccess]()
			{
				auto _ = OnConnectSuccess.ExecuteIfBound();
			}));
}

void ULobby::SetOnCreatePartyResponse(FDPartyCreateResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetCreatePartyResponseDelegate(
		Api::Lobby::FPartyCreateResponse::CreateLambda(
			[OnResponse](FAccelByteModelsCreatePartyResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendCreatePartyRequest() const
{
	ApiClientPtr->Lobby.SendCreatePartyRequest();
}

void ULobby::SetOnPartyDataUpdateNotif(FDPartyDataUpdateNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyDataUpdateNotifDelegate(
		Api::Lobby::FPartyDataUpdateNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyDataNotif const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyInfoResponse(FDInfoPartyResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInfoPartyResponseDelegate(
		Api::Lobby::FPartyInfoResponse::CreateLambda(
			[OnResponse](FAccelByteModelsInfoPartyResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendPartyInfoRequest() const
{
	ApiClientPtr->Lobby.SendInfoPartyRequest();
}

void ULobby::SetOnPartyLeaveResponse(FDLeavePartyResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetLeavePartyResponseDelegate(
		Api::Lobby::FPartyLeaveResponse::CreateLambda(
			[OnResponse](FAccelByteModelsLeavePartyResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendPartyLeaveRequest() const
{
	ApiClientPtr->Lobby.SendLeavePartyRequest();
}

void ULobby::SetOnPartyInviteResponse(FDPartyInviteResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInvitePartyResponseDelegate(
		Api::Lobby::FPartyInviteResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyInviteResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

auto ULobby::SendPartyInviteRequest(FPartyInviteRequest Request) -> void
{
	ApiClientPtr->Lobby.SendInviteToPartyRequest(Request.friendID);
}

void ULobby::SetOnPartyJoinResponse(FDPartyJoinResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInvitePartyJoinResponseDelegate(
		Api::Lobby::FPartyJoinResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyJoinReponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendPartyJoinRequest(FPartyJoinRequest Request) const
{
	ApiClientPtr->Lobby.SendAcceptInvitationRequest(Request.partyID, Request.invitationToken);
}

void ULobby::SetOnPartyRejectResponse(FDPartyRejectResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInvitePartyRejectResponseDelegate(
		Api::Lobby::FPartyRejectResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyRejectResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendPartyRejectRequest(FPartyRejectRequest Request) const
{
	ApiClientPtr->Lobby.SendRejectInvitationRequest(Request.partyID, Request.invitationToken);
}

void ULobby::SetOnPartyKickResponse(FDPartyKickResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInvitePartyKickMemberResponseDelegate(
		Api::Lobby::FPartyKickResponse::CreateLambda(
			[OnResponse](FAccelByteModelsKickPartyMemberResponse Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendPartyKickRequest(FPartyKickRequest Request) const
{
	ApiClientPtr->Lobby.SendKickPartyMemberRequest(Request.memberID);
}

void ULobby::SetOnPartyGetInvitedNotif(FDPartyGetInvitedNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyGetInvitedNotifDelegate(
		Api::Lobby::FPartyGetInvitedNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyGetInvitedNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyJoinNotif(FDPartyJoinNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyJoinNotifDelegate(
		Api::Lobby::FPartyJoinNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyJoinNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyRejectNotif(FDPartyRejectNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyInvitationRejectedNotifDelegate(
		Api::Lobby::FPartyRejectNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyRejectNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyLeaveNotif(FDPartyLeaveNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyLeaveNotifDelegate(
		Api::Lobby::FPartyLeaveNotif::CreateLambda(
			[OnNotif](FAccelByteModelsLeavePartyNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyKickNotif(FDPartyKickNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyKickNotifDelegate(
		Api::Lobby::FPartyKickNotif::CreateLambda(
			[OnNotif](FAccelByteModelsGotKickedFromPartyNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyInviteNotif(FDPartyInviteNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyInviteNotifDelegate(
		Api::Lobby::FPartyInviteNotif::CreateLambda(
			[OnNotif](FAccelByteModelsInvitationNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnMatchmakingStartResponse(FDMatchmakingStartResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetStartMatchmakingResponseDelegate(
		Api::Lobby::FMatchmakingResponse::CreateLambda(
			[OnResponse](FAccelByteModelsMatchmakingResponse Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendMatchmakingStartRequest(FMatchmakingStartRequest Request) const
{
	ApiClientPtr->Lobby.SendStartMatchmaking(Request.gameMode);
}

bool ULobby::IsConnected() const
{
	return ApiClientPtr->Lobby.IsConnected();
}

void ULobby::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}
