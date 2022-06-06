// Copyright (c) 2019 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Misc/AutomationTest.h"
#include "JsonObjectWrapper.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Api/AccelByteLobbyApi.h"
#include "Api/AccelByteQos.h"
#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "GameServerApi/AccelByteServerUserApi.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"
#include "LobbyTestAdmin.h"

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::Lobby;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLobbyTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteLobbyTest);

const int32 AutomationFlagMaskLobby = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto LobbyTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteLobbyTest, Error, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const int WaitMatchmakingTime = 125;
const int DsNotifWaitTime = 60;
const int TestUserCount = 6;
const double ShortTimeout = 10.0f;
const double DefaultTimeout = 30.0f;
const double DefaultDeltaTime = .5f;
TArray<FTestUser> LobbyUsers;
TArray<FApiClientPtr> LobbyApiClients;
TArray<TPair<FString, float>> PreferredLatencies;
//General
bool bUsersConnected, bUsersConnectionSuccess, bGetMessage, bGetAllUserPresenceSuccess, bGetFriendsPresenceSuccess;
//Friends
bool bRequestFriendError, bRequestFriendByPublicIdError, bAcceptFriendSuccess, bAcceptFriendError, bRequestFriendSuccess, bRequestFriendByPublicIdSuccess, bRejectFriendSuccess, bRejectFriendError, bCancelFriendSuccess, bCancelFriendError;
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
FAccelByteModelsPartyJoinResponse joinPartyResponse;
FAccelByteModelsPartyGenerateCodeResponse partyGenerateCodeResponse;
FAccelByteModelsPartyGetCodeResponse partyCodeResponse;
FAccelByteModelsPartyRejectResponse rejectPartyResponse;
FAccelByteModelsPartyDataNotif partyDataNotif;
FAccelByteModelsKickPartyMemberResponse kickMemberFromPartyResponse;
FAccelByteModelsPartyInviteResponse partyInviteResponse;
FAccelByteModelsCreatePartyResponse partyCreateResponse;

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

FLobbyModelConfig OriginalLobbyConfig;

void LobbyConnect(int UserCount)
{
	if (UserCount > TestUserCount)
	{
		UserCount = TestUserCount;
	}
	if (UserCount <= 0)
	{
		UserCount = 1;
	}
	for (int Index = 0; Index < UserCount; Index++)
	{
		FApiClientPtr ApiClient = LobbyApiClients[Index];
		if (!ApiClient->Lobby.IsConnected())
		{
			ApiClient->Lobby.Connect();
		}

		WaitUntil([ApiClient]()->bool
				{
					return ApiClient->Lobby.IsConnected();
				}
			, FString::Printf(TEXT("Lobby Connect: Wait user %s to connect"), *ApiClient->CredentialsRef->GetUserId())
			, DefaultTimeout
			, DefaultDeltaTime);
	}
}

void LobbyDisconnect(int UserCount)
{
	if (UserCount > TestUserCount)
	{
		UserCount = TestUserCount;
	}
	if (UserCount <= 0)
	{
		UserCount = 1;
	}
	for (int Index = 0; Index < UserCount; Index++)
	{
		FApiClientPtr ApiClient = LobbyApiClients[Index];
		ApiClient->Lobby.UnbindEvent();
		ApiClient->Lobby.Disconnect();
		WaitUntil([ApiClient]()->bool
				{
					return !ApiClient->Lobby.IsConnected();
				}
			, FString::Printf(TEXT("Lobby Disconnect: Wait user %s to disconnect"), *ApiClient->CredentialsRef->GetUserId())
			, DefaultTimeout
			, DefaultDeltaTime);
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
	bRequestFriendByPublicIdSuccess = false;
	bRequestFriendByPublicIdError = false;
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

const auto RequestFriendByPublicIdDelegate = Api::Lobby::FRequestFriendsResponse::CreateLambda([](FAccelByteModelsRequestFriendsResponse result)
{
	bRequestFriendByPublicIdSuccess = true;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Request Friend by public ID Success!"));
	if (result.Code != "0")
	{
		bRequestFriendByPublicIdError = true;
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
	partyCreateResponse = result;
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


const auto JoinViaCodeDelegate = Api::Lobby::FPartyJoinResponse::CreateLambda([](FAccelByteModelsPartyJoinResponse result)
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
	partyInviteResponse = result;
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

const auto JoinPartyDelegate = Api::Lobby::FPartyJoinResponse::CreateLambda([](FAccelByteModelsPartyJoinResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Join Party Success! Member : %d"), result.Members.Num());
	joinPartyResponse = result;
	bJoinPartySuccess = true;
	if (result.Code != "0")
	{
		bJoinPartyError = true;
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
	kickMemberFromPartyResponse = result;
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
	blockPlayerResponse = result;
	if (result.Code != "0")
	{
		bBlockPlayerError = true;
	}
});

const auto UnblockPlayerDelegate = Api::Lobby::FUnblockPlayerResponse::CreateLambda([](FAccelByteModelsUnblockPlayerResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Unblock Player Success!"));
	bUnblockPlayerSuccess = true;
	unblockPlayerResponse = result;
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
	bool UsersCreationSuccess[TestUserCount];
	bool UsersLoginSuccess[TestUserCount];

	for (int i = 0; i < TestUserCount; i++)
	{
		UsersCreationSuccess[i] = false;
		UsersLoginSuccess[i] = false;
		FTestUser TestUser{i};
		TestUser.Email = FString::Printf(TEXT("lobbyUE4Test+%d-%d@example.com"), i, FMath::RandRange(0, 100000000));
		TestUser.Email.ToLowerInline();
		TestUser.Password = TEXT("123Password123");
		TestUser.DisplayName = FString::Printf(TEXT("lobbyUE4%d"), i);
		TestUser.Country = TEXT("US");
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
		TestUser.DateOfBirth = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

		UsersCreationSuccess[i] = RegisterTestUser(TestUser); 

		UsersLoginSuccess[i] = LoginTestUser(TestUser);

		LobbyUsers.Add(TestUser);
		
		LobbyApiClients.Add(FMultiRegistry::GetApiClient(TestUser.Email));
	}
	
	for (int i = 0; i < TestUserCount; i++)
	{
		AB_TEST_TRUE(UsersCreationSuccess[i]);
		AB_TEST_TRUE(UsersLoginSuccess[i]);
	}

	FString PreferredDSRegion = Environment::GetEnvironmentVariable("PREFERED_DS_REGION", 1000);

	if (!PreferredDSRegion.IsEmpty()) 
	{
		PreferredLatencies = FRegistry::Qos.GetCachedLatencies();
		for (int i = 0; i < PreferredLatencies.Num(); i++)
		{
			if (PreferredLatencies[i].Key == PreferredDSRegion)
			{
				PreferredLatencies[i].Value = 10;
				PreferredLatencies = { PreferredLatencies[i] };
				break;
			}
		}
	}

	// Setup dsm config
	bool bIsUpdateDsmConfig = false;
	bool bGetDsmConfigComplete = false;
	
	FDsmConfig DsmConfig;
	AdminGetLobbyDSMConfig(THandler<FDsmConfig>::CreateLambda(
			[&DsmConfig, &bGetDsmConfigComplete, &bIsUpdateDsmConfig](const FDsmConfig& result)
			{
				DsmConfig = result;

				auto CustomPorts = GetNewCustomPorts();
				for (auto& Port : CustomPorts.Get())
				{
					if (!DsmConfig.Ports.Contains(Port.Key))
					{
						DsmConfig.Ports.Add(Port.Key, Port.Value);
						bIsUpdateDsmConfig = true;
					}
				}

				bGetDsmConfigComplete = true;
			})
		, FErrorHandler::CreateLambda(
			[&DsmConfig, &bIsUpdateDsmConfig, &bGetDsmConfigComplete](int32 ErrorCode, FString ErrorMessage)
			{
				if (ErrorCode == (int)ErrorCodes::DedicatedServerConfigNotFoundException)
				{
					DsmConfig = GetNewDsmConfig().Get();
					bIsUpdateDsmConfig = true;
				}
				else
				{
					LobbyTestErrorHandler.Execute(ErrorCode, ErrorMessage);
				}
				bGetDsmConfigComplete = true;
			})
		);
	WaitUntil(bGetDsmConfigComplete, TEXT("Waiting get dsm config"), DefaultTimeout);

	if (bIsUpdateDsmConfig)
	{
		bool bSetDsmConfigComplete = false;
		AdminSetLobbyDSMConfig(DsmConfig
			, FVoidHandler::CreateLambda([&bSetDsmConfigComplete]()
				{
					bSetDsmConfigComplete = true;
				})
			, LobbyTestErrorHandler);
		WaitUntil(bSetDsmConfigComplete, TEXT("Waiting set dsm config"), DefaultTimeout);
	}

	// update lobby config (set lobby burst limit to 100)
	bool bGetLobbyConfigDone {false};
	AdminGetLobbyConfig(THandler<FLobbyModelConfig>::CreateLambda(
			[&](const FLobbyModelConfig& result)
			{
				bGetLobbyConfigDone = true;
				OriginalLobbyConfig = result;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bGetLobbyConfigDone, TEXT("Waiting fetching original lobby config"), DefaultTimeout);
	
	FLobbyModelConfig NewLobbyConfig {OriginalLobbyConfig};
	NewLobbyConfig.GeneralRateLimitBurst = 100;

	bool bSetLobbyConfigDone {false};
	AdminSetLobbyConfig(NewLobbyConfig
		, THandler<FLobbyModelConfig>::CreateLambda(
			[&bSetLobbyConfigDone](const FLobbyModelConfig& )
			{
				bSetLobbyConfigDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bSetLobbyConfigDone, TEXT("Waiting set new lobby config"), DefaultTimeout);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestTeardown, "AccelByte.Tests.Lobby.Z.Teardown", AutomationFlagMaskLobby);
bool LobbyTestTeardown::RunTest(const FString& Parameters)
{
	LobbyDisconnect(TestUserCount);
	AB_TEST_TRUE(TeardownTestUsers(LobbyUsers));

	bool bSetLobbyConfigDone {false};
	AdminSetLobbyConfig(OriginalLobbyConfig
		, THandler<FLobbyModelConfig>::CreateLambda(
			[&bSetLobbyConfigDone](const FLobbyModelConfig& )
			{
				bSetLobbyConfigDone = true;
			})
		, LobbyTestErrorHandler);
	WaitUntil(bSetLobbyConfigDone, TEXT("Waiting set new lobby config"), DefaultTimeout);

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
	const LobbyMessageTest LobbyMessageArrayWithCommaInFront("type: array\narray: [, foo, bar,]", R"({"type":"array","array":["","foo","bar"]})");
	const LobbyMessageTest LobbyMessageArrayWithDoubleCommaInMiddle("type: array\narray: [, test, , foo, bar,]", R"({"type":"array","array":["","test","","foo","bar"]})");
	const LobbyMessageTest LobbyMessageArrayWithDoubleCommaBeforeTrailingComma("type: array\narray: [, test, , foo, bar, ,]", R"({"type":"array","array":["","test","","foo","bar",""]})");
	const LobbyMessageTest LobbyMessageArrayQuoteComma("type: array\narray: [\"test,string\", foo, bar,]", R"({"type":"array","array":["test,string","foo","bar"]})");
	const LobbyMessageTest LobbyMessageArrayQuoteEscape("type: array\narray: [\"test\\\"string\", foo, bar,]", R"({"type":"array","array":["test\"string","foo","bar"]})");
	const LobbyMessageTest LobbyMessageArrayOfJsonObject("type: object\nexample: [{\"key\":\"value\"},{\"key2\":\"value2\"}]", R"({"type":"object","example":[{"key":"value"},{"key2":"value2"}]})");
	const LobbyMessageTest LobbyMessageObject("type: object\nexample: {\"key\":\"value\"}", R"({"type":"object","example":{"key":"value"}})");
	const LobbyMessageTest LobbyMessageKeyValue("type: keyval\nkey: value\nkey2: value2", R"({"type":"keyval","key":"value","key2":"value2"})");
	const LobbyMessageTest LobbyMessageWithBracket("type: test\nmessage: this is message with[] {bracket}", R"({"type":"test","message":"this is message with[] {bracket}"})");

	TArray<LobbyMessageTest> TestData = {
		LobbyMessageArray,
		LobbyMessageArrayWithTrailingComma,
		LobbyMessageArrayWithCommaInFront,
		LobbyMessageArrayWithDoubleCommaInMiddle,
		LobbyMessageArrayWithDoubleCommaBeforeTrailingComma,
		LobbyMessageArrayQuoteComma,
		LobbyMessageArrayQuoteEscape,
		LobbyMessageObject,
		LobbyMessageKeyValue,
		LobbyMessageWithBracket,
		LobbyMessageArrayOfJsonObject,
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

	LobbyApiClients[0]->Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	LobbyApiClients[1]->Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	LobbyApiClients[0]->Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);
	LobbyApiClients[1]->Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);

	LobbyApiClients[0]->Lobby.Connect();

	WaitUntil([]()->bool
			{
				return LobbyApiClients[0]->Lobby.IsConnected() && bUsersConnectionSuccess;
			}
		, TEXT("LobbyTestConnect2Users: Waiting for user 0 to connect...")
		, DefaultTimeout
		, DefaultDeltaTime);
	userResponded[0] = bUsersConnectionSuccess;
	userConnected[0] = bUsersConnected;
	bUsersConnectionSuccess = false;
	bUsersConnected = false;

	LobbyApiClients[1]->Lobby.Connect();

	WaitUntil([]()->bool
			{
				return LobbyApiClients[1]->Lobby.IsConnected() && bUsersConnectionSuccess;
			}
		, TEXT("LobbyTestConnect2Users: Waiting for user 1 to connect...")
		, DefaultTimeout
		, DefaultDeltaTime);
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
	LobbyApiClients[0]->Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	LobbyApiClients[0]->Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);

	LobbyConnect(1);
	WaitUntil([]()->bool
			{
				return LobbyApiClients[0]->Lobby.IsConnected() && bUsersConnectionSuccess;
			}
		, TEXT("LobbyTestConnectUser: Waiting for user to connect...")
		, DefaultTimeout
		, DefaultDeltaTime);

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

	int ReceivedChatCount = 0;

	LobbyApiClients[0]->Lobby.SetPrivateMessageNotifDelegate(GetMessageDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		FString UserId = LobbyUsers[0].UserId;
		FString ChatMessage = "Hello " + LobbyUsers[0].DisplayName + " from " + LobbyUsers[i].DisplayName;
		bGetMessage = false;
		LobbyApiClients[i]->Lobby.SendPrivateMessage(UserId, ChatMessage);
		FString Text = FString::Printf(TEXT("Wait receiving message : %d"), ReceivedChatCount);
		WaitUntil([&]()
				{
					return bGetMessage;
				}
			, Text
			, DefaultTimeout);

		if (bGetMessage)
		{
			bGetMessage = false;
			ReceivedChatCount++;
		}
	}

	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Received Message : %d"), ReceivedChatCount);
	AB_TEST_TRUE(ReceivedChatCount >= (TestUserCount - 1));

	LobbyDisconnect(TestUserCount);
	ResetResponses();
	return true;
}

/********** 
 * commented channel chat tests since channel chat is broken in BE, will enable when BE is fixed
IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSendChannelChat_FromMultipleUsers_ChatReceived, "AccelByte.Tests.Lobby.B.SendChannelChat", AutomationFlagMaskLobby);
bool LobbyTestSendChannelChat_FromMultipleUsers_ChatReceived::RunTest(const FString& Parameters)
{
	LobbyConnect(TestUserCount);

	int receivedChatCount = 0;
	const int chatMultiplier = 5;
	bool bChatAllReceived = false;

	for (int i = 0; i < TestUserCount; i++)
	{
		LobbyApiClients[i]->Lobby.SetChannelMessageNotifDelegate(THandler<FAccelByteModelsChannelMessageNotice>::CreateLambda([&receivedChatCount, &bChatAllReceived, chatMultiplier](const FAccelByteModelsChannelMessageNotice& result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Payload : %s"), *result.Payload);
			FPlatformAtomics::InterlockedIncrement(&receivedChatCount);
			if (receivedChatCount == TestUserCount * TestUserCount * chatMultiplier)
			{
				bChatAllReceived = true;
			}
		}));

		bool bJoinedChannel = false;
		LobbyApiClients[i]->Lobby.SetJoinChannelChatResponseDelegate(THandler<FAccelByteModelsJoinDefaultChannelResponse>::CreateLambda([&bJoinedChannel](const FAccelByteModelsJoinDefaultChannelResponse& result)
		{
			bJoinedChannel = true;
		}));
		LobbyApiClients[i]->Lobby.SendJoinDefaultChannelChatRequest();
		WaitUntil(bJoinedChannel, TEXT("Waiting for join..."));
	}

	for (int j = 0; j < chatMultiplier; j++)
	{
		for (int i = 0; i < TestUserCount; i++)
		{
			FString chatMessage = "Hello Chat, from " + LobbyUsers[i].DisplayName;
			LobbyApiClients[i]->Lobby.SendChannelMessage(chatMessage);
		}
	}

	FString text = FString::Printf(TEXT("Wait receiving message : %d"), receivedChatCount);
	WaitUntil([&]()
	{
		return bChatAllReceived;
	}, text, DefaultTimeout);

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
		LobbyApiClients[i]->Lobby.SetChannelMessageNotifDelegate(THandler<FAccelByteModelsChannelMessageNotice>::CreateLambda([&receivedChatCount, &bChatAllReceived, playerCount](const FAccelByteModelsChannelMessageNotice& result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Payload : %s"), *result.Payload);
			FPlatformAtomics::InterlockedIncrement(&receivedChatCount);
			if (receivedChatCount == (playerCount * playerCount)-1)
			{
				bChatAllReceived = true;
			}
		}));

		bool bJoinedChannel = false;
		LobbyApiClients[i]->Lobby.SetJoinChannelChatResponseDelegate(THandler<FAccelByteModelsJoinDefaultChannelResponse>::CreateLambda([&bJoinedChannel](const FAccelByteModelsJoinDefaultChannelResponse& result)
		{
			bJoinedChannel = true;
		}));
		LobbyApiClients[i]->Lobby.SendJoinDefaultChannelChatRequest();
		WaitUntil(bJoinedChannel, TEXT("Waiting for join..."));
	}

	FString chatMessage = "Hello Chat, from " + LobbyUsers[0].DisplayName;
	LobbyApiClients[0]->Lobby.SendChannelMessage(chatMessage);
	DelaySeconds(3);

	LobbyApiClients[1]->Lobby.Disconnect();
	DelaySeconds(1);
	LobbyApiClients[1]->Lobby.Connect();
	DelaySeconds(1);

	LobbyApiClients[0]->Lobby.SendChannelMessage(chatMessage);

	FString text = FString::Printf(TEXT("Wait receiving message : %d"), receivedChatCount);
	WaitUntil([&]()
	{
		return bChatAllReceived;
	}, text, DefaultTimeout);


	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Received Message : %d"), receivedChatCount);
	AB_TEST_EQUAL(receivedChatCount, ((playerCount * playerCount) - 1));

	LobbyDisconnect(playerCount);
	ResetResponses();
	return true;
}
end channel chat comment ********************************/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllUsers, "AccelByte.Tests.Lobby.B.ListOnlineFriends", AutomationFlagMaskLobby);
bool LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllUsers::RunTest(const FString& Parameters)
{
	LobbyConnect(TestUserCount);

	LobbyApiClients[0]->Lobby.SetGetAllUserPresenceResponseDelegate(GetAllUsersPresenceDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyApiClients[i]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);
	}

	LobbyApiClients[0]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyApiClients[i]->Lobby.RequestFriend(LobbyUsers[0].UserId);
		FString Text = FString::Printf(TEXT("Requesting Friend %d... "), i);
		WaitUntil(bRequestFriendSuccess, Text, DefaultTimeout);

		LobbyApiClients[0]->Lobby.AcceptFriend(LobbyUsers[i].UserId);
		Text = FString::Printf(TEXT("Accepting Friend %d... "), i);
		WaitUntil(bAcceptFriendSuccess, Text, DefaultTimeout);

		LobbyApiClients[i]->Lobby.SendSetPresenceStatus(Availability::Availabe, TEXT("random activity"));
		bRequestFriendSuccess = false;
		bAcceptFriendSuccess = false;
	}

	LobbyApiClients[0]->Lobby.SendGetOnlineUsersRequest();
	WaitUntil([&]()
			{
				return bGetAllUserPresenceSuccess;
			}
		, TEXT("Getting Friend Status...")
		, DefaultTimeout);

	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyApiClients[i]->Lobby.SendSetPresenceStatus(Availability::Offline, TEXT("disappearing"));
		LobbyApiClients[i]->Lobby.Unfriend(LobbyUsers[0].UserId);
	}

	LobbyDisconnect(TestUserCount);

	AB_TEST_TRUE(bGetAllUserPresenceSuccess);
	AB_TEST_TRUE(onlineUserResponse.friendsId.Num() >= 3);
	AB_TEST_TRUE(onlineUserResponse.friendsId.Contains(LobbyUsers[1].UserId));
	AB_TEST_TRUE(onlineUserResponse.friendsId.Contains(LobbyUsers[2].UserId));
	AB_TEST_TRUE(onlineUserResponse.friendsId.Contains(LobbyUsers[3].UserId));
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestListOnlineFriends_InviteByPublicId_Success, "AccelByte.Tests.Lobby.B.InviteFriendByPublicId", AutomationFlagMaskLobby);
bool LobbyTestListOnlineFriends_InviteByPublicId_Success::RunTest(const FString& Parameters)
{
	bRequestFriendByPublicIdError = false;
	const int LobbyUserCount {2};
	LobbyConnect(LobbyUserCount);
	
	LobbyApiClients[0]->Lobby.SetRequestFriendsByPublicIdResponseDelegate(RequestFriendByPublicIdDelegate);
	LobbyApiClients[0]->Lobby.SetCancelFriendsResponseDelegate(CancelFriendDelegate);

	FAccelByteModelsUserProfileCreateRequest ProfileCreate;
	ProfileCreate.FirstName = "first";
	ProfileCreate.LastName = "last";
	ProfileCreate.Language = "en";
	ProfileCreate.Timezone = "Etc/UTC";
	ProfileCreate.DateOfBirth = "1970-01-01";
	ProfileCreate.AvatarSmallUrl = "http://example.com";
	ProfileCreate.AvatarUrl = "http://example.com";
	ProfileCreate.AvatarLargeUrl = "http://example.com";

	bool bCreateUserProfileDone {false};
	FAccelByteModelsUserProfileInfo ProfileInfo;
	const auto OnCreateProfileSuccess = THandler<FAccelByteModelsUserProfileInfo>::CreateLambda(
		[&bCreateUserProfileDone, &ProfileInfo](const FAccelByteModelsUserProfileInfo& Result)
		{
			ProfileInfo = Result;
			bCreateUserProfileDone = true;
		});
	
	LobbyApiClients[1]->UserProfile.CreateUserProfile(ProfileCreate, OnCreateProfileSuccess, LobbyTestErrorHandler);

	WaitUntil(bCreateUserProfileDone, "Wait creating user profile");
	
	LobbyApiClients[0]->Lobby.RequestFriendByPublicId(ProfileInfo.PublicId);

	WaitUntil(bRequestFriendByPublicIdSuccess, "Wait sending friend request");

	LobbyApiClients[0]->Lobby.CancelFriendRequest(LobbyApiClients[1]->CredentialsRef->GetUserId());
	WaitUntil(bCancelFriendSuccess, "wait cancelling friend request");

	bool bDeleteProfileDone = false;
	bool bDeleteProfileSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DeleteUserProfile"));
	AdminDeleteUserProfile(LobbyApiClients[1]->CredentialsRef->GetNamespace(), LobbyApiClients[1]->CredentialsRef->GetUserId(), FVoidHandler::CreateLambda([&bDeleteProfileDone, &bDeleteProfileSuccessful]()
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
			bDeleteProfileSuccessful = true;
			bDeleteProfileDone = true;
		}), LobbyTestErrorHandler);

	WaitUntil(bDeleteProfileDone, "Waiting for Deletion...");

	AB_TEST_FALSE(bRequestFriendByPublicIdError);

	LobbyDisconnect(LobbyUserCount);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FriendPresenceWithInvalidChar, "AccelByte.Tests.Lobby.B.FriendPresenceWithInvalidChar", AutomationFlagMaskLobby);
bool FriendPresenceWithInvalidChar::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetGetAllUserPresenceResponseDelegate(GetAllUsersPresenceDelegate);
	LobbyApiClients[0]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);
	LobbyApiClients[0]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[1]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);
	LobbyApiClients[1]->Lobby.SetUserPresenceResponseDelegate(UserPresenceDelegate);
	
	LobbyApiClients[1]->Lobby.RequestFriend(LobbyUsers[0].UserId);
	FString Text = FString::Printf(TEXT("Requesting Friend %d... "), 1);
	WaitUntil(bRequestFriendSuccess, Text, DefaultTimeout);

	LobbyApiClients[0]->Lobby.AcceptFriend(LobbyUsers[1].UserId);
	Text = FString::Printf(TEXT("Accepting Friend %d... "), 1);
	WaitUntil(bAcceptFriendSuccess, Text, DefaultTimeout);

	LobbyApiClients[1]->Lobby.SendSetPresenceStatus(Availability::Availabe, TEXT("\"acti \" vity\""));
	bRequestFriendSuccess = false;
	bAcceptFriendSuccess = false;

	Text = FString::Printf(TEXT("Setting presence %d... "), 1);
	WaitUntil(bUserPresenceSuccess, Text, DefaultTimeout);

	LobbyApiClients[0]->Lobby.SendGetOnlineUsersRequest();
	WaitUntil([&]()
			{
				return bGetAllUserPresenceSuccess;
			}
		, "Getting Friend Status..."
		, DefaultTimeout);

	bUnfriendSuccess = false;
	bUnfriendError = false;
	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);
	WaitUntil(bUnfriendSuccess, TEXT("Waiting Unfriend..."), DefaultTimeout);

	LobbyDisconnect(2);

	AB_TEST_TRUE(bGetAllUserPresenceSuccess);
	FString ExpectedValue = TEXT("0");
	AB_TEST_NOT_EQUAL(onlineUserResponse.Code, ExpectedValue);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllFriends, "AccelByte.Tests.Lobby.B.ListOnlineFriendsNew", AutomationFlagMaskLobby);
