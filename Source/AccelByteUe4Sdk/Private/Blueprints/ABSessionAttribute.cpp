// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABSessionAttribute.h"

using namespace AccelByte;

void UABSessionAttribute::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

FString UABSessionAttribute::SetSessionAttribute(
	FSetSessionAttributeRequest const& Request,
	FDSetSessionAttributeResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetSetSessionAttributeDelegate(
			Api::Lobby::FSetSessionAttributeResponse::CreateLambda(
				[OnResponse](FAccelByteModelsSetSessionAttributesResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));

		return LobbyPtr->SetSessionAttribute(Request.Key, Request.Value);
	}
	return TEXT("");
}

FString UABSessionAttribute::GetSessionAttribute(
	FGetSessionAttributeRequest const& Request,
	FDGetSessionAttributeResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetGetSessionAttributeDelegate(
			Api::Lobby::FGetSessionAttributeResponse::CreateLambda(
				[OnResponse](FAccelByteModelsGetSessionAttributesResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));

		return LobbyPtr->GetSessionAttribute(Request.Key);
	}
	return TEXT("");
}

FString UABSessionAttribute::GetAllSessionAttribute(
	FDGetAllSessionAttributeResponse OnResponse,
	FDErrorHandler OnError) 
{
	const auto LobbyPtr = ApiClientPtr->GetLobbyApi().Pin();
	if (LobbyPtr.IsValid())
	{
		LobbyPtr->SetGetAllSessionAttributeDelegate(
			Api::Lobby::FGetAllSessionAttributeResponse::CreateLambda(
				[OnResponse](FAccelByteModelsGetAllSessionAttributesResponse const& Response)
				{
					OnResponse.ExecuteIfBound(Response);
				}),
			FErrorHandler::CreateLambda(
				[OnError](int32 Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));

		return LobbyPtr->GetAllSessionAttribute();
	}
	return TEXT("");
}
