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
#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "GameServerApi/AccelByteServerUserApi.h"
#include "TestUtilities.h"
#include "UserTestAdmin.h"
#include "LobbyTestAdmin.h"
#include "MatchmakingTestAdmin.h"
	
#include <IPAddress.h>
#include <SocketSubsystem.h>

#include "Core/AccelByteEntitlementTokenGenerator.h"

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

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
TArray<FTestUser> LobbyUsers;
TArray<FApiClientPtr> LobbyApiClients;
TArray<TPair<FString, float>> PreferredLatencies;
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

FString ChannelName;
FString ChannelNameSubGameMode;
const TArray<FString> SubGameModeNames {"1v1", "4pffa"};
FString ChannelNameExtraAttributeTest;
FString ChannelNameNewSessionTest;
FString ChannelNameTempParty2;
FString ChannelNameTest3v3;

inline static bool LatenciesPredicate(const TPair<FString, float>& left, const TPair<FString, float>& right)
{
	return left.Value < right.Value;
}

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
		
		while (!ApiClient->Lobby.IsConnected())
		{
			FPlatformProcess::Sleep(.5f);
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LobbyConnect: Wait user %s to connect")
				, *ApiClient->CredentialsRef->GetUserId());
			FTicker::GetCoreTicker().Tick(.5f);
		}
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
		while (ApiClient->Lobby.IsConnected())
		{
			FPlatformProcess::Sleep(.5f);
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby Disconnect: Waiting user %s to disconnect")
				, *ApiClient->CredentialsRef->GetUserId());
			FTicker::GetCoreTicker().Tick(.5f);
		}
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
	bool bClientLoginSuccess = false;
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
		TestUser.Country = "US";
		const FDateTime DateOfBirth = (FDateTime::Now() - FTimespan::FromDays(365 * 35));
		TestUser.DateOfBirth = FString::Printf(TEXT("%04d-%02d-%02d"), DateOfBirth.GetYear(), DateOfBirth.GetMonth(), DateOfBirth.GetDay());

		UsersCreationSuccess[i] = RegisterTestUser(TestUser); 

		UsersLoginSuccess[i] = LoginTestUser(TestUser);

		LobbyUsers.Add(TestUser);
		
		LobbyApiClients.Add(FMultiRegistry::GetApiClient(TestUser.Email));
	}
	
	for (int i = 0; i < TestUserCount; i++)
	{
		AB_TEST_TRUE(UsersLoginSuccess[i]);
	}

	FString PreferedDSRegion = Environment::GetEnvironmentVariable("PREFERED_DS_REGION", 1000);

	if (!PreferedDSRegion.IsEmpty()) 
	{
		PreferredLatencies = FRegistry::Qos.GetCachedLatencies();
		for (int i = 0; i < PreferredLatencies.Num(); i++)
		{
			if (PreferredLatencies[i].Key == PreferedDSRegion)
			{
				PreferredLatencies[i].Value = 10;
				PreferredLatencies = { PreferredLatencies[i] };
				break;
			}
		}
	}

	// Setup dsm config
	bool isUpdateDsmConfig = false;
	bool bGetDsmConfigComplete = false;
	
	FDsmConfig dsmConfig;
	AdminGetLobbyDSMConfig(THandler<FDsmConfig>::CreateLambda(
			[&dsmConfig, &bGetDsmConfigComplete, &isUpdateDsmConfig](const FDsmConfig& result)
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
			})
		, FErrorHandler::CreateLambda([&dsmConfig, &isUpdateDsmConfig, &bGetDsmConfigComplete](int32 ErrorCode, FString ErrorMessage)
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
			})
		);
	WaitUntil(bGetDsmConfigComplete, "Waiting get dsm config");

	if (isUpdateDsmConfig)
	{
		bool bSetDsmConfigComplete = false;
		AdminSetLobbyDSMConfig(dsmConfig
			, FVoidHandler::CreateLambda([&bSetDsmConfigComplete]()
				{
					bSetDsmConfigComplete = true;
				})
			, LobbyTestErrorHandler);
		WaitUntil(bSetDsmConfigComplete, "Waiting set dsm config");
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

	WaitUntil(bGetLobbyConfigDone, "Waiting fetching original lobby config");
	
	FLobbyModelConfig NewLobbyConfig {OriginalLobbyConfig};
	NewLobbyConfig.GeneralRateLimitBurst = 100;

	bool bSetLobbyConfigDone {false};
	AdminSetLobbyConfig(NewLobbyConfig
		, THandler<FLobbyModelConfig>::CreateLambda([&bSetLobbyConfigDone](const FLobbyModelConfig& )
			{
				bSetLobbyConfigDone = true;
			})
		, LobbyTestErrorHandler);

	WaitUntil(bSetLobbyConfigDone, "Waiting set new lobby config");

	// Create matchmaking channel
	{
		ChannelName = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

		bool bCreateMatchmakingChannelSuccess = false;
		AdminCreateMatchmakingChannel(ChannelName
			, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				})
			, LobbyTestErrorHandler);
		
		WaitUntil([&]()
		{
			return bCreateMatchmakingChannelSuccess;
		}, "Waiting for Create Matchmaking channel...", 60);
	}

	// Create matchmaking channel with sub gamemode
	{
		ChannelNameSubGameMode = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);

		FAllianceRule AllianceRule;
		AllianceRule.min_number = 3;
		AllianceRule.max_number = 3;
		AllianceRule.player_min_number = 1;
		AllianceRule.player_max_number = 1;

		FAllianceRule AllianceRuleSub;
		AllianceRuleSub.min_number = 2;
		AllianceRuleSub.max_number = 2;
		AllianceRuleSub.player_min_number = 1;
		AllianceRuleSub.player_max_number = 1;

		FAllianceRule AllianceRuleSub4;
		AllianceRuleSub4.min_number = 4;
		AllianceRuleSub4.max_number = 4;
		AllianceRuleSub4.player_min_number = 1;
		AllianceRuleSub4.player_max_number = 1;
	
		TArray<FSubGameMode> SubGameModes;
		SubGameModes.Add({SubGameModeNames[0], AllianceRuleSub});
		SubGameModes.Add({SubGameModeNames[1], AllianceRuleSub4});
	
		bool bCreateMatchmakingChannelSuccess = false;
		AdminCreateMatchmakingChannel(ChannelNameSubGameMode
			, AllianceRule
			, {}
			, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				})
			, LobbyTestErrorHandler
			, false
			, SubGameModes);
		
		WaitUntil([&]()
		{
			return bCreateMatchmakingChannelSuccess;
		}, "Waiting for Create Matchmaking channel...", 60);
	}

	// create matchmaking channel for extra attribute matchmaking
	{
		ChannelNameExtraAttributeTest = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	
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
		AdminCreateMatchmakingChannel(ChannelNameExtraAttributeTest
			, AllianceRule
			, MatchingRules
			, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				})
			, LobbyTestErrorHandler);

		WaitUntil([&]()
		{
			return bCreateMatchmakingChannelSuccess;
		}, "Create Matchmaking channel...", 60);
	}

	// create matchmaking channel for new session only matchmaking
	{
		ChannelNameNewSessionTest = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
	
		FAllianceRule AllianceRule;
		AllianceRule.min_number = 2;
		AllianceRule.max_number = 3;
		AllianceRule.player_min_number = 1;
		AllianceRule.player_max_number = 1;

		bool bCreateMatchmakingChannelSuccess = false;
		AdminCreateMatchmakingChannel(ChannelNameNewSessionTest
			, AllianceRule
			, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				})
			, LobbyTestErrorHandler
			, true);

		WaitUntil([&]()
		{
			return bCreateMatchmakingChannelSuccess;
		}, "Create Matchmaking channel...", 60);
	}

	// Create matchmaking channel for temp party of 2
	{
		ChannelNameTempParty2 = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
		FAllianceRule AllianceRule;
		AllianceRule.min_number = 2;
		AllianceRule.max_number = 2;
		AllianceRule.player_min_number = 1;
		AllianceRule.player_max_number = 2;

		bool bCreateMatchmakingChannelSuccess = false;
		AdminCreateMatchmakingChannel(ChannelNameTempParty2
			, AllianceRule
			, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				})
			, LobbyTestErrorHandler);
		WaitUntil([&]()
		{
			return bCreateMatchmakingChannelSuccess;
		}, "Create Matchmaking channel...", 60);
	}

	// Create matchmaking channel for 3v3
	{
		ChannelNameTest3v3 = "ue4sdktest" + FGuid::NewGuid().ToString(EGuidFormats::Digits);
		bool bCreateMatchmakingChannelSuccess = false;
		FAllianceRule AllianceRule;
		AllianceRule.min_number = 3;
		AllianceRule.max_number = 3;
		AllianceRule.player_min_number = 1;
		AllianceRule.player_max_number = 1;

		AdminCreateMatchmakingChannel(ChannelNameTest3v3
			, AllianceRule
			, FSimpleDelegate::CreateLambda([&bCreateMatchmakingChannelSuccess]()
				{
					bCreateMatchmakingChannelSuccess = true;
					UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Matchmaking Channel Success..!"));
				})
			, LobbyTestErrorHandler);

		WaitUntil([&]()
		{
			return bCreateMatchmakingChannelSuccess;
		}, "Create Matchmaking channel...", 60);
	}

	DelaySeconds(60, "Waiting 60s for lobby service to refresh matchmaking game mode cache from MM service");
	
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestTeardown, "AccelByte.Tests.Lobby.Z.Teardown", AutomationFlagMaskLobby);
bool LobbyTestTeardown::RunTest(const FString& Parameters)
{
	LobbyDisconnect(TestUserCount);
	AB_TEST_TRUE(TeardownTestUsers(LobbyUsers));

	bool bSetLobbyConfigDone {false};
	AdminSetLobbyConfig(OriginalLobbyConfig, THandler<FLobbyModelConfig>::CreateLambda(
		[&bSetLobbyConfigDone](const FLobbyModelConfig& )
		{
			bSetLobbyConfigDone = true;
		}), LobbyTestErrorHandler);

	WaitUntil(bSetLobbyConfigDone, "Waiting set new lobby config");

	if(!ChannelName.IsEmpty())
	{
		bool bDeleteMatchmakingChannelSuccess = false;
		AdminDeleteMatchmakingChannel(ChannelName, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
		{
			bDeleteMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
		}), LobbyTestErrorHandler);

		WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	}

	if(!ChannelNameSubGameMode.IsEmpty())
	{
		bool bDeleteMatchmakingChannelSuccess = false;
		AdminDeleteMatchmakingChannel(ChannelNameSubGameMode, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
		{
			bDeleteMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
		}), LobbyTestErrorHandler);

		WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	}

	if(!ChannelNameExtraAttributeTest.IsEmpty())
	{
		bool bDeleteMatchmakingChannelSuccess = false;
		AdminDeleteMatchmakingChannel(ChannelNameExtraAttributeTest, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
		{
			bDeleteMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
		}), LobbyTestErrorHandler);

		WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	}


	if(!ChannelNameNewSessionTest.IsEmpty())
	{
		bool bDeleteMatchmakingChannelSuccess = false;
		AdminDeleteMatchmakingChannel(ChannelNameNewSessionTest, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
		{
			bDeleteMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
		}), LobbyTestErrorHandler);
		WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	}

	if(!ChannelNameTempParty2.IsEmpty())
	{
		bool bDeleteMatchmakingChannelSuccess = false;
		AdminDeleteMatchmakingChannel(ChannelNameTempParty2, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
		{
			bDeleteMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
		}), LobbyTestErrorHandler);
		WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	}

	if(!ChannelNameTest3v3.IsEmpty())
	{
		AB_TEST_FALSE(bDsNotifError);
		bool bDeleteMatchmakingChannelSuccess = false;
		AdminDeleteMatchmakingChannel(ChannelNameTest3v3, FSimpleDelegate::CreateLambda([&bDeleteMatchmakingChannelSuccess]()
		{
			bDeleteMatchmakingChannelSuccess = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Delete Matchmaking Channel Success..!"));
		}), LobbyTestErrorHandler);

		WaitUntil(bDeleteMatchmakingChannelSuccess, "Delete Matchmaking channel...");
	}

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
		FString MessageJSON = AccelByte::Api::Lobby::LobbyMessageToJson(*Data.Message);
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

	while (!LobbyApiClients[0]->Lobby.IsConnected() || !bUsersConnectionSuccess)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LobbyTestConnect2Users: Wait user 0"));
		FTicker::GetCoreTicker().Tick(.5f);
	}
	userResponded[0] = bUsersConnectionSuccess;
	userConnected[0] = bUsersConnected;
	bUsersConnectionSuccess = false;
	bUsersConnected = false;

	LobbyApiClients[1]->Lobby.Connect();

	while (!LobbyApiClients[1]->Lobby.IsConnected() || !bUsersConnectionSuccess)
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
	LobbyApiClients[0]->Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	LobbyApiClients[0]->Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);

	LobbyConnect(1);
	while (!LobbyApiClients[0]->Lobby.IsConnected() || !bUsersConnectionSuccess)
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

	LobbyApiClients[0]->Lobby.SetPrivateMessageNotifDelegate(GetMessageDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		FString userId = LobbyUsers[0].UserId;
		FString chatMessage = "Hello " + LobbyUsers[0].DisplayName + " from " + LobbyUsers[i].DisplayName;
		LobbyApiClients[i]->Lobby.SendPrivateMessage(userId, chatMessage);
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
	}, text, 30);


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
		FString text = FString::Printf(TEXT("Requesting Friend %d... "), i);
		WaitUntil(bRequestFriendSuccess, text);

		LobbyApiClients[0]->Lobby.AcceptFriend(LobbyUsers[i].UserId);
		text = FString::Printf(TEXT("Accepting Friend %d... "), i);
		WaitUntil(bAcceptFriendSuccess, text);

		LobbyApiClients[i]->Lobby.SendSetPresenceStatus(Availability::Availabe, "random activity");
		bRequestFriendSuccess = false;
		bAcceptFriendSuccess = false;
	}

	LobbyApiClients[0]->Lobby.SendGetOnlineUsersRequest();
	WaitUntil([&]()
	{
		return bGetAllUserPresenceSuccess;
	}, "Getting Friend Status...", 30);

	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyApiClients[i]->Lobby.SendSetPresenceStatus(Availability::Offline, "disappearing");
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
	FString text = FString::Printf(TEXT("Requesting Friend %d... "), 1);
	WaitUntil(bRequestFriendSuccess, text);

	LobbyApiClients[0]->Lobby.AcceptFriend(LobbyUsers[1].UserId);
	text = FString::Printf(TEXT("Accepting Friend %d... "), 1);
	WaitUntil(bAcceptFriendSuccess, text);

	LobbyApiClients[1]->Lobby.SendSetPresenceStatus(Availability::Availabe, "\"acti \" vity\"");
	bRequestFriendSuccess = false;
	bAcceptFriendSuccess = false;

	text = FString::Printf(TEXT("Setting presence %d... "), 1);
	WaitUntil(bUserPresenceSuccess, text);

	LobbyApiClients[0]->Lobby.SendGetOnlineUsersRequest();
	WaitUntil([&]()
	{
		return bGetAllUserPresenceSuccess;
	}, "Getting Friend Status...", 30);

	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);
	WaitUntil(bUnfriendSuccess, "Waiting unfriend...");

	LobbyDisconnect(2);

	AB_TEST_TRUE(bGetAllUserPresenceSuccess);
	AB_TEST_FALSE(onlineUserResponse.Code == "0");
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
		FString text = FString::Printf(TEXT("Requesting Friend %d... "), i);
		WaitUntil(bRequestFriendSuccess, text);

		LobbyApiClients[0]->Lobby.AcceptFriend(LobbyUsers[i].UserId);
		text = FString::Printf(TEXT("Accepting Friend %d... "), i);
		WaitUntil(bAcceptFriendSuccess, text);

		LobbyApiClients[i]->Lobby.SendSetPresenceStatus(Availability::Availabe, "random activity");
		bRequestFriendSuccess = false;
		bAcceptFriendSuccess = false;
	}

	LobbyApiClients[0]->Lobby.SendGetOnlineFriendPresenceRequest();
	WaitUntil([&]()
	{
		return bGetFriendsPresenceSuccess;
	}, "Getting Friend Status...", 30);

	for (int i = 1; i < TestUserCount; i++)
	{
		LobbyApiClients[i]->Lobby.SendSetPresenceStatus(Availability::Offline, "disappearing");
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

	DelaySeconds(5, "Waiting backend to sync lobby disconnect");

	// Act (call bulk get user presence)
	FAccelByteModelsBulkUserStatusNotif GetPresenceResult;
	bool bGetPresenceSuccess = false;
	LobbyApiClients[0]->Lobby.BulkGetUserPresence(LobbyUserIds, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda([&bGetPresenceSuccess, &GetPresenceResult](const FAccelByteModelsBulkUserStatusNotif& Result)
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

	DelaySeconds(5, "Waiting backend to sync lobby disconnect");

	// Act (call bulk get user presence)
	FAccelByteModelsBulkUserStatusNotif GetPresenceResult;
	bool bGetPresenceSuccess = false;
	LobbyApiClients[0]->Lobby.BulkGetUserPresence(LobbyUserIds, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda([&bGetPresenceSuccess, &GetPresenceResult](const FAccelByteModelsBulkUserStatusNotif& Result)
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
	LobbyApiClients[0]->Lobby.BulkGetUserPresence(LobbyUserIds, THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda([&bGetPresenceSuccess, &bGetPresenceDone, &GetPresenceResult](const FAccelByteModelsBulkUserStatusNotif& Result)
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

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "Leaving Party...");

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();
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

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

	WaitUntil(bLeavePartySuccess, "Leaving Party...");

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
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	{ // Case 1: No response delegate changes
		const FString MessageIdA = LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
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

		const FString MessageIdA = LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegateA);
		const FString MessageIdB = LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegateB);
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

			const FString MessageId = LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate_Unique);
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

		LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate_ShouldNotBeCalled);
		WaitUntil(bGetInfoPartySuccess, "Waiting for cached response ...", 15);
		UTEST_FALSE("Overriding response will not affect response for request that is already on the way. Single Sequence", bLastAssignedWrongDelegateCalled);

		bLastAssignedWrongDelegateCalled = false;
		LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate_ShouldNotBeCalled);
		LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SendInfoPartyRequest(GetInfoPartyDelegate);
		LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate_ShouldNotBeCalled);
		UTEST_FALSE("Overriding response will not affect response for request that is already on the way. Pseudo-Random Sequence", bLastAssignedWrongDelegateCalled);
	}

	//@TODO dummy uncached/unregistered/unknown message id will not call a response

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
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

	const AccelByte::Api::Lobby::FPartyInfoResponse OnInfoPartyResponse = AccelByte::Api::Lobby::FPartyInfoResponse::CreateLambda(
		[&PartyId](FAccelByteModelsInfoPartyResponse Response)
		{
			PartyId = Response.PartyId;	
		});
	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(OnInfoPartyResponse);
	
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);
	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating party..");

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
	WaitUntil(bGetPartyDataDone, "Getting party data...");
	
	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
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

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

	WaitUntil(bLeavePartySuccess, "Leaving Party...");

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	bCreatePartySuccess = false;

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

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

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	// Create Party
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	// Get Party Code
	LobbyApiClients[0]->Lobby.SendPartyGetCodeRequest();
	WaitUntil(bGetPartyCodeSuccess, "Getting Party Code...");
	AB_TEST_TRUE(bGetPartyCodeSuccess);
	bGetPartyCodeSuccess = false;
	partyCode = partyCodeResponse.PartyCode;

	bCreatePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating party...");

	// Join via Party Code
	LobbyApiClients[1]->Lobby.SendPartyJoinViaCodeRequest(partyCode);
	WaitUntil(bJoinPartyViaCodeSuccess, "Joining Party via Code...");
	AB_TEST_TRUE(bJoinPartyViaCodeSuccess);
	bJoinPartyViaCodeError = false;
	bJoinPartyViaCodeSuccess = false;

	// Leave Party
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "Leaving Party...");

	// Generate new Party Code
	LobbyApiClients[0]->Lobby.SendPartyGenerateCodeRequest();
	WaitUntil(bGeneratePartyCodeSuccess, "Generate Party Code...");
	AB_TEST_TRUE(bGeneratePartyCodeSuccess);

	// Join via Party Code using previous code
	LobbyApiClients[1]->Lobby.SendPartyJoinViaCodeRequest(partyCode);
	WaitUntil(bJoinPartyViaCodeSuccess, "Joining Party via Code...");
	AB_TEST_TRUE(bJoinPartyViaCodeError);
	bJoinPartyViaCodeError = false;
	bJoinPartyViaCodeSuccess = false;

	// Get Party Code
	LobbyApiClients[0]->Lobby.SendPartyGetCodeRequest();
	WaitUntil(bGetPartyCodeSuccess, "Getting Party Code...");
	AB_TEST_TRUE(bGetPartyCodeSuccess);
	bGetPartyCodeSuccess = false;
	partyCode = partyCodeResponse.PartyCode;
	AB_TEST_TRUE(partyCode == partyGenerateCodeResponse.PartyCode);

	// Join via Party Code using new code
	LobbyApiClients[1]->Lobby.SendPartyJoinViaCodeRequest(partyCode);
	WaitUntil(bJoinPartyViaCodeSuccess, "Joining Party via Code...");
	AB_TEST_TRUE(bJoinPartyViaCodeSuccess);
	bJoinPartyViaCodeError = false;
	bJoinPartyViaCodeSuccess = false;

	// Delete Party Code
	LobbyApiClients[0]->Lobby.SendPartyDeleteCodeRequest();
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
	LobbyApiClients[0]->Lobby.SetPartyJoinViaCodeResponseDelegate(AccelByte::Api::Lobby::FPartyJoinViaCodeResponse::CreateLambda([&](const FAccelByteModelsPartyJoinResponse& Result)
	{
		if(Result.Code.Equals("11573") || !Result.Code.Equals("0"))
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("[EXPECTED] Join party code is invalid with code : %s"), *Result.Code);
			bJoinFailed = true;
		}
	}));
	LobbyApiClients[0]->Lobby.SendPartyJoinViaCodeRequest("INVALID");
	WaitUntil(bJoinFailed, "Waiting to join party via code...");
	
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

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[1].UserId);
	WaitUntil(bInvitePartySuccess, "Inviting to Party...");

	AB_TEST_TRUE(bInvitePartySuccess);

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation...", 30);

	AB_TEST_FALSE(bGetInvitedNotifError);

	LobbyApiClients[0]->Lobby.SetPartyDataUpdateNotifDelegate(PartyDataUpdateDelegate);

	LobbyApiClients[1]->Lobby.SendAcceptInvitationRequest(*invitedToPartyResponse.PartyId, *invitedToPartyResponse.InvitationToken);
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
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();
	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");
	const auto partyInfoResult = infoPartyResponse;

	AB_TEST_FALSE(bGetInfoPartyError);

	// Compare partydata notif with partyget info
	AB_TEST_EQUAL(partyDataUpdate.PartyId, partyInfoResult.PartyId);
	AB_TEST_EQUAL(partyDataUpdate.Leader, partyInfoResult.LeaderId);
	AB_TEST_EQUAL(partyDataUpdate.Members.Num(), partyInfoResult.Members.Num());

	LobbyApiClients[1]->Lobby.SendPartyMessage("This is a party chat");
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
	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "Leaving Party...");
	AB_TEST_FALSE(bLeavePartyError);

	bLeavePartySuccess = false;
	bLeavePartyError = false;
	LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
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
	
	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bRejectPartySuccess = false;
	bRejectPartyError = false; 
	bRejectedPartyNotifSuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[1].UserId);
	WaitUntil(bInvitePartySuccess, "Inviting to Party...");

	AB_TEST_TRUE(bInvitePartySuccess);

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation...", 30);


	AB_TEST_FALSE(bGetInvitedNotifError);

	LobbyApiClients[1]->Lobby.SendRejectInvitationRequest(*invitedToPartyResponse.PartyId, *invitedToPartyResponse.InvitationToken);
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

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[2]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[2]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[1].UserId);

	WaitUntil(bInvitePartySuccess, "Inviting to Party...");

	bInvitePartySuccess = false;

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation", 30);


	bGetInvitedNotifSuccess = false;
	invitedToParty[0] = invitedToPartyResponse;

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[2].UserId);

	WaitUntil(bInvitePartySuccess, "Inviting to Party...");
	const FString InvitedUserId {LobbyUsers[2].UserId};
	const FAccelByteModelsPartyInviteResponse InviteToPartyResponse {partyInviteResponse};

	bInvitePartySuccess = false;

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation", 30);

	bGetInvitedNotifSuccess = false;
	invitedToParty[1] = invitedToPartyResponse;

	LobbyApiClients[1]->Lobby.SendAcceptInvitationRequest(invitedToParty[0].PartyId, invitedToParty[0].InvitationToken);

	WaitUntil(bJoinPartySuccess, "Joining a Party...");

	bJoinPartySuccess = false;
	joinParty[0] = joinPartyResponse;

	LobbyApiClients[2]->Lobby.SendAcceptInvitationRequest(invitedToParty[1].PartyId, invitedToParty[1].InvitationToken);

	WaitUntil(bJoinPartySuccess, "Joining a Party...");

	bJoinPartySuccess = false;
	joinParty[1] = joinPartyResponse;

	LobbyApiClients[0]->Lobby.SendKickPartyMemberRequest(LobbyUsers[2].UserId);

	WaitUntil(bKickPartyMemberSuccess, "Kicking Party Member...");
	const FString KickedUserId {LobbyUsers[2].UserId};
	const FAccelByteModelsKickPartyMemberResponse KickMemberFromPartyResponse {kickMemberFromPartyResponse};

	WaitUntil(bKickedFromPartySuccess, "Waiting to Get Kicked from Party...");

	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	for (int i = 0; i < 2; i++)
	{
		LobbyApiClients[i]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
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

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	bLeavePartySuccess = false;

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendInviteToPartyRequest(LobbyUsers[1].UserId);

	WaitUntil(bInvitePartySuccess, "Inviting to Party...");

	bInvitePartySuccess = false;

	WaitUntil([&]()
	{
		return bGetInvitedNotifSuccess;
	}, "Waiting for Party Invitation", 30);

	bGetInvitedNotifSuccess = false;

	LobbyApiClients[1]->Lobby.SendAcceptInvitationRequest(*invitedToPartyResponse.PartyId, *invitedToPartyResponse.InvitationToken);

	WaitUntil(bJoinPartySuccess, "Joining a Party...");

	AB_TEST_TRUE(bJoinPartySuccess);

	bJoinPartySuccess = false;

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	bGetInfoPartySuccess = false;

	LobbyApiClients[0]->Lobby.SendPartyPromoteLeaderRequest(LobbyUsers[1].UserId);

	WaitUntil(bPromotePartyLeaderSuccess, "Promoting Party leader...");

	AB_TEST_TRUE(bPromotePartyLeaderSuccess);

	bPromotePartyLeaderSuccess = false;

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

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
	for (int i = 0; i < 100; i += 5)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Wait for %d seconds. Lobby.IsConnected=%d"), i, LobbyApiClients[0]->Lobby.IsConnected());
		FPlatformProcess::Sleep(5);
		FTicker::GetCoreTicker().Tick(5);
	}
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

		UAccelByteBlueprintsTest::SendNotif(LobbyUsers[0].UserId, payloads[i], true, FVoidHandler::CreateLambda([&]()
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

	LobbyApiClients[1]->Lobby.SetMessageNotifDelegate(GetNotifDelegate);

	for (int i = 0; i < repetition; i++)
	{
		MessageRequest[i].Topic = FString::Printf(TEXT("Message number %d"), i);
		MessageRequest[i].Message = FString::Printf(TEXT("Test from the integration test UE4 number %d"), i);

		LobbyApiClients[0]->Lobby.SendNotificationToUser(LobbyUsers[1].UserId, MessageRequest[i], false, FVoidHandler::CreateLambda([&]()
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

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[1]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	LobbyApiClients[0]->Lobby.SetUserPresenceResponseDelegate(UserPresenceDelegate);

	LobbyApiClients[1]->Lobby.SetUserPresenceResponseDelegate(UserPresenceDelegate);

	LobbyApiClients[1]->Lobby.SetUserPresenceNotifDelegate(UserPresenceNotifDelegate);

	LobbyApiClients[1]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend...");

	LobbyApiClients[1]->Lobby.SendSetPresenceStatus(Availability::Availabe, "ready to play");

	WaitUntil([&]()
	{
		return bUserPresenceSuccess;
	}, "Changing User Status...", 30);

	bUserPresenceSuccess = false;

	LobbyApiClients[0]->Lobby.SendSetPresenceStatus(expectedUserStatus, "expected activity");

	WaitUntil([&]()
	{
		return bUserPresenceSuccess;
	}, "Changing User Status...", 30);

	WaitUntil([&]() 
	{
		return bUserPresenceNotifSuccess; 
	}, "Waiting for Changing User Presence...", 30);

	LobbyApiClients[1]->Lobby.Unfriend(LobbyUsers[0].UserId);

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

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[1]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	LobbyApiClients[1]->Lobby.SetUserPresenceResponseDelegate(UserPresenceDelegate);

	LobbyApiClients[0]->Lobby.SetUserPresenceResponseDelegate(UserPresenceDelegate);

	LobbyApiClients[1]->Lobby.SetUserPresenceNotifDelegate(UserPresenceNotifDelegate);

	LobbyApiClients[1]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend...");

	LobbyApiClients[1]->Lobby.SendSetPresenceStatus(Availability::Availabe, "ready to play again");

	WaitUntil([&]()
	{
		return bUserPresenceSuccess;
	}, "Changing User Status...", 30);

	bUserPresenceSuccess = false;
	LobbyApiClients[0]->Lobby.SendSetPresenceStatus(Availability::Availabe, "ready to play too");

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
	LobbyApiClients[0]->Lobby.SendSetPresenceStatus(expectedUserStatus, "busy, can't play");

	WaitUntil([&]()
	{
		return bUserPresenceSuccess;
	}, "Changing User Status...", 30);

	WaitUntil([&]()
	{
		return bUserPresenceNotifSuccess;
	}, "Waiting for Changing User Presence...", 30);

	LobbyApiClients[1]->Lobby.Unfriend(LobbyUsers[0].UserId);

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

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");

	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend Request...");
	AB_TEST_FALSE(bAcceptFriendError);

	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);

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

	LobbyApiClients[1]->Lobby.SetOnIncomingRequestFriendsNotifDelegate(OnIncomingRequestNotifDelegate);

	LobbyApiClients[0]->Lobby.SetOnFriendRequestAcceptedNotifDelegate(OnRequestAcceptedNotifDelegate);

	LobbyApiClients[0]->Lobby.SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	LobbyApiClients[1]->Lobby.SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	LobbyApiClients[0]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[1]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);

	LobbyApiClients[0]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	WaitUntil(bOnIncomingRequestNotifSuccess, "Waiting for Incoming Friend Request...");
	AB_TEST_FALSE(bOnIncomingRequestNotifError);
	AB_TEST_EQUAL(requestFriendNotifResponse.friendId, LobbyUsers[0].UserId);

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend Request...");
	AB_TEST_FALSE(bAcceptFriendError);

	WaitUntil(bOnRequestAcceptedNotifSuccess, "Waiting for Accepted Friend Request...");
	AB_TEST_FALSE(bOnRequestAcceptedNotifError);
	AB_TEST_EQUAL(acceptFriendNotifResponse.friendId, LobbyUsers[1].UserId);

	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);

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

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend Request...");
	AB_TEST_FALSE(bAcceptFriendError);

	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);

	WaitUntil(bUnfriendSuccess, "Waiting Unfriend...");
	AB_TEST_FALSE(bUnfriendError);

	WaitUntil(bUnfriendNotifSuccess, "Waiting Unfriend Notif...");
	AB_TEST_TRUE(bUnfriendNotifSuccess);

	AB_TEST_EQUAL(UnfriendNotifResponse.friendId, LobbyUsers[0].UserId);

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));
	}

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
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

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.RejectFriend(LobbyUsers[0].UserId);

	WaitUntil(bRejectFriendSuccess, "Rejecting Friend Request...");
	AB_TEST_FALSE(bRejectFriendError);

	WaitUntil(bRejectFriendNotifSuccess, "Getting Reject Friend Notif...");
	AB_TEST_TRUE(bRejectFriendNotifSuccess);
	AB_TEST_EQUAL(RejectFriendNotifResponse.userId, LobbyUsers[1].UserId);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_FALSE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	bListOutgoingFriendSuccess = false;
	bListOutgoingFriendError = false;
	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
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

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[0]->Lobby.CancelFriendRequest(LobbyUsers[1].UserId);

	WaitUntil(bCancelFriendSuccess, "Cancelling Friend Request...");
	AB_TEST_FALSE(bCancelFriendError);

	WaitUntil(bCancelFriendNotifSuccess, "Getting Cancel Friend Notif...");
	AB_TEST_TRUE(bCancelFriendNotifSuccess);
	AB_TEST_EQUAL(CancelFriendNotifResponse.userId, LobbyUsers[0].UserId);

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_FALSE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(LobbyUsers[0].UserId));
	}

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
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

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::NotFriend);

	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	AB_TEST_TRUE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Outgoing);

	LobbyApiClients[0]->Lobby.CancelFriendRequest(LobbyUsers[1].UserId);

	WaitUntil(bCancelFriendSuccess, "Cancelling Friend Request...");
	AB_TEST_FALSE(bCancelFriendError);

	bListOutgoingFriendSuccess = false;
	bListOutgoingFriendError = false;
	LobbyApiClients[0]->Lobby.ListOutgoingFriends();

	WaitUntil(bListOutgoingFriendSuccess, "Getting List Outgoing Friend...");
	AB_TEST_FALSE(bListOutgoingFriendError);
	if (listOutgoingFriendResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(listOutgoingFriendResponse.friendsId.Contains(LobbyUsers[1].UserId));
	}

	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);

	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Incoming);

	LobbyApiClients[1]->Lobby.RejectFriend(LobbyUsers[0].UserId);

	WaitUntil(bRejectFriendSuccess, "Rejecting Friend Request...");
	AB_TEST_FALSE(bRejectFriendError);

	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	LobbyApiClients[0]->Lobby.RequestFriend(LobbyUsers[1].UserId);

	WaitUntil(bRequestFriendSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bRequestFriendError);
	WaitUntil([&]()
	{
		return bOnIncomingRequestNotifSuccess;
	}, "Waiting for Incoming Friend Request...", 30);
	AB_TEST_FALSE(bOnIncomingRequestNotifError);
	AB_TEST_EQUAL(requestFriendNotifResponse.friendId, LobbyUsers[0].UserId);

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	LobbyApiClients[1]->Lobby.ListIncomingFriends();

	WaitUntil(bListIncomingFriendSuccess, "Getting List Incoming Friend...");
	AB_TEST_FALSE(bListIncomingFriendError);
	AB_TEST_TRUE(listIncomingFriendResponse.friendsId.Contains(LobbyUsers[0].UserId));

	LobbyApiClients[1]->Lobby.AcceptFriend(LobbyUsers[0].UserId);

	WaitUntil(bAcceptFriendSuccess, "Accepting Friend Request...");
	AB_TEST_FALSE(bAcceptFriendError);
	WaitUntil([&]()
	{
		return bOnRequestAcceptedNotifSuccess;
	}, "Waiting for Accepted Friend Request...", 30);
	AB_TEST_FALSE(bOnRequestAcceptedNotifError);
	AB_TEST_EQUAL(acceptFriendNotifResponse.friendId, LobbyUsers[1].UserId);

	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[0]->Lobby.GetFriendshipStatus(LobbyUsers[1].UserId);

	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Friend);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);
	
	WaitUntil(bGetFriendshipStatusSuccess, "Getting Friendship Status...");
	AB_TEST_FALSE(bGetFriendshipStatusError);
	AB_TEST_EQUAL(getFriendshipStatusResponse.friendshipStatus, ERelationshipStatusCode::Friend);

	LobbyApiClients[0]->Lobby.Unfriend(LobbyUsers[1].UserId);

	WaitUntil(bUnfriendSuccess, "Waiting Unfriend...");
	AB_TEST_FALSE(bUnfriendError);

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	LobbyApiClients[0]->Lobby.LoadFriendsList();

	WaitUntil(bLoadFriendListSuccess, "Loading Friend List...");
	AB_TEST_FALSE(bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		AB_TEST_FALSE(loadFriendListResponse.friendsId.Contains(LobbyUsers[1].UserId));
	}

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	LobbyApiClients[1]->Lobby.GetFriendshipStatus(LobbyUsers[0].UserId);

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
		FriendUserIds.FriendIds.Add(LobbyUsers[i].UserId);
	}

	bool bBulkAddFriendSuccess = false;
	LobbyApiClients[0]->Lobby.BulkFriendRequest(FriendUserIds, FVoidHandler::CreateLambda([&bBulkAddFriendSuccess]()
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Bulk Add Friend Success!"));
		bBulkAddFriendSuccess = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bBulkAddFriendSuccess, "Waiting Bulk Add Friend...");
	
	LobbyConnect(1);

	LobbyApiClients[0]->Lobby.SetLoadFriendListResponseDelegate(LoadFriendListDelegate);
	LobbyApiClients[0]->Lobby.LoadFriendsList();
	WaitUntil(bLoadFriendListSuccess, "Waiting Load Friend List...!");

	LobbyApiClients[0]->Lobby.SetUnfriendResponseDelegate(UnfriendDelegate);

	for (auto FriendId : FriendUserIds.FriendIds)
	{
		AB_TEST_TRUE(loadFriendListResponse.friendsId.Contains(FriendId));
		LobbyApiClients[0]->Lobby.Unfriend(FriendId);
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
	FriendUserIds.FriendIds.Add(LobbyUsers[0].UserId);

	bool bBulkAddFriendSuccess = false;
	bool bBulkAddFriendError = false;
	bool bBulkAddFriendDone = false;
	LobbyApiClients[0]->Lobby.BulkFriendRequest(FriendUserIds, FVoidHandler::CreateLambda([&bBulkAddFriendSuccess, &bBulkAddFriendDone]()
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
	LobbyApiClients[0]->Lobby.SetBlockPlayerResponseDelegate(BlockPlayerDelegate);

	LobbyApiClients[1]->Lobby.SetBlockPlayerNotifDelegate(BlockPlayerNotifDelegate);

	LobbyApiClients[0]->Lobby.SetUnblockPlayerResponseDelegate(UnblockPlayerDelegate);

	LobbyApiClients[1]->Lobby.SetUnblockPlayerNotifDelegate(UnblockPlayerNotifDelegate);

	LobbyApiClients[0]->Lobby.GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);

	WaitUntil(bListBlockedUserListSuccess, "Getting List of Blocked User for Lobby 0...");
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	auto CurrentListBlockedUser = listBlockedUserResponse;
	
	LobbyApiClients[0]->Lobby.BlockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bBlockPlayerSuccess, "Player 0 Blocks Player 1...");
	AB_TEST_FALSE(bBlockPlayerError);
	AB_TEST_EQUAL(blockPlayerResponse.BlockedUserId, LobbyUsers[1].UserId);

	bListBlockedUserListSuccess = false;
	FBlockedData BlockedUserData;
	BlockedUserData.BlockedUserId = LobbyUsers[1].UserId;
	LobbyApiClients[0]->Lobby.GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);
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
	BlockerUserData.UserId = LobbyUsers[0].UserId;
	LobbyApiClients[1]->Lobby.GetListOfBlockers(ListBlockerDelegate, LobbyTestErrorHandler);
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
	LobbyApiClients[1]->Lobby.SetRequestFriendsResponseDelegate(AccelByte::Api::Lobby::FRequestFriendsResponse::CreateLambda([&bRequestFriendAttemptDone, &bBlockedFriendRequestPassed](FAccelByteModelsRequestFriendsResponse Result)
	{
		bRequestFriendAttemptDone = true;
		// Should be changed to something to signify blocked user response, but for now we'll use != 0
		if (Result.Code != "0")
		{
			bBlockedFriendRequestPassed = true;
		}
	}));
	LobbyApiClients[1]->Lobby.RequestFriend(LobbyUsers[0].UserId);
	WaitUntil(bRequestFriendAttemptDone, "Checking if Player 1 has been blocked from Requesting Friend Request properly.");
	AB_TEST_TRUE(bBlockedFriendRequestPassed);
	bRequestFriendAttemptDone = false;
	bBlockedFriendRequestPassed = false;

	LobbyApiClients[0]->Lobby.UnblockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bUnblockPlayerSuccess, "Player 0 Unblocks Player 1...");
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

	WaitUntil(bListBlockedUserListSuccess, "Getting List of Blocked User for Lobby 0...");
	AB_TEST_TRUE(bListBlockedUserListSuccess);
	auto CurrentListBlockedUser = listBlockedUserResponse;

	LobbyApiClients[0]->Lobby.BlockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bBlockPlayerSuccess, "Requesting Friend...");
	AB_TEST_FALSE(bBlockPlayerError);

	bListBlockedUserListSuccess = false;
	FBlockedData BlockedUserData;
	BlockedUserData.BlockedUserId = LobbyUsers[1].UserId;
	LobbyApiClients[0]->Lobby.GetListOfBlockedUsers(ListBlockedUserDelegate, LobbyTestErrorHandler);
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
	BlockerUserData.UserId = LobbyUsers[0].UserId;
	LobbyApiClients[1]->Lobby.GetListOfBlockers(ListBlockerDelegate, LobbyTestErrorHandler);
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
	LobbyApiClients[0]->Lobby.BlockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bBlockPlayerSuccess, "Player 0 Blocks Player 1 Again");
	AB_TEST_FALSE(bBlockPlayerError);

	WaitUntil(bBlockPlayerNotifSuccess, "Waiting for block notif");

	LobbyApiClients[0]->Lobby.UnblockPlayer(LobbyUsers[1].UserId);
	WaitUntil(bUnblockPlayerSuccess, "Player 0 Unblocks Player 1...");
	AB_TEST_FALSE(bUnblockPlayerError);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmaking_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStartBasic", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmaking_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, "", "", PreferredLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelName, "", "", PreferredLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);
	
	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bDsNotifError);

	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);

	// check new addition to matchmaking notif fields.
	AB_TEST_EQUAL(matchmakingNotifResponse[0].GameMode, ChannelName);
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Joinable, false);
	AB_TEST_TRUE(matchmakingNotifResponse[0].MatchingAllies.Data.Num() > 0);

	bool bUserFound {false};
	for(auto Ally : matchmakingNotifResponse[0].MatchingAllies.Data)
	{
		for(auto Party : Ally.Matching_parties)
		{
			for(auto Member : Party.Party_members)
			{
				if(Member.User_id == LobbyUsers[0].UserId)
				{
					bUserFound = true;
					break;
				}
			}
			if(bUserFound)
				break;
		}
		if(bUserFound)
			break;
	}
	AB_TEST_TRUE(bUserFound);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmaking_SubGameMode_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStart_TestSubGameMode", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmaking_SubGameMode_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	Api::FMatchmakingOptionalParams MMOptionals;
	MMOptionals.SubGameModes = SubGameModeNames;

	Api::FMatchmakingOptionalParams MMOptionals2;
	MMOptionals2.SubGameModes = {SubGameModeNames[0]};

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelNameSubGameMode, MMOptionals);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelNameSubGameMode, MMOptionals2);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);
	
	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bDsNotifError);


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

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;


	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, "", "", PreferredLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelName, "", "", PreferredLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bDsNotifError);
	FAccelByteModelsDsNotice DsNotice {dsNotice};
	
	LobbyDisconnect(2);
	ResetResponses();
	
	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);

	// Test custom ports names returned from 
	AB_TEST_FALSE(DsNotice.MatchId.IsEmpty());
	int customPortFoundCount = 0;
	for (auto portName : customPortNames)
	{
		if (DsNotice.Ports.Contains(portName))
		{
			customPortFoundCount++;
		}
	}
	AB_TEST_EQUAL(customPortFoundCount, customPortNames.Num());
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmaking_withPartyAttributes, "AccelByte.Tests.Lobby.B.MatchmakingStartPartyAttributes", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmaking_withPartyAttributes::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	bGetInfoPartyError = false;
	bGetInfoPartySuccess = false;
	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();
	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");
	
	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	
	bCreatePartySuccess = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();
	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();
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
	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, ServerName, "", TArray<TPair<FString, float>>(), PartyAttributes);
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelName, ServerName, "", TArray<TPair<FString, float>>(), PartyAttributes);
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{ 
		return matchMakingNotifNum >= 2; 
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);

	bDsNotifSuccess = false;
	bDsNotifError = false;

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);
	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification 0", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);
	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification 1", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", DsNotifWaitTime);
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

	LobbyDisconnect(2);
	ResetResponses();
	
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
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmakingExtraAttributes_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStartExtraAttributes", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmakingExtraAttributes_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

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

	LobbyApiClients[0]->Lobby.SetSetSessionAttributeDelegate(SetSessionAttributeResponseHandler);
	LobbyApiClients[1]->Lobby.SetSetSessionAttributeDelegate(SetSessionAttributeResponseHandler);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");
	
	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SetSessionAttribute("mmr", "120");
	LobbyApiClients[1]->Lobby.SetSessionAttribute("mmr", "110");

	TArray<FString> ExtraAttributes = { "mmr" };

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelNameExtraAttributeTest, "", "", PreferredLatencies, TMap<FString, FString>(), TArray<FString>(), ExtraAttributes);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelNameExtraAttributeTest, "", "", PreferredLatencies, TMap<FString, FString>(), TArray<FString>(), ExtraAttributes);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bDsNotifError);

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
	// Arrange, set party attribute to be passed to start matchmaking function
	const TMap<FString, FString> partyAttribute ({{"map", "ffarena"}, {"difficulty", "hard"}});

	// Arrange Lobby delegates
	LobbyConnect(1);
	bool bMatchmakingNotifReceived = false;
	
	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&bMatchmakingNotifReceived](const FAccelByteModelsMatchmakingNotice& Result){bMatchmakingNotifReceived = true;}));
	LobbyApiClients[0]->Lobby.SetCancelMatchmakingResponseDelegate(CancelMatchmakingDelegate);

	// Arrange, make sure lobby not in party.
	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "waiting 0 leave party");

	// Arrange, lobby set session attribute
	bool bSetSessionAttributeFinish = false;
	bool bSetSessionAttributeError = false;
	LobbyApiClients[0]->Lobby.SetSetSessionAttributeDelegate(THandler<FAccelByteModelsSetSessionAttributesResponse>::CreateLambda([&bSetSessionAttributeFinish, &bSetSessionAttributeError](const FAccelByteModelsSetSessionAttributesResponse& result)
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

	LobbyApiClients[0]->Lobby.SetSessionAttribute("mmr", "120");
	WaitUntil(bSetSessionAttributeFinish, "Waiting set session attribute");

	// Arrange, extra attribute to be passed to start matchmaking function
	TArray<FString> ExtraAttributes = { "mmr" };

	// ACT
	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, "", "", PreferredLatencies, partyAttribute, TArray<FString>({LobbyUsers[0].UserId}), ExtraAttributes);
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");

	LobbyApiClients[0]->Lobby.SendCancelMatchmaking(ChannelName, true);
	WaitUntil(bCancelMatchmakingSuccess, "Canceling matchmaking");

	WaitUntil(bMatchmakingNotifReceived, "Waiting cancel matchmaking notification");

	// Asserts
	AB_TEST_FALSE(bStartMatchmakingError);

	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmakingAllParamsStruct_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStartAllOptionalParamsStruct", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmakingAllParamsStruct_ReturnOk::RunTest(const FString& Parameters)
{
	// Arrange, set party attribute to be passed to start matchmaking function
	const TMap<FString, FString> partyAttribute ({{"map", "ffarena"}, {"difficulty", "hard"}});

	// Arrange Lobby delegates
	LobbyConnect(1);
	bool bMatchmakingNotifReceived = false;

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&bMatchmakingNotifReceived](const FAccelByteModelsMatchmakingNotice& Result){bMatchmakingNotifReceived = true;}));
	LobbyApiClients[0]->Lobby.SetCancelMatchmakingResponseDelegate(CancelMatchmakingDelegate);
	// Arrange, make sure lobby not in party.
	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "waiting 0 leave party");

	// Arrange, lobby set session attribute
	bool bSetSessionAttributeFinish = false;
	bool bSetSessionAttributeError = false;
	LobbyApiClients[0]->Lobby.SetSetSessionAttributeDelegate(THandler<FAccelByteModelsSetSessionAttributesResponse>::CreateLambda([&bSetSessionAttributeFinish, &bSetSessionAttributeError](const FAccelByteModelsSetSessionAttributesResponse& result)
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

	LobbyApiClients[0]->Lobby.SetSessionAttribute("mmr", "120");
	WaitUntil(bSetSessionAttributeFinish, "Waiting set session attribute");

	// Arrange, extra attribute to be passed to start matchmaking function
	TArray<FString> ExtraAttributes = { "mmr" };

	Api::FMatchmakingOptionalParams OptionalParams;
	OptionalParams.ServerName = "TestServer";
	OptionalParams.ClientVersion = "0.0.0";
	OptionalParams.Latencies = PreferredLatencies;
	OptionalParams.PartyAttributes = partyAttribute;
	OptionalParams.TempPartyUserIds.Add(LobbyUsers[0].UserId);
	OptionalParams.ExtraAttributes = ExtraAttributes;
	OptionalParams.NewSessionOnly = true;
	OptionalParams.SubGameModes = {"sgm", "test", "another1"};

	// ACT
	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, OptionalParams);
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");

	LobbyApiClients[0]->Lobby.SendCancelMatchmaking(ChannelName, true);
	WaitUntil(bCancelMatchmakingSuccess, "Canceling Matchmaking...");

	WaitUntil(bMatchmakingNotifReceived, "Waiting cancel matchmaking notification");

	// Asserts
	AB_TEST_FALSE(bStartMatchmakingError);

	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmakingNewSessinOnly, "AccelByte.Tests.Lobby.B.MatchmakingStartNewSessionOnly", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmakingNewSessinOnly::RunTest(const FString& Parameters)
{
	// Arrange Lobby delegates
	LobbyConnect(4);

	FString Lobby0MatchId;
	bool Lobby0MatchFound = false;
	bool bDSBusy = false;
	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);
	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&Lobby0MatchId, &Lobby0MatchFound](const FAccelByteModelsMatchmakingNotice& notif)
	{
		if(notif.Status == EAccelByteMatchmakingStatus::Done)
		{
			Lobby0MatchId = notif.MatchId;
			Lobby0MatchFound = true;
		}
	}));
	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(Api::Lobby::FDsNotif::CreateLambda([&bDSBusy](const FAccelByteModelsDsNotice& result)
	{
		if(result.Status == "BUSY")
		{
			bDSBusy = true;
		}
	}));

	FString Lobby1MatchId;
	bool Lobby1MatchFound = false;
	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);
	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&Lobby1MatchId, &Lobby1MatchFound](const FAccelByteModelsMatchmakingNotice& notif)
	{
		if(notif.Status == EAccelByteMatchmakingStatus::Done)
		{
			Lobby1MatchFound = true;
			Lobby1MatchId = notif.MatchId;	
		}
	}));

	FString Lobby2MatchId;
	bool Lobby2MatchFound = false;
	int RematchmakingNotifCount = 0;
	auto IncrementCounterOnRematchmakingDelegate = Api::Lobby::FRematchmakingNotif::CreateLambda([&RematchmakingNotifCount](const FAccelByteModelsRematchmakingNotice& result)
	{
		RematchmakingNotifCount++;
	});
	LobbyApiClients[2]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	LobbyApiClients[2]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[2]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);
	LobbyApiClients[2]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	LobbyApiClients[2]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&Lobby2MatchId, &Lobby2MatchFound](const FAccelByteModelsMatchmakingNotice& notif)
	{
		if(notif.Status == EAccelByteMatchmakingStatus::Done)
		{
			Lobby2MatchFound = true;
			Lobby2MatchId = notif.MatchId;	
		}
	}));
	LobbyApiClients[2]->Lobby.SetRematchmakingNotifDelegate(IncrementCounterOnRematchmakingDelegate);

	FString Lobby3MatchId;
	bool Lobby3MatchFound = false;
	LobbyApiClients[3]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);
	LobbyApiClients[3]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);
	LobbyApiClients[3]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);
	LobbyApiClients[3]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	LobbyApiClients[3]->Lobby.SetMatchmakingNotifDelegate(THandler<FAccelByteModelsMatchmakingNotice>::CreateLambda([&Lobby3MatchId, &Lobby3MatchFound](const FAccelByteModelsMatchmakingNotice& notif)
	{
		if(notif.Status == EAccelByteMatchmakingStatus::Done)
		{
			Lobby3MatchFound = true;
			Lobby3MatchId = notif.MatchId;	
		}
	}));
	LobbyApiClients[3]->Lobby.SetRematchmakingNotifDelegate(IncrementCounterOnRematchmakingDelegate);

	// Arrange, make sure lobby not in party.
	LobbyApiClients[0]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "waiting 0 leave party");
	bLeavePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "waiting 1 leave party");
	bLeavePartySuccess = false;
	LobbyApiClients[2]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "waiting 2 leave party");
	bLeavePartySuccess = false;
	LobbyApiClients[3]->Lobby.SendLeavePartyRequest();
	WaitUntil(bLeavePartySuccess, "waiting 3 leave party");
	bLeavePartySuccess = false;

	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "waiting 0 create party");
	bCreatePartySuccess = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "waiting 1 create party");
	bCreatePartySuccess = false;
	LobbyApiClients[2]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "waiting 2 create party");
	bCreatePartySuccess = false;
	LobbyApiClients[3]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartySuccess, "waiting 3 create party");
	
	// Set optional params to be used 
	Api::FMatchmakingOptionalParams OptionalParams;
	OptionalParams.NewSessionOnly = true;

	// ACT
	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelNameNewSessionTest);
	WaitUntil(bStartMatchmakingSuccess, "Lobby 0 Starting Matchmaking...");
	bStartMatchmakingSuccess = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelNameNewSessionTest);
	WaitUntil(bStartMatchmakingSuccess, "Lobby 1 Starting Matchmaking...");

	WaitUntil(Lobby0MatchFound, "Waiting Lobby 0 get matchId");
	WaitUntil(Lobby1MatchFound, "Waiting Lobby 1 get matchId");

	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(Lobby0MatchId);
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(Lobby1MatchId);

	WaitUntil(bDSBusy, "Waiting DS to be busy");

	DelaySeconds(3, "waiting backend to sync");
	bool bSessionEnqueued;
	AdminEnqueueSession(Lobby0MatchId, FVoidHandler::CreateLambda([&bSessionEnqueued]()
	{
		bSessionEnqueued = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bSessionEnqueued, "Waiting enqueue session");

	DelaySeconds(5, "Waiting 5 sec");

	bStartMatchmakingSuccess = false;
	LobbyApiClients[2]->Lobby.SendStartMatchmaking(ChannelNameNewSessionTest, OptionalParams);
	WaitUntil(bStartMatchmakingSuccess, "Lobby 2 Starting Matchmaking...");
	bStartMatchmakingSuccess = false;
	LobbyApiClients[3]->Lobby.SendStartMatchmaking(ChannelNameNewSessionTest, OptionalParams);
	WaitUntil(bStartMatchmakingSuccess, "Lobby 3 Starting Matchmaking...");

	WaitUntil(Lobby2MatchFound, "Waiting Lobby 2 get matchId");
	WaitUntil(Lobby3MatchFound, "Waiting Lobby 3 get matchId");
	
// Asserts
	AB_TEST_EQUAL(Lobby2MatchId, Lobby3MatchId);
	AB_TEST_NOT_EQUAL(Lobby1MatchId, Lobby2MatchId);

	WaitUntil([&RematchmakingNotifCount](){return RematchmakingNotifCount >= 2;}, "Waiting rematchmaking notif", WaitMatchmakingTime);
	
	LobbyDisconnect(4);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmaking_Timeout, "AccelByte.Tests.Lobby.B.MatchmakingStartTimeout", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmaking_Timeout::RunTest(const FString& Parameters)
{
	LobbyConnect(2);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	bool bMatchmakingNotifTimeout[2] = { false };
	int matchMakingNotifNum = 0;
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil(bMatchmakingNotifTimeout[0], "Waiting for matchmaking timeout...", 300);

	AB_TEST_TRUE(bMatchmakingNotifError[0]);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bDsNotifError);

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

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	bool bIsTheRightRegion[2] = { false };
	bool bLatencyDsNotifSuccess[2] = { false };
	bool bLatencyDsNotifError[2] = { false };
	TArray<TPair<FString, float>> Latencies;
	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(Api::Lobby::FDsNotif::CreateLambda([&Latencies, &bIsTheRightRegion, &bLatencyDsNotifSuccess, &bLatencyDsNotifError](FAccelByteModelsDsNotice result)
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

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(Api::Lobby::FDsNotif::CreateLambda([&Latencies, &bIsTheRightRegion, &bLatencyDsNotifSuccess, &bLatencyDsNotifError](FAccelByteModelsDsNotice result)
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
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();

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
	}, "Waiting for get server latencies (MatchmakingLatenciesStart)...", 60);
	AB_TEST_TRUE(bGetServerLatenciesSuccess);
	AB_TEST_TRUE(Latencies.Num() > 0);

	for (int i = 0; i < Latencies.Num(); i++)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Latencies Region: %s %f"), *Latencies[i].Key, Latencies[i].Value);
	}

	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Selected Region: %s"), *Latencies[0].Key);
	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, "", "", { Latencies[0]});

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelName, "", "", { Latencies[0] });

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bLatencyDsNotifSuccess[0];
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bLatencyDsNotifError[0]);

	WaitUntil([&]()
	{
		return bLatencyDsNotifSuccess[1];
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bLatencyDsNotifError[1]);

	AB_TEST_TRUE(bIsTheRightRegion[0]);
	AB_TEST_TRUE(bIsTheRightRegion[1]);
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
	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));

		if(result.Status == EAccelByteMatchmakingStatus::Done)
		{
			matchmakingNotifResponse[1] = result;
			matchMakingNotifNum++;
			bMatchmakingNotifSuccess[1] = true;
		}
	}));

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, TArray<FString>
	{LobbyUsers[0].UserId});
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelName, TArray<FString>
	{LobbyUsers[1].UserId});
	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&matchMakingNotifNum]()
		{ 
			return matchMakingNotifNum == 2; 
		}, "Wait matchmaking notifs all arrived", WaitMatchmakingTime);

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bDsNotifError);
	
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
		LobbyApiClients[i]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

		LobbyApiClients[i]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

		LobbyApiClients[i]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

		LobbyApiClients[i]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&, i](FAccelByteModelsMatchmakingNotice result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
			matchmakingNotifResponse[i] = result;
			if (result.Status == EAccelByteMatchmakingStatus::Done)
			{
				matchmakingNotifDone++;
				bMatchmakingNotifSuccess[i] = true;
			}
		}));

		LobbyApiClients[i]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);
	}

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelNameTempParty2, TArray<FString>
	{LobbyUsers[0].UserId, LobbyUsers[1].UserId});

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[2]->Lobby.SendStartMatchmaking(ChannelNameTempParty2, TArray<FString>
	{LobbyUsers[2].UserId, LobbyUsers[3].UserId});

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchmakingNotifDone >= UserNum;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[UserNum];
	for (int i = 0; i < UserNum; i++)
	{
		LobbyApiClients[i]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

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
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bDsNotifError);

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
	//Login with FRegistry to trigger Qos poll
	bool bIsLoggedIn = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bIsLoggedIn]()
		{
			bIsLoggedIn = true;
		}), LobbyTestErrorHandler);
	WaitUntil(bIsLoggedIn, TEXT("Waiting for login..."));

	DelaySeconds(5, TEXT("Wait foir QOS fetch"));

	LobbyConnect(1);

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SetCancelMatchmakingResponseDelegate(CancelMatchmakingDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse;
	bool bMatchmakingNotifSuccess = false;
	bool bMatchmakingNotifError = false;
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse = result;
		bMatchmakingNotifSuccess = true;
		if (result.Status != EAccelByteMatchmakingStatus::Cancel)
		{
			bMatchmakingNotifError = true;
		}
	}));

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	LobbyApiClients[0]->Lobby.SendCancelMatchmaking(ChannelName);

	WaitUntil(bCancelMatchmakingSuccess, "Cancelling Matchmaking...");
	AB_TEST_FALSE(bCancelMatchmakingError);


	WaitUntil(bMatchmakingNotifSuccess, "Waiting for Matchmaking Notification...");
	AB_TEST_TRUE(!bMatchmakingNotifError);
	AB_TEST_TRUE(matchmakingNotifResponse.Status == EAccelByteMatchmakingStatus::Cancel);

	//Logout form FRegistry
	bool bIsLoggedOut = false;
	FRegistry::User.Logout(FVoidHandler::CreateLambda([&bIsLoggedOut]()
		{
			bIsLoggedOut = true;
		}), LobbyTestErrorHandler);
	WaitUntil(bIsLoggedOut, TEXT("Waiting for log out..."));

	LobbyDisconnect(1);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestCancelMatchmakingTempParty_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingCancelTempParty", AutomationFlagMaskLobby);