bool LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllFriends::RunTest(const FString& Parameters)
{
	LobbyConnect(TestUserCount);

	LobbyApiClients[0]->Lobby.SetGetOnlineFriendsPresenceResponseDelegate(GetFriendsPresenceDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyApiClients[i]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);
	}

	LobbyApiClients[0]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyApiClients[i]->Lobby.RequestFriend(LobbyUsers[0].UserId);
		FString Text = FString::Printf(TEXT("Requesting Friend %d... "), i);
		WaitUntil(bRequestFriendSuccess, Text, DefaultTimeout);

		LobbyApiClients[0]->Lobby.AcceptFriend(LobbyUsers[i].UserId);
		Text = FString::Printf(TEXT("Accepting Friend %d... "), i);
		WaitUntil(bAcceptFriendSuccess, Text, DefaultTimeout);

		LobbyApiClients[i]->Lobby.SendSetPresenceStatus(Availability::Availabe, TEXT("random activity"));
		bRequestFriendSuccess = false;
		bAcceptFriendSuccess = false;
	}

	LobbyApiClients[0]->Lobby.SendGetOnlineFriendPresenceRequest();
	WaitUntil([&]()
			{
				return bGetFriendsPresenceSuccess;
			}
		, TEXT("Getting Friend Status...")
		, DefaultTimeout);

	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyApiClients[i]->Lobby.SendSetPresenceStatus(Availability::Offline, TEXT("disappearing"));
		LobbyApiClients[i]->Lobby.Unfriend(LobbyUsers[0].UserId);
	}

	LobbyDisconnect(TestUserCount);

	AB_TEST_TRUE(bGetFriendsPresenceSuccess);
	AB_TEST_TRUE(onlineFriendResponse.friendsId.Num() >= 3);
	AB_TEST_TRUE(onlineFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));
	AB_TEST_TRUE(onlineFriendResponse.friendsId.Contains(LobbyUsers[2].UserId));
	AB_TEST_TRUE(onlineFriendResponse.friendsId.Contains(LobbyUsers[3].UserId));
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
		LobbyUserIds.Add(LobbyUsers[i].UserId);
		if(i >= (TestUserCount - ExpectedOffline))
		{
			LobbyApiClients[i]->Lobby.Disconnect();
		}
	}

	DelaySeconds(5, TEXT("Waiting backend to sync lobby disconnect"));

	// Act (call bulk get user presence)
	FAccelByteModelsBulkUserStatusNotif GetPresenceResult;
	bool bGetPresenceSuccess = false;
	LobbyApiClients[0]->Lobby.BulkGetUserPresence(LobbyUserIds
		, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda(
			[&bGetPresenceSuccess, &GetPresenceResult](const FAccelByteModelsBulkUserStatusNotif& Result)
			{
				GetPresenceResult = Result;
				bGetPresenceSuccess = true;
			})
		, LobbyTestErrorHandler);
	FlushHttpRequests();
	WaitUntil([&]()
			{
				return bGetPresenceSuccess;
			}
		, TEXT("Waiting for get presence...")
		, DefaultTimeout);

	// Act (count the result)
	bool ExpectedOnline = true;
	int OnlineCount = 0;
	int OfflineCount = 0;
	for(auto& Online : GetPresenceResult.Data)
	{
		// static_cast<int> is used because older UE4 doesn't convert enum to int automatically, for fixing UE 4.22 compile error.
		UE_LOG(LogAccelByteLobbyTest, Warning, TEXT("User: %s | Status: %d | %d"), *Online.UserID, static_cast<int32>(Online.Availability), GetPresenceResult.Data.Num());
		bool bIsOnline = false;
		bool bIsExpectedOnline = false;
		for (int i = 1; i < (TestUserCount - ExpectedOffline); i++) {
			if (Online.UserID == LobbyUsers[i].UserId)
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
		LobbyUserIds.Add(LobbyUsers[i].UserId);
		if (i >= (TestUserCount - ExpectedOffline))
		{
			LobbyApiClients[i]->Lobby.Disconnect();
		}
	}

	DelaySeconds(5, TEXT("Waiting backend to sync lobby disconnect"));

	// Act (call bulk get user presence)
	FAccelByteModelsBulkUserStatusNotif GetPresenceResult;
	bool bGetPresenceSuccess = false;
	LobbyApiClients[0]->Lobby.BulkGetUserPresence(LobbyUserIds
		, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda(
			[&bGetPresenceSuccess, &GetPresenceResult](const FAccelByteModelsBulkUserStatusNotif& Result)
			{
				GetPresenceResult = Result;
				bGetPresenceSuccess = true;
			})
		, LobbyTestErrorHandler
		, true);
	FlushHttpRequests();
	WaitUntil([&]()
			{
				return bGetPresenceSuccess;
			}
		, TEXT("Waiting for get presence...")
		, DefaultTimeout);

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
	LobbyApiClients[0]->Lobby.BulkGetUserPresence(LobbyUserIds
		, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda(
			[&bGetPresenceSuccess, &bGetPresenceDone, &GetPresenceResult](const FAccelByteModelsBulkUserStatusNotif& Result)
			{
				GetPresenceResult = Result;
				bGetPresenceSuccess = true;
				bGetPresenceDone = true;
			})
		, FErrorHandler::CreateLambda([&bGetPresenceDone, &GetPresenceErrorCode](int32 Code, const FString& Message) 
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get User's Presence Error. Code: %d | Message: %s"), Code, *Message);
				GetPresenceErrorCode = Code;
				bGetPresenceDone = true;
			})
		);
	FlushHttpRequests();
	WaitUntil([&]()
			{
				return bGetPresenceDone;
			}
		, TEXT("Waiting for get presence...")
		, DefaultTimeout);

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

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();
	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	LobbyDisconnect(2);
	AB_TEST_TRUE(bGetInfoPartyError);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestGetPartyInfo_PartyCreated_ReturnOk, "AccelByte.Tests.Lobby.B.GetPartyInfo", AutomationFlagMaskLobby);
