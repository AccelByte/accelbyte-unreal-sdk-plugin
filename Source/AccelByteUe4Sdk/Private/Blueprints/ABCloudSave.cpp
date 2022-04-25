// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#include "Blueprints/ABCloudSave.h"

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
				FModelsUserRecord Result;
				Result.Key = Response.Key;
				Result.Namespace = Response.Namespace;
				Result.UserId = Response.UserId;
				Result.IsPublic = Response.IsPublic;
				Result.CreatedAt = Response.CreatedAt;
				Result.UpdatedAt = Response.UpdatedAt;
				Result.SetBy = Response.SetBy;
				Result.Value.JsonObject = MakeShared<FJsonObject>(Response.Value);

				OnSuccess.ExecuteIfBound(Result);
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
				FModelsUserRecord Result;
				Result.Key = Response.Key;
				Result.Namespace = Response.Namespace;
				Result.UserId = Response.UserId;
				Result.IsPublic = Response.IsPublic;
				Result.CreatedAt = Response.CreatedAt;
				Result.UpdatedAt = Response.UpdatedAt;
				Result.SetBy = Response.SetBy;
				Result.Value.JsonObject = MakeShared<FJsonObject>(Response.Value);

				OnSuccess.ExecuteIfBound(Result);
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
		*RecordRequest.JsonObject,
		THandlerPayloadModifier<FJsonObject, FJsonObject>::CreateLambda(
			[PayloadModifier](FJsonObject const& Response)
			{
				FJsonObjectWrapper Result;
				Result.JsonObject = MakeShared<FJsonObject>(Response);
				return *PayloadModifier.Execute(Result).JsonObject;
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
				FModelsGameRecord Result;
				Result.Key = Response.Key;
				Result.Namespace = Response.Namespace;
				Result.CreatedAt = Response.CreatedAt;
				Result.UpdatedAt = Response.UpdatedAt;
				Result.SetBy = Response.SetBy;
				Result.Value.JsonObject = MakeShared<FJsonObject>(Response.Value);

				OnSuccess.ExecuteIfBound(Result);
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
		*RecordRequest.JsonObject,
		THandlerPayloadModifier<FJsonObject, FJsonObject>::CreateLambda(
			[PayloadModifier](FJsonObject const& Response)
			{
				FJsonObjectWrapper Result;
				Result.JsonObject = MakeShared<FJsonObject>(Response);
				return *PayloadModifier.Execute(Result).JsonObject;
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