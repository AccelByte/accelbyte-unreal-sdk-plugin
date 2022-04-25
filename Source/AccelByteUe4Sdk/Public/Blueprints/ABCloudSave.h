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
USTRUCT(BlueprintType)
struct FModelsUserRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Key{};

	UPROPERTY(BlueprintReadWrite)
	FString Namespace{};

	UPROPERTY(BlueprintReadWrite)
	FString UserId{};

	UPROPERTY(BlueprintReadWrite)
	bool IsPublic{};

	UPROPERTY(BlueprintReadWrite)
	FDateTime CreatedAt{0};

	UPROPERTY(BlueprintReadWrite)
	FDateTime UpdatedAt{0};

	UPROPERTY(BlueprintReadWrite)
	ESetByMetadataRecord SetBy{};

	UPROPERTY(BlueprintReadWrite)
	FJsonObjectWrapper Value{};
};

USTRUCT(BlueprintType)
struct FModelsGameRecord
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString Key{};

	UPROPERTY(BlueprintReadWrite)
	FString Namespace{};

	UPROPERTY(BlueprintReadWrite)
	FDateTime CreatedAt{0};

	UPROPERTY(BlueprintReadWrite)
	FDateTime UpdatedAt{0};

	UPROPERTY(BlueprintReadWrite)
	ESetByMetadataRecord SetBy{};
	
	UPROPERTY(BlueprintReadWrite)
	FJsonObjectWrapper Value{};
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsUserRecord, FModelsUserRecord, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDModelsGameRecord, FModelsGameRecord, Response);

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(FJsonObjectWrapper, FDPayloadJsonObject, FJsonObjectWrapper, Response);

#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class UABCloudSave : public UObject
{
	GENERATED_BODY()
	
public:
	void SetApiClient(FApiClientPtr const& NewApiClientPtr);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void SaveUserRecord(
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		bool IsPublic,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void GetUserRecord(
		FString const& Key,
		FDModelsUserRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void GetPublicUserRecord(
		FString const& Key,
		FString const& UserId,
		FDModelsUserRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void ReplaceUserRecord(
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		bool IsPublic,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void ReplaceUserRecordCheckLatest(
		FString const& Key,
		FDateTime const LastUpdated,
		FJsonObjectWrapper RecordRequest,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void ReplaceUserRecordCheckLatestRetry(
		int TryAttempt,
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		FDPayloadJsonObject const& PayloadModifier,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void DeleteUserRecord(
		FString const& Key,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void SaveGameRecord(
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void GetGameRecord(
		FString const& Key,
		FDModelsGameRecord const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void ReplaceGameRecord(
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void ReplaceGameRecordCheckLatest(
		FString const& Key,
		FDateTime const LastUpdated,
		FJsonObjectWrapper RecordRequest,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void ReplaceGameRecordCheckLatestRetry(
		int TryAttempt,
		FString const& Key,
		FJsonObjectWrapper RecordRequest,
		FDPayloadJsonObject const& PayloadModifier,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | CloudSave")
	void DeleteGameRecord(
		FString const& Key,
		FDHandler const& OnSuccess,
		FDErrorHandler const& OnError);

private:
	FApiClientPtr ApiClientPtr;
};
