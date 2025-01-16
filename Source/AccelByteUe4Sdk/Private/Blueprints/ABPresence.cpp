// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABPresence.h"

using namespace AccelByte;

void UABPresence::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABPresence::GetAllFriendsStatus(FDGetAllFriendsStatusResponse OnResponse, FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetGetOnlineFriendsPresenceResponseDelegate(
			Api::Lobby::FGetAllFriendsStatusResponse::CreateLambda(
				[OnResponse](FAccelByteModelsGetOnlineUsersResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));

		LobbyPtr->SendGetOnlineFriendPresenceRequest();
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABPresence::BulkGetUserPresence(FBulkGetUserPresenceRequest const& Request, FDBulkGetUserPresence OnResponse, FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->BulkGetUserPresence(
			Request.UserIds,
			THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda([OnResponse](FAccelByteModelsBulkUserStatusNotif const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}),
			Request.bCountOnly);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABPresence::BulkGetUserPresenceV2(FBulkGetUserPresenceRequest const& Request, FDBulkGetUserPresence OnResponse,
	FDErrorHandler OnError)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->BulkGetUserPresenceV2(
			Request.UserIds,
			THandler<FAccelByteModelsBulkUserStatusNotif>::CreateLambda([OnResponse](FAccelByteModelsBulkUserStatusNotif const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}),
			Request.bCountOnly);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABPresence::SetPresenceStatus(FAccelBytePresenceStatus const& Request, FDOnSetUserPresence OnResponse, FDErrorHandler OnError)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetUserPresenceResponseDelegate(
			Api::Lobby::FSetUserPresenceResponse::CreateLambda(
				[OnResponse](FAccelByteModelsSetOnlineUsersResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));

		LobbyPtr->SendSetPresenceStatus(Request.Availability, Request.Activity);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABPresence::SetOnFriendStatusNotif(FDFriendStatusNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetUserPresenceNotifDelegate(
			Api::Lobby::FFriendStatusNotif::CreateLambda(
				[OnNotif](FAccelByteModelsUsersPresenceNotice const& Response)
				{
					OnNotif.ExecuteIfBound(Response);
				}));
	}
}
