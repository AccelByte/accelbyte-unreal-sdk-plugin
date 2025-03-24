// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABBlock.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteBlock, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteBlock);

using namespace AccelByte;

void UABBlock::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABBlock::BlockPlayer(
	FBlockPlayerRequest const& Request,
	FDBlockPlayerResponse OnResponse,
	FDErrorHandler OnError)
{
	auto Lobby = ApiClientPtr->GetLobbyApi().Pin();
	if (Lobby.IsValid())
	{
		Lobby->SetBlockPlayerResponseDelegate(
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
		Lobby->BlockPlayer(Request.UserID);
	}
	else
	{
		UE_LOG(LogAccelByteBlock, Warning, TEXT("Invalid Lobby API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Lobby API already destroyed!"));
	}
}

void UABBlock::UnblockPlayer(
	FUnblockPlayerRequest const& Request,
	FDUnblockPlayerResponse OnResponse,
	FDErrorHandler OnError)
{
	auto Lobby = ApiClientPtr->GetLobbyApi().Pin();
	if (Lobby.IsValid())
	{
		Lobby->SetUnblockPlayerResponseDelegate(
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
		Lobby->UnblockPlayer(Request.UserID);
	}
	else
	{
		UE_LOG(LogAccelByteBlock, Warning, TEXT("Invalid Lobby API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Lobby API already destroyed!"));
	}
}

void UABBlock::GetListOfBlockedUsers(
	FDListOfBlockedUserResponse OnResponse,
	FDErrorHandler OnError)
{
	auto Lobby = ApiClientPtr->GetLobbyApi().Pin();
	if (Lobby.IsValid())
	{
		Lobby->GetListOfBlockedUsers(
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
	else
	{
		UE_LOG(LogAccelByteBlock, Warning, TEXT("Invalid Lobby API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Lobby API already destroyed!"));
	}
}

void UABBlock::GetListOfBlockers(FDListOfBlockersResponse OnResponse, FDErrorHandler OnError)
{
	auto Lobby = ApiClientPtr->GetLobbyApi().Pin();
	if (Lobby.IsValid())
	{
		Lobby->GetListOfBlockers(
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
	else
	{
		UE_LOG(LogAccelByteBlock, Warning, TEXT("Invalid Lobby API from API Client"));
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Lobby API already destroyed!"));
	}
}

void UABBlock::SetBlockPlayerNotifDelegate(FDBlockPlayerNotif OnNotif)
{
	auto Lobby = ApiClientPtr->GetLobbyApi().Pin();
	if (Lobby.IsValid())
	{
		Lobby->SetBlockPlayerNotifDelegate(
			Api::Lobby::FBlockPlayerNotif::CreateLambda(
				[OnNotif](FAccelByteModelsBlockPlayerNotif const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
	else
	{
		UE_LOG(LogAccelByteBlock, Warning, TEXT("Invalid Lobby API from API Client"));
	}
}

void UABBlock::SetUnblockPlayerNotifDelegate(FDUnblockPlayerNotif OnNotif)
{
	auto Lobby = ApiClientPtr->GetLobbyApi().Pin();
	if (Lobby.IsValid())
	{
		Lobby->SetUnblockPlayerNotifDelegate(
			Api::Lobby::FUnblockPlayerNotif::CreateLambda(
				[OnNotif](FAccelByteModelsUnblockPlayerNotif const& Notif)
				{
					OnNotif.ExecuteIfBound(Notif);
				}));
	}
	else
	{
		UE_LOG(LogAccelByteBlock, Warning, TEXT("Invalid Lobby API from API Client"));
	}
}
