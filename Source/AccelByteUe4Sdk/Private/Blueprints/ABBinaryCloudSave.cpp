// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
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
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->SaveUserBinaryRecord(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::GetCurrentUserBinaryRecord(
	FString const& Key,
	FDModelsUserBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->GetCurrentUserBinaryRecord(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::GetPublicUserBinaryRecord(
	FString const& Key,
	FString const& UserId,
	FDModelsUserBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->GetPublicUserBinaryRecord(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::BulkGetCurrentUserBinaryRecords(
	TArray<FString> const& Keys,
	FDModelsListUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->BulkGetCurrentUserBinaryRecords(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::BulkGetPublicUserBinaryRecordsByKeys(
	TArray<FString> const& Keys,
	FString const& UserId,
	FDModelsListUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->BulkGetPublicUserBinaryRecords(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::BulkGetPublicUserBinaryRecordsByUserIds(
	FString const& Key,
	TArray<FString> const& UserIds,
	FDModelsListUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->BulkGetPublicUserBinaryRecords(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::BulkQueryCurrentUserBinaryRecords(
	FString const& Query,
	FDModelsPaginatedUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->BulkQueryCurrentUserBinaryRecords(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::BulkQueryPublicUserBinaryRecords(
	FString const& UserId,
	FDModelsPaginatedUserBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->BulkQueryPublicUserBinaryRecords(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::UpdateUserBinaryRecordFile(
	FString const& Key,
	FString const& FileType,
	FString const& FileLocation,
	FDModelsUserBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->UpdateUserBinaryRecordFile(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::UpdateUserBinaryRecordMetadata(
	FString const& Key,
	bool bIsPublic,
	FDModelsUserBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->UpdateUserBinaryRecordMetadata(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::DeleteUserBinaryRecord(
	FString const& Key,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->DeleteUserBinaryRecord(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::RequestUserBinaryRecordPresignedUrl(
	FString const& Key,
	FString const& FileType,
	FDModelsBinaryInfo const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->RequestUserBinaryRecordPresignedUrl(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::GetGameBinaryRecord(
	FString const& Key,
	FDModelsGameBinaryRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->GetGameBinaryRecord(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::BulkGetGameBinaryRecords(
	TArray<FString> const& Keys,
	FDModelsListGameBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->BulkGetGameBinaryRecords(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABBinaryCloudSave::BulkQueryGameBinaryRecords(
	FString const& Query,
	FDModelsPaginatedGameBinaryRecords const& OnSuccess,
	FDErrorHandler const& OnError,
	int32 const& Offset,
	int32 const& Limit)
{
	const auto BinaryCloudSavePtr = ApiClientPtr->GetBinaryCloudSaveApi().Pin();
	if (BinaryCloudSavePtr.IsValid())
	{
		BinaryCloudSavePtr->BulkQueryGameBinaryRecords(
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
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}