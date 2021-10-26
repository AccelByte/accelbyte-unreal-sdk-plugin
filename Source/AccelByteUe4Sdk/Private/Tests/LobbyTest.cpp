// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "JsonObjectWrapper.h"
#include "Api/AccelByteOauth2Api.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteEnvironment.h"
#include "Api/AccelByteLobbyApi.h"
#include "Api/AccelByteQos.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"
#include "LobbyTestAdmin.h"
#include "MatchmakingTestAdmin.h"


#include <IPAddress.h>
#include <SocketSubsystem.h>
#include <IWebSocket.h>

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::Oauth2;
using AccelByte::Api::Lobby;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLobbyTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteLobbyTest);

const int32 AutomationFlagMaskLobby = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto LobbyTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteLobbyTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const int TestUserCount = 6;
FString UserIds[TestUserCount];
Credentials UserCreds[TestUserCount];
TArray<TSharedPtr<Api::User>> LobbyUsers;
TArray<TSharedPtr<Api::Lobby>> Lobbies;
TArray<TPair<FString, float>> PreferedLatencies;
//General
bool bUsersConnected, bUsersConnectionSuccess, bGetMessage, bGetAllUserPresenceSuccess, bGetFriendsPresenceSuccess;
//Friends
bool bRequestFriendError, bAcceptFriendSuccess, bAcceptFriendError, bRequestFriendSuccess, bRejectFriendSuccess, bRejectFriendError, bCancelFriendSuccess, bCancelFriendError;
bool bGetFriendshipStatusError, bListOutgoingFriendSuccess, bListOutgoingFriendError, bListIncomingFriendSuccess, bListIncomingFriendError;
bool bLoadFriendListSuccess, bLoadFriendListError, bOnIncomingRequestNotifSuccess, bOnIncomingRequestNotifError, bOnRequestAcceptedNotifSuccess, bOnRequestAcceptedNotifError;
bool bUnfriendNotifSuccess, bCancelFriendNotifSuccess, bRejectFriendNotifSuccess;
//Party
bool bCreatePartySuccess, bCreatePartyError, bInvitePartySuccess, bRejectPartySuccess, bRejectPartyError, bRejectedPartyNotifSuccess, bGetInvitedNotifSuccess, bGetInvitedNotifError, bGetInvitationRejectedNotifSuccess, bGetInvitationRejectedNotifError;
bool bJoinPartySuccess, bJoinPartyError, bLeavePartySuccess, bLeavePartyError, bGetInfoPartySuccess, bGetInfoPartyError, bGetPartyDataUpdateNotifSuccess;
bool bKickPartyMemberSuccess, bKickPartyMemberError, bKickedFromPartySuccess, bReceivedPartyChatSuccess, bSendPartyChatSuccess, bSendPartyChatError;
bool bGeneratePartyCodeSuccess, bGeneratePartyCodeError, bGetPartyCodeSuccess, bGetPartyCodeError, bDeletePartyCodeSuccess, bDeletePartyCodeError, bJoinPartyViaCodeSuccess, bJoinPartyViaCodeError;
bool bPromotePartyLeaderSuccess, bPromotePartyLeaderError;
//Matchmaking
bool bStartMatchmakingSuccess, bStartMatchmakingError, bCancelMatchmakingSuccess, bCancelMatchmakingError;
bool bReadyConsentResponseSuccess, bReadyConsentResponseError, bReadyConsentNotifSuccess, bReadyConsentNotifError;
bool bDsNotifSuccess, bDsNotifError;
//Presence
bool bUserPresenceError, bUserPresenceNotifSuccess, bUserPresenceNotifError, bUnfriendSuccess, bUnfriendError, bGetFriendshipStatusSuccess, bGetNotifSuccess, bUserPresenceSuccess;
// Block 
bool bBlockPlayerSuccess, bBlockPlayerError, bUnblockPlayerSuccess, bUnblockPlayerError;
bool bListBlockedUserListSuccess,  bListBlockerUserListError, bListBlockerListSuccess, bListBlockerListError; 
// Block Notif
bool bBlockPlayerNotifSuccess, bUnblockPlayerNotifSuccess, bBlockPlayerNotifError, bUnblockPlayerNotifError;
// Ban
bool bUserBannedNotif, bUsersUnbannedNotif;

FAccelByteModelsPartyGetInvitedNotice invitedToPartyResponse;
FAccelByteModelsInfoPartyResponse infoPartyResponse;
FAccelByteModelsPartyJoinReponse joinPartyResponse;
FAccelByteModelsPartyGenerateCodeResponse partyGenerateCodeResponse;
FAccelByteModelsPartyGetCodeResponse partyCodeResponse;
FAccelByteModelsPartyRejectResponse rejectPartyResponse;
FAccelByteModelsPartyDataNotif partyDataNotif;

FAccelByteModelsGetOnlineUsersResponse onlineUserResponse;
FAccelByteModelsGetOnlineUsersResponse onlineFriendResponse;
FAccelByteModelsNotificationMessage getNotifResponse;
FAccelByteModelsUsersPresenceNotice userPresenceNotifResponse;

FAccelByteModelsGetFriendshipStatusResponse getFriendshipStatusResponse;
FAccelByteModelsListOutgoingFriendsResponse listOutgoingFriendResponse;
FAccelByteModelsListIncomingFriendsResponse listIncomingFriendResponse;
FAccelByteModelsLoadFriendListResponse loadFriendListResponse;
FAccelByteModelsRequestFriendsNotif requestFriendNotifResponse;
FAccelByteModelsAcceptFriendsNotif acceptFriendNotifResponse;
FAccelByteModelsUnfriendNotif UnfriendNotifResponse;
FAccelByteModelsCancelFriendsNotif CancelFriendNotifResponse;
FAccelByteModelsRejectFriendsNotif RejectFriendNotifResponse;

FAccelByteModelsBlockPlayerNotif blockPlayerNotifResponse;
FAccelByteModelsUnblockPlayerNotif unblockPlayerNotifResponse;
FAccelByteModelsBlockPlayerResponse blockPlayerResponse;
FAccelByteModelsUnblockPlayerResponse unblockPlayerResponse;
FAccelByteModelsListBlockedUserResponse listBlockedUserResponse;
FAccelByteModelsListBlockerResponse listBlockerResponse;

FAccelByteModelsUserBannedNotification userBanNotifResponse;

FAccelByteModelsMatchmakingResponse matchmakingResponse;
FAccelByteModelsReadyConsentNotice readyConsentNotice;
FAccelByteModelsDsNotice dsNotice;

inline static bool LatenciesPredicate(const TPair<FString, float>& left, const TPair<FString, float>& right)
{
	return left.Value < right.Value;
}

void LobbyConnect(int userCount)
{
	if (userCount > TestUserCount)
	{
		userCount = TestUserCount;
	}
	if (userCount <= 0)
	{
		userCount = 1;
	}
	for (int i = 0; i < userCount; i++)
	{
		if (!Lobbies[i]->IsConnected())
		{
			Lobbies[i]->Connect();
		}
		FString text = FString::Printf(TEXT("LobbyConnect: Wait user %d"), i);
		while (!Lobbies[i]->IsConnected())
		{
			FPlatformProcess::Sleep(.5f);
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("%s"), *text);
			FTicker::GetCoreTicker().Tick(.5f);
		}
	}
}

void LobbyDisconnect(int userCount)
{
	if (userCount > TestUserCount)
	{
		userCount = TestUserCount;
	}
	if (userCount <= 0)
	{
		userCount = 1;
	}
	for (int i = 0; i < userCount; i++)
	{
		Lobbies[i]->UnbindEvent();
		Lobbies[i]->Disconnect();
		while (Lobbies[i]->IsConnected())
		{
			FPlatformProcess::Sleep(.5f);
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("disconecting lobby %d"), i);
			FTicker::GetCoreTicker().Tick(.5f);
		}
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("lobby %d disconnected"), i);
	}
}

void ResetResponses()
{
	bUsersConnected = false;
	bUsersConnectionSuccess = false;
	bGetMessage = false;
	bGetAllUserPresenceSuccess = false;
	bGetFriendsPresenceSuccess = false;
	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	bAcceptFriendSuccess = false;
	bAcceptFriendError = false;

	// Block
	bBlockPlayerSuccess = false;
	bBlockPlayerError = false;
	bUnblockPlayerSuccess = false;
	bUnblockPlayerError = false;
	bListBlockedUserListSuccess = false;
	bListBlockerUserListError = false;
	bListBlockerListSuccess = false;
	bListBlockerListError = false;
	// Block Notifications
	bBlockPlayerNotifSuccess = false;
	bBlockPlayerNotifError = false;
	bUnblockPlayerNotifSuccess = false;
	bUnblockPlayerNotifError = false;

	bCreatePartySuccess = false;
	bCreatePartyError = false;
	bInvitePartySuccess = false;
	bGetInvitedNotifSuccess = false;
	bGetInvitedNotifError = false;
	bJoinPartySuccess = false;
	bJoinPartyError = false;
	bRejectPartySuccess = false;
	bRejectPartyError = false;
	bRejectedPartyNotifSuccess = false;
	bLeavePartySuccess = false;
	bLeavePartyError = false;
	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	bKickPartyMemberSuccess = false;
	bKickPartyMemberError = false;
	bPromotePartyLeaderSuccess = false;
	bPromotePartyLeaderError = false;
	bKickedFromPartySuccess = false;
	bReceivedPartyChatSuccess = false;
	bGetPartyCodeSuccess = false;
	bGetPartyCodeError = false;
	bDeletePartyCodeSuccess = false;
	bDeletePartyCodeError = false;
	bJoinPartyViaCodeSuccess = false;
	bJoinPartyViaCodeError = false;
	bSendPartyChatSuccess = false;
	bSendPartyChatError = false;
	bGetNotifSuccess = false;
	bUserPresenceSuccess = false;
	bUserPresenceError = false;
	bUserPresenceNotifSuccess = false;
	bUserPresenceNotifError = false;
	bUnfriendSuccess = false;
	bUnfriendError = false;
	bUnfriendNotifSuccess = false;
	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	bListOutgoingFriendSuccess = false;
	bListOutgoingFriendError = false;
	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	bOnIncomingRequestNotifSuccess = false;
	bOnIncomingRequestNotifError = false;
	bOnRequestAcceptedNotifSuccess = false;
	bOnRequestAcceptedNotifError = false;
	bRejectFriendSuccess = false;
	bRejectFriendError = false;
	bRejectFriendNotifSuccess = false;
	bCancelFriendSuccess = false;
	bCancelFriendError = false;
	bCancelFriendNotifSuccess = false;
	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	bCancelMatchmakingSuccess = false;
	bCancelMatchmakingError = false;
	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	bReadyConsentResponseSuccess = false;
	bReadyConsentResponseError = false;
	bDsNotifSuccess = false;
	bDsNotifError = false;
	bGetPartyDataUpdateNotifSuccess = false;
}

#pragma region GeneralDelegate
const auto ConnectSuccessDelegate = Api::Lobby::FConnectSuccess::CreateLambda([]()
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User connected!"));
	bUsersConnected = true;
	bUsersConnectionSuccess = true;
});

const auto ConnectFailedDelegate = FErrorHandler::CreateLambda([](int32 Code, FString Message)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User failed to connect!"));
	bUsersConnected = false;
	bUsersConnectionSuccess = true;
});

const auto GetMessageDelegate = Api::Lobby::FPersonalChatNotif::CreateLambda([](FAccelByteModelsPersonalMessageNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Payload : %s"), *result.Payload);
	bGetMessage = true;
});

const auto GetNotifDelegate = Api::Lobby::FMessageNotif::CreateLambda([](FAccelByteModelsNotificationMessage result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Notification!"));
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("From : %s | Message : %s"), *result.From, *result.Payload);
	getNotifResponse = result;
	bGetNotifSuccess = true;
});
#pragma endregion

#pragma region FriendsDelegate
const auto RequestFriendDelegate = Api::Lobby::FRequestFriendsResponse::CreateLambda([](FAccelByteModelsRequestFriendsResponse result)
{
	bRequestFriendSuccess = true;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Request Friend Success!"));
	if (result.Code != "0")
	{
		bRequestFriendError = true;
	}
});

const auto AcceptFriendsDelegate = Api::Lobby::FAcceptFriendsResponse::CreateLambda([](FAccelByteModelsAcceptFriendsResponse result)
{
	bAcceptFriendSuccess = true;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Accept Friend Success!"));
	if (result.Code != "0")
	{
		bAcceptFriendError = true;
	}
});

const auto UnfriendDelegate = Api::Lobby::FUnfriendResponse::CreateLambda([](FAccelByteModelsUnfriendResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Unfriend Success!"));
	bUnfriendSuccess = true;
	if (result.Code != "0")
	{
		bUnfriendError = true;
	}
});

const auto GetFriendshipStatusDelegate = Api::Lobby::FGetFriendshipStatusResponse::CreateLambda([](FAccelByteModelsGetFriendshipStatusResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Friendship Status!"));
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Code: %s, Status: %d"), *result.Code, (int)result.friendshipStatus);
	getFriendshipStatusResponse = result;
	bGetFriendshipStatusSuccess = true;
	if (result.Code != "0")
	{
		bGetFriendshipStatusError = true;
	}
});

const auto ListOutgoingFriendDelegate = Api::Lobby::FListOutgoingFriendsResponse::CreateLambda([](FAccelByteModelsListOutgoingFriendsResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get List Outgoing Friend Request!"));
	listOutgoingFriendResponse = result;
	bListOutgoingFriendSuccess = true;
	if (result.Code != "0")
	{
		bListOutgoingFriendError = true;
	}
});

const auto ListIncomingFriendDelegate = Api::Lobby::FListIncomingFriendsResponse::CreateLambda([](FAccelByteModelsListIncomingFriendsResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get List Incoming Friend Request!"));
	listIncomingFriendResponse = result;
	bListIncomingFriendSuccess = true;
	if (result.Code != "0")
	{
		bListIncomingFriendError = true;
	}
});

const auto LoadFriendListDelegate = Api::Lobby::FLoadFriendListResponse::CreateLambda([](FAccelByteModelsLoadFriendListResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Load Friend List!"));
	loadFriendListResponse = result;
	bLoadFriendListSuccess = true;
	if (result.Code != "0")
	{
		bLoadFriendListError = true;
	}
});

const auto OnIncomingRequestNotifDelegate = Api::Lobby::FRequestFriendsNotif::CreateLambda([](FAccelByteModelsRequestFriendsNotif result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Friend Request!"));
	requestFriendNotifResponse = result;
	bOnIncomingRequestNotifSuccess = true;
	if (result.friendId.IsEmpty())
	{
		bOnIncomingRequestNotifError = true;
	}
});

const auto OnRequestAcceptedNotifDelegate = Api::Lobby::FAcceptFriendsNotif::CreateLambda([](FAccelByteModelsAcceptFriendsNotif result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Friend Request Accepted!"));
	acceptFriendNotifResponse = result;
	bOnRequestAcceptedNotifSuccess = true;
	if (result.friendId.IsEmpty())
	{
		bOnRequestAcceptedNotifError = true;
	}
});

const auto RejectFriendDelegate = Api::Lobby::FRejectFriendsResponse::CreateLambda([](FAccelByteModelsRejectFriendsResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Reject Friend Success!"));
	bRejectFriendSuccess = true;
	if (result.Code != "0")
	{
		bRejectFriendError = true;
	}
});

const auto CancelFriendDelegate = Api::Lobby::FCancelFriendsResponse::CreateLambda([](FAccelByteModelsCancelFriendsResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Cancel Friend Success!"));
	bCancelFriendSuccess = true;
	if (result.Code != "0")
	{
		bCancelFriendError = true;
	}
});

const auto UnfriendNotifDelegate = Api::Lobby::FUnfriendNotif::CreateLambda([](FAccelByteModelsUnfriendNotif result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Unfriend Notif!"));
	UnfriendNotifResponse = result;
	bUnfriendNotifSuccess = true;
});

const auto CancelFriendNotifDelegate = Api::Lobby::FCancelFriendsNotif::CreateLambda([](FAccelByteModelsCancelFriendsNotif result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Cancel Friend Notif!"));
	CancelFriendNotifResponse = result;
	bCancelFriendNotifSuccess = true;
});

const auto RejectFriendNotifDelegate = Api::Lobby::FRejectFriendsNotif::CreateLambda([](FAccelByteModelsRejectFriendsNotif result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Reject Friend Notif!"));
	RejectFriendNotifResponse = result;
	bRejectFriendNotifSuccess = true;
});
#pragma endregion

#pragma region PartyDelegate
const auto GetInfoPartyDelegate = Api::Lobby::FPartyInfoResponse::CreateLambda([](FAccelByteModelsInfoPartyResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Info Party Success!"));
	bGetInfoPartySuccess = true;
	if (!result.PartyId.IsEmpty())
	{
		infoPartyResponse = result;
	}
	else
	{
		bGetInfoPartyError = true;
	}
});

const auto LeavePartyDelegate = Api::Lobby::FPartyLeaveResponse::CreateLambda([](FAccelByteModelsLeavePartyResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Leave Party Success!"));
	bLeavePartySuccess = true;
	if (result.Code != "0")
	{
		bLeavePartyError = true;
	}
});

const auto CreatePartyDelegate = Api::Lobby::FPartyCreateResponse::CreateLambda([](FAccelByteModelsCreatePartyResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Party Success!"));
	bCreatePartySuccess = true;
	if (result.PartyId.IsEmpty())
	{
		bCreatePartyError = true;
	}
});

const auto GeneratePartyCodeDelegate = Api::Lobby::FPartyGenerateCodeResponse::CreateLambda([](FAccelByteModelsPartyGenerateCodeResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Party Code Success!"));
	bGeneratePartyCodeSuccess = true;
	partyGenerateCodeResponse = result;
	if (result.PartyCode.IsEmpty())
	{
		bGeneratePartyCodeError = true;
	}
});

const auto GetPartyCodeDelegate = Api::Lobby::FPartyGetCodeResponse::CreateLambda([](FAccelByteModelsPartyGetCodeResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Party Code Success!"));
	bGetPartyCodeSuccess = true;
	partyCodeResponse = result;
	if (result.PartyCode.IsEmpty())
	{
		bGetPartyCodeError = true;
	}
});

const auto DeletePartyCodeDelegate = Api::Lobby::FPartyDeleteCodeResponse::CreateLambda([](FAccelByteModelsPartyDeleteCodeResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Party Code Success!"));
	bDeletePartyCodeSuccess = true;
	if (result.Code != "0")
	{
		bDeletePartyCodeError = true;
	}
});


const auto JoinViaCodeDelegate = Api::Lobby::FPartyJoinResponse::CreateLambda([](FAccelByteModelsPartyJoinReponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Join Party Via Code Success! Member : %d"), result.Members.Num());
	joinPartyResponse = result;
	bJoinPartyViaCodeSuccess = true;
	if (result.Code != "0")
	{
		bJoinPartyViaCodeError = true;
	}
});

const auto InvitePartyDelegate = Api::Lobby::FPartyInviteResponse::CreateLambda([](FAccelByteModelsPartyInviteResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Invite Party Success!"));
	bInvitePartySuccess = true;
});

const auto InvitedToPartyDelegate = Api::Lobby::FPartyGetInvitedNotif::CreateLambda([](FAccelByteModelsPartyGetInvitedNotice result)
{
	invitedToPartyResponse = result;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Party Invitation!"));
	bGetInvitedNotifSuccess = true;
	if (result.PartyId.IsEmpty())
	{
		bGetInvitedNotifError = true;
	}
});

const auto JoinPartyDelegate = Api::Lobby::FPartyJoinResponse::CreateLambda([](FAccelByteModelsPartyJoinReponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Join Party Success! Member : %d"), result.Members.Num());
	joinPartyResponse = result;
	bJoinPartySuccess = true;
	if (result.Code != "0")
	{
		bJoinPartyError = false;
	}
});

const auto PartyDataUpdateDelegate = Api::Lobby::FPartyDataUpdateNotif::CreateLambda([](FAccelByteModelsPartyDataNotif result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get a Party data update notif!"));	
	bGetPartyDataUpdateNotifSuccess = true;
	partyDataNotif = result;
});

const auto PartyChatNotifDelegate = Api::Lobby::FPartyChatNotif::CreateLambda([](FAccelByteModelsPartyMessageNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get a Party Message!"));
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("From : %s | Message : %s | At : %s"), *result.From, *result.Payload, *result.ReceivedAt);
	bReceivedPartyChatSuccess = true;
});

const auto PartyChatSendDelegate = Api::Lobby::FPartyChatResponse::CreateLambda([](FAccelByteModelsPartyMessageResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Send Party Chat Success!"));
	bSendPartyChatSuccess = true;
	if (result.Code != "0")
	{
		bSendPartyChatError = true;
	}
});

const auto KickPartyMemberDelegate = Api::Lobby::FPartyKickResponse::CreateLambda([](FAccelByteModelsKickPartyMemberResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Kick Party Member Success!"));
	bKickPartyMemberSuccess = true;
	if (result.Code != "0")
	{
		bKickPartyMemberError = true;
	}
});

const auto KickedFromPartyDelegate = Api::Lobby::FPartyKickNotif::CreateLambda([](FAccelByteModelsGotKickedFromPartyNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Kicked From Party!"));
	{
		bKickedFromPartySuccess = true;
	}
});
const auto RejectPartyNotifDelegate = Api::Lobby::FPartyRejectNotif::CreateLambda([](FAccelByteModelsPartyRejectNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Reject Party Success! Reject UserId : %s, PartyLeader : %s, PartyId : %s"), *result.UserId, *result.LeaderId, *result.PartyId);
	bRejectedPartyNotifSuccess = true;
});
const auto RejectPartyDelegate = Api::Lobby::FPartyRejectResponse::CreateLambda([](FAccelByteModelsPartyRejectResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Reject Party Success!"));
	rejectPartyResponse = result;
	bRejectPartySuccess = true;
});


