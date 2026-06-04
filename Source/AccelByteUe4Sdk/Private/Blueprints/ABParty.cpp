// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABParty.h"
#include "Api/AccelByteLobbyApi.h"


using namespace AccelByte;

void UABParty::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}
void UABParty::SetPartySizeLimit(const FString& PartyId, const int32 Limit, const FDHandler& OnSuccess,
	FDErrorHandler OnError)
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetPartySizeLimit(PartyId, Limit,
		FVoidHandler::CreateLambda([OnSuccess]()
		{
			OnSuccess.ExecuteIfBound();
		}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::GetPartyData(
	FGetPartyDataRequest const& Request,
	FDPartyGetDataResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->GetPartyData(Request.partyId,
		THandler<FAccelByteModelsPartyData>::CreateLambda(
			[OnResponse](FAccelByteModelsPartyData Response)
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
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::GetPartyStorage(
	FGetPartyStorageRequest const& Request,
	FDPartyGetStorageResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->GetPartyStorage(Request.partyId,
		THandler<FAccelByteModelsPartyDataNotif>::CreateLambda(
			[OnResponse](FAccelByteModelsPartyDataNotif Response)
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
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABParty::WritePartyStorage(
	FWritePartyStorageRequest const& Request,
	FDPartyWriteDataResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->WritePartyStorage(
		Request.partyId,
		Request.PayloadModifier,
		THandler<FAccelByteModelsPartyDataNotif>::CreateLambda(
			[OnResponse](FAccelByteModelsPartyDataNotif Response)
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
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}
