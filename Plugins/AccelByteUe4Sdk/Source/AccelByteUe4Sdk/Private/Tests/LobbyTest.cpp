// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AutomationTest.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "AccelByteOauth2Api.h"
#include "AccelByteUserApi.h"
#include "AccelByteRegistry.h"
#include "AccelByteLobbyApi.h"
#include "TestUtilities.h"
#include "FileManager.h"

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;
using AccelByte::Api::User;
using AccelByte::Api::Oauth2;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLobbyTest, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteLobbyTest);

const int32 AutomationFlagMaskLobby = (EAutomationTestFlags::EditorContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ClientContext);

const auto LobbyTestErrorHandler = FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
{
	UE_LOG(LogAccelByteLobbyTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
});

const int TestUserCount = 5;
FString UserIds[TestUserCount];
Credentials UserCreds[TestUserCount];
TArray<TSharedPtr<Api::Lobby>> Lobbies;
bool bUsersConnected, bUsersConnectionSuccess, bGetMessage, bGetAllUserPresenceSuccess, bRequestFriendSuccess;
bool bRequestFriendError, bAcceptFriendSuccess, bAcceptFriendError, bReceivedPartyChatSuccess, bSendPartyChatSuccess, bSendPartyChatError;
bool bCreatePartySuccess, bCreatePartyError, bInvitePartySuccess, bGetInvitedNotifSuccess, bGetInvitedNotifError;
bool bJoinPartySuccess, bJoinPartyError, bLeavePartySuccess, bLeavePartyError, bGetInfoPartySuccess, bGetInfoPartyError;
bool bKickPartyMemberSuccess, bKickPartyMemberError, bKickedFromPartySuccess, bGetNotifSuccess, bUserPresenceSuccess;
bool bUserPresenceError, bUserPresenceNotifSuccess, bUserPresenceNotifError, bUnfriendSuccess, bUnfriendError, bGetFriendshipStatusSuccess;
bool bGetFriendshipStatusError, bListOutgoingFriendSuccess, bListOutgoingFriendError, bListIncomingFriendSuccess, bListIncomingFriendError;
bool bLoadFriendListSuccess, bLoadFriendListError, bOnIncomingRequestNotifSuccess, bOnIncomingRequestNotifError, bOnRequestAcceptedNotifSuccess, bOnRequestAcceptedNotifError;
bool bRejectFriendSuccess, bRejectFriendError, bCancelFriendSuccess, bCancelFriendError, bStartMatchmakingSuccess, bStartMatchmakingError;
bool bCancelMatchmakingSuccess, bCancelMatchmakingError, bReadyConsentResponseSuccess, bReadyConsentResponseError, bReadyConsentNotifSuccess, bReadyConsentNotifError;
bool bDsNotifSuccess, bDsNotifError;
FAccelByteModelsPartyGetInvitedNotice invitedToPartyResponse;
FAccelByteModelsInfoPartyResponse infoPartyResponse;
FAccelByteModelsPartyJoinReponse joinPartyResponse;
FAccelByteModelsGetOnlineUsersResponse onlineUserResponse;
FAccelByteModelsNotificationMessage getNotifResponse;
FAccelByteModelsUsersPresenceNotice userPresenceNotifResponse;
FAccelByteModelsGetFriendshipStatusResponse getFriendshipStatusResponse;
FAccelByteModelsListOutgoingFriendsResponse listOutgoingFriendResponse;
FAccelByteModelsListIncomingFriendsResponse listIncomingFriendResponse;
FAccelByteModelsLoadFriendListResponse loadFriendListResponse;
FAccelByteModelsRequestFriendsNotif requestFriendNotifResponse;
FAccelByteModelsAcceptFriendsNotif acceptFriendNotifResponse;
FAccelByteModelsMatchmakingResponse matchmakingResponse;
FAccelByteModelsReadyConsentNotice readyConsentNotice;
FAccelByteModelsDsNotice dsNotice;

void Waiting(FString text)
{
	FPlatformProcess::Sleep(.5f);
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("%s"), *text);
	FTicker::GetCoreTicker().Tick(.5f);
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
		FString text = FString::Printf(TEXT("Wait user %d"), i);
		while (!Lobbies[i]->IsConnected())
		{
			Waiting(text);
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
	}
}

void resetResponses()
{
	bUsersConnected = false;
	bUsersConnectionSuccess = false;
	bGetMessage = false;
	bGetAllUserPresenceSuccess = false;
	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	bAcceptFriendSuccess = false;
	bAcceptFriendError = false;
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	bInvitePartySuccess = false;
	bGetInvitedNotifSuccess = false;
	bGetInvitedNotifError = false;
	bJoinPartySuccess = false;
	bJoinPartyError = false;
	bLeavePartySuccess = false;
	bLeavePartyError = false;
	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	bKickPartyMemberSuccess = false;
	bKickPartyMemberError = false;
	bKickedFromPartySuccess = false;
	bReceivedPartyChatSuccess = false;
	bSendPartyChatSuccess = false;
	bSendPartyChatError = false;
	bGetNotifSuccess = false;
	bUserPresenceSuccess = false;
	bUserPresenceError = false;
	bUserPresenceNotifSuccess = false;
	bUserPresenceNotifError = false;
	bUnfriendSuccess = false;
	bUnfriendError = false;
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
	bCancelFriendSuccess = false;
	bCancelFriendError = false;
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
}

auto ConnectSuccessDelegate = Api::Lobby::FConnectSuccess::CreateLambda([&]()
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User connected!"));
	bUsersConnected = true;
	bUsersConnectionSuccess = true;
});

auto ConnectFailedDelegate = FErrorHandler::CreateLambda([&](int32 Code, FString Message)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User failed to connect!"));
	bUsersConnected = false;
	bUsersConnectionSuccess = true;
});

auto GetMessageDelegate = Api::Lobby::FPersonalChatNotif::CreateLambda([&](FAccelByteModelsPersonalMessageNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Payload : %s"), *result.Payload);
	bGetMessage = true;
});

auto GetAllUsersPresenceDelegate = Api::Lobby::FGetAllFriendsStatusResponse::CreateLambda([&](FAccelByteModelsGetOnlineUsersResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("GetAllUserPresence Success!"));
	bGetAllUserPresenceSuccess = true;
	onlineUserResponse = result;
});

