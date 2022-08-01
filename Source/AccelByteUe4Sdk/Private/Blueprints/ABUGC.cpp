// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABUGC.h"

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
	int32 Offset
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