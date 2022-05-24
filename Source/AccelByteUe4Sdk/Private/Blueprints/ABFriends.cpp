// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABFriends.h"
#include "Api/AccelByteLobbyApi.h"
#include "Core/AccelByteRegistry.h"

void UABFriends::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABFriends::RequestFriend(
	FString UserId,
	FDRequestFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetRequestFriendsResponseDelegate(
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
	ApiClientPtr->Lobby.RequestFriend(UserId);
}

void UABFriends::Unfriend(
	FString UserId,
	FDUnfriendResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetUnfriendResponseDelegate(
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
	ApiClientPtr->Lobby.Unfriend(UserId);
}

void UABFriends::ListOutgoingFriends(
	FDListOutgoingFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetListOutgoingFriendsResponseDelegate(
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
	ApiClientPtr->Lobby.ListOutgoingFriends();
}

void UABFriends::CancelFriendRequest(
	FString UserId,
	FDCancelFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetCancelFriendsResponseDelegate(
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
	ApiClientPtr->Lobby.CancelFriendRequest(UserId);
}

void UABFriends::ListIncomingFriends(
	FDListIncomingFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetListIncomingFriendsResponseDelegate(
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
	ApiClientPtr->Lobby.ListIncomingFriends();
}

void UABFriends::AcceptFriend(
	FString UserId,
	FDAcceptFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetAcceptFriendsResponseDelegate(
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
	ApiClientPtr->Lobby.AcceptFriend(UserId);
}

void UABFriends::RejectFriend(
	FString UserId,
	FDRejectFriendsResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetRejectFriendsResponseDelegate(
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
	ApiClientPtr->Lobby.RejectFriend(UserId);
}

void UABFriends::LoadFriendsList(
	FDLoadFriendListResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetLoadFriendListResponseDelegate(
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
	ApiClientPtr->Lobby.LoadFriendsList();
}

void UABFriends::GetFriendshipStatus(
	FString UserId,
	FDGetFriendshipStatusResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetGetFriendshipStatusResponseDelegate(
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
	ApiClientPtr->Lobby.GetFriendshipStatus(UserId);
}

void UABFriends::BulkFriendRequest(
	FAccelByteModelsBulkFriendsRequest UserIds,
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.BulkFriendRequest(
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

void UABFriends::SetOnFriendRequestAcceptedNotifDelegate(FDAcceptFriendsNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetOnFriendRequestAcceptedNotifDelegate(
		Api::Lobby::FAcceptFriendsNotif::CreateLambda(
			[OnNotif](FAccelByteModelsAcceptFriendsNotif const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABFriends::SetOnIncomingRequestFriendsNotifDelegate(FDRequestFriendsNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetOnIncomingRequestFriendsNotifDelegate(
		Api::Lobby::FRequestFriendsNotif::CreateLambda(
			[OnNotif](FAccelByteModelsRequestFriendsNotif const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABFriends::SetOnUnfriendNotifDelegate(FDUnfriendNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetOnUnfriendNotifDelegate(
		Api::Lobby::FUnfriendNotif::CreateLambda(
			[OnNotif](FAccelByteModelsUnfriendNotif const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABFriends::SetOnCancelFriendsNotifDelegate(FDCancelFriendsNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetOnCancelFriendsNotifDelegate(
		Api::Lobby::FCancelFriendsNotif::CreateLambda(
			[OnNotif](FAccelByteModelsCancelFriendsNotif const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}

void UABFriends::SetOnRejectFriendsNotifDelegate(FDRejectFriendsNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetOnRejectFriendsNotifDelegate(
		Api::Lobby::FRejectFriendsNotif::CreateLambda(
			[OnNotif](FAccelByteModelsRejectFriendsNotif const& Notif)
			{
				OnNotif.ExecuteIfBound(Notif);
			}));
}