auto RequestFriendDelegate = Api::Lobby::FRequestFriendsResponse::CreateLambda([&](FAccelByteModelsRequestFriendsResponse result)
{
	bRequestFriendSuccess = true;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Request Friend Success!"));
	if (result.Code != "0")
	{
		bRequestFriendError = true;
	}
});

auto AcceptFriendsDelegate = Api::Lobby::FAcceptFriendsResponse::CreateLambda([&](FAccelByteModelsAcceptFriendsResponse result)
{
	bAcceptFriendSuccess = true;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Accept Friend Success!"));
	if (result.Code != "0")
	{
		bAcceptFriendError = true;
	}
});

auto GetInfoPartyDelegate = Api::Lobby::FPartyInfoResponse::CreateLambda([&](FAccelByteModelsInfoPartyResponse result)
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

auto LeavePartyDelegate = Api::Lobby::FPartyLeaveResponse::CreateLambda([&](FAccelByteModelsLeavePartyResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Leave Party Success!"));
	bLeavePartySuccess = true;
	if (result.Code != "0")
	{
		bLeavePartyError = true;
	}
});

auto CreatePartyDelegate = Api::Lobby::FPartyCreateResponse::CreateLambda([&](FAccelByteModelsCreatePartyResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Create Party Success!"));
	bCreatePartySuccess = true;
	if (result.PartyId.IsEmpty())
	{
		bCreatePartyError = true;
	}
});

auto InvitePartyDelegate = Api::Lobby::FPartyInviteResponse::CreateLambda([&](FAccelByteModelsPartyInviteResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Invite Party Success!"));
	bInvitePartySuccess = true;
});

auto InvitedToPartyDelegate = Api::Lobby::FPartyGetInvitedNotif::CreateLambda([&](FAccelByteModelsPartyGetInvitedNotice result)
{
	invitedToPartyResponse = result;
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Party Invitation!"));
	bGetInvitedNotifSuccess = true;
	if (result.PartyId.IsEmpty())
	{
		bGetInvitedNotifError = true;
	}
});

auto JoinPartyDelegate = Api::Lobby::FPartyJoinResponse::CreateLambda([&](FAccelByteModelsPartyJoinReponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Join Party Success! Member : %d"), result.Members.Num());
	joinPartyResponse = result;
	bJoinPartySuccess = true;
	if (result.Code != "0")
	{
		bJoinPartyError = false;
	}
});

auto PartyChatNotifDelegate = Api::Lobby::FPartyChatNotif::CreateLambda([&](FAccelByteModelsPartyMessageNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get a Party Message!"));
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("From : %s | Message : %s | At : %s"), *result.From, *result.Payload, *result.ReceivedAt);
	bReceivedPartyChatSuccess = true;
});

auto PartyChatSendDelegate = Api::Lobby::FPartyChatResponse::CreateLambda([&](FAccelByteModelsPartyMessageResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Send Party Chat Success!"));
	bSendPartyChatSuccess = true;
	if (result.Code != "0")
	{
		bSendPartyChatError = true;
	}
});

auto KickPartyMemberDelegate = Api::Lobby::FPartyKickResponse::CreateLambda([&](FAccelByteModelsKickPartyMemberResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Kick Party Member Success!"));
	bKickPartyMemberSuccess = true;
	if (result.Code != "0")
	{
		bKickPartyMemberError = true;
	}
});

auto KickedFromPartyDelegate = Api::Lobby::FPartyKickNotif::CreateLambda([&](FAccelByteModelsGotKickedFromPartyNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Kicked From Party!"));
	{
		bKickedFromPartySuccess = true;
	}
});

auto GetNotifDelegate = Api::Lobby::FMessageNotif::CreateLambda([&](FAccelByteModelsNotificationMessage result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Notification!"));
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("From : %s | Message : %s"), *result.From, *result.Payload);
	getNotifResponse = result;
	bGetNotifSuccess = true;
});

auto UserPresenceDelegate = Api::Lobby::FSetUserPresenceResponse::CreateLambda([&](FAccelByteModelsSetOnlineUsersResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("User Presence Changed!"));
	bUserPresenceSuccess = true;
	if (result.Code != "0")
	{
		bUserPresenceError = true;
	}
});

