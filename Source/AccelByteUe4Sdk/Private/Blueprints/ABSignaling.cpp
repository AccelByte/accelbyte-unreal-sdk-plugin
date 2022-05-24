// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABSignaling.h"

void UABSignaling::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}


FString UABSignaling::SendSignalingMessage(FSendSignalingMessageRequest const& Request) 
{
	return ApiClientPtr->Lobby.SendSignalingMessage(Request.UserID, Request.Message);
}

void UABSignaling::SetSignalingP2PNotif(FDSignalingP2PNotif OnNotif) 
{
	ApiClientPtr->Lobby.SetSignalingP2PDelegate(
	Api::Lobby::FSignalingP2P::CreateLambda(
		[OnNotif](const FString& UserID, const FString& Message)
		{
			OnNotif.ExecuteIfBound(UserID, Message);
		}));
}