bool LobbyTestGetPartyInfo_PartyCreated_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(1);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, TEXT("Creating Party..."), DefaultTimeout);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

	WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);

	LobbyDisconnect(1);

	AB_TEST_TRUE(bGetInfoPartySuccess);
	AB_TEST_FALSE(infoPartyResponse.PartyId.IsEmpty());
	AB_TEST_FALSE(infoPartyResponse.InvitationToken.IsEmpty());
	AB_TEST_TRUE(infoPartyResponse.Members.Num() > 0);
	AB_TEST_EQUAL(infoPartyResponse.Members[0], LobbyUsers[0].UserId);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(MessageIdCachedResponse, "AccelByte.Tests.Lobby.B.MessageIdCachedResponse", AutomationFlagMaskLobby);
bool MessageIdCachedResponse::RunTest(const FString& Parameters)
{
	// Lobby Prep
	LobbyConnect(1);
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);
	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, TEXT("Creating Party..."), DefaultTimeout);

	{ // Case 1: No response delegate changes
		const FString MessageIdA = LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
		WaitUntil(bGetInfoPartySuccess, TEXT("Waiting for basic delegate response..."), DefaultTimeout);
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

		const FString MessageIdA = LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegateA);
		const FString MessageIdB = LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegateB);
		WaitUntil([&]
				{
					return CallbackCount == 2;
				}
			, TEXT("Waiting for 2 responses...")
			, DefaultTimeout);
		UTEST_TRUE(TEXT("2 different cached callbacks returning 2 different responses"), bIsACalled && bIsBCalled);
	}

	{ // Case 3. unique responses for each requests
		TArray<uint8> UniqueResponseChecker, Index;
		TMap<uint8, FString> MessageIdResponseMap;

		// Work Test Item Prep
		const uint8 MaxRequestCount = 5;

		for (uint8 i = 0; i < MaxRequestCount;i++)
		{
			const auto GetInfoPartyDelegate_Unique = Api::Lobby::FPartyInfoResponse::CreateLambda([i, &UniqueResponseChecker](const FAccelByteModelsInfoPartyResponse&)
				{
					UniqueResponseChecker.Add(i);
				});

			const FString MessageId = LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate_Unique);
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Just sent request with message id [%s] expecting response [%d]"), *MessageId, i);

			Index.Add(i);
			MessageIdResponseMap.Emplace(i, MessageId);
		}


		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Expected responses"));
		for (const TPair<uint8, FString>& MessageIdResponsePair : MessageIdResponseMap)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("message id [%s] -> response [%d]"), *MessageIdResponsePair.Value, MessageIdResponsePair.Key);
		}

		WaitUntil([&]
				{
					return UniqueResponseChecker.Num() == Index.Num();
				}
			, TEXT("Waiting for all unique responses being called ...")
			, DefaultTimeout);
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

		LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate_ShouldNotBeCalled);
		WaitUntil(bGetInfoPartySuccess, TEXT("Waiting for cached response ..."), DefaultTimeout);
		UTEST_FALSE(TEXT("Overriding response will not affect response for request that is already on the way. Single Sequence"), bLastAssignedWrongDelegateCalled);

		bLastAssignedWrongDelegateCalled = false;
		LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate_ShouldNotBeCalled);
		LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate_ShouldNotBeCalled);
		UTEST_FALSE(TEXT("Overriding response will not affect response for request that is already on the way. Pseudo-Random Sequence"), bLastAssignedWrongDelegateCalled);
	}

	//@TODO dummy uncached/unregistered/unknown message id will not call a response

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
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
	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(OnInfoPartyResponse);
	
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);
	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, TEXT("Creating party.."), DefaultTimeout);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();
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
	
	LobbyApiClients[0]->Lobby.GetPartyData(PartyId, OnPartyDataSuccess, OnPartyDataError);
	WaitUntil(bGetPartyDataDone, TEXT("Getting party data..."), DefaultTimeout);
	
	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);

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

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

	WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, TEXT("Creating Party..."), DefaultTimeout);

	bCreatePartySuccess = false;

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, TEXT("Creating Party..."), DefaultTimeout);

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

	WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);

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

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetPartyGenerateCodeResponseDelegate(GeneratePartyCodeDelegate);

	LobbyApiClients[0]->Lobby.SetPartyGetCodeResponseDelegate(GetPartyCodeDelegate);

	LobbyApiClients[0]->Lobby.SetPartyDeleteCodeResponseDelegate(DeletePartyCodeDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetPartyJoinViaCodeResponseDelegate(JoinViaCodeDelegate);

	// Leave Previous parties
	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	// Create Party
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, TEXT("Creating Party..."), DefaultTimeout);
	AB_TEST_FALSE(bCreatePartyError);

	// Get Party Code
	LobbyApiClients[0]->Lobby.SendPartyGetCodeRequest();
	WaitUntil(bGetPartyCodeSuccess, TEXT("Getting Party Code..."), DefaultTimeout);
	AB_TEST_TRUE(bGetPartyCodeSuccess);
	bGetPartyCodeSuccess = false;
	partyCode = partyCodeResponse.PartyCode;

	bCreatePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, TEXT("Creating party..."), DefaultTimeout);

	// Join via Party Code
	LobbyApiClients[1]->Lobby.SendPartyJoinViaCodeRequest(partyCode);
	WaitUntil(bJoinPartyViaCodeSuccess, TEXT("Joining Party via Code..."), DefaultTimeout);
	AB_TEST_TRUE(bJoinPartyViaCodeSuccess);
	bJoinPartyViaCodeError = false;
	bJoinPartyViaCodeSuccess = false;

	// Leave Party
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);

	// Generate new Party Code
	LobbyApiClients[0]->Lobby.SendPartyGenerateCodeRequest();
	WaitUntil(bGeneratePartyCodeSuccess, TEXT("Generate Party Code..."), DefaultTimeout);
	AB_TEST_TRUE(bGeneratePartyCodeSuccess);

	// Join via Party Code using previous code
	LobbyApiClients[1]->Lobby.SendPartyJoinViaCodeRequest(partyCode);
	WaitUntil(bJoinPartyViaCodeSuccess, TEXT("Joining Party via Code..."), DefaultTimeout);
	AB_TEST_TRUE(bJoinPartyViaCodeError);
	bJoinPartyViaCodeError = false;
	bJoinPartyViaCodeSuccess = false;

	// Get Party Code
	LobbyApiClients[0]->Lobby.SendPartyGetCodeRequest();
	WaitUntil(bGetPartyCodeSuccess, TEXT("Getting Party Code..."), DefaultTimeout);
	AB_TEST_TRUE(bGetPartyCodeSuccess);
	bGetPartyCodeSuccess = false;
	partyCode = partyCodeResponse.PartyCode;
	AB_TEST_TRUE(partyCode == partyGenerateCodeResponse.PartyCode);

	// Join via Party Code using new code
	LobbyApiClients[1]->Lobby.SendPartyJoinViaCodeRequest(partyCode);
	WaitUntil(bJoinPartyViaCodeSuccess, TEXT("Joining Party via Code..."), DefaultTimeout);
	AB_TEST_TRUE(bJoinPartyViaCodeSuccess);
	bJoinPartyViaCodeError = false;
	bJoinPartyViaCodeSuccess = false;

	// Delete Party Code
	LobbyApiClients[0]->Lobby.SendPartyDeleteCodeRequest();
	WaitUntil(bDeletePartyCodeSuccess, TEXT("Deleting Party Code..."), DefaultTimeout);
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
	LobbyApiClients[0]->Lobby.SetPartyJoinViaCodeResponseDelegate(Lobby::FPartyJoinViaCodeResponse::CreateLambda(
			[&](const FAccelByteModelsPartyJoinResponse& Result)
			{
				if(Result.Code.Equals("11573") || !Result.Code.Equals("0"))
				{
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("[EXPECTED] Join party code is invalid with code : %s"), *Result.Code);
					bJoinFailed = true;
				}
			})
		);
	LobbyApiClients[0]->Lobby.SendPartyJoinViaCodeRequest("INVALID");
	WaitUntil(bJoinFailed, TEXT("Waiting to join party via code..."), DefaultTimeout);
	
	AB_TEST_TRUE(bJoinFailed);

	LobbyDisconnect(1);
	ResetResponses();
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTest_SetPartyLimit_Success, "AccelByte.Tests.Lobby.B.PartySetLimit", AutomationFlagMaskLobby);
bool LobbyTest_SetPartyLimit_Success::RunTest(const FString& Parameters)
{
	constexpr int UserNum {2};
	LobbyConnect(UserNum);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);
	LobbyApiClients[0]->Lobby.SetInvitePartyResponseDelegate(InvitePartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[1]->Lobby.SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);
	LobbyApiClients[1]->Lobby.SetInvitePartyJoinResponseDelegate(JoinPartyDelegate);

	// lobby 0 leave party
	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "waiting lobby 0 leave party");
	
	// lobby 1 leave party
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "waiting lobby 1 leave party");
	
	// lobby 0 create party
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess);
	FString PartyId = partyCreateResponse.PartyId;
	
	// lobby 0 set party limit to 1, save response A to assert
	bool bSetPartyLimitDone {false};
	const auto OnSetPartyLimitSuccess = FVoidHandler::CreateLambda(
		[&bSetPartyLimitDone]()
		{
			bSetPartyLimitDone = true;
		});
	
	LobbyApiClients[0]->Lobby.SetPartySizeLimit(PartyId, 1, OnSetPartyLimitSuccess, LobbyTestErrorHandler);
	WaitUntil(bSetPartyLimitDone, "Waiting set party limit to 1");
	const bool bSetPartyLimitSuccess = bSetPartyLimitDone;
	
	DelaySeconds(1, "delay 1 sec");

	// lobby 0 invite lobby 1
	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyApiClients[1]->CredentialsRef->GetUserId());
	WaitUntil(bGetInvitedNotifSuccess, "Waiting get invitation notif when party limit 1");

	// wait lobby 1 join response, expected to fail
	LobbyApiClients[1]->Lobby.SendAcceptInvitationRequest(invitedToPartyResponse.PartyId, invitedToPartyResponse.InvitationToken);
	WaitUntil(bJoinPartySuccess, "waiting invite party limited");
	const bool bJoinPartyLimitedError {bJoinPartyError};

	// lobby 0 set party limit to 2
	bSetPartyLimitDone = false;
	LobbyApiClients[0]->Lobby.SetPartySizeLimit(PartyId, 2, OnSetPartyLimitSuccess, LobbyTestErrorHandler);
	WaitUntil(bSetPartyLimitDone, "Waiting set party limit to 2");

	DelaySeconds(1, "waiting 1 sec");

	// lobby 1 accept invitation, expected to succeed
	bJoinPartySuccess = false;
	bJoinPartyError = false;
	LobbyApiClients[1]->Lobby.SendAcceptInvitationRequest(invitedToPartyResponse.PartyId, invitedToPartyResponse.InvitationToken);
	WaitUntil(bJoinPartySuccess, "Waiting join party success");
	const bool bJoinPartyNotLimitedSuccess {!bJoinPartyError};

	// lobby 1 tries set party limit, failed, save response D to assert
	bSetPartyLimitDone = false;
	bool bSetPartyLimitError = false;
	const auto OnSetPartyLimitError = FErrorHandler::CreateLambda([&bSetPartyLimitError](const int32 Code, const FString& Message)
	{
		bSetPartyLimitError = true;
	});
	
	LobbyApiClients[1]->Lobby.SetPartySizeLimit(PartyId, 3, OnSetPartyLimitSuccess, OnSetPartyLimitError);
	WaitUntil([&](){return bSetPartyLimitDone || bSetPartyLimitError;}, "Waiting non leader set party limit");
	const bool bNonPartyLeaderSetLimitError {bSetPartyLimitError};
	
	LobbyDisconnect(UserNum);
	ResetResponses();

	// assert A is ok
	AB_TEST_TRUE(bSetPartyLimitSuccess);

	// assert B is fail
	AB_TEST_TRUE(bJoinPartyLimitedError);
	
	// assert c is ok
	AB_TEST_TRUE(bJoinPartyNotLimitedSuccess);
	
	// assert d is fail
	AB_TEST_TRUE(bNonPartyLeaderSetLimitError);
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestInviteToParty_InvitationAccepted_CanChat, "AccelByte.Tests.Lobby.B.PartyChat", AutomationFlagMaskLobby);
bool LobbyTestInviteToParty_InvitationAccepted_CanChat::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInvitePartyResponseDelegate(InvitePartyDelegate);

	LobbyApiClients[1]->Lobby.SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);

	LobbyApiClients[1]->Lobby.SetInvitePartyJoinResponseDelegate(JoinPartyDelegate);
	
	LobbyApiClients[0]->Lobby.SetPartyChatNotifDelegate(PartyChatNotifDelegate);
	
	LobbyApiClients[1]->Lobby.SetPartyMessageResponseDelegate(PartyChatSendDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, TEXT("Creating Party..."), DefaultTimeout);

	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[1].UserId);
	WaitUntil(bInvitePartySuccess, TEXT("Inviting to Party..."), DefaultTimeout);

	AB_TEST_TRUE(bInvitePartySuccess);

	WaitUntil([&]()
			{
				return bGetInvitedNotifSuccess;
			}
		, TEXT("Waiting for Party Invitation...")
		, DefaultTimeout
		, DefaultDeltaTime);

	AB_TEST_FALSE(bGetInvitedNotifError);

	LobbyApiClients[0]->Lobby.SetPartyDataUpdateNotifDelegate(PartyDataUpdateDelegate);

	LobbyApiClients[1]->Lobby.SendAcceptInvitationRequest(*invitedToPartyResponse.PartyId, *invitedToPartyResponse.InvitationToken);
	
	WaitUntil([]()->bool
			{
				return bJoinPartySuccess || bGetInvitedNotifError;
			}
		, TEXT("Joining a Party...")
		, DefaultTimeout
		, DefaultDeltaTime);
	AB_TEST_FALSE(bJoinPartyError);

	WaitUntil(bGetPartyDataUpdateNotifSuccess, TEXT("Waiting receive party data update notif"));
	const FAccelByteModelsPartyDataNotif partyDataUpdate = partyDataNotif;

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();
	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);
	const auto partyInfoResult = infoPartyResponse;

	AB_TEST_FALSE(bGetInfoPartyError);

	// Compare partydata notif with partyget info
	AB_TEST_EQUAL(partyDataUpdate.PartyId, partyInfoResult.PartyId);
	AB_TEST_EQUAL(partyDataUpdate.Leader, partyInfoResult.LeaderId);
	AB_TEST_EQUAL(partyDataUpdate.Members.Num(), partyInfoResult.Members.Num());

	LobbyApiClients[1]->Lobby.SendPartyMessage(TEXT("This is a party chat"));
	WaitUntil(bSendPartyChatSuccess, TEXT("Sending a Party Chat..."), DefaultTimeout);
	AB_TEST_FALSE(bSendPartyChatError);

	WaitUntil([]()->bool
			{
				return bReceivedPartyChatSuccess || bSendPartyChatError;
			}
		, TEXT("Fetching Party Chat...")
		, DefaultTimeout
		, DefaultDeltaTime);
	AB_TEST_TRUE(bReceivedPartyChatSuccess);

	bLeavePartySuccess = false;
	bLeavePartyError = false;
	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	AB_TEST_FALSE(bLeavePartyError);

	bLeavePartySuccess = false;
	bLeavePartyError = false;
	LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	AB_TEST_FALSE(bLeavePartyError);

	LobbyDisconnect(2);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestInviteToParty_InvitationRejected, "AccelByte.Tests.Lobby.B.RejectPartyInvitation", AutomationFlagMaskLobby);
