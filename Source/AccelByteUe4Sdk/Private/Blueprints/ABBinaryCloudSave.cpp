// Copyright (c) 2023 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABBinaryCloudSave.h"

using namespace AccelByte;

void UABBinaryCloudSave::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABBinaryCloudSave::SaveUserBinaryRecord(
	FString const& Key,
	bool bIsPublic,
	FString const& FileType,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.SaveUserBinaryRecord(
		Key,
		FileType,
		bIsPublic,
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

void UABBinaryCloudSave::GetCurrentUserBinaryRecord(
	FString const& Key,
	FDModelsUserBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.GetCurrentUserBinaryRecord(
		Key,
		THandler<FAccelByteModelsUserBinaryRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserBinaryRecord const& Response)
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

void UABBinaryCloudSave::GetPublicUserBinaryRecord(
	FString const& Key,
	FString const& UserId,
	FDModelsUserBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.GetPublicUserBinaryRecord(
		Key,
		UserId,
		THandler<FAccelByteModelsUserBinaryRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserBinaryRecord const& Response)
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

void UABBinaryCloudSave::BulkGetCurrentUserBinaryRecords(
	TArray<FString> const& Keys,
	FDModelsListUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.BulkGetCurrentUserBinaryRecords(
		Keys,
		THandler<FAccelByteModelsListUserBinaryRecords>::CreateLambda(
			[OnSuccess](FAccelByteModelsListUserBinaryRecords const& Response)
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

void UABBinaryCloudSave::BulkGetPublicUserBinaryRecordsByKeys(
	TArray<FString> const& Keys,
	FString const& UserId,
	FDModelsListUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.BulkGetPublicUserBinaryRecords(
		Keys,
		UserId,
		THandler<FAccelByteModelsListUserBinaryRecords>::CreateLambda(
			[OnSuccess](FAccelByteModelsListUserBinaryRecords const& Response)
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

void UABBinaryCloudSave::BulkGetPublicUserBinaryRecordsByUserIds(
	FString const& Key,
	TArray<FString> const& UserIds,
	FDModelsListUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.BulkGetPublicUserBinaryRecords(
		Key,
		UserIds,
		THandler<FAccelByteModelsListUserBinaryRecords>::CreateLambda(
			[OnSuccess](FAccelByteModelsListUserBinaryRecords const& Response)
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

void UABBinaryCloudSave::BulkQueryCurrentUserBinaryRecords(
	FString const& Query,
	FDModelsPaginatedUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit)
{
	ApiClientPtr->BinaryCloudSave.BulkQueryCurrentUserBinaryRecords(
		Query,
		THandler<FAccelByteModelsPaginatedUserBinaryRecords>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedUserBinaryRecords const& Response)
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
		Offset,
		Limit
	);
}

void UABBinaryCloudSave::BulkQueryPublicUserBinaryRecords(
	FString const& UserId,
	FDModelsPaginatedUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit)
{
	ApiClientPtr->BinaryCloudSave.BulkQueryPublicUserBinaryRecords(
		UserId,
		THandler<FAccelByteModelsPaginatedUserBinaryRecords>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedUserBinaryRecords const& Response)
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
		Offset,
		Limit
	);
}

void UABBinaryCloudSave::UpdateUserBinaryRecordFile(
	FString const& Key,
	FString const& FileType,
	FString const& FileLocation,
	FDModelsUserBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.UpdateUserBinaryRecordFile(
		Key,
		FileType,
		FileLocation,
		THandler<FAccelByteModelsUserBinaryRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserBinaryRecord const& Response)
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

void UABBinaryCloudSave::UpdateUserBinaryRecordMetadata(
	FString const& Key,
	bool bIsPublic,
	FDModelsUserBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.UpdateUserBinaryRecordMetadata(
		Key,
		bIsPublic,
		THandler<FAccelByteModelsUserBinaryRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserBinaryRecord const& Response)
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

void UABBinaryCloudSave::DeleteUserBinaryRecord(
	FString const& Key,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.DeleteUserBinaryRecord(
		Key,
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

void UABBinaryCloudSave::RequestUserBinaryRecordPresignedUrl(
	FString const& Key,
	FString const& FileType,
	FDModelsBinaryInfo const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.RequestUserBinaryRecordPresignedUrl(
		Key,
		FileType,
		THandler<FAccelByteModelsBinaryInfo>::CreateLambda(
			[OnSuccess](FAccelByteModelsBinaryInfo const& Response)
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

void UABBinaryCloudSave::GetGameBinaryRecord(
	FString const& Key,
	FDModelsGameBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.GetGameBinaryRecord(
		Key,
		THandler<FAccelByteModelsGameBinaryRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsGameBinaryRecord const& Response)
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

void UABBinaryCloudSave::BulkGetGameBinaryRecords(
	TArray<FString> const& Keys,
	FDModelsListGameBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->BinaryCloudSave.BulkGetGameBinaryRecords(
		Keys,
		THandler<FAccelByteModelsListGameBinaryRecords>::CreateLambda(
			[OnSuccess](FAccelByteModelsListGameBinaryRecords const& Response)
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

void UABBinaryCloudSave::BulkQueryGameBinaryRecords(
	FString const& Query,
	FDModelsPaginatedGameBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit)
{
	ApiClientPtr->BinaryCloudSave.BulkQueryGameBinaryRecords(
		Query,
		THandler<FAccelByteModelsPaginatedGameBinaryRecords>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedGameBinaryRecords const& Response)
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
		Offset,
		Limit
	);
}