auto UserPresenceNotifDelegate = Api::Lobby::FFriendStatusNotif::CreateLambda([&](FAccelByteModelsUsersPresenceNotice result)
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

auto UnfriendDelegate = Api::Lobby::FUnfriendResponse::CreateLambda([&](FAccelByteModelsUnfriendResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Unfriend Success!"));
	bUnfriendSuccess = true;
	if (result.Code != "0")
	{
		bUnfriendError = true;
	}
});

auto GetFriendshipStatusDelegate = Api::Lobby::FGetFriendshipStatusResponse::CreateLambda([&](FAccelByteModelsGetFriendshipStatusResponse result)
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

auto ListOutgoingFriendDelegate = Api::Lobby::FListOutgoingFriendsResponse::CreateLambda([&](FAccelByteModelsListOutgoingFriendsResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get List Outgoing Friend Request!"));
	listOutgoingFriendResponse = result;
	bListOutgoingFriendSuccess = true;
	if (result.Code != "0")
	{
		bListOutgoingFriendError = true;
	}
});

auto ListIncomingFriendDelegate = Api::Lobby::FListIncomingFriendsResponse::CreateLambda([&](FAccelByteModelsListIncomingFriendsResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get List Incoming Friend Request!"));
	listIncomingFriendResponse = result;
	bListIncomingFriendSuccess = true;
	if (result.Code != "0")
	{
		bListIncomingFriendError = true;
	}
});

auto LoadFriendListDelegate = Api::Lobby::FLoadFriendListResponse::CreateLambda([&](FAccelByteModelsLoadFriendListResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Load Friend List!"));
	loadFriendListResponse = result;
	bLoadFriendListSuccess = true;
	if (result.Code != "0")
	{
		bLoadFriendListError = true;
	}
});

auto OnIncomingRequestNotifDelegate = Api::Lobby::FRequestFriendsNotif::CreateLambda([&](FAccelByteModelsRequestFriendsNotif result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get Friend Request!"));
	requestFriendNotifResponse = result;
	bOnIncomingRequestNotifSuccess = true;
	if (result.friendId.IsEmpty())
	{
		bOnIncomingRequestNotifError = true;
	}
});

auto OnRequestAcceptedNotifDelegate = Api::Lobby::FAcceptFriendsNotif::CreateLambda([&](FAccelByteModelsAcceptFriendsNotif result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Friend Request Accepted!"));
	acceptFriendNotifResponse = result;
	bOnRequestAcceptedNotifSuccess = true;
	if (result.friendId.IsEmpty())
	{
		bOnRequestAcceptedNotifError = true;
	}
});

auto RejectFriendDelegate = Api::Lobby::FRejectFriendsResponse::CreateLambda([&](FAccelByteModelsRejectFriendsResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Reject Friend Success!"));
	bRejectFriendSuccess = true;
	if (result.Code != "0")
	{
		bRejectFriendError = true;
	}
});

auto CancelFriendDelegate = Api::Lobby::FCancelFriendsResponse::CreateLambda([&](FAccelByteModelsCancelFriendsResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Cancel Friend Success!"));
	bCancelFriendSuccess = true;
	if (result.Code != "0")
	{
		bCancelFriendError = true;
	}
});

auto StartMatchmakingDelegate = Api::Lobby::FMatchmakingResponse::CreateLambda([&](FAccelByteModelsMatchmakingResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Start Matchmaking Success!"));
	matchmakingResponse = result;
	bStartMatchmakingSuccess = true;
	if (result.Code != "0")
	{
		bStartMatchmakingError = true;
	}
});

auto CancelMatchmakingDelegate = Api::Lobby::FMatchmakingResponse::CreateLambda([&](FAccelByteModelsMatchmakingResponse result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Cancel Matchmaking Success!"));
	matchmakingResponse = result;
	bCancelMatchmakingSuccess = true;
	if (result.Code != "0")
	{
		bCancelMatchmakingError = true;
	}
});

auto ReadyConsentResponseDelegate = Api::Lobby::FReadyConsentResponse::CreateLambda([&](FAccelByteModelsReadyConsentRequest result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Ready Consent Send!"));
	bReadyConsentResponseSuccess = true;
});

auto ReadyConsentNotifDelegate = Api::Lobby::FReadyConsentNotif::CreateLambda([&](FAccelByteModelsReadyConsentNotice result)
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

auto DsNotifDelegate = Api::Lobby::FDsNotif::CreateLambda([&](FAccelByteModelsDsNotice result)
{
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Get DS Notice!"));
	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DS ID: %s | Message: %s | Status: %s"), *result.MatchId, *result.Message, *result.Status);
	dsNotice = result;
	bDsNotifSuccess = true;
	if (result.MatchId.IsEmpty())
	{
		bDsNotifError = true;
	}
});

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestSetup, "AccelByte.Tests.Lobby.A.Setup", AutomationFlagMaskLobby);
bool LobbyTestSetup::RunTest(const FString& Parameters)
{
	bool bClientLoginSuccess = false;
	bool UsersCreationSuccess[TestUserCount];
	bool UsersLoginSuccess[TestUserCount];

	User::LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoginSuccess]()
	{
		bClientLoginSuccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Client Login Success"));
	}), LobbyTestErrorHandler);
	FlushHttpRequests();

	for (int i = 0; i < TestUserCount; i++)
	{
		FString Email = FString::Printf(TEXT("lobbyUE4Test+%d@example.com"), i);
		FString Password = TEXT("Password");
		FString DisplayName = FString::Printf(TEXT("lobbyUE4%d"), i);

		AccelByte::Api::User::Register(Email, Password, DisplayName, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
		{
			UsersCreationSuccess[i] = true;
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Test Lobby User %d/%d is Created"), i, TestUserCount);
		}), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			if (Code == EHttpResponseCodes::Conflict)
			{
				UsersCreationSuccess[i] = true;
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Test Lobby User %d/%d is already"), i, TestUserCount);
			}
			else
			{
				UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Test Lobby User %d/%d can't be created"), i, TestUserCount);
			}
		}));
		FlushHttpRequests();

		Oauth2::GetAccessTokenWithPasswordGrant(
			FRegistry::Settings.ClientId,
			FRegistry::Settings.ClientSecret,
			Email,
			Password,
			THandler<FOauth2Token>::CreateLambda([&](const FOauth2Token& Token)
		{
			UsersLoginSuccess[i] = true;
			UserCreds[i].SetUserToken(Token.Access_token, Token.Refresh_token, Token.Expires_in, Token.User_id, Token.Display_name, Token.Namespace);
			UserIds[i] = Token.User_id;
		}), LobbyTestErrorHandler);
		FlushHttpRequests();

		Lobbies.Add(MakeShared<Api::Lobby>(UserCreds[i], FRegistry::Settings));
	}

	check(bClientLoginSuccess);
	for (int i = 0; i < TestUserCount; i++)
	{
		check(UsersLoginSuccess[i]);
	}

	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestTeardown, "AccelByte.Tests.Lobby.Z.Teardown", AutomationFlagMaskLobby);
bool LobbyTestTeardown::RunTest(const FString& Parameters)
{
	bool bDeleteUsersSuccessful[TestUserCount];
	Lobbies.Reset(0);

	for (int i = 0; i < TestUserCount; i++)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("DeleteUserById (%d/%d)"), i + 1, TestUserCount);
		DeleteUserById(UserCreds[i].GetUserId(), FSimpleDelegate::CreateLambda([&]()
		{
			UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Success"));
			bDeleteUsersSuccessful[i] = true;
		}), LobbyTestErrorHandler);
		FlushHttpRequests();
	}

	for (int i = 0; i < TestUserCount; i++)
	{
		check(bDeleteUsersSuccessful[i]);
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
		Waiting("Wait user 0");
	}
	userResponded[0] = bUsersConnectionSuccess;
	userConnected[0] = bUsersConnected;
	bUsersConnectionSuccess = false;
	bUsersConnected = false;

	Lobbies[1]->Connect();

	while (!Lobbies[1]->IsConnected() || !bUsersConnectionSuccess)
	{
		Waiting("Wait user 1");
	}
	userResponded[1] = bUsersConnectionSuccess;
	userConnected[1] = bUsersConnected;

	for (int i = 0; i < 2; i++)
	{
		check(userConnected[i]);
		check(userResponded[i]);
	}
	LobbyDisconnect(2);
	resetResponses();
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
		Waiting("Wait user 0");
	}

	check(bUsersConnected);
	check(bUsersConnectionSuccess);

	LobbyDisconnect(1);
	resetResponses();
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
		while (!bGetMessage)
		{
			Waiting(text);
		}
		if (bGetMessage)
		{
			bGetMessage = false;
			receivedChatCount++;
		}
	}

	UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Received Message : %d"), receivedChatCount);
	check(receivedChatCount >= (TestUserCount - 1));

	LobbyDisconnect(TestUserCount);
	resetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllUsers, "AccelByte.Tests.Lobby.B.ListOnlineFriends", AutomationFlagMaskLobby);