const auto PromotePartyLeaderDelegate = Api::Lobby::FPartyPromoteLeaderResponse::CreateLambda([](FAccelByteModelsPartyPromoteLeaderResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Promote Party Leader Success!"));
	bPromotePartyLeaderSuccess = true;
	if (result.Code != "0")
	{
		bPromotePartyLeaderError = true;
	}
});
#pragma endregion

#pragma region BlocksDelegate
const auto BlockPlayerDelegate = Api::Lobby::FBlockPlayerResponse::CreateLambda([](FAccelByteModelsBlockPlayerResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Block Player Success!"));
	bBlockPlayerSuccess = true;
	if (result.Code != "0")
	{
		bBlockPlayerError = true;
	}
});

const auto UnblockPlayerDelegate = Api::Lobby::FUnblockPlayerResponse::CreateLambda([](FAccelByteModelsUnblockPlayerResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Unblock Player Success!"));
	bUnblockPlayerSuccess = true;
	if (result.Code != "0")
	{
		bUnblockPlayerError = true;
	}
});

const auto ListBlockedUserDelegate = Api::Lobby::FListBlockedUserResponse::CreateLambda([](FAccelByteModelsListBlockedUserResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("List Blocked User Success!"));
	bListBlockedUserListSuccess = true;
	listBlockedUserResponse = result;
});

const auto ListBlockerDelegate = Api::Lobby::FListBlockerResponse::CreateLambda([](FAccelByteModelsListBlockerResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("List Blocker User Success!"));
	bListBlockerListSuccess = true;
	listBlockerResponse = result;
});

const auto BlockPlayerNotifDelegate = Api::Lobby::FBlockPlayerNotif::CreateLambda([](FAccelByteModelsBlockPlayerNotif result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Block Player Notif Success!"));
		bBlockPlayerNotifSuccess = true;
		if (result.BlockedUserId.IsEmpty())
		{
			bBlockPlayerNotifError = true;
		}
	});

const auto UnblockPlayerNotifDelegate = Api::Lobby::FUnblockPlayerNotif::CreateLambda([](FAccelByteModelsUnblockPlayerNotif result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Unblock Player Notif Success!"));
	bUnblockPlayerNotifSuccess = true;
	if (result.UnblockedUserId.IsEmpty())
	{
		bUnblockPlayerNotifError = true;
	}
});
#pragma endregion

#pragma region MatchmakingDegelate
const auto StartMatchmakingDelegate = Api::Lobby::FMatchmakingResponse::CreateLambda([](FAccelByteModelsMatchmakingResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Start Matchmaking Success!"));
	matchmakingResponse = result;
	bStartMatchmakingSuccess = true;
	if (result.Code != "0")
	{
		bStartMatchmakingError = true;
	}
});

const auto CancelMatchmakingDelegate = Api::Lobby::FMatchmakingResponse::CreateLambda([](FAccelByteModelsMatchmakingResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Cancel Matchmaking Success!"));
	matchmakingResponse = result;
	bCancelMatchmakingSuccess = true;
	if (result.Code != "0")
	{
		bCancelMatchmakingError = true;
	}
});

const auto ReadyConsentResponseDelegate = Api::Lobby::FReadyConsentResponse::CreateLambda([](FAccelByteModelsReadyConsentRequest result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Ready Consent Send!"));
	bReadyConsentResponseSuccess = true;
});

const auto ReadyConsentNotifDelegate = Api::Lobby::FReadyConsentNotif::CreateLambda([](FAccelByteModelsReadyConsentNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Ready Consent Notice!"));
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User %s is ready for match."), *result.UserId);
	readyConsentNotice = result;
	bReadyConsentNotifSuccess = true;
	if (result.MatchId.IsEmpty())
	{
		bReadyConsentNotifError = true;
	}
});

const auto DsNotifDelegate = Api::Lobby::FDsNotif::CreateLambda([](FAccelByteModelsDsNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get DS Notice!"));
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DS ID: %s | Message: %s | Status: %s"), *result.MatchId, *result.Message, *result.Status);
	dsNotice = result;
	bDsNotifError = !dsNotice.IsOK.ToBool();
	if (dsNotice.Status == "READY" || dsNotice.Status == "BUSY" || bDsNotifError)
	{
		bDsNotifSuccess = true;
	}
});
#pragma endregion

#pragma region PresenceDelegate
const auto GetAllUsersPresenceDelegate = Api::Lobby::FGetAllFriendsStatusResponse::CreateLambda([](FAccelByteModelsGetOnlineUsersResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("GetAllUserPresence Success!"));
	bGetAllUserPresenceSuccess = true;
	onlineUserResponse = result;
});

const auto GetFriendsPresenceDelegate = Api::Lobby::FGetAllFriendsStatusResponse::CreateLambda([](FAccelByteModelsGetOnlineUsersResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("GetFriendsPresence Success!"));
	bGetFriendsPresenceSuccess = true;
	onlineFriendResponse = result;
});

const auto UserPresenceDelegate = Api::Lobby::FSetUserPresenceResponse::CreateLambda([](FAccelByteModelsSetOnlineUsersResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Presence Changed!"));
	bUserPresenceSuccess = true;
	if (result.Code != "0")
	{
		bUserPresenceError = true;
	}
});

const auto UserPresenceNotifDelegate = Api::Lobby::FFriendStatusNotif::CreateLambda([](FAccelByteModelsUsersPresenceNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Changed Their Presence!"));
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("ID: %s | Status: %s | Activity: %s"), *result.UserID, *result.Availability, *result.Activity);
	userPresenceNotifResponse = result;
	bUserPresenceNotifSuccess = true;
	if (result.UserID.IsEmpty())
	{
		bUserPresenceNotifError = true;
	}
});
#pragma endregion

#pragma region BanDelegate
const auto UserBannedNotifDelegate = Api::Lobby::FUserBannedNotification::CreateLambda([](FAccelByteModelsUserBannedNotification Result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned"));
		userBanNotifResponse = Result;
		bUserBannedNotif = true;
	});

const auto UserUnbannedNotifDelegate = Api::Lobby::FUserUnbannedNotification::CreateLambda([](FAccelByteModelsUserBannedNotification Result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User UnBanned"));
		userBanNotifResponse = Result;
		bUsersUnbannedNotif = true;
	});
#pragma endregion

TSharedRef<TMap<FString, int32>> GetNewCustomPorts()
{
	TMap<FString, int32> customPorts;
	customPorts.Add(TEXT("custom"), 1001);
	customPorts.Add(TEXT("custom2"), 1002);

	auto sharedResult = MakeShared<TMap<FString, int32>>(customPorts);
	return sharedResult;
}

TSharedRef<FDsmConfig> GetNewDsmConfig()
{
	TMap<FString, FString> versionMapping;
	versionMapping.Add("defult", "no_image");

	FDsmConfig dsmConfig;
	dsmConfig.Namespace = FRegistry::Settings.Namespace;
	dsmConfig.Providers = { "aws" };
	dsmConfig.Port = 1000;
	dsmConfig.Creation_timeout = 60;
	dsmConfig.Claim_timeout = 120;
	dsmConfig.Session_timeout = 1800;
	dsmConfig.Heartbeat_timeout = 30;
	dsmConfig.Unreachable_timeout = 3600;
	dsmConfig.Image_version_mapping = versionMapping;
	dsmConfig.Default_version = "default";
	dsmConfig.Cpu_limit = 200;
	dsmConfig.Mem_limit = 256;
	dsmConfig.Min_count = 0;
	dsmConfig.Max_count = 3;
	dsmConfig.Buffer_count = 0;
	dsmConfig.Allow_version_override = false;
	dsmConfig.Ports = GetNewCustomPorts().Get();
	dsmConfig.Protocol = "udp";

	TSharedRef<FDsmConfig> sharedResult = MakeShared<FDsmConfig>(dsmConfig);
	return sharedResult;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSetup, "AccelByte.Tests.Lobby.A.Setup", AutomationFlagMaskLobby);
bool LobbyTestSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess[TestUserCount];
	bool UsersLoginSuccess[TestUserCount];

	for (int i = 0; i < TestUserCount; i++)
	{
		UsersCreationSuccess[i] = false;
		UsersLoginSuccess[i] = false;
		bClientLoginSuccess = false;

		LobbyUsers.Add(MakeShared<Api::User>(UserCreds[i], FRegistry::Settings, FRegistry::HttpRetryScheduler));

		FString Email = FString::Printf(TEXT("lobbyUE4Test+%d-%d@example.com"), i, FMath::RandRange(0, 100000000));
		Email.ToLowerInline();
		FString Password = TEXT("123Password123");
		FString DisplayName = FString::Printf(TEXT("lobbyUE4%d"), i);
		FString Country = "US";
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
		const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

		LobbyUsers[i]->Register(
			Email, 
			Password, 
			DisplayName, 
			Country, 
			format, 
			THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
			{
				UsersCreationSuccess[i] = true;
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Test Lobby User %d/%d is Created"), i, TestUserCount);

			}), 
			FErrorHandler::CreateLambda([&](int32 Code, FString Message)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Code=%d"), Code);
				if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException || (ErrorCodes)Code == ErrorCodes::UserDisplayNameAlreadyUsedException) //email already used
				{
					UsersCreationSuccess[i] = true;
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Test Lobby User %d/%d is already"), i, TestUserCount);
				}
				else
				{
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Test Lobby User %d/%d can't be created"), i, TestUserCount);
				}
			}));
		
		WaitUntil(UsersCreationSuccess[i],"Waiting for user created..."); 

		LobbyUsers[i]->LoginWithUsername(
			Email,
			Password,
			FVoidHandler::CreateLambda([&]()
			{
				UsersLoginSuccess[i] = true;
				UserIds[i] = UserCreds[i].GetUserId();
			}), 
			LobbyTestErrorHandler);
		
		WaitUntil(UsersLoginSuccess[i],"Waiting for Login...");

		Lobbies.Add(MakeShared<Api::Lobby>(UserCreds[i], FRegistry::Settings, FRegistry::HttpRetryScheduler));
	}
	
	for (int i = 0; i < TestUserCount; i++)
	{
		AB_TEST_TRUE(UsersLoginSuccess[i]);
	}

	FString PreferedDSRegion = Environment::GetEnvironmentVariable("PREFERED_DS_REGION", 1000);

	if (!PreferedDSRegion.IsEmpty()) 
	{
		bool bGetServerLatenciesSuccess = false;
		FRegistry::Qos.GetServerLatencies(THandler<TArray<TPair<FString, float>>>::CreateLambda([&bGetServerLatenciesSuccess](const TArray<TPair<FString, float>>& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Server Latencies Success! Count: %d"), Result.Num());
			bGetServerLatenciesSuccess = true;
			PreferedLatencies = Result;
		}), LobbyTestErrorHandler);
		WaitUntil([&]()
		{
			return bGetServerLatenciesSuccess;
		}, "Waiting for get server latencies...", 60);
		AB_TEST_TRUE(bGetServerLatenciesSuccess);

		for (int i = 0; i < PreferedLatencies.Num(); i++)
		{
			if (PreferedLatencies[i].Key == PreferedDSRegion)
			{
				PreferedLatencies[i].Value = 10;
				PreferedLatencies = { PreferedLatencies[i] };
				break;
			}
		}
	}

	// Setup dsm config
	bool isUpdateDsmConfig = false;
	bool bGetDsmConfigComplete = false;
	
	FDsmConfig dsmConfig;
	AdminGetLobbyDSMConfig(THandler<FDsmConfig>::CreateLambda([&dsmConfig, &bGetDsmConfigComplete, &isUpdateDsmConfig](const FDsmConfig& result)
	{
		dsmConfig = result;

		auto customPorts = GetNewCustomPorts();
		for (auto port : customPorts.Get())
		{
			if (!dsmConfig.Ports.Contains(port.Key))
			{
				dsmConfig.Ports.Add(port.Key, port.Value);
				isUpdateDsmConfig = true;
			}
		}

		bGetDsmConfigComplete = true;
	}), FErrorHandler::CreateLambda([&dsmConfig, &isUpdateDsmConfig, &bGetDsmConfigComplete](int32 ErrorCode, FString ErrorMessage)
	{
		if (ErrorCode == (int)ErrorCodes::DedicatedServerConfigNotFoundException)
		{
			dsmConfig = GetNewDsmConfig().Get();
			isUpdateDsmConfig = true;
		}
		else
		{
			LobbyTestErrorHandler.Execute(ErrorCode, ErrorMessage);
		}
		bGetDsmConfigComplete = true;
	}));
	WaitUntil(bGetDsmConfigComplete, "Waiting get dsm config");

	if (isUpdateDsmConfig)
	{
		bool bSetDsmConfigComplete = false;
		AdminSetLobbyDSMConfig(dsmConfig, FVoidHandler::CreateLambda([&bSetDsmConfigComplete]() {
			bSetDsmConfigComplete = true;
		}), LobbyTestErrorHandler);
		WaitUntil(bSetDsmConfigComplete, "Waiting set dsm config");
	}
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestTeardown, "AccelByte.Tests.Lobby.Z.Teardown", AutomationFlagMaskLobby);

bool LobbyTestTeardown::RunTest(const FString& Parameters)
{
	TArray<bool> SuccessfulDeletions;
	SuccessfulDeletions.Init(false, TestUserCount);

	for (int i = 0; i < TestUserCount; i++)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DeleteUserById (%d/%d)"), i + 1, TestUserCount);
		AdminDeleteUser(
			UserCreds[i].GetUserId(),
			FSimpleDelegate::CreateLambda([&SuccessfulDeletions, i]()
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Success"));
				SuccessfulDeletions[i] = true;
			}),
			FErrorHandler::CreateLambda([&SuccessfulDeletions, i](int32 ErrorCode, FString ErrorMessage)
			{
				//Not found means we don't need to delete it
				if (ErrorCode == static_cast<int32>(ErrorCodes::StatusNotFound))
				{
					SuccessfulDeletions[i] = true;
				}
			}));
		WaitUntil([&]() { return SuccessfulDeletions[i]; }, "Waiting for user deletion...", 60.0);
	}

	AB_TEST_FALSE(SuccessfulDeletions.ContainsByPredicate([](const bool SuccessfulDeletion){ return !SuccessfulDeletion; }));
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestMessage, "AccelByte.Tests.Lobby.B.LobbyMessage", AutomationFlagMaskLobby);
bool LobbyTestMessage::RunTest(const FString& Parameters)
{
	struct LobbyMessageTest
	{
		FString Message;
		FString ExpectedJSON;
		LobbyMessageTest(const FString& message, const FString& expectedJSON)
			: Message(message)
			, ExpectedJSON(expectedJSON){}
	};

	const LobbyMessageTest LobbyMessageArray("type: array\narray: [test, foo, bar]", R"({"type":"array","array":["test","foo","bar"]})");
	const LobbyMessageTest LobbyMessageArrayWithTrailingComma("type: array\narray: [test, foo, bar,]", R"({"type":"array","array":["test","foo","bar"]})");
	const LobbyMessageTest LobbyMessageArrayQuoteComma("type: array\narray: [\"test,string\", foo, bar,]", R"({"type":"array","array":["test,string","foo","bar"]})");
	const LobbyMessageTest LobbyMessageArrayQuoteEscape("type: array\narray: [\"test\\\"string\", foo, bar,]", R"({"type":"array","array":["test\"string","foo","bar"]})");
	const LobbyMessageTest LobbyMessageObject("type: object\nexample: {\"key\":\"value\"}", R"({"type":"object","example":{"key":"value"}})");
	const LobbyMessageTest LobbyMessageKeyValue("type: keyval\nkey: value\nkey2: value2", R"({"type":"keyval","key":"value","key2":"value2"})");
	const LobbyMessageTest LobbyMessageWithBracket("type: test\nmessage: this is message with[] {bracket}", R"({"type":"test","message":"this is message with[] {bracket}"})");

	TArray<LobbyMessageTest> TestData = {
		LobbyMessageArray,
		LobbyMessageArrayWithTrailingComma,
		LobbyMessageArrayQuoteComma,
		LobbyMessageArrayQuoteEscape,
		LobbyMessageObject,
		LobbyMessageKeyValue,
		LobbyMessageWithBracket,
	};

	for (const auto& Data : TestData)
	{
		FString MessageJSON = Lobby::LobbyMessageToJson(*Data.Message);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("%s -> %s"), *Data.Message, *MessageJSON);
		AB_TEST_EQUAL(Data.ExpectedJSON, MessageJSON);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestConnect2Users, "AccelByte.Tests.Lobby.B.ConnectUsers2", AutomationFlagMaskLobby);
bool LobbyTestConnect2Users::RunTest(const FString& Parameters)
{
	bool userResponded[2], userConnected[2];

	Lobbies[0]->SetConnectSuccessDelegate(ConnectSuccessDelegate);
	Lobbies[1]->SetConnectSuccessDelegate(ConnectSuccessDelegate);
	Lobbies[0]->SetConnectFailedDelegate(ConnectFailedDelegate);
	Lobbies[1]->SetConnectFailedDelegate(ConnectFailedDelegate);

	Lobbies[0]->Connect();

	while (!Lobbies[0]->IsConnected() || !bUsersConnectionSuccess)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LobbyTestConnect2Users: Wait user 0"));
		FTicker::GetCoreTicker().Tick(.5f);
	}
	userResponded[0] = bUsersConnectionSuccess;
	userConnected[0] = bUsersConnected;
	bUsersConnectionSuccess = false;
	bUsersConnected = false;

	Lobbies[1]->Connect();

	while (!Lobbies[1]->IsConnected() || !bUsersConnectionSuccess)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LobbyTestConnect2Users: Wait user 1"));
		FTicker::GetCoreTicker().Tick(.5f);
	}
	userResponded[1] = bUsersConnectionSuccess;
	userConnected[1] = bUsersConnected;

	for (int i = 0; i < 2; i++)
	{
		AB_TEST_TRUE(userConnected[i]);
		AB_TEST_TRUE(userResponded[i]);
	}
	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestConnectUser, "AccelByte.Tests.Lobby.B.ConnectUser", AutomationFlagMaskLobby);
bool LobbyTestConnectUser::RunTest(const FString& Parameters)
{
	Lobbies[0]->SetConnectSuccessDelegate(ConnectSuccessDelegate);
	Lobbies[0]->SetConnectFailedDelegate(ConnectFailedDelegate);

	LobbyConnect(1);
	while (!Lobbies[0]->IsConnected() || !bUsersConnectionSuccess)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LobbyTestConnectUser: Wait user 0"));
		FTicker::GetCoreTicker().Tick(.5f);
	}

	AB_TEST_TRUE(bUsersConnected);
	AB_TEST_TRUE(bUsersConnectionSuccess);

	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSendPrivateChat_FromMultipleUsers_ChatReceived, "AccelByte.Tests.Lobby.B.SendPrivateChat", AutomationFlagMaskLobby);
bool LobbyTestSendPrivateChat_FromMultipleUsers_ChatReceived::RunTest(const FString& Parameters)
{
	LobbyConnect(TestUserCount);

	int receivedChatCount = 0;

	Lobbies[0]->SetPrivateMessageNotifDelegate(GetMessageDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		FString userId = UserCreds[0].GetUserId();
		FString chatMessage = "Hello " + UserCreds[0].GetUserDisplayName() + " from " + UserCreds[i].GetUserDisplayName();
		Lobbies[i]->SendPrivateMessage(userId, chatMessage);
		FString text = FString::Printf(TEXT("Wait receiving message : %d"), receivedChatCount);
		WaitUntil([&]()
		{
			return bGetMessage;
		}, text, 30);


		if (bGetMessage)
		{
			bGetMessage = false;
			receivedChatCount++;
		}
	}

	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Received Message : %d"), receivedChatCount);
	AB_TEST_TRUE(receivedChatCount >= (TestUserCount - 1));

	LobbyDisconnect(TestUserCount);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSendChannelChat_FromMultipleUsers_ChatReceived, "AccelByte.Tests.Lobby.B.SendChannelChat", AutomationFlagMaskLobby);
bool LobbyTestSendChannelChat_FromMultipleUsers_ChatReceived::RunTest(const FString& Parameters)
{
	LobbyConnect(TestUserCount);

	int receivedChatCount = 0;
	const int chatMultiplier = 5;
	bool bChatAllReceived = false;

	for (int i = 0; i < TestUserCount; i++)
	{
		Lobbies[i]->SetChannelMessageNotifDelegate(THandler<FAccelByteModelsChannelMessageNotice>::CreateLambda([&receivedChatCount, &bChatAllReceived, chatMultiplier](const FAccelByteModelsChannelMessageNotice& result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Payload : %s"), *result.Payload);
			FPlatformAtomics::InterlockedIncrement(&receivedChatCount);
			if (receivedChatCount == TestUserCount * TestUserCount * chatMultiplier)
			{
				bChatAllReceived = true;
			}
		}));

		bool bJoinedChannel = false;
		Lobbies[i]->SetJoinChannelChatResponseDelegate(THandler<FAccelByteModelsJoinDefaultChannelResponse>::CreateLambda([&bJoinedChannel](const FAccelByteModelsJoinDefaultChannelResponse& result)
		{
			bJoinedChannel = true;
		}));
		Lobbies[i]->SendJoinDefaultChannelChatRequest();
		WaitUntil(bJoinedChannel, TEXT("Waiting for join..."));
	}

	for (int j = 0; j < chatMultiplier; j++)
	{
		for (int i = 0; i < TestUserCount; i++)
		{
			FString chatMessage = "Hello Chat, from " + UserCreds[i].GetUserDisplayName();
			Lobbies[i]->SendChannelMessage(chatMessage);
		}
	}

	FString text = FString::Printf(TEXT("Wait receiving message : %d"), receivedChatCount);
	WaitUntil([&]()
	{
		return bChatAllReceived;
	}, text, 30);

	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Received Message : %d"), receivedChatCount);
	AB_TEST_EQUAL(receivedChatCount, (TestUserCount * TestUserCount * chatMultiplier));

	LobbyDisconnect(TestUserCount);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSendChannelChat_Reconnected_ReceiveNoMessage, "AccelByte.Tests.Lobby.B.SendChannelChatReconnected", AutomationFlagMaskLobby);
