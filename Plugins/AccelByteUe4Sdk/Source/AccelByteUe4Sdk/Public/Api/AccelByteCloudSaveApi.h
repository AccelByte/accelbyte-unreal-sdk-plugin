// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteError.h"
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
	CloudSave(const Credentials& Credentials, const Settings& Settings);
	~CloudSave();

	///**
	// * @brief Save a user-level record. If the record doesn't exist, it will create and save the record, if already exists, it will append to the existing one.
	// *
	// * @param Key Key of record.
	// * @param RecordRequest The request of the record with JSON formatted.
	// * @param IsPublic Save the record as a public/private record.
	// * @param OnSuccess This will be called when the operation succeeded.
	// * @param OnError This will be called when the operation failed.
	// */
	void SaveUserRecord(const FString& Key, FJsonObject RecordRequest, bool IsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	///**
	// * @brief Get a record (arbitrary JSON data) by its key in user-level.
	// *
	// * @param Key Key of record.
	// * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserRecord&.
	// * @param OnError This will be called when the operation failed.
	// */
	void GetUserRecord(const FString& Key, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError);

	///**
	// * @brief Get a public record (arbitrary JSON data) by its key and userId in user-level.
	// *
	// * @param Key Key of record.
	// * @param UserId UserId of the record owner.
	// * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserRecord&.
	// * @param OnError This will be called when the operation failed.
	// */
	void GetPublicUserRecord(const FString& Key, const FString& UserId, const THandler<FAccelByteModelsUserRecord>& OnSuccess, const FErrorHandler& OnError);

	///**
	// * @brief Replace a record in user-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
	// *
	// * @param Key Key of record.
	// * @param RecordRequest The request of the record with JSON formatted.
	// * @param IsPublic Save the record as a public/private record.
	// * @param OnSuccess This will be called when the operation succeeded.
	// * @param OnError This will be called when the operation failed.
	// */
	void ReplaceUserRecord(const FString& Key, FJsonObject RecordRequest, bool IsPublic, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	///**
	// * @brief Delete a record under the given key in user-level.
	// *
	// * @param Key Key of record.
	// * @param OnSuccess This will be called when the operation succeeded.
	// * @param OnError This will be called when the operation failed.
	// */
	void DeleteUserRecord(const FString& Key, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	///**
	// * @brief Save a namespace-level record. If the record doesn't exist, it will create and save the record, if already exists, it will append to the existing one.
	// *
	// * @param Key Key of record.
	// * @param RecordRequest The request of the record with JSON formatted.
	// * @param OnSuccess This will be called when the operation succeeded.
	// * @param OnError This will be called when the operation failed.
	// */
	void SaveGameRecord(const FString& Key, FJsonObject RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	///**
	// * @brief Get a record by its key in namespace-level.
	// *
	// * @param Key Key of record.
	// * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGameRecord&.
	// * @param OnError This will be called when the operation failed.
	// */
	void GetGameRecord(const FString& Key, const THandler<FAccelByteModelsGameRecord>& OnSuccess, const FErrorHandler& OnError);

	///**
	// * @brief Replace a record in namespace-level. If the record doesn't exist, it will create and save the record. If already exists, it will replace the existing one.
	// *
	// * @param Key Key of record.
	// * @param RecordRequest The request of the record with JSON formatted.
	// * @param OnSuccess This will be called when the operation succeeded.
	// * @param OnError This will be called when the operation failed.
	// */
	void ReplaceGameRecord(const FString& Key, FJsonObject RecordRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	///**
	// * @brief Delete a record under the given key in namespace-level.
	// *
	// * @param Key Key of record.
	// * @param OnSuccess This will be called when the operation succeeded.
	// * @param OnError This will be called when the operation failed.
	// */
	void DeleteGameRecord(const FString& Key, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

private:
	const Credentials& Credentials;
	const Settings& Settings;

	CloudSave() = delete;
	CloudSave(CloudSave const&) = delete;
	CloudSave(CloudSave&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte
