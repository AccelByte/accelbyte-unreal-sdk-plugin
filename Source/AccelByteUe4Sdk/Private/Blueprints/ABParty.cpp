// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABParty.h"
#include "Api/AccelByteLobbyApi.h"


using namespace AccelByte;

void UABParty::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}
#if 1 // MMv1 Deprecation
void UABParty::PartyInfo(FDInfoPartyResponse OnResponse, FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetInfoPartyResponseDelegate(
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
		LobbyPtr->SendInfoPartyRequest();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::CreateParty(FDPartyCreateResponse OnResponse, FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetCreatePartyResponseDelegate(
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

		LobbyPtr->SendCreatePartyRequest();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyLeave(FDLeavePartyResponse OnResponse, FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetLeavePartyResponseDelegate(
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
		LobbyPtr->SendLeavePartyRequest();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyInvite(
	FPartyInviteRequest const& Request,
	FDPartyInviteResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetInvitePartyResponseDelegate(
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
		LobbyPtr->SendInviteToPartyRequest(Request.friendID);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyJoin(
	FPartyJoinRequest const& Request,
	FDPartyJoinResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetInvitePartyJoinResponseDelegate(
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
		LobbyPtr->SendAcceptInvitationRequest(Request.partyID, Request.invitationToken);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyReject(
	FPartyRejectRequest const& Request,
	FDPartyRejectResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetInvitePartyRejectResponseDelegate(
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
		LobbyPtr->SendRejectInvitationRequest(Request.partyID, Request.invitationToken);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyKick(
	FPartyKickRequest const& Request,
	FDPartyKickResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetInvitePartyKickMemberResponseDelegate(
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
		LobbyPtr->SendKickPartyMemberRequest(Request.memberID);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyGenerateCode(
	FDPartyGenerateCodeResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyGenerateCodeResponseDelegate(
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
		LobbyPtr->SendPartyGenerateCodeRequest();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyGetCode(
	FDPartyGetCodeResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyGetCodeResponseDelegate(
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
		LobbyPtr->SendPartyGetCodeRequest();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyDeleteCode(
	FDPartyDeleteCodeResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyDeleteCodeResponseDelegate(
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
		LobbyPtr->SendPartyDeleteCodeRequest();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyJoinViaCode(
	FPartyJoinViaCodeRequest const& Request,
	FDPartyJoinViaCodeResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyJoinViaCodeResponseDelegate(
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
		LobbyPtr->SendPartyJoinViaCodeRequest(Request.partyCode);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::PartyPromoteLeader(
	FPartyPromoteLeaderRequest const& Request,
	FDPartyPromoteLeaderResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyPromoteLeaderResponseDelegate(
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
		LobbyPtr->SendPartyPromoteLeaderRequest(Request.userId);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::SetOnPartyDataUpdateNotifDelegate(FDPartyDataUpdateNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyDataUpdateNotifDelegate(
			Api::Lobby::FPartyDataUpdateNotif::CreateLambda(
				[OnNotif](FAccelByteModelsPartyDataNotif const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABParty::SetOnPartyDataUpdate(FDPartyDataUpdateNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyDataUpdateNotifDelegate(
			Api::Lobby::FPartyDataUpdateNotif::CreateLambda(
				[OnNotif](FAccelByteModelsPartyDataNotif const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

#endif
void UABParty::SetPartySizeLimit(const FString& PartyId, const int32 Limit, const FDHandler& OnSuccess,
	FDErrorHandler OnError)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartySizeLimit(PartyId, Limit,
		FVoidHandler::CreateLambda([OnSuccess]()
		{
			OnSuccess.ExecuteIfBound();
		}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::GetPartyData(
	FGetPartyDataRequest const& Request,
	FDPartyGetDataResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->GetPartyData(Request.partyId,
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::GetPartyStorage(
	FGetPartyStorageRequest const& Request,
	FDPartyGetStorageResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->GetPartyStorage(Request.partyId,
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::WritePartyStorage(
	FWritePartyStorageRequest const& Request,
	FDPartyWriteDataResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->WritePartyStorage(
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
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
#if 1 // MMv1 Deprecation
void UABParty::SetOnPartyGetInvited(FDPartyGetInvitedNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyGetInvitedNotifDelegate(
			Api::Lobby::FPartyGetInvitedNotif::CreateLambda(
				[OnNotif](FAccelByteModelsPartyGetInvitedNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABParty::SetOnPartyJoin(FDPartyJoinNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyJoinNotifDelegate(
			Api::Lobby::FPartyJoinNotif::CreateLambda(
				[OnNotif](FAccelByteModelsPartyJoinNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABParty::SetOnPartyReject(FDPartyRejectNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyInvitationRejectedNotifDelegate(
			Api::Lobby::FPartyRejectNotif::CreateLambda(
				[OnNotif](FAccelByteModelsPartyRejectNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABParty::SetOnPartyLeave(FDPartyLeaveNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyLeaveNotifDelegate(
			Api::Lobby::FPartyLeaveNotif::CreateLambda(
				[OnNotif](FAccelByteModelsLeavePartyNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABParty::SetOnPartyMemberLeave(FDPartyMemberLeaveNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyMemberLeaveNotifDelegate(
			Api::Lobby::FPartyMemberLeaveNotif::CreateLambda(
				[OnNotif](FAccelByteModelsLeavePartyNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABParty::SetOnPartyKick(FDPartyKickNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyKickNotifDelegate(
			Api::Lobby::FPartyKickNotif::CreateLambda(
				[OnNotif](FAccelByteModelsGotKickedFromPartyNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABParty::SetOnPartyUpdate(FDPartyUpdateNotif OnNotif)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyNotifDelegate(
			Api::Lobby::FPartyNotif::CreateLambda([OnNotif](const FAccelByteModelsPartyNotif& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABParty::SetOnPartyInvite(FDPartyInviteNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyInviteNotifDelegate(
			Api::Lobby::FPartyInviteNotif::CreateLambda(
				[OnNotif](FAccelByteModelsInvitationNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}


void UABParty::SetOnPartyMemberConnect(FDPartyMemberConnectNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyMemberConnectNotifDelegate(
			Api::Lobby::FPartyMemberConnectNotif::CreateLambda(
				[OnNotif](FAccelByteModelsPartyMemberConnectionNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABParty::SetOnPartyMemberDisconnect(FDPartyMemberDisconnectNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartyMemberDisconnectNotifDelegate(
			Api::Lobby::FPartyMemberDisconnectNotif::CreateLambda(
				[OnNotif](FAccelByteModelsPartyMemberConnectionNotice const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}
#endif