bool LobbyTestSendChannelChat_Reconnected_ReceiveNoMessage::RunTest(const FString& Parameters)
{
	const int playerCount = 2;
	LobbyConnect(playerCount);

	int receivedChatCount = 0;
	bool bChatAllReceived = false;

	for (int i = 0; i < playerCount; i++)
	{
		Lobbies[i]->SetChannelMessageNotifDelegate(THandler<FAccelByteModelsChannelMessageNotice>::CreateLambda([&receivedChatCount, &bChatAllReceived, playerCount](const FAccelByteModelsChannelMessageNotice& result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Payload : %s"), *result.Payload);
			FPlatformAtomics::InterlockedIncrement(&receivedChatCount);
			if (receivedChatCount == (playerCount * playerCount)-1)
			{
				bChatAllReceived = true;
			}
		}));

		bool bJoinedChannel = false;
		Lobbies[i]->SetJoinChannelChatResponseDelegate(THandler<FAccelByteModelsJoinDefaultChannelResponse>::CreateLambda([&bJoinedChannel](const FAccelByteModelsJoinDefaultChannelResponse& result)
		{
			bJoinedChannel = true;
		}));
		Lobbies[i]->SendJoinDefaultChannelChatRequest();
		WaitUntil(bJoinedChannel, TEXT("Waiting for join..."));
	}

	FString chatMessage = "Hello Chat, from " + UserCreds[0].GetUserDisplayName();
	Lobbies[0]->SendChannelMessage(chatMessage);
	DelaySeconds(3);

	Lobbies[1]->Disconnect();
	DelaySeconds(1);
	Lobbies[1]->Connect();
	DelaySeconds(1);

	Lobbies[0]->SendChannelMessage(chatMessage);

	FString text = FString::Printf(TEXT("Wait receiving message : %d"), receivedChatCount);
	WaitUntil([&]()
	{
		return bChatAllReceived;
	}, text, 30);


	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Received Message : %d"), receivedChatCount);
	AB_TEST_EQUAL(receivedChatCount, ((playerCount * playerCount) - 1));

	LobbyDisconnect(playerCount);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllUsers, "AccelByte.Tests.Lobby.B.ListOnlineFriends", AutomationFlagMaskLobby);
bool LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllUsers::RunTest(const FString& Parameters)
{
	LobbyConnect(TestUserCount);

	Lobbies[0]->SetGetAllUserPresenceResponseDelegate(GetAllUsersPresenceDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		Lobbies[i]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);
	}

	Lobbies[0]->SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		Lobbies[i]->RequestFriend(UserCreds[0].GetUserId());
		FString text = FString::Printf(TEXT("Requesting Friend %d... "), i);
		WaitUntil(bRequestFriendSuccess, text);

		Lobbies[0]->AcceptFriend(UserCreds[i].GetUserId());
		text = FString::Printf(TEXT("Accepting Friend %d... "), i);
		WaitUntil(bAcceptFriendSuccess, text);

		Lobbies[i]->SendSetPresenceStatus(Availability::Availabe, "random activity");
		bRequestFriendSuccess = false;
		bAcceptFriendSuccess = false;
	}

	Lobbies[0]->SendGetOnlineUsersRequest();
	WaitUntil([&]()
	{
		return bGetAllUserPresenceSuccess;
	}, "Getting Friend Status...", 30);

	for (int i = 1; i < TestUserCount; i++)
	{
		Lobbies[i]->SendSetPresenceStatus(Availability::Offline, "disappearing");
		Lobbies[i]->Unfriend(UserCreds[0].GetUserId());
	}

	LobbyDisconnect(TestUserCount);

	AB_TEST_TRUE(bGetAllUserPresenceSuccess);
	AB_TEST_TRUE(onlineUserResponse.friendsId.Num() >= 3);
	AB_TEST_TRUE(onlineUserResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	AB_TEST_TRUE(onlineUserResponse.friendsId.Contains(UserCreds[2].GetUserId()));
	AB_TEST_TRUE(onlineUserResponse.friendsId.Contains(UserCreds[3].GetUserId()));
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllFriends, "AccelByte.Tests.Lobby.B.ListOnlineFriendsNew", AutomationFlagMaskLobby);
bool LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllFriends::RunTest(const FString& Parameters)
{
	LobbyConnect(TestUserCount);

	Lobbies[0]->SetGetOnlineFriendsPresenceResponseDelegate(GetFriendsPresenceDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		Lobbies[i]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);
	}

	Lobbies[0]->SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		Lobbies[i]->RequestFriend(UserCreds[0].GetUserId());
		FString text = FString::Printf(TEXT("Requesting Friend %d... "), i);
		WaitUntil(bRequestFriendSuccess, text);

		Lobbies[0]->AcceptFriend(UserCreds[i].GetUserId());
		text = FString::Printf(TEXT("Accepting Friend %d... "), i);
		WaitUntil(bAcceptFriendSuccess, text);

		Lobbies[i]->SendSetPresenceStatus(Availability::Availabe, "random activity");
		bRequestFriendSuccess = false;
		bAcceptFriendSuccess = false;
	}

	Lobbies[0]->SendGetOnlineFriendPresenceRequest();
	WaitUntil([&]()
	{
		return bGetFriendsPresenceSuccess;
	}, "Getting Friend Status...", 30);

	for (int i = 1; i < TestUserCount; i++)
	{
		Lobbies[i]->SendSetPresenceStatus(Availability::Offline, "disappearing");
		Lobbies[i]->Unfriend(UserCreds[0].GetUserId());
	}

	LobbyDisconnect(TestUserCount);

	AB_TEST_TRUE(bGetFriendsPresenceSuccess);
	AB_TEST_TRUE(onlineFriendResponse.friendsId.Num() >= 3);
	AB_TEST_TRUE(onlineFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	AB_TEST_TRUE(onlineFriendResponse.friendsId.Contains(UserCreds[2].GetUserId()));
	AB_TEST_TRUE(onlineFriendResponse.friendsId.Contains(UserCreds[3].GetUserId()));
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestBulk_User_Get_Presence_Success, "AccelByte.Tests.Lobby.B.Bulk_User_Get_Presence_Success", AutomationFlagMaskLobby);
bool LobbyTestBulk_User_Get_Presence_Success::RunTest(const FString& Parameters)
{
	// Arrange (connect some lobby user)
	const int ExpectedOffline = 2;
	LobbyConnect(TestUserCount);
	TArray<FString> LobbyUserIds;
	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyUserIds.Add(UserIds[i]);
		if(i >= (TestUserCount - ExpectedOffline))
		{
			Lobbies[i]->Disconnect();
		}
	}

	DelaySeconds(5, "Waiting backend to sync lobby disconnect");

	// Act (call bulk get user presence)
	FAccelByteModelsBulkUserStatusNotif GetPresenceResult;
	bool bGetPresenceSuccess = false;
	Lobbies[0]->BulkGetUserPresence(LobbyUserIds, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda([&bGetPresenceSuccess, &GetPresenceResult](const FAccelByteModelsBulkUserStatusNotif& Result)
	{
		GetPresenceResult = Result;
		bGetPresenceSuccess = true;
	}), LobbyTestErrorHandler);
	FlushHttpRequests();
	WaitUntil([&]()
	{
		return bGetPresenceSuccess;
	}, "Waiting for get presence...", 30);

	// Act (count the result)
	bool ExpectedOnline = true;
	int OnlineCount = 0;
	int OfflineCount = 0;
	for(auto Online : GetPresenceResult.Data)
	{
		// static_cast<int> is used because older UE4 doesn't convert enum to int automatically, for fixing UE 4.22 compile error.
		UE_LOG(LogAccelByteLobbyTest, Warning, TEXT("User: %s | Status: %d | %d"), *Online.UserID, static_cast<int32>(Online.Availability), GetPresenceResult.Data.Num());
		bool bIsOnline = false;
		bool bIsExpectedOnline = false;
		for (int i = 1; i < (TestUserCount - ExpectedOffline); i++) {
			if (Online.UserID == UserIds[i])
			{
				bIsExpectedOnline = true;
				if (Online.Availability == EAccelByteGeneralUserStatus::Online)
				{
					bIsOnline = true;
					OnlineCount++;
				}
				break;
			}
		}
		if (!bIsOnline && bIsExpectedOnline)
		{
			ExpectedOnline = false;
			break;
		}
		else if (!bIsExpectedOnline)
		{
			OfflineCount++;
		}
	}

	// Cleanup
	LobbyDisconnect(TestUserCount - ExpectedOffline);

	// Assert
	AB_TEST_TRUE(bGetPresenceSuccess);
	AB_TEST_EQUAL(OnlineCount, (TestUserCount - ExpectedOffline - 1)/*exclude the lobby 0*/);
	AB_TEST_EQUAL(OnlineCount, GetPresenceResult.Online);
	AB_TEST_EQUAL(OfflineCount, ExpectedOffline);
	AB_TEST_EQUAL(OfflineCount, GetPresenceResult.Offline);
	AB_TEST_TRUE(ExpectedOnline);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestBulk_User_Get_Presence_CountOnly, "AccelByte.Tests.Lobby.B.Bulk_User_Get_Presence_CountOnly", AutomationFlagMaskLobby);
bool LobbyTestBulk_User_Get_Presence_CountOnly::RunTest(const FString& Parameters)
{
	// Arrange (connect some lobby user)
	const int ExpectedOffline = 2;
	LobbyConnect(TestUserCount);
	TArray<FString> LobbyUserIds;
	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyUserIds.Add(UserIds[i]);
		if (i >= (TestUserCount - ExpectedOffline))
		{
			Lobbies[i]->Disconnect();
		}
	}

	DelaySeconds(5, "Waiting backend to sync lobby disconnect");

	// Act (call bulk get user presence)
	FAccelByteModelsBulkUserStatusNotif GetPresenceResult;
	bool bGetPresenceSuccess = false;
	Lobbies[0]->BulkGetUserPresence(LobbyUserIds, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda([&bGetPresenceSuccess, &GetPresenceResult](const FAccelByteModelsBulkUserStatusNotif& Result)
		{
			GetPresenceResult = Result;
			bGetPresenceSuccess = true;
		}), LobbyTestErrorHandler, true);
	FlushHttpRequests();
	WaitUntil([&]()
	{
		return bGetPresenceSuccess;
	}, "Waiting for get presence...", 30);

	// Cleanup
	LobbyDisconnect(TestUserCount - ExpectedOffline);

	AB_TEST_TRUE(bGetPresenceSuccess);
	AB_TEST_EQUAL(GetPresenceResult.Data.Num(), 0);
	AB_TEST_EQUAL(GetPresenceResult.Online, (TestUserCount - ExpectedOffline - 1));
	AB_TEST_EQUAL(GetPresenceResult.Offline, ExpectedOffline);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestBulk_User_Get_Presence_EmptyUserId, "AccelByte.Tests.Lobby.B.Bulk_User_Get_Presence_EmptyUserId", AutomationFlagMaskLobby);
bool LobbyTestBulk_User_Get_Presence_EmptyUserId::RunTest(const FString& Parameters)
{
	// Arrange (connect a lobby user)
	LobbyConnect(1);
	
	TArray<FString> LobbyUserIds;

	// Act (call bulk get user presence)
	FAccelByteModelsBulkUserStatusNotif GetPresenceResult;
	bool bGetPresenceSuccess = false;
	bool bGetPresenceDone = false;
	int32 GetPresenceErrorCode;
	Lobbies[0]->BulkGetUserPresence(LobbyUserIds, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda([&bGetPresenceSuccess, &bGetPresenceDone, &GetPresenceResult](const FAccelByteModelsBulkUserStatusNotif& Result)
		{
			GetPresenceResult = Result;
			bGetPresenceSuccess = true;
			bGetPresenceDone = true;
		}), FErrorHandler::CreateLambda([&bGetPresenceDone, &GetPresenceErrorCode](int32 Code, const FString& Message) 
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get User's Presence Error. Code: %d | Message: %s"), Code, *Message);
			GetPresenceErrorCode = Code;
			bGetPresenceDone = true;
		}));
	FlushHttpRequests();
	WaitUntil([&]()
	{
		return bGetPresenceDone;
	}, "Waiting for get presence...", 30);

	// Cleanup
	LobbyDisconnect(1);

	AB_TEST_FALSE(bGetPresenceSuccess);
	AB_TEST_EQUAL(GetPresenceErrorCode, (int32)ErrorCodes::InvalidRequest);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestGetPartyInfo_NoParty_ReturnError, "AccelByte.Tests.Lobby.B.GetPartyInfoError", AutomationFlagMaskLobby);
bool LobbyTestGetPartyInfo_NoParty_ReturnError::RunTest(const FString& Parameters)
{
	LobbyConnect(1);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "Leaving Party...");

	Lobbies[0]->SendInfoPartyRequest();
	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	LobbyDisconnect(2);
	AB_TEST_TRUE(bGetInfoPartyError);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestGetPartyInfo_PartyCreated_ReturnOk, "AccelByte.Tests.Lobby.B.GetPartyInfo", AutomationFlagMaskLobby);
bool LobbyTestGetPartyInfo_PartyCreated_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(1);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	Lobbies[0]->SendLeavePartyRequest();

	WaitUntil(bLeavePartySuccess, "Leaving Party...");

	LobbyDisconnect(1);

	AB_TEST_TRUE(bGetInfoPartySuccess);
	AB_TEST_FALSE(infoPartyResponse.PartyId.IsEmpty());
	AB_TEST_FALSE(infoPartyResponse.InvitationToken.IsEmpty());
	AB_TEST_TRUE(infoPartyResponse.Members.Num() > 0);
	AB_TEST_EQUAL(infoPartyResponse.Members[0], UserCreds[0].GetUserId());

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MessageIdCachedResponse, "AccelByte.Tests.Lobby.B.MessageIdCachedResponse", AutomationFlagMaskLobby);
bool MessageIdCachedResponse::RunTest(const FString& Parameters)
{
	// Lobby Prep
	LobbyConnect(1);
	TSharedPtr<Lobby> Lobby = Lobbies[0];
	Lobby->SetCreatePartyResponseDelegate(CreatePartyDelegate);
	Lobby->SetLeavePartyResponseDelegate(LeavePartyDelegate);
	Lobby->SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	{ // Case 1: No response delegate changes
		const FString MessageIdA = Lobby->SendInfoPartyRequest(GetInfoPartyDelegate);
		WaitUntil(bGetInfoPartySuccess, "Waiting for basic delegate response...");
		AB_TEST_TRUE(bGetInfoPartySuccess);
		bGetInfoPartySuccess = false;
	}

	{ // Case 2: 2 different delegates back to back 
		bool bIsACalled, bIsBCalled;
		uint8 CallbackCount = 0;

		const auto GetInfoPartyDelegateA = Api::Lobby::FPartyInfoResponse::CreateLambda([&bIsACalled, &CallbackCount](FAccelByteModelsInfoPartyResponse result)
			{
				bIsACalled = true;
				CallbackCount += 1;
			});
		const auto GetInfoPartyDelegateB = Api::Lobby::FPartyInfoResponse::CreateLambda([&bIsBCalled, &CallbackCount](FAccelByteModelsInfoPartyResponse result)
			{
				bIsBCalled = true;
				CallbackCount += 1;
			});

		const FString MessageIdA = Lobby->SendInfoPartyRequest(GetInfoPartyDelegateA);
		const FString MessageIdB = Lobby->SendInfoPartyRequest(GetInfoPartyDelegateB);
		WaitUntil([&] { return CallbackCount == 2; }, "Waiting for 2 responses...");
		UTEST_TRUE("2 different cached callbacks returning 2 different responses", bIsACalled && bIsBCalled);
	}

	{ // Case 3. unique responses for each requests
		TArray<uint8> UniqueResponseChecker, Index;
		TMap<uint8, FString> MessageIdResponseMap;

		// Work Test Item Prep
		const uint8 MaxRequestCount = 5;

		for (uint8 i = 0; i < MaxRequestCount;i++)
		{
			const auto GetInfoPartyDelegate_Unique = Api::Lobby::FPartyInfoResponse::CreateLambda([i, &MessageIdResponseMap, &UniqueResponseChecker](FAccelByteModelsInfoPartyResponse result)
				{
					UniqueResponseChecker.Add(i);
				});

			const FString MessageId = Lobby->SendInfoPartyRequest(GetInfoPartyDelegate_Unique);
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Just sent request with message id [%s] expecting response [%d]"), *MessageId, i);

			Index.Add(i);
			MessageIdResponseMap.Emplace(i, MessageId);
		}


		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Expected responses"));
		for (const TPair<uint8, FString>& MessageIdResponsePair : MessageIdResponseMap) {
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("message id [%s] -> response [%d]"), *MessageIdResponsePair.Value, MessageIdResponsePair.Key);
		}

		WaitUntil([&] { return UniqueResponseChecker.Num() == Index.Num(); }, "Waiting for all unique responses being called ...", 30);
		for (uint8 anIndex : Index)
		{
			UTEST_TRUE(FString::Printf(TEXT("Response [%d] is called correctly"), anIndex), UniqueResponseChecker.Contains(anIndex));
		}

	}

	{ // Case 4. New Delegate does not affect request that is still in progress
		bool bLastAssignedWrongDelegateCalled = false;
		const auto GetInfoPartyDelegate_ShouldNotBeCalled = Api::Lobby::FPartyInfoResponse::CreateLambda([&bLastAssignedWrongDelegateCalled](FAccelByteModelsInfoPartyResponse result)
			{
				bLastAssignedWrongDelegateCalled = true;
			});

		Lobby->SendInfoPartyRequest(GetInfoPartyDelegate);
		Lobby->SetInfoPartyResponseDelegate(GetInfoPartyDelegate_ShouldNotBeCalled);
		WaitUntil(bGetInfoPartySuccess, "Waiting for cached response ...", 15);
		UTEST_FALSE("Overriding response will not affect response for request that is already on the way. Single Sequence", bLastAssignedWrongDelegateCalled);

		bLastAssignedWrongDelegateCalled = false;
		Lobby->SendInfoPartyRequest(GetInfoPartyDelegate);
		Lobby->SendInfoPartyRequest(GetInfoPartyDelegate);
		Lobby->SetInfoPartyResponseDelegate(GetInfoPartyDelegate_ShouldNotBeCalled);
		Lobby->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
		Lobby->SendInfoPartyRequest(GetInfoPartyDelegate);
		Lobby->SetInfoPartyResponseDelegate(GetInfoPartyDelegate_ShouldNotBeCalled);
		UTEST_FALSE("Overriding response will not affect response for request that is already on the way. Pseudo-Random Sequence", bLastAssignedWrongDelegateCalled);
	}

	//@TODO dummy uncached/unregistered/unknown message id will not call a response

	Lobby->SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "Leaving Party...");
	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestGetPartyData_PartyCreated_ReturnOk, "AccelByte.Tests.Lobby.B.GetPartyData", AutomationFlagMaskLobby);
bool LobbyTestGetPartyData_PartyCreated_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(1);
	FString PartyId;

	const Lobby::FPartyInfoResponse OnInfoPartyResponse = Lobby::FPartyInfoResponse::CreateLambda(
		[&PartyId](FAccelByteModelsInfoPartyResponse Response)
		{
			PartyId = Response.PartyId;	
		});
	Lobbies[0]->SetInfoPartyResponseDelegate(OnInfoPartyResponse);
	
	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);
	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating party..");

	Lobbies[0]->SendInfoPartyRequest();
	WaitUntil([&]() { return !PartyId.IsEmpty(); });

	bool bGetPartyDataDone = false;
	bool bGetPartyDataError = false;
	bool bGetPartyDataSuccess = false;
	FAccelByteModelsPartyData PartyData;
	const THandler<FAccelByteModelsPartyData> OnPartyDataSuccess = THandler<FAccelByteModelsPartyData>::CreateLambda(
		[&bGetPartyDataSuccess, &bGetPartyDataDone, &PartyData](const FAccelByteModelsPartyData& Response)
		{
			bGetPartyDataDone = true;
			bGetPartyDataSuccess = true;
			PartyData = Response;
		});	
	const FErrorHandler OnPartyDataError = FErrorHandler::CreateLambda(
		[&bGetPartyDataError, &bGetPartyDataDone](int32 ErrorCode, const FString& ErrorMessage)
		{
			bGetPartyDataDone = true;
			bGetPartyDataError = true;
		});
	
	Lobbies[0]->GetPartyData(PartyId, OnPartyDataSuccess, OnPartyDataError);
	WaitUntil(bGetPartyDataDone, "Getting party data...");
	
	Lobbies[0]->SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "Leaving Party...");

	LobbyDisconnect(1);
	ResetResponses();

	AB_TEST_TRUE(bGetPartyDataSuccess);
	AB_TEST_EQUAL(PartyId, PartyData.PartyId);
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestCreateParty_PartyAlreadyCreated_ReturnError, "AccelByte.Tests.Lobby.B.CreatePartyError", AutomationFlagMaskLobby);
bool LobbyTestCreateParty_PartyAlreadyCreated_ReturnError::RunTest(const FString& Parameters)
{
	LobbyConnect(1);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SendLeavePartyRequest();

	WaitUntil(bLeavePartySuccess, "Leaving Party...");

	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	bCreatePartySuccess = false;

	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	Lobbies[0]->SendLeavePartyRequest();

	WaitUntil(bLeavePartySuccess, "Leaving Party...");

	LobbyDisconnect(1);
	AB_TEST_TRUE(bCreatePartyError);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestJoinParty_Via_PartyCode, "AccelByte.Tests.Lobby.B.JoinPartyCode", AutomationFlagMaskLobby);