bool LobbyTestListOnlineFriends_MultipleUsersConnected_ReturnAllUsers::RunTest(const FString& Parameters)
{
	LobbyConnect(TestUserCount);

	Lobbies[0]->SetGetAllUserPresenceResponseDelegate(GetAllUsersPresenceDelegate);

	Lobbies[1]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);
	Lobbies[2]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);
	Lobbies[3]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);
	Lobbies[4]->SetRequestFriendsResponseDelegate(RequestFriendDelegate);

	Lobbies[0]->SetAcceptFriendsResponseDelegate(AcceptFriendsDelegate);

	for (int i = 1; i < TestUserCount; i++)
	{
		Lobbies[i]->RequestFriend(UserCreds[0].GetUserId());
		FString text = FString::Printf(TEXT("Requesting Friend %d... "), i);
		while (!bRequestFriendSuccess)
		{
			Waiting(text);
		}
		Lobbies[0]->AcceptFriend(UserCreds[i].GetUserId());
		text = FString::Printf(TEXT("Accepting Friend %d... "), i);
		while (!bAcceptFriendSuccess)
		{
			Waiting(text);
		}
		Lobbies[i]->SendSetPresenceStatus(Availability::Availabe, "random activity");
		bRequestFriendSuccess = false;
		bAcceptFriendSuccess = false;
	}


	Lobbies[0]->SendGetOnlineUsersRequest();
	while (!bGetAllUserPresenceSuccess)
	{
		Waiting("Getting Friend Status...");;
	}

	for (int i = 1; i < TestUserCount; i++)
	{
		Lobbies[i]->SendSetPresenceStatus(Availability::Offline, "disappearing");
		Lobbies[i]->Unfriend(UserCreds[0].GetUserId());
	}

	LobbyDisconnect(TestUserCount);

	check(bGetAllUserPresenceSuccess);
	check(onlineUserResponse.friendsId.Num() >= 3);
	check(onlineUserResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	check(onlineUserResponse.friendsId.Contains(UserCreds[2].GetUserId()));
	check(onlineUserResponse.friendsId.Contains(UserCreds[3].GetUserId()));
	resetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestGetPartyInfo_NoParty_ReturnError, "AccelByte.Tests.Lobby.B.GetPartyInfoError", AutomationFlagMaskLobby);
bool LobbyTestGetPartyInfo_NoParty_ReturnError::RunTest(const FString& Parameters)
{
	LobbyConnect(1);

	Lobbies[0]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SendLeavePartyRequest();
	while (!bLeavePartySuccess)
	{
		Waiting("Leaving Party...");
	}

	Lobbies[0]->SendInfoPartyRequest();
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}

	LobbyDisconnect(2);
	check(bGetInfoPartyError);
	resetResponses();
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
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}

	Lobbies[0]->SendInfoPartyRequest();
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}

	Lobbies[0]->SendLeavePartyRequest();
	while (!bLeavePartySuccess)
	{
		Waiting("Leaving Party...");
	}

	LobbyDisconnect(1);

	check(bGetInfoPartySuccess);
	check(!infoPartyResponse.PartyId.IsEmpty());
	check(!infoPartyResponse.InvitationToken.IsEmpty());
	check(infoPartyResponse.Members.Num() > 0);
	check(infoPartyResponse.Members[0] == UserCreds[0].GetUserId());

	resetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestCreateParty_PartyAlreadyCreated_ReturnError, "AccelByte.Tests.Lobby.B.CreatePartyError", AutomationFlagMaskLobby);
