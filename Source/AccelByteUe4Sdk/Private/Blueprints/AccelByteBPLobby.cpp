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

void ULobby::SetCreatePartyResponseDelegate(FDPartyCreateResponse OnResponse, FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetCreatePartyResponseDelegate(
		Api::Lobby::FPartyCreateResponse::CreateLambda(
			[OnResponse](FAccelByteModelsCreatePartyResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
}

void ULobby::SendCreatePartyRequest() const
{
	ApiClientPtr->Lobby.SendCreatePartyRequest();
}

void ULobby::SetPartyDataUpdateNotifDelegate(FDPartyDataUpdateNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyDataUpdateNotifDelegate(
		Api::Lobby::FPartyDataUpdateNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyDataNotif const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetInfoPartyResponseDelegate(FDInfoPartyResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInfoPartyResponseDelegate(
		Api::Lobby::FPartyInfoResponse::CreateLambda(
			[OnResponse](FAccelByteModelsInfoPartyResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendInfoPartyRequest() const
{
	ApiClientPtr->Lobby.SendInfoPartyRequest();
}

void ULobby::SetLeavePartyResponseDelegate(FDLeavePartyResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetLeavePartyResponseDelegate(
		Api::Lobby::FPartyLeaveResponse::CreateLambda(
			[OnResponse](FAccelByteModelsLeavePartyResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendLeavePartyRequest() const
{
	ApiClientPtr->Lobby.SendLeavePartyRequest();
}

void ULobby::SetInviteToPartyResponseDelegate(FDInviteToPartyResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInvitePartyResponseDelegate(
		Api::Lobby::FPartyInviteResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyInviteResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

auto ULobby::SendInviteToPartyRequest(FString const& UserId) -> void
{
	ApiClientPtr->Lobby.SendInviteToPartyRequest(UserId);
}

void ULobby::SetInvitePartyJoinResponseDelegate(FDPartyJoinResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInvitePartyJoinResponseDelegate(
		Api::Lobby::FPartyJoinResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyJoinReponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendAcceptInvitationRequest(FString const& PartyId, FString const& InvitationToken) const
{
	ApiClientPtr->Lobby.SendAcceptInvitationRequest(PartyId, InvitationToken);
}

void ULobby::SetRejectInvitationResponseDelegate(FDPartyRejectResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInvitePartyRejectResponseDelegate(
		Api::Lobby::FPartyRejectResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyRejectResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendRejectInvitationRequest(FString const& PartyId, FString const& InvitationToken) const
{
	ApiClientPtr->Lobby.SendRejectInvitationRequest(PartyId, InvitationToken);
}

void ULobby::SetKickPartyMemberResponseDelegate(FDPartyKickResponse OnResponse) const
{
	ApiClientPtr->Lobby.SetInvitePartyKickMemberResponseDelegate(
		Api::Lobby::FPartyKickResponse::CreateLambda(
			[OnResponse](FAccelByteModelsKickPartyMemberResponse Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}));
}

void ULobby::SendKickPartyMemberRequest(FString const& UserId) const
{
	ApiClientPtr->Lobby.SendKickPartyMemberRequest(UserId);
}

void ULobby::SetPartyGetInvitedNotifDelegate(FDPartyGetInvitedNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyGetInvitedNotifDelegate(
		Api::Lobby::FPartyGetInvitedNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyGetInvitedNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetPartyJoinNotifDelegate(FDPartyJoinNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyJoinNotifDelegate(
		Api::Lobby::FPartyJoinNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyJoinNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetPartyInvitationRejectedNotifDelegate(FDPartyRejectNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyInvitationRejectedNotifDelegate(
		Api::Lobby::FPartyRejectNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyRejectNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetPartyLeaveNotifDelegate(FDPartyLeaveNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyLeaveNotifDelegate(
		Api::Lobby::FPartyLeaveNotif::CreateLambda(
			[OnNotif](FAccelByteModelsLeavePartyNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetPartyKickNotifDelegate(FDPartyKickNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyKickNotifDelegate(
		Api::Lobby::FPartyKickNotif::CreateLambda(
			[OnNotif](FAccelByteModelsGotKickedFromPartyNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetPartyInviteNotifDelegate(FDPartyInviteNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyInviteNotifDelegate(
		Api::Lobby::FPartyInviteNotif::CreateLambda(
			[OnNotif](FAccelByteModelsInvitationNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

bool ULobby::IsConnected() const
{
	return ApiClientPtr->Lobby.IsConnected();
}

void ULobby::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}