bool LobbyTestJoinParty_Via_PartyCode::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	FString partyCode;

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetPartyGenerateCodeResponseDelegate(GeneratePartyCodeDelegate);

	Lobbies[0]->SetPartyGetCodeResponseDelegate(GetPartyCodeDelegate);

	Lobbies[0]->SetPartyDeleteCodeResponseDelegate(DeletePartyCodeDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetPartyJoinViaCodeResponseDelegate(JoinViaCodeDelegate);

	// Leave Previous parties
	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[1]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	// Create Party
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	// Get Party Code
	Lobbies[0]->SendPartyGetCodeRequest();
	WaitUntil(bGetPartyCodeSuccess, "Getting Party Code...");
	AB_TEST_TRUE(bGetPartyCodeSuccess);
	bGetPartyCodeSuccess = false;
	partyCode = partyCodeResponse.PartyCode;

	// Join via Party Code
	Lobbies[1]->SendPartyJoinViaCodeRequest(partyCode);
	WaitUntil(bJoinPartyViaCodeSuccess, "Joining Party via Code...");
	AB_TEST_TRUE(bJoinPartyViaCodeSuccess);
	bJoinPartyViaCodeError = false;
	bJoinPartyViaCodeSuccess = false;

	// Leave Party
	bLeavePartySuccess = false;
	Lobbies[1]->SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "Leaving Party...");

	// Generate new Party Code
	Lobbies[0]->SendPartyGenerateCodeRequest();
	WaitUntil(bGeneratePartyCodeSuccess, "Generate Party Code...");
	AB_TEST_TRUE(bGeneratePartyCodeSuccess);

	// Join via Party Code using previous code
	Lobbies[1]->SendPartyJoinViaCodeRequest(partyCode);
	WaitUntil(bJoinPartyViaCodeSuccess, "Joining Party via Code...");
	AB_TEST_TRUE(bJoinPartyViaCodeError);
	bJoinPartyViaCodeError = false;
	bJoinPartyViaCodeSuccess = false;

	// Get Party Code
	Lobbies[0]->SendPartyGetCodeRequest();
	WaitUntil(bGetPartyCodeSuccess, "Getting Party Code...");
	AB_TEST_TRUE(bGetPartyCodeSuccess);
	bGetPartyCodeSuccess = false;
	partyCode = partyCodeResponse.PartyCode;
	AB_TEST_TRUE(partyCode == partyGenerateCodeResponse.PartyCode);

	// Join via Party Code using new code
	Lobbies[1]->SendPartyJoinViaCodeRequest(partyCode);
	WaitUntil(bJoinPartyViaCodeSuccess, "Joining Party via Code...");
	AB_TEST_TRUE(bJoinPartyViaCodeSuccess);
	bJoinPartyViaCodeError = false;
	bJoinPartyViaCodeSuccess = false;

	// Delete Party Code
	Lobbies[0]->SendPartyDeleteCodeRequest();
	WaitUntil(bDeletePartyCodeSuccess, "Deleting Party Code...");
	AB_TEST_TRUE(bDeletePartyCodeSuccess);

	LobbyDisconnect(2);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestJoinParty_Via_PartyCodeInvalid, "AccelByte.Tests.Lobby.B.JoinPartyCodeInvalid", AutomationFlagMaskLobby);
bool LobbyTestJoinParty_Via_PartyCodeInvalid::RunTest(const FString& Parameters)
{
	LobbyConnect(1);

	bool bJoinFailed = false;
	Lobbies[0]->SetPartyJoinViaCodeResponseDelegate(Lobby::FPartyJoinViaCodeResponse::CreateLambda([&](const FAccelByteModelsPartyJoinReponse& Result)
	{
		if(Result.Code.Equals("11573") || !Result.Code.Equals("0"))
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("[EXPECTED] Join party code is invalid with code : %s"), *Result.Code);
			bJoinFailed = true;
		}
	}));
	Lobbies[0]->SendPartyJoinViaCodeRequest("INVALID");
	WaitUntil(bJoinFailed, "Waiting to join party via code...");
	
	AB_TEST_TRUE(bJoinFailed);

	LobbyDisconnect(1);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestInviteToParty_InvitationAccepted_CanChat, "AccelByte.Tests.Lobby.B.PartyChat", AutomationFlagMaskLobby);
bool LobbyTestInviteToParty_InvitationAccepted_CanChat::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInvitePartyResponseDelegate(InvitePartyDelegate);

	Lobbies[1]->SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);

	Lobbies[1]->SetInvitePartyJoinResponseDelegate(JoinPartyDelegate);
	
	Lobbies[0]->SetPartyChatNotifDelegate(PartyChatNotifDelegate);
	
	Lobbies[1]->SetPartyMessageResponseDelegate(PartyChatSendDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[1]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	Lobbies[0]->SendInviteToPartyRequest(UserCreds[1].GetUserId());
	WaitUntil(bInvitePartySuccess, "Inviting to Party...");

	AB_TEST_TRUE(bInvitePartySuccess);

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation...", 30);

	AB_TEST_FALSE(bGetInvitedNotifError);

	Lobbies[0]->SetPartyDataUpdateNotifDelegate(PartyDataUpdateDelegate);

	Lobbies[1]->SendAcceptInvitationRequest(*invitedToPartyResponse.PartyId, *invitedToPartyResponse.InvitationToken);
	while (!bJoinPartySuccess && !bGetInvitedNotifError)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Joining a Party..."));
		FTicker::GetCoreTicker().Tick(.5f);
	}
	AB_TEST_FALSE(bJoinPartyError);

	WaitUntil(bGetPartyDataUpdateNotifSuccess, "Waiting receive party data update notif");
	const FAccelByteModelsPartyDataNotif partyDataUpdate = partyDataNotif;

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();
	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");
	const auto partyInfoResult = infoPartyResponse;

	AB_TEST_FALSE(bGetInfoPartyError);

	// Compare partydata notif with partyget info
	AB_TEST_EQUAL(partyDataUpdate.PartyId, partyInfoResult.PartyId);
	AB_TEST_EQUAL(partyDataUpdate.Leader, partyInfoResult.LeaderId);
	AB_TEST_EQUAL(partyDataUpdate.Members.Num(), partyInfoResult.Members.Num());

	Lobbies[1]->SendPartyMessage("This is a party chat");
	WaitUntil(bSendPartyChatSuccess, "Sending a Party Chat...");
	AB_TEST_FALSE(bSendPartyChatError);

	while (!bReceivedPartyChatSuccess && !bSendPartyChatError)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Fetching Party Chat..."));
		FTicker::GetCoreTicker().Tick(.5f);
	}
	AB_TEST_TRUE(bReceivedPartyChatSuccess);

	bLeavePartySuccess = false;
	bLeavePartyError = false;
	Lobbies[0]->SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "Leaving Party...");
	AB_TEST_FALSE(bLeavePartyError);

	bLeavePartySuccess = false;
	bLeavePartyError = false;
	Lobbies[1]->SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "Leaving Party...");
	AB_TEST_FALSE(bLeavePartyError);

	LobbyDisconnect(2);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestInviteToParty_InvitationRejected, "AccelByte.Tests.Lobby.B.RejectPartyInvitation", AutomationFlagMaskLobby);
bool LobbyTestInviteToParty_InvitationRejected::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);
	Lobbies[0]->SetInvitePartyResponseDelegate(InvitePartyDelegate);
	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	Lobbies[1]->SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);
	Lobbies[1]->SetInvitePartyRejectResponseDelegate(RejectPartyDelegate);
	Lobbies[0]->SetPartyInvitationRejectedNotifDelegate(RejectPartyNotifDelegate);
	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SendInfoPartyRequest();
	
	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bRejectPartySuccess = false;
	bRejectPartyError = false; 
	bRejectedPartyNotifSuccess = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[1]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	Lobbies[0]->SendInviteToPartyRequest(UserCreds[1].GetUserId());
	WaitUntil(bInvitePartySuccess, "Inviting to Party...");

	AB_TEST_TRUE(bInvitePartySuccess);

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation...", 30);


	AB_TEST_FALSE(bGetInvitedNotifError);

	Lobbies[1]->SendRejectInvitationRequest(*invitedToPartyResponse.PartyId, *invitedToPartyResponse.InvitationToken);
	while (!bRejectPartySuccess && !bGetInvitedNotifError)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Rejecting a Party..."));
		FTicker::GetCoreTicker().Tick(.5f);
	}
	AB_TEST_FALSE(bRejectPartyError);

	WaitUntil([&]()
	{
		return bRejectedPartyNotifSuccess;
	}, "Waiting for Reject Party Notification...", 30);

	AB_TEST_TRUE(bRejectedPartyNotifSuccess)

	LobbyDisconnect(2);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestPartyMember_Kicked, "AccelByte.Tests.Lobby.B.PartyKicked", AutomationFlagMaskLobby);
bool LobbyTestPartyMember_Kicked::RunTest(const FString& Parameters)
{
	FAccelByteModelsPartyGetInvitedNotice invitedToParty[2];
	FAccelByteModelsPartyJoinReponse joinParty[2];
	LobbyConnect(3);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInvitePartyResponseDelegate(InvitePartyDelegate);

	Lobbies[1]->SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);

	Lobbies[2]->SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);

	Lobbies[1]->SetInvitePartyJoinResponseDelegate(JoinPartyDelegate);

	Lobbies[2]->SetInvitePartyJoinResponseDelegate(JoinPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[2]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[2]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetInvitePartyKickMemberResponseDelegate(KickPartyMemberDelegate);

	Lobbies[2]->SetPartyKickNotifDelegate(KickedFromPartyDelegate);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[1]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	Lobbies[2]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[2]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;

	Lobbies[0]->SendCreatePartyRequest();
	
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	Lobbies[0]->SendInviteToPartyRequest(UserCreds[1].GetUserId());

	WaitUntil(bInvitePartySuccess, "Inviting to Party...");

	bInvitePartySuccess = false;

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation", 30);


	bGetInvitedNotifSuccess = false;
	invitedToParty[0] = invitedToPartyResponse;

	Lobbies[0]->SendInviteToPartyRequest(UserCreds[2].GetUserId());

	WaitUntil(bInvitePartySuccess, "Inviting to Party...");

	bInvitePartySuccess = false;

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation", 30);

	bGetInvitedNotifSuccess = false;
	invitedToParty[1] = invitedToPartyResponse;

	Lobbies[1]->SendAcceptInvitationRequest(invitedToParty[0].PartyId, invitedToParty[0].InvitationToken);

	WaitUntil(bJoinPartySuccess, "Joining a Party...");

	bJoinPartySuccess = false;
	joinParty[0] = joinPartyResponse;

	Lobbies[2]->SendAcceptInvitationRequest(invitedToParty[1].PartyId, invitedToParty[1].InvitationToken);

	WaitUntil(bJoinPartySuccess, "Joining a Party...");

	bJoinPartySuccess = false;
	joinParty[1] = joinPartyResponse;

	Lobbies[0]->SendKickPartyMemberRequest(UserCreds[2].GetUserId());

	WaitUntil(bKickPartyMemberSuccess, "Kicking Party Member...");

	WaitUntil(bKickedFromPartySuccess, "Waiting to Get Kicked from Party...");

	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	for (int i = 0; i < 2; i++)
	{
		Lobbies[i]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
		bLeavePartySuccess = false;
	}


	LobbyDisconnect(3);

	AB_TEST_TRUE(bKickPartyMemberSuccess);
	AB_TEST_TRUE(bKickedFromPartySuccess);
	AB_TEST_TRUE(joinParty[2].Members.Num() == 3 || joinParty[1].Members.Num() == 3);
	AB_TEST_EQUAL(infoPartyResponse.Members.Num(), 2);

	ResetResponses();
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestPartyMember_PromoteLeader, "AccelByte.Tests.Lobby.B.PromotePartyLeader", AutomationFlagMaskLobby);
bool LobbyTestPartyMember_PromoteLeader::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetInvitePartyResponseDelegate(InvitePartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetPartyPromoteLeaderResponseDelegate(PromotePartyLeaderDelegate);

	Lobbies[1]->SetInvitePartyJoinResponseDelegate(JoinPartyDelegate);

	Lobbies[1]->SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[1]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;

	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	Lobbies[0]->SendInviteToPartyRequest(UserCreds[1].GetUserId());

	WaitUntil(bInvitePartySuccess, "Inviting to Party...");

	bInvitePartySuccess = false;

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation", 30);

	bGetInvitedNotifSuccess = false;

	Lobbies[1]->SendAcceptInvitationRequest(*invitedToPartyResponse.PartyId, *invitedToPartyResponse.InvitationToken);

	WaitUntil(bJoinPartySuccess, "Joining a Party...");

	AB_TEST_TRUE(bJoinPartySuccess);

	bJoinPartySuccess = false;

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	bGetInfoPartySuccess = false;

	Lobbies[0]->SendPartyPromoteLeaderRequest(UserCreds[1].GetUserId());

	WaitUntil(bPromotePartyLeaderSuccess, "Promoting Party leader...");

	AB_TEST_TRUE(bPromotePartyLeaderSuccess);

	bPromotePartyLeaderSuccess = false;

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	bGetInfoPartySuccess = false;

	AB_TEST_EQUAL(infoPartyResponse.LeaderId, UserCreds[1].GetUserId());

	LobbyDisconnect(2);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestConnected_ForMoreThan1Minutes_DoesntDisconnect, "AccelByte.DisabledTests.Lobby.B.Connect1Mins", AutomationFlagMaskLobby);
bool LobbyTestConnected_ForMoreThan1Minutes_DoesntDisconnect::RunTest(const FString& Parameters)
{
	LobbyConnect(1);
	for (int i = 0; i < 100; i += 5)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Wait for %d seconds. Lobby.IsConnected=%d"), i, Lobbies[0]->IsConnected());
		FPlatformProcess::Sleep(5);
		FTicker::GetCoreTicker().Tick(5);
	}
	AB_TEST_TRUE(Lobbies[0]->IsConnected());

	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestNotification_GetSyncNotification, "AccelByte.Tests.Lobby.B.NotifSync", AutomationFlagMaskLobby);
bool LobbyTestNotification_GetSyncNotification::RunTest(const FString& Parameters)
{
	const int repetition = 2;
	FString payloads[repetition];
	bool bSendNotifSucccess[repetition] = { false };
	bool bGetNotifCheck[repetition] = { false };
	FAccelByteModelsNotificationMessage getNotifCheck[repetition];

	LobbyConnect(1);

	Lobbies[0]->SetMessageNotifDelegate(GetNotifDelegate);

	for (int i = 0; i < repetition; i++)
	{
		payloads[i] = FString::Printf(TEXT("Notification number %d"), i);

		UAccelByteBlueprintsTest::SendNotif(UserCreds[0].GetUserId(), payloads[i], true, FVoidHandler::CreateLambda([&]()
		{
			bSendNotifSucccess[i] = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Notification Sent!"));
		}), LobbyTestErrorHandler);

		WaitUntil(bSendNotifSucccess[i], "Sending Notification...");

		WaitUntil([&]()
		{
			return bGetNotifSuccess;
		}, "Getting All Notifications...", 30);
		

		bGetNotifCheck[i] = bGetNotifSuccess;
		getNotifCheck[i] = getNotifResponse;
		bGetNotifSuccess = false;
	}
	LobbyDisconnect(1);
	for (int i = 0; i < repetition; i++)
	{
		AB_TEST_TRUE(bGetNotifCheck[i]);
		AB_TEST_EQUAL(getNotifCheck[i].Payload, payloads[i]);
	}
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestNotification_SendNotifUserToUserSync, "AccelByte.Tests.Lobby.B.NotifSyncUserToUser", AutomationFlagMaskLobby);
bool LobbyTestNotification_SendNotifUserToUserSync::RunTest(const FString& Parameters)
{
	const int repetition = 2;
	FAccelByteModelsFreeFormNotificationRequest MessageRequest[repetition];
	bool bSendNotifSucccess[repetition] = { false };
	bool bGetNotifCheck[repetition] = { false };
	FAccelByteModelsNotificationMessage getNotifCheck[repetition];

	LobbyConnect(2);

	Lobbies[1]->SetMessageNotifDelegate(GetNotifDelegate);

	for (int i = 0; i < repetition; i++)
	{
		MessageRequest[i].Topic = FString::Printf(TEXT("Message number %d"), i);
		MessageRequest[i].Message = FString::Printf(TEXT("Test from the integration test UE4 number %d"), i);

		Lobbies[0]->SendNotificationToUser(UserCreds[1].GetUserId(), MessageRequest[i], false, FVoidHandler::CreateLambda([&]()
		{
			bSendNotifSucccess[i] = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Notification Sent!"));
		}), LobbyTestErrorHandler);

		WaitUntil(bSendNotifSucccess[i], "Sending Notification...");

		WaitUntil([&]()
		{
			return bGetNotifSuccess;
		}, "Getting All Notifications...", 30);

		bGetNotifCheck[i] = bGetNotifSuccess;
		getNotifCheck[i] = getNotifResponse;
		bGetNotifSuccess = false;
	}
	LobbyDisconnect(2);
	for (int i = 0; i < repetition; i++)
	{
		AB_TEST_TRUE(bGetNotifCheck[i]);
		AB_TEST_EQUAL(getNotifCheck[i].Payload, MessageRequest[i].Message);
		AB_TEST_EQUAL(getNotifCheck[i].Topic, MessageRequest[i].Topic);
	}
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSetUserStatus_CheckedByAnotherUser, "AccelByte.Tests.Lobby.B.SetUserStatus", AutomationFlagMaskLobby);
bool LobbyTestSetUserStatus_CheckedByAnotherUser::RunTest(const FString& Parameters)
{
	Availability expectedUserStatus = Availability::Availabe;

	LobbyConnect(2);

	Lobbies[0]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	Lobbies[1]->SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	Lobbies[0]->SetUserPresenceResponseDelegate(UserPresenceDelegate);

	Lobbies[1]->SetUserPresenceResponseDelegate(UserPresenceDelegate);

	Lobbies[1]->SetUserPresenceNotifDelegate(UserPresenceNotifDelegate);

	Lobbies[1]->SetUnfriendResponseDelegate(UnfriendDelegate);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend...");

	Lobbies[1]->SendSetPresenceStatus(Availability::Availabe, "ready to play");

	WaitUntil([&]()
	{
		return bUserPresenceSuccess;
	}, "Changing User Status...", 30);

	bUserPresenceSuccess = false;

	Lobbies[0]->SendSetPresenceStatus(expectedUserStatus, "expected activity");

	WaitUntil([&]()
	{
		return bUserPresenceSuccess;
	}, "Changing User Status...", 30);

	WaitUntil([&]() 
	{
		return bUserPresenceNotifSuccess; 
	}, "Waiting for Changing User Presence...", 30);

	Lobbies[1]->Unfriend(UserCreds[0].GetUserId());

	WaitUntil(bUnfriendSuccess, "Waiting Unfriend...");

	LobbyDisconnect(2);

	FString FExpectedUserPresence = FString::FromInt((int)expectedUserStatus);

	AB_TEST_TRUE(bUserPresenceSuccess);
	AB_TEST_TRUE(bUserPresenceNotifSuccess);
	AB_TEST_FALSE(bUserPresenceError);
	AB_TEST_FALSE(bUserPresenceNotifError);
	AB_TEST_EQUAL(FExpectedUserPresence.Compare(userPresenceNotifResponse.Availability), 0);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestChangeUserStatus_CheckedByAnotherUser, "AccelByte.Tests.Lobby.B.ChangeUserStatus", AutomationFlagMaskLobby);
bool LobbyTestChangeUserStatus_CheckedByAnotherUser::RunTest(const FString& Parameters)
{
	Availability expectedUserStatus = Availability::Busy;

	LobbyConnect(2);

	Lobbies[0]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	Lobbies[1]->SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	Lobbies[1]->SetUserPresenceResponseDelegate(UserPresenceDelegate);

	Lobbies[0]->SetUserPresenceResponseDelegate(UserPresenceDelegate);

	Lobbies[1]->SetUserPresenceNotifDelegate(UserPresenceNotifDelegate);

	Lobbies[1]->SetUnfriendResponseDelegate(UnfriendDelegate);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend...");

	Lobbies[1]->SendSetPresenceStatus(Availability::Availabe, "ready to play again");

	WaitUntil([&]()
	{
		return bUserPresenceSuccess;
	}, "Changing User Status...", 30);

	bUserPresenceSuccess = false;
	Lobbies[0]->SendSetPresenceStatus(Availability::Availabe, "ready to play too");

	WaitUntil([&]()
	{
		return bUserPresenceSuccess;
	}, "Changing User Status...", 30);

	WaitUntil([&]()
	{
		return bUserPresenceNotifSuccess;
	}, "Waiting for Changing User Presence...", 30);

	bUserPresenceSuccess = false;
	bUserPresenceNotifSuccess = false;
	Lobbies[0]->SendSetPresenceStatus(expectedUserStatus, "busy, can't play");

	WaitUntil([&]()
	{
		return bUserPresenceSuccess;
	}, "Changing User Status...", 30);

	WaitUntil([&]()
	{
		return bUserPresenceNotifSuccess;
	}, "Waiting for Changing User Presence...", 30);

	Lobbies[1]->Unfriend(UserCreds[0].GetUserId());

	WaitUntil(bUnfriendSuccess, "Waiting Unfriend...");

	LobbyDisconnect(2);

	FString FExpectedUserPresence = FString::FromInt((int)expectedUserStatus);

	AB_TEST_TRUE(bUserPresenceSuccess);
	AB_TEST_TRUE(bUserPresenceNotifSuccess);
	AB_TEST_FALSE(bUserPresenceError);
	AB_TEST_FALSE(bUserPresenceNotifError);
	AB_TEST_EQUAL(FExpectedUserPresence.Compare(userPresenceNotifResponse.Availability), 0);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Request_Accept, "AccelByte.Tests.Lobby.B.FriendRequest", AutomationFlagMaskLobby);
bool LobbyTestFriends_Request_Accept::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[1]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[0]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	Lobbies[1]->SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	Lobbies[0]->SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	Lobbies[1]->SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	Lobbies[0]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	Lobbies[1]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	Lobbies[0]->SetUnfriendResponseDelegate(UnfriendDelegate);

	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");

	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	Lobbies[0]->ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	Lobbies[1]->ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend Request...");
	AB_TEST_FALSE(bAcceptFriendError);

	Lobbies[0]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[1]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[0]->Unfriend(UserCreds[1].GetUserId());

	WaitUntil(bUnfriendSuccess, "Waiting Unfriend...");
	AB_TEST_FALSE(bUnfriendError);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Notification_Request_Accept, "AccelByte.Tests.Lobby.B.FriendNotifRequest", AutomationFlagMaskLobby);