bool LobbyTestInviteToParty_InvitationRejected::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);
	LobbyApiClients[0]->Lobby.SetInvitePartyResponseDelegate(InvitePartyDelegate);
	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[1]->Lobby.SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);
	LobbyApiClients[1]->Lobby.SetInvitePartyRejectResponseDelegate(RejectPartyDelegate);
	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[0]->Lobby.SetPartyInvitationRejectedNotifDelegate(RejectPartyNotifDelegate);
	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();
	
	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bRejectPartySuccess = false;
	bRejectPartyError = false; 
	bRejectedPartyNotifSuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, TEXT("Creating Party..."), DefaultTimeout);

	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[1].UserId);
	WaitUntil(bInvitePartySuccess, TEXT("Inviting to Party..."), DefaultTimeout);

	AB_TEST_TRUE(bInvitePartySuccess);

	WaitUntil([&]()
			{
				return bGetInvitedNotifSuccess;
			}
		, TEXT("Waiting for Party Invitation...")
		, DefaultTimeout
		, DefaultDeltaTime);


	AB_TEST_FALSE(bGetInvitedNotifError);

	LobbyApiClients[1]->Lobby.SendRejectInvitationRequest(*invitedToPartyResponse.PartyId, *invitedToPartyResponse.InvitationToken);
	WaitUntil([]()->bool
			{
				return bRejectPartySuccess || bGetInvitedNotifError;
			}
		, TEXT("Rejecting Party Invitation...")
		, DefaultTimeout
		, DefaultDeltaTime);
	
	AB_TEST_FALSE(bRejectPartyError);

	WaitUntil([]()
			{
				return bRejectedPartyNotifSuccess;
			}
		, TEXT("Waiting for Reject Party Notification...")
		, DefaultTimeout);

	AB_TEST_TRUE(bRejectedPartyNotifSuccess)

	LobbyDisconnect(2);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestPartyMember_Kicked, "AccelByte.Tests.Lobby.B.PartyKicked", AutomationFlagMaskLobby);
bool LobbyTestPartyMember_Kicked::RunTest(const FString& Parameters)
{
	FAccelByteModelsPartyGetInvitedNotice invitedToParty[2];
	FAccelByteModelsPartyJoinResponse joinParty[2];
	LobbyConnect(3);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInvitePartyResponseDelegate(InvitePartyDelegate);

	LobbyApiClients[1]->Lobby.SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);

	LobbyApiClients[2]->Lobby.SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);

	LobbyApiClients[1]->Lobby.SetInvitePartyJoinResponseDelegate(JoinPartyDelegate);

	LobbyApiClients[2]->Lobby.SetInvitePartyJoinResponseDelegate(JoinPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[2]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[2]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetInvitePartyKickMemberResponseDelegate(KickPartyMemberDelegate);

	LobbyApiClients[2]->Lobby.SetPartyKickNotifDelegate(KickedFromPartyDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[2]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[2]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	
	WaitUntil(bCreatePartySuccess, TEXT("Creating Party..."), DefaultTimeout);

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[1].UserId);

	WaitUntil(bInvitePartySuccess, TEXT("Inviting to Party..."), DefaultTimeout);

	bInvitePartySuccess = false;

	WaitUntil([&]()
			{
				return bGetInvitedNotifSuccess;
			}
		, TEXT("Waiting for Party Invitation")
		, DefaultTimeout);

	bGetInvitedNotifSuccess = false;
	invitedToParty[0] = invitedToPartyResponse;

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[2].UserId);

	WaitUntil(bInvitePartySuccess, TEXT("Inviting to Party..."), DefaultTimeout);
	const FString InvitedUserId {LobbyUsers[2].UserId};
	const FAccelByteModelsPartyInviteResponse InviteToPartyResponse {partyInviteResponse};

	bInvitePartySuccess = false;

	WaitUntil([&]()
			{
				return bGetInvitedNotifSuccess;
			}
		, TEXT("Waiting for Party Invitation")
		, DefaultTimeout);

	bGetInvitedNotifSuccess = false;
	invitedToParty[1] = invitedToPartyResponse;

	LobbyApiClients[1]->Lobby.SendAcceptInvitationRequest(invitedToParty[0].PartyId, invitedToParty[0].InvitationToken);

	WaitUntil(bJoinPartySuccess, TEXT("Joining a Party..."), DefaultTimeout);

	bJoinPartySuccess = false;
	joinParty[0] = joinPartyResponse;

	LobbyApiClients[2]->Lobby.SendAcceptInvitationRequest(invitedToParty[1].PartyId, invitedToParty[1].InvitationToken);

	WaitUntil(bJoinPartySuccess, TEXT("Joining a Party..."), DefaultTimeout);

	bJoinPartySuccess = false;
	joinParty[1] = joinPartyResponse;

	LobbyApiClients[0]->Lobby.SendKickPartyMemberRequest(LobbyUsers[2].UserId);

	WaitUntil(bKickPartyMemberSuccess, TEXT("Kicking Party Member..."), DefaultTimeout);
	const FString KickedUserId {LobbyUsers[2].UserId};
	const FAccelByteModelsKickPartyMemberResponse KickMemberFromPartyResponse {kickMemberFromPartyResponse};

	WaitUntil(bKickedFromPartySuccess, TEXT("Waiting to Get Kicked from Party..."), DefaultTimeout);

	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	for (int i = 0; i < 2; i++)
	{
		LobbyApiClients[i]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
		bLeavePartySuccess = false;
	}

	LobbyDisconnect(3);

	AB_TEST_TRUE(bKickPartyMemberSuccess);
	AB_TEST_TRUE(bKickedFromPartySuccess);
	AB_TEST_TRUE(joinParty[2].Members.Num() == 3 || joinParty[1].Members.Num() == 3);
	AB_TEST_EQUAL(infoPartyResponse.Members.Num(), 2);
	AB_TEST_EQUAL(InviteToPartyResponse.InviteeID, InvitedUserId);
	AB_TEST_EQUAL(KickMemberFromPartyResponse.UserId, KickedUserId);

	ResetResponses();
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestPartyMember_PromoteLeader, "AccelByte.Tests.Lobby.B.PromotePartyLeader", AutomationFlagMaskLobby);
bool LobbyTestPartyMember_PromoteLeader::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetInvitePartyResponseDelegate(InvitePartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetPartyPromoteLeaderResponseDelegate(PromotePartyLeaderDelegate);

	LobbyApiClients[1]->Lobby.SetInvitePartyJoinResponseDelegate(JoinPartyDelegate);

	LobbyApiClients[1]->Lobby.SetPartyGetInvitedNotifDelegate(InvitedToPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, TEXT("Leaving Party..."), DefaultTimeout);
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, TEXT("Creating Party..."), DefaultTimeout);

	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[1].UserId);

	WaitUntil(bInvitePartySuccess, TEXT("Inviting to Party..."), DefaultTimeout);

	bInvitePartySuccess = false;

	WaitUntil([&]()
			{
				return bGetInvitedNotifSuccess;
			}
		, TEXT("Waiting for Party Invitation")
		, DefaultTimeout);

	bGetInvitedNotifSuccess = false;

	LobbyApiClients[1]->Lobby.SendAcceptInvitationRequest(*invitedToPartyResponse.PartyId
		, *invitedToPartyResponse.InvitationToken);

	WaitUntil(bJoinPartySuccess, TEXT("Joining a Party..."), DefaultTimeout);

	AB_TEST_TRUE(bJoinPartySuccess);

	bJoinPartySuccess = false;

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	bGetInfoPartySuccess = false;

	LobbyApiClients[0]->Lobby.SendPartyPromoteLeaderRequest(LobbyUsers[1].UserId);

	WaitUntil(bPromotePartyLeaderSuccess, TEXT("Promoting Party leader..."), DefaultTimeout);

	AB_TEST_TRUE(bPromotePartyLeaderSuccess);

	bPromotePartyLeaderSuccess = false;

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, TEXT("Getting Info Party..."), DefaultTimeout);

	bGetInfoPartySuccess = false;

	AB_TEST_EQUAL(infoPartyResponse.LeaderId, LobbyUsers[1].UserId);

	LobbyDisconnect(2);

	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestConnected_ForMoreThan1Minutes_DoesntDisconnect, "AccelByte.DisabledTests.Lobby.B.Connect1Mins", AutomationFlagMaskLobby);
bool LobbyTestConnected_ForMoreThan1Minutes_DoesntDisconnect::RunTest(const FString& Parameters)
{
	LobbyConnect(1);
	DelaySeconds(60.0f, TEXT("Connect 1 Mins: Waiting..."));
	AB_TEST_TRUE(LobbyApiClients[0]->Lobby.IsConnected());

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

	LobbyApiClients[0]->Lobby.SetMessageNotifDelegate(GetNotifDelegate);

	for (int i = 0; i < repetition; i++)
	{
		payloads[i] = FString::Printf(TEXT("Notification number %d"), i);

		UAccelByteBlueprintsTest::SendNotif(LobbyUsers[0].UserId
			, payloads[i]
			, true
			, FVoidHandler::CreateLambda([&]()
				{
					bSendNotifSucccess[i] = true;
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Notification Sent!"));
				})
			, LobbyTestErrorHandler);

		WaitUntil(bSendNotifSucccess[i], TEXT("Sending Notification..."), DefaultTimeout);

		WaitUntil([&]()
				{
					return bGetNotifSuccess;
				}
			, TEXT("Getting All Notifications...")
			, DefaultTimeout);
		

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

	LobbyApiClients[1]->Lobby.SetMessageNotifDelegate(GetNotifDelegate);

	for (int i = 0; i < repetition; i++)
	{
		MessageRequest[i].Topic = FString::Printf(TEXT("Message number %d"), i);
		MessageRequest[i].Message = FString::Printf(TEXT("Test from the integration test UE4 number %d"), i);

		LobbyApiClients[0]->Lobby.SendNotificationToUser(LobbyUsers[1].UserId
			, MessageRequest[i]
			, false
			, FVoidHandler::CreateLambda([&]()
				{
					bSendNotifSucccess[i] = true;
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Notification Sent!"));
				})
			, LobbyTestErrorHandler);

		WaitUntil(bSendNotifSucccess[i], TEXT("Sending Notification..."), DefaultTimeout);

		WaitUntil([&]()
				{
					return bGetNotifSuccess;
				}
			, TEXT("Getting All Notifications...")
			, DefaultTimeout);

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

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[1]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	LobbyApiClients[0]->Lobby.SetUserPresenceResponseDelegate(UserPresenceDelegate);

	LobbyApiClients[1]->Lobby.SetUserPresenceResponseDelegate(UserPresenceDelegate);

	LobbyApiClients[1]->Lobby.SetUserPresenceNotifDelegate(UserPresenceNotifDelegate);

	LobbyApiClients[1]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, TEXT("Accepting Friend..."), DefaultTimeout);

	LobbyApiClients[1]->Lobby.SendSetPresenceStatus(Availability::Availabe, TEXT("ready to play"));

	WaitUntil([&]()
			{
				return bUserPresenceSuccess;
			}
		, TEXT("Changing User Status...")
		, DefaultTimeout);

	bUserPresenceSuccess = false;

	LobbyApiClients[0]->Lobby.SendSetPresenceStatus(expectedUserStatus, TEXT("expected activity"));

	WaitUntil([&]()
			{
				return bUserPresenceSuccess;
			}
		, TEXT("Changing User Status...")
		, DefaultTimeout);

	WaitUntil([&]() 
			{
				return bUserPresenceNotifSuccess; 
			}
		, TEXT("Waiting for Changing User Presence...")
		, DefaultTimeout);

	bUnfriendSuccess = false;
	bUnfriendError = false;
	LobbyApiClients[1]->Lobby.Unfriend(LobbyUsers[0].UserId);

	WaitUntil(bUnfriendSuccess, TEXT("Waiting Unfriend..."), DefaultTimeout);

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

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[1]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	LobbyApiClients[1]->Lobby.SetUserPresenceResponseDelegate(UserPresenceDelegate);

	LobbyApiClients[0]->Lobby.SetUserPresenceResponseDelegate(UserPresenceDelegate);

	LobbyApiClients[1]->Lobby.SetUserPresenceNotifDelegate(UserPresenceNotifDelegate);

	LobbyApiClients[1]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, TEXT("Accepting Friend..."), DefaultTimeout);

	LobbyApiClients[1]->Lobby.SendSetPresenceStatus(Availability::Availabe, TEXT("ready to play again"));

	WaitUntil([&]()
			{
				return bUserPresenceSuccess;
			}
		, TEXT("Changing User Status...")
		, DefaultTimeout);

	bUserPresenceSuccess = false;
	LobbyApiClients[0]->Lobby.SendSetPresenceStatus(Availability::Availabe, TEXT("ready to play too"));

	WaitUntil([&]()
			{
				return bUserPresenceSuccess;
			}
		, TEXT("Changing User Status...")
		, DefaultTimeout);

	WaitUntil([&]()
			{
				return bUserPresenceNotifSuccess;
			}
		, TEXT("Waiting for Changing User Presence...")
		, DefaultTimeout);

	bUserPresenceSuccess = false;
	bUserPresenceNotifSuccess = false;
	LobbyApiClients[0]->Lobby.SendSetPresenceStatus(expectedUserStatus, TEXT("busy, can't play"));

	WaitUntil([&]()
			{
				return bUserPresenceSuccess;
			}
		, TEXT("Changing User Status...")
		, DefaultTimeout);

	WaitUntil([&]()
			{
				return bUserPresenceNotifSuccess;
			}
		, TEXT("Waiting for Changing User Presence...")
		, DefaultTimeout);

	bUnfriendSuccess = false;
	bUnfriendError = false;
	LobbyApiClients[1]->Lobby.Unfriend(LobbyUsers[0].UserId);

	WaitUntil(bUnfriendSuccess, TEXT("Waiting Unfriend..."), DefaultTimeout);

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

	LobbyApiClients[0]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[1]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[1]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	LobbyApiClients[0]->Lobby.SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	LobbyApiClients[1]->Lobby.SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	LobbyApiClients[0]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[1]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[0]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);

	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, TEXT("Getting List Outgoing Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, TEXT("Getting List Incoming Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, TEXT("Accepting Friend Request..."), DefaultTimeout);
	AB_TEST_FALSE(bAcceptFriendError);

	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[0].UserId));

	bUnfriendSuccess = false;
	bUnfriendError = false;
	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);

	WaitUntil(bUnfriendSuccess, TEXT("Waiting Unfriend..."), DefaultTimeout);
	AB_TEST_FALSE(bUnfriendError);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Notification_Request_Accept, "AccelByte.Tests.Lobby.B.FriendNotifRequest", AutomationFlagMaskLobby);
bool LobbyTestFriends_Notification_Request_Accept::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[1]->Lobby.SetOnIncomingRequestFriendsNotifDelegate(OnIncomingRequestNotifDelegate);

	LobbyApiClients[0]->Lobby.SetOnFriendRequestAcceptedNotifDelegate(OnRequestAcceptedNotifDelegate);

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[1]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	LobbyApiClients[0]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[1]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[0]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bRequestFriendError);

	WaitUntil(bOnIncomingRequestNotifSuccess, TEXT("Waiting for Incoming Friend Request..."));
	AB_TEST_FALSE(bOnIncomingRequestNotifError);
	AB_TEST_EQUAL(requestFriendNotifResponse.friendId, LobbyUsers[0].UserId);

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, TEXT("Accepting Friend Request..."), DefaultTimeout);
	AB_TEST_FALSE(bAcceptFriendError);

	WaitUntil(bOnRequestAcceptedNotifSuccess, TEXT("Waiting for Accepted Friend Request..."), DefaultTimeout);
	AB_TEST_FALSE(bOnRequestAcceptedNotifError);
	AB_TEST_EQUAL(acceptFriendNotifResponse.friendId, LobbyUsers[1].UserId);

	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[0].UserId));

	bUnfriendSuccess = false;
	bUnfriendError = false;
	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);

	WaitUntil(bUnfriendSuccess, TEXT("Waiting Unfriend..."), DefaultTimeout);
	AB_TEST_FALSE(bUnfriendError);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Request_Unfriend, "AccelByte.Tests.Lobby.B.FriendRequestUnfriend", AutomationFlagMaskLobby);