bool LobbyTestCancelMatchmakingTempParty_ReturnOk::RunTest(const FString& Parameters)
{
	//Login with FRegistry to trigger Qos poll
	bool bIsLoggedIn = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bIsLoggedIn]()
		{
			bIsLoggedIn = true;
		}), LobbyTestErrorHandler);
	WaitUntil(bIsLoggedIn, TEXT("Waiting for login..."));

	DelaySeconds(5, TEXT("Wait foir QOS fetch"));

	LobbyConnect(1);

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SetCancelMatchmakingResponseDelegate(CancelMatchmakingDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse;
	bool bMatchmakingNotifSuccess = false;
	bool bMatchmakingNotifError = false;
	bStartMatchmakingSuccess = false;
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Matchmaking Notification!"));
		matchmakingNotifResponse = result;
		bMatchmakingNotifSuccess = true;
		if (result.Status != EAccelByteMatchmakingStatus::Cancel)
		{
			bMatchmakingNotifError = true;
		}
	}));

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, TArray<FString>{LobbyUsers[0].UserId});

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bCancelMatchmakingSuccess = false;
	LobbyApiClients[0]->Lobby.SendCancelMatchmaking(ChannelName, true);

	WaitUntil(bCancelMatchmakingSuccess, "Cancelling Matchmaking...");
	AB_TEST_FALSE(bCancelMatchmakingError);

	WaitUntil(bMatchmakingNotifSuccess, "Waiting or Matchmaking Notification...");
	AB_TEST_TRUE(!bMatchmakingNotifError);
	AB_TEST_TRUE(matchmakingNotifResponse.Status == EAccelByteMatchmakingStatus::Cancel);

	//Logout form FRegistry
	bool bIsLoggedOut = false;
	FRegistry::User.Logout(FVoidHandler::CreateLambda([&bIsLoggedOut]()
		{
			bIsLoggedOut = true;
		}), LobbyTestErrorHandler);
	WaitUntil(bIsLoggedOut, TEXT("Waiting for log out..."));

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

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SetRematchmakingNotifDelegate(Api::Lobby::FRematchmakingNotif::CreateLambda([&](FAccelByteModelsRematchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby 0 Get Rematchmaking Notification!"));
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User %s received: banned for %d secs"), *LobbyUsers[0].UserId, result.BanDuration);
		rematchmakingNotifNum++;
	}));

	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[1]->Lobby.SetRematchmakingNotifDelegate(Api::Lobby::FRematchmakingNotif::CreateLambda([&](FAccelByteModelsRematchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby 1 Get Rematchmaking Notification!"));
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User %s received: banned for %d secs"), *LobbyUsers[1].UserId, result.BanDuration);
		rematchmakingNotifNum++;
	}));

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[2]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[2]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[2]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[2]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[2]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[2]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[2]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[2]->Lobby.SetRematchmakingNotifDelegate(Api::Lobby::FRematchmakingNotif::CreateLambda([&](FAccelByteModelsRematchmakingNotice result)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Lobby 2 Get Rematchmaking Notification!"));
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User %s received: banned for %d secs"), *LobbyUsers[2].UserId, result.BanDuration);
		rematchmakingNotifNum++;
	}));

	LobbyApiClients[2]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[2]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[2]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[2]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");
	AB_TEST_FALSE(bCreatePartyError);

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, "", "", PreferredLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Lobby 0 Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelName, "", "", PreferredLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Lobby 1 Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);

	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

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
	LobbyApiClients[2]->Lobby.SendStartMatchmaking(ChannelName, "", "", PreferredLatencies);

	WaitUntil(bStartMatchmakingSuccess, "Lobby 2 Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 2;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[2].Status, EAccelByteMatchmakingStatus::Done);

	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[2]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[2].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);
	readyConsentNoticeResponse[2] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bDsNotifError);

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
	//Login with FRegistry to trigger Qos poll
	bool bIsLoggedIn = false;
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bIsLoggedIn]()
		{
			bIsLoggedIn = true;
		}), LobbyTestErrorHandler);
	WaitUntil(bIsLoggedIn, TEXT("Waiting for login..."));

	DelaySeconds(5, TEXT("Wait foir QOS fetch"));

	LobbyConnect(2);
	FString ExpectedCustomAttribute {"test"};

	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[0]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[0]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[0]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[0]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	LobbyApiClients[1]->Lobby.SetCreatePartyResponseDelegate(CreatePartyDelegate);

	LobbyApiClients[1]->Lobby.SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	LobbyApiClients[1]->Lobby.SetLeavePartyResponseDelegate(LeavePartyDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponseDelegate);

	LobbyApiClients[1]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotifDelegate);

	LobbyApiClients[1]->Lobby.SetDsNotifDelegate(DsNotifDelegate);

	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[2];
	bool bMatchmakingNotifSuccess[2] = { false };
	bool bMatchmakingNotifError[2] = { false };
	int matchMakingNotifNum = 0;
	LobbyApiClients[0]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[1]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&](FAccelByteModelsMatchmakingNotice result)
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

	LobbyApiClients[0]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[1]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingDelegate);

	LobbyApiClients[0]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		LobbyApiClients[0]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();

	WaitUntil(bCreatePartySuccess, "Creating Party...");

	AB_TEST_FALSE(bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	LobbyApiClients[1]->Lobby.SendInfoPartyRequest();

	WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");

	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		LobbyApiClients[1]->Lobby.SendLeavePartyRequest();

		WaitUntil(bLeavePartySuccess, "Leaving Party...");
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	LobbyApiClients[1]->Lobby.SendCreatePartyRequest();

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
		LobbyTestErrorHandler, ExpectedCustomAttribute);

	WaitUntil(bRegisterLocalServerToDSMDone, "Local DS Register To DSM");

	LobbyApiClients[0]->Lobby.SendStartMatchmaking(ChannelName, ServerName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	LobbyApiClients[1]->Lobby.SendStartMatchmaking(ChannelName, ServerName);

	WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
	AB_TEST_FALSE(bStartMatchmakingError);

	WaitUntil([&]()
	{
		return (matchMakingNotifNum == 2);
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	LobbyApiClients[0]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	LobbyApiClients[1]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);

	WaitUntil([&]()
	{
		return bReadyConsentNotifSuccess;
	}, "Waiting for Ready Consent Notification...", 30);

	AB_TEST_FALSE(bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	WaitUntil([&]()
	{
		return bDsNotifSuccess;
	}, "Waiting for DS Notification...", DsNotifWaitTime);
	AB_TEST_FALSE(bDsNotifError);
	FAccelByteModelsDsNotice ResultDsNotice {dsNotice};

	bool bDeregisterLocalServerFromDSMDone = false;
	FRegistry::ServerDSM.DeregisterLocalServerFromDSM(
		ServerName,
		FVoidHandler::CreateLambda([&bDeregisterLocalServerFromDSMDone]()
		{
			bDeregisterLocalServerFromDSMDone = true;
		}),
		LobbyTestErrorHandler);

	WaitUntil(bDeregisterLocalServerFromDSMDone, "Waiting Deregister Local DS From DSM");

	//Logout form FRegistry
	bool bIsLoggedOut = false;
	FRegistry::User.Logout(FVoidHandler::CreateLambda([&bIsLoggedOut]() 
		{
			bIsLoggedOut = true;
		}), LobbyTestErrorHandler);
	WaitUntil(bIsLoggedOut, TEXT("Waiting for log out..."));

	AB_TEST_FALSE(bMatchmakingNotifError[0]);
	AB_TEST_FALSE(bMatchmakingNotifError[1]);
	AB_TEST_FALSE(matchmakingNotifResponse[0].MatchId.IsEmpty());
	AB_TEST_FALSE(matchmakingNotifResponse[1].MatchId.IsEmpty());
	AB_TEST_EQUAL(matchmakingNotifResponse[0].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(matchmakingNotifResponse[1].Status, EAccelByteMatchmakingStatus::Done);
	AB_TEST_EQUAL(readyConsentNoticeResponse[0].MatchId, matchmakingNotifResponse[0].MatchId);
	AB_TEST_EQUAL(readyConsentNoticeResponse[1].MatchId, matchmakingNotifResponse[1].MatchId);
	AB_TEST_EQUAL(ResultDsNotice.CustomAttribute, ExpectedCustomAttribute);

	LobbyDisconnect(2);
	ResetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestStartMatchmaking3vs3_ReturnOk, "AccelByte.Tests.Lobby.B.MatchmakingStart3vs3", AutomationFlagMaskLobby);
bool LobbyTestStartMatchmaking3vs3_ReturnOk::RunTest(const FString& Parameters)
{
	LobbyConnect(6);

	FThreadSafeCounter readyConsentNoticeCounter = 0;
	auto ReadyConsentNotif = AccelByte::Api::Lobby::FReadyConsentNotif::CreateLambda([&readyConsentNoticeCounter](FAccelByteModelsReadyConsentNotice result)
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
		LobbyApiClients[i]->Lobby.SetCreatePartyResponseDelegate(CreateParty);
		auto GetInfoParty = GetInfoPartyDelegate;
		LobbyApiClients[i]->Lobby.SetInfoPartyResponseDelegate(GetInfoParty);
		auto LeaveParty = LeavePartyDelegate;
		LobbyApiClients[i]->Lobby.SetLeavePartyResponseDelegate(LeaveParty);
		auto ReadyConsentResponse = ReadyConsentResponseDelegate;
		LobbyApiClients[i]->Lobby.SetReadyConsentResponseDelegate(ReadyConsentResponse);
		LobbyApiClients[i]->Lobby.SetReadyConsentNotifDelegate(ReadyConsentNotif);
		auto DsNotif = DsNotifDelegate;
		LobbyApiClients[i]->Lobby.SetDsNotifDelegate(DsNotif);
	}
	FAccelByteModelsMatchmakingNotice matchmakingNotifResponse[6];
	bool bMatchmakingNotifSuccess[6] = { false };
	bool bMatchmakingNotifError[6] = { false };
	int matchMakingNotifNum = 0;
	for (int i = 0; i < 6; i++)
	{
		int numb = i;
		LobbyApiClients[i]->Lobby.SetMatchmakingNotifDelegate(Api::Lobby::FMatchmakingNotif::CreateLambda([&, numb](FAccelByteModelsMatchmakingNotice result)
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
		LobbyApiClients[i]->Lobby.SetStartMatchmakingResponseDelegate(StartMatchmakingResponse);
	}

	for (int i = 0; i < 6; i++)
	{
		LobbyApiClients[i]->Lobby.SendInfoPartyRequest();
		WaitUntil(bGetInfoPartySuccess, "Getting Info Party...");
		if (!bGetInfoPartyError)
		{
			LobbyApiClients[i]->Lobby.SendLeavePartyRequest();
			WaitUntil(bLeavePartySuccess, "Leaving Party...");
			LobbyApiClients[i]->Lobby.SendCreatePartyRequest();
			WaitUntil(bCreatePartySuccess, "Creating Party...");
			AB_TEST_FALSE(bCreatePartyError);
			bLeavePartySuccess = false;
			bLeavePartyError = false;
		}
		else
		{
			LobbyApiClients[i]->Lobby.SendCreatePartyRequest();
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
		LobbyApiClients[i]->Lobby.SendStartMatchmaking(ChannelNameTest3v3, "", "", PreferredLatencies);
		WaitUntil(bStartMatchmakingSuccess, "Starting Matchmaking...");
		AB_TEST_FALSE(bStartMatchmakingError);
		bStartMatchmakingSuccess = false;
		bStartMatchmakingError = false;
	}

	WaitUntil([&]()
	{
		return matchMakingNotifNum >= 6;
	}, "Waiting for Matchmaking Notification...", WaitMatchmakingTime);

	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[6];
	for (int i = 0; i < 6; i++)
	{
		LobbyApiClients[i]->Lobby.SendReadyConsentRequest(matchmakingNotifResponse[i].MatchId);
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
	}, "Waiting for DS Notification...", DsNotifWaitTime);

	AB_TEST_FALSE(bDsNotifError);
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
	bool bLoginDone = false;

	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bLoginDone]() { bLoginDone = true; }), LobbyTestErrorHandler);

	WaitUntil([&]() { return bLoginDone; });
	FThreadSafeCounter setSessionAttributeSuccessCounter;
	FAccelByteModelsGetSessionAttributesResponse getSessionAttributesResponse;
	bool bGetSessionAttributeFinish = false;
	FAccelByteModelsGetAllSessionAttributesResponse getAllSessionAttributesResponse;
	bool bGetAllSessionAttributeFinish = false;


	bool bSetSessionAttributeFailed = false;
	FRegistry::Lobby.SetSetSessionAttributeDelegate(THandler<FAccelByteModelsSetSessionAttributesResponse>::CreateLambda([&setSessionAttributeSuccessCounter, &bSetSessionAttributeFailed](const FAccelByteModelsSetSessionAttributesResponse& result)
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

	FRegistry::Lobby.SetGetSessionAttributeDelegate(THandler<FAccelByteModelsGetSessionAttributesResponse>::CreateLambda([&getSessionAttributesResponse, &bGetSessionAttributeFinish](const FAccelByteModelsGetSessionAttributesResponse& result)
	{
		bGetSessionAttributeFinish = true;
		getSessionAttributesResponse = result;
	}));

	FRegistry::Lobby.SetGetAllSessionAttributeDelegate(THandler<FAccelByteModelsGetAllSessionAttributesResponse>::CreateLambda([&getAllSessionAttributesResponse, &bGetAllSessionAttributeFinish](const FAccelByteModelsGetAllSessionAttributesResponse& result)
	{
		bGetAllSessionAttributeFinish = true;
		getAllSessionAttributesResponse = result;
	}));

	FRegistry::Lobby.Connect();
	WaitUntil([&]() { return FRegistry::Lobby.IsConnected(); }, "", 5);

	// ACT setting session attribute

	for (auto attribute : sessionAttributes)
	{
		FRegistry::Lobby.SetSessionAttribute(attribute.Key, attribute.Value);
	}

	WaitUntil([&setSessionAttributeSuccessCounter, &sessionAttributes]() {return setSessionAttributeSuccessCounter.GetValue() == sessionAttributes.Num(); }, "Waiting set session Attribute", 10);

	FRegistry::Lobby.GetSessionAttribute(sessionAttributesKeys[0]);
	WaitUntil(bGetSessionAttributeFinish, "Wait for get session attribute");

	FRegistry::Lobby.GetAllSessionAttribute();
	WaitUntil(bGetAllSessionAttributeFinish, "Wait for get all session attribute");
	bool bCompareAllSessionAttributes = CompareSessionAttributes(getAllSessionAttributesResponse.attributes, sessionAttributes);

	//Asserts
	AB_TEST_FALSE(bSetSessionAttributeFailed);
	AB_TEST_EQUAL(getSessionAttributesResponse.Value, sessionAttributes[sessionAttributesKeys[0]]);
	AB_TEST_TRUE(bCompareAllSessionAttributes);

	FRegistry::Lobby.Disconnect();

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
		AccelByte::Api::Lobby::FConnectionClosed::CreateLambda([&](int32 CloseCode, FString Reason, bool bWasClean)
		{
			bLobbyDisconnected = true;
		}));
	Lobby.SetDisconnectNotifDelegate(AccelByte::Api::Lobby::FDisconnectNotif::CreateLambda([&](const FAccelByteModelsDisconnectNotif& Result)
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

	DelaySeconds(10, "waiting 10 second");

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
		AccelByte::Api::Lobby::FConnectionClosed::CreateLambda([&](int32 CloseCode, FString Reason, bool bWasClean)
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

	LobbyApiClients[0]->Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	LobbyApiClients[1]->Lobby.SetConnectSuccessDelegate(ConnectSuccessDelegate);
	LobbyApiClients[0]->Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);
	LobbyApiClients[1]->Lobby.SetConnectFailedDelegate(ConnectFailedDelegate);

	LobbyApiClients[0]->Lobby.Connect();

	while (!LobbyApiClients[0]->Lobby.IsConnected() || !bUsersConnectionSuccess)
	{
		FPlatformProcess::Sleep(.5f);
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("LobbyTestSignalingP2P: Wait user 0"));
		FTicker::GetCoreTicker().Tick(.5f);
	}
	userResponded[0] = bUsersConnectionSuccess;
	userConnected[0] = bUsersConnected;
	bUsersConnectionSuccess = false;
	bUsersConnected = false;

	LobbyApiClients[1]->Lobby.Connect();

	while (!LobbyApiClients[1]->Lobby.IsConnected() || !bUsersConnectionSuccess)
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

	LobbyApiClients[0]->Lobby.SetSignalingP2PDelegate(OnP2PDelegate);
	LobbyApiClients[1]->Lobby.SetSignalingP2PDelegate(OnP2PDelegate);
	
	LobbyApiClients[0]->Lobby.SendSignalingMessage(LobbyUsers[1].UserId, Message);
	WaitUntil([&P2PMessageAvailable]() ->bool
	{
		return P2PMessageAvailable;
	});

	AB_TEST_TRUE(P2PMessageAvailable);
	AB_TEST_EQUAL(P2PMessage, Message);
	AB_TEST_EQUAL(P2PDestinationId, LobbyUsers[0].UserId);

	P2PMessageAvailable = false;
	LobbyApiClients[1]->Lobby.SendSignalingMessage(LobbyUsers[0].UserId, Message);
	WaitUntil([&P2PMessageAvailable]() ->bool
	{
		return P2PMessageAvailable;
	});

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
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&bCreatePartyDone, &CreatePartyResult](const FAccelByteModelsCreatePartyResponse result)
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
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party");

	// crate party again, error
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
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
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(THandler<FAccelByteModelsCreatePartyResponse>::CreateLambda([&bCreatePartyDone, &CreatePartyResult](const FAccelByteModelsCreatePartyResponse result)
	{
		CreatePartyResult = result;
		bCreatePartyDone = true;
	}));
	
	// create party
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party");

	// crate party again, error
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
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
	
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartySuccess, CreatePartyError);
	
	// create party
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party");

	// Clear error message, then test error creating party
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.ClearLobbyErrorMessages();
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party2");
	bool bIsErrorAfterClear(bError);
	int32 ErrorCodeAfterClear(ErrorCode);
	FString ErrorMessageAfterClear(ErrorMessage);
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create party after clear error message responded with code %d, codename %s"), ErrorCode, *ErrorMessage);

	DelaySeconds(3, "Delaying 3 sec");
	
	// Disconnect Lobby, reapply setup then try again.
	LobbyDisconnect(1);
	WaitUntil(!LobbyApiClients[0]->Lobby.IsConnected(), "Waiting lobby disconnect");
	
	LobbyConnect(1);
	LobbyApiClients[0]->Lobby.SetCreatePartyResponseDelegate(CreatePartySuccess, CreatePartyError);
	WaitUntil(LobbyApiClients[0]->Lobby.IsConnected(), "Wait reconnecting lobby");

	// Create party again after disconnect
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
	WaitUntil(bCreatePartyDone, "waiting create party", 3);

	// Create party again to test error after reconnecting
	bCreatePartyDone = false;
	LobbyApiClients[0]->Lobby.SendCreatePartyRequest();
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
	LobbyApiClients[0]->Lobby.SetErrorNotifDelegate(AccelByte::Api::Lobby::FErrorNotif::CreateLambda([&](int32 Code, const FString& Message)
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
		LobbyApiClients[0]->Lobby.SendGetOnlineUsersRequest();
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
		}), FCustomErrorHandler::CreateLambda([](int Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
			}));

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
			}), LobbyTestErrorHandler);
	WaitUntil(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

	AB_TEST_TRUE(bServerLoginWithClientCredentialsDone);
	
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("BanUser"));
	FRegistry::ServerUser.BanSingleUser(FRegistry::Credentials.GetUserId(), body, 
		THandler<FBanUserResponse>::CreateLambda([&bBanSuccessful, &BanId](const FBanUserResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			BanId = Result.BanId;
			bBanSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();

	WaitUntil(bBanSuccessful, "Waiting for Ban...");
	WaitUntil(bUserBannedNotif, "Waiting Ban Notification...");

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, "Waiting Lobby Connection Closed...", 65.0);
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
	AdminBanUserChangeStatus(UserId, BanId, false, THandler<FBanUserResponse>::CreateLambda([&bUnbanSuccessful](const FBanUserResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Unbanned: %s"), *Result.UserId);
			bUnbanSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bUnbanSuccessful, "Waiting for Unban...");
	WaitUntil(bUsersUnbannedNotif, "Waiting Unban Notification...");

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, "Waiting Lobby Connection Closed...", 65.0);
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
	AdminBanUserChangeStatus(UserId, BanId, true, THandler<FBanUserResponse>::CreateLambda([&bEnableBanSuccessful](const FBanUserResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			bEnableBanSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();
	WaitUntil(bEnableBanSuccessful, "Waiting for Enable Ban...");
	WaitUntil(bUserBannedNotif, "Waiting Ban Notification...");

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, "Waiting Lobby Connection Closed...", 65.0);
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
		}), FCustomErrorHandler::CreateLambda([](int Code, const FString& Message, const FJsonObject& ErrorJson)
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    Error. Code: %d, Reason: %s"), Code, *Message);
			}));

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
			}), LobbyTestErrorHandler);
	WaitUntil(bServerLoginWithClientCredentialsDone, "Server Login With Client Credentials");

	AB_TEST_TRUE(bServerLoginWithClientCredentialsDone);
	 
	FRegistry::ServerUser.BanSingleUser(FRegistry::Credentials.GetUserId(), body, 
		THandler<FBanUserResponse>::CreateLambda([&bBanSuccessful, &BanId](const FBanUserResponse& Result)
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Banned: %s"), *Result.UserId);
			BanId = Result.BanId;
			bBanSuccessful = true;
		}), LobbyTestErrorHandler);

	FlushHttpRequests();

	WaitUntil(bBanSuccessful, "Waiting for Ban...");

	// Wait Lobby connection closed
	WaitUntil(bLobbyConnectionClosed, "Waiting Lobby Connection Closed...", 65.0);
	WaitUntil(bLobbyDisconnected, "Waiting Lobby Disconnected...", 65.0);
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
		}), FCustomErrorHandler::CreateLambda([&bLoginError, &bLoginDone](int Code, const FString& Message, const FJsonObject& ErrorJson)
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
	AdminBanUserChangeStatus(UserId, BanId, false, THandler<FBanUserResponse>::CreateLambda([&bUnbanSuccessful](const FBanUserResponse& Result)
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
		}), FCustomErrorHandler::CreateLambda([&bLoginDone](int Code, const FString& Message, const FJsonObject& ErrorJson)
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

	WaitUntil(bUserLoggedIn, "Waiting user logged in");

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
	DelaySeconds(10, "Waiting Lobby Connecting");

	Lobby.Disconnect();
	DelaySeconds(3, "Waiting Lobby Disconnect");
	
	bool bUserDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bUserDeleteDone]()
	{
		bUserDeleteDone = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bUserDeleteDone, "Waiting delete user for cleanup");

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
	AdminGetEcommercePublishedStore(PublisherNamespace,
		THandler<FStoreInfo>::CreateLambda([&StoreId, &bGetPublishedStoreDone](const FStoreInfo& Result)
		{
			StoreId = Result.storeId;
			bGetPublishedStoreDone = true;
		}), LobbyTestErrorHandler);

	WaitUntil(bGetPublishedStoreDone, "Waiting get store ID from published store");
	
	// Arrange Get itemID from SKU
	FString ItemId = "";
	bool bGetItemIdDone = false;
	AdminGetEcommerceItemBySKU(PublisherNamespace, StoreId, Sku, false,
		THandler<FItemFullInfo>::CreateLambda([&ItemId, &bGetItemIdDone](const FItemFullInfo& result)
		{
			ItemId = result.itemId;
			bGetItemIdDone = true;
		}), LobbyTestErrorHandler);

	WaitUntil(bGetItemIdDone, "Waiting get Item Id from SKU");
	
	// Arrange
	AccelByte::Api::User& User = FRegistry::User;
	AccelByte::Api::Lobby& Lobby = FRegistry::Lobby;
	AccelByte::Credentials& Credentials = FRegistry::Credentials;
	
	// Arrange login user
	bool bUserLoggedIn = false;
	User.LoginWithDeviceId(FVoidHandler::CreateLambda([&bUserLoggedIn]()
	{
		bUserLoggedIn = true;
	}), LobbyTestErrorHandler);

	WaitUntil(bUserLoggedIn, "Waiting user logged in");

	// Arrange Get User Publisher Id
	FString PublisherUserId = "";
	bool bGetPublisherUserId = false;
	AdminGetUserMap(Credentials.GetUserId(),
		THandler<FUserMapResponse>::CreateLambda([&PublisherUserId, &bGetPublisherUserId](const FUserMapResponse& result)
		{
			PublisherUserId = result.userId;
			bGetPublisherUserId = true;
		}), LobbyTestErrorHandler);

	WaitUntil(bGetPublisherUserId, "Waiting get publisher user id");
	
	// Arrange Grant App to user
	FFulfillmentRequest ToGrant;
	ToGrant.ItemId = ItemId;
	ToGrant.Quantity = 1;
	ToGrant.EndDate = (FDateTime::UtcNow() + FTimespan::FromMinutes(10)).ToIso8601();

	bool bItemGrantDone = false;
	AdminFulfillItem(PublisherNamespace, PublisherUserId, ToGrant,
		FVoidHandler::CreateLambda([&bItemGrantDone]()
		{
			bItemGrantDone = true;
		}), LobbyTestErrorHandler);

	WaitUntil(bItemGrantDone, "Waiting granting Item to user");

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
	DelaySeconds(3, "Waiting Lobby Connecting");

	Lobby.Disconnect();
	DelaySeconds(3, "Waiting Lobby Disconnect");

	bool bUserDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bUserDeleteDone]()
	{
		bUserDeleteDone = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bUserDeleteDone, "Waiting delete user for cleanup");

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
	}), LobbyTestErrorHandler);

	WaitUntil(bUserLoggedIn, "Waiting user logged in");

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
	DelaySeconds(10, "Waiting Lobby Connecting");

	Lobby.Disconnect();
	DelaySeconds(3, "Waiting Lobby Disconnect");
	
	bool bUserDeleteDone = false;
	AdminDeleteUser(FRegistry::Credentials.GetUserId(), FSimpleDelegate::CreateLambda([&bUserDeleteDone]()
	{
		bUserDeleteDone = true;
	}), LobbyTestErrorHandler);
	WaitUntil(bUserDeleteDone, "Waiting delete user for cleanup");

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