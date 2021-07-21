// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteGeneralModels.h"
#include "Models/AccelByteCloudSaveModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{
	/**
	 * @brief CloudSave API for storing records.
	 */
	class ACCELBYTEUE4SDK_API ServerCloudSave
	{
	public:
		ServerCloudSave(const ServerCredentials& Credentials, const ServerSettings& Settings);
		~ServerCloudSave();

		/**
		 * @brief Retrieve all available namespace-level game records key.
		 *
		 * @param OnSuccess This will be called when the operation succeeded. The result is in const FAccelByteModelsPaginatedRecordsKey&.
		 * @param OnError This will be called when the operation failed.
		 * @param Offset The offset of the achievements result. Default value is 0.
		 * @param Limit The limit of the achievements result. Default value is 20.
		 */
		void RetrieveGameRecordsKey(const THandler<FAccelByteModelsPaginatedRecordsKey>& OnSuccess, const FErrorHandler& OnError, int32 Offset = 0, int32 Limit = 20);

		/**
		 * @brief Save a namespace-level game record. If the record doesn't exist, it will create and save the record, if already exists, it will append to the existing one.
		 *
		 * @param Key Key of record.
		 * @param RecordRequest The request of the record with JSON formatted.
		 * @param OnSuccess This will be called when the operation succeeded.
		 * @param OnError This will be called when the operation failed.
		 */
		void SaveGameRecord(const FString& Key, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

		/**
		 * @brief Get a game record by its key in namespace-level.
		 *
		 * @param Key Key of record.
		 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGameRecord&.
		 * @param OnError This will be called when the operation failed.
		 */
		void GetGameRecord(const FString& Key, const THandler<FAccelByteModelsGameRecord>& OnSuccess, const FErrorHandler& OnError);

		/**
		 * @brief Replace a game record in namespace-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
		 *
		 * @param Key Key of record.
		 * @param RecordRequest The request of the record with JSON formatted.
		 * @param OnSuccess This will be called when the operation succeeded.
		 * @param OnError This will be called when the operation failed.
		 */
		void ReplaceGameRecord(const FString& Key, const FJsonObject& RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

		/**
		 * @brief Delete a game record under the given key in namespace-level.
		 *
		 * @param Key Key of record.
		 * @param OnSuccess This will be called when the operation succeeded.
		 * @param OnError This will be called when the operation failed.
		 */
		void DeleteGameRecord(const FString& Key, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
		
		/**
		 * @brief Save a user-level record. If the record doesn't exist, it will create and save the record, if already exists, it will append to the existing one.
		 *
		 * @param Key Key of record.
		 * @param UserId UserId of the record owner.
		 * @param RecordRequest The request of the record with JSON formatted.
		 * @param bIsPublic Save the record as a public/private record.
		 * @param OnSuccess This will be called when the operation succeeded.
		 * @param OnError This will be called when the operation failed.
		 */
		void SaveUserRecord(const FString& Key, const FString& UserId, const FJsonObject& RecordRequest, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

		/**
		 * @brief Get a record (arbitrary JSON data) by its key in user-level.
		 *
		 * @param Key Key of record.
		 * @param UserId UserId of the record owner.
		 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserRecord&.
		 * @param OnError This will be called when the operation failed.
		 */
		void GetUserRecord(const FString& Key, const FString& UserId, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError);

		/**
		 * @brief Get a public record (arbitrary JSON data) by its key and userId in user-level.
		 *
		 * @param Key Key of record.
		 * @param UserId UserId of the record owner.
		 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserRecord&.
		 * @param OnError This will be called when the operation failed.
		 */
		void GetPublicUserRecord(const FString& Key, const FString& UserId, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError);

		/**
		 * @brief Replace a record in user-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
		 *
		 * @param Key Key of record.
		 * @param RecordRequest The request of the record with JSON formatted.
		 * @param bIsPublic Save the record as a public/private record.
		 * @param OnSuccess This will be called when the operation succeeded.
		 * @param OnError This will be called when the operation failed.
		 */
		void ReplaceUserRecord(const FString& Key, const FString& UserId, const FJsonObject& RecordRequest, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

		/**
		 * @brief Delete a record under the given key in user-level.
		 *
		 * @param Key Key of record.
		 * @param UserId UserId of the record owner.
		 * @param bIsPublic Save the record as a public/private record.
		 * @param OnSuccess This will be called when the operation succeeded.
		 * @param OnError This will be called when the operation failed.
		 */
		void DeleteUserRecord(const FString& Key, const FString& UserId, bool bIsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);


	private:
		const ServerCredentials& Credentials;
		const ServerSettings& Settings;

		ServerCloudSave() = delete;
		ServerCloudSave(ServerCloudSave const&) = delete;
		ServerCloudSave(ServerCloudSave&&) = delete;
	};
} // namespace GameServerApi
} // namespace AccelByte