bool LobbyTestFriends_Request_Unfriend::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[1]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[1]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	LobbyApiClients[0]->Lobby.SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	LobbyApiClients[1]->Lobby.SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	LobbyApiClients[0]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[1]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[0]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	LobbyApiClients[1]->Lobby.SetOnUnfriendNotifDelegate(UnfriendNotifDelegate);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, TEXT("Getting List Outgoing Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, TEXT("Getting List Incoming Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, TEXT("Accepting Friend Request..."), DefaultTimeout);
	AB_TEST_FALSE(bAcceptFriendError);

	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[0].UserId));

	bUnfriendSuccess = false;
	bUnfriendError = false;
	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);

	WaitUntil(bUnfriendSuccess, TEXT("Waiting Unfriend..."), DefaultTimeout);
	AB_TEST_FALSE(bUnfriendError);

	WaitUntil(bUnfriendNotifSuccess, TEXT("Waiting Unfriend Notif..."), DefaultTimeout);
	AB_TEST_TRUE(bUnfriendNotifSuccess);

	AB_TEST_EQUAL(UnfriendNotifResponse.friendId, LobbyUsers[0].UserId);

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));
	}

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(LobbyUsers[0].UserId));
	}

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Request_Reject, "AccelByte.Tests.Lobby.B.FriendRequestReject", AutomationFlagMaskLobby);
bool LobbyTestFriends_Request_Reject::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[1]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[0]->Lobby.SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	LobbyApiClients[1]->Lobby.SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	LobbyApiClients[1]->Lobby.SetRejectFriendsResponseDelegate(RejectFriendDelegate);

	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	LobbyApiClients[0]->Lobby.SetOnRejectFriendsNotifDelegate(RejectFriendNotifDelegate);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, TEXT("Getting List Outgoing Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, TEXT("Getting List Incoming Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.RejectFriend(LobbyUsers[0].UserId);

	WaitUntil(bRejectFriendSuccess, TEXT("Rejecting Friend Request..."), DefaultTimeout);
	AB_TEST_FALSE(bRejectFriendError);

	WaitUntil(bRejectFriendNotifSuccess, TEXT("Getting Reject Friend Notif..."), DefaultTimeout);
	AB_TEST_TRUE(bRejectFriendNotifSuccess);
	AB_TEST_EQUAL(RejectFriendNotifResponse.userId, LobbyUsers[1].UserId);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, TEXT("Getting List Incoming Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_FALSE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	bListOutgoingFriendSuccess = false;
	bListOutgoingFriendError = false;
	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, TEXT("Getting List Outgoing Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_FALSE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Request_Cancel, "AccelByte.Tests.Lobby.B.FriendRequestCancel", AutomationFlagMaskLobby);
bool LobbyTestFriends_Request_Cancel::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[1]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[0]->Lobby.SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	LobbyApiClients[1]->Lobby.SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	LobbyApiClients[0]->Lobby.SetCancelFriendsResponseDelegate(CancelFriendDelegate);

	LobbyApiClients[0]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[1]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	LobbyApiClients[1]->Lobby.SetOnCancelFriendsNotifDelegate(CancelFriendNotifDelegate);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, TEXT("Getting List Outgoing Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, TEXT("Getting List Incoming Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[0]->Lobby.CancelFriendRequest(LobbyUsers[1].UserId);

	WaitUntil(bCancelFriendSuccess, TEXT("Cancelling Friend Request..."), DefaultTimeout);
	AB_TEST_FALSE(bCancelFriendError);

	WaitUntil(bCancelFriendNotifSuccess, TEXT("Getting Cancel Friend Notif..."), DefaultTimeout);
	AB_TEST_TRUE(bCancelFriendNotifSuccess);
	AB_TEST_EQUAL(CancelFriendNotifResponse.userId, LobbyUsers[0].UserId);

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, TEXT("Getting List Incoming Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_FALSE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(LobbyUsers[0].UserId));
	}

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));
	}

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_Complete_Scenario, "AccelByte.Tests.Lobby.B.FriendComplete", AutomationFlagMaskLobby);
bool LobbyTestFriends_Complete_Scenario::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[1]->Lobby.SetOnIncomingRequestFriendsNotifDelegate(OnIncomingRequestNotifDelegate);

	LobbyApiClients[0]->Lobby.SetOnFriendRequestAcceptedNotifDelegate(OnRequestAcceptedNotifDelegate);

	LobbyApiClients[0]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[1]->Lobby.SetGetFriendshipStatusResponseDelegate(GetFriendshipStatusDelegate);

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[0]->Lobby.SetListOutgoingFriendsResponseDelegate(ListOutgoingFriendDelegate);

	LobbyApiClients[1]->Lobby.SetListIncomingFriendsResponseDelegate(ListIncomingFriendDelegate);

	LobbyApiClients[0]->Lobby.SetCancelFriendsResponseDelegate(CancelFriendDelegate);

	LobbyApiClients[1]->Lobby.SetRejectFriendsResponseDelegate(RejectFriendDelegate);

	LobbyApiClients[1]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	LobbyApiClients[0]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[0]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bRequestFriendError);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, TEXT("Getting List Outgoing Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.CancelFriendRequest(LobbyUsers[1].UserId);

	WaitUntil(bCancelFriendSuccess, TEXT("Cancelling Friend Request..."), DefaultTimeout);
	AB_TEST_FALSE(bCancelFriendError);

	bListOutgoingFriendSuccess = false;
	bListOutgoingFriendError = false;
	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, TEXT("Getting List Outgoing Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListOutgoingFriendError);
	if (listOutgoingFriendResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));
	}

	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bRequestFriendError);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, TEXT("Getting List Incoming Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.RejectFriend(LobbyUsers[0].UserId);

	WaitUntil(bRejectFriendSuccess, TEXT("Rejecting Friend Request..."), DefaultTimeout);
	AB_TEST_FALSE(bRejectFriendError);

	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, TEXT("Requesting Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bRequestFriendError);
	WaitUntil([&]()
			{
				return bOnIncomingRequestNotifSuccess;
			}
		, TEXT("Waiting for Incoming Friend Request...")
		, DefaultTimeout);
	AB_TEST_FALSE(bOnIncomingRequestNotifError);
	AB_TEST_EQUAL(requestFriendNotifResponse.friendId, LobbyUsers[0].UserId);

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, TEXT("Getting List Incoming Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, TEXT("Accepting Friend Request..."), DefaultTimeout);
	AB_TEST_FALSE(bAcceptFriendError);
	WaitUntil([&]()
			{
				return bOnRequestAcceptedNotifSuccess;
			}
		, TEXT("Waiting for Accepted Friend Request...")
		, DefaultTimeout);
	AB_TEST_FALSE(bOnRequestAcceptedNotifError);
	AB_TEST_EQUAL(acceptFriendNotifResponse.friendId, LobbyUsers[1].UserId);

	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Friend);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);
	
	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Friend);

	bUnfriendSuccess = false;
	bUnfriendError = false;
	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);

	WaitUntil(bUnfriendSuccess, TEXT("Waiting Unfriend..."), DefaultTimeout);
	AB_TEST_FALSE(bUnfriendError);

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, TEXT("Loading Friend List..."), DefaultTimeout);
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));
	}

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, TEXT("Getting Friendship Status..."), DefaultTimeout);
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
		FriendUserIds.FriendIds.Add(LobbyUsers[i].UserId);
	}

	bool bBulkAddFriendSuccess = false;
	LobbyApiClients[0]->Lobby.BulkFriendRequest(FriendUserIds
		, FVoidHandler::CreateLambda([&bBulkAddFriendSuccess]()
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Bulk Add Friend Success!"));
				bBulkAddFriendSuccess = true;
			})
		, LobbyTestErrorHandler);
	WaitUntil(bBulkAddFriendSuccess, TEXT("Waiting Bulk Add Friend..."), DefaultTimeout);
	
	LobbyConnect(1);

	LobbyApiClients[0]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);
	LobbyApiClients[0]->Lobby.LoadFriendsList();
	WaitUntil(bLoadFriendListSuccess, TEXT("Waiting Load Friend List...!"), DefaultTimeout);

	LobbyApiClients[0]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	for (auto& FriendId : FriendUserIds.FriendIds)
	{
		AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(FriendId));
		bUnfriendSuccess = false;
		bUnfriendError = false;
		LobbyApiClients[0]->Lobby.Unfriend(FriendId);
		WaitUntil(bUnfriendSuccess, TEXT("Waiting Unfriend..."), DefaultTimeout);
		AB_TEST_FALSE(bUnfriendError);
	}

	LobbyDisconnect(1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestFriends_BulkFriendRequest_AddSelfUserId_Failed, "AccelByte.Tests.Lobby.B.FriendBulkFriendRequest_Failed", AutomationFlagMaskLobby);
bool LobbyTestFriends_BulkFriendRequest_AddSelfUserId_Failed::RunTest(const FString& Parameters)
{
	FAccelByteModelsBulkFriendsRequest FriendUserIds;
	FriendUserIds.FriendIds.Add(LobbyUsers[0].UserId);

	bool bBulkAddFriendSuccess = false;
	bool bBulkAddFriendError = false;
	bool bBulkAddFriendDone = false;
	LobbyApiClients[0]->Lobby.BulkFriendRequest(FriendUserIds
		, FVoidHandler::CreateLambda([&bBulkAddFriendSuccess, &bBulkAddFriendDone]()
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Bulk Add Friend Success!"));
				bBulkAddFriendSuccess = true;
				bBulkAddFriendDone = true;
			})
		, FErrorHandler::CreateLambda([&bBulkAddFriendDone, &bBulkAddFriendError](int32 Code, const FString& Message)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Error! Code: %d | Message: %s"), Code, *Message);
				bBulkAddFriendError = true;
				bBulkAddFriendDone = true;
			})
		);
	FlushHttpRequests();
	WaitUntil(bBulkAddFriendDone, TEXT("Waiting Bulk Add Friend..."), DefaultTimeout);

	AB_TEST_TRUE(bBulkAddFriendError);
	AB_TEST_FALSE(bBulkAddFriendSuccess);

	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestPlayer_BlockPlayer, "AccelByte.Tests.Lobby.B.Player.BlockPlayer", AutomationFlagMaskLobby);
bool LobbyTestPlayer_BlockPlayer::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	// Set Block Player Delegates
	LobbyApiClients[0]->Lobby.SetBlockPlayerResponseDelegate(BlockPlayerDelegate);

	LobbyApiClients[1]->Lobby.SetBlockPlayerNotifDelegate(BlockPlayerNotifDelegate);

	LobbyApiClients[0]->Lobby.SetUnblockPlayerResponseDelegate(UnblockPlayerDelegate);

	LobbyApiClients[1]->Lobby.SetUnblockPlayerNotifDelegate(UnblockPlayerNotifDelegate);

	LobbyApiClients[0]->Lobby.GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);

	WaitUntil(bListBlockedUserListSuccess, TEXT("Getting List of Blocked User for Lobby 0..."), DefaultTimeout);
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	auto CurrentListBlockedUser = listBlockedUserResponse;
	
	LobbyApiClients[0]->Lobby.BlockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bBlockPlayerSuccess, TEXT("Player 0 Blocks Player 1..."));
	AB_TEST_FALSE(bBlockPlayerError);
	AB_TEST_EQUAL(blockPlayerResponse.BlockedUserId, LobbyUsers[1].UserId);

	bListBlockedUserListSuccess = false;
	FBlockedData BlockedUserData;
	BlockedUserData.BlockedUserId = LobbyUsers[1].UserId;
	LobbyApiClients[0]->Lobby.GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);
	WaitUntil(bListBlockedUserListSuccess, TEXT("Checking if Player 1 is in Player 0 Block list."), DefaultTimeout);
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	bool bFound = false;
	for (auto& ResponseData : listBlockedUserResponse.Data)
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
	BlockerUserData.UserId = LobbyUsers[0].UserId;
	LobbyApiClients[1]->Lobby.GetListOfBlockers(ListBlockerDelegate, LobbyTestErrorHandler);
	WaitUntil(bListBlockerListSuccess, TEXT("Checking if Player 0 is in Player 1 Blocker list."), DefaultTimeout);
	AB_TEST_TRUE(bListBlockerListSuccess);
	for (auto& ResponseData : listBlockerResponse.Data)
	{
		if (ResponseData.UserId.Equals(BlockerUserData.UserId))
		{
			bFound = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Blocker Found in the List"));
		}
	}
	AB_TEST_TRUE(bFound);

	bool bRequestFriendAttemptDone = false;
	bool bBlockedFriendRequestPassed = false;
	LobbyApiClients[1]->Lobby.SetRequestFriendsResponseDelegate(Lobby::FRequestFriendsResponse::CreateLambda(
			[&bRequestFriendAttemptDone, &bBlockedFriendRequestPassed](FAccelByteModelsRequestFriendsResponse Result)
			{
				bRequestFriendAttemptDone = true;
				// Should be changed to something to signify blocked user response, but for now we'll use != 0
				if (Result.Code != "0")
				{
					bBlockedFriendRequestPassed = true;
				}
			})
		);
	LobbyApiClients[1]->Lobby.RequestFriend(LobbyUsers[0].UserId);
	WaitUntil(bRequestFriendAttemptDone, TEXT("Checking if Player 1 has been blocked from Requesting Friend Request properly."), DefaultTimeout);
	AB_TEST_TRUE(bBlockedFriendRequestPassed);
	bRequestFriendAttemptDone = false;
	bBlockedFriendRequestPassed = false;

	LobbyApiClients[0]->Lobby.UnblockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bUnblockPlayerSuccess, TEXT("Player 0 Unblocks Player 1..."), DefaultTimeout);
	AB_TEST_FALSE(bUnblockPlayerError);
	AB_TEST_EQUAL(unblockPlayerResponse.UnblockedUserId, LobbyUsers[1].UserId);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}


IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestPlayer_BlockPlayerReblockPlayer, "AccelByte.Tests.Lobby.B.Player.BlockReblockPlayer", AutomationFlagMaskLobby);
bool LobbyTestPlayer_BlockPlayerReblockPlayer::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	// Set Block Player Delegates
	LobbyApiClients[0]->Lobby.SetBlockPlayerResponseDelegate(BlockPlayerDelegate);

	LobbyApiClients[1]->Lobby.SetBlockPlayerNotifDelegate(BlockPlayerNotifDelegate);

	LobbyApiClients[0]->Lobby.SetUnblockPlayerResponseDelegate(UnblockPlayerDelegate);

	LobbyApiClients[1]->Lobby.SetUnblockPlayerNotifDelegate(UnblockPlayerNotifDelegate);

	// Start Test
	LobbyApiClients[0]->Lobby.GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);

	WaitUntil(bListBlockedUserListSuccess, TEXT("Getting List of Blocked User for Lobby 0..."), DefaultTimeout);
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	auto CurrentListBlockedUser = listBlockedUserResponse;

	LobbyApiClients[0]->Lobby.BlockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bBlockPlayerSuccess, TEXT("Requesting Friend..."), DefaultTimeout);
	AB_TEST_FALSE(bBlockPlayerError);

	bListBlockedUserListSuccess = false;
	FBlockedData BlockedUserData;
	BlockedUserData.BlockedUserId = LobbyUsers[1].UserId;
	LobbyApiClients[0]->Lobby.GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);
	WaitUntil(bListBlockedUserListSuccess, TEXT("Checking if Player 1 is in Player 0 Block list."), DefaultTimeout);
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	bool bFound = false;
	for(auto& ResponseData : listBlockedUserResponse.Data)
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
	BlockerUserData.UserId = LobbyUsers[0].UserId;
	LobbyApiClients[1]->Lobby.GetListOfBlockers(ListBlockerDelegate, LobbyTestErrorHandler);
	WaitUntil(bListBlockerListSuccess, TEXT("Checking if Player 0 is in Player 1 Blocker list."), DefaultTimeout);

	AB_TEST_TRUE(bListBlockerListSuccess);
	for (auto& ResponseData : listBlockerResponse.Data)
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
	LobbyApiClients[0]->Lobby.BlockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bBlockPlayerSuccess, TEXT("Player 0 Blocks Player 1 Again"), DefaultTimeout);
	AB_TEST_FALSE(bBlockPlayerError);

	WaitUntil(bBlockPlayerNotifSuccess, TEXT("Waiting for block notif"), DefaultTimeout);

	LobbyApiClients[0]->Lobby.UnblockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bUnblockPlayerSuccess, TEXT("Player 0 Unblocks Player 1..."), DefaultTimeout);
	AB_TEST_FALSE(bUnblockPlayerError);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

