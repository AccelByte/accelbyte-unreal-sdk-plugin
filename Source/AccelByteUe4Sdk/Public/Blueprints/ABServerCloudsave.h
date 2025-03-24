// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteServerApiClient.h"
#include "Core/AccelByteError.h"
#include "ABInventory.h"
#include "Blueprints/ABCloudSave.h"
#include "Models/AccelByteInventoryModels.h"
#include "ABServerCloudsave.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedRecordsKey, FAccelByteModelsPaginatedRecordsKey, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserRecords, TArray<FAccelByteModelsUserRecord>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsAdminGetUserRecords, FAccelByteModelsAdminGetUserRecords, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsBulkReplaceUserRecordResponses, TArray<FAccelByteModelsBulkReplaceUserRecordResponse>, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedBulkGetPlayerRecordSizeResponse, FAccelByteModelsPaginatedBulkGetPlayerRecordSizeResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsAdminGameRecord, FAccelByteModelsAdminGameRecord, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsAdminUserRecord, FAccelByteModelsAdminUserRecord, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedGetAdminUserRecordKeysResponse, FAccelByteModelsPaginatedGetAdminUserRecordKeysResponse, Response);

#pragma endregion

UCLASS(Blueprintable, BlueprintType)
class UABServerCloudsave : public UObject
{
	GENERATED_BODY()

public:
	void SetServerApiClient(AccelByte::FServerApiClientPtr const& NewServerApiClientPtr);

#pragma region V1 Cloudsave

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void RetrieveGameRecordsKey(FDModelsPaginatedRecordsKey const& OnSuccess
		, FDErrorHandler const& OnError
		, FString const& Query = TEXT("")
		, int32 Limit = 20
		, int32 Offset = 0);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void GetGameRecord(FString const& Key
		, FDModelsGameRecord const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void DeleteGameRecord(FString const& Key
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void DeleteGameRecordTTLConfig(FString const& Key
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void GetUserRecord(FString const& Key
		, FString const& UserId
		, FDModelsUserRecord const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void BulkGetUserRecord(FString const& Key
		, TArray<FString> const& UserIds
		, FDModelsUserRecords const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void BulkGetUserRecordsByKeys(FString const& UserId
		, TArray<FString> const& Keys
		, FDModelsAdminGetUserRecords const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void GetPublicUserRecord(FString const& Key
		, FString const& UserId
		, FDModelsUserRecord const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void BulkReplaceUserRecord(FString const& Key
		, FAccelByteModelsBulkReplaceUserRecordRequest const& Request
		, FDModelsBulkReplaceUserRecordResponses const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void DeleteUserRecord(FString const& Key
		, FString const& UserId
		, bool bIsPublic
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void BulkGetPlayerRecordSize(FAccelByteModelsBulkGetPlayerRecordSizeRequest const& GetPlayerRecordSizeRequest
		, FDModelsPaginatedBulkGetPlayerRecordSizeResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void QueryAdminGameRecordsByKey(FString const& Key
		, FDModelsAdminGameRecord const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void QueryAdminGameRecordKeys(int Limit
		, int Offset
		, FDModelsPaginatedRecordsKey const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void DeleteAdminGameRecord(FString const& Key
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void DeleteAdminGameRecordTTLConfig(FString const& Key
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void QueryAdminUserRecordsByKey(FString const& Key
		, FString const& UserId
		, FDModelsAdminUserRecord const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void QueryAdminUserRecordKeys(FString const& UserId
		, int Limit
		, int Offset
		, FDModelsPaginatedGetAdminUserRecordKeysResponse const& OnSuccess
		, FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Cloudsave | Api")
	void DeleteAdminUserRecord(FString const& Key
		, FString const& UserId
		, FDHandler const& OnSuccess
		, FDErrorHandler const& OnError);

#pragma endregion

private:
	AccelByte::FServerApiClientPtr ApiClientPtr;
};
