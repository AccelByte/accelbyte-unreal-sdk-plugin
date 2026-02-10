// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteBinaryCloudSaveModels.h"
#include "Dom/JsonObject.h"
#include "JsonObjectWrapper.h"
#include "ABBinaryCloudSave.generated.h"

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE 

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsBinaryInfo, FAccelByteModelsBinaryInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserBinaryRecord, FAccelByteModelsUserBinaryRecord, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsGameBinaryRecord, FAccelByteModelsGameBinaryRecord, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsListUserBinaryRecords, FAccelByteModelsListUserBinaryRecords, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsListGameBinaryRecords, FAccelByteModelsListGameBinaryRecords, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedUserBinaryRecords, FAccelByteModelsPaginatedUserBinaryRecords, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsPaginatedGameBinaryRecords, FAccelByteModelsPaginatedGameBinaryRecords, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABBinaryCloudSave : public UObject
{
	GENERATED_BODY()

public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void SaveUserBinaryRecord(
		FString const& Key,
		bool bIsPublic,
		FString const& FileType,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void GetCurrentUserBinaryRecord(
		FString const& Key,
		FDModelsUserBinaryRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void GetPublicUserBinaryRecord(
		FString const& Key,
		FString const& UserId,
		FDModelsUserBinaryRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void BulkGetCurrentUserBinaryRecords(
		TArray<FString> const& Keys,
		FDModelsListUserBinaryRecords const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void BulkGetPublicUserBinaryRecordsByKeys(
		TArray<FString> const& Keys,
		FString const& UserId,
		FDModelsListUserBinaryRecords const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void BulkGetPublicUserBinaryRecordsByUserIds(
		FString const& Key,
		TArray<FString> const& UserIds,
		FDModelsListUserBinaryRecords const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void BulkQueryCurrentUserBinaryRecords(
		FString const& Query,
		FDModelsPaginatedUserBinaryRecords const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 const& Offset = 0,
		int32 const& Limit = 20);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void BulkQueryPublicUserBinaryRecords(
		FString const& UserId,
		FDModelsPaginatedUserBinaryRecords const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 const& Offset = 0,
		int32 const& Limit = 20);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void UpdateUserBinaryRecordFile(
		FString const& Key,
		FString const& FileType,
		FString const& FileLocation,
		FDModelsUserBinaryRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void UpdateUserBinaryRecordMetadata(
		FString const& Key,
		bool bIsPublic,
		FDModelsUserBinaryRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void DeleteUserBinaryRecord(
		FString const& Key,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void RequestUserBinaryRecordPresignedUrl(
		FString const& Key,
		FString const& FileType,
		FDModelsBinaryInfo const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void GetGameBinaryRecord(
		FString const& Key,
		FDModelsGameBinaryRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void BulkGetGameBinaryRecords(
		TArray<FString> const& Keys,
		FDModelsListGameBinaryRecords const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Binary | Api")
	void BulkQueryGameBinaryRecords(
		FString const& Query,
		FDModelsPaginatedGameBinaryRecords const& OnSuccess,
		FDErrorHandler const& OnError,
		int32 const& Offset = 0,
		int32 const& Limit = 20);

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};
