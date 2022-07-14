// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABParty.h"
#include "Api/AccelByteLobbyApi.h"
#include "Core/AccelByteRegistry.h"

void UABParty::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABParty::PartyInfo(FDInfoPartyResponse OnResponse, FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetInfoPartyResponseDelegate(
		Api::Lobby::FPartyInfoResponse::CreateLambda(
			[OnResponse](FAccelByteModelsInfoPartyResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendInfoPartyRequest();
}

void UABParty::CreateParty(FDPartyCreateResponse OnResponse, FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetCreatePartyResponseDelegate(
		Api::Lobby::FPartyCreateResponse::CreateLambda(
			[OnResponse](FAccelByteModelsCreatePartyResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));

	ApiClientPtr->Lobby.SendCreatePartyRequest();
}

void UABParty::PartyLeave(FDLeavePartyResponse OnResponse, FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetLeavePartyResponseDelegate(
		Api::Lobby::FPartyLeaveResponse::CreateLambda(
			[OnResponse](FAccelByteModelsLeavePartyResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendLeavePartyRequest();
}

void UABParty::PartyInvite(
	FPartyInviteRequest Request,
	FDPartyInviteResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetInvitePartyResponseDelegate(
		Api::Lobby::FPartyInviteResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyInviteResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendInviteToPartyRequest(Request.friendID);
}

void UABParty::PartyJoin(
	FPartyJoinRequest const& Request,
	FDPartyJoinResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetInvitePartyJoinResponseDelegate(
		Api::Lobby::FPartyJoinResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyJoinResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendAcceptInvitationRequest(Request.partyID, Request.invitationToken);
}

void UABParty::PartyReject(
	FPartyRejectRequest Request,
	FDPartyRejectResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetInvitePartyRejectResponseDelegate(
		Api::Lobby::FPartyRejectResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyRejectResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendRejectInvitationRequest(Request.partyID, Request.invitationToken);
}

void UABParty::PartyKick(
	FPartyKickRequest const& Request,
	FDPartyKickResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetInvitePartyKickMemberResponseDelegate(
		Api::Lobby::FPartyKickResponse::CreateLambda(
			[OnResponse](FAccelByteModelsKickPartyMemberResponse Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendKickPartyMemberRequest(Request.memberID);
}

void UABParty::PartyGenerateCode(
	FDPartyGenerateCodeResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetPartyGenerateCodeResponseDelegate(
		Api::Lobby::FPartyGenerateCodeResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyGenerateCodeResponse Response)
			{

				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendPartyGenerateCodeRequest();
}

void UABParty::PartyGetCode(
	FDPartyGetCodeResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetPartyGetCodeResponseDelegate(
		Api::Lobby::FPartyGetCodeResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyGetCodeResponse Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendPartyGetCodeRequest();
}

void UABParty::PartyDeleteCode(
	FDPartyDeleteCodeResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetPartyDeleteCodeResponseDelegate(
		Api::Lobby::FPartyDeleteCodeResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyDeleteCodeResponse Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendPartyDeleteCodeRequest();
}

void UABParty::PartyJoinViaCode(
	FPartyJoinViaCodeRequest const& Request,
	FDPartyJoinViaCodeResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetPartyJoinViaCodeResponseDelegate(
		Api::Lobby::FPartyJoinViaCodeResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyJoinResponse Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendPartyJoinViaCodeRequest(Request.partyCode);
}

void UABParty::PartyPromoteLeader(
	FPartyPromoteLeaderRequest const& Request,
	FDPartyPromoteLeaderResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetPartyPromoteLeaderResponseDelegate(
		Api::Lobby::FPartyPromoteLeaderResponse::CreateLambda(
			[OnResponse](FAccelByteModelsPartyPromoteLeaderResponse Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
	ApiClientPtr->Lobby.SendPartyPromoteLeaderRequest(Request.userId);
}

void UABParty::GetPartyData(
	FGetPartyDataRequest const& Request,
	FDPartyGetDataResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.GetPartyData(Request.partyId,
		THandler<FAccelByteModelsPartyData>::CreateLambda(
			[OnResponse](FAccelByteModelsPartyData Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABParty::GetPartyStorage(
	FGetPartyStorageRequest const& Request,
	FDPartyGetStorageResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.GetPartyStorage(Request.partyId,
		THandler<FAccelByteModelsPartyDataNotif>::CreateLambda(
			[OnResponse](FAccelByteModelsPartyDataNotif Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABParty::WritePartyStorage(
	FWritePartyStorageRequest const& Request,
	FDPartyWriteDataResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.WritePartyStorage(
		Request.partyId,
		Request.PayloadModifier,
		THandler<FAccelByteModelsPartyDataNotif>::CreateLambda(
			[OnResponse](FAccelByteModelsPartyDataNotif Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));}

void UABParty::SetOnPartyDataUpdate(FDPartyDataUpdateNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyDataUpdateNotifDelegate(
		Api::Lobby::FPartyDataUpdateNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyDataNotif const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyGetInvited(FDPartyGetInvitedNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyGetInvitedNotifDelegate(
		Api::Lobby::FPartyGetInvitedNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyGetInvitedNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyJoin(FDPartyJoinNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyJoinNotifDelegate(
		Api::Lobby::FPartyJoinNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyJoinNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyReject(FDPartyRejectNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyInvitationRejectedNotifDelegate(
		Api::Lobby::FPartyRejectNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyRejectNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyLeave(FDPartyLeaveNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyLeaveNotifDelegate(
		Api::Lobby::FPartyLeaveNotif::CreateLambda(
			[OnNotif](FAccelByteModelsLeavePartyNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyMemberLeave(FDPartyMemberLeaveNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyMemberLeaveNotifDelegate(
		Api::Lobby::FPartyMemberLeaveNotif::CreateLambda(
			[OnNotif](FAccelByteModelsLeavePartyNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyKick(FDPartyKickNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyKickNotifDelegate(
		Api::Lobby::FPartyKickNotif::CreateLambda(
			[OnNotif](FAccelByteModelsGotKickedFromPartyNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyInvite(FDPartyInviteNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyInviteNotifDelegate(
		Api::Lobby::FPartyInviteNotif::CreateLambda(
			[OnNotif](FAccelByteModelsInvitationNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyDataUpdateNotifDelegate(FDPartyDataUpdateNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyDataUpdateNotifDelegate(
		Api::Lobby::FPartyDataUpdateNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyDataNotif const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyMemberConnect(FDPartyMemberConnectNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyMemberConnectNotifDelegate(
		Api::Lobby::FPartyMemberConnectNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyMemberConnectionNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABParty::SetOnPartyMemberDisconnect(FDPartyMemberDisconnectNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetPartyMemberDisconnectNotifDelegate(
		Api::Lobby::FPartyMemberDisconnectNotif::CreateLambda(
			[OnNotif](FAccelByteModelsPartyMemberConnectionNotice const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}