bool CompareSessionAttributes(const FJsonObjectWrapper& resultAttributes, TMap<FString, FString>& expected)
{
	for (auto& attribute : expected)
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

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]()
			{
				bLoginDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });
	FThreadSafeCounter setSessionAttributeSuccessCounter;
	FAccelByteModelsGetSessionAttributesResponse getSessionAttributesResponse;
	bool bGetSessionAttributeFinish = false;
	FAccelByteModelsGetAllSessionAttributesResponse getAllSessionAttributesResponse;
	bool bGetAllSessionAttributeFinish = false;

	Lobby& Lobby = FRegistry::Lobby;

	bool bSetSessionAttributeFailed = false;
	Lobby.SetSetSessionAttributeDelegate(THandler<FAccelByteModelsSetSessionAttributesResponse>::CreateLambda(
			[&setSessionAttributeSuccessCounter, &bSetSessionAttributeFailed](const FAccelByteModelsSetSessionAttributesResponse& result)
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
			})
		);

	Lobby.SetGetSessionAttributeDelegate(THandler<FAccelByteModelsGetSessionAttributesResponse>::CreateLambda(
			[&getSessionAttributesResponse, &bGetSessionAttributeFinish](const FAccelByteModelsGetSessionAttributesResponse& result)
			{
				bGetSessionAttributeFinish = true;
				getSessionAttributesResponse = result;
			})
		);

	Lobby.SetGetAllSessionAttributeDelegate(THandler<FAccelByteModelsGetAllSessionAttributesResponse>::CreateLambda(
			[&getAllSessionAttributesResponse, &bGetAllSessionAttributeFinish](const FAccelByteModelsGetAllSessionAttributesResponse& result)
			{
				bGetAllSessionAttributeFinish = true;
				getAllSessionAttributesResponse = result;
			})
		);

	Lobby.Connect();
	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 5);

	// ACT setting session attribute

	for (auto& attribute : sessionAttributes)
	{
		Lobby.SetSessionAttribute(attribute.Key, attribute.Value);
	}

	WaitUntil([&setSessionAttributeSuccessCounter, &sessionAttributes]() {return setSessionAttributeSuccessCounter.GetValue() == sessionAttributes.Num(); }, "Waiting set session Attribute", 10);

	Lobby.GetSessionAttribute(sessionAttributesKeys[0]);
	WaitUntil(bGetSessionAttributeFinish, TEXT("Wait for get session attribute"), DefaultTimeout);

	Lobby.GetAllSessionAttribute();
	WaitUntil(bGetAllSessionAttributeFinish, TEXT("Wait for get all session attribute"), DefaultTimeout);
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

	Waiting(bDeleteDone, TEXT("Waiting for Deletion..."));

	return true;
}
*/

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSameUserDifferentToken_Disconnected, "AccelByte.Tests.Lobby.C.LobbyTestSameUserDifferentToken_Disconnected", AutomationFlagMaskLobby);
bool LobbyTestSameUserDifferentToken_Disconnected::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]()
			{
				bLoginDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	int NumLobbyConnected = 0;
	bool bLobbyDisconnected = false;
	FAccelByteModelsDisconnectNotif DisconnectNotifResponse;
	Lobby.SetConnectSuccessDelegate(FSimpleDelegate::CreateLambda(
			[&]()
			{
				NumLobbyConnected++;
			})
		);
	Lobby.SetConnectionClosedDelegate(Lobby::FConnectionClosed::CreateLambda(
			[&](int32 CloseCode, FString Reason, bool bWasClean)
			{
				bLobbyDisconnected = true;
			})
		);
	Lobby.SetDisconnectNotifDelegate(Lobby::FDisconnectNotif::CreateLambda(
			[&](const FAccelByteModelsDisconnectNotif& Result)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DisconnectNotif: %s"), *Result.Message);
				DisconnectNotifResponse = Result;
			})
		);

	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", DefaultTimeout);

	User.ForgetAllCredentials();

	DelaySeconds(1, TEXT("Waiting 1 sec to make sure different token is generated."));

	bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]()
			{
				bLoginDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby OtherLobby{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };

	OtherLobby.Connect();

	WaitUntil([&]() { return OtherLobby.IsConnected(); }, "", DefaultTimeout);

	WaitUntil([&]() { return bLobbyDisconnected; }, "", DefaultTimeout);

	DelaySeconds(10, TEXT("waiting 10 second"));

	AB_TEST_FALSE(Lobby.IsConnected());
	AB_TEST_TRUE(bLobbyDisconnected);
	AB_TEST_TRUE(OtherLobby.IsConnected());
	AB_TEST_EQUAL(NumLobbyConnected, 1);
	AB_TEST_EQUAL(DisconnectNotifResponse.Message, TEXT("multiple session login detected"));

	Lobby.Disconnect();
	OtherLobby.Disconnect();

	WaitUntil([&]() { return !OtherLobby.IsConnected() && !Lobby.IsConnected(); }, "", 15);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId()
		, FVoidHandler::CreateLambda([&bDeleteDone]()
			{
				bDeleteDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, TEXT("Waiting for Deletion..."), DefaultTimeout);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSameUserSameToken_Disconnected, "AccelByte.Tests.Lobby.C.LobbyTestSameUserSameToken_Disconnected", AutomationFlagMaskLobby);
bool LobbyTestSameUserSameToken_Disconnected::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]()
			{
				bLoginDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	int NumLobbyConnected = 0;
	bool bLobbyDisconnected = false;
	Lobby.SetConnectSuccessDelegate(FSimpleDelegate::CreateLambda(
			[&]()
			{
				NumLobbyConnected++;
			})
		);
	Lobby.SetConnectionClosedDelegate(Lobby::FConnectionClosed::CreateLambda(
			[&](int32 CloseCode, FString Reason, bool bWasClean)
			{
				bLobbyDisconnected = true;
			})
		);

	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", DefaultTimeout);

	AccelByte::Api::Lobby OtherLobby{ FRegistry::Credentials, FRegistry::Settings, FRegistry::HttpRetryScheduler };

	OtherLobby.Connect();

	DelaySeconds(10, TEXT("waiting if lobby is connecting"));

	AB_TEST_TRUE(Lobby.IsConnected());
	AB_TEST_FALSE(bLobbyDisconnected);
	AB_TEST_FALSE(OtherLobby.IsConnected());
	AB_TEST_EQUAL(NumLobbyConnected, 1);

	Lobby.Disconnect();
	OtherLobby.Disconnect();

	WaitUntil([&]() { return !OtherLobby.IsConnected() && !Lobby.IsConnected(); }, "", DefaultTimeout);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId()
		, FVoidHandler::CreateLambda([&bDeleteDone]()
			{
				bDeleteDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, TEXT("Waiting for Deletion..."));

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestReconnect_SameToken_withSessionIdHeader, "AccelByte.Tests.Lobby.C.Reconnect_SameToken_withSessionIdHeader", AutomationFlagMaskLobby);
bool LobbyTestReconnect_SameToken_withSessionIdHeader::RunTest(const FString& Parameters)
{
	//Arrange
	AccelByte::Api::User& User = FRegistry::User;
	
	User.ForgetAllCredentials();

	bool bLoginDone = false;
	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]()
			{
				bLoginDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil([&]()
			{
				return bLoginDone;
			}
		, TEXT("Wait for user to login")
		, DefaultTimeout);

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	Lobby.Disconnect();
	WaitUntil(!Lobby.IsConnected(), TEXT("Wait make sure initial lobby disconnect"), DefaultTimeout);
	
	int NumLobbyConnected = 0;
	Lobby.SetConnectSuccessDelegate(
		FSimpleDelegate::CreateLambda([&]()
			{
				NumLobbyConnected++;
			}));

	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", DefaultTimeout);

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", DefaultTimeout);
	
	//Act
	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", DefaultTimeout);

	bool bIsLobbyConnected = Lobby.IsConnected();

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", DefaultTimeout);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId()
		, FVoidHandler::CreateLambda([&bDeleteDone]()
			{
				bDeleteDone = true;
			})
		, LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, TEXT("Waiting for Deletion..."), DefaultTimeout);
	
	AB_TEST_TRUE(bIsLobbyConnected);
	AB_TEST_TRUE(NumLobbyConnected > 1);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestAccessTokenRefreshed_SendNewAccessToken, "AccelByte.Tests.Lobby.C.LobbyTestAccessTokenRefreshed_SendNewAccessToken", AutomationFlagMaskLobby);