bool LobbyTestFriends_Notification_Request_Accept::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[1]->SetOnIncomingRequestFriendsNotifDelegate(OnIncomingRequestNotifDelegate);

	Lobbies[0]->SetOnFriendRequestAcceptedNotifDelegate(OnRequestAcceptedNotifDelegate);

	Lobbies[0]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	Lobbies[1]->SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	Lobbies[0]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	Lobbies[1]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	Lobbies[0]->SetUnfriendResponseDelegate(UnfriendDelegate);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	WaitUntil(bOnIncomingRequestNotifSuccess, "Waiting for Incoming Friend Request...");
	AB_TEST_FALSE(bOnIncomingRequestNotifError);
	AB_TEST_EQUAL(requestFriendNotifResponse.friendId, UserCreds[0].GetUserId());

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend Request...");
	AB_TEST_FALSE(bAcceptFriendError);

	WaitUntil(bOnRequestAcceptedNotifSuccess, "Waiting for Accepted Friend Request...");
	AB_TEST_FALSE(bOnRequestAcceptedNotifError);
	AB_TEST_EQUAL(acceptFriendNotifResponse.friendId, UserCreds[1].GetUserId());

	Lobbies[0]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[1]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[0]->Unfriend(UserCreds[1].GetUserId());

	WaitUntil(bUnfriendSuccess, "Waiting Unfriend...");
	AB_TEST_FALSE(bUnfriendError);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Request_Unfriend, "AccelByte.Tests.Lobby.B.FriendRequestUnfriend", AutomationFlagMaskLobby);
bool LobbyTestFriends_Request_Unfriend::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[1]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[0]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	Lobbies[1]->SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	Lobbies[0]->SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	Lobbies[1]->SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	Lobbies[0]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	Lobbies[1]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	Lobbies[0]->SetUnfriendResponseDelegate(UnfriendDelegate);

	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	Lobbies[1]->SetOnUnfriendNotifDelegate(UnfriendNotifDelegate);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	Lobbies[0]->ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	Lobbies[1]->ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend Request...");
	AB_TEST_FALSE(bAcceptFriendError);

	Lobbies[0]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[1]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[0]->Unfriend(UserCreds[1].GetUserId());

	WaitUntil(bUnfriendSuccess, "Waiting Unfriend...");
	AB_TEST_FALSE(bUnfriendError);

	WaitUntil(bUnfriendNotifSuccess, "Waiting Unfriend Notif...");
	AB_TEST_TRUE(bUnfriendNotifSuccess);

	AB_TEST_EQUAL(UnfriendNotifResponse.friendId, UserCreds[0].GetUserId());

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[0]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	}

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[1]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));
	}

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Request_Reject, "AccelByte.Tests.Lobby.B.FriendRequestReject", AutomationFlagMaskLobby);
bool LobbyTestFriends_Request_Reject::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[1]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[0]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	Lobbies[0]->SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	Lobbies[1]->SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	Lobbies[1]->SetRejectFriendsResponseDelegate(RejectFriendDelegate);

	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	Lobbies[0]->SetOnRejectFriendsNotifDelegate(RejectFriendNotifDelegate);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	Lobbies[0]->ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	Lobbies[1]->ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->RejectFriend(UserCreds[0].GetUserId());

	WaitUntil(bRejectFriendSuccess, "Rejecting Friend Request...");
	AB_TEST_FALSE(bRejectFriendError);

	WaitUntil(bRejectFriendNotifSuccess, "Getting Reject Friend Notif...");
	AB_TEST_TRUE(bRejectFriendNotifSuccess);
	AB_TEST_EQUAL(RejectFriendNotifResponse.userId, UserCreds[1].GetUserId());

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	Lobbies[1]->ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_FALSE(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	bListOutgoingFriendSuccess = false;
	bListOutgoingFriendError = false;
	Lobbies[0]->ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_FALSE(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Request_Cancel, "AccelByte.Tests.Lobby.B.FriendRequestCancel", AutomationFlagMaskLobby);
bool LobbyTestFriends_Request_Cancel::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[1]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[0]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	Lobbies[0]->SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	Lobbies[1]->SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	Lobbies[0]->SetCancelFriendsResponseDelegate(CancelFriendDelegate);

	Lobbies[0]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	Lobbies[1]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	Lobbies[1]->SetOnCancelFriendsNotifDelegate(CancelFriendNotifDelegate);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	Lobbies[0]->ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	Lobbies[1]->ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[0]->CancelFriendRequest(UserCreds[1].GetUserId());

	WaitUntil(bCancelFriendSuccess, "Cancelling Friend Request...");
	AB_TEST_FALSE(bCancelFriendError);

	WaitUntil(bCancelFriendNotifSuccess, "Getting Cancel Friend Notif...");
	AB_TEST_TRUE(bCancelFriendNotifSuccess);
	AB_TEST_EQUAL(CancelFriendNotifResponse.userId, UserCreds[0].GetUserId());

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	Lobbies[1]->ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_FALSE(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));
	}

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[0]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	}

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Complete_Scenario, "AccelByte.Tests.Lobby.B.FriendComplete", AutomationFlagMaskLobby);
bool LobbyTestFriends_Complete_Scenario::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[1]->SetOnIncomingRequestFriendsNotifDelegate(OnIncomingRequestNotifDelegate);

	Lobbies[0]->SetOnFriendRequestAcceptedNotifDelegate(OnRequestAcceptedNotifDelegate);

	Lobbies[0]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[1]->SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	Lobbies[0]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	Lobbies[0]->SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	Lobbies[1]->SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	Lobbies[0]->SetCancelFriendsResponseDelegate(CancelFriendDelegate);

	Lobbies[1]->SetRejectFriendsResponseDelegate(RejectFriendDelegate);

	Lobbies[1]->SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	Lobbies[0]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	Lobbies[0]->SetUnfriendResponseDelegate(UnfriendDelegate);

	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	Lobbies[0]->ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	Lobbies[0]->CancelFriendRequest(UserCreds[1].GetUserId());

	WaitUntil(bCancelFriendSuccess, "Cancelling Friend Request...");
	AB_TEST_FALSE(bCancelFriendError);

	bListOutgoingFriendSuccess = false;
	bListOutgoingFriendError = false;
	Lobbies[0]->ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	if (listOutgoingFriendResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	}

	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	Lobbies[1]->ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	Lobbies[1]->RejectFriend(UserCreds[0].GetUserId());

	WaitUntil(bRejectFriendSuccess, "Rejecting Friend Request...");
	AB_TEST_FALSE(bRejectFriendError);

	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);
	WaitUntil([&]()
	{
		return bOnIncomingRequestNotifSuccess;
	}, "Waiting for Incoming Friend Request...", 30);
	AB_TEST_FALSE(bOnIncomingRequestNotifError);
	AB_TEST_EQUAL(requestFriendNotifResponse.friendId, UserCreds[0].GetUserId());

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	Lobbies[1]->ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend Request...");
	AB_TEST_FALSE(bAcceptFriendError);
	WaitUntil([&]()
	{
		return bOnRequestAcceptedNotifSuccess;
	}, "Waiting for Accepted Friend Request...", 30);
	AB_TEST_FALSE(bOnRequestAcceptedNotifError);
	AB_TEST_EQUAL(acceptFriendNotifResponse.friendId, UserCreds[1].GetUserId());

	Lobbies[0]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Friend);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());
	
	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Friend);

	Lobbies[0]->Unfriend(UserCreds[1].GetUserId());

	WaitUntil(bUnfriendSuccess, "Waiting Unfriend...");
	AB_TEST_FALSE(bUnfriendError);

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[0]->LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	}

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_BulkFriendRequest, "AccelByte.Tests.Lobby.B.FriendBulkFriendRequest", AutomationFlagMaskLobby);
bool LobbyTestFriends_BulkFriendRequest::RunTest(const FString& Parameters)
{
	FAccelByteModelsBulkFriendsRequest FriendUserIds;
	for (int i = 1; i < TestUserCount; i++)
	{
		FriendUserIds.FriendIds.Add(UserIds[i]);
	}

	bool bBulkAddFriendSuccess = false;
	Lobbies[0]->BulkFriendRequest(FriendUserIds, FVoidHandler::CreateLambda([&bBulkAddFriendSuccess]()
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Bulk Add Friend Success!"));
		bBulkAddFriendSuccess = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bBulkAddFriendSuccess, "Waiting Bulk Add Friend...");
	
	LobbyConnect(1);

	Lobbies[0]->SetLoadFriendListResponseDelegate(LoadFriendListDelegate);
	Lobbies[0]->LoadFriendsList();
	WaitUntil(bLoadFriendListSuccess, "Waiting Load Friend List...!");

	Lobbies[0]->SetUnfriendResponseDelegate(UnfriendDelegate);

	for (auto FriendId : FriendUserIds.FriendIds)
	{
		AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(FriendId));
		Lobbies[0]->Unfriend(FriendId);
		WaitUntil(bUnfriendSuccess, "Waiting Unfriend...");
		AB_TEST_FALSE(bUnfriendError);
		bUnfriendError = false;
		bUnfriendSuccess = false;
	}

	LobbyDisconnect(1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_BulkFriendRequest_AddSelfUserId_Failed, "AccelByte.Tests.Lobby.B.FriendBulkFriendRequest_Failed", AutomationFlagMaskLobby);
bool LobbyTestFriends_BulkFriendRequest_AddSelfUserId_Failed::RunTest(const FString& Parameters)
{
	FAccelByteModelsBulkFriendsRequest FriendUserIds;
	FriendUserIds.FriendIds.Add(UserIds[0]);

	bool bBulkAddFriendSuccess = false;
	bool bBulkAddFriendError = false;
	bool bBulkAddFriendDone = false;
	Lobbies[0]->BulkFriendRequest(FriendUserIds, FVoidHandler::CreateLambda([&bBulkAddFriendSuccess, &bBulkAddFriendDone]()
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Bulk Add Friend Success!"));
		bBulkAddFriendSuccess = true;
		bBulkAddFriendDone = true;
	}), FErrorHandler::CreateLambda([&bBulkAddFriendDone, &bBulkAddFriendError](int32 Code, const FString& Message)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Error! Code: %d | Message: %s"), Code, *Message);
		bBulkAddFriendError = true;
		bBulkAddFriendDone = true;
	}));
	FlushHttpRequests();
	WaitUntil(bBulkAddFriendDone, "Waiting Bulk Add Friend...");

	AB_TEST_TRUE(bBulkAddFriendError);
	AB_TEST_FALSE(bBulkAddFriendSuccess);

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestPlayer_BlockPlayer, "AccelByte.Tests.Lobby.B.Player.BlockPlayer", AutomationFlagMaskLobby);
bool LobbyTestPlayer_BlockPlayer::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	// Set Block Player Delegates
	Lobbies[0]->SetBlockPlayerResponseDelegate(BlockPlayerDelegate);

	Lobbies[1]->SetBlockPlayerNotifDelegate(BlockPlayerNotifDelegate);

	Lobbies[0]->SetUnblockPlayerResponseDelegate(UnblockPlayerDelegate);

	Lobbies[1]->SetUnblockPlayerNotifDelegate(UnblockPlayerNotifDelegate);

	Lobbies[0]->GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);

	WaitUntil(bListBlockedUserListSuccess, "Getting List of Blocked User for Lobby 0...");
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	auto CurrentListBlockedUser = listBlockedUserResponse;
	
	Lobbies[0]->BlockPlayer(UserCreds[1].GetUserId());
	WaitUntil(bBlockPlayerSuccess, "Player 0 Blocks Player 1...");
	AB_TEST_FALSE(bBlockPlayerError);

	bListBlockedUserListSuccess = false;
	FBlockedData BlockedUserData;
	BlockedUserData.BlockedUserId = UserCreds[1].GetUserId();
	Lobbies[0]->GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);
	WaitUntil(bListBlockedUserListSuccess, "Checking if Player 1 is in Player 0 Block list.");
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	bool bFound = false;
	for (auto ResponseData : listBlockedUserResponse.Data)
	{
		if (ResponseData.BlockedUserId.Equals(BlockedUserData.BlockedUserId))
		{
			bFound = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Blocked User Found in the List"));
		}
	}
	AB_TEST_TRUE(bFound);
	bFound = false;

	FBlockerData BlockerUserData;
	BlockerUserData.UserId = UserCreds[0].GetUserId();
	Lobbies[1]->GetListOfBlockers(ListBlockerDelegate, LobbyTestErrorHandler);
	WaitUntil(bListBlockerListSuccess, "Checking if Player 0 is in Player 1 Blocker list.");
	AB_TEST_TRUE(bListBlockerListSuccess);
	for (auto ResponseData : listBlockerResponse.Data)
	{
		if (ResponseData.UserId.Equals(BlockerUserData.UserId))
		{
			bFound = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Blocker Found in the List"));
		}
	}
	AB_TEST_TRUE(bFound);
	bFound = false;

	bool bRequestFriendAttemptDone = false;
	bool bBlockedFriendRequestPassed = false;
	Lobbies[1]->SetRequestFriendsResponseDelegate(Lobby::FRequestFriendsResponse::CreateLambda([&bRequestFriendAttemptDone, &bBlockedFriendRequestPassed](FAccelByteModelsRequestFriendsResponse Result)
	{
		bRequestFriendAttemptDone = true;
		// Should be changed to something to signify blocked user response, but for now we'll use != 0
		if (Result.Code != "0")
		{
			bBlockedFriendRequestPassed = true;
		}
	}));
	Lobbies[1]->RequestFriend(UserCreds[0].GetUserId());
	WaitUntil(bRequestFriendAttemptDone, "Checking if Player 1 has been blocked from Requesting Friend Request properly.");
	AB_TEST_TRUE(bBlockedFriendRequestPassed);
	bRequestFriendAttemptDone = false;
	bBlockedFriendRequestPassed = false;

	Lobbies[0]->UnblockPlayer(UserCreds[1].GetUserId());
	WaitUntil(bUnblockPlayerSuccess, "Player 0 Unblocks Player 1...");
	AB_TEST_FALSE(bUnblockPlayerError);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestPlayer_BlockPlayerReblockPlayer, "AccelByte.Tests.Lobby.B.Player.BlockReblockPlayer", AutomationFlagMaskLobby);
