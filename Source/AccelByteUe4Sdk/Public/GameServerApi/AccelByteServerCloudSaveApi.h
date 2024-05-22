// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteGeneralModels.h"
#include "Models/AccelByteCloudSaveModels.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{

/**
 * @brief CloudSave API for storing records.
 */
class ACCELBYTEUE4SDK_API ServerCloudSave : public FServerApiBase
{
public:
	ServerCloudSave(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~ServerCloudSave();

	/**
	 * @brief Retrieve all available namespace-level game records key.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is in const FAccelByteModelsPaginatedRecordsKey&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset Distance between the beginning of the data list and a given point (optional, default = 0).
	 * @param Limit The limit of the GameRecord data result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RetrieveGameRecordsKey(THandler<FAccelByteModelsPaginatedRecordsKey> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& Query = TEXT("{}")
		, int32 Offset = 0
		, int32 Limit = 20);

	/**
	 * @brief Save a namespace-level game record. If the record doesn't exist, it will create and save the record, 
	 *		if already exists, it will append to the existing one.
	 *
	 * @param Key Key of record.
	 * @param SetBy Metadata value 
     * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param TTLConfig The configuration to control the action taken if the record has expired. If the action set to NONE, 
	 *		then it will not send the TTL (Time to live) meta data.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveGameRecord(FString const& Key
		, ESetByMetadataRecord SetBy
		, FJsonObject const& RecordRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FTTLConfig const& TTLConfig = FTTLConfig{});
		
	/**
	 * @brief Save a namespace-level game record. If the record doesn't exist, it will create and save the record,
	 *		if already exists, it will append to the existing one.
	 *
	 * @param Key Key of record.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveGameRecord(FString const& Key
		, FJsonObject const& RecordRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a game record by its key in namespace-level.
	 *
	 * @param Key Key of record.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGameRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetGameRecord(FString const& Key
		, THandler<FAccelByteModelsGameRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Replace a game record in namespace-level. If the record doesn't exist, it will create and save the record. 
	 *		If already exists, it will replace the existing one.
	 *
	 * @param Key Key of record.
	 * @param SetBy Metadata Value 
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param TTLConfig The configuration to control the action taken if the record has expired. If the action set to NONE,
	 *		then it will not send the TTL (Time to live) meta data.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ReplaceGameRecord(FString const& Key
		, ESetByMetadataRecord SetBy
		, FJsonObject const& RecordRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FTTLConfig const& TTLConfig = FTTLConfig{});
	
	/**
	 * @brief Replace a game record in namespace-level. If the record doesn't exist, it will create and save the record. 
	 *		If already exists, it will replace the existing one.
	 *
	 * @param Key Key of record.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ReplaceGameRecord(FString const& Key
		, FJsonObject const& RecordRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete a game record under the given key in namespace-level.
	 *
	 * @param Key Key of record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteGameRecord(FString const& Key
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Save a user-level record with Metadata input value
	 *
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
     * @param SetBy Metadata set_by value, the value can be SERVER or CLIENT 
     * @param SetPublic Metadata is_public value 
	 * @param RecordRequest The request of the record with JSON formatted. 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveUserRecord(FString const& Key
		, FString const& UserId
		, ESetByMetadataRecord SetBy
		, bool SetPublic
		, FJsonObject const& RecordRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Save a user-level record. If the record doesn't exist, it will create and save the record, if already exists, it will append to the existing one.
	 *
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param RecordRequest The request of the record with JSON formatted. 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveUserRecord(FString const& Key
		, FString const& UserId
		, FJsonObject const& RecordRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief This function that contains option adding suffix /public in the Url will be DEPRECATED.
	 * @brief Save a user-level record. If the record doesn't exist, it will create and save the record, if already exists, it will append to the existing one.
	 *
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param SetBy Metadata value
	 * @param SetPublic Metadata value 
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param bIsPublic Save the record as a public/private record. If true, no support metadata object field/will not included in record. 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveUserRecord(FString const& Key
		, FString const& UserId
		, ESetByMetadataRecord SetBy
		, bool SetPublic
		, FJsonObject const& RecordRequest
		, bool bIsPublic
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief This function that contains option adding suffix /public in the Url will be DEPRECATED.
	 * @brief Save a user-level record. If the record doesn't exist, it will create and save the record, if already exists, it will append to the existing one.
     * @brief The end point of this method if bIsPublic is true (using suffix /public) will be deprecated in future
     * 
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param bIsPublic Save the record as a public/private record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveUserRecord(FString const& Key
		, FString const& UserId
		, FJsonObject const& RecordRequest
		, bool bIsPublic
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a record (arbitrary JSON data) by its key in user-level.
	 *
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetUserRecord(FString const& Key
		, FString const& UserId
		, THandler<FAccelByteModelsUserRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a public record (arbitrary JSON data) by its key and userId in user-level.
	 *
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPublicUserRecord(FString const& Key
		, FString const& UserId
		, THandler<FAccelByteModelsUserRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Replace a record in user-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
	 * @brief The end point of this method if bIsPublic is true (using suffix /public) will be deprecated in future   
	 * @param Key Key of record.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ReplaceUserRecord(FString const& Key
		, ESetByMetadataRecord SetBy
		, bool SetPublic
		, FString const& UserId
		, FJsonObject const& RecordRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**         
	 * @brief Replace a record in user-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
	 *
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param RecordRequest The request of the record with JSON formatted. 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ReplaceUserRecord(FString const& Key
		, FString const& UserId
		, FJsonObject const& RecordRequest
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief 
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param SetBy Metadata value
	 * @param SetPublic Metadata value 
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param bIsPublic Save the record as a public/private record. If true, no support metadata object field/will not included in record. 
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ReplaceUserRecord(FString const& Key
		, FString const& UserId
		, ESetByMetadataRecord SetBy
		, bool SetPublic
		, FJsonObject const& RecordRequest
		, bool bIsPublic
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Replace a record in user-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
	 * @brief The end point of this method if bIsPublic is true (using suffix /public) will be deprecated in future   
	 *
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param bIsPublic Save the record as a public/private record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ReplaceUserRecord(FString const& Key
		, FString const& UserId
		, FJsonObject const& RecordRequest
		, bool bIsPublic
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete a record under the given key in user-level.
	 * @brief The end point of this method if bIsPublic is true (using suffix /public) will be deprecated in future   
	 *
	 * @param Key Key of record.
	 * @param UserId UserId of the record owner.
	 * @param bIsPublic Save the record as a public/private record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteUserRecord(FString const& Key
		, FString const& UserId
		, bool bIsPublic
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk Get Multiple Size of Player Record 
	 *
	 * @param GetPlayerRecordSizeRequest Detail information for the Player Record Request.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetPlayerRecordSize(FAccelByteModelsBulkGetPlayerRecordSizeRequest const& GetPlayerRecordSizeRequest
		, THandler<FAccelByteModelsPaginatedBulkGetPlayerRecordSizeResponse> const& OnSuccess
		, FErrorHandler const& OnError);

#pragma region AdminRecord

	/**
	 * @brief Create new admin game record or append the existing admin game record.
	 *
	 * @param Key Key of record
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateAdminGameRecord(FString const& Key
		, FJsonObject const& RecordRequest
		, THandler<FAccelByteModelsAdminGameRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a record by its key in namespace-level.
	 *
	 * @param Key Key of record
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryAdminGameRecordsByKey(FString const& Key
		, THandler<FAccelByteModelsAdminGameRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Retrieve list of records key by namespace
	 *
	 * @param Limit The limit of the keys result.
	 * @param Offset Distance between the beginning of the data list and a given point.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryAdminGameRecordKeys(int Limit
		, int Offset
		, THandler<FAccelByteModelsPaginatedRecordsKey> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Create new admin game record or replace the existing admin game record.
	 *
	 * @param Key Key of record
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ReplaceAdminGameRecord(FString const& Key
		, FJsonObject const& RecordRequest
		, THandler<FAccelByteModelsAdminGameRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete game record in namespace-level
	 *
	 * @param Key Key of record
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteAdminGameRecord(FString const& Key
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Create new admin user record or append the existing admin user record.
	 *
	 * @param Key Key of record
	 * @param UserId UserId of the record owner.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateAdminUserRecord(FString const& Key
		, FString const& UserId
		, FJsonObject const& RecordRequest
		, THandler<FAccelByteModelsAdminUserRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query admin user records by Key
	 *
	 * @param Key Key of record
	 * @param UserId UserId of the record owner.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryAdminUserRecordsByKey(FString const& Key
		, FString const& UserId
		, THandler<FAccelByteModelsAdminUserRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Query admin user record Keys
	 *
	 * @param UserId UserId of the record owner.
	 * @param Limit
	 * @param Offset
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr QueryAdminUserRecordKeys(FString const& UserId
		, int Limit
		, int Offset
		, THandler<FAccelByteModelsPaginatedGetAdminUserRecordKeysResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Replace admin user record
	 *
	 * @param Key Key of record
	 * @param UserId UserId of the record owner.
	 * @param RecordRequest The request of the record with JSON formatted.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ReplaceAdminUserRecord(FString const& Key
		, FString const& UserId
		, FJsonObject const& RecordRequest
		, THandler<FAccelByteModelsAdminUserRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete admin user record
	 *
	 * @param Key Key of record
	 * @param UserId UserId of the record owner.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteAdminUserRecord(FString const& Key
		, FString const& UserId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

#pragma endregion

private:
	ServerCloudSave() = delete;
	ServerCloudSave(ServerCloudSave const&) = delete;
	ServerCloudSave(ServerCloudSave&&) = delete;

	FJsonObject CreateGameRecordWithMetadata(ESetByMetadataRecord SetBy
		, FTTLConfig const& TTLConfig
		, FJsonObject const& RecordRequest);
	
	FJsonObject CreatePlayerRecordWithMetadata(ESetByMetadataRecord SetBy
		, bool SetPublic
		, FJsonObject const& RecordRequest);
	
	static FJsonObjectWrapper ConvertJsonObjToJsonObjWrapper(TSharedPtr<FJsonObject> const*& value);
};

} // namespace GameServerApi
} // namespace AccelByte