// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerLobby.h"

void UABServerLobby::SetServerApiClient(FServerApiClientPtr NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerLobby::GetPartyDataByUserId(
	FString const& UserId,
	FDDataPartyResponseDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerLobby.GetPartyDataByUserId(
		UserId,
		THandler<FAccelByteModelsDataPartyResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsDataPartyResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerLobby::WritePartyStorage(
	FString const& PartyId,
	FDPayloadModifier PayloadModifier,
	int const& RetryAttempt,
	FDPartyDataNotifDelegate OnSuccess,
	FDErrorHandler OnError)
{
	TFunction<FJsonObjectWrapper(FJsonObjectWrapper)> Function = [PayloadModifier](FJsonObjectWrapper const& Original)
	{
		FJsonObjectWrapper Modified = Original;
		PayloadModifier.ExecuteIfBound(Modified);
		return Modified;
	};

	ApiClientPtr->ServerLobby.WritePartyStorage(
		PartyId,
		Function,
		THandler<FAccelByteModelsPartyDataNotif>::CreateLambda(
			[OnSuccess](FAccelByteModelsPartyDataNotif const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		RetryAttempt);
}


void UABServerLobby::GetPartyStorage(
	FString const& PartyId,
	FDPartyDataNotifDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerLobby.GetPartyStorage(
		PartyId,
		THandler<FAccelByteModelsPartyDataNotif>::CreateLambda(
			[OnSuccess](FAccelByteModelsPartyDataNotif const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerLobby::GetSessionAttributeAll(
	FString const& UserId,
	FDSessionAttributeAllResponseDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerLobby.GetSessionAttributeAll(
		UserId,
		THandler<FAccelByteModelsGetSessionAttributeAllResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsGetSessionAttributeAllResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerLobby::GetSessionAttribute(
	FString const& UserId,
	FString const& Key,
	FDSessionAttributeResponseDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerLobby.GetSessionAttribute(
		UserId,
		Key,
		THandler<FAccelByteModelsGetSessionAttributeResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsGetSessionAttributeResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerLobby::SetSessionAttribute(
	FString const& UserId,
	TMap<FString,
	FString> const& Attributes,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerLobby.SetSessionAttribute(
		UserId,
		Attributes,
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

void UABServerLobby::SetSessionAttributeString(
	FString const& UserId,
	FString const& Key,
	FString const& Value,
	FDHandler OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerLobby.SetSessionAttribute(
		UserId,
		Key,
		Value,
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

void UABServerLobby::GetListOfBlockers(
	FString const& UserId,
	FDListBlockerResponseDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerLobby.GetListOfBlockers(
		UserId,
		THandler<FAccelByteModelsListBlockerResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsListBlockerResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerLobby::GetListOfBlockedUsers(
	FString const& UserId,
	FDListBlockedUserResponseDelegate OnSuccess,
	FDErrorHandler OnError)
{
	ApiClientPtr->ServerLobby.GetListOfBlockedUsers(
		UserId,
		THandler<FAccelByteModelsListBlockedUserResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsListBlockedUserResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}