bool LobbyTestAccessTokenRefreshed_SendNewAccessToken::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]()
			{
				bLoginDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;

	bool bRefreshTokenSuccess = false;
	Lobby.SetRefreshTokenDelegate(Api::Lobby::FRefreshTokenResponse::CreateLambda(
			[&bRefreshTokenSuccess](const FAccelByteModelsRefreshTokenResponse& result)
			{
				bRefreshTokenSuccess = true;
			})
		);
	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", 15);

	// set session expired time to 0
	const FString AccessToken = FRegistry::Credentials.GetAccessToken();
	const FString RefreshToken = FRegistry::Credentials.GetRefreshToken();
	FString NewAccessToken = FRegistry::Credentials.GetAccessToken();
	FString NewRefreshToken = FRegistry::Credentials.GetRefreshToken();
	FRegistry::Credentials.ScheduleRefreshToken(FPlatformTime::Seconds() + 2.0);

	WaitUntil([&]()
		{
			NewAccessToken = FRegistry::Credentials.GetAccessToken();
			NewRefreshToken = FRegistry::Credentials.GetRefreshToken();

			return AccessToken != NewAccessToken && RefreshToken != NewRefreshToken;
		}
		, TEXT("Wait refresh token")
		, DefaultTimeout);

	WaitUntil(bRefreshTokenSuccess, TEXT("waiting refresh token to lobby"), DefaultTimeout);

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", DefaultTimeout);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId()
		, FVoidHandler::CreateLambda([&bDeleteDone]()
			{
				bDeleteDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, TEXT("Waiting for Deletion..."), DefaultTimeout);

	AB_TEST_TRUE(bRefreshTokenSuccess);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestAccessTokenRefreshed_SendNewAccessTokenMultipleTime, "AccelByte.Tests.Lobby.C.LobbyTestAccessTokenRefreshed_SendNewAccessToken_MultipleTime", AutomationFlagMaskLobby);
bool LobbyTestAccessTokenRefreshed_SendNewAccessTokenMultipleTime::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]()
			{
				bLoginDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil([&]()
			{
				return bLoginDone;
			}
		, TEXT("Waiting for user to login")
		, DefaultTimeout);

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;

	bool bRefreshTokenSuccess = false;
	int RefreshTokenSuccessCount = 0;
	Lobby.SetRefreshTokenDelegate(Api::Lobby::FRefreshTokenResponse::CreateLambda(
			[&bRefreshTokenSuccess, &RefreshTokenSuccessCount](const FAccelByteModelsRefreshTokenResponse& result)
			{
				bRefreshTokenSuccess = true;
				RefreshTokenSuccessCount++;
			})
		);
	Lobby.Connect();

	WaitUntil([&]()
			{
				return Lobby.IsConnected();
			}
		, ""
		, DefaultTimeout);

	// set session expired time to 0
	const FString AccessToken = FRegistry::Credentials.GetAccessToken();
	const FString RefreshToken = FRegistry::Credentials.GetRefreshToken();
	FString NewAccessToken = FRegistry::Credentials.GetAccessToken();
	FString NewRefreshToken = FRegistry::Credentials.GetRefreshToken();
	FRegistry::Credentials.ScheduleRefreshToken(FPlatformTime::Seconds() + 2.0);

	WaitUntil([&]()
			{
				NewAccessToken = FRegistry::Credentials.GetAccessToken();
				NewRefreshToken = FRegistry::Credentials.GetRefreshToken();

				return AccessToken != NewAccessToken && RefreshToken != NewRefreshToken;
			}
		, TEXT("Wait refresh token")
		, DefaultTimeout);

	WaitUntil(bRefreshTokenSuccess, TEXT("waiting refresh token to lobby"), DefaultTimeout);

	// set session expired time to 0 again
	bRefreshTokenSuccess = false;
	const FString AccessToken1 = FRegistry::Credentials.GetAccessToken();
	const FString RefreshToken1 = FRegistry::Credentials.GetRefreshToken();
	FString NewAccessToken1 = FRegistry::Credentials.GetAccessToken();
	FString NewRefreshToken1 = FRegistry::Credentials.GetRefreshToken();
	FRegistry::Credentials.ScheduleRefreshToken(FPlatformTime::Seconds() + 2.0);

	WaitUntil([&]()
			{
				NewAccessToken1 = FRegistry::Credentials.GetAccessToken();
				NewRefreshToken1 = FRegistry::Credentials.GetRefreshToken();

				return AccessToken1 != NewAccessToken1 && RefreshToken1 != NewRefreshToken1;
			}
		, TEXT("Wait refresh token1")
		, DefaultTimeout);

	WaitUntil(bRefreshTokenSuccess, TEXT("waiting refresh token to lobby1"), DefaultTimeout);

	Lobby.Disconnect();

	WaitUntil([&]()
			{
				return !Lobby.IsConnected();
			}
		, ""
		, DefaultTimeout);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId()
		, FVoidHandler::CreateLambda([&bDeleteDone]()
			{
				bDeleteDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, TEXT("Waiting for Deletion..."), DefaultTimeout);

	AB_TEST_EQUAL(RefreshTokenSuccessCount, 2);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestInvalidTokenRefresh_Failed, "AccelByte.Tests.Lobby.C.LobbyTestAccessTokenRefreshInvalid_Failed", AutomationFlagMaskLobby);
bool LobbyTestInvalidTokenRefresh_Failed::RunTest(const FString& Parameters)
{
	AccelByte::Api::User& User = FRegistry::User;
	bool bLoginDone = false;

	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]()
			{
				bLoginDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil([&]()
			{
				return bLoginDone;
			}
		, TEXT("")
		, DefaultTimeout);

	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;

	bool bRefreshTokenSuccess = false;
	FString RefreshTokenCode = "0";
	Lobby.SetRefreshTokenDelegate(Api::Lobby::FRefreshTokenResponse::CreateLambda(
			[&bRefreshTokenSuccess, &RefreshTokenCode](const FAccelByteModelsRefreshTokenResponse& result)
			{
				bRefreshTokenSuccess = true;
				RefreshTokenCode = result.Code;
			})
		);
	Lobby.Connect();

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", DefaultTimeout);

	Lobby.RefreshToken(TEXT("Invalid"));

	WaitUntil(bRefreshTokenSuccess, TEXT("waiting refresh lobby token"), DefaultTimeout);

	Lobby.Disconnect();

	WaitUntil([&]() { return !Lobby.IsConnected(); }, "", DefaultTimeout);

	bool bDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId()
		, FVoidHandler::CreateLambda([&bDeleteDone]()
			{
				bDeleteDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bDeleteDone, TEXT("Waiting for Deletion..."), DefaultTimeout);

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

	LobbyApiClients[0]->Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	LobbyApiClients[1]->Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	LobbyApiClients[0]->Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);
	LobbyApiClients[1]->Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);

	LobbyApiClients[0]->Lobby.Connect();

	WaitUntil([]()->bool
			{
				return LobbyApiClients[0]->Lobby.IsConnected() && bUsersConnectionSuccess;
			}
		, TEXT("LobbyTestSignalingP2P: Waiting for user 0 to connect...")
		, DefaultTimeout
		, DefaultDeltaTime);
	
	userResponded[0] = bUsersConnectionSuccess;
	userConnected[0] = bUsersConnected;
	bUsersConnectionSuccess = false;
	bUsersConnected = false;

	LobbyApiClients[1]->Lobby.Connect();

	WaitUntil([]()->bool
			{
				return LobbyApiClients[1]->Lobby.IsConnected() && bUsersConnectionSuccess;
			}
		, TEXT("LobbyTestSignalingP2P: Waiting for user 1 to connect...")
		, DefaultTimeout
		, DefaultDeltaTime);
	
	userResponded[1] = bUsersConnectionSuccess;
	userConnected[1] = bUsersConnected;

	for (int i = 0; i < 2; i++)
	{
		AB_TEST_TRUE(userConnected[i]);
		AB_TEST_TRUE(userResponded[i]);
	}

	LobbyApiClients[0]->Lobby.SetSignalingP2PDelegate(OnP2PDelegate);
	LobbyApiClients[1]->Lobby.SetSignalingP2PDelegate(OnP2PDelegate);
	
	LobbyApiClients[0]->Lobby.SendSignalingMessage(LobbyUsers[1].UserId, Message);
	WaitUntil([&P2PMessageAvailable]() ->bool
			{
				return P2PMessageAvailable;
			}
		, TEXT("LobbyTestSignalingP2P: Waiting for P2P signals from user 0")
		, DefaultTimeout);

	AB_TEST_TRUE(P2PMessageAvailable);
	AB_TEST_EQUAL(P2PMessage, Message);
	AB_TEST_EQUAL(P2PDestinationId, LobbyUsers[0].UserId);

	P2PMessageAvailable = false;
	LobbyApiClients[1]->Lobby.SendSignalingMessage(LobbyUsers[0].UserId, Message);
	WaitUntil([&P2PMessageAvailable]() ->bool
			{
				return P2PMessageAvailable;
			}
		, TEXT("LobbyTestSignalingP2P: Waiting for P2P signals from user 1")
		, DefaultTimeout);

	AB_TEST_TRUE(P2PMessageAvailable);
	AB_TEST_EQUAL(P2PMessage, Message);
	AB_TEST_EQUAL(P2PDestinationId, LobbyUsers[1].UserId);
	
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
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda(
			[&bCreatePartyDone, &CreatePartyResult](const FAccelByteModelsCreatePartyResponse result)
			{
				CreatePartyResult = result;
				bCreatePartyDone = true;
			})
		, FErrorHandler::CreateLambda([&bCreatePartyDone, &bError, &ErrorCode, &ErrorMessage](const int32 code, const FString& message)
			{
				bCreatePartyDone = true;
				bError = true;
				ErrorCode = code;
				ErrorMessage = message;
			})
		);
	
	// create party
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, TEXT("waiting create party"), DefaultTimeout);

	// crate party again, error
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, TEXT("waiting create party2"), DefaultTimeout);

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
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&bCreatePartyDone, &CreatePartyResult](const FAccelByteModelsCreatePartyResponse result)
	{
		CreatePartyResult = result;
		bCreatePartyDone = true;
	}));
	
	// create party
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, TEXT("waiting create party"), DefaultTimeout);

	// crate party again, error
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, TEXT("waiting create party2"), DefaultTimeout);

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
	
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartySuccess, CreatePartyError);
	
	// create party
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, TEXT("waiting create party"), DefaultTimeout);

	// Clear error message, then test error creating party
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.ClearLobbyErrorMessages();
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, TEXT("waiting create party2"), DefaultTimeout);
	bool bIsErrorAfterClear(bError);
	int32 ErrorCodeAfterClear(ErrorCode);
	FString ErrorMessageAfterClear(ErrorMessage);
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create party after clear error message responded with code %d, codename %s"), ErrorCode, *ErrorMessage);

	DelaySeconds(3, TEXT("Delaying 3 sec"));
	
	// Disconnect Lobby, reapply setup then try again.
	LobbyDisconnect(1);
	WaitUntil(!LobbyApiClients[0]->Lobby.IsConnected(), TEXT("Waiting lobby disconnect"), DefaultTimeout);
	
	LobbyConnect(1);
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartySuccess, CreatePartyError);
	WaitUntil(LobbyApiClients[0]->Lobby.IsConnected(), TEXT("Wait reconnecting lobby"), DefaultTimeout);

	// Create party again after disconnect
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, TEXT("waiting create party"), DefaultTimeout);

	// Create party again to test error after reconnecting
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, TEXT("waiting create party2"), DefaultTimeout);
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
			})
		, LobbyTestErrorHandler);
	WaitUntil(bGetConfigSuccess, TEXT("Waiting Get Config..."), DefaultTimeout);
	AB_TEST_TRUE(bGetConfigSuccess);

	FLobbyModelConfig ShortBurstConfig = DefaultConfig;
	ShortBurstConfig.GeneralRateLimitBurst = ShortLimitBurst;
	ShortBurstConfig.GeneralRateLimitDuration = HighLimitDuration;

	bool bSetConfigSuccess = false;
	AdminSetLobbyConfig(ShortBurstConfig
		, THandler<FLobbyModelConfig>::CreateLambda([&](const FLobbyModelConfig& Response)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Setting config success"));
				bSetConfigSuccess = true;
			})
		, LobbyTestErrorHandler);
	WaitUntil(bSetConfigSuccess, TEXT("Waiting Set Config..."), DefaultTimeout);
	AB_TEST_TRUE(bSetConfigSuccess);
	
	bool bReachedBurst = false;
	LobbyApiClients[0]->Lobby.SetErrorNotifDelegate(Lobby::FErrorNotif::CreateLambda(
			[&](int32 Code, const FString& Message)
			{
				// Too many request
				if(Code == 429)
				{
					bReachedBurst = true;
				}
			})
		);
	int SendRequestCounter = 0;
	while(SendRequestCounter <= ShortLimitBurst)
	{
		LobbyApiClients[0]->Lobby.SendGetOnlineUsersRequest();
		SendRequestCounter++;
	}
	WaitUntil(bReachedBurst, TEXT("Waiting to reach burst limit"), DefaultTimeout);
	AB_TEST_TRUE(bReachedBurst);
	
	bSetConfigSuccess = false;
	AdminSetLobbyConfig(DefaultConfig
		, THandler<FLobbyModelConfig>::CreateLambda([&](const FLobbyModelConfig& Response)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Setting config to default success"));
				bSetConfigSuccess = true;
			})
		, LobbyTestErrorHandler);
	WaitUntil(bSetConfigSuccess, TEXT("Waiting Set Back Default Configuration..."), DefaultTimeout);
	AB_TEST_TRUE(bSetConfigSuccess);

	LobbyDisconnect(1);
	ResetResponses();
	
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

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("CreateEmailAccount"));
	User.Register(EmailAddress
		, Password
		, DisplayName
		, Country
		, format
		, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("   Success"));
				bRegisterSuccessful = true;
				bRegisterDone = true;
			})
		, FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteLobbyTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			})
		);

	FlushHttpRequests();
	WaitUntil(bRegisterDone, TEXT("Waiting for Registered..."), DefaultTimeout);

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress
		, Password
		, FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
				bLoginSuccessful = true;
			})
		, FCustomErrorHandler::CreateLambda([](int Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
			})
		);

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, TEXT("Waiting for Login..."), DefaultTimeout);

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
	FBanUserRequest body =
	{
		EBanType::MATCHMAKING,
		"User Ban Test",
		(FDateTime::Now() + FTimespan::FromSeconds(180)).ToIso8601(),
		EBanReason::IMPERSONATION,
		false
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

	bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
			FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
			{
				bServerLoginWithClientCredentialsDone = true;
			})
		, LobbyTestErrorHandler);
	WaitUntil(bServerLoginWithClientCredentialsDone, TEXT("Server Login With Client Credentials"), DefaultTimeout);

	AB_TEST_TRUE(bServerLoginWithClientCredentialsDone);
	
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("BanUser"));
	FRegistry::ServerUser.BanSingleUser(FRegistry::Credentials.GetUserId()
		, body 
		, THandler<FBanUserResponse>::CreateLambda([&bBanSuccessful, &BanId](const FBanUserResponse& Result)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned: %s"), *Result.UserId);
				BanId = Result.BanId;
				bBanSuccessful = true;
			})
		, LobbyTestErrorHandler);

	FlushHttpRequests();

	WaitUntil(bBanSuccessful, TEXT("Waiting for Ban..."), DefaultTimeout);
	WaitUntil(bUserBannedNotif, TEXT("Waiting Ban Notification..."), DefaultTimeout);

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, TEXT("Waiting Lobby Connection Closed..."), 65.0);
	bool bUserBannedLobbyConnectionClosed = bLobbyConnectionClosed && !Lobby.IsConnected();

	// Wait Lobby connection auto reconnect
	WaitUntil(bUsersConnected, TEXT("Waiting Lobby Reconnected..."), DefaultTimeout);
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
	AdminBanUserChangeStatus(UserId
		, BanId
		, false
		, THandler<FBanUserResponse>::CreateLambda([&bUnbanSuccessful](const FBanUserResponse& Result)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Unbanned: %s"), *Result.UserId);
				bUnbanSuccessful = true;
			})
		, LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bUnbanSuccessful, TEXT("Waiting for Unban..."), DefaultTimeout);
	WaitUntil(bUsersUnbannedNotif, TEXT("Waiting Unban Notification..."), DefaultTimeout);

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, TEXT("Waiting Lobby Connection Closed..."), 65.0);
	bool bUserUnbannedLobbyConnectionClosed = bLobbyConnectionClosed && !Lobby.IsConnected();

	// Wait Lobby connection auto reconnect
	WaitUntil(bUsersConnected, TEXT("Waiting Lobby Reconnected..."), DefaultTimeout);
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
	AdminBanUserChangeStatus(UserId
		, BanId
		, true
		, THandler<FBanUserResponse>::CreateLambda([&bEnableBanSuccessful](const FBanUserResponse& Result)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned: %s"), *Result.UserId);
				bEnableBanSuccessful = true;
			})
		, LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bEnableBanSuccessful, TEXT("Waiting for Enable Ban..."), DefaultTimeout);
	WaitUntil(bUserBannedNotif, TEXT("Waiting Ban Notification..."), DefaultTimeout);

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, TEXT("Waiting Lobby Connection Closed..."), 65.0);
	bool bUserBanEnableLobbyConnectionClosed = bLobbyConnectionClosed && !FRegistry::Lobby.IsConnected();

	// Wait Lobby connection auto reconnect
	WaitUntil(bUsersConnected, TEXT("Waiting Lobby Reconnected..."), DefaultTimeout);
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
	AdminDeleteUser(UserId
		, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
				bDeleteSuccessful = true;
				bDeleteDone = true;
			})
		, LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, TEXT("Waiting for Deletion..."), DefaultTimeout);

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

	bool bRegisterSuccessful = false;
	bool bRegisterDone = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("CreateEmailAccount"));
	User.Register(EmailAddress
		, Password
		, DisplayName
		, Country
		, format
		, THandler<FRegisterResponse>::CreateLambda([&bRegisterSuccessful, &bRegisterDone](const FRegisterResponse& Result)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("   Success"));
				bRegisterSuccessful = true;
				bRegisterDone = true;
			})
		, FErrorHandler::CreateLambda([&bRegisterDone](int32 ErrorCode, const FString& ErrorMessage)
			{
				UE_LOG(LogAccelByteLobbyTest, Warning, TEXT("    Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage);
				bRegisterDone = true;
			})
		);

	FlushHttpRequests();
	WaitUntil(bRegisterDone, TEXT("Waiting for Registered..."), DefaultTimeout);

	bool bLoginSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LoginWithUsernameAndPassword"));
	FRegistry::User.LoginWithUsername(EmailAddress
		, Password
		, FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
				bLoginSuccessful = true;
			})
		, FCustomErrorHandler::CreateLambda([](int Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
			})
		);

	FlushHttpRequests();
	WaitUntil(bLoginSuccessful, TEXT("Waiting for Login..."), DefaultTimeout);

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

	WaitUntil([&]() { return Lobby.IsConnected(); }, "", DefaultTimeout);

	//Ban
	FBanUserRequest body =
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
	bool bServerLoginWithClientCredentialsDone = false;
	FRegistry::ServerOauth2.LoginWithClientCredentials(
			FVoidHandler::CreateLambda([&bServerLoginWithClientCredentialsDone]()
			{
				bServerLoginWithClientCredentialsDone = true;
			})
		, LobbyTestErrorHandler);
	WaitUntil(bServerLoginWithClientCredentialsDone, TEXT("Server Login With Client Credentials"), DefaultTimeout);

	AB_TEST_TRUE(bServerLoginWithClientCredentialsDone);
	 
	FRegistry::ServerUser.BanSingleUser(FRegistry::Credentials.GetUserId()
		, body 
		, THandler<FBanUserResponse>::CreateLambda([&bBanSuccessful, &BanId](const FBanUserResponse& Result)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned: %s"), *Result.UserId);
				BanId = Result.BanId;
				bBanSuccessful = true;
			})
		, LobbyTestErrorHandler);

	FlushHttpRequests();

	WaitUntil(bBanSuccessful, TEXT("Waiting for Ban..."), DefaultTimeout);

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, TEXT("Waiting Lobby Connection Closed..."), 65.0);
	WaitUntil(bLobbyDisconnected, TEXT("Waiting Lobby Disconnected..."), 65.0);
	bool bUserBannedNotifReceived = bUserBannedNotif;
	bool bUserBannedLobbyDisconnected = bLobbyConnectionClosed && bLobbyDisconnected && !Lobby.IsConnected();

	bLobbyConnectionClosed = false;
	bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	DelaySeconds(10, TEXT("Delay 10 sec"));

	Lobby.Connect();
	WaitUntil(bUsersConnectionSuccess, TEXT("Wait Lobby Connect"), DefaultTimeout);
	bool bBannedLobbyConnectFailed = !bUsersConnected;

	//try to relogin
	FRegistry::Credentials.ForgetAll();
	bLoginSuccessful = false;
	bool bLoginError = false;
	bool bLoginDone = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LoginWithUsernameAndPassword"));
	User.LoginWithUsername(EmailAddress
		, Password
		, FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
				bLoginDone = true;
			})
		, FCustomErrorHandler::CreateLambda([&bLoginError, &bLoginDone](int Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
				bLoginError = true;
				bLoginDone = true;
			})
		);

	FlushHttpRequests();
	WaitUntil(bLoginDone, TEXT("Waiting for Login..."), DefaultTimeout);
	bool bLoginFailedUserBanned = bLoginError;

	//Unban
	bool bUnbanSuccessful = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("UnbanUser"));
	AdminBanUserChangeStatus(UserId
		, BanId
		, false
		, THandler<FBanUserResponse>::CreateLambda([&bUnbanSuccessful](const FBanUserResponse& Result)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Unbanned: %s"), *Result.UserId);
				bUnbanSuccessful = true;
			})
		, LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bUnbanSuccessful, TEXT("Waiting for Unban..."), DefaultTimeout);

	//try to relogin
	FRegistry::Credentials.ForgetAll();
	bLoginSuccessful = false;
	bLoginDone = false;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LoginWithUsernameAndPassword"));
	User.LoginWithUsername(EmailAddress
		, Password
		, FVoidHandler::CreateLambda([&]()
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
				bLoginSuccessful = true;
				bLoginDone = true;
			})
		, FCustomErrorHandler::CreateLambda([&bLoginDone](int Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
				bLoginDone = true;
			})
		);

	FlushHttpRequests();
	WaitUntil(bLoginDone, TEXT("Waiting for Login..."), DefaultTimeout);
	bool bLoginSuccessUnbannedUser = bLoginSuccessful;

	bLobbyConnectionClosed = false;
	bLobbyDisconnected = false;
	bUserBannedNotif = false;
	bUsersUnbannedNotif = false;
	bUsersConnected = false;
	bUsersConnectionSuccess = false;

	Lobby.Connect();
	WaitUntil(bUsersConnectionSuccess, TEXT("Wait Lobby Connect"), DefaultTimeout);
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
	AdminDeleteUser(UserId
		, FVoidHandler::CreateLambda([&bDeleteDone, &bDeleteSuccessful]()
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Success"));
				bDeleteSuccessful = true;
				bDeleteDone = true;
			})
		, LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bDeleteDone, TEXT("Waiting for Deletion..."), DefaultTimeout);

	AB_TEST_TRUE(bDeleteSuccessful);

