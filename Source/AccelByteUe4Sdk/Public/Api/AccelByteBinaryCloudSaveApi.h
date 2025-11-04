// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteBinaryCloudSaveModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief CloudSave API for storing binaries.
 */
class ACCELBYTEUE4SDK_API BinaryCloudSave 
	: public FApiBase
	, public TSharedFromThis<BinaryCloudSave, ESPMode::ThreadSafe>
{
public:
	BinaryCloudSave(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	BinaryCloudSave(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~BinaryCloudSave();
		
	/**
	 * @brief Save a namespace-level user binary.
	 * If the binary doesn't exist, it will create the binary save, if already exists, it will append to the existing one.
	 *
	 * @param Key Key of the binary record.
	 * @param FileType File type of the binary (supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin).
	 * @param bIsPublic Whether to save the binary as a public or private record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveUserBinaryRecord(FString const& Key
		, FString const& FileType
		, bool bIsPublic
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Save a namespace-level user binary.
	 * If the binary doesn't exist, it will create the binary save, if already exists, it will append to the existing one.
	 *
	 * @param Key Key of the binary record.
	 * @param FileType File type of the binary (supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin).
	 * @param bIsPublic Whether to save the binary as a public or private record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SaveUserBinaryRecord(FString const& Key
		, EAccelByteFileType FileType
		, bool bIsPublic
		, FVoidHandler const& OnSuccess, FErrorHandler const& OnError);

	/**
	 * @brief Get current user's binary record by its key in namespace-level.
	 *
	 * @param Key Key of the binary record.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserBinaryRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetCurrentUserBinaryRecord(FString const& Key
		, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a user's public binary record by its key and the owner's UserId in namespace-level.
	 *
	 * @param Key Key of the binary record.
	 * @param UserId UserId of the binary record owner.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserBinaryRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetPublicUserBinaryRecord(FString const& Key
		, FString const& UserId
		, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk get current user's binary records by their keys.
	 *
	 * @param Keys List of keys of the binary records.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsListUserBinaryRecords&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetCurrentUserBinaryRecords(const TArray<FString>& Keys
		, THandler<FAccelByteModelsListUserBinaryRecords> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk get multiple public binary records from a single user by their keys.
	 *
	 * @param Keys List of key(s) of the binary record.
	 * @param UserId UserId of the binary record owner.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsListUserBinaryRecords&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetPublicUserBinaryRecords(const TArray<FString>& Keys
		, FString const& UserId
		, THandler<FAccelByteModelsListUserBinaryRecords> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk get public binary records with the same key from multiple users.
	 *
	 * @param Key Key of the binary record.
	 * @param UserIds UserId list of the binary record owner(s).
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsListUserBinaryRecords&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetPublicUserBinaryRecords(FString const& Key
		, const TArray<FString>& UserIds
		, THandler<FAccelByteModelsListUserBinaryRecords> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk query current user's binary records.
	 *
	 * @param Query String that will be used to query the user's keys.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedUserBinaryRecords&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the result. Default value is 0.
	 * @param Limit The limit of the result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkQueryCurrentUserBinaryRecords(FString const& Query
		, THandler<FAccelByteModelsPaginatedUserBinaryRecords> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20);

	/**
	 * @brief Bulk query all of a user's public binary records.
	 *
	 * @param UserId UserId of the binary record owner.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedUserBinaryRecords&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the result. Default value is 0.
	 * @param Limit The limit of the result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkQueryPublicUserBinaryRecords(FString const& UserId
		, THandler<FAccelByteModelsPaginatedUserBinaryRecords> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20);

	/**
	 * @brief Update current user's binary record to point to an uploaded file.
	 *
	 * @param Key Key of the binary record.
	 * @param FileType File type of the uploaded binary (supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin).
	 * @param FileLocation Location of the uploaded binary file.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserBinaryRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateUserBinaryRecordFile(FString const& Key
		, FString const& FileType
		, FString const& FileLocation
		, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update current user's binary record to point to an uploaded file.
	 *
	 * @param Key Key of the binary record.
	 * @param ContentType The specific type of the binary record created.
	 * @param FileLocation Location of the uploaded binary file.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserBinaryRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateUserBinaryRecordFile(FString const& Key
		, EAccelByteFileType ContentType
		, FString const& FileLocation
		, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update current user's binary record's metadata in namespace-level.
	 *
	 * @param Key Key of the binary record.
	 * @param IsPublic Whether to update the binary into a public or private record.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserBinaryRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateUserBinaryRecordMetadata(FString const& Key
		, bool bIsPublic
		, THandler<FAccelByteModelsUserBinaryRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete current user's binary record under the given key in namespace-level.
	 *
	 * @param Key Key of the binary record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteUserBinaryRecord(FString const& Key
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Request a presigned url to upload current user's binary record file.
	 *
	 * @param Key Key of the binary record.
	 * @param FileType File type of the binary (supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin).
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RequestUserBinaryRecordPresignedUrl(FString const& Key
		, FString const& FileType
		, THandler<FAccelByteModelsBinaryInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Request a presigned url to upload current user's binary record file.
	 *
	 * @param Key Key of the binary record.
	 * @param FileType File type of the binary (supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin).
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RequestUserBinaryRecordPresignedUrl(FString const& Key
		, EAccelByteFileType FileType
		, THandler<FAccelByteModelsBinaryInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get a game binary record by its key.
	 *
	 * @param Key Key of the binary record.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsGameBinaryRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetGameBinaryRecord(FString const& Key
		, THandler<FAccelByteModelsGameBinaryRecord> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk get game binary records by their keys.
	 *
	 * @param Keys List of keys of the binary record.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsListGameBinaryRecords&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkGetGameBinaryRecords(TArray<FString> const& Keys
		, THandler<FAccelByteModelsListGameBinaryRecords> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Bulk query game binary records.
	 *
	 * @param Query String that will be used to query the game's binary record keys.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsPaginatedGameBinaryRecords&.
	 * @param OnError This will be called when the operation failed.
	 * @param Offset The offset of the result. Default value is 0.
	 * @param Limit The limit of the result. Default value is 20.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr BulkQueryGameBinaryRecords(FString const& Query
		, THandler<FAccelByteModelsPaginatedGameBinaryRecords> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 const& Offset = 0
		, int32 const& Limit = 20);

private:
	BinaryCloudSave() = delete;
	BinaryCloudSave(BinaryCloudSave const&) = delete;
	BinaryCloudSave(BinaryCloudSave&&) = delete;
	BinaryCloudSave& operator=(BinaryCloudSave const&) = delete;
	BinaryCloudSave& operator=(BinaryCloudSave&&) = delete;

};

using BinaryCloudSaveRef = TSharedRef<BinaryCloudSave, ESPMode::ThreadSafe>;
using BinaryCloudSavePtr = TSharedPtr<BinaryCloudSave, ESPMode::ThreadSafe>;
using BinaryCloudSaveWPtr = TWeakPtr<BinaryCloudSave, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
