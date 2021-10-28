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

void ULobby::SetOnConnected(FDHandler OnConnected) const
{
	ApiClientPtr->Lobby.SetConnectSuccessDelegate(
		FVoidHandler::CreateLambda(
			[OnConnected]()
			{
				auto _ = OnConnected.ExecuteIfBound();
			}));
}

void ULobby::CreateParty(FDPartyCreateResponse OnResponse, FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetCreatePartyResponseDelegate(
		Api::Lobby::FPartyCreateResponse::CreateLambda(
			[OnResponse](FAccelByteModelsCreatePartyResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));

	ApiClientPtr->Lobby.SendCreatePartyRequest();
}

void ULobby::SetOnPartyDataUpdate(FDPartyDataUpdateNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyDataUpdateNotifDelegate(
		Api::Lobby::FPartyDataUpdateNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyDataNotif const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::PartyInfo(FDInfoPartyResponse OnResponse, FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetInfoPartyResponseDelegate(
		Api::Lobby::FPartyInfoResponse::CreateLambda(
			[OnResponse](FAccelByteModelsInfoPartyResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendInfoPartyRequest();
}

void ULobby::PartyLeave(FDLeavePartyResponse OnResponse, FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetLeavePartyResponseDelegate(
		Api::Lobby::FPartyLeaveResponse::CreateLambda(
			[OnResponse](FAccelByteModelsLeavePartyResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendLeavePartyRequest();
}

void ULobby::PartyInvite(
	FPartyInviteRequest Request,
	FDPartyInviteResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetInvitePartyResponseDelegate(
		Api::Lobby::FPartyInviteResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyInviteResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendInviteToPartyRequest(Request.friendID);
}

void ULobby::PartyJoin(
	FPartyJoinRequest const& Request,
	FDPartyJoinResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetInvitePartyJoinResponseDelegate(
		Api::Lobby::FPartyJoinResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyJoinReponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendAcceptInvitationRequest(Request.partyID, Request.invitationToken);
}

void ULobby::PartyReject(
	FPartyRejectRequest Request,
	FDPartyRejectResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetInvitePartyRejectResponseDelegate(
		Api::Lobby::FPartyRejectResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyRejectResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendRejectInvitationRequest(Request.partyID, Request.invitationToken);
}

void ULobby::PartyKick(
	FPartyKickRequest const& Request,
	FDPartyKickResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetInvitePartyKickMemberResponseDelegate(
		Api::Lobby::FPartyKickResponse::CreateLambda(
			[OnResponse](FAccelByteModelsKickPartyMemberResponse Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendKickPartyMemberRequest(Request.memberID);
}

void ULobby::SetOnPartyGetInvited(FDPartyGetInvitedNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyGetInvitedNotifDelegate(
		Api::Lobby::FPartyGetInvitedNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyGetInvitedNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyJoin(FDPartyJoinNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyJoinNotifDelegate(
		Api::Lobby::FPartyJoinNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyJoinNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyReject(FDPartyRejectNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyInvitationRejectedNotifDelegate(
		Api::Lobby::FPartyRejectNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyRejectNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyLeave(FDPartyLeaveNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyLeaveNotifDelegate(
		Api::Lobby::FPartyLeaveNotif::CreateLambda(
			[OnNotif](FAccelByteModelsLeavePartyNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyKick(FDPartyKickNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyKickNotifDelegate(
		Api::Lobby::FPartyKickNotif::CreateLambda(
			[OnNotif](FAccelByteModelsGotKickedFromPartyNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnPartyInvite(FDPartyInviteNotif OnNotif) const
{
	ApiClientPtr->Lobby.SetPartyInviteNotifDelegate(
		Api::Lobby::FPartyInviteNotif::CreateLambda(
			[OnNotif](FAccelByteModelsInvitationNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::StartMatchmaking(
	FStartMatchmakingRequest const& Request,
	FDStartMatchmakingResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetStartMatchmakingResponseDelegate(
		Api::Lobby::FMatchmakingResponse::CreateLambda(
			[OnResponse](FAccelByteModelsMatchmakingResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendStartMatchmaking(Request.gameMode);
}

void ULobby::CancelMatchmaking(
	FCancelMatchmakingRequest const& Request,
	FDCancelMatchmakingResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetCancelMatchmakingResponseDelegate(
		Api::Lobby::FMatchmakingResponse::CreateLambda(
			[OnResponse](FAccelByteModelsMatchmakingResponse const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendCancelMatchmaking(Request.gameMode, Request.isTempParty);
}

void ULobby::SetReadyConsent(
	FSetReadyConsentRequest const& Request,
	FDSetReadyConsentResponse OnResponse,
	FDErrorHandler OnError) const
{
	ApiClientPtr->Lobby.SetReadyConsentResponseDelegate(
		Api::Lobby::FReadyConsentResponse::CreateLambda(
			[OnResponse](FAccelByteModelsReadyConsentRequest const& Response)
			{
				auto _ = OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				auto _ = OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendReadyConsentRequest(Request.matchId);
}

void ULobby::SetOnMatchmaking(FDMatchmakingNotif OnNotif)
{
	ApiClientPtr->Lobby.SetMatchmakingNotifDelegate(
		Api::Lobby::FMatchmakingNotif::CreateLambda(
			[OnNotif](FAccelByteModelsMatchmakingNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnSetReadyConsent(FDSetReadyConsentNotif OnNotif)
{
	ApiClientPtr->Lobby.SetReadyConsentNotifDelegate(
		Api::Lobby::FReadyConsentNotif::CreateLambda(
			[OnNotif](FAccelByteModelsReadyConsentNotice const& Notif)
			{
				auto _ = OnNotif.ExecuteIfBound(Notif);
			}));
}

void ULobby::SetOnRematchmaking(FDRematchmakingNotif OnNotif)
{
	ApiClientPtr->Lobby.SetRematchmakingNotifDelegate(
		Api::Lobby::FRematchmakingNotif::CreateLambda(
			[OnNotif](FAccelByteModelsRematchmakingNotice const& Notif)
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
