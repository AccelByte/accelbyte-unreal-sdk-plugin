// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerApiBase.h"
#include "Models/AccelByteBinaryCloudSaveModels.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{

/**
 * @brief Binary CloudSave API for storing records.
 */
class ACCELBYTEUE4SDK_API ServerBinaryCloudSave : public FServerApiBase
{
public:
	ServerBinaryCloudSave(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerBinaryCloudSave();

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
	FAccelByteTaskWPtr QueryGameBinaryRecords(FString const& Query
		, THandler<FAccelByteModelsPaginatedGameBinaryRecords> const& OnSuccess
		, FErrorHandler const& OnError
		, int32 Offset = 0
		, int32 Limit = 20);

	/**
	 * @brief Create game binary records.
	 *
	 * @param Key Key of the binary record.
	 * @param FileType File type of the binary (supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin).
	 * @param SetBy Metadata value.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param TTLConfig The configuration to control the action taken if the record has expired. If the action set to NONE,
	 *		then it will not send the TTL (Time to live) meta data.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateGameBinaryRecord(FString const& Key
		, EAccelByteFileType FileType
		, ESetByMetadataRecord SetBy
		, THandler<FAccelByteModelsBinaryInfo> const& OnSuccess
		, FErrorHandler const& OnError
		, FTTLConfig const& TTLConfig = FTTLConfig{});

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
	 * @brief Update a game binary record file by its key.
	 *
	 * @param Key Key of the binary record.
	 * @param ContentType The specific type of the binary record created.
	 * @param FileLocation Location of the uploaded binary file.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserBinaryRecord&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateGameBinaryRecord(FString const& Key
		, EAccelByteFileType ContentType
		, FString const& FileLocation
		, THandler<FAccelByteModelsGameBinaryRecord> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Delete a game binary record.
	 *
	 * @param Key Key of the binary record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteGameBinaryRecord(FString const& Key
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete the ttl config of the game binary record.
	 *
	 * @param Key Key of the binary record.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteGameBinaryRecordTTLConfig(FString const& Key
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Update a game binary record metadata by its key.
	 *
	 * @param Key Key of the binary record.
	 * @param SetBy Metadata value.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsUserBinaryRecord&.
	 * @param OnError This will be called when the operation failed.
	 * @param TTLConfig The configuration to control the action taken if the record has expired. If the action set to NONE,
	 *		then it will not send the TTL (Time to live) meta data.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateGameBinaryRecordMetadata(FString const& Key
		, ESetByMetadataRecord SetBy
		, THandler<FAccelByteModelsGameBinaryRecord> const& OnSuccess
		, FErrorHandler const& OnError
		, FTTLConfig const& TTLConfig = FTTLConfig{});

	/**
	 * @brief Request presigned URL to upload the binary record to s3.
	 *
	 * @param Key Key of the binary record.
	 * @param FileType File type of the binary (supported file types are jpeg, jpg, png, bmp, gif, mp3, webp, and bin).
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RequestGameBinaryRecordPresignedUrl(FString const& Key
		, EAccelByteFileType FileType
		, THandler<FAccelByteModelsBinaryInfo> const& OnSuccess
		, FErrorHandler const& OnError);

#pragma endregion

private:
	ServerBinaryCloudSave() = delete;
	ServerBinaryCloudSave(ServerBinaryCloudSave const&) = delete;
	ServerBinaryCloudSave(ServerBinaryCloudSave&&) = delete;

	static FAccelByteModelsGameBinaryRecord ConvertJsonToGameBinaryRecord(FJsonObject const& JsonObject);
};

} // namespace GameServerApi
} // namespace AccelByte
