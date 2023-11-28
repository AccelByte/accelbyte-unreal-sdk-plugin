// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABUGC.h"

using namespace AccelByte;

void UABUGC::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABUGC::CreateContentByStruct(
	FString const& ChannelId,
	FAccelByteModelsUGCRequest const& UGCRequest,
	FDModelsUGCResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.CreateContent(
		ChannelId,
		UGCRequest,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCResponse const& Response)
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

void UABUGC::CreateContent(
	FString const& ChannelId,
	FString const& Name,
	FString const& Type,
	FString const& SubType,
	TArray<FString> const& Tags,
	TArray<uint8> const& Preview,
	FString const& FileExtension,
	FDModelsUGCResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	FString ContentType
)
{
	ApiClientPtr->UGC.CreateContent(
		ChannelId,
		Name,
		Type,
		SubType,
		Tags,
		Preview,
		FileExtension,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCResponse const& Response)
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

void UABUGC::ModifyContentByUpdateStruct(
	FString const& ChannelId,
	FString const& ContentId,
	FAccelByteModelsUGCUpdateRequest const& UGCRequest,
	FDModelsUGCResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.ModifyContent(
		ChannelId,
		ContentId,
		UGCRequest,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCResponse const& Response)
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

void UABUGC::ModifyContentByStruct(
	FString const& ChannelId,
	FString const& ContentId,
	FAccelByteModelsUGCRequest const& UGCRequest,
	FDModelsUGCResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.ModifyContent(
		ChannelId,
		ContentId,
		UGCRequest,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCResponse const& Response)
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

void UABUGC::ModifyContent(
	FString const& ChannelId,
	FString const& ContentId,
	FString const& Name,
	FString const& Type,
	FString const& SubType,
	TArray<FString> const& Tags,
	TArray<uint8> const& Preview,
	FString const& FileExtension,
	FDModelsUGCResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	FString ContentType
)
{
	ApiClientPtr->UGC.ModifyContent(
		ChannelId,
		ContentId,
		Name,
		Type,
		SubType,
		Tags,
		Preview,
		FileExtension,
		THandler<FAccelByteModelsUGCResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCResponse const& Response)
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

void UABUGC::DeleteContent(
	FString const& ChannelId,
	FString const& ContentId,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.DeleteContent(
		ChannelId,
		ContentId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
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

void UABUGC::GetContentByContentId(
	FString const& ContentId,
	FDModelsUGCContentResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.GetContentByContentId(
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

void UABUGC::GetContentByShareCode(
	FString const& ShareCode,
	FDModelsUGCContentResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.GetContentByShareCode(
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

void UABUGC::GetContentPreviewAsStruct(
	FString const& ContentId,
	FDModelsUGCPreviewResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.GetContentPreview(
		ContentId,
		THandler<FAccelByteModelsUGCPreview>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCPreview const& Response)
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

void UABUGC::GetContentPreviewAsIntArray(
	FString const& ContentId,
	FDArrayUint8 const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.GetContentPreview(
		ContentId,
		THandler<TArray<uint8>>::CreateLambda(
			[OnSuccess](TArray<uint8> const& Response)
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

void UABUGC::GetTags(
	FDModelsUGCTagsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset
)
{
	ApiClientPtr->UGC.GetTags(
		THandler<FAccelByteModelsUGCTagsPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCTagsPagingResponse const& Response)
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

void UABUGC::GetTypes(
	FDModelsUGCTypesPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset
)
{
	ApiClientPtr->UGC.GetTypes(
		THandler<FAccelByteModelsUGCTypesPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCTypesPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		Limit,
		Offset
	);
}

void UABUGC::CreateChannel(
	FString const& ChannelName,
	FDModelsUGCChannelResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.CreateChannel(
		ChannelName,
		THandler<FAccelByteModelsUGCChannelResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCChannelResponse const& Response)
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

void UABUGC::GetChannels(
	FDModelsUGCChannelsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset,
	FString const& ChannelName
)
{
	ApiClientPtr->UGC.GetChannels(
		THandler<FAccelByteModelsUGCChannelsPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCChannelsPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		), Limit, Offset, ChannelName
	);
}

void UABUGC::GetChannelsByUserId(const FString& UserId,
	FDModelsUGCChannelsPagingResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	int32 Limit, 
	int32 Offset)
{
	ApiClientPtr->UGC.GetChannels(UserId,
		THandler<FAccelByteModelsUGCChannelsPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCChannelsPagingResponse const& Response)
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

void UABUGC::DeleteChannel(
	FString const& ChannelId,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.DeleteChannel(
		ChannelId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
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

void UABUGC::SearchContents(const FString& Name,
	const FString& Creator,
	const FString& Type,
	const FString& Subtype,
	const TArray<FString>& Tags,
	bool IsOfficial,
	const FString& UserId,
	FDModelsUGCSearchContentsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	EAccelByteUgcSortBy SortBy,
	EAccelByteUgcOrderBy OrderBy,
	int32 Limit,
	int32 Offset)
{
	ApiClientPtr->UGC.SearchContents(Name, Creator, Type, Subtype, Tags, IsOfficial, UserId,
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

void UABUGC::SearchAllContents(FAccelByteModelsUGCSearchContentsRequest const& Request, 
	FDModelsUGCSearchContentsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit, 
	int32 Offset)
{
	ApiClientPtr->UGC.SearchContents(Request,
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

void UABUGC::UpdateLikeStatusToContent(const FString& ContentId,
	bool bLikeStatus,
	FDModelsUGCUpdateLikeStatusToContentResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	ApiClientPtr->UGC.UpdateLikeStatusToContent(
		ContentId,
		bLikeStatus, 
		THandler<FAccelByteModelsUGCUpdateLikeStatusToContentResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCUpdateLikeStatusToContentResponse const& Response)
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

void UABUGC::GetListFollowers(const FString& UserId,
	FDModelsUGCGetListFollowersPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset
)
{
	ApiClientPtr->UGC.GetListFollowers(UserId,
		THandler<FAccelByteModelsUGCGetListFollowersPagingResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCGetListFollowersPagingResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		),
		Limit,
		Offset
	);
}

void UABUGC::UpdateFollowStatusToUser(const FString& UserId,
	bool bFollowStatus, 
	FDModelsUGCUpdateFollowStatusToUserResponse const& OnSuccess,
	FDErrorHandler const& OnError 
)
{
	ApiClientPtr->UGC.UpdateFollowStatusToUser(UserId,
		bFollowStatus, 
		THandler<FAccelByteModelsUGCUpdateFollowStatusToUserResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCUpdateFollowStatusToUserResponse const& Response)
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

void UABUGC::SearchContentsSpecificToChannel(const FString& ChannelId,
	const FString& Name,
	const FString& Creator,
	const FString& Type,
	const FString& Subtype,
	const TArray<FString>& Tags,
	bool IsOfficial,
	const FString& UserId,
	FDModelsUGCSearchContentsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	EAccelByteUgcSortBy SortBy,
	EAccelByteUgcOrderBy OrderBy,
	int32 Limit,
	int32 Offset)
{
	ApiClientPtr->UGC.SearchContentsSpecificToChannel(ChannelId, Name, Creator, Type, Subtype, Tags, IsOfficial, UserId,
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

void UABUGC::SearchContentsByChannelId(FString const& ChannelId,
	FAccelByteModelsUGCSearchContentsRequest const& Request,
	FDModelsUGCSearchContentsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset)
{
	ApiClientPtr->UGC.SearchContentsSpecificToChannel(ChannelId, Request,
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

void UABUGC::SearchContentsSpecificToChannelV2(const FString& ChannelId, 
	FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess,
	FDErrorHandler const& OnError, 
	int32 Limit, 
	int32 Offset,
	EAccelByteUGCContentSortByV2 SortBy)
{
	ApiClientPtr->UGC.SearchContentsSpecificToChannelV2(ChannelId,
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

void UABUGC::SearchContentsV2(const FAccelByteModelsUGCFilterRequestV2& Filter,
	FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess,
	FDErrorHandler const& OnError, 
	int32 Limit, 
	int32 Offset, 
	EAccelByteUGCContentSortByV2 SortBy)
{
	ApiClientPtr->UGC.SearchContentsV2(Filter,
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

void UABUGC::GetContentBulkByIdsV2(const TArray<FString>& ContentIds, 
	FDModelsUGCBulkContentResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.GetContentBulkByIdsV2(ContentIds,
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

void UABUGC::GetContentByShareCodeV2(FString const& ShareCode, 
	FDModelsUGCContentResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.GetContentByShareCodeV2(ShareCode,
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

void UABUGC::GetContentByContentIdV2(FString const& ContentId, 
	FDModelsUGCContentResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.GetContentByContentIdV2(ContentId,
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

void UABUGC::CreateContentV2(FString const& ChannelId, 
	FAccelByteModelsCreateUGCRequestV2 const& CreateRequest, 
	FDModelsUGCCreateUGCResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.CreateContentV2(ChannelId, CreateRequest,
		THandler<FAccelByteModelsUGCCreateUGCResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCCreateUGCResponseV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABUGC::DeleteContentV2(FString const& ChannelId, 
	FString const& ContentId, 
	FDHandler const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.DeleteContentV2(ChannelId, ContentId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABUGC::ModifyContentV2(FString const& ChannelId, 
	FString const& ContentId, 
	FAccelByteModelsModifyUGCRequestV2 const& ModifyRequest, 
	FDModelsUGCModifyUGCResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.ModifyContentV2(ChannelId, ContentId, ModifyRequest,
		THandler<FAccelByteModelsUGCModifyUGCResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCModifyUGCResponseV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABUGC::GenerateUploadContentURLV2(FString const& ChannelId, 
	FString const& ContentId, 
	FAccelByteModelsUploadContentURLRequestV2 const& UploadRequest, 
	FDModelsUGCUploadContentResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.GenerateUploadContentURLV2(ChannelId, ContentId, UploadRequest,
		THandler<FAccelByteModelsUGCUploadContentURLResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCUploadContentURLResponseV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABUGC::UpdateContentFileLocationV2(FString const& ChannelId, 
	FString const& ContentId, 
	FString const& FileExtension,
	FString const& S3Key, 
	FDModelsUGCUpdateContentFileLocation const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.UpdateContentFileLocationV2(ChannelId, ContentId, FileExtension, S3Key,
		THandler<FAccelByteModelsUGCUpdateContentFileLocationResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCUpdateContentFileLocationResponseV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABUGC::GetUserContentsV2(const FString& UserId, 
	FDModelsUGCContentPageResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError, 
	int32 Limit, 
	int32 Offset)
{
	ApiClientPtr->UGC.GetUserContentsV2(UserId,
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

void UABUGC::UpdateContentScreenshotV2(const FString& ContentId, 
	FAccelByteModelsUGCUpdateScreenshotsV2 ScreenshotsRequest, 
	FDModelsUGCUpdateScreenshotsV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.UpdateContentScreenshotV2(ContentId, ScreenshotsRequest,
		THandler<FAccelByteModelsUGCUpdateScreenshotsV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCUpdateScreenshotsV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABUGC::UploadContentScreenshotV2(const FString& ContentId, 
	FAccelByteModelsUGCUploadScreenshotsRequestV2 ScreenshotsRequest, 
	FDModelsUGCUpdateContentScreenshotResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.UploadContentScreenshotV2(ContentId, ScreenshotsRequest,
		THandler<FAccelByteModelsUGCUpdateContentScreenshotResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCUpdateContentScreenshotResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABUGC::DeleteContentScreenshotV2(const FString& ContentId, 
	const FString& ScreenshotId, 
	FDHandler const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.DeleteContentScreenshotV2(ContentId, ScreenshotId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABUGC::AddDownloadContentCountV2(FString const& ContentId, 
	FDModelsUGCAddDownloadContentCountResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.AddDownloadContentCountV2(ContentId, 
		THandler<FAccelByteModelsUGCAddDownloadContentCountResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCAddDownloadContentCountResponseV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}

void UABUGC::GetListContentDownloaderV2(FString const& ContentId, 
	FDModelsUGCGetPaginatedContentDownloaderResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	FString const& UserId,
	int32 Limit, 
	int32 Offset, 
	EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	ApiClientPtr->UGC.GetListContentDownloaderV2(ContentId,
		THandler<FAccelByteModelsUGCGetPaginatedContentDownloaderResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCGetPaginatedContentDownloaderResponseV2 const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		UserId, Limit, Offset, SortBy
	);
}

void UABUGC::GetListContentLikerV2(FString const& ContentId
	, FDModelsUGCGetPaginatedContentLikerResponse const& OnSuccess
	, FDErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	ApiClientPtr->UGC.GetListContentLikerV2(ContentId,
		THandler<FAccelByteModelsUGCGetPaginatedContentLikerResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCGetPaginatedContentLikerResponseV2 const& Response)
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

void UABUGC::UpdateLikeStatusToContentV2(const FString& ContentId
	, bool bLikeStatus
	, FDModelsUGCUpdateLikeStatusToContentResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->UGC.UpdateLikeStatusToContentV2(ContentId, bLikeStatus,
		THandler<FAccelByteModelsUGCUpdateLikeStatusToContentResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCUpdateLikeStatusToContentResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
	);
}
