// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABBlock.h"

void UABBlock::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABBlock::BlockPlayer(
	FBlockPlayerRequest const& Request,
	FDBlockPlayerResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetBlockPlayerResponseDelegate(
	Api::Lobby::FBlockPlayerResponse::CreateLambda(
		[OnResponse](FAccelByteModelsBlockPlayerResponse const& Response)
		{
			OnResponse.ExecuteIfBound(Response);
		}),
	FErrorHandler::CreateLambda(
		[OnError](int32 Code, FString const& Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));

	ApiClientPtr->Lobby.BlockPlayer(Request.UserID);
}

void UABBlock::UnblockPlayer(
	FUnblockPlayerRequest const& Request,
	FDUnblockPlayerResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetUnblockPlayerResponseDelegate(
	Api::Lobby::FUnblockPlayerResponse::CreateLambda(
		[OnResponse](FAccelByteModelsUnblockPlayerResponse const& Response)
		{
			OnResponse.ExecuteIfBound(Response);
		}),
	FErrorHandler::CreateLambda(
		[OnError](int32 Code, FString const& Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));

	ApiClientPtr->Lobby.UnblockPlayer(Request.UserID);
}

void UABBlock::GetListOfBlockedUsers(
	FDListOfBlockedUserResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.GetListOfBlockedUsers(
		THandler<FAccelByteModelsListBlockedUserResponse>::CreateLambda(
			[OnResponse](FAccelByteModelsListBlockedUserResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABBlock::GetListOfBlockers(FDListOfBlockersResponse OnResponse, FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.GetListOfBlockers(
		THandler<FAccelByteModelsListBlockerResponse>::CreateLambda(
			[OnResponse](FAccelByteModelsListBlockerResponse const& Response)
			{
				OnResponse.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABBlock::SetBlockPlayerNotifDelegate(FDBlockPlayerNotif OnNotif)
{
	ApiClientPtr->Lobby.SetBlockPlayerNotifDelegate(
	Api::Lobby::FBlockPlayerNotif::CreateLambda(
		[OnNotif](FAccelByteModelsBlockPlayerNotif const& Notif)
		{
			OnNotif.ExecuteIfBound(Notif);
		}));
}

void UABBlock::SetUnblockPlayerNotifDelegate(FDUnblockPlayerNotif OnNotif)
{
	ApiClientPtr->Lobby.SetUnblockPlayerNotifDelegate(
	Api::Lobby::FUnblockPlayerNotif::CreateLambda(
		[OnNotif](FAccelByteModelsUnblockPlayerNotif const& Notif)
		{
			OnNotif.ExecuteIfBound(Notif);
		}));
}
