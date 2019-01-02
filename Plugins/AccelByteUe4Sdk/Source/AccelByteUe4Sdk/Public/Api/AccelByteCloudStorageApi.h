// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteError.h"
#include "AccelByteCloudStorageModels.h"

namespace AccelByte
{
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
	DECLARE_DELEGATE_OneParam(FGetAllSlotsSuccess, const TArray<FAccelByteModelsSlot>&);
	/**
	 * @brief This function gets list of slot(s) those owned by the player.
	 *
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<FAccelByteModelsSlot>&.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetAllSlots(const FGetAllSlotsSuccess& OnSuccess, const FErrorHandler& OnError);
	
	DECLARE_DELEGATE_OneParam(FCreateSlotSuccess, const FAccelByteModelsSlot&);
	/**
	 * @brief This function creates a slot for an uploaded binary data.
	 *
	 * @param BinaryData This is the data that will be stored in the slot.
	 * @param FileName This is the filename of the item that saved in the slot.
	 * @param Tags This is the tags that will be stored in the slot.
	 * @param Label This is the label that will be stored in the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSlot&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateSlot(TArray<uint8> BinaryData, const FString& FileName, const FString& Tags, const FString& Label, const FCreateSlotSuccess& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FUpdateSlotSuccess, const FAccelByteModelsSlot&);
	/**
	 * @brief This function updates a stored slot.
	 *
	 * @param SlotId This is specific slot that will be updated.
	 * @param BinaryData This is the data that will be stored in the slot.
	 * @param FileName This is the filename of the item that saved in the slot.
	 * @param Tags This is the tags that will be stored in the slot.
	 * @param Label This is the label that will be stored in the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const FAccelByteModelsSlot&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UpdateSlot(FString SlotId, const TArray<uint8> BinaryData, const FString& FileName, const FString& Tags, const FString& Label, const FUpdateSlotSuccess& OnSuccess, FHttpRequestProgressDelegate OnProgress, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetSlotSuccess, const TArray<uint8>&);
	/**
	 * @brief This function gets the data that stored in the slot.
	 *
	 * @param SlotId Specify the slot.
	 * @param OnSuccess This will be called when the operation succeeded. The result is const TArray<uint8>&.
	 * @param OnProgress This is delegate called per tick to update an Http request upload or download size progress.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetSlot(FString SlotId, const FGetSlotSuccess& OnSuccess, const FErrorHandler& OnError);

    DECLARE_DELEGATE(FDeleteSlotSuccess);
	/**
	 * @brief This function delete the specified slot.
	 *
	 * @param SlotId Specify the slot.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	*/
    static void DeleteSlot(FString SlotId, const FDeleteSlotSuccess& OnSuccess, const FErrorHandler& OnError);

private:
	CloudStorage() = delete;
	CloudStorage(CloudStorage const&) = delete;
	CloudStorage(CloudStorage&&) = delete;

	static TArray<uint8> FormDataBuilder(TArray<uint8> BinaryData,FString BoundaryGuid, FString FileName);
    static void OnGetAllSlotsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetAllSlotsSuccess OnSuccess, FErrorHandler OnError);
    static void OnCreateSlotResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateSlotSuccess OnSuccess, FErrorHandler OnError);
    static void OnUpdateSlotResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateSlotSuccess OnSuccess, FErrorHandler OnError);
    static void OnGetSlotResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetSlotSuccess OnSuccess, FErrorHandler OnError);
    static void OnDeleteSlotResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FDeleteSlotSuccess OnSuccess, FErrorHandler OnError);

};

} // Namespace Api
} // Namespace AccelByte
