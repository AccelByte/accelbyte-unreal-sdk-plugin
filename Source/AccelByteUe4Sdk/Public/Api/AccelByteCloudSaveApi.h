// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteCloudSaveModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief CloudSave API for storing records.
 */
class ACCELBYTEUE4SDK_API CloudSave
{
public:
	CloudSave(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~CloudSave();
	
	/**
	 * @brief Save a user-level record with Metadata input value
	 * 
	 * @param Key Key of record. 
	 * @param SetPublic Metadata is_public value and set by client 
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void SaveUserRecord(const FString& Key, bool bSetPublic, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
	
	/**
	 * @brief Save a user-level record. If the record doesn't exist, it will create and save the record, if already exists, it will append to the existing one.
	 * @brief The end point of this method if bIsPublic is true (using suffix /public) will be deprecated in future   
	 *
	 * @param Key Key of record.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param IsPublic Save the record as a public/private record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void SaveUserRecord(FString const& Key, FJsonObject RecordRequest, bool IsPublic, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get a record (arbitrary JSON data) by its key in user-level.
	 *
	 * @param Key Key of record.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserRecord&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetUserRecord(FString const& Key, THandler<FAccelByteModelsUserRecord> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get a public record (arbitrary JSON data) by its key and userId in user-level.
	 *
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserRecord&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetPublicUserRecord(FString const& Key, FString const& UserId, THandler<FAccelByteModelsUserRecord> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get a public record (arbitrary JSON data) by its key and userId in user-level.
	 *
	 * @param Key Key of record.
	 * @param UserIds List UserId(s) of the record owner.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserRecord&.
	 * @param OnError This will be called when the operation failed.
	 */
	void BulkGetPublicUserRecord(FString const& Key, const TArray<FString>& UserIds, THandler<FListAccelByteModelsUserRecord> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Replace a record in user-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
	 * @param Key Key of record. 
	 * @param SetPublic Metadata is_public value and set by client 
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void ReplaceUserRecord(const FString& Key, bool bSetPublic, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief Replace a record in user-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
	 * @brief The end point of this method if bIsPublic is true (using suffix /public) will be deprecated in future   
	 * @param Key Key of record.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param IsPublic Save the record as a public/private record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void ReplaceUserRecord(FString const& Key, FJsonObject RecordRequest, bool IsPublic, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Replace a record in user-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one, but will failed if lastUpdated is not up-to-date.
	 * 
	 * @param Key Key of record.
	 * @param LastUpdated last time the record is updated. Retrieve it from GetGameRecord.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void ReplaceUserRecordCheckLatest(FString const& Key, FDateTime const LastUpdated, FJsonObjectWrapper RecordRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Replace a record in user-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one. Beware: Function will try to get the latest value, put it in the custom modifier and request to replace the record. will retry it again when the record is updated by other user, until exhaust all the attempt.
	 * 
	 * @param TryAttempt Attempt to try to replace the game record.
	 * @param Key Key of record.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param PayloadModifier Function to modify the latest record value with your customized modifier.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void ReplaceUserRecordCheckLatest(int TryAttempt, FString const& Key, FJsonObjectWrapper RecordRequest, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	 * @brief Delete a record under the given key in user-level.
	 *
	 * @param Key Key of record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void DeleteUserRecord(FString const& Key, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	 * @brief Save a namespace-level record. If the record doesn't exist, it will create and save the record, if already exists, it will append to the existing one.
	 *
	 * @param Key Key of record.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void SaveGameRecord(FString const& Key, FJsonObject RecordRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get a record by its key in namespace-level.
	 *
	 * @param Key Key of record.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGameRecord&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetGameRecord(FString const& Key, THandler<FAccelByteModelsGameRecord> const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Replace a record in namespace-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
	 *
	 * @param Key Key of record.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void ReplaceGameRecord(FString const& Key, FJsonObject RecordRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	 /**
	 * @brief Replace a record in namespace-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one, but will failed if lastUpdated is not up-to-date.
	 *
	 * @param Key Key of record.
	 * @param LastUpdated last time the record is updated. Retrieve it from GetGameRecord.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void ReplaceGameRecordCheckLatest(FString const& Key, FDateTime const LastUpdated, FJsonObjectWrapper RecordRequest, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Replace a record in namespace-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one. Beware: Function will try to get the latest value, put it in the custom modifier and request to replace the record. will retry it again when the record is updated by other user, until exhaust all the attempt.
	 *
	 * @param TryAttempt Attempt to try to replace the game record.
	 * @param Key Key of record.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param PayloadModifier Function to modify the latest record value with your customized modifier.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void ReplaceGameRecordCheckLatest(int TryAttempt, FString const& Key, FJsonObjectWrapper RecordRequest, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);
	
	/**
	 * @brief Delete a record under the given key in namespace-level.
	 *
	 * @param Key Key of record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	void DeleteGameRecord(FString const& Key, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;

	int32 const UserIdsRequestLimit = 20;

	CloudSave() = delete;
	CloudSave(CloudSave const&) = delete;
	CloudSave(CloudSave&&) = delete;

	void ReplaceUserRecord(int TryAttempt, FString const& Key, FAccelByteModelsConcurrentReplaceRequest const& Data, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);
	void ReplaceGameRecord(int TryAttempt, FString const& Key, FAccelByteModelsConcurrentReplaceRequest const& Data, THandlerPayloadModifier<FJsonObjectWrapper, FJsonObjectWrapper> const& PayloadModifier, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	FJsonObject CreateGameRecordWithMetadata(ESetByMetadataRecord SetBy, FJsonObject const& RecordRequest);
	FJsonObject CreatePlayerRecordWithMetadata(ESetByMetadataRecord SetBy, bool bSetPublic, FJsonObject const& RecordRequest);
	static FJsonObjectWrapper ConvertJsonObjToJsonObjWrapper(const TSharedPtr<FJsonObject>*& value);
};

} // Namespace Api
} // Namespace AccelByte
