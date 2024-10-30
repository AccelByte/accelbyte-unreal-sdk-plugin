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
struct ACCELBYTEUE4SDK_API FAccelByteArrayByteObsolete
{
	FAccelByteArrayByteObsolete(const TArray<uint8>& InContent = TArray<uint8>())
	{
		Content = InContent;
	}

	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | FArrayByte")
	TArray<uint8> Content{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteBinaryFileStructureObsolete
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | FBinaryFileStructure")
	TMap<FString, FAccelByteArrayByteObsolete> Segments{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBinaryContentIndependentSegment
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | FBinaryContentIndependentSegment")
	FString Key{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | FBinaryContentIndependentSegment")
	TArray<uint8> ArrayByte{};

	FBinaryContentIndependentSegment() {};
	FBinaryContentIndependentSegment(const FString& InKey, const TArray<uint8>& InByte)
		:Key(InKey), ArrayByte(InByte) {};
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
	void DeleteItem(const FString& Key, const FVoidHandler OnDone, const FString& Filename = TEXT("DefaultFileName")) override;

	/**
	 * @brief Insert Item to a file by overwrite operation.
	 *
	 * @param Key The Key of the Item.
	 * @param Item The Data to be inserted to the Table. The Data would be a FString.
	 * @param OnDone This will be called when the operation done. The result is bool.
	 * @param Filename optional. The name of the table. Default will insert an item to the default KeyValue table.
	*/
	void SaveItemOverwiteEntireFile(FString Key, FString Item, const THandler<bool>& OnDone, const FString& Filename = TEXT("DefaultFileName")) override;

	/**
		* @brief Insert Item to the Key Value file.
		*
		* @param Key The Key of the Item.
		* @param Item The Data to be inserted to the file. The Data would be an array of uint8.
		* @param OnDone This will be called when the operation done. The result is bool.
		* @param FileName optional. The name of the file. Default will insert an item to the default storage file.
	*/
	void SaveItem(const FString& Key, const TArray<uint8>& Item, const THandler<bool>& OnDone, const FString& Filename = TEXT("DefaultFileName")) override;

	/**
		* @brief Insert Item to the Key Value file.
		*
		* @param Key The Key of the Item.
		* @param Item The Data to be inserted to the file. The Data would be a FString.
		* @param OnDone This will be called when the operation done. The result is bool.
		* @param FileName optional. The name of the file. Default will insert an item to the default storage file.
	*/
	void SaveItem(const FString& Key, const FString& Item, const THandler<bool>& OnDone, const FString& Filename = TEXT("DefaultFileName")) override;

	/**
		* @brief Insert Item to the Key Value file.
		*
		* @param Key The Key of the Item.
		* @param Item The Data to be inserted to the file. The Data would be a FJsonObjectWrapper.
		* @param OnDone This will be called when the operation done. The result is bool.
		* @param FileName optional. The name of the file. Default will insert an item to the default storage file.
	*/
	void SaveItem(const FString& Key, const FJsonObjectWrapper& Item, const THandler<bool>& OnDone, const FString& Filename = TEXT("DefaultFileName")) override;

	/**
		* @brief Get an Item from the Key Value file.
		*
		* @param Key The Key of the Item to Get.
		* @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and an array of uint8 Value.
		* @param FileName optional. The name of the file. Default will get an item from the default storage file.
	*/
	void GetItem(const FString& Key, const THandler<TPair<FString, TArray<uint8>>>& OnDone, const FString& Filename = TEXT("DefaultFileName")) override;

	/**
		* @brief Get an Item from the Key Value file.
		*
		* @param Key The Key of the Item to Get.
		* @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and a FString Value.
		* @param FileName optional. The name of the file. Default will get an item from the default storage file.
	*/
	void GetItem(const FString& Key, const THandler<TPair<FString, FString>>& OnDone, const FString& Filename = TEXT("DefaultFileName")) override;

	/**
		* @brief Get an Item from the Key Value file.
		*
		* @param Key The Key of the Item to Get.
		* @param OnDone This will be called when the operation done. The result is Pair of a FString Key, and a FJsonObjectWrapper Value.
		* @param FileName optional. The name of the file. Default will get an item from the default storage file.
	*/
	void GetItem(const FString& Key, const THandler<TPair<FString, FJsonObjectWrapper>>& OnDone, const FString& Filename = TEXT("DefaultFileName")) override;

	/**
	* @brief Helper function
	*/
	FDirectoryPath GetAbsoluteFileDirectory();

	/**
	* @brief Change the old JSON-Array structure into new-line (\n) separated JSON.
	* Telemetry will be extracted from the old cache into a Telemetry specific cache.
	* DeviceID & refresh token will be extracted as well into a GeneralPurpose cache.
	* 
	* @param OldCacheFilename The targeted file that will be extracted
	* @param NewCacheFilenameForTelemetry
	* @param NewCacheFilenameForGeneralPurpose
	*/
	void ConvertExistingCache(const FString& OldCacheFilename, const FString& NewCacheFilenameForTelemetry, const FString& NewCacheFilenameForGeneralPurpose) override;

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
	* @brief Serialize an Array of Struct manually due to UE limitation
	* Using Newline as delimiter
	* 
	* @param Pointer to the structure that will be serialized
	*/
	virtual FString FABBinaryFileStructureToString(FABBinaryFileStructure* Structure);

	/**
	* @brief Read file content and deserialize it 
	*
	* @param FileName The name of the file, not the absolute path.
	* @return The deserialization result. 
	*/
	virtual TSharedPtr<FABBinaryFileStructure> ParseStructureFromFile(const FString& FileName);

	/**
	* @brief Deserialize string into an array string of FABBinaryFileStructure
	*
	* @param TargetedString The string that will be deserialized.
	* @return The deserialization result. 
	*/
	virtual TSharedPtr<FABBinaryFileStructure> ParseStructureOnly(const FString& TargetedString);

	/**
	* @brief Check the existing file within the file storage directory.
	*/
	virtual bool IsFileExist(const FString& FileName);

	/**
	* @brief Convert the old cache format into new cache format.
	* 
	* @param Input The loaded old cache
	* @return The deserialized cache in a new format
	*/
	virtual FABBinaryFileStructure ConvertOldCacheToNewFormat(const FString& Input);

	/**
	* @brief Migrate the value from the converted old cache into each new files.
	*
	* @param CacheContent Cache that will be stored into new files.
	* @param NewTelemetryCacheFilename 
	* @param NewGenerelPurposeCacheFilename 
	*/
	virtual void MoveOldCacheToNewCacheFiles(const FABBinaryFileStructure& CacheContent, const FString& NewTelemetryCacheFilename, const FString& NewGenerelPurposeCacheFilename);
};

}