#pragma endregion DeleteUserById

	return true;
}

#if 0 // don't automatically run this tests.
/***********************************************************************************
* Setup needed to run test
* 
* 1. create an item that have appId in publisher namespace
* 2. enable the entitlement check in lobby config
* 3. add item id for entitlement check in lobby config
* 4. put the appId in SKU var in each test
* 
* Don't forget to disable entitlement check in lobby config to test another lobby tests. 
***********************************************************************************/
IMPLEMENT_SIMPLE_AUTOMATION_TEST(EntitlementNotOwned, "AccelByte.Tests.Lobby.G.EntitlementNotOwned", AutomationFlagMaskLobby);
bool EntitlementNotOwned::RunTest(const FString& Parameter)
{
	// Arrange
	const FString Sku("sdktestSkuApp001");

	AccelByte::Api::User& User = FRegistry::User;
	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	
	// login user
	bool bUserLoggedIn = false;
	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoggedIn]()
	{
		bUserLoggedIn = true;
	}), LobbyTestErrorHandler);

	WaitUntil(bUserLoggedIn, TEXT("Waiting user logged in"), DefaultTimeout);

	// Set Lobby Delegates
	bool bLobbyConnected = false;
	Lobby.SetConnectSuccessDelegate(Api::Lobby::FConnectSuccess::CreateLambda([&bLobbyConnected]()
	{
		bLobbyConnected = true;
	}));
	
	bool bLobbyConnectError = false;
	Lobby.SetConnectFailedDelegate(FErrorHandler::CreateLambda([&bLobbyConnectError](const int code, const FString& Message)
	{
		bLobbyConnectError = true;
	}));
	
	// set entitlement ownership token generator
	FTokenGeneratorParams GeneratorParams;
	GeneratorParams.Skus = {Sku};
	Lobby.SetTokenGenerator(MakeShared<FAccelByteEntitlementTokenGenerator>(GeneratorParams));
	
	// Act
	Lobby.Connect();
	DelaySeconds(10, TEXT("Waiting Lobby Connecting"));

	Lobby.Disconnect();
	DelaySeconds(3, TEXT("Waiting lobby disconnect"));
	
	bool bUserDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId()
		, FSimpleDelegate::CreateLambda([&bUserDeleteDone]()
			{
				bUserDeleteDone = true;
			})
		, LobbyTestErrorHandler);
	WaitUntil(bUserDeleteDone, TEXT("Waiting delete user for cleanup"), DefaultTimeout);

	// Assert
	AB_TEST_FALSE(bLobbyConnected);
	AB_TEST_TRUE(bLobbyConnectError);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(EntilementOwned, "AccelByte.Tests.Lobby.G.EntilementOwned", AutomationFlagMaskLobby);
bool EntilementOwned::RunTest(const FString& Parameter)
{	
	// Arrange Setup Item
	//const FString Sku("apiseed1635893021d5ec135");
	const FString Sku("APPG0101D");
	const FString PublisherNamespace(GetPublisherNamespace());

	// Arrange Get Publisher Store ID
	FString StoreId = "";
	bool bGetPublishedStoreDone = false;
	AdminGetEcommercePublishedStore(PublisherNamespace
		, THandler<FStoreInfo>::CreateLambda([&StoreId, &bGetPublishedStoreDone](const FStoreInfo& Result)
			{
				StoreId = Result.storeId;
				bGetPublishedStoreDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bGetPublishedStoreDone, TEXT("Waiting get store ID from published store"), DefaultTimeout);
	
	// Arrange Get itemID from SKU
	FString ItemId = "";
	bool bGetItemIdDone = false;
	AdminGetEcommerceItemBySKU(PublisherNamespace
		, StoreId
		, Sku
		, false
		, THandler<FItemFullInfo>::CreateLambda([&ItemId, &bGetItemIdDone](const FItemFullInfo& result)
			{
				ItemId = result.itemId;
				bGetItemIdDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bGetItemIdDone, TEXT("Waiting get Item Id from SKU"), DefaultTimeout);
	
	// Arrange
	AccelByte::Api::User& User = FRegistry::User;
	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	AccelByte::Credentials& Credentials = FRegistry::Credentials;
	
	// Arrange login user
	bool bUserLoggedIn = false;
	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoggedIn]()
			{
				bUserLoggedIn = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bUserLoggedIn, TEXT("Waiting user logged in"), DefaultTimeout);

	// Arrange Get User Publisher Id
	FString PublisherUserId = "";
	bool bGetPublisherUserId = false;
	AdminGetUserMap(Credentials.GetUserId()
		, THandler<FUserMapResponse>::CreateLambda([&PublisherUserId, &bGetPublisherUserId](const FUserMapResponse& result)
			{
				PublisherUserId = result.userId;
				bGetPublisherUserId = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bGetPublisherUserId, TEXT("Waiting get publisher user id"), DefaultTimeout);
	
	// Arrange Grant App to user
	FFulfillmentRequest ToGrant;
	ToGrant.ItemId = ItemId;
	ToGrant.Quantity = 1;
	ToGrant.EndDate = (FDateTime::UtcNow() + FTimespan::FromMinutes(10)).ToIso8601();

	bool bItemGrantDone = false;
	AdminFulfillItem(PublisherNamespace
		, PublisherUserId
		, ToGrant
		, FVoidHandler::CreateLambda([&bItemGrantDone]()
			{
				bItemGrantDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bItemGrantDone, TEXT("Waiting granting Item to user"), DefaultTimeout);

	// Arrange Set Lobby Delegates
	bool bLobbyConnected = false;
	Lobby.SetConnectSuccessDelegate(Api::Lobby::FConnectSuccess::CreateLambda([&bLobbyConnected]()
	{
		bLobbyConnected = true;
	}));
	
	bool bLobbyConnectError = false;
	Lobby.SetConnectFailedDelegate(FErrorHandler::CreateLambda([&bLobbyConnectError](const int code, const FString& Message)
	{
		bLobbyConnectError = true;
	}));
	
	// Arrange set entitlement ownership token generator
	FTokenGeneratorParams GeneratorParams;
	GeneratorParams.Skus = { Sku };
	Lobby.SetTokenGenerator(MakeShared<FAccelByteEntitlementTokenGenerator>(GeneratorParams));
	
	// Act
	Lobby.Connect();
	DelaySeconds(3, TEXT("Waiting Lobby Connecting"));

	Lobby.Disconnect();
	DelaySeconds(3, TEXT("Waiting lobby disconnect"));

	bool bUserDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId()
		, FSimpleDelegate::CreateLambda([&bUserDeleteDone]()
			{
				bUserDeleteDone = true;
			})
		, LobbyTestErrorHandler);
	WaitUntil(bUserDeleteDone, TEXT("Waiting delete user for cleanup"), DefaultTimeout);

	// Assert
	AB_TEST_TRUE(bLobbyConnected);
	AB_TEST_FALSE(bLobbyConnectError);

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(SetUnsetTokenGenerator, "AccelByte.Tests.Lobby.G.SetUnsetTokenGenerator", AutomationFlagMaskLobby);
bool SetUnsetTokenGenerator::RunTest(const FString& Parameter)
{
	// Arrange
	const FString Sku("APPG0101D");

	AccelByte::Api::User& User = FRegistry::User;
	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	
	// login user
	bool bUserLoggedIn = false;
	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoggedIn]()
			{
				bUserLoggedIn = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bUserLoggedIn, TEXT("Waiting user logged in"), DefaultTimeout);

	// Set Lobby Delegates
	bool bLobbyConnected = false;
	Lobby.SetConnectSuccessDelegate(Api::Lobby::FConnectSuccess::CreateLambda([&bLobbyConnected]()
	{
		bLobbyConnected = true;
	}));
	
	bool bLobbyConnectError = false;
	Lobby.SetConnectFailedDelegate(FErrorHandler::CreateLambda([&bLobbyConnectError](const int code, const FString& Message)
	{
		bLobbyConnectError = true;
	}));
	
	// set entitlement ownership token generator
	FTokenGeneratorParams GeneratorParams;
	GeneratorParams.Skus = {Sku};
	Lobby.SetTokenGenerator(MakeShared<FAccelByteEntitlementTokenGenerator>(GeneratorParams));
	
	Lobby.SetTokenGenerator(nullptr);
	
	// Act
	Lobby.Connect();
	DelaySeconds(10, TEXT("Waiting Lobby Connecting"));

	Lobby.Disconnect();
	DelaySeconds(3, TEXT("Waiting lobby disconnect"));
	
	bool bUserDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId()
		, FSimpleDelegate::CreateLambda([&bUserDeleteDone]()
			{
				bUserDeleteDone = true;
			})
		, LobbyTestErrorHandler);
	WaitUntil(bUserDeleteDone, TEXT("Waiting delete user for cleanup"), DefaultTimeout);

	// Assert
	AB_TEST_TRUE(bLobbyConnected);

	return true;
}
#endif // lobby connect with entitlement check test

#pragma region LobbyMultithreadTesting
/* test is unstable, need to properly debug why test seems to stuck if some lobby is failed connecting or failed to receive invite
class FTestLobbyUser : FRunnable
{
private:
	// Thread handle. Control the thread using this, with operators like Kill and Suspend
	FRunnableThread* Thread;

	// Used to know when the thread should exit, changed in Stop(), read in Run()
	bool bThreadStopping {false};

	Credentials& TestCredentials;
	const Settings& TestSetting;
	FHttpRetryScheduler& HttpRef;
	const FString& ThreadName;
	const Lobby::FConnectSuccess& GetConnectSuccessDelegate;
	const Lobby::FPartyGetInvitedNotif& GetMessageNotifDelegate;
	float DelayTime {0};
	float WaitTimeOut{30};
	

public:
	virtual uint32 Run() override 
	{
		FPlatformProcess::Sleep(DelayTime);

		FDateTime TestStartTime {FDateTime::UtcNow()};
		Lobby TestLobby {TestCredentials, TestSetting, HttpRef};

		bool bLobbyConnected {false};
		const auto OnLobbyConnected {FVoidHandler::CreateLambda([&]()
		{
			GetConnectSuccessDelegate.ExecuteIfBound();
			bLobbyConnected = true;
		})};

		TestLobby.SetConnectSuccessDelegate(OnLobbyConnected);
		TestLobby.SetPartyGetInvitedNotifDelegate(GetMessageNotifDelegate);

		TestLobby.Connect();
		float testElapsedTime {0};
		float threadSleepTime {0.1f};
		UE_LOG(LogAccelByteLobbyTest, Display, TEXT("connecting lobby from thread"));
		while(!bLobbyConnected || !bThreadStopping || testElapsedTime < WaitTimeOut)
		{
			FPlatformProcess::Sleep(threadSleepTime);
			testElapsedTime += threadSleepTime;
		}

		TestLobby.SendLeavePartyRequest();

		while(!bThreadStopping || testElapsedTime < WaitTimeOut)
		{
			FPlatformProcess::Sleep(threadSleepTime);
			testElapsedTime += threadSleepTime;
		}

		TestLobby.Disconnect();

		UE_LOG(LogAccelByteLobbyTest, Display, TEXT("Disconnected from thread"));
		if(testElapsedTime < WaitTimeOut)
		{
			return 124; // test timed out
		}
		
		return 0;
	}

	void EnsureCompletion()
	{
		bThreadStopping = true;
		Stop();
		Thread->WaitForCompletion();
	}
	
	FTestLobbyUser(const FString& ThreadName, float DelayTime, const Lobby::FConnectSuccess& ConnectedDelegate, const Lobby::FPartyGetInvitedNotif& GetMessageNotifDelegate, Credentials& TestCredentials, const Settings& TestSetting, FHttpRetryScheduler& HttpRef) :
	TestCredentials(TestCredentials),
	TestSetting(TestSetting),
	HttpRef(HttpRef),
	ThreadName(ThreadName),
	GetConnectSuccessDelegate(ConnectedDelegate),
	GetMessageNotifDelegate(GetMessageNotifDelegate),
	DelayTime(DelayTime)
	{
		Thread = FRunnableThread::Create(this, *ThreadName);
	}
	
	
	virtual ~FTestLobbyUser() override
	{
		delete Thread;
		Thread = nullptr;
	}
};

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FLobbyTestConcurrency, "AccelByte.Tests.Lobby.F.Concurrent", AutomationFlagMaskLobby);
bool FLobbyTestConcurrency::RunTest(const FString& Parameter)
{
	const int32 TestCount = 4;
	TArray<TSharedPtr<FTestLobbyUser>> TestLobbyUsers;
	TArray<FString> TestUserIds;

	// Arrange Inviter
	TSharedPtr<Lobby> InviterLobby {Lobbies[TestCount]};
	bool bInviterLeavePartyDone {false};
	InviterLobby->SetLeavePartyResponseDelegate(Lobby::FPartyLeaveResponse::CreateLambda([&bInviterLeavePartyDone](const FAccelByteModelsLeavePartyResponse&)
	{
		bInviterLeavePartyDone = true;
	}));

	bool bInviterCreatePartyDone {false};
	InviterLobby->SetCreatePartyResponseDelegate(Lobby::FPartyCreateResponse::CreateLambda([&bInviterCreatePartyDone](const FAccelByteModelsCreatePartyResponse&)
	{
		bInviterCreatePartyDone = true;
	}));
	
	InviterLobby->Connect();
	WaitUntil([&InviterLobby](){ return InviterLobby->IsConnected(); }, "Waiting inviter lobby to connect");

	InviterLobby->SendLeavePartyRequest();
	WaitUntil(bInviterLeavePartyDone);
	
	InviterLobby->SendCreatePartyRequest();
	WaitUntil(bInviterCreatePartyDone);

	FThreadSafeCounter GetLobbyConnectedCounter;
	const auto OnLobbyConnected {FVoidHandler::CreateLambda([&GetLobbyConnectedCounter]()
		{
			GetLobbyConnectedCounter.Increment();
		})};
	
	FThreadSafeCounter GetPartyInviteCounter;
	const auto GetMessageNotifDelegateHandler = Lobby::FPartyGetInvitedNotif::CreateLambda([&GetPartyInviteCounter](const FAccelByteModelsPartyGetInvitedNotice&)
	{
		GetPartyInviteCounter.Increment();
	});

	for(int i = 0; i < TestCount; i++)
	{
		const int32 Count = i;
		FString ThreadName = FString::Printf(TEXT("Lobby_Concurrency_%03d"), Count);
		TSharedPtr<FTestLobbyUser> Task = MakeShared<FTestLobbyUser>(ThreadName, 0.3f, OnLobbyConnected, GetMessageNotifDelegateHandler, UserCreds[Count], FRegistry::Settings, FRegistry::HttpRetryScheduler);
		TestLobbyUsers.Add(Task);
		TestUserIds.Add(UserCreds[Count].GetUserId());
	}

	WaitUntil([&GetLobbyConnectedCounter, &TestCount]()
	{
		return GetLobbyConnectedCounter.GetValue() == TestCount;
	}, "Waiting lobby connected in thread");

	for(const auto& TestUserId : TestUserIds)
	{
		InviterLobby->SendInviteToPartyRequest(TestUserId);
	}

	WaitUntil([&GetPartyInviteCounter, &TestCount]()
	{
		return GetPartyInviteCounter.GetValue() == TestCount;
	}, "Waiting to get all party info response");

	LobbyDisconnect(TestCount+1); // +1 is inviter
	ResetResponses();
	for(const auto& Task : TestLobbyUsers)
	{
		Task->EnsureCompletion();
	}

	AB_TEST_EQUAL(GetPartyInviteCounter.GetValue(), TestCount);
	return true;
}
*/
#pragma endregion LobbyMultithreadTesting