bool LobbyTestPlayer_BlockPlayerReblockPlayer::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	// Set Block Player Delegates
	Lobbies[0]->SetBlockPlayerResponseDelegate(BlockPlayerDelegate);

	Lobbies[1]->SetBlockPlayerNotifDelegate(BlockPlayerNotifDelegate);

	Lobbies[0]->SetUnblockPlayerResponseDelegate(UnblockPlayerDelegate);

	Lobbies[1]->SetUnblockPlayerNotifDelegate(UnblockPlayerNotifDelegate);

	// Start Test
	Lobbies[0]->GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);

	WaitUntil(bListBlockedUserListSuccess, "Getting List of Blocked User for Lobby 0...");
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	auto CurrentListBlockedUser = listBlockedUserResponse;

	Lobbies[0]->BlockPlayer(UserCreds[1].GetUserId());
	WaitUntil(bBlockPlayerSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bBlockPlayerError);

	bListBlockedUserListSuccess = false;
	FBlockedData BlockedUserData;
	BlockedUserData.BlockedUserId = UserCreds[1].GetUserId();
	Lobbies[0]->GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);
	WaitUntil(bListBlockedUserListSuccess, "Checking if Player 1 is in Player 0 Block list.");
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	bool bFound = false;
	for(auto ResponseData : listBlockedUserResponse.Data)
	{
		if (ResponseData.BlockedUserId.Equals(BlockedUserData.BlockedUserId))
		{
			bFound = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Blocked User Found in the List"));
		}
	}
	AB_TEST_TRUE(bFound);
	bFound = false;

	FBlockerData BlockerUserData;
	BlockerUserData.UserId = UserCreds[0].GetUserId();
	Lobbies[1]->GetListOfBlockers(ListBlockerDelegate, LobbyTestErrorHandler);
	WaitUntil(bListBlockerListSuccess, "Checking if Player 0 is in Player 1 Blocker list.");

	AB_TEST_TRUE(bListBlockerListSuccess);
	for (auto ResponseData : listBlockerResponse.Data)
	{
		if (ResponseData.UserId.Equals(BlockerUserData.UserId))
		{
			bFound = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Blocker User Found in the List"));
		}
	}
	AB_TEST_TRUE(bFound);

	// reblock should result in success
	bBlockPlayerSuccess = false;
	bBlockPlayerNotifSuccess = false;
	bUnblockPlayerSuccess = false;
	Lobbies[0]->BlockPlayer(UserCreds[1].GetUserId());
	WaitUntil(bBlockPlayerSuccess, "Player 0 Blocks Player 1 Again");
	AB_TEST_FALSE(bBlockPlayerError);

	WaitUntil(bBlockPlayerNotifSuccess, "Waiting for block notif");

	Lobbies[0]->UnblockPlayer(UserCreds[1].GetUserId());
	WaitUntil(bUnblockPlayerSuccess, "Player 0 Unblocks Player 1...");
	AB_TEST_FALSE(bUnblockPlayerError);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmaking_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStart", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmaking_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[0]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[0]->SetDsNotifDelegate(DsNotifDelegate);

	Lobbies[1]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[1]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[1]->SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[0] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[0] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[0] = true;
		}
	}));

	Lobbies[1]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[1] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[1] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[1] = true;
		}
	}));

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[1]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);
		
	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Waiting for Create Matchmaking channel...", 60);

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	Lobbies[0]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", 120);
	
	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[1]->SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bDsNotifError);

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmakingCheckCustomPort_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStartCheckCustomPort", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmakingCheckCustomPort_ReturnOk::RunTest(const FString& Parameters)
{
	TArray<FString> customPortNames = { TEXT("custom"), TEXT("custom2") };

	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[0]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[0]->SetDsNotifDelegate(DsNotifDelegate);

	Lobbies[1]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[1]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[1]->SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;


	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[0] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[0] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[0] = true;
		}
	}));

	Lobbies[1]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[1] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[1] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[1] = true;
		}
	}));

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[1]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	Lobbies[0]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", 120);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[1]->SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bDsNotifError);

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);

	// Test custom ports names returned from 
	AB_TEST_FALSE(dsNotice.MatchId.IsEmpty());
	int customPortFoundCount = 0;
	for (auto portName : customPortNames)
	{
		if (dsNotice.Ports.Contains(portName))
		{
			customPortFoundCount++;
		}
	}
	AB_TEST_EQUAL(customPortFoundCount, customPortNames.Num());

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmaking_withPartyAttributes, "AccelByte.Tests.Lobby.B.MatchmakingStartPartyAttributes", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmaking_withPartyAttributes::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[0]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[0]->SetDsNotifDelegate(DsNotifDelegate);

	Lobbies[1]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[1]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[1]->SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	
	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
			matchmakingNotifResponse[0] = result;
			matchMakingNotifNum++;
			bMatchmakingNotifSuccess[0] = true;
			if (result.MatchId.IsEmpty())
			{
				bMatchmakingNotifError[0] = true;
			}
		}));

	Lobbies[1]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
			matchmakingNotifResponse[1] = result;
			matchMakingNotifNum++;
			bMatchmakingNotifSuccess[1] = true;
			if (result.MatchId.IsEmpty())
			{
				bMatchmakingNotifError[1] = true;
			}
		}));

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	Lobbies[1]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	Lobbies[0]->SendInfoPartyRequest();
	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
		{
			bCreateMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
		}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		Lobbies[0]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	
	bCreatePartySuccess = false;
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();
	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	//GameServer SDK / Local DS
	FString ServerName = "ue4SdkTestServerName" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
		FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
		{
			bServerLoginWithClientCredentialsDone = true;
		}),
		LobbyTestErrorHandler);
	WaitUntil(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

	TMap<FString, FString> PartyAttributes;
	PartyAttributes.Add("GameMap", "BasicTutorial");
	PartyAttributes.Add("SomeInt", "100");
	PartyAttributes.Add("Label", "New Island");

	bool canBind = false;
	TSharedRef<FInternetAddr> LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	FString LocalIPStr = LocalIp->IsValid() ? LocalIp->ToString(false) : "";

	bool bRegisterLocalServerToDSMDone = false;
	FRegistry::ServerDSM.RegisterLocalServerToDSM(
		LocalIPStr,
		7777,
		ServerName,
		FVoidHandler::CreateLambda([&bRegisterLocalServerToDSMDone]()
		{
			bRegisterLocalServerToDSMDone = true;
		}),
		LobbyTestErrorHandler);
	WaitUntil(bRegisterLocalServerToDSMDone, "Local DS Register To DSM");

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[0]->SendStartMatchmaking(ChannelName, ServerName, "", TArray<TPair<FString, float>>(), PartyAttributes);
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking(ChannelName, ServerName, "", TArray<TPair<FString, float>>(), PartyAttributes);
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{ 
		return matchMakingNotifNum >= 2; 
	}, "Waiting for Matchmaking Notification...", 120);

	bDsNotifSuccess = false;
	bDsNotifError = false;

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);
	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification 0", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[1]->SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);
	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification 1", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bDsNotifError);

	bool bSessionIdDone = false;
	FString sessionId;
	FRegistry::ServerDSM.GetSessionId(THandler<FAccelByteModelsServerSessionResponse>::CreateLambda([&sessionId, &bSessionIdDone](const FAccelByteModelsServerSessionResponse& result)
	{
		sessionId = result.Session_id;
		bSessionIdDone = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bSessionIdDone, "Waiting GetSessionId");
	
	FAccelByteModelsMatchmakingResult matchResult;
	bool bSessionStatusDone = false;
	FRegistry::ServerMatchmaking.QuerySessionStatus(sessionId, THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([&matchResult, &bSessionStatusDone](const FAccelByteModelsMatchmakingResult& result)
	{
		matchResult = result;
		bSessionStatusDone = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bSessionStatusDone, "Waiting query session status");

	bool bDeregisterLocalServerFromDSMDone = false;
	FRegistry::ServerDSM.DeregisterLocalServerFromDSM(
		ServerName,
		FVoidHandler::CreateLambda([&bDeregisterLocalServerFromDSMDone]()
			{
				bDeregisterLocalServerFromDSMDone = true;
			}),
		LobbyTestErrorHandler);

	WaitUntil(bDeregisterLocalServerFromDSMDone, "Waiting Deregister Local DS From DSM");

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
		{
			bDeleteMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
		}), LobbyTestErrorHandler);

	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);

	for (auto Ally : matchResult.Matching_allies)
	{
		for (auto Party : Ally.Matching_parties)
		{
			for (auto partyAttribute : PartyAttributes)
			{
				FString Value;
				bool bKeyFound = Party.Party_attributes.JsonObject->TryGetStringField(partyAttribute.Key, Value);
				AB_TEST_TRUE(bKeyFound);
				if (bKeyFound)
				{
					AB_TEST_EQUAL(*Value, partyAttribute.Value);
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Party Attribute, Key: %s | Value: %s"), *partyAttribute.Key, *Value);
				}
				else
				{
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Party Attribute, Key: %s | not found"), *partyAttribute.Key);
				}
			}
		}
	}

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmakingExtraAttributes_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStartExtraAttributes", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmakingExtraAttributes_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[0]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[0]->SetDsNotifDelegate(DsNotifDelegate);

	Lobbies[1]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[1]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[1]->SetDsNotifDelegate(DsNotifDelegate);

	FThreadSafeCounter setSessionAttributeSuccessCounter = 0;
	auto SetSessionAttributeResponseHandler = THandler<FAccelByteModelsSetSessionAttributesResponse>::CreateLambda([&setSessionAttributeSuccessCounter](const FAccelByteModelsSetSessionAttributesResponse& result)
	{
		if (result.Code == "0")
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("SetSessionAttribute success!"));
			setSessionAttributeSuccessCounter.Add(1);
		}
		else
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("SetSessionAttribute failed error code %s !"), *result.Code);
		}
	});

	Lobbies[0]->SetSetSessionAttributeDelegate(SetSessionAttributeResponseHandler);
	Lobbies[1]->SetSetSessionAttributeDelegate(SetSessionAttributeResponseHandler);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[0] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[0] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[0] = true;
		}
	}));

	Lobbies[1]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[1] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[1] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[1] = true;
		}
	}));

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[1]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	
	FAllianceRule AllianceRule;
	AllianceRule.min_number = 2;
	AllianceRule.max_number = 2;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 1;

	// Create Matchmaking rule with name mmr that has distance of at least 10 apart.
	FMatchingRule MmrRule;
	MmrRule.attribute = "mmr";
	MmrRule.criteria = "distance";
	MmrRule.reference = 10;
	TArray<FMatchingRule> MatchingRules;
	MatchingRules.Add(MmrRule);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, AllianceRule, MatchingRules,
		FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	Lobbies[0]->SetSessionAttribute("mmr", "120");
	Lobbies[1]->SetSessionAttribute("mmr", "110");

	TArray<FString> ExtraAttributes = { "mmr" };

	Lobbies[0]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies, TMap<FString, FString>(), TArray<FString>(), ExtraAttributes);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies, TMap<FString, FString>(), TArray<FString>(), ExtraAttributes);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", 120);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[1]->SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bDsNotifError);

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);
	AB_TEST_EQUAL(setSessionAttributeSuccessCounter.GetValue(), 2);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmakingAllParams_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStartAllParameters", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmakingAllParams_ReturnOk::RunTest(const FString& Parameters)
{
	// Arrange, create matchmaking channels
	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	
	FAllianceRule AllianceRule;
	AllianceRule.min_number = 2;
	AllianceRule.max_number = 2;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 1;

	FMatchingRule MmrRule;
	MmrRule.attribute = "mmr";
	MmrRule.criteria = "distance";
	MmrRule.reference = 10;
	TArray<FMatchingRule> MatchingRules;
	MatchingRules.Add(MmrRule);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, AllianceRule, MatchingRules,
		FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	// Arrange, set party attribute to be passed to start matchmaking function
	const TMap<FString, FString> partyAttribute ({{"map", "ffarena"}, {"difficulty", "hard"}});

	// Arrange Lobby delegates
	LobbyConnect(1);
	bool bMatchmakingNotifReceived = false;
	
	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);
	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	Lobbies[0]->SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&bMatchmakingNotifReceived](const FAccelByteModelsMatchmakingNotice& Result){bMatchmakingNotifReceived = true;}));

	// Arrange, make sure lobby not in party.
	Lobbies[0]->SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "waiting 0 leave party");

	// Arrange, lobby set session attribute
	bool bSetSessionAttributeFinish = false;
	bool bSetSessionAttributeError = false;
	Lobbies[0]->SetSetSessionAttributeDelegate(THandler<FAccelByteModelsSetSessionAttributesResponse>::CreateLambda([&bSetSessionAttributeFinish, &bSetSessionAttributeError](const FAccelByteModelsSetSessionAttributesResponse& result)
	{
		if (result.Code == "0")
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("SetSessionAttribute success!"));
			bSetSessionAttributeFinish = true;
		}
		else
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("SetSessionAttribute failed error code %s !"), *result.Code);
			bSetSessionAttributeError = true;
		}
	}));

	Lobbies[0]->SetSessionAttribute("mmr", "120");
	WaitUntil(bSetSessionAttributeFinish, "Waiting set session attribute");

	// Arrange, extra attribute to be passed to start matchmaking function
	TArray<FString> ExtraAttributes = { "mmr" };

	// ACT
	Lobbies[0]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies, partyAttribute, TArray<FString>({UserIds[0]}), ExtraAttributes);
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");

	WaitUntil(bMatchmakingNotifReceived, "Wait Matchmaking notif received", 120); // match timeout in backend is hardcoded to 2 min

	// Asserts
	AB_TEST_FALSE(bStartMatchmakingError);

	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmaking_Timeout, "AccelByte.Tests.Lobby.B.MatchmakingStartTimeout", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmaking_Timeout::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[0]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[0]->SetDsNotifDelegate(DsNotifDelegate);

	Lobbies[1]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[1]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[1]->SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	bool bMatchmakingNotifTimeout[2] = { false };
	int matchMakingNotifNum = 0;
	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[0] = result;
		if (result.Status == EAccelByteMatchmakingStatus::Done)
		{
			matchMakingNotifNum++;
		}
		if (result.Status == EAccelByteMatchmakingStatus::Timeout)
		{
			bMatchmakingNotifTimeout[0] = true;
		}
		bMatchmakingNotifSuccess[0] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[0] = true;
		}
	}));

	Lobbies[1]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[1] = result;
		if (result.Status == EAccelByteMatchmakingStatus::Done)
		{
			matchMakingNotifNum++;
		}
		if (result.Status == EAccelByteMatchmakingStatus::Timeout)
		{
			bMatchmakingNotifTimeout[1] = true;
		}
		bMatchmakingNotifSuccess[1] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[1] = true;
		}
	}));

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[1]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	Lobbies[0]->SendStartMatchmaking(ChannelName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil(bMatchmakingNotifTimeout[0], "Waiting for matchmaking timeout...", 300);

	AB_TEST_TRUE(bMatchmakingNotifError[0]);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[0]->SendStartMatchmaking(ChannelName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking(ChannelName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", 120);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[1]->SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bDsNotifError);

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmakingLatencies_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingLatenciesStart", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmakingLatencies_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[0]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[1]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[1]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	bool bIsTheRightRegion[2] = { false };
	bool bLatencyDsNotifSuccess[2] = { false };
	bool bLatencyDsNotifError[2] = { false };
	TArray<TPair<FString, float>> Latencies;
	Lobbies[0]->SetDsNotifDelegate(Api::Lobby::FDsNotif::CreateLambda([&Latencies, &bIsTheRightRegion, &bLatencyDsNotifSuccess, &bLatencyDsNotifError](FAccelByteModelsDsNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get DS Notice!"));
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DS ID: %s | Message: %s | Status: %s | Region: %s"), *result.MatchId, *result.Message, *result.Status, *result.Region);
		dsNotice = result;
		if (dsNotice.Status == "READY" || dsNotice.Status == "BUSY")
		{
			bLatencyDsNotifSuccess[0] = true;
		}
		if (result.MatchId.IsEmpty())
		{
			bLatencyDsNotifError[0] = true;
		}
		if (result.Region == Latencies[0].Key)
		{
			bIsTheRightRegion[0] = true;
		}
	}));

	Lobbies[1]->SetDsNotifDelegate(Api::Lobby::FDsNotif::CreateLambda([&Latencies, &bIsTheRightRegion, &bLatencyDsNotifSuccess, &bLatencyDsNotifError](FAccelByteModelsDsNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get DS Notice!"));
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DS ID: %s | Message: %s | Status: %s | Region: %s"), *result.MatchId, *result.Message, *result.Status, *result.Region);
		dsNotice = result;
		if (dsNotice.Status == "READY" || dsNotice.Status == "BUSY")
		{
			bLatencyDsNotifSuccess[1] = true;
		}
		if (result.MatchId.IsEmpty())
		{
			bLatencyDsNotifError[1] = true;
		}
		if (result.Region == Latencies[0].Key)
		{
			bIsTheRightRegion[1] = true;
		}
	}));

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[0] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[0] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[0] = true;
		}
	}));

	Lobbies[1]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[1] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[1] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[1] = true;
		}
	}));

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[1]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	bool bGetServerLatenciesSuccess = false;
	FRegistry::Qos.GetServerLatencies(THandler<TArray<TPair<FString, float>>>::CreateLambda([&bGetServerLatenciesSuccess, &Latencies](const TArray<TPair<FString, float>>& Result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Server Latencies Success! Count: %d"), Result.Num());
		bGetServerLatenciesSuccess = true;
		Latencies = Result;
		Latencies.Sort(LatenciesPredicate);
	}), LobbyTestErrorHandler);
	WaitUntil([&]()
	{
		return bGetServerLatenciesSuccess;
	}, "Waiting for get server latencies...", 60);
	AB_TEST_TRUE(bGetServerLatenciesSuccess);
	AB_TEST_TRUE(Latencies.Num() > 0);

	for (int i = 0; i < Latencies.Num(); i++)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Latencies Region: %s %f"), *Latencies[i].Key, Latencies[i].Value);
	}

	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Selected Region: %s"), *Latencies[0].Key);
	Lobbies[0]->SendStartMatchmaking(ChannelName, "", "", { Latencies[0]});

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking(ChannelName, "", "", { Latencies[0] });

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", 120);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[1]->SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bLatencyDsNotifSuccess[0];
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bLatencyDsNotifError[0]);

	WaitUntil([&]()
	{
		return bLatencyDsNotifSuccess[1];
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bLatencyDsNotifError[1]);

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bIsTheRightRegion[0]);
	AB_TEST_TRUE(bIsTheRightRegion[1]);
	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmakingTempParty_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStartTempParty", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmakingTempParty_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(2);
	bDsNotifSuccess = false;
	Lobbies[0]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[0]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[0]->SetDsNotifDelegate(DsNotifDelegate);

	Lobbies[1]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[1]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[1]->SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));

		if(result.Status == EAccelByteMatchmakingStatus::Done)
		{
			matchmakingNotifResponse[0] = result;
			matchMakingNotifNum++;
			bMatchmakingNotifSuccess[0] = true;

			if (result.MatchId.IsEmpty())
			{
				bMatchmakingNotifError[0] = true;
			}
		}
	}));

	Lobbies[1]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));

		if(result.Status == EAccelByteMatchmakingStatus::Done)
		{
			matchmakingNotifResponse[1] = result;
			matchMakingNotifNum++;
			bMatchmakingNotifSuccess[1] = true;
		}
	}));

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	Lobbies[1]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);
	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[0]->SendStartMatchmaking(ChannelName, TArray<FString>
	{UserCreds[0].GetUserId()});
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking(ChannelName, TArray<FString>
	{UserCreds[1].GetUserId()});
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&matchMakingNotifNum]()
		{ 
			return matchMakingNotifNum == 2; 
		}, "Wait matchmaking notifs all arrived", 60);

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[1]->SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bDsNotifError);

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);
	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmakingTempPartyOfTwo_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStartTempPartyOfTwo", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmakingTempPartyOfTwo_ReturnOk::RunTest(const FString& Parameters)
{
	const int UserNum = 4;
	LobbyConnect(UserNum);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[UserNum];
	bool bMatchmakingNotifSuccess[UserNum] = { false };
	bool bMatchmakingNotifError[UserNum] = { false };
	int matchmakingNotifDone = 0;
	for (int i = 0; i < UserNum; i++)
	{
		Lobbies[i]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

		Lobbies[i]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

		Lobbies[i]->SetDsNotifDelegate(DsNotifDelegate);

		Lobbies[i]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&, i](FAccelByteModelsMatchmakingNotice result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
			matchmakingNotifResponse[i] = result;
			if (result.Status == EAccelByteMatchmakingStatus::Done)
			{
				matchmakingNotifDone++;
				bMatchmakingNotifSuccess[i] = true;
			}
		}));

		Lobbies[i]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	}

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FAllianceRule AllianceRule;
	AllianceRule.min_number = 2;
	AllianceRule.max_number = 2;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 2;

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);
	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	DelaySeconds(5, "waiting for 5 sec");

	Lobbies[0]->SendStartMatchmaking(ChannelName, TArray<FString>
	{UserCreds[0].GetUserId(), UserCreds[1].GetUserId()});

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[2]->SendStartMatchmaking(ChannelName, TArray<FString>
	{UserCreds[2].GetUserId(), UserCreds[3].GetUserId()});

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchmakingNotifDone >= UserNum;
	}, "Waiting for Matchmaking Notification...", 120);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[UserNum];
	for (int i = 0; i < UserNum; i++)
	{
		Lobbies[i]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

		WaitUntil([&]()
		{
			return bReadyConsentNotifSuccess;
		}, "Waiting for Ready Consent Notification...", 30);

		AB_TEST_FALSE(bReadyConsentNotifError);
		readyConsentNoticeResponse[i] = readyConsentNotice;

		bReadyConsentNotifSuccess = false;
		bReadyConsentNotifError = false;
	}

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bDsNotifError);

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);
	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	for (int i = 0; i < UserNum; i++)
	{
		AB_TEST_FALSE(bMatchmakingNotifError[i]);
		AB_TEST_FALSE(matchmakingNotifResponse[i].MatchId.IsEmpty());
		AB_TEST_EQUAL(matchmakingNotifResponse[i].Status, EAccelByteMatchmakingStatus::Done);
		AB_TEST_EQUAL(readyConsentNoticeResponse[i].MatchId, matchmakingNotifResponse[i].MatchId);
	}

	LobbyDisconnect(UserNum);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestCancelMatchmaking_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingCancel", AutomationFlagMaskLobby);
bool LobbyTestCancelMatchmaking_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(1);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[0]->SetCancelMatchmakingResponseDelegate(CancelMatchmakingDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse;
	bool bMatchmakingNotifSuccess = false;
	bool bMatchmakingNotifError = false;
	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse = result;
		bMatchmakingNotifSuccess = true;
		if (result.Status != EAccelByteMatchmakingStatus::Cancel)
		{
			bMatchmakingNotifError = true;
		}
	}));

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	Lobbies[0]->SendStartMatchmaking("test");

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	Lobbies[0]->SendCancelMatchmaking("test");

	WaitUntil(bCancelMatchmakingSuccess, "Cancelling Matchmaking...");
	AB_TEST_FALSE(bCancelMatchmakingError);


	/*Waiting(bMatchmakingNotifSuccess, "Waiting or Matchmaking Notification...");
	AB_TEST_TRUE(!bMatchmakingNotifError);
	AB_TEST_TRUE(matchmakingNotifResponse.Status == EAccelByteMatchmakingStatus::Cancel);*/

	DelaySeconds(30, "wait 30 second to make sure matchmaking process finishes");

	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestCancelMatchmakingTempParty_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingCancelTempParty", AutomationFlagMaskLobby);
bool LobbyTestCancelMatchmakingTempParty_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(1);

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[0]->SetCancelMatchmakingResponseDelegate(CancelMatchmakingDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse;
	bool bMatchmakingNotifSuccess = false;
	bool bMatchmakingNotifError = false;
	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse = result;
		bMatchmakingNotifSuccess = true;
		if (result.Status != EAccelByteMatchmakingStatus::Cancel)
		{
			bMatchmakingNotifError = true;
		}
	}));

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FAllianceRule AllianceRule;
	AllianceRule.min_number = 2;
	AllianceRule.max_number = 2;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 1;

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);
	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	Lobbies[0]->SendStartMatchmaking(ChannelName, TArray<FString>{UserCreds[0].GetUserId()});

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	Lobbies[0]->SendCancelMatchmaking(ChannelName, true);

	WaitUntil(bCancelMatchmakingSuccess, "Cancelling Matchmaking...");
	AB_TEST_FALSE(bCancelMatchmakingError);

	/*Waiting(bMatchmakingNotifSuccess, "Waiting or Matchmaking Notification...");
	AB_TEST_TRUE(!bMatchmakingNotifError);
	AB_TEST_TRUE(matchmakingNotifResponse.Status == EAccelByteMatchmakingStatus::Cancel);*/

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);
	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	DelaySeconds(30, "wait 30 second to make sure matchmaking process finishes");

	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestReMatchmaking_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingRematch", AutomationFlagMaskLobby);
bool LobbyTestReMatchmaking_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(3);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[3];
	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[3];
	bool bMatchmakingNotifSuccess[3] = { false };
	bool bMatchmakingNotifError[3] = { false };
	int matchMakingNotifNum = 0;
	int rematchmakingNotifNum = 0;

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[0]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby 0 Get Matchmaking Notification!"));
		matchmakingNotifResponse[0] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[0] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[0] = true;
		}
	}));

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[0]->SetRematchmakingNotifDelegate(Api::Lobby::FRematchmakingNotif::CreateLambda([&](FAccelByteModelsRematchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby 0 Get Rematchmaking Notification!"));
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User %s received: banned for %d secs"), *UserCreds[0].GetUserId(), result.BanDuration);
		rematchmakingNotifNum++;
	}));

	Lobbies[0]->SetDsNotifDelegate(DsNotifDelegate);

	Lobbies[1]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[1]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[1]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby 1 Get Matchmaking Notification!"));
		matchmakingNotifResponse[1] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[1] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[1] = true;
		}
	}));

	Lobbies[1]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[1]->SetRematchmakingNotifDelegate(Api::Lobby::FRematchmakingNotif::CreateLambda([&](FAccelByteModelsRematchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby 1 Get Rematchmaking Notification!"));
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User %s received: banned for %d secs"), *UserCreds[1].GetUserId(), result.BanDuration);
		rematchmakingNotifNum++;
	}));

	Lobbies[1]->SetDsNotifDelegate(DsNotifDelegate);

	Lobbies[2]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[2]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[2]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[2]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[2]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[2]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby 2 Get Matchmaking Notification!"));
		matchmakingNotifResponse[2] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[2] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[2] = true;
		}
	}));

	Lobbies[2]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[2]->SetRematchmakingNotifDelegate(Api::Lobby::FRematchmakingNotif::CreateLambda([&](FAccelByteModelsRematchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby 2 Get Rematchmaking Notification!"));
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User %s received: banned for %d secs"), *UserCreds[2].GetUserId(), result.BanDuration);
		rematchmakingNotifNum++;
	}));

	Lobbies[2]->SetDsNotifDelegate(DsNotifDelegate);

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[2]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[2]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[2]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	Lobbies[0]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Lobby 0 Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Lobby 1 Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", 120);

	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	WaitUntil([&]()
	{
		return rematchmakingNotifNum >= 2;
	}, "Waiting for Rematchmaking Notification...", 60);
	AB_TEST_EQUAL(rematchmakingNotifNum, 2);

	matchMakingNotifNum = 0;

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[2]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Lobby 2 Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", 120);
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[2].Status, EAccelByteMatchmakingStatus::Done);

	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[2]->SendReadyConsentRequest(matchmakingNotifResponse[2].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	readyConsentNoticeResponse[2] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bDsNotifError);

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[2]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[2].MatchId.IsEmpty());

	LobbyDisconnect(3);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestLocalDSWithMatchmaking_ReturnOk, "AccelByte.Tests.Lobby.B.LocalDSWithMatchmaking", AutomationFlagMaskLobby);
