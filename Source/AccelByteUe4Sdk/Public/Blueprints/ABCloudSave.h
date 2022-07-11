// Copyright (c) 2021 AccelByte, Inc. All rights reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AccelByteMultiRegistry.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteCloudSaveModels.h"
#include "Dom/JsonObject.h"
#include "JsonObjectWrapper.h"
#include "ABCloudSave.generated.h"

using namespace AccelByte;

#pragma region MODEL_AND_DELEGATE_FOR_REQUEST_RESPONSE 

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserRecord, FAccelByteModelsUserRecord, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsGameRecord, FAccelByteModelsGameRecord, Response);

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(FJsonObjectWrapper, FDPayloadJsonObject, FJsonObjectWrapper, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABCloudSave : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void SaveUserRecord(
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		bool IsPublic,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void GetUserRecord(
		FString const& Key,
		FDModelsUserRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void GetPublicUserRecord(
		FString const& Key,
		FString const& UserId,
		FDModelsUserRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void ReplaceUserRecord(
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		bool IsPublic,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void ReplaceUserRecordCheckLatest(
		FString const& Key,
		FDateTime const LastUpdated,
		FJsonObjectWrapper RecordRequest,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void ReplaceUserRecordCheckLatestRetry(
		int TryAttempt,
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		FDPayloadJsonObject const& PayloadModifier,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void DeleteUserRecord(
		FString const& Key,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void SaveGameRecord(
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void GetGameRecord(
		FString const& Key,
		FDModelsGameRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void ReplaceGameRecord(
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void ReplaceGameRecordCheckLatest(
		FString const& Key,
		FDateTime const LastUpdated,
		FJsonObjectWrapper RecordRequest,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void ReplaceGameRecordCheckLatestRetry(
		int TryAttempt,
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		FDPayloadJsonObject const& PayloadModifier,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave | Api")
	void DeleteGameRecord(
		FString const& Key,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

private:
	FApiClientPtr ApiClientPtr;
};
