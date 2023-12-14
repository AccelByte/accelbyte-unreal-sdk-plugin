// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerUGC.h"

using namespace AccelByte;

void UABServerUGC::SetServerApiClient(FServerApiClientPtr const& NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerUGC::SearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request,
	FDModelsUGCSearchContentsResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset)
{
	ApiClientPtr->ServerUGC.SearchContents(Request,
		THandler<FAccelByteModelsUGCSearchContentsPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCSearchContentsPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		Limit, Offset
	);
}

void UABServerUGC::SearchContentsSpecificToChannel(FString const& ChannelId,
	FAccelByteModelsUGCSearchContentsRequest const& Request,
	FDModelsUGCSearchContentsResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset)
{
	ApiClientPtr->ServerUGC.SearchContentsSpecificToChannel(ChannelId, Request,
		THandler<FAccelByteModelsUGCSearchContentsPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCSearchContentsPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		Limit, Offset
	);
}

void UABServerUGC::ModifyContentByShareCode(FString const& UserId,
	FString const& ChannelId,
	FString const& ShareCode,
	FAccelByteModelsUGCUpdateRequest const& ModifyRequest,
	FDModelsUGCServerResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerUGC.ModifyContentByShareCode(UserId, ChannelId, ShareCode, ModifyRequest,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABServerUGC::DeleteContentByShareCode(FString const& UserId,
	FString const& ChannelId,
	FString const& ShareCode,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerUGC.DeleteContentByShareCode(UserId, ChannelId, ShareCode,FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}
