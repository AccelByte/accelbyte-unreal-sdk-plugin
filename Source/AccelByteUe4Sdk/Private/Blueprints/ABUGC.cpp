// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
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
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->CreateContent(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
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
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->CreateContent(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::ModifyContentByUpdateStruct(
	FString const& ChannelId,
	FString const& ContentId,
	FAccelByteModelsUGCUpdateRequest const& UGCRequest,
	FDModelsUGCResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->ModifyContent(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::ModifyContentByStruct(
	FString const& ChannelId,
	FString const& ContentId,
	FAccelByteModelsUGCRequest const& UGCRequest,
	FDModelsUGCResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->ModifyContent(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
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
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->ModifyContent(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::DeleteContent(
	FString const& ChannelId,
	FString const& ContentId,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->DeleteContent(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetContentByContentId(
	FString const& ContentId,
	FDModelsUGCContentResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetContentByContentId(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicGetContentByContentId(FString const& ContentId, FDModelsUGCContentResponse const& OnSuccess, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicGetContentByContentId(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetContentByShareCode(
	FString const& ShareCode,
	FDModelsUGCContentResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetContentByShareCode(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicGetContentByShareCode(FString const& ShareCode, FDModelsUGCContentResponse const& OnSuccess, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicGetContentByShareCode(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetContentPreviewAsStruct(
	FString const& ContentId,
	FDModelsUGCPreviewResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetContentPreview(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetContentPreviewAsIntArray(
	FString const& ContentId,
	FDArrayUint8 const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetContentPreview(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetContentBulk(TArray<FString> const& ContentIds, FDModelsUGCBulkContentResponse const& OnSuccess, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetContentBulk(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicGetContentBulk(TArray<FString> const& ContentIds, FDModelsUGCBulkContentResponse const& OnSuccess, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicGetContentBulk(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetUserContent(FString const& UserId, FDModelsUGCContentPagingResponse const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetUserContent(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicGetUserContent(FString const& UserId, FDModelsUGCContentPagingResponse const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicGetUserContent(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetTags(
	FDModelsUGCTagsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetTags(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetTypes(
	FDModelsUGCTypesPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetTypes(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::CreateChannel(
	FString const& ChannelName,
	FDModelsUGCChannelResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->CreateChannel(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetChannels(
	FDModelsUGCChannelsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset,
	FString const& ChannelName
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetChannels(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetChannelsByUserId(const FString& UserId,
	FDModelsUGCChannelsPagingResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	int32 Limit, 
	int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetChannels(UserId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::DeleteChannel(
	FString const& ChannelId,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->DeleteChannel(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
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
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->SearchContents(Name, Creator, Type, Subtype, Tags, IsOfficial, UserId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::SearchAllContents(FAccelByteModelsUGCSearchContentsRequest const& Request, 
	FDModelsUGCSearchContentsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit, 
	int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->SearchContents(Request,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicSearchContents(const FString& Name, const FString& Creator, const FString& Type, const FString& Subtype, const TArray<FString>& Tags, bool IsOfficial, const FString& UserId, FDModelsUGCSearchContentsPagingResponse const& OnSuccess, FDErrorHandler const& OnError, EAccelByteUgcSortBy SortBy, EAccelByteUgcOrderBy OrderBy, int32 Limit, int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicSearchContents(Name, Creator, Type, Subtype, Tags, IsOfficial, UserId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicSearchAllContents(FAccelByteModelsUGCSearchContentsRequest const& Request, FDModelsUGCSearchContentsPagingResponse const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicSearchContents(Request,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::UpdateLikeStatusToContent(const FString& ContentId,
	bool bLikeStatus,
	FDModelsUGCUpdateLikeStatusToContentResponse const& OnSuccess,
	FDErrorHandler const& OnError
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->UpdateLikeStatusToContent(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetListFollowers(const FString& UserId,
	FDModelsUGCGetListFollowersPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetListFollowers(UserId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::UpdateFollowStatusToUser(const FString& UserId,
	bool bFollowStatus, 
	FDModelsUGCUpdateFollowStatusToUserResponse const& OnSuccess,
	FDErrorHandler const& OnError 
)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->UpdateFollowStatusToUser(UserId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
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
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->SearchContentsSpecificToChannel(ChannelId, Name, Creator, Type, Subtype, Tags, IsOfficial, UserId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::SearchContentsByChannelId(FString const& ChannelId,
	FAccelByteModelsUGCSearchContentsRequest const& Request,
	FDModelsUGCSearchContentsPagingResponse const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 Limit,
	int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->SearchContentsSpecificToChannel(ChannelId, Request,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::SearchContentsSpecificToChannelV2(const FString& ChannelId, 
	FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess,
	FDErrorHandler const& OnError, 
	int32 Limit, 
	int32 Offset,
	EAccelByteUGCContentSortByV2 SortBy)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->SearchContentsSpecificToChannelV2(ChannelId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicSearchContentsSpecificToChannelV2(const FString& ChannelId, FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset, EAccelByteUGCContentSortByV2 SortBy)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicSearchContentsSpecificToChannelV2(ChannelId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::SearchContentsV2(const FAccelByteModelsUGCFilterRequestV2& Filter,
	FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess,
	FDErrorHandler const& OnError, 
	int32 Limit, 
	int32 Offset, 
	EAccelByteUGCContentSortByV2 SortBy)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->SearchContentsV2(Filter,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicSearchContentsV2(const FAccelByteModelsUGCFilterRequestV2& Filter, FDModelsUGCSearchContentsPagingResponseV2 const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset, EAccelByteUGCContentSortByV2 SortBy)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicSearchContentsV2(Filter,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetContentBulkByIdsV2(const TArray<FString>& ContentIds, 
	FDModelsUGCBulkContentResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetContentBulkByIdsV2(ContentIds,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicGetContentBulkByIdsV2(const TArray<FString>& ContentIds, FDModelsUGCBulkContentResponseV2 const& OnSuccess, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicGetContentBulkByIdsV2(ContentIds,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetContentByShareCodeV2(FString const& ShareCode, 
	FDModelsUGCContentResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetContentByShareCodeV2(ShareCode,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicGetContentByShareCodeV2(FString const& ShareCode, FDModelsUGCContentResponseV2 const& OnSuccess, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicGetContentByShareCodeV2(ShareCode,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetContentByContentIdV2(FString const& ContentId, 
	FDModelsUGCContentResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetContentByContentIdV2(ContentId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicGetContentByContentIdV2(FString const& ContentId, FDModelsUGCContentResponseV2 const& OnSuccess, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicGetContentByContentIdV2(ContentId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::CreateContentV2(FString const& ChannelId, 
	FAccelByteModelsCreateUGCRequestV2 const& CreateRequest, 
	FDModelsUGCCreateUGCResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->CreateContentV2(ChannelId, CreateRequest,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::DeleteContentV2(FString const& ChannelId, 
	FString const& ContentId, 
	FDHandler const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->DeleteContentV2(ChannelId, ContentId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::ModifyContentV2(FString const& ChannelId, 
	FString const& ContentId, 
	FAccelByteModelsModifyUGCRequestV2 const& ModifyRequest, 
	FDModelsUGCModifyUGCResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->ModifyContentV2(ChannelId, ContentId, ModifyRequest,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GenerateUploadContentURLV2(FString const& ChannelId, 
	FString const& ContentId, 
	FAccelByteModelsUploadContentURLRequestV2 const& UploadRequest, 
	FDModelsUGCUploadContentResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GenerateUploadContentURLV2(ChannelId, ContentId, UploadRequest,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::UpdateContentFileLocationV2(FString const& ChannelId, 
	FString const& ContentId, 
	FString const& FileExtension,
	FString const& S3Key, 
	FDModelsUGCUpdateContentFileLocation const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->UpdateContentFileLocationV2(ChannelId, ContentId, FileExtension, S3Key,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetUserContentsV2(const FString& UserId, 
	FDModelsUGCContentPageResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError, 
	int32 Limit, 
	int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetUserContentsV2(UserId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::PublicGetUserContentsV2(const FString& UserId, FDModelsUGCContentPageResponseV2 const& OnSuccess, FDErrorHandler const& OnError, int32 Limit, int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->PublicGetUserContentsV2(UserId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::UpdateContentScreenshotV2(const FString& ContentId, 
	FAccelByteModelsUGCUpdateScreenshotsV2 ScreenshotsRequest, 
	FDModelsUGCUpdateScreenshotsV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->UpdateContentScreenshotV2(ContentId, ScreenshotsRequest,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::UploadContentScreenshotV2(const FString& ContentId, 
	FAccelByteModelsUGCUploadScreenshotsRequestV2 ScreenshotsRequest, 
	FDModelsUGCUpdateContentScreenshotResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->UploadContentScreenshotV2(ContentId, ScreenshotsRequest,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::DeleteContentScreenshotV2(const FString& ContentId, 
	const FString& ScreenshotId, 
	FDHandler const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->DeleteContentScreenshotV2(ContentId, ScreenshotId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::AddDownloadContentCountV2(FString const& ContentId, 
	FDModelsUGCAddDownloadContentCountResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->AddDownloadContentCountV2(ContentId, 
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetListContentDownloaderV2(FString const& ContentId, 
	FDModelsUGCGetPaginatedContentDownloaderResponse const& OnSuccess, 
	FDErrorHandler const& OnError, 
	FString const& UserId,
	int32 Limit, 
	int32 Offset, 
	EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetListContentDownloaderV2(ContentId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetListContentLikerV2(FString const& ContentId
	, FDModelsUGCGetPaginatedContentLikerResponse const& OnSuccess
	, FDErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetListContentLikerV2(ContentId,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::UpdateLikeStatusToContentV2(const FString& ContentId
	, bool bLikeStatus
	, FDModelsUGCUpdateLikeStatusToContentResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->UpdateLikeStatusToContentV2(ContentId, bLikeStatus,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::ModifyContentByShareCode(FString const& ChannelId
	, FString const& ShareCode
	, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
	, FDModelsUGCResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->ModifyContentByShareCode(ChannelId, ShareCode, ModifyRequest,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::DeleteContentByShareCode(FString const& ChannelId,
	FString const& ShareCode,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->DeleteContentByShareCode(ChannelId, ShareCode,
		FVoidHandler::CreateLambda(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::BulkGetContentByShareCode(const TArray<FString>& ShareCodes, 
	FDModelsUGCBulkContentResponse const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->BulkGetContentByShareCode(ShareCodes,
		THandler<TArray<FAccelByteModelsUGCContentResponse>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsUGCContentResponse> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
			FErrorHandler::CreateLambda([OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			})
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::BulkGetContentByShareCodeV2(const TArray<FString>& ShareCodes, 
	FDModelsUGCBulkContentResponseV2 const& OnSuccess, 
	FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->BulkGetContentByShareCodeV2(ShareCodes,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::ModifyContentShareCode(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCModifyContentShareCodeRequest const& ModifyContentShareCodeRequest
	, FDModelsUGCResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->ModifyContentShareCode(ChannelId, ContentId, ModifyContentShareCodeRequest,
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::ModifyContentShareCodeV2(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCModifyContentShareCodeRequest const& ModifyContentShareCodeRequest
	, FDModelsUGCCreateUGCResponseV2 const& OnSuccess
	, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->ModifyContentShareCodeV2(ChannelId, ContentId, ModifyContentShareCodeRequest,
		THandler<FAccelByteModelsUGCCreateUGCResponseV2>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCCreateUGCResponseV2 const& Response)
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetStagingContents(EStagingContentRequestStatus Status
	, FDModelsUGCPaginatedListStagingContent const& OnSuccess
	, FDErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStagingContentUtilitiesSortBy SortBy)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetStagingContents(Status,
		THandler<FAccelByteModelsUGCPaginatedListStagingContentResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCPaginatedListStagingContentResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		), Limit, Offset, SortBy);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::GetStagingContentById(FString const& ContentId
	, FDModelsUGCStagingContent const& OnSuccess
	, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->GetStagingContentById(ContentId,
		THandler<FAccelByteModelsUGCStagingContentResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCStagingContentResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		));
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::UpdateStagingContent(FString const& ContentId
	, FAccelByteModelsUGCUpdateContentFileLocationRequestV2 const& UpdateRequest
	, FDModelsUGCStagingContent const& OnSuccess
	, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->UpdateStagingContent(ContentId, UpdateRequest,
		THandler<FAccelByteModelsUGCStagingContentResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCStagingContentResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		));
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUGC::DeleteStagingContent(FString const& ContentId
	, FDHandler const& OnSuccess
	, FDErrorHandler const& OnError)
{
	const auto UGCPtr = ApiClientPtr->GetUGCApi().Pin();
	if (UGCPtr.IsValid())
	{
		UGCPtr->DeleteStagingContent(ContentId,
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
		));
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}


