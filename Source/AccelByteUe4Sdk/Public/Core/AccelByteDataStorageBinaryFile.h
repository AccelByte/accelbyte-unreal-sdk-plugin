// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Models/AccelByteOauth2Models.h"
#include "Core/IAccelByteDataStorage.h"
#include "HAL/FileManager.h"
#include "Misc/Optional.h"
#include "AccelByteDataStorageBinaryFile.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FArrayByte
{
	FArrayByte(const TArray<uint8>& InContent = TArray<uint8>())
	{
		Content = InContent;
	}

	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | FArrayByte")
	TArray<uint8> Content{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBinaryFileStructure
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | FBinaryFileStructure")
	TMap<FString, FArrayByte> Segments{};
};

namespace AccelByte
{

class ACCELBYTEUE4SDK_API DataStorageBinaryFile : public IAccelByteDataStorage
{

public:
#if PLATFORM_WINDOWS
	DataStorageBinaryFile(FString DirectoryPath = FPaths::ProjectContentDir());
#else
	DataStorageBinaryFile(FString DirectoryPath = TEXT(""));
#endif
	virtual ~DataStorageBinaryFile() {}

	/**
		* @brief Reset an existing Table in the Storage.
		*
		* @param Result This will be called when the operation done. The result is bool.
		* @param FileName optional. The name of the file. Default will drop the default storage file.
	*/
	void Reset(const THandler<bool>& Result, const FString& FileName = TEXT("DefaultFileName")) override;

	/**
		* @brief Delete an Item in the file.
		*
		* @param Key The Key of the Item.
		* @param OnDone This will be called when the operation done.
		* @param FileName optional. The name of the file. Default will drop the default storage file.
	*/
	void DeleteItem(const FString& Key, const FVoidHandler OnDone, const FString& TableName = TEXT("DefaultFileName")) override;

	/**
		* @brief Insert Item to the Key Value file.
		*
		* @param Key The Key of the Item.
		* @param Item The Data to be inserted to the file. The Data would be an array of uint8.
		* @param OnDone This will be called when the operation done. The result is bool.
		* @param FileName optional. The name of the file. Default will insert an item to the default storage file.
	*/
	void SaveItem(const FString& Key, const TArray<uint8>& Item, const THandler<bool>& OnDone, const FString& TableName = TEXT("DefaultFileName")) override;

	/**
		* @brief Insert Item to the Key Value file.
		*
		* @param Key The Key of the Item.
		* @param Item The Data to be inserted to the file. The Data would be a FString.
		* @param OnDone This will be called when the operation done. The result is bool.
		* @param FileName optional. The name of the file. Default will insert an item to the default storage file.
	*/
	void SaveItem(const FString& Key, const FString& Item, const THandler<bool>& OnDone, const FString& TableName = TEXT("DefaultFileName")) override;

	/**
		* @brief Insert Item to the Key Value file.
		*
		* @param Key The Key of the Item.
		* @param Item The Data to be inserted to the file. The Data would be a FJsonObjectWrapper.
		* @param OnDone This will be called when the operation done. The result is bool.
		* @param FileName optional. The name of the file. Default will insert an item to the default storage file.
	*/
	void SaveItem(const FString& Key, const FJsonObjectWrapper& Item, const THandler<bool>& OnDone, const FString& TableName = TEXT("DefaultFileName")) override;

	/**
		* @brief Get an Item from the Key Value file.
		*
		* @param Key The Key of the Item to Get.
		* @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and an array of uint8 Value.
		* @param FileName optional. The name of the file. Default will get an item from the default storage file.
	*/
	void GetItem(const FString& Key, const THandler<TPair<FString, TArray<uint8>>>& OnDone, const FString& TableName = TEXT("DefaultFileName")) override;

	/**
		* @brief Get an Item from the Key Value file.
		*
		* @param Key The Key of the Item to Get.
		* @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and a FString Value.
		* @param FileName optional. The name of the file. Default will get an item from the default storage file.
	*/
	void GetItem(const FString& Key, const THandler<TPair<FString, FString>>& OnDone, const FString& TableName = TEXT("DefaultFileName")) override;

	/**
		* @brief Get an Item from the Key Value file.
		*
		* @param Key The Key of the Item to Get.
		* @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and a FJsonObjectWrapper Value.
		* @param FileName optional. The name of the file. Default will get an item from the default storage file.
	*/
	void GetItem(const FString& Key, const THandler<TPair<FString, FJsonObjectWrapper>>& OnDone, const FString& TableName = TEXT("DefaultFileName")) override;

	/**
	* @brief Helper function
	*/
	FDirectoryPath GetAbsoluteFileDirectory();

protected:
	/**
	* @brief The main storage directory.
	*/
	FDirectoryPath RelativeFileDirectory;

	/**
	* @brief Convert the filename within the storage directory into fullpath.
	* 
	* @param FileName File name only, without any prefix.
	* @return The complete absolute path of the file, appent by the storage directory.
	*/
	virtual FString CompleteAbsoluteFilePath(const FString& FileName);

	/**
	* @brief Obtain the content of the specified file and return it.
	*
	* @param FileName The name of the file, not the absolute path.
	* @return The content of the file if the file exists. If value is not set, error did occur.
	*/
	virtual TOptional<FString> LoadFromFile(const FString& FileName);

	/**
	* @brief Load the targeted file, modify the specified key, and then save it again.
	*
	* @param FileName The name of the file, not the absolute path.
	* @param Key The key to store the specified value.
	* @param Value The array of byte that will be written based on the key.
	* @return Is the save operation success.
	*/
	virtual bool SaveToFile(const FString& FileName, const FString& Key, const TArray<uint8>& Value);

	/**
	* @brief Read file content and deserialize it into a pair mapped binary information
	*
	* @param FileName The name of the file, not the absolute path.
	* @return The deserialization result. Nullptr if error occurs.
	*/
	virtual TSharedPtr<FBinaryFileStructure> ParseStructureFromFile(const FString& FileName);

	/**
	* @brief Check the existing file within the file storage directory.
	*/
	virtual bool IsFileExist(const FString& FileName);
};

}
