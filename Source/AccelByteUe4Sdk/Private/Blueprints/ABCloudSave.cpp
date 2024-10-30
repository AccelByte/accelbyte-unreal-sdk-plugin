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
	FJsonObjectWrapper RecordRequest,
	bool IsPublic,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.SaveUserRecord(
		Key,
		*RecordRequest.JsonObject,
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
}

void UABCloudSave::GetUserRecord(
	FString const& Key,
	FDModelsUserRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->CloudSave.GetUserRecord(
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
}

void UABCloudSave::GetPublicUserRecord(
	FString const& Key,
	FString const& UserId,
	FDModelsUserRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->CloudSave.GetPublicUserRecord(
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
}

void UABCloudSave::ReplaceUserRecord(
	FString const& Key,
	FJsonObjectWrapper RecordRequest,
	bool IsPublic,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.ReplaceUserRecord(
		Key,
		*RecordRequest.JsonObject,
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
}

void UABCloudSave::ReplaceUserRecordCheckLatest(
	FString const& Key,
	FDateTime const LastUpdated,
	FJsonObjectWrapper RecordRequest,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.ReplaceUserRecordCheckLatest(
		Key,
		LastUpdated,
		RecordRequest,
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

void UABCloudSave::ReplaceUserRecordCheckLatestRetry(
	int TryAttempt,
	FString const& Key,
	FJsonObjectWrapper RecordRequest,
	FDPayloadJsonObject const& PayloadModifier,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.ReplaceUserRecordCheckLatest(
		TryAttempt,
		Key,
		RecordRequest,
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
}

void UABCloudSave::ReplaceUserRecordCheckLatestWithResponse(
	FString const& Key,
	FDateTime const LastUpdated,
	FJsonObjectWrapper RecordRequest,
	FDModelsReplaceUserRecordResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.ReplaceUserRecordCheckLatest(
		Key,
		LastUpdated,
		RecordRequest,
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
}

void UABCloudSave::ReplaceUserRecordCheckLatestRetryWithResponse(
	int TryAttempt,
	FString const& Key,
	FJsonObjectWrapper RecordRequest,
	FDPayloadJsonObject const& PayloadModifier,
	FDModelsReplaceUserRecordResponse const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.ReplaceUserRecordCheckLatest(
		TryAttempt,
		Key,
		RecordRequest,
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
}

void UABCloudSave::DeleteUserRecord(
	FString const& Key,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->CloudSave.DeleteUserRecord(
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

void UABCloudSave::SaveGameRecord(
	FString const& Key,
	FJsonObjectWrapper RecordRequest,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.SaveGameRecord(
		Key,
		*RecordRequest.JsonObject,
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

void UABCloudSave::GetGameRecord(
	FString const& Key,
	FDModelsGameRecord const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->CloudSave.GetGameRecord(
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
}

void UABCloudSave::ReplaceGameRecord(
	FString const& Key,
	FJsonObjectWrapper RecordRequest,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.ReplaceGameRecord(
		Key,
		*RecordRequest.JsonObject,
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

void UABCloudSave::ReplaceGameRecordCheckLatest(
	FString const& Key,
	FDateTime const LastUpdated,
	FJsonObjectWrapper RecordRequest,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.ReplaceGameRecordCheckLatest(
		Key,
		LastUpdated,
		RecordRequest,
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

void UABCloudSave::ReplaceGameRecordCheckLatestRetry(
	int TryAttempt,
	FString const& Key,
	FJsonObjectWrapper RecordRequest,
	FDPayloadJsonObject const& PayloadModifier,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	RecordRequest.JsonObjectFromString(RecordRequest.JsonString);
	ApiClientPtr->CloudSave.ReplaceGameRecordCheckLatest(
		TryAttempt,
		Key,
		RecordRequest,
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
}

void UABCloudSave::DeleteGameRecord(
	FString const& Key,
	FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->CloudSave.DeleteGameRecord(
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

void UABCloudSave::BulkGetUserRecords(TArray<FString> const& Keys,
	FDModelsListUserRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->CloudSave.BulkGetUserRecords(Keys,
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
}

void UABCloudSave::BulkGetGameRecords(TArray<FString> const& Keys,
	FDModelsListGameRecords const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->CloudSave.BulkGetGameRecords(Keys,
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
}

void UABCloudSave::BulkGetOtherPlayerPublicRecordKeys(FString const& UserId
	, FDModelsPaginatedBulkGetPublicUserRecordKeysResponse const& OnSuccess
	, FDErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	ApiClientPtr->CloudSave.BulkGetOtherPlayerPublicRecordKeys(UserId,
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
}

void UABCloudSave::BulkGetCurrentPlayerPublicRecordKeys(FDModelsPaginatedBulkGetPublicUserRecordKeysResponse const& OnSuccess
	, FDErrorHandler const& OnError
	, int32 const& Offset
	, int32 const& Limit)
{
	ApiClientPtr->CloudSave.BulkGetCurrentPlayerPublicRecordKeys(
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
}

void UABCloudSave::BulkGetOtherPlayerPublicRecords(FString const& UserId
	, TArray<FString> const& Keys
	, FDModelsListUserRecords const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->CloudSave.BulkGetOtherPlayerPublicRecords(UserId,
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
}
