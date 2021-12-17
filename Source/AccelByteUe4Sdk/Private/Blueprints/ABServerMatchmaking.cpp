// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerMatchmaking.h"

void UABServerMatchmaking::SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerMatchmaking::EnqueueJoinableSession(
	FAccelByteModelsMatchmakingResult const& MatchmakingResult,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerMatchmaking.EnqueueJoinableSession(
		MatchmakingResult,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerMatchmaking::DequeueJoinableSession(
	FString const& MatchId,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerMatchmaking.DequeueJoinableSession(
		MatchId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerMatchmaking::AddUserToSession(
	FString const& ChannelName,
	FString const& MatchId,
	FString const& UserId, FString const& OptionalPartyId, FDHandler OnSuccess, FDErrorHandler OnError)
{
	ApiClientPtr->ServerMatchmaking.AddUserToSession(
		ChannelName,
		MatchId,
		UserId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		OptionalPartyId);
}

void UABServerMatchmaking::RemoveUserFromSession(
	FString const& ChannelName,
	FString const& MatchId,
	FString const& UserId,
	FAccelByteModelsMatchmakingResult const& OptionalBody,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerMatchmaking.RemoveUserFromSession(
		ChannelName,
		MatchId,
		UserId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		OptionalBody);
}

void UABServerMatchmaking::QuerySessionStatus(
	FString const& MatchId,
	FDMatchmakingResultDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerMatchmaking.QuerySessionStatus(
		MatchId,
		THandler<FAccelByteModelsMatchmakingResult>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMatchmakingResult& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerMatchmaking::ActivateSessionStatusPolling(
	FString const& MatchId,
	int IntervalSec,
	FDMatchmakingResultDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerMatchmaking.ActivateSessionStatusPolling(
		MatchId,
		THandler<FAccelByteModelsMatchmakingResult>::CreateLambda(
			[OnSuccess](const FAccelByteModelsMatchmakingResult& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		IntervalSec);
}

void UABServerMatchmaking::DeactivateStatusPolling()
{
	ApiClientPtr->ServerMatchmaking.DeactivateStatusPolling();
}
