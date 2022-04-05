// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.
#pragma once

#include "CoreMinimal.h"
#include "JsonUtilities.h"
#include "JsonObjectConverter.h"
#include "Core/AccelByteError.h"

namespace AccelByte
{
	class ACCELBYTEUE4SDK_API IAccelByteDataStorage
	{
	public:
		/**
		 * @brief Reset an existing Table in the Storage.
		 *
		 * @param Result This will be called when the operation done. The result is bool.
		 * @param TableName optional. The name of the table. Default will drop the default KeyValue table.
		*/
		virtual void Reset(const THandler<bool>& Result, const FString& TableName = TEXT("DefaultKeyValueTable")) = 0;

		/**
		 * @brief Delete an Item in the Table.
		 *
		 * @param Key The Key of the Item.
		 * @param OnDone This will be called when the operation done.
		 * @param TableName optional. The name of the table. Default will drop the default KeyValue table.
		*/
		virtual void DeleteItem(const FString& Key, const FVoidHandler OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) = 0;

		/**
		 * @brief Insert Item to the Key Value Table.
		 *
		 * @param Key The Key of the Item.
		 * @param Item The Data to be inserted to the Table. The Data would be an array of uint8.
		 * @param OnDone This will be called when the operation done. The result is bool.
		 * @param TableName optional. The name of the table. Default will insert an item to the default KeyValue table.
		*/
		virtual void SaveItem(const FString& Key, const TArray<uint8>& Item, const THandler<bool>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) = 0;

		/**
		 * @brief Insert Item to the Key Value Table.
		 *
		 * @param Key The Key of the Item.
		 * @param Item The Data to be inserted to the Table. The Data would be a FString.
		 * @param OnDone This will be called when the operation done. The result is bool.
		 * @param TableName optional. The name of the table. Default will insert an item to the default KeyValue table.
		*/
		virtual void SaveItem(const FString& Key, const FString& Item, const THandler<bool>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) = 0;

		/**
		 * @brief Insert Item to the Key Value Table.
		 *
		 * @param Key The Key of the Item.
		 * @param Item The Data to be inserted to the Table. The Data would be a FJsonObjectWrapper.
		 * @param OnDone This will be called when the operation done. The result is bool.
		 * @param TableName optional. The name of the table. Default will insert an item to the default KeyValue table.
		*/
		virtual void SaveItem(const FString& Key, const FJsonObjectWrapper& Item, const THandler<bool>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) = 0;

		/**
		 * @brief Get an Item from the Key Value Table.
		 *
		 * @param Key The Key of the Item to Get.
		 * @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and an array of uint8 Value.
		 * @param TableName optional. The name of the table. Default will get an item from the default KeyValue table.
		*/
		virtual void GetItem(const FString& Key, const THandler<TPair<FString, TArray<uint8>>>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) = 0;

		/**
		 * @brief Get an Item from the Key Value Table.
		 *
		 * @param Key The Key of the Item to Get.
		 * @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and a FString Value.
		 * @param TableName optional. The name of the table. Default will get an item from the default KeyValue table.
		*/
		virtual void GetItem(const FString& Key, const THandler<TPair<FString, FString>>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) = 0;

		/**
		 * @brief Get an Item from the Key Value Table.
		 *
		 * @param Key The Key of the Item to Get.
		 * @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and a FJsonObjectWrapper Value.
		 * @param TableName optional. The name of the table. Default will get an item from the default KeyValue table.
		*/
		virtual void GetItem(const FString& Key, const THandler<TPair<FString, FJsonObjectWrapper>>& OnDone, const FString& TableName = TEXT("DefaultKeyValueTable")) = 0;
	};
}
