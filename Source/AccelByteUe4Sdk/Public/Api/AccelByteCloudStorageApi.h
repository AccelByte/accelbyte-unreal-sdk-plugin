// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteApiBase.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteCloudStorageModels.h"

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
 * @brief DEPRECATED.Cloud Storage API for storing binary data on the cloud.
 * Each player has configurable slot(s) that controlled by admin.
 * The configuration affects the amount of slot that owned by user and the allowed size for each slot (byte).
 */
class ACCELBYTEUE4SDK_API CloudStorage 
	: public FApiBase
	, public TSharedFromThis<CloudStorage, ESPMode::ThreadSafe>
{
public:
	CloudStorage(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient = nullptr);

	CloudStorage(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetrySchedulerBase& InHttpRef
		, FAccelBytePlatformPtr const& InAccelBytePlatform);

	virtual ~CloudStorage();

	/**
	 * @brief DEPRECATED.This function gets list of slot(s) those owned by the player. Cloud Storage is DEPRECATED.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsSlot>&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetAllSlots(THandler<TArray<FAccelByteModelsSlot>> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief DEPRECATED.This function creates a slot for an uploaded binary data. Cloud Storage is DEPRECATED.
	 *
	 * @param BinaryData This is the data that will be stored in the slot.
	 * @param FileName This is the filename of the item that saved in the slot.
	 * @param Tags This is the tags that will be stored in the slot.
	 * @param Label This is the label that will be stored in the slot.
	 * @param CustomAttribute The custom attribute for the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSlot&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateSlot(TArray<uint8> const& BinaryData
		, FString const& FileName
		, TArray<FString> const& Tags
		, FString const& Label
		, FString const& CustomAttribute
		, THandler<FAccelByteModelsSlot> const& OnSuccess
		, FHttpRequestProgressDelegate const& OnProgress
		, FErrorHandler const& OnError);

	/**
	 * @brief DEPRECATED.This function updates a stored slot. Cloud Storage is DEPRECATED.
	 *
	 * @param SlotId This is specific slot that will be updated.
	 * @param BinaryData This is the data that will be stored in the slot.
	 * @param FileName This is the filename of the item that saved in the slot.
	 * @param Tags This is the tags that will be stored in the slot.
	 * @param Label This is the label that will be stored in the slot.
	 * @param CustomAttribute The custom attribute for the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSlot&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateSlot(FString const& SlotId
		, TArray<uint8> const& BinaryData
		, FString const& FileName
		, TArray<FString> const& Tags
		, FString const& Label
		, FString const& CustomAttribute
		, THandler<FAccelByteModelsSlot> const& OnSuccess
		, FHttpRequestProgressDelegate const& OnProgress
		, FErrorHandler const& OnError);
	
	/**
	 * @brief DEPRECATED.This function updates stored slot's metadata. Cloud Storage is DEPRECATED.
	 *
	 * @param SlotId This is specific slot that will be updated.
	 * @param FileName This is the filename of the item that saved in the slot.
	 * @param Tags This is an array of tag that will be stored in the slot.
	 * @param Label This is the label that will be stored in the slot.
	 * @param CustomAttribute The custom attribute for the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSlot&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateSlotMetadata(FString const& SlotId
		, FString const& FileName
		, TArray<FString> const& Tags
		, FString const& Label
		, FString const& CustomAttribute
		, THandler<FAccelByteModelsSlot> const& OnSuccess
		, FHttpRequestProgressDelegate const& OnProgress
		, FErrorHandler const& OnError);

	/**
	 * @brief DEPRECATED.This function updates stored slot's metadata. Cloud Storage is DEPRECATED.
	 *
	 * @param SlotId This is specific slot that will be updated.
	 * @param Tags This is an array of tag that will be stored in the slot.
	 * @param Label This is the label that will be stored in the slot.
	 * @param CustomAttribute The custom attribute for the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSlot&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr UpdateSlotMetadata(FString const& SlotId
		, TArray<FString> const& Tags
		, FString const& Label
		, FString const& CustomAttribute
		, THandler<FAccelByteModelsSlot> const& OnSuccess
		, FHttpRequestProgressDelegate const& OnProgress
		, FErrorHandler const& OnError);

	/**
	 * @brief DEPRECATED.This function gets the data that stored in the slot. Cloud Storage is DEPRECATED.
	 *
	 * @param SlotId Specify the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<uint8>&.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetSlot(FString SlotId
		, THandler<TArray<uint8>> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief DEPRECATED.This function delete the specified slot. Cloud Storage is DEPRECATED.
	 *
	 * @param SlotId Specify the slot.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteSlot(FString const& SlotId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

private:
	CloudStorage() = delete;
	CloudStorage(CloudStorage const&) = delete;
	CloudStorage(CloudStorage&&) = delete;

	TArray<uint8> FormDataBuilder(TArray<uint8> const& BinaryData
		, FString const& BoundaryGuid
		, FString const& FileName);
	
	/**
	 * @brief DEPRECATED.Generate custom attribute form data. Cloud Storage is DEPRECATED.
	 * 
	 * @param CustomAttribute This is the content.
	 * @param BoundaryGuid This is the boundary.
	 * @param CloseFooter TRUE = the boundary will be close with extra double dash "--"; FALSE = the boundary will be left open and it can be appended with another form data.
	*/
	TArray<uint8> CustomAttributeFormDataBuilder(FString const& CustomAttribute
		, FString const& BoundaryGuid
		, bool CloseFooter);
};

using CloudStorageRef = TSharedRef<CloudStorage, ESPMode::ThreadSafe>;
using CloudStoragePtr = TSharedPtr<CloudStorage, ESPMode::ThreadSafe>;
using CloudStorageWPtr = TWeakPtr<CloudStorage, ESPMode::ThreadSafe>;

} // Namespace Api
} // Namespace AccelByte
