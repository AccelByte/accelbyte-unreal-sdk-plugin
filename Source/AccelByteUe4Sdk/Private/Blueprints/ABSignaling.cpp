// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABSignaling.h"

using namespace AccelByte;

void UABSignaling::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}


FString UABSignaling::SendSignalingMessage(FSendSignalingMessageRequest const& Request) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{

		return LobbyPtr->SendSignalingMessage(Request.UserID, Request.Message);
	}
	return TEXT("");
}

void UABSignaling::SetSignalingP2PNotif(FDSignalingP2PNotif OnNotif) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetSignalingP2PDelegate(
			Api::Lobby::FSignalingP2P::CreateLambda(
				[OnNotif](const FString& UserID, const FString& Message)
				{
					OnNotif.ExecuteIfBound(UserID, Message);
				}));
	}
}
