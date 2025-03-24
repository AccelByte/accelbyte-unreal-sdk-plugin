// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABFriends.h"
#include "Api/AccelByteLobbyApi.h"


using namespace AccelByte;

void UABFriends::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABFriends::RequestFriend(
	FString UserId,
	FDRequestFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetRequestFriendsResponseDelegate(
			Api::Lobby::FRequestFriendsResponse::CreateLambda(
				[OnResponse](FAccelByteModelsRequestFriendsResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->RequestFriend(UserId);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABFriends::Unfriend(
	FString UserId,
	FDUnfriendResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetUnfriendResponseDelegate(
			Api::Lobby::FUnfriendResponse::CreateLambda(
				[OnResponse](FAccelByteModelsUnfriendResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->Unfriend(UserId);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABFriends::ListOutgoingFriends(
	FDListOutgoingFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetListOutgoingFriendsResponseDelegate(
			Api::Lobby::FListOutgoingFriendsResponse::CreateLambda(
				[OnResponse](FAccelByteModelsListOutgoingFriendsResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->ListOutgoingFriends();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABFriends::CancelFriendRequest(
	FString UserId,
	FDCancelFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetCancelFriendsResponseDelegate(
			Api::Lobby::FCancelFriendsResponse::CreateLambda(
				[OnResponse](FAccelByteModelsCancelFriendsResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->CancelFriendRequest(UserId);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABFriends::ListIncomingFriends(
	FDListIncomingFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetListIncomingFriendsResponseDelegate(
			Api::Lobby::FListIncomingFriendsResponse::CreateLambda(
				[OnResponse](FAccelByteModelsListIncomingFriendsResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->ListIncomingFriends();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABFriends::AcceptFriend(
	FString UserId,
	FDAcceptFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetAcceptFriendsResponseDelegate(
			Api::Lobby::FAcceptFriendsResponse::CreateLambda(
				[OnResponse](FAccelByteModelsAcceptFriendsResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->AcceptFriend(UserId);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABFriends::RejectFriend(
	FString UserId,
	FDRejectFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetRejectFriendsResponseDelegate(
			Api::Lobby::FRejectFriendsResponse::CreateLambda(
				[OnResponse](FAccelByteModelsRejectFriendsResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->RejectFriend(UserId);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABFriends::LoadFriendsList(
	FDLoadFriendListResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetLoadFriendListResponseDelegate(
			Api::Lobby::FLoadFriendListResponse::CreateLambda(
				[OnResponse](FAccelByteModelsLoadFriendListResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->LoadFriendsList();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABFriends::GetFriendshipStatus(
	FString UserId,
	FDGetFriendshipStatusResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetGetFriendshipStatusResponseDelegate(
			Api::Lobby::FGetFriendshipStatusResponse::CreateLambda(
				[OnResponse](FAccelByteModelsGetFriendshipStatusResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
		LobbyPtr->GetFriendshipStatus(UserId);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABFriends::BulkFriendRequest(
	FAccelByteModelsBulkFriendsRequest UserIds,
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->BulkFriendRequest(
			UserIds,
			FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
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

void UABFriends::SetOnFriendRequestAcceptedNotifDelegate(FDAcceptFriendsNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetOnFriendRequestAcceptedNotifDelegate(
			Api::Lobby::FAcceptFriendsNotif::CreateLambda(
				[OnNotif](FAccelByteModelsAcceptFriendsNotif const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABFriends::SetOnIncomingRequestFriendsNotifDelegate(FDRequestFriendsNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetOnIncomingRequestFriendsNotifDelegate(
			Api::Lobby::FRequestFriendsNotif::CreateLambda(
				[OnNotif](FAccelByteModelsRequestFriendsNotif const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABFriends::SetOnUnfriendNotifDelegate(FDUnfriendNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetOnUnfriendNotifDelegate(
			Api::Lobby::FUnfriendNotif::CreateLambda(
				[OnNotif](FAccelByteModelsUnfriendNotif const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABFriends::SetOnCancelFriendsNotifDelegate(FDCancelFriendsNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetOnCancelFriendsNotifDelegate(
			Api::Lobby::FCancelFriendsNotif::CreateLambda(
				[OnNotif](FAccelByteModelsCancelFriendsNotif const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}

void UABFriends::SetOnRejectFriendsNotifDelegate(FDRejectFriendsNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetOnRejectFriendsNotifDelegate(
			Api::Lobby::FRejectFriendsNotif::CreateLambda(
				[OnNotif](FAccelByteModelsRejectFriendsNotif const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
}