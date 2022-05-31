// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

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
 * @brief Cloud Storage API for storing binary data on the cloud.
 * Each player has configurable slot(s) that controlled by admin.
 * The configuration affects the amount of slot that owned by user and the allowed size for each slot (byte).
 */
class ACCELBYTEUE4SDK_API CloudStorage
{
public:
	CloudStorage(Credentials const& InCredentialsRef, Settings const& InSettingsRef);
	~CloudStorage();
private:
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;
public:
	/**
	 * @brief This function gets list of slot(s) those owned by the player.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsSlot>&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetAllSlots(const THandler<TArray<FAccelByteModelsSlot>>& OnSuccess, const FErrorHandler& OnError);
	
	/**
	 * @brief This function creates a slot for an uploaded binary data.
	 *
	 * @param BinaryData This is the data that will be stored in the slot.
	 * @param FileName This is the filename of the item that saved in the slot.
	 * @param Tags This is the tags that will be stored in the slot.
	 * @param Label This is the label that will be stored in the slot.
	 * @param CustomAttribute The custom attribute for the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSlot&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 */
	void CreateSlot(TArray<uint8> BinaryData, const FString& FileName, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const THandler<FAccelByteModelsSlot>& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError);

	/**
	 * @brief This function updates a stored slot.
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
	 */
	void UpdateSlot(FString SlotId, const TArray<uint8> BinaryData, const FString& FileName, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const THandler<FAccelByteModelsSlot>& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError);
	
	/**
	 * @brief This function updates stored slot's metadata. This function is DEPRECATED.
	 *
	 * @param SlotId This is specific slot that will be updated.
	 * @param FileName This is the filename of the item that saved in the slot.
	 * @param Tags This is an array of tag that will be stored in the slot.
	 * @param Label This is the label that will be stored in the slot.
	 * @param CustomAttribute The custom attribute for the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSlot&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 */
	void UpdateSlotMetadata(const FString& SlotId, const FString& FileName, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const THandler<FAccelByteModelsSlot>& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError);

	/**
	 * @brief This function updates stored slot's metadata.
	 *
	 * @param SlotId This is specific slot that will be updated.
	 * @param Tags This is an array of tag that will be stored in the slot.
	 * @param Label This is the label that will be stored in the slot.
	 * @param CustomAttribute The custom attribute for the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSlot&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 */
	void UpdateSlotMetadata(const FString& SlotId, const TArray<FString>& Tags, const FString& Label, const FString& CustomAttribute, const THandler<FAccelByteModelsSlot>& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError);

	/**
	 * @brief This function gets the data that stored in the slot.
	 *
	 * @param SlotId Specify the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<uint8>&.
	 * @param OnError This will be called when the operation failed.
	 */
	void GetSlot(FString SlotId, const THandler<TArray<uint8>>& OnSuccess, const FErrorHandler& OnError);

	/**
	 * @brief This function delete the specified slot.
	 *
	 * @param SlotId Specify the slot.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	*/
	void DeleteSlot(FString SlotId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

private:
	CloudStorage() = delete;
	CloudStorage(CloudStorage const&) = delete;
	CloudStorage(CloudStorage&&) = delete;

	TArray<uint8> FormDataBuilder(TArray<uint8> BinaryData,FString BoundaryGuid, FString FileName);
	
	/*
	 * @brief Generate custom attribute form data.
	 * 
	 * @param CustomAttribute This is the content.
	 * @param BoundaryGuid This is the boundary.
	 * @param CloseFooter TRUE = the boundary will be close with extra double dash "--"; FALSE = the boundary will be left open and it can be appended with another form data.
	*/
	TArray<uint8> CustomAttributeFormDataBuilder(const FString& CustomAttribute, FString BoundaryGuid, bool CloseFooter);
};

} // Namespace Api
} // Namespace AccelByte