bool LobbyTestCreateParty_PartyAlreadyCreated_ReturnError::RunTest(const FString& Parameters)
{
	LobbyConnect(1);

	Lobbies[0]->SetCreatePartyResponseDelegate(CreatePartyDelegate);

	Lobbies[0]->SetLeavePartyResponseDelegate(LeavePartyDelegate);

	Lobbies[0]->SendLeavePartyRequest();
	while (!bLeavePartySuccess)
	{
		Waiting("Leaving Party...");
	}

	Lobbies[0]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}
	bCreatePartySuccess = false;

	Lobbies[0]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}

	Lobbies[0]->SendLeavePartyRequest();
	while (!bLeavePartySuccess)
	{
		Waiting("Leaving Party...");
	}

	LobbyDisconnect(1);
	check(bCreatePartyError);

	resetResponses();
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

	Lobbies[1]->SetInfoPartyResponseDelegate(GetInfoPartyDelegate);

	Lobbies[0]->SendInfoPartyRequest();
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}
	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();
		while (!bLeavePartySuccess)
		{
			Waiting("Leaving Party...");
		}
	}
	Lobbies[0]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}
	check(!bCreatePartyError);

	Lobbies[0]->SendInviteToPartyRequest(UserCreds[1].GetUserId());
	while (!bInvitePartySuccess)
	{
		Waiting("Inviting to Party...");
	}
	check(bInvitePartySuccess);

	while (!bGetInvitedNotifSuccess)
	{
		Waiting("Waiting for Party Invitation...");
	}
	check(!bGetInvitedNotifError);

	Lobbies[1]->SendAcceptInvitationRequest(*invitedToPartyResponse.PartyId, *invitedToPartyResponse.InvitationToken);
	while (!bJoinPartySuccess && !bGetInvitedNotifError)
	{
		Waiting("Joining a Party...");
	}
	check(!bJoinPartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}
	check(!bGetInfoPartyError);

	Lobbies[1]->SendPartyMessage("This is a party chat");
	while (!bSendPartyChatSuccess)
	{
		Waiting("Sending a Party Chat...");
	}
	check(!bSendPartyChatError);

	while (!bReceivedPartyChatSuccess && !bSendPartyChatError)
	{
		Waiting("Fetching Party Chat...");
	}
	check(bReceivedPartyChatSuccess);

	bLeavePartySuccess = false;
	bLeavePartyError = false;
	Lobbies[0]->SendLeavePartyRequest();
	while (!bLeavePartySuccess)
	{
		Waiting("Leaving Party...");
	}
	check(!bLeavePartyError);

	bLeavePartySuccess = false;
	bLeavePartyError = false;
	Lobbies[1]->SendLeavePartyRequest();
	while (!bLeavePartySuccess)
	{
		Waiting("Leaving Party...");
	}
	check(!bLeavePartyError);

	LobbyDisconnect(2);

	resetResponses();
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

	Lobbies[0]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}

	Lobbies[0]->SendInviteToPartyRequest(UserCreds[1].GetUserId());
	while (!bInvitePartySuccess)
	{
		Waiting("Inviting to Party...");
	}
	bInvitePartySuccess = false;
	while (!bGetInvitedNotifSuccess)
	{
		Waiting("Waiting for Party Invitation");
	}
	bGetInvitedNotifSuccess = false;
	invitedToParty[0] = invitedToPartyResponse;

	Lobbies[0]->SendInviteToPartyRequest(UserCreds[2].GetUserId());
	while (!bInvitePartySuccess)
	{
		Waiting("Inviting to Party...");
	}
	bInvitePartySuccess = false;
	while (!bGetInvitedNotifSuccess)
	{
		Waiting("Waiting for Party Invitation");
	}
	bGetInvitedNotifSuccess = false;
	invitedToParty[1] = invitedToPartyResponse;

	Lobbies[1]->SendAcceptInvitationRequest(invitedToParty[0].PartyId, invitedToParty[0].InvitationToken);
	while (!bJoinPartySuccess)
	{
		Waiting("Joining a Party...");
	}
	bJoinPartySuccess = false;
	joinParty[0] = joinPartyResponse;

	Lobbies[2]->SendAcceptInvitationRequest(invitedToParty[1].PartyId, invitedToParty[1].InvitationToken);
	while (!bJoinPartySuccess)
	{
		Waiting("Joining a Party...");
	}
	bJoinPartySuccess = false;
	joinParty[1] = joinPartyResponse;

	Lobbies[0]->SendKickPartyMemberRequest(UserCreds[2].GetUserId());
	while (!bKickPartyMemberSuccess)
	{
		Waiting("Kicking Party Member...");
	}

	while (!bKickedFromPartySuccess)
	{
		Waiting("Waiting to Get Kicked from Party...");
	}

	Lobbies[1]->SendInfoPartyRequest();
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}

	for (int i = 0; i < 2; i++)
	{
		Lobbies[i]->SendLeavePartyRequest();
		while (!bLeavePartySuccess)
		{
			Waiting("Leaving Party...");
		}
		bLeavePartySuccess = false;
	}


	LobbyDisconnect(3);

	check(bKickPartyMemberSuccess);
	check(bKickedFromPartySuccess);
	check(joinParty[2].Members.Num() == 3 || joinParty[1].Members.Num() == 3);
	check(infoPartyResponse.Members.Num() == 2);

	resetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestConnected_ForMoreThan1Minutes_DoesntDisconnect, "AccelByte.IgnoredTests.Lobby.B.Connect1Mins", AutomationFlagMaskLobby);
bool LobbyTestConnected_ForMoreThan1Minutes_DoesntDisconnect::RunTest(const FString& Parameters)
{
	LobbyConnect(1);
	for (int i = 0; i < 100; i += 5)
	{
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Wait for %d seconds. Lobby.IsConnected=%d"), i, Lobbies[0]->IsConnected());
		FPlatformProcess::Sleep(5);
		FTicker::GetCoreTicker().Tick(5);
	}
	check(Lobbies[0]->IsConnected());

	LobbyDisconnect(1);
	resetResponses();
	return true;
}