bool LobbyTestLocalDSWithMatchmaking_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[0]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[0]->SetDsNotifDelegate(DsNotifDelegate);

	Lobbies[1]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[1]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[1]->SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	Lobbies[1]->SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	Lobbies[1]->SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	Lobbies[0]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[0] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[0] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[0] = true;
		}
	}));

	Lobbies[1]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse[1] = result;
		matchMakingNotifNum++;
		bMatchmakingNotifSuccess[1] = true;
		if (result.MatchId.IsEmpty())
		{
			bMatchmakingNotifError[1] = true;
		}
	}));

	Lobbies[0]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[1]->SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	Lobbies[0]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

	bool bCreateMatchmakingChannelSuccess = false;
	AdminCreateMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Waiting for Create Matchmaking channel...", 60);

	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	Lobbies[0]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	//GameServer SDK / Local DS
	FString ServerName = "ue4SdkTestServerName" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
		FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
		{
			bServerLoginWithClientCredentialsDone = true;
		}),
		LobbyTestErrorHandler);

	WaitUntil(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

	bool canBind = false;
	TSharedRef<FInternetAddr> LocalIp = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, canBind);
	FString LocalIPStr = LocalIp->IsValid() ? LocalIp->ToString(false) : "";

	bool bRegisterLocalServerToDSMDone = false;
	FRegistry::ServerDSM.RegisterLocalServerToDSM(
		LocalIPStr, 
		7777, 
		ServerName, 
		FVoidHandler::CreateLambda([&bRegisterLocalServerToDSMDone]()
		{
			bRegisterLocalServerToDSMDone = true;
		}),
		LobbyTestErrorHandler);

	WaitUntil(bRegisterLocalServerToDSMDone, "Local DS Register To DSM");

	Lobbies[0]->SendStartMatchmaking(ChannelName, ServerName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking(ChannelName, ServerName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return (matchMakingNotifNum == 2);
	}, "Waiting for Matchmaking Notification...", 120);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[1]->SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);
	AB_TEST_FALSE(bDsNotifError);

	bool bDeregisterLocalServerFromDSMDone = false;
	FRegistry::ServerDSM.DeregisterLocalServerFromDSM(
		ServerName,
		FVoidHandler::CreateLambda([&bDeregisterLocalServerFromDSMDone]()
		{
			bDeregisterLocalServerFromDSMDone = true;
		}),
		LobbyTestErrorHandler);

	WaitUntil(bDeregisterLocalServerFromDSMDone, "Waiting Deregister Local DS From DSM");

	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bDeleteMatchmakingChannelSuccess;
	}, "Delete Matchmaking channel...", 30);

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmaking3vs3_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStart3vs3", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmaking3vs3_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(6);

	FThreadSafeCounter readyConsentNoticeCounter = 0;
	auto ReadyConsentNotif = Lobby::FReadyConsentNotif::CreateLambda([&readyConsentNoticeCounter](FAccelByteModelsReadyConsentNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Ready Consent Notice!"));
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User %s is ready for match."), *result.UserId);
		readyConsentNotice = result;
		bReadyConsentNotifSuccess = true;
		readyConsentNoticeCounter.Increment();
		if (result.MatchId.IsEmpty())
		{
			bReadyConsentNotifError = true;
		}
	});

	for (int i = 0; i < 6; i++)
	{
		auto CreateParty = CreatePartyDelegate;
		Lobbies[i]->SetCreatePartyResponseDelegate(CreateParty);
		auto GetInfoParty = GetInfoPartyDelegate;
		Lobbies[i]->SetInfoPartyResponseDelegate(GetInfoParty);
		auto LeaveParty = LeavePartyDelegate;
		Lobbies[i]->SetLeavePartyResponseDelegate(LeaveParty);
		auto ReadyConsentResponse = ReadyConsentResponseDelegate;
		Lobbies[i]->SetReadyConsentResponseDelegate(ReadyConsentResponse);
		Lobbies[i]->SetReadyConsentNotifDelegate(ReadyConsentNotif);
		auto DsNotif = DsNotifDelegate;
		Lobbies[i]->SetDsNotifDelegate(DsNotif);
	}
	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[6];
	bool bMatchmakingNotifSuccess[6] = { false };
	bool bMatchmakingNotifError[6] = { false };
	int matchMakingNotifNum = 0;
	for (int i = 0; i < 6; i++)
	{
		int numb = i;
		Lobbies[i]->SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&, numb](FAccelByteModelsMatchmakingNotice result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User %d Get Matchmaking Notification!"), numb);
			matchmakingNotifResponse[numb] = result;
			matchMakingNotifNum++;
			bMatchmakingNotifSuccess[numb] = true;
			if (result.MatchId.IsEmpty())
			{
				bMatchmakingNotifError[numb] = true;
			}
		}));
		auto StartMatchmakingResponse = StartMatchmakingDelegate;
		Lobbies[i]->SetStartMatchmakingResponseDelegate(StartMatchmakingResponse);
	}
	FString ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	bool bCreateMatchmakingChannelSuccess = false;
	FAllianceRule AllianceRule;
	AllianceRule.min_number = 3;
	AllianceRule.max_number = 3;
	AllianceRule.player_min_number = 1;
	AllianceRule.player_max_number = 1;

	AdminCreateMatchmakingChannel(ChannelName, AllianceRule, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
	{
		bCreateMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil([&]()
	{
		return bCreateMatchmakingChannelSuccess;
	}, "Create Matchmaking channel...", 60);

	for (int i = 0; i < 6; i++)
	{
		Lobbies[i]->SendInfoPartyRequest();
		WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");
		if (!bGetInfoPartyError)
		{
			Lobbies[i]->SendLeavePartyRequest();
			WaitUntil(bLeavePartySuccess, "Leaving Party...");
			Lobbies[i]->SendCreatePartyRequest();
			WaitUntil(bCreatePartySuccess, "Creating Party...");
			AB_TEST_FALSE(bCreatePartyError);
			bLeavePartySuccess = false;
			bLeavePartyError = false;
		}
		else
		{
			Lobbies[i]->SendCreatePartyRequest();
			WaitUntil(bCreatePartySuccess, "Creating Party...");
			AB_TEST_FALSE(bCreatePartyError);
		}
		bGetInfoPartySuccess = false;
		bGetInfoPartyError = false;
		bCreatePartySuccess = false;
		bCreatePartyError = false;
	}
	for (int i = 0; i < 6; i++)
	{
		Lobbies[i]->SendStartMatchmaking(ChannelName, "", "", PreferedLatencies);
		WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
		AB_TEST_FALSE(bStartMatchmakingError);
		bStartMatchmakingSuccess = false;
		bStartMatchmakingError = false;
	}

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 6;
	}, "Waiting for Matchmaking Notification...", 120);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[6];
	for (int i = 0; i < 6; i++)
	{
		Lobbies[i]->SendReadyConsentRequest(matchmakingNotifResponse[i].MatchId);
		WaitUntil([&]()
		{
			return readyConsentNoticeCounter.GetValue() >= 3;
		}, FString::Printf(TEXT("Waiting for Ready Consent Notification... %d"), i), 30);
		AB_TEST_FALSE(bReadyConsentNotifError);
		readyConsentNoticeResponse[i] = readyConsentNotice;
		readyConsentNoticeCounter.Set(0);
		bReadyConsentNotifSuccess = false;
		bReadyConsentNotifError = false;
	}
	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", 30);

	AB_TEST_FALSE(bDsNotifError);
	bool bDeleteMatchmakingChannelSuccess = false;
	AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
	{
		bDeleteMatchmakingChannelSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");

	AB_TEST_TRUE(bCreateMatchmakingChannelSuccess);
	AB_TEST_TRUE(bDeleteMatchmakingChannelSuccess);
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);
	LobbyDisconnect(6);
	ResetResponses();

	return true;
}

bool CompareSessionAttributes(const FJsonObjectWrapper& resultAttributes, TMap<FString, FString>& expected)
{
	for (auto attribute : expected)
	{
		if (!resultAttributes.JsonObject->HasField(attribute.Key))
		{
			UE_LOG(LogAccelByteLobbyTest, Error, TEXT("Session Attribute not found: key %s !"), *attribute.Key);
			return false;
		}

		if (resultAttributes.JsonObject->GetStringField(attribute.Key) != attribute.Value)
		{
			UE_LOG(LogAccelByteLobbyTest, Error, TEXT("Session Attribute Wrong Value: key %s, value %s, expected %s !"), *attribute.Key, *resultAttributes.JsonObject->GetStringField(attribute.Key), *attribute.Value);
			return false;
		}
	}

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSessionAttributesSetGet_Ok, "AccelByte.Tests.Lobby.E.SessionAttributes_Set_Get_Ok", AutomationFlagMaskLobby);
bool LobbyTestSessionAttributesSetGet_Ok::RunTest(const FString& Parameters)
{
	// Temporary disable, will be removed
	if (Environment::GetEnvironmentVariable("UE4_SDK_DISABLE_INTERMITTEN_TEST", 16) == "true")
	{
		return true;
	}

	TMap<FString, FString> sessionAttributes = { {"Difficulty", "Easy"}, {"Rank", "1"} };
	TArray<FString> sessionAttributesKeys = {};
	sessionAttributes.GenerateKeyArray(sessionAttributesKeys);

	// Arrange setup user & lobby
	User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });
	FThreadSafeCounter setSessionAttributeSuccessCounter;
	FAccelByteModelsGetSessionAttributesResponse getSessionAttributesResponse;
	bool bGetSessionAttributeFinish = false;
	FAccelByteModelsGetAllSessionAttributesResponse getAllSessionAttributesResponse;
	bool bGetAllSessionAttributeFinish = false;

	Lobby& Lobby = FRegistry::Lobby;

	bool bSetSessionAttributeFailed = false;
	Lobby.SetSetSessionAttributeDelegate(THandler<FAccelByteModelsSetSessionAttributesResponse>::CreateLambda([&setSessionAttributeSuccessCounter, &bSetSessionAttributeFailed](const FAccelByteModelsSetSessionAttributesResponse& result)
	{
		setSessionAttributeSuccessCounter.Add(1);
		if (result.Code == "0")
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("SetSessionAttribute success!"));
		}
		else
		{
			bSetSessionAttributeFailed = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("SetSessionAttribute failed error code %s !"), *result.Code);
		}
	}));

	Lobby.SetGetSessionAttributeDelegate(THandler<FAccelByteModelsGetSessionAttributesResponse>::CreateLambda([&getSessionAttributesResponse, &bGetSessionAttributeFinish](const FAccelByteModelsGetSessionAttributesResponse& result)
	{
		bGetSessionAttributeFinish = true;
		getSessionAttributesResponse = result;
	}));

	Lobby.SetGetAllSessionAttributeDelegate(THandler<FAccelByteModelsGetAllSessionAttributesResponse>::CreateLambda([&getAllSessionAttributesResponse, &bGetAllSessionAttributeFinish](const FAccelByteModelsGetAllSessionAttributesResponse& result)
	{
		bGetAllSessionAttributeFinish = true;
		getAllSessionAttributesResponse = result;
	}));

	Lobby.Connect();
	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 5);

	// ACT setting session attribute

	for (auto attribute : sessionAttributes)
	{
		Lobby.SetSessionAttribute(attribute.Key, attribute.Value);
	}

	WaitUntil([&setSessionAttributeSuccessCounter, &sessionAttributes]() {return setSessionAttributeSuccessCounter.GetValue() == sessionAttributes.Num(); }, "Waiting set session Attribute", 10);

	Lobby.GetSessionAttribute(sessionAttributesKeys[0]);
	WaitUntil(bGetSessionAttributeFinish, "Wait for get session attribute");

	Lobby.GetAllSessionAttribute();
	WaitUntil(bGetAllSessionAttributeFinish, "Wait for get all session attribute");
	bool bCompareAllSessionAttributes = CompareSessionAttributes(getAllSessionAttributesResponse.attributes, sessionAttributes);

	//Asserts
	AB_TEST_FALSE(bSetSessionAttributeFailed);
	AB_TEST_EQUAL(getSessionAttributesResponse.Value, sessionAttributes[sessionAttributesKeys[0]]);
	AB_TEST_TRUE(bCompareAllSessionAttributes);

	Lobby.Disconnect();

	return true;
}

enum class WebSocketState
{
	None = 0,
	Connecting = 1,
	Open = 2,
	CloseSent = 3,
	CloseReceived = 4,
	Closed = 5,
	Aborted = 6
};

/* commented this because the test flow is not suitable for non apigw release, but still may be useful in the future
* 
IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestTokenRevoked_Disconnected, "AccelByte.Tests.Lobby.C.LobbyTestTokenRevoked_Disconnected", AutomationFlagMaskLobby);
bool LobbyTestTokenRevoked_Disconnected::RunTest(const FString& Parameters)
{
	User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	Lobby& Lobby = FRegistry::Lobby;
	int NumLobbyConnected = 0;
	bool bLobbyDisconnected = false;
	FAccelByteModelsDisconnectNotif DisconnectNotifResponse;
	Lobby.SetConnectSuccessDelegate(
		FSimpleDelegate::CreateLambda([&]()
		{
			NumLobbyConnected++;
		}));
	Lobby.SetConnectionClosedDelegate(
		Lobby::FConnectionClosed::CreateLambda([&](int32 CloseCode, FString Reason, bool bWasClean)
		{
			bLobbyDisconnected = true;
		}));
	Lobby.SetDisconnectNotifDelegate(Lobby::FDisconnectNotif::CreateLambda([&](const FAccelByteModelsDisconnectNotif& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DisconnectNotif: %s"), *Result.Message);
			DisconnectNotifResponse = Result;
		}));

	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, 5);

	bool LogoutDone = false;
	FString UserId = FRegistry::Credentials.GetUserId();
	Oauth2::RevokeToken(FRegistry::Credentials.GetAccessToken(), FVoidHandler::CreateLambda([&]() { LogoutDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return LogoutDone; }, 10);

	WaitUntil([&]() { return bLobbyDisconnected; }, 15);

	AB_TEST_FALSE(Lobby.IsConnected());
	AB_TEST_TRUE(bLobbyDisconnected);
	AB_TEST_EQUAL(NumLobbyConnected, 1);
	AB_TEST_EQUAL(DisconnectNotifResponse.Message, "session deleted");
	AB_TEST_TRUE(LogoutDone);

	Lobby.Disconnect();

	bool bDeleteDone = false;
	DeleteUserById(
		UserId, 
		FVoidHandler::CreateLambda([&bDeleteDone]()
		{
			bDeleteDone = true;
		}), 
		LobbyTestErrorHandler);

	Waiting(bDeleteDone, "Waiting for Deletion...");

	return true;
}
*/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSameUserDifferentToken_Disconnected, "AccelByte.Tests.Lobby.C.LobbyTestSameUserDifferentToken_Disconnected", AutomationFlagMaskLobby);
bool LobbyTestSameUserDifferentToken_Disconnected::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	int NumLobbyConnected = 0;
	bool bLobbyDisconnected = false;
	FAccelByteModelsDisconnectNotif DisconnectNotifResponse;
	Lobby.SetConnectSuccessDelegate(
		FSimpleDelegate::CreateLambda([&]()
		{
			NumLobbyConnected++;
		}));
	Lobby.SetConnectionClosedDelegate(
		Lobby::FConnectionClosed::CreateLambda([&](int32 CloseCode, FString Reason, bool bWasClean)
		{
			bLobbyDisconnected = true;
		}));
	Lobby.SetDisconnectNotifDelegate(Lobby::FDisconnectNotif::CreateLambda([&](const FAccelByteModelsDisconnectNotif& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DisconnectNotif: %s"), *Result.Message);
			DisconnectNotifResponse = Result;
		}));

	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 5);

	User.ForgetAllCredentials();

	DelaySeconds(1, "Waiting 1 sec to make sure different token is generated.");

	bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby OtherLobby{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };

	OtherLobby.Connect();

	WaitUntil([&]() { return OtherLobby.IsConnected(); }, "", 5);

	WaitUntil([&]() { return bLobbyDisconnected; }, "", 15);

	AB_TEST_FALSE(Lobby.IsConnected());
	AB_TEST_TRUE(bLobbyDisconnected);
	AB_TEST_TRUE(OtherLobby.IsConnected());
	AB_TEST_EQUAL(NumLobbyConnected, 1);
	AB_TEST_EQUAL(DisconnectNotifResponse.Message, "multiple session login detected");

	Lobby.Disconnect();
	OtherLobby.Disconnect();

	WaitUntil([&]() { return !OtherLobby.IsConnected() && !Lobby.IsConnected(); }, "", 15);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone]()
	{
		bDeleteDone = true;
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSameUserSameToken_Disconnected, "AccelByte.Tests.Lobby.C.LobbyTestSameUserSameToken_Disconnected", AutomationFlagMaskLobby);
bool LobbyTestSameUserSameToken_Disconnected::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	int NumLobbyConnected = 0;
	bool bLobbyDisconnected = false;
	Lobby.SetConnectSuccessDelegate(
		FSimpleDelegate::CreateLambda([&]()
		{
			NumLobbyConnected++;
		}));
	Lobby.SetConnectionClosedDelegate(
		Lobby::FConnectionClosed::CreateLambda([&](int32 CloseCode, FString Reason, bool bWasClean)
		{
			bLobbyDisconnected = true;
		}));

	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 5);

	AccelByte::Api::Lobby OtherLobby{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };

	OtherLobby.Connect();

	DelaySeconds(10, "waiting if lobby is connecting");

	AB_TEST_TRUE(Lobby.IsConnected());
	AB_TEST_FALSE(bLobbyDisconnected);
	AB_TEST_FALSE(OtherLobby.IsConnected());
	AB_TEST_EQUAL(NumLobbyConnected, 1);

	Lobby.Disconnect();
	OtherLobby.Disconnect();

	WaitUntil([&]() { return !OtherLobby.IsConnected() && !Lobby.IsConnected(); }, "", 15);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone]()
	{
		bDeleteDone = true;
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestReconnect_SameToken_withSessionIdHeader, "AccelByte.Tests.Lobby.C.Reconnect_SameToken_withSessionIdHeader", AutomationFlagMaskLobby);
bool LobbyTestReconnect_SameToken_withSessionIdHeader::RunTest(const FString& Parameters)
{
	//Arrange
	AccelByte::Api::User& User = FRegistry::User;
	
	User.ForgetAllCredentials();

	bool bLoginDone = false;
	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	Lobby.Disconnect();
	WaitUntil(!Lobby.IsConnected(), "Wait make sure initial lobby disconnect");
	
	int NumLobbyConnected = 0;
	Lobby.SetConnectSuccessDelegate(
		FSimpleDelegate::CreateLambda([&]()
			{
				NumLobbyConnected++;
			}));

	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 5);

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", 5);
	
	//Act
	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 5);

	bool bIsLobbyConnected = Lobby.IsConnected();

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", 15);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone]()
		{
			bDeleteDone = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, "Waiting for Deletion...");
	
	AB_TEST_TRUE(bIsLobbyConnected);
	AB_TEST_TRUE(NumLobbyConnected > 1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestAccessTokenRefreshed_SendNewAccessToken, "AccelByte.Tests.Lobby.C.LobbyTestAccessTokenRefreshed_SendNewAccessToken", AutomationFlagMaskLobby);
bool LobbyTestAccessTokenRefreshed_SendNewAccessToken::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;

	bool bRefreshTokenSuccess = false;
	Lobby.SetRefreshTokenDelegate(Api::Lobby::FRefreshTokenResponse::CreateLambda([&bRefreshTokenSuccess](const FAccelByteModelsRefreshTokenResponse& result)
	{
		bRefreshTokenSuccess = true;
	}));
	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 15);

	// set session expired time to 0
	const FString AccessToken = FRegistry::Credentials.GetAccessToken();
	const FString RefreshToken = FRegistry::Credentials.GetRefreshToken();
	FString NewAccessToken = FRegistry::Credentials.GetAccessToken();
	FString NewRefreshToken = FRegistry::Credentials.GetRefreshToken();
	FRegistry::Credentials.ScheduleRefreshToken(FPlatformTime::Seconds() + 2.0);

	WaitUntil(
		[&]()
		{
			NewAccessToken = FRegistry::Credentials.GetAccessToken();
			NewRefreshToken = FRegistry::Credentials.GetRefreshToken();

			return AccessToken != NewAccessToken && RefreshToken != NewRefreshToken;
		},
		"Wait refresh token",
		10);

	WaitUntil(bRefreshTokenSuccess, "waiting refresh token to lobby");

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", 15);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone]()
	{
		bDeleteDone = true;
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	AB_TEST_TRUE(bRefreshTokenSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestAccessTokenRefreshed_SendNewAccessTokenMultipleTime, "AccelByte.Tests.Lobby.C.LobbyTestAccessTokenRefreshed_SendNewAccessToken_MultipleTime", AutomationFlagMaskLobby);
bool LobbyTestAccessTokenRefreshed_SendNewAccessTokenMultipleTime::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;

	bool bRefreshTokenSuccess = false;
	int RefreshTokenSuccessCount = 0;
	Lobby.SetRefreshTokenDelegate(Api::Lobby::FRefreshTokenResponse::CreateLambda([&bRefreshTokenSuccess, &RefreshTokenSuccessCount](const FAccelByteModelsRefreshTokenResponse& result)
	{
		bRefreshTokenSuccess = true;
		RefreshTokenSuccessCount++;
	}));
	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 15);

	// set session expired time to 0
	const FString AccessToken = FRegistry::Credentials.GetAccessToken();
	const FString RefreshToken = FRegistry::Credentials.GetRefreshToken();
	FString NewAccessToken = FRegistry::Credentials.GetAccessToken();
	FString NewRefreshToken = FRegistry::Credentials.GetRefreshToken();
	FRegistry::Credentials.ScheduleRefreshToken(FPlatformTime::Seconds() + 2.0);

	WaitUntil(
		[&]()
		{
			NewAccessToken = FRegistry::Credentials.GetAccessToken();
			NewRefreshToken = FRegistry::Credentials.GetRefreshToken();

			return AccessToken != NewAccessToken && RefreshToken != NewRefreshToken;
		},
		"Wait refresh token",
		10);

	WaitUntil(bRefreshTokenSuccess, "waiting refresh token to lobby");

	// set session expired time to 0 again
	bRefreshTokenSuccess = false;
	const FString AccessToken1 = FRegistry::Credentials.GetAccessToken();
	const FString RefreshToken1 = FRegistry::Credentials.GetRefreshToken();
	FString NewAccessToken1 = FRegistry::Credentials.GetAccessToken();
	FString NewRefreshToken1 = FRegistry::Credentials.GetRefreshToken();
	FRegistry::Credentials.ScheduleRefreshToken(FPlatformTime::Seconds() + 2.0);

	WaitUntil(
		[&]()
		{
			NewAccessToken1 = FRegistry::Credentials.GetAccessToken();
			NewRefreshToken1 = FRegistry::Credentials.GetRefreshToken();

			return AccessToken1 != NewAccessToken1 && RefreshToken1 != NewRefreshToken1;
		},
		"Wait refresh token1",
		10);

	WaitUntil(bRefreshTokenSuccess, "waiting refresh token to lobby1");

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", 15);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone]()
	{
		bDeleteDone = true;
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	AB_TEST_EQUAL(RefreshTokenSuccessCount, 2);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestInvalidTokenRefresh_Failed, "AccelByte.Tests.Lobby.C.LobbyTestAccessTokenRefreshInvalid_Failed", AutomationFlagMaskLobby);
bool LobbyTestInvalidTokenRefresh_Failed::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;

	bool bRefreshTokenSuccess = false;
	FString RefreshTokenCode = "0";
	Lobby.SetRefreshTokenDelegate(Api::Lobby::FRefreshTokenResponse::CreateLambda([&bRefreshTokenSuccess, &RefreshTokenCode](const FAccelByteModelsRefreshTokenResponse& result)
	{
		bRefreshTokenSuccess = true;
		RefreshTokenCode = result.Code;
	}));
	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 15);

	Lobby.RefreshToken("Invalid");

	WaitUntil(bRefreshTokenSuccess, "waiting refresh lobby token");

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", 15);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FVoidHandler::CreateLambda([&bDeleteDone]()
	{
		bDeleteDone = true;
	}), LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	AB_TEST_TRUE(bRefreshTokenSuccess);
	FString NotExpectedCode = "0";
	AB_TEST_NOT_EQUAL(RefreshTokenCode, NotExpectedCode);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSignalingP2P, "AccelByte.Tests.Lobby.D.LobbyTestSignalingP2P", AutomationFlagMaskLobby);
