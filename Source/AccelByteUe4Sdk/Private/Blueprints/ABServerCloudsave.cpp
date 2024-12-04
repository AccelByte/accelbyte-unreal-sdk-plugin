// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/ABServerCloudsave.h"

using namespace AccelByte;

void UABServerCloudsave::SetServerApiClient(FServerApiClientPtr const& NewServerApiClientPtr)
{
	ApiClientPtr = NewServerApiClientPtr;
}

void UABServerCloudsave::RetrieveGameRecordsKey(FDModelsPaginatedRecordsKey const& OnSuccess
	, FDErrorHandler const& OnError
	, FString const& Query
	, int32 Limit
	, int32 Offset)
{
	ApiClientPtr->ServerCloudSave.RetrieveGameRecordsKey(
		THandler<FAccelByteModelsPaginatedRecordsKey>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedRecordsKey const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),
		Query, Limit, Offset);
}

void UABServerCloudsave::GetGameRecord(FString const& Key
	, FDModelsGameRecord const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.GetGameRecord(
		Key,
		THandler<FAccelByteModelsGameRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsGameRecord const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::DeleteGameRecord(FString const& Key
	, FDHandler const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.DeleteGameRecord(
		Key,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::DeleteGameRecordTTLConfig(FString const& Key, FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.DeleteGameRecordTTLConfig(
		Key,
		FVoidHandler::CreateLambda([OnSuccess]()
		{
			OnSuccess.ExecuteIfBound();
		}), FErrorHandler::CreateLambda([OnError](int Code, FString const& Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));
}

void UABServerCloudsave::GetUserRecord(FString const& Key
	, FString const& UserId
	, FDModelsUserRecord const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.GetUserRecord(
		Key,
		UserId,
		THandler<FAccelByteModelsUserRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserRecord const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::BulkGetUserRecord(FString const& Key
	, TArray<FString> const& UserIds
	, FDModelsUserRecords const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.BulkGetUserRecord(
		Key,
		UserIds,
		THandler<TArray<FAccelByteModelsUserRecord>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsUserRecord> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::BulkGetUserRecordsByKeys(FString const& UserId
	, TArray<FString> const& Keys
	, FDModelsAdminGetUserRecords const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.BulkGetUserRecordsByKeys(
		UserId,
		Keys,
		THandler<FAccelByteModelsAdminGetUserRecords>::CreateLambda(
			[OnSuccess](FAccelByteModelsAdminGetUserRecords const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::GetPublicUserRecord(FString const& Key
	, FString const& UserId
	, FDModelsUserRecord const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.GetPublicUserRecord(
		Key,
		UserId,
		THandler<FAccelByteModelsUserRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsUserRecord const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::BulkReplaceUserRecord(FString const& Key
	, FAccelByteModelsBulkReplaceUserRecordRequest const& Request
	, FDModelsBulkReplaceUserRecordResponses const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.BulkReplaceUserRecord(
		Key,
		Request,
		THandler<TArray<FAccelByteModelsBulkReplaceUserRecordResponse>>::CreateLambda(
			[OnSuccess](TArray<FAccelByteModelsBulkReplaceUserRecordResponse> const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::DeleteUserRecord(FString const& Key
	, FString const& UserId
	, bool bIsPublic
	, FDHandler const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.DeleteUserRecord(
		Key,
		UserId,
		bIsPublic,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::BulkGetPlayerRecordSize(FAccelByteModelsBulkGetPlayerRecordSizeRequest const& GetPlayerRecordSizeRequest
	, FDModelsPaginatedBulkGetPlayerRecordSizeResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.BulkGetPlayerRecordSize(
		GetPlayerRecordSizeRequest,
		THandler<FAccelByteModelsPaginatedBulkGetPlayerRecordSizeResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedBulkGetPlayerRecordSizeResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::QueryAdminGameRecordsByKey(FString const& Key
	, FDModelsAdminGameRecord const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.QueryAdminGameRecordsByKey(
		Key,
		THandler<FAccelByteModelsAdminGameRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsAdminGameRecord const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::QueryAdminGameRecordKeys(int Limit
	, int Offset
	, FDModelsPaginatedRecordsKey const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.QueryAdminGameRecordKeys(
		Limit,
		Offset,
		THandler<FAccelByteModelsPaginatedRecordsKey>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedRecordsKey const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::DeleteAdminGameRecord(FString const& Key
	, FDHandler const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.DeleteAdminGameRecord(
		Key,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::DeleteAdminGameRecordTTLConfig(FString const& Key, FDHandler const& OnSuccess,
	FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.DeleteAdminGameRecordTTLConfig(
		Key,
		FVoidHandler::CreateLambda([OnSuccess]()
		{
			OnSuccess.ExecuteIfBound();
		}), FErrorHandler::CreateLambda([OnError](int Code, FString const& Message)
		{
			OnError.ExecuteIfBound(Code, Message);
		}));
}

void UABServerCloudsave::QueryAdminUserRecordsByKey(FString const& Key
	, FString const& UserId
	, FDModelsAdminUserRecord const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.QueryAdminUserRecordsByKey(
		Key,
		UserId,
		THandler<FAccelByteModelsAdminUserRecord>::CreateLambda(
			[OnSuccess](FAccelByteModelsAdminUserRecord const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::QueryAdminUserRecordKeys(FString const& UserId
	, int Limit
	, int Offset
	, FDModelsPaginatedGetAdminUserRecordKeysResponse const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.QueryAdminUserRecordKeys(
		UserId,
		Limit,
		Offset,
		THandler<FAccelByteModelsPaginatedGetAdminUserRecordKeysResponse>::CreateLambda(
			[OnSuccess](FAccelByteModelsPaginatedGetAdminUserRecordKeysResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABServerCloudsave::DeleteAdminUserRecord(FString const& Key
	, FString const& UserId
	, FDHandler const& OnSuccess
	, FDErrorHandler const& OnError)
{
	ApiClientPtr->ServerCloudSave.DeleteAdminUserRecord(
		Key,
		UserId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}