IMPLEMENT_SIMPLE_AUTOMATION_TEST(LobbyTestNotification_GetAsyncNotification, "AccelByte.Tests.Lobby.B.NotifAsync", AutomationFlagMaskLobby);
bool LobbyTestNotification_GetAsyncNotification::RunTest(const FString& Parameters)
{
	bool bSendNotifSucccess = false;
	FString notification = "this is a notification";
	UAccelByteBlueprintsTest::SendNotif(UserCreds[0].GetUserId(), notification, true, FVoidHandler::CreateLambda([&]()
	{
		bSendNotifSucccess = true;
		UE_LOG(LogAccelByteLobbyTest, Log, TEXT("Notification Sent!"));
	}), LobbyTestErrorHandler);

	while (!bSendNotifSucccess)
	{
		Waiting("Sending Notification...");
	}

	LobbyConnect(1);

	Lobbies[0]->SetMessageNotifDelegate(GetNotifDelegate);

	Lobbies[0]->GetAllAsyncNotification();

	while (!bGetNotifSuccess)
	{
		Waiting("Getting All Notifications...");
	}

	LobbyDisconnect(1);
	check(bSendNotifSucccess);
	check(bGetNotifSuccess);
	check(getNotifResponse.Payload == notification);

	resetResponses();
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

		while (!bSendNotifSucccess[i])
		{
			Waiting("Sending Notification...");
		}

		while (!bGetNotifSuccess)
		{
			Waiting("Getting All Notifications...");
		}

		bGetNotifCheck[i] = bGetNotifSuccess;
		getNotifCheck[i] = getNotifResponse;
		bGetNotifSuccess = false;
	}
	LobbyDisconnect(1);
	for (int i = 0; i < repetition; i++)
	{
		check(bGetNotifCheck[i]);
		check(getNotifCheck[i].Payload == payloads[i]);
	}
	resetResponses();
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
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());
	while (!bAcceptFriendSuccess)
	{
		Waiting("Accepting Friend...");
	}

	Lobbies[1]->SendSetPresenceStatus(Availability::Availabe, "ready to play");
	while (!bUserPresenceSuccess)
	{
		Waiting("Changing User Status...");
	}
	bUserPresenceSuccess = false;

	Lobbies[0]->SendSetPresenceStatus(expectedUserStatus, "expected activity");
	while (!bUserPresenceSuccess)
	{
		Waiting("Changing User Status...");
	}

	while (!bUserPresenceNotifSuccess)
	{
		Waiting("Waiting for Changing User Presence...");
	}

	Lobbies[1]->Unfriend(UserCreds[0].GetUserId());
	while (!bUnfriendSuccess)
	{
		Waiting("Waiting Unfriend...");
	}

	LobbyDisconnect(2);

	FString FExpectedUserPresence = FString::FromInt((int)expectedUserStatus);

	check(bUserPresenceSuccess);
	check(bUserPresenceNotifSuccess);
	check(!bUserPresenceError);
	check(!bUserPresenceNotifError);
	check(FExpectedUserPresence.Compare(userPresenceNotifResponse.Availability) == 0);

	resetResponses();
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
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());
	while (!bAcceptFriendSuccess)
	{
		Waiting("Accepting Friend...");
	}

	Lobbies[1]->SendSetPresenceStatus(Availability::Availabe, "ready to play again");
	while (!bUserPresenceSuccess)
	{
		Waiting("Changing User Status...");
	}

	bUserPresenceSuccess = false;
	Lobbies[0]->SendSetPresenceStatus(Availability::Availabe, "ready to play too");
	while (!bUserPresenceSuccess)
	{
		Waiting("Changing User Status...");
	}

	while (!bUserPresenceNotifSuccess)
	{
		Waiting("Waiting for Changing User Presence...");
	}

	bUserPresenceSuccess = false;
	bUserPresenceNotifSuccess = false;
	Lobbies[0]->SendSetPresenceStatus(expectedUserStatus, "busy, can't play");
	while (!bUserPresenceSuccess)
	{
		Waiting("Changing User Status...");
	}

	while (!bUserPresenceNotifSuccess)
	{
		Waiting("Waiting for Changing User Presence...");
	}

	Lobbies[1]->Unfriend(UserCreds[0].GetUserId());
	while (!bUnfriendSuccess)
	{
		Waiting("Waiting Unfriend...");
	}

	LobbyDisconnect(2);

	FString FExpectedUserPresence = FString::FromInt((int)expectedUserStatus);

	check(bUserPresenceSuccess);
	check(bUserPresenceNotifSuccess);
	check(!bUserPresenceError);
	check(!bUserPresenceNotifError);
	check(FExpectedUserPresence.Compare(userPresenceNotifResponse.Availability) == 0);

	resetResponses();
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
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}
	check(!bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Outgoing);

	Lobbies[0]->ListOutgoingFriends();
	while (!bListOutgoingFriendSuccess)
	{
		Waiting("Getting List Outgoing Friend...");
	}
	check(!bListOutgoingFriendError);
	check(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Incoming);

	Lobbies[1]->ListIncomingFriends();
	while (!bListIncomingFriendSuccess)
	{
		Waiting("Getting List Incoming Friend...");
	}
	check(!bListIncomingFriendError);
	check(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());
	while (!bAcceptFriendSuccess)
	{
		Waiting("Accepting Friend Request...");
	}
	check(!bAcceptFriendError);

	Lobbies[0]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	check(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[1]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	check(loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[0]->Unfriend(UserCreds[1].GetUserId());
	while (!bUnfriendSuccess)
	{
		Waiting("Waiting Unfriend...");
	}
	check(!bUnfriendError);

	LobbyDisconnect(2);
	resetResponses();
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
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}
	check(!bRequestFriendError);

	while (!bOnIncomingRequestNotifSuccess)
	{
		Waiting("Waiting for Incoming Friend Request...");
	}
	check(!bOnIncomingRequestNotifError);
	check(requestFriendNotifResponse.friendId == UserCreds[0].GetUserId());

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());
	while (!bAcceptFriendSuccess)
	{
		Waiting("Accepting Friend Request...");
	}
	check(!bAcceptFriendError);

	while (!bOnRequestAcceptedNotifSuccess)
	{
		Waiting("Waiting for Accepted Friend Request...");
	}
	check(!bOnRequestAcceptedNotifError);
	check(acceptFriendNotifResponse.friendId == UserCreds[1].GetUserId());

	Lobbies[0]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	check(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[1]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	check(loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[0]->Unfriend(UserCreds[1].GetUserId());
	while (!bUnfriendSuccess)
	{
		Waiting("Waiting Unfriend...");
	}
	check(!bUnfriendError);

	LobbyDisconnect(2);
	resetResponses();
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
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}
	check(!bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Outgoing);

	Lobbies[0]->ListOutgoingFriends();
	while (!bListOutgoingFriendSuccess)
	{
		Waiting("Getting List Outgoing Friend...");
	}
	check(!bListOutgoingFriendError);
	check(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Incoming);

	Lobbies[1]->ListIncomingFriends();
	while (!bListIncomingFriendSuccess)
	{
		Waiting("Getting List Incoming Friend...");
	}
	check(!bListIncomingFriendError);
	check(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());
	while (!bAcceptFriendSuccess)
	{
		Waiting("Accepting Friend Request...");
	}
	check(!bAcceptFriendError);

	Lobbies[0]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	check(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[1]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	check(loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[0]->Unfriend(UserCreds[1].GetUserId());
	while (!bUnfriendSuccess)
	{
		Waiting("Waiting Unfriend...");
	}
	check(!bUnfriendError);

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[0]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		check(!loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	}

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[1]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		check(!loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));
	}

	LobbyDisconnect(2);
	resetResponses();
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
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}
	check(!bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Outgoing);

	Lobbies[0]->ListOutgoingFriends();
	while (!bListOutgoingFriendSuccess)
	{
		Waiting("Getting List Outgoing Friend...");
	}
	check(!bListOutgoingFriendError);
	check(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Incoming);

	Lobbies[1]->ListIncomingFriends();
	while (!bListIncomingFriendSuccess)
	{
		Waiting("Getting List Incoming Friend...");
	}
	check(!bListIncomingFriendError);
	check(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->RejectFriend(UserCreds[0].GetUserId());
	while (!bRejectFriendSuccess)
	{
		Waiting("Rejecting Friend Request...");
	}
	check(!bRejectFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::NotFriend);

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	Lobbies[1]->ListIncomingFriends();
	while (!bListIncomingFriendSuccess)
	{
		Waiting("Getting List Incoming Friend...");
	}
	check(!bListIncomingFriendError);
	check(!listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::NotFriend);

	bListOutgoingFriendSuccess = false;
	bListOutgoingFriendError = false;
	Lobbies[0]->ListOutgoingFriends();
	while (!bListOutgoingFriendSuccess)
	{
		Waiting("Getting List Outgoing Friend...");
	}
	check(!bListOutgoingFriendError);
	check(!listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	LobbyDisconnect(2);
	resetResponses();
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
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}
	check(!bRequestFriendError);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Outgoing);

	Lobbies[0]->ListOutgoingFriends();
	while (!bListOutgoingFriendSuccess)
	{
		Waiting("Getting List Outgoing Friend...");
	}
	check(!bListOutgoingFriendError);
	check(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Incoming);

	Lobbies[1]->ListIncomingFriends();
	while (!bListIncomingFriendSuccess)
	{
		Waiting("Getting List Incoming Friend...");
	}
	check(!bListIncomingFriendError);
	check(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[0]->CancelFriendRequest(UserCreds[1].GetUserId());
	while (!bCancelFriendSuccess)
	{
		Waiting("Cancelling Friend Request...");
	}
	check(!bCancelFriendError);

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	Lobbies[1]->ListIncomingFriends();
	while (!bListIncomingFriendSuccess)
	{
		Waiting("Getting List Incoming Friend...");
	}
	check(!bListIncomingFriendError);
	check(!listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		check(!loadFriendListResponse.friendsId.Contains(UserCreds[0].GetUserId()));
	}

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[0]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		check(!loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	}

	LobbyDisconnect(2);
	resetResponses();
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
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::NotFriend);

	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}
	check(!bRequestFriendError);

	Lobbies[0]->ListOutgoingFriends();
	while (!bListOutgoingFriendSuccess)
	{
		Waiting("Getting List Outgoing Friend...");
	}
	check(!bListOutgoingFriendError);
	check(listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Outgoing);

	Lobbies[0]->CancelFriendRequest(UserCreds[1].GetUserId());
	while (!bCancelFriendSuccess)
	{
		Waiting("Cancelling Friend Request...");
	}
	check(!bCancelFriendError);

	bListOutgoingFriendSuccess = false;
	bListOutgoingFriendError = false;
	Lobbies[0]->ListOutgoingFriends();
	while (!bListOutgoingFriendSuccess)
	{
		Waiting("Getting List Outgoing Friend...");
	}
	check(!bListOutgoingFriendError);
	if (listOutgoingFriendResponse.friendsId.Num() != 0)
	{
		check(!listOutgoingFriendResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	}

	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}
	check(!bRequestFriendError);

	Lobbies[1]->ListIncomingFriends();
	while (!bListIncomingFriendSuccess)
	{
		Waiting("Getting List Incoming Friend...");
	}
	check(!bListIncomingFriendError);
	check(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Incoming);

	Lobbies[1]->RejectFriend(UserCreds[0].GetUserId());
	while (!bRejectFriendSuccess)
	{
		Waiting("Rejecting Friend Request...");
	}
	check(!bRejectFriendError);

	bRequestFriendSuccess = false;
	bRequestFriendError = false;
	Lobbies[0]->RequestFriend(UserCreds[1].GetUserId());
	while (!bRequestFriendSuccess)
	{
		Waiting("Requesting Friend...");
	}
	check(!bRequestFriendError);

	while (!bOnIncomingRequestNotifSuccess)
	{
		Waiting("Waiting for Incoming Friend Request...");
	}
	check(!bOnIncomingRequestNotifError);
	check(requestFriendNotifResponse.friendId == UserCreds[0].GetUserId());

	bListIncomingFriendSuccess = false;
	bListIncomingFriendError = false;
	Lobbies[1]->ListIncomingFriends();
	while (!bListIncomingFriendSuccess)
	{
		Waiting("Getting List Incoming Friend...");
	}
	check(!bListIncomingFriendError);
	check(listIncomingFriendResponse.friendsId.Contains(UserCreds[0].GetUserId()));

	Lobbies[1]->AcceptFriend(UserCreds[0].GetUserId());
	while (!bAcceptFriendSuccess)
	{
		Waiting("Accepting Friend Request...");
	}
	check(!bAcceptFriendError);

	while (!bOnRequestAcceptedNotifSuccess)
	{
		Waiting("Waiting for Accepted Friend Request...");
	}
	check(!bOnRequestAcceptedNotifError);
	check(acceptFriendNotifResponse.friendId == UserCreds[1].GetUserId());

	Lobbies[0]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	check(loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[0]->GetFriendshipStatus(UserCreds[1].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Friend);

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::Friend);

	Lobbies[0]->Unfriend(UserCreds[1].GetUserId());
	while (!bUnfriendSuccess)
	{
		Waiting("Waiting Unfriend...");
	}
	check(!bUnfriendError);

	bLoadFriendListSuccess = false;
	bLoadFriendListError = false;
	Lobbies[0]->LoadFriendsList();
	while (!bLoadFriendListSuccess)
	{
		Waiting("Loading Friend List...");
	}
	check(!bLoadFriendListError);
	if (loadFriendListResponse.friendsId.Num() != 0)
	{
		check(!loadFriendListResponse.friendsId.Contains(UserCreds[1].GetUserId()));
	}

	bGetFriendshipStatusSuccess = false;
	bGetFriendshipStatusError = false;
	Lobbies[1]->GetFriendshipStatus(UserCreds[0].GetUserId());
	while (!bGetFriendshipStatusSuccess)
	{
		Waiting("Getting Friendship Status...");
	}
	check(!bGetFriendshipStatusError);
	check(getFriendshipStatusResponse.friendshipStatus == ERelationshipStatusCode::NotFriend);

	LobbyDisconnect(2);
	resetResponses();
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
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}
	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();
		while (!bLeavePartySuccess)
		{
			Waiting("Leaving Party...");
		}
	}
	Lobbies[0]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}
	check(!bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}
	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();
		while (!bLeavePartySuccess)
		{
			Waiting("Leaving Party...");
		}
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}
	check(!bCreatePartyError);

	Lobbies[0]->SendStartMatchmaking("test");
	while (!bStartMatchmakingSuccess)
	{
		Waiting("Starting Matchmaking...");
	}
	check(!bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking("test");
	while (!bStartMatchmakingSuccess)
	{
		Waiting("Starting Matchmaking...");
	}
	check(!bStartMatchmakingError);

	while (matchMakingNotifNum < 2)
	{
		Waiting("Waiting for Matchmaking Notification...");
	}
	
	FAccelByteModelsReadyConsentNotice readyConsentNoticeResponse[2];
	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);
	while (!bReadyConsentNotifSuccess)
	{
		Waiting("Waiting for Ready Consent Notification...");
	}
	check(!bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[1]->SendReadyConsentRequest(matchmakingNotifResponse[1].MatchId);
	while (!bReadyConsentNotifSuccess)
	{
		Waiting("Waiting for Ready Consent Notification...");
	}
	check(!bReadyConsentNotifError);
	readyConsentNoticeResponse[1] = readyConsentNotice;

	while (!bDsNotifSuccess)
	{
		Waiting("Waiting for DS Notification...");
	}
	check(!bDsNotifError);

	check(!bMatchmakingNotifError[0]);
	check(!bMatchmakingNotifError[1]);
	check(!matchmakingNotifResponse[0].MatchId.IsEmpty());
	check(!matchmakingNotifResponse[1].MatchId.IsEmpty());
	check(matchmakingNotifResponse[0].Status == EAccelByteMatchmakingStatus::Done);
	check(matchmakingNotifResponse[1].Status == EAccelByteMatchmakingStatus::Done);
	check(readyConsentNoticeResponse[0].MatchId == matchmakingNotifResponse[0].MatchId);
	check(readyConsentNoticeResponse[1].MatchId == matchmakingNotifResponse[1].MatchId);

	LobbyDisconnect(2);
	resetResponses();
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
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}
	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();
		while (!bLeavePartySuccess)
		{
			Waiting("Leaving Party...");
		}
	}
	Lobbies[0]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}
	check(!bCreatePartyError);

	Lobbies[0]->SendStartMatchmaking("test");
	while (!bStartMatchmakingSuccess)
	{
		Waiting("Starting Matchmaking...");
	}
	check(!bStartMatchmakingError);

	Lobbies[0]->SendCancelMatchmaking("test");
	while (!bCancelMatchmakingSuccess)
	{
		Waiting("Cancelling Matchmaking...");
	}
	check(!bCancelMatchmakingError);

	while (!bMatchmakingNotifSuccess)
	{
		Waiting("Waiting or Matchmaking Notification...");
	}
	check(!bMatchmakingNotifError);
	check(matchmakingNotifResponse.Status == EAccelByteMatchmakingStatus::Cancel);

	LobbyDisconnect(1);
	resetResponses();
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

	Lobbies[0]->SendInfoPartyRequest();
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}
	if (!bGetInfoPartyError)
	{
		Lobbies[0]->SendLeavePartyRequest();
		while (!bLeavePartySuccess)
		{
			Waiting("Leaving Party...");
		}
	}
	Lobbies[0]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}
	check(!bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[1]->SendInfoPartyRequest();
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}
	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[1]->SendLeavePartyRequest();
		while (!bLeavePartySuccess)
		{
			Waiting("Leaving Party...");
		}
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[1]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}
	check(!bCreatePartyError);

	bGetInfoPartySuccess = false;
	bGetInfoPartyError = false;
	Lobbies[2]->SendInfoPartyRequest();
	while (!bGetInfoPartySuccess)
	{
		Waiting("Getting Info Party...");
	}
	if (!bGetInfoPartyError)
	{
		bLeavePartySuccess = false;
		bLeavePartyError = false;
		Lobbies[2]->SendLeavePartyRequest();
		while (!bLeavePartySuccess)
		{
			Waiting("Leaving Party...");
		}
	}
	bCreatePartySuccess = false;
	bCreatePartyError = false;
	Lobbies[2]->SendCreatePartyRequest();
	while (!bCreatePartySuccess)
	{
		Waiting("Creating Party...");
	}
	check(!bCreatePartyError);

	Lobbies[0]->SendStartMatchmaking("test");
	while (!bStartMatchmakingSuccess)
	{
		Waiting("Lobby 0 Starting Matchmaking...");
	}
	check(!bStartMatchmakingError);

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[1]->SendStartMatchmaking("test");
	while (!bStartMatchmakingSuccess)
	{
		Waiting("Lobby 1 Starting Matchmaking...");
	}
	check(!bStartMatchmakingError);

	while (matchMakingNotifNum < 2)
	{
		Waiting("Waiting for Matchmaking Notification...");
	}

	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);
	while (!bReadyConsentNotifSuccess)
	{
		Waiting("Waiting for Ready Consent Notification...");
	}
	check(!bReadyConsentNotifError);
	readyConsentNoticeResponse[0] = readyConsentNotice;

	while (rematchmakingNotifNum < 2)
	{
		Waiting("Waiting for Rematchmaking Notification...");
	}
	check(rematchmakingNotifNum == 2);

	matchMakingNotifNum = 0;

	bStartMatchmakingSuccess = false;
	bStartMatchmakingError = false;
	Lobbies[2]->SendStartMatchmaking("test");
	while (!bStartMatchmakingSuccess)
	{
		Waiting("Lobby 2 Starting Matchmaking...");
	}
	check(!bStartMatchmakingError);

	while (matchMakingNotifNum < 2)
	{
		Waiting("Waiting for Matchmaking Notification...");
	}
	check(matchmakingNotifResponse[0].Status == EAccelByteMatchmakingStatus::Done);
	check(matchmakingNotifResponse[2].Status == EAccelByteMatchmakingStatus::Done);

	Lobbies[0]->SendReadyConsentRequest(matchmakingNotifResponse[0].MatchId);
	while (!bReadyConsentNotifSuccess)
	{
		Waiting("Waiting for Ready Consent Notification...");
	}
	readyConsentNoticeResponse[0] = readyConsentNotice;

	bReadyConsentNotifSuccess = false;
	bReadyConsentNotifError = false;
	Lobbies[2]->SendReadyConsentRequest(matchmakingNotifResponse[2].MatchId);
	while (!bReadyConsentNotifSuccess)
	{
		Waiting("Waiting for Ready Consent Notification...");
	}
	readyConsentNoticeResponse[2] = readyConsentNotice;

	while (!bDsNotifSuccess)
	{
		Waiting("Waiting for DS Notification...");
	}
	check(!bDsNotifError);

	check(!bMatchmakingNotifError[0]);
	check(!bMatchmakingNotifError[2]);
	check(!matchmakingNotifResponse[0].MatchId.IsEmpty());
	check(!matchmakingNotifResponse[2].MatchId.IsEmpty());

	LobbyDisconnect(3);
	resetResponses();
	return true;
}