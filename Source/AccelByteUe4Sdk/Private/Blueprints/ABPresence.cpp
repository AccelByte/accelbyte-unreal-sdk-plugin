// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABPresence.h"

void UABPresence::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABPresence::GetAllFriendsStatus(FDGetAllFriendsStatusResponse OnResponse, FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetGetOnlineFriendsPresenceResponseDelegate(
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

	ApiClientPtr->Lobby.SendGetOnlineFriendPresenceRequest();
}

void UABPresence::BulkGetUserPresence(FBulkGetUserPresenceRequest const& Request, FDBulkGetUserPresence OnResponse, FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.BulkGetUserPresence(
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

void UABPresence::SetPresenceStatus(FPresenceStatus const& Request, FDOnSetUserPresence OnResponse, FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetUserPresenceResponseDelegate(
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

	ApiClientPtr->Lobby.SendSetPresenceStatus(Request.Availability, Request.Activity);
}


void UABPresence::SetOnFriendStatusNotif(FDFriendStatusNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetUserPresenceNotifDelegate(
	Api::Lobby::FFriendStatusNotif::CreateLambda(
		[OnNotif](FAccelByteModelsUsersPresenceNotice const& Response)
		{
			OnNotif.ExecuteIfBound(Response);
		}));
}
