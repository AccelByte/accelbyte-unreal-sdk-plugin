// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABSessionAttribute.h"

void UABSessionAttribute::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

FString UABSessionAttribute::SetSessionAttribute(
	FSetSessionAttributeRequest const& Request,
	FDSetSessionAttributeResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetSetSessionAttributeDelegate(
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

	return ApiClientPtr->Lobby.SetSessionAttribute(Request.Key, Request.Value);
}

FString UABSessionAttribute::GetSessionAttribute(
	FGetSessionAttributeRequest const& Request,
	FDGetSessionAttributeResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetGetSessionAttributeDelegate(
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

	return ApiClientPtr->Lobby.GetSessionAttribute(Request.Key);
}

FString UABSessionAttribute::GetAllSessionAttribute(
	FDGetAllSessionAttributeResponse OnResponse,
	FDErrorHandler OnError) 
{
	ApiClientPtr->Lobby.SetGetAllSessionAttributeDelegate(
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

	return ApiClientPtr->Lobby.GetAllSessionAttribute();
}