bool LobbyTestSignalingP2P::RunTest(const FString& Parameters)
{
	bool userResponded[2], userConnected[2];
	const FString Message("MESSAGESIGNALINGTEST");
	bool P2PMessageAvailable = false;	
	FString P2PMessage;
	FString P2PDestinationId;
	auto OnP2PDelegate = AccelByte::Api::Lobby::FSignalingP2P::CreateLambda([&P2PMessageAvailable, &P2PMessage, &P2PDestinationId](const FString &UserId, const FString &Message)
	{
		P2PMessageAvailable = true;
		P2PDestinationId = UserId;
		P2PMessage = Message;
	});

	Lobbies[0]->SetConnectSuccessDelegate(ConnectSuccessDelegate);
	Lobbies[1]->SetConnectSuccessDelegate(ConnectSuccessDelegate);
	Lobbies[0]->SetConnectFailedDelegate(ConnectFailedDelegate);
	Lobbies[1]->SetConnectFailedDelegate(ConnectFailedDelegate);

	Lobbies[0]->Connect();

	while (!Lobbies[0]->IsConnected() || !bUsersConnectionSuccess)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LobbyTestSignalingP2P: Wait user 0"));
		FTicker::GetCoreTicker().Tick(.5f);
	}
	userResponded[0] = bUsersConnectionSuccess;
	userConnected[0] = bUsersConnected;
	bUsersConnectionSuccess = false;
	bUsersConnected = false;

	Lobbies[1]->Connect();

	while (!Lobbies[1]->IsConnected() || !bUsersConnectionSuccess)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LobbyTestSignalingP2P: Wait user 1"));
		FTicker::GetCoreTicker().Tick(.5f);
	}
	userResponded[1] = bUsersConnectionSuccess;
	userConnected[1] = bUsersConnected;

	for (int i = 0; i < 2; i++)
	{
		AB_TEST_TRUE(userConnected[i]);
		AB_TEST_TRUE(userResponded[i]);
	}

	Lobbies[0]->SetSignalingP2PDelegate(OnP2PDelegate);
	Lobbies[1]->SetSignalingP2PDelegate(OnP2PDelegate);
	
	Lobbies[0]->SendSignalingMessage(UserCreds[1].GetUserId(), Message);
	WaitUntil([&P2PMessageAvailable]() ->bool
	{
		return P2PMessageAvailable;
	});

	AB_TEST_TRUE(P2PMessageAvailable);
	AB_TEST_EQUAL(P2PMessage, Message);
	AB_TEST_EQUAL(P2PDestinationId, UserCreds[0].GetUserId());

	P2PMessageAvailable = false;
	Lobbies[1]->SendSignalingMessage(UserCreds[0].GetUserId(), Message);
	WaitUntil([&P2PMessageAvailable]() ->bool
	{
		return P2PMessageAvailable;
	});

	AB_TEST_TRUE(P2PMessageAvailable);
	AB_TEST_EQUAL(P2PMessage, Message);
	AB_TEST_EQUAL(P2PDestinationId, UserCreds[1].GetUserId());
	
	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestErrorMessage, "AccelByte.Tests.Lobby.E.LobbyTestErrorHasMessage", AutomationFlagMaskLobby);
bool LobbyTestErrorMessage::RunTest(const FString& Parameters)
{
	// ACT
	// connect lobby
	LobbyConnect(1);
	
	// ARRANGE
	FAccelByteModelsCreatePartyResponse CreatePartyResult;
	bool bCreatePartyDone = false;
	bool bError = false;
	int32 ErrorCode;
	FString ErrorMessage = "";
	Lobbies[0]->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&bCreatePartyDone, &CreatePartyResult](const FAccelByteModelsCreatePartyResponse result)
	{
		CreatePartyResult = result;
		bCreatePartyDone = true;
	}), FErrorHandler::CreateLambda([&bCreatePartyDone, &bError, &ErrorCode, &ErrorMessage](const int32 code, const FString& message)
	{
		bCreatePartyDone = true;
		bError = true;
		ErrorCode = code;
		ErrorMessage = message;
	}));
	
	// create party
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party");

	// crate party again, error
	bCreatePartyDone = false;
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party2");

	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create party2 responded with code %d, codename %s"), ErrorCode, *ErrorMessage);

	// ASSERT error create party has message
	AB_TEST_TRUE(bError);
	AB_TEST_NOT_EQUAL(ErrorCode, 0);
	AB_TEST_FALSE(ErrorMessage.IsEmpty());

	// Cleanup
	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestNoErrorHandlerHasResponseCode, "AccelByte.Tests.Lobby.E.LobbyTestNoErrorHandlerHasResponseCode", AutomationFlagMaskLobby);
bool LobbyTestNoErrorHandlerHasResponseCode::RunTest(const FString& Parameters)
{
	// ACT
	// connect lobby
	LobbyConnect(1);

	// ARRANGE
	FAccelByteModelsCreatePartyResponse CreatePartyResult;
	bool bCreatePartyDone = false;
	Lobbies[0]->SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&bCreatePartyDone, &CreatePartyResult](const FAccelByteModelsCreatePartyResponse result)
	{
		CreatePartyResult = result;
		bCreatePartyDone = true;
	}));
	
	// create party
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party");

	// crate party again, error
	bCreatePartyDone = false;
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party2");

	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create party2 responded with code %s"), *CreatePartyResult.Code);

	// ASSERT error create party has message
	const FString SuccessCode("0");
	AB_TEST_NOT_EQUAL(CreatePartyResult.Code, SuccessCode);

	// Cleanup
	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestErrorMessageEmptyReconnectHasMessage, "AccelByte.Tests.Lobby.E.LobbyTestErrorMessageEmptyReconnectHasMessage", AutomationFlagMaskLobby);
bool LobbyTestErrorMessageEmptyReconnectHasMessage::RunTest(const FString& Parameters)
{
	// ACT
	// connect lobby
	LobbyConnect(1);
	
	// ARRANGE
	FAccelByteModelsCreatePartyResponse CreatePartyResult;
	bool bCreatePartyDone = false;
	bool bError = false;
	int32 ErrorCode;
	FString ErrorMessage = "";
	
	auto CreatePartySuccess = THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&bCreatePartyDone, &CreatePartyResult](const FAccelByteModelsCreatePartyResponse result)
	{
		CreatePartyResult = result;
		bCreatePartyDone = true;
	});
	
	auto CreatePartyError = FErrorHandler::CreateLambda([&bCreatePartyDone, &bError, &ErrorCode, &ErrorMessage](const int32 code, const FString& message)
	{
		bCreatePartyDone = true;
		bError = true;
		ErrorCode = code;
		ErrorMessage = message;
	});
	
	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartySuccess, CreatePartyError);
	
	// create party
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party");

	// Clear error message, then test error creating party
	bCreatePartyDone = false;
	Lobbies[0]->ClearLobbyErrorMessages();
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party2");
	bool bIsErrorAfterClear(bError);
	int32 ErrorCodeAfterClear(ErrorCode);
	FString ErrorMessageAfterClear(ErrorMessage);
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create party after clear error message responded with code %d, codename %s"), ErrorCode, *ErrorMessage);

	DelaySeconds(3, "Delaying 3 sec");
	
	// Disconnect Lobby, reapply setup then try again.
	LobbyDisconnect(1);
	WaitUntil(!Lobbies[0]->IsConnected(), "Waiting lobby disconnect");
	
	LobbyConnect(1);
	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartySuccess, CreatePartyError);
	WaitUntil(Lobbies[0]->IsConnected(), "Wait reconnecting lobby");

	// Create party again after disconnect
	bCreatePartyDone = false;
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party", 3);

	// Create party again to test error after reconnecting
	bCreatePartyDone = false;
	Lobbies[0]->SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party2", 3);
	bool bIsErrorAfterReconnect(bError);
	int32 ErrorCodeAfterReconnect(ErrorCode);
	FString ErrorMessaegAfterReconnect(ErrorMessage);
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create party after reconnect error message responded with code %d, codename %s"), ErrorCode, *ErrorMessage);
	
	// ASSERT error create party has message
	AB_TEST_TRUE(bIsErrorAfterClear);
	AB_TEST_NOT_EQUAL(ErrorCodeAfterClear, 0);
	AB_TEST_FALSE(ErrorMessageAfterClear.IsEmpty());

	AB_TEST_TRUE(bIsErrorAfterReconnect);
	AB_TEST_NOT_EQUAL(ErrorCodeAfterReconnect, 0);
	AB_TEST_FALSE(ErrorMessaegAfterReconnect.IsEmpty());

	AB_TEST_NOT_EQUAL(ErrorMessageAfterClear, ErrorMessaegAfterReconnect);

	// Cleanup
	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestRequestReachBurst, "AccelByte.Tests.Lobby.Y.LobbyTestRequestReachBurst", AutomationFlagMaskLobby);
bool LobbyTestRequestReachBurst::RunTest(const FString& Parameters)
{
	LobbyConnect(1);
	const int32 ShortLimitBurst = 5;
	const int32 HighLimitDuration = 1000000000;

	bool bGetConfigSuccess = false;
	FLobbyModelConfig DefaultConfig;
	AdminGetLobbyConfig(THandler<FLobbyModelConfig>::CreateLambda([&](const FLobbyModelConfig& Response)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Getting config success"));
		bGetConfigSuccess = true;
		DefaultConfig = Response;
	}), LobbyTestErrorHandler);
	WaitUntil(bGetConfigSuccess, "Waiting Get Config...");
	AB_TEST_TRUE(bGetConfigSuccess);

	FLobbyModelConfig ShortBurstConfig = DefaultConfig;
	ShortBurstConfig.GeneralRateLimitBurst = ShortLimitBurst;
	ShortBurstConfig.GeneralRateLimitDuration = HighLimitDuration;

	bool bSetConfigSuccess = false;
	AdminSetLobbyConfig(ShortBurstConfig, THandler<FLobbyModelConfig>::CreateLambda([&](const FLobbyModelConfig& Response)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Setting config success"));
		bSetConfigSuccess = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bSetConfigSuccess, "Waiting Set Config...");
	AB_TEST_TRUE(bSetConfigSuccess);
	
	bool bReachedBurst = false;
	Lobbies[0]->SetErrorNotifDelegate(Lobby::FErrorNotif::CreateLambda([&](int32 Code, const FString& Message)
	{
		// Too many request
		if(Code == 429)
		{
			bReachedBurst = true;
		}
	}));
	int SendRequestCounter = 0;
	while(SendRequestCounter <= ShortLimitBurst)
	{
		Lobbies[0]->SendGetOnlineUsersRequest();
		SendRequestCounter++;
	}
	WaitUntil(bReachedBurst, "Waiting to reach burst limit");
	AB_TEST_TRUE(bReachedBurst);
	
	bSetConfigSuccess = false;
	AdminSetLobbyConfig(DefaultConfig, THandler<FLobbyModelConfig>::CreateLambda([&](const FLobbyModelConfig& Response)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Setting config to default success"));
		bSetConfigSuccess = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bSetConfigSuccess, "Waiting Set Back Default Configuration...");
	AB_TEST_TRUE(bSetConfigSuccess);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLobbyTestFeatureBan, "AccelByte.Tests.Lobby.E.LobbyTestFeatureBan", AutomationFlagMaskLobby);
bool FLobbyTestFeatureBan::RunTest(const FString& Parameter)
{
	//Arrange
	AccelByte::Api::User& User = FRegistry::User;

	User.ForgetAllCredentials();

	//Create User and Login
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("CreateEmailAccount"));
	User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteLobbyTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bRegisterDone, "Waiting for Registered...");

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLobbyConnectionClosed = false;
	bool bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	auto LobbyConnectionClosed = Api::Lobby::FConnectionClosed::CreateLambda([&bLobbyConnectionClosed](int32 CloseCode, FString Reason, bool bWasClean)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby Connection Closed"));
			bLobbyConnectionClosed = true;
		});

	auto LobbyDisconnected = Api::Lobby::FDisconnectNotif::CreateLambda([&bLobbyDisconnected](const FAccelByteModelsDisconnectNotif& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby Disconnected"));
			bLobbyDisconnected = true;
		});

	const FString UserId = FRegistry::Credentials.GetUserId();
	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	Lobby.SetUserBannedNotificationDelegate(UserBannedNotifDelegate);
	Lobby.SetUserUnbannedNotificationDelegate(UserUnbannedNotifDelegate);
	Lobby.SetConnectionClosedDelegate(LobbyConnectionClosed);
	Lobby.SetDisconnectNotifDelegate(LobbyDisconnected);
	Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);
	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 5);

	//Ban
	FBanRequest body =
	{
		EBanType::MATCHMAKING,
		"User Ban Test",
		(FDateTime::Now() + FTimespan::FromSeconds(180)).ToIso8601(),
		EBanReason::IMPERSONATION
	};

	bLobbyConnectionClosed = false;
	bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	FString BanId;
	bool bBanSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("BanUser"));
	AdminBanUser(FRegistry::Credentials.GetUserId(), body, THandler<FBanResponse>::CreateLambda([&bBanSuccessful, &BanId](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			BanId = Result.BanId;
			bBanSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();

	WaitUntil(bBanSuccessful, "Waiting for Ban...");
	WaitUntil(bUserBannedNotif, "Waiting Ban Notification...");

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, "Waiting Lobby Connection Closed...", 120.0);
	bool bUserBannedLobbyConnectionClosed = bLobbyConnectionClosed && !Lobby.IsConnected();

	// Wait Lobby connection auto reconnect
	WaitUntil(bUsersConnected, "Waiting Lobby Reconnected...");
	bool bUserBannedLobbyReconnected = bUsersConnected && Lobby.IsConnected();

	bLobbyConnectionClosed = false;
	bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	//Unban
	bool bUnbanSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("UnbanUser"));
	AdminBanUserChangeStatus(UserId, BanId, false, THandler<FBanResponse>::CreateLambda([&bUnbanSuccessful](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Unbanned: %s"), *Result.UserId);
			bUnbanSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bUnbanSuccessful, "Waiting for Unban...");
	WaitUntil(bUsersUnbannedNotif, "Waiting Unban Notification...");

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, "Waiting Lobby Connection Closed...", 120.0);
	bool bUserUnbannedLobbyConnectionClosed = bLobbyConnectionClosed && !Lobby.IsConnected();

	// Wait Lobby connection auto reconnect
	WaitUntil(bUsersConnected, "Waiting Lobby Reconnected...");
	bool bUserUnbannedLobbyReconnected = bUsersConnected && Lobby.IsConnected();

	bLobbyConnectionClosed = false;
	bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	//Enable Ban
	bool bEnableBanSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("EnablebanUser"));
	AdminBanUserChangeStatus(UserId, BanId, true, THandler<FBanResponse>::CreateLambda([&bEnableBanSuccessful](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			bEnableBanSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bEnableBanSuccessful, "Waiting for Enable Ban...");
	WaitUntil(bUserBannedNotif, "Waiting Ban Notification...");

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, "Waiting Lobby Connection Closed...", 120.0);
	bool bUserBanEnableLobbyConnectionClosed = bLobbyConnectionClosed && !FRegistry::Lobby.IsConnected();

	// Wait Lobby connection auto reconnect
	WaitUntil(bUsersConnected, "Waiting Lobby Reconnected...");
	bool bUserBanEnableLobbyReconnected = bUsersConnected && FRegistry::Lobby.IsConnected();

	//Assert
	AB_TEST_TRUE(bUserBannedLobbyConnectionClosed);
	AB_TEST_TRUE(bUserBannedLobbyReconnected);
	AB_TEST_TRUE(bUserUnbannedLobbyConnectionClosed);
	AB_TEST_TRUE(bUserUnbannedLobbyReconnected);
	AB_TEST_TRUE(bUserBanEnableLobbyConnectionClosed);
	AB_TEST_TRUE(bUserBanEnableLobbyReconnected);

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", 15);
#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(UserId, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	AB_TEST_TRUE(bDeleteSuccessful);

#pragma endregion DeleteUserById


	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLobbyTestAccountBan, "AccelByte.Tests.Lobby.E.LobbyTestAccountBan", AutomationFlagMaskLobby);
bool FLobbyTestAccountBan::RunTest(const FString& Parameter)
{//Arrange
	AccelByte::Api::User& User = FRegistry::User;

	User.ForgetAllCredentials();

	//Create User and Login
	FRegistry::User.ForgetAllCredentials();
	FString DisplayName = "ab" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	FString EmailAddress = "test+u4esdk+" + DisplayName + "@game.test";
	EmailAddress.ToLowerInline();
	FString Password = "123SDKTest123";
	const FString Country = "US";
	const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 25));
	const FString format = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());
	double LastTime = 0;

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("CreateEmailAccount"));
	User.Register(EmailAddress, Password, DisplayName, Country, format, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("   Success"));
			bRegisterSuccessful = true;
			bRegisterDone = true;
		}), FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteLobbyTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bRegisterDone, "Waiting for Registered...");

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, "Waiting for Login...");

	if (!bRegisterSuccessful)
	{
		return false;
	}

	bool bLobbyConnectionClosed = false;
	bool bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	auto LobbyConnectionClosed = Api::Lobby::FConnectionClosed::CreateLambda([&bLobbyConnectionClosed](int32 CloseCode, FString Reason, bool bWasClean)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby Connection Closed"));
			bLobbyConnectionClosed = true;
		});

	auto LobbyDisconnected = Api::Lobby::FDisconnectNotif::CreateLambda([&bLobbyDisconnected](const FAccelByteModelsDisconnectNotif& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby Disconnected"));
			bLobbyDisconnected = true;
		});

	const FString UserId = FRegistry::Credentials.GetUserId();
	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	Lobby.SetUserBannedNotificationDelegate(UserBannedNotifDelegate);
	Lobby.SetUserUnbannedNotificationDelegate(UserUnbannedNotifDelegate);
	Lobby.SetConnectionClosedDelegate(LobbyConnectionClosed);
	Lobby.SetDisconnectNotifDelegate(LobbyDisconnected);
	Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);
	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 5);

	//Ban
	FBanRequest body =
	{
		EBanType::LOGIN,
		"User Ban Test",
		(FDateTime::Now() + FTimespan::FromSeconds(180)).ToIso8601(),
		EBanReason::IMPERSONATION
	};

	bLobbyConnectionClosed = false;
	bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	FString BanId;
	bool bBanSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("BanUser"));
	AdminBanUser(UserId, body, THandler<FBanResponse>::CreateLambda([&bBanSuccessful, &BanId](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			BanId = Result.BanId;
			bBanSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();

	WaitUntil(bBanSuccessful, "Waiting for Ban...");

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, "Waiting Lobby Connection Closed...", 120.0);
	WaitUntil(bLobbyDisconnected, "Waiting Lobby Disconnected...", 120.0);
	bool bUserBannedNotifReceived = bUserBannedNotif;
	bool bUserBannedLobbyDisconnected = bLobbyConnectionClosed && bLobbyDisconnected && !Lobby.IsConnected();

	bLobbyConnectionClosed = false;
	bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	DelaySeconds(10, "Delay 10 sec");

	Lobby.Connect();
	WaitUntil(bUsersConnectionSuccess, "Wait Lobby Connect");
	bool bBannedLobbyConnectFailed = !bUsersConnected;

	//try to relogin
	FRegistry::Credentials.ForgetAll();
	bLoginSuccessful = false;
	bool bLoginError = false;
	bool bLoginDone = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LoginWithUsernameAndPassword"));
	User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
			bLoginDone = true;
		}), FErrorHandler::CreateLambda([&bLoginError, &bLoginDone](int Code, const FString& Message)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
				bLoginError = true;
				bLoginDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bLoginDone, "Waiting for Login...");
	bool bLoginFailedUserBanned = bLoginError;

	//Unban
	bool bUnbanSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("UnbanUser"));
	AdminBanUserChangeStatus(UserId, BanId, false, THandler<FBanResponse>::CreateLambda([&bUnbanSuccessful](const FBanResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Unbanned: %s"), *Result.UserId);
			bUnbanSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bUnbanSuccessful, "Waiting for Unban...");

	//try to relogin
	FRegistry::Credentials.ForgetAll();
	bLoginSuccessful = false;
	bLoginDone = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LoginWithUsernameAndPassword"));
	User.LoginWithUsername(EmailAddress, Password, FVoidHandler::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
			bLoginSuccessful = true;
			bLoginDone = true;
		}), FErrorHandler::CreateLambda([&bLoginDone](int Code, const FString& Message)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
				bLoginDone = true;
			}));

	FlushHttpRequests();
	WaitUntil(bLoginDone, "Waiting for Login...");
	bool bLoginSuccessUnbannedUser = bLoginSuccessful;

	bLobbyConnectionClosed = false;
	bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	Lobby.Connect();
	WaitUntil(bUsersConnectionSuccess, "Wait Lobby Connect");
	bool bUnbannedLobbyConnectSuccess = bUsersConnected;

	//Assert
	AB_TEST_FALSE(bUserBannedNotifReceived);
	AB_TEST_TRUE(bUserBannedLobbyDisconnected);
	AB_TEST_TRUE(bBannedLobbyConnectFailed);
	AB_TEST_TRUE(bLoginFailedUserBanned);
	AB_TEST_TRUE(bLoginSuccessUnbannedUser);
	AB_TEST_TRUE(bUnbannedLobbyConnectSuccess);

	Lobby.Disconnect();

#pragma region DeleteUserById

	bool bDeleteDone = false;
	bool bDeleteSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DeleteUserById"));
	AdminDeleteUser(UserId, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
			bDeleteSuccessful = true;
			bDeleteDone = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, "Waiting for Deletion...");

	AB_TEST_TRUE(bDeleteSuccessful);

#pragma endregion DeleteUserById

	return true;
}
