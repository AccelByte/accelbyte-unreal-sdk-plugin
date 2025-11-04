// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABCloudSave.h"

using namespace AccelByte;

void UABCloudSave::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

void UABCloudSave::SaveUserRecord(
	FString const& Key,
	FJsonObjectWrapper const& RecordRequest,
	bool IsPublic,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->SaveUserRecord(
			Key,
			*RecordRequestCopy.JsonObject,
			IsPublic,
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

void UABCloudSave::GetUserRecord(
	FString const& Key,
	FDModelsUserRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->GetUserRecord(
		Key,
		THandler<FAccelByteModelsUserRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserRecord const& Response)
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

void UABCloudSave::GetPublicUserRecord(
	FString const& Key,
	FString const& UserId,
	FDModelsUserRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->GetPublicUserRecord(
		Key,
		UserId,
		THandler<FAccelByteModelsUserRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserRecord const& Response)
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

void UABCloudSave::ReplaceUserRecord(
	FString const& Key,
	FJsonObjectWrapper const& RecordRequest,
	bool IsPublic,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->ReplaceUserRecord(
		Key,
		*RecordRequestCopy.JsonObject,
		IsPublic,
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

void UABCloudSave::ReplaceUserRecordCheckLatest(
	FString const& Key,
	FDateTime const& LastUpdated,
	FJsonObjectWrapper const& RecordRequest,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->ReplaceUserRecordCheckLatest(
		Key,
		LastUpdated,
		RecordRequestCopy,
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

void UABCloudSave::ReplaceUserRecordCheckLatestRetry(
	int TryAttempt,
	FString const& Key,
	FJsonObjectWrapper const& RecordRequest,
	FDPayloadJsonObject const& PayloadModifier,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->ReplaceUserRecordCheckLatest(
		TryAttempt,
		Key,
		RecordRequestCopy,
		THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper>::CreateLambda(
			[PayloadModifier](FJsonObjectWrapper const& Response)
			{
				return PayloadModifier.Execute(Response);
			}
		),
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

void UABCloudSave::ReplaceUserRecordCheckLatestWithResponse(
	FString const& Key,
	FDateTime const& LastUpdated,
	FJsonObjectWrapper const& RecordRequest,
	FDModelsReplaceUserRecordResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->ReplaceUserRecordCheckLatest(
		Key,
		LastUpdated,
		RecordRequestCopy,
		THandler<FAccelByteModelsReplaceUserRecordResponse>::CreateLambda(
		[OnSuccess](FAccelByteModelsReplaceUserRecordResponse const& Response)
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

void UABCloudSave::ReplaceUserRecordCheckLatestRetryWithResponse(
	int TryAttempt,
	FString const& Key,
	FJsonObjectWrapper const& RecordRequest,
	FDPayloadJsonObject const& PayloadModifier,
	FDModelsReplaceUserRecordResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->ReplaceUserRecordCheckLatest(
		TryAttempt,
		Key,
		RecordRequestCopy,
		THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper>::CreateLambda(
			[PayloadModifier](FJsonObjectWrapper const& Response)
			{
				return PayloadModifier.Execute(Response);
			}
		),
		THandler<FAccelByteModelsReplaceUserRecordResponse>::CreateLambda(
		[OnSuccess](FAccelByteModelsReplaceUserRecordResponse const& Response)
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

void UABCloudSave::DeleteUserRecord(
	FString const& Key,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->DeleteUserRecord(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABCloudSave::SaveGameRecord(
	FString const& Key,
	FJsonObjectWrapper const& RecordRequest,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->SaveGameRecord(
		Key,
		*RecordRequestCopy.JsonObject,
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

void UABCloudSave::GetGameRecord(
	FString const& Key,
	FDModelsGameRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->GetGameRecord(
		Key,
		THandler<FAccelByteModelsGameRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsGameRecord const& Response)
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

void UABCloudSave::ReplaceGameRecord(
	FString const& Key,
	FJsonObjectWrapper const& RecordRequest,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->ReplaceGameRecord(
		Key,
		*RecordRequestCopy.JsonObject,
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

void UABCloudSave::ReplaceGameRecordCheckLatest(
	FString const& Key,
	FDateTime const& LastUpdated,
	FJsonObjectWrapper const& RecordRequest,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->ReplaceGameRecordCheckLatest(
		Key,
		LastUpdated,
		RecordRequestCopy,
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

void UABCloudSave::ReplaceGameRecordCheckLatestRetry(
	int TryAttempt,
	FString const& Key,
	FJsonObjectWrapper const& RecordRequest,
	FDPayloadJsonObject const& PayloadModifier,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	FJsonObjectWrapper RecordRequestCopy = RecordRequest;
	RecordRequestCopy.JsonObjectFromString(RecordRequestCopy.JsonString);
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->ReplaceGameRecordCheckLatest(
		TryAttempt,
		Key,
		RecordRequestCopy,
		THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper>::CreateLambda(
			[PayloadModifier](FJsonObjectWrapper const& Response)
			{ 
				return PayloadModifier.Execute(Response);
			}
		),
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

void UABCloudSave::DeleteGameRecord(
	FString const& Key,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->DeleteGameRecord(
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
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABCloudSave::BulkGetUserRecords(TArray<FString> const& Keys,
	FDModelsListUserRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->BulkGetUserRecords(Keys,
		THandler<FListAccelByteModelsUserRecord>::CreateLambda(
			[OnSuccess](FListAccelByteModelsUserRecord const& Response)
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

void UABCloudSave::BulkGetGameRecords(TArray<FString> const& Keys,
	FDModelsListGameRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->BulkGetGameRecords(Keys,
		THandler<FAccelByteModelsListGameRecords>::CreateLambda(
			[OnSuccess](FAccelByteModelsListGameRecords const& Response)
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

void UABCloudSave::BulkGetOtherPlayerPublicRecordKeys(FString const& UserId
	, FDModelsPaginatedBulkGetPublicUserRecordKeysResponse const& OnSuccess
	, FDErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->BulkGetOtherPlayerPublicRecordKeys(UserId,
		THandler<FAccelByteModelsPaginatedBulkGetPublicUserRecordKeysResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedBulkGetPublicUserRecordKeysResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		), Offset, Limit
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABCloudSave::BulkGetCurrentPlayerPublicRecordKeys(FDModelsPaginatedBulkGetPublicUserRecordKeysResponse const& OnSuccess
	, FDErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->BulkGetCurrentPlayerPublicRecordKeys(
		THandler<FAccelByteModelsPaginatedBulkGetPublicUserRecordKeysResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedBulkGetPublicUserRecordKeysResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}
		),
		FErrorHandler::CreateLambda(
			[OnError](int32 Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}
		), Offset, Limit
		);
	}else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABCloudSave::BulkGetOtherPlayerPublicRecords(FString const& UserId
	, TArray<FString> const& Keys
	, FDModelsListUserRecords const& OnSuccess
	, FDErrorHandler const& OnError)
{
	const auto CloudSavePtr = ApiClientPtr->GetCloudSaveApi().Pin();
	if (CloudSavePtr.IsValid())
	{
		CloudSavePtr->BulkGetOtherPlayerPublicRecords(UserId,
		Keys,
		THandler<FListAccelByteModelsUserRecord>::CreateLambda(
			[OnSuccess](FListAccelByteModelsUserRecord const& Response)
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
