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

void UABServerUGC::PublicGetContentByContentId(FString const& ContentId, FDModelsUGCContentResponse const& OnSuccess, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerUGC.PublicGetContentByContentId(
		ContentId,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCContentResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABServerUGC::PublicGetContentByShareCode(FString const& ShareCode, FDModelsUGCContentResponse const& OnSuccess, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerUGC.PublicGetContentByShareCode(
		ShareCode,
		THandler<FAccelByteModelsUGCContentResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCContentResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABServerUGC::PublicGetContentBulk(TArray<FString> const& ContentIds, FDModelsUGCBulkContentResponse const& OnSuccess, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerUGC.PublicGetContentBulk(
		ContentIds,
		THandler<TArray<FAccelByteModelsUGCContentResponse>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsUGCContentResponse> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABServerUGC::PublicGetUserContent(FString const& UserId, FDModelsUGCContentPagingResponse const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset)
{
	ApiClientPtr->ServerUGC.PublicGetUserContent(
		UserId,
		THandler<FAccelByteModelsUGCContentPageResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCContentPageResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		)
	);
}

void UABServerUGC::PublicSearchContents(const FString & Name, const FString & Creator, const FString & Type, const FString & Subtype, const TArray<FString>&Tags, bool IsOfficial, const FString & UserId, FDModelsUGCSearchContentsPagingResponse const& OnSuccess, FDErrorHandler const& OnError, EAccelByteUgcSortBy SortBy, EAccelByteUgcOrderBy OrderBy, int32 Limit, int32 Offset)
{
	ApiClientPtr->ServerUGC.PublicSearchContents(Name, Creator, Type, Subtype, Tags, IsOfficial, UserId,
		THandler<FAccelByteModelsUGCSearchContentsPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCSearchContentsPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		SortBy, OrderBy, Limit, Offset
	);
}

void UABServerUGC::PublicSearchAllContents(FAccelByteModelsUGCSearchContentsRequest const& Request, FDModelsUGCSearchContentsPagingResponse const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset)
{
	ApiClientPtr->ServerUGC.PublicSearchContents(Request,
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

void UABServerUGC::PublicSearchContentsSpecificToChannelV2(const FString & ChannelId, FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset, EAccelByteUGCContentSortByV2 SortBy)
{
	ApiClientPtr->ServerUGC.PublicSearchContentsSpecificToChannelV2(ChannelId,
		THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCSearchContentsPagingResponseV2 const& Response)
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

void UABServerUGC::PublicSearchContentsV2(const FAccelByteModelsUGCFilterRequestV2 & Filter, FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset, EAccelByteUGCContentSortByV2 SortBy)
{
	ApiClientPtr->ServerUGC.PublicSearchContentsV2(Filter,
		THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCSearchContentsPagingResponseV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		Limit, Offset, SortBy
	);
}

void UABServerUGC::PublicGetContentBulkByIdsV2(const TArray<FString>&ContentIds, FDModelsUGCBulkContentResponseV2 const& OnSuccess, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerUGC.PublicGetContentBulkByIdsV2(ContentIds,
		THandler<TArray<FAccelByteModelsUGCContentResponseV2>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsUGCContentResponseV2> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABServerUGC::PublicGetContentByShareCodeV2(FString const& ShareCode, FDModelsUGCContentResponseV2 const& OnSuccess, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerUGC.PublicGetContentByShareCodeV2(ShareCode,
		THandler<FAccelByteModelsUGCContentResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCContentResponseV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABServerUGC::PublicGetContentByContentIdV2(FString const& ContentId, FDModelsUGCContentResponseV2 const& OnSuccess, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerUGC.PublicGetContentByContentIdV2(ContentId,
		THandler<FAccelByteModelsUGCContentResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCContentResponseV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABServerUGC::PublicGetUserContentsV2(const FString & UserId, FDModelsUGCContentPageResponseV2 const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset)
{
	ApiClientPtr->ServerUGC.PublicGetUserContentsV2(UserId,
		THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCSearchContentsPagingResponseV2 const& Response)
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
