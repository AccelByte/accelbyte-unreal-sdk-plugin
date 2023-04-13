// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Core/AccelByteLRUCache.h"
#include "Core/AccelByteDataStorageBinaryFile.h"
#include "Core/AccelByteTypeConverter.h"
#include "Models/AccelByteGeneralModels.h"
#include "HAL/FileManager.h"
#include "HAL/FileManagerGeneric.h"

namespace AccelByte
{
namespace Core
{

template <typename T>
class FAccelByteLRUCacheFile : public FAccelByteLRUCache<T> 
{
public:
	const FString PREFIX_CACHE_NAME = TEXT("ACCELBYTE_LRU_CACHE");
	const FString EXTENSION_CACHE_NAME = TEXT(".cache");
	inline FAccelByteLRUCacheFile(){}

	inline FAccelByteLRUCacheFile(int32 MaxFileCount_, size_t MaxFileSizeBytes_)
	{
		MaxFileCount = MaxFileCount_;
		MaxFileSizeBytes = MaxFileSizeBytes_;
		FAccelByteLRUCacheFile();
	}

	inline ~FAccelByteLRUCacheFile(){}

private:
	int32 MaxFileCount = 100;
	int32 CurrentFileCount = 0;

	size_t MaxFileSizeBytes = 10 * 1024 * 1024;
	size_t CurrentFileSizeBytes = 0;
	
	DataStorageBinaryFile DataStorage;

	TArray<FAccelByteCacheWrapper<T>> DerivedChunkArray;

	/**
	* @brief Initialize the Storage
	*/
	inline void InitializeStorage()
	{
		this->bIsInitialized = true;
	}

	FString ConvertKeyToFilename(FString Key)
	{
		FString FileName = TEXT("");
		if (!Key.IsEmpty())
		{
			Key = FGenericPlatformHttp::UrlEncode(Key);
			FileName = PREFIX_CACHE_NAME + Key + EXTENSION_CACHE_NAME;
		}
		return FileName;
	}

	FString CompleteFilenameToAbsolute(const FString& Filename)
	{
		FDirectoryPath Directory = DataStorage.GetAbsoluteFileDirectory();
		return Directory.Path + Filename;
	}

	FString ConvertKeyToFilename(const FName& Key) { return ConvertKeyToFilename(Key.ToString()); }

	/**
	* @brief Get All files from data storage binary class directory
	* delete each file with specified prefix & suffix
	*/
	inline void FreeCache() override 
	{
#if FW_BUILD_ENGINE_CHANGE_MISC
	    if (IsEngineExitRequested())
	    {
	        return;
	    }
#endif	    
		FDirectoryPath Directory = DataStorage.GetAbsoluteFileDirectory();
		FString AbsoluteDirPath = Directory.Path;
		TArray<FString> Files;
		IFileManager::Get().FindFiles(Files, *AbsoluteDirPath, *EXTENSION_CACHE_NAME);

		for (int i = 0; i < Files.Num(); i++)
		{
			if (!Files[i].Contains(PREFIX_CACHE_NAME))
			{
				continue;
			}
			FString AbsFilePath = AbsoluteDirPath + Files[i];
			IFileManager::Get().Delete(*AbsFilePath, true, true, true);
		}

		CurrentFileCount = 0;
		CurrentFileSizeBytes = 0;
		DerivedChunkArray.Empty();
	}

	inline void RemoveCache(const FName& Key) override
	{
		auto Filename = ConvertKeyToFilename(Key);
		size_t CurrentSize = this->ArrayGetIndex(this->FindIndex(Key)).Length;
		
		IFileManager::Get().Delete(*Filename, true, true, true);
		CurrentFileCount -= 1;
		CurrentFileSizeBytes -= CurrentSize;

		for (int i = 0; i < DerivedChunkArray.Num(); i++)
		{
			if (DerivedChunkArray[i].Key == Key)
			{
				DerivedChunkArray.RemoveAt(i);
			}
		}

	}

	inline bool FreeCacheBeforeInsertion(T& Item) override
	{
		auto Required = FAccelByteLRUCache<T>::GetRequiredSize(Item);
		size_t Left = MaxFileSizeBytes - CurrentFileSizeBytes;

		bool bChunkCountIsSafe = CurrentFileCount < MaxFileCount;

		if (Left >= Required && bChunkCountIsSafe)
		{
			return true;
		}

		auto Node = this->DLLGetTail();
		while (Node != nullptr && (Required > Left || this->ArrayGetNum() >= MaxFileCount))
		{
			size_t TailSize = Node->GetValue().Length;
			this->Remove(this->DLLGetTail()->GetValue().Key);
			Left += TailSize;
			Node = this->DLLGetTail();
		}

		size_t ModifiedStorageSizeLeft = MaxFileSizeBytes - CurrentFileSizeBytes;
		return (Required <= ModifiedStorageSizeLeft) && (this->ArrayGetNum() < MaxFileCount);
	}

	inline const FAccelByteCacheWrapper<T>* InsertToCache(T& Item, const FName& Key) override
	{
		if (!InsertPrerequisiteOkay())
		{
			return nullptr;
		}
		
		auto Size = FAccelByteLRUCache<T>::GetRequiredSize(Item);
		FAccelByteCacheWrapper<T> Result{ Key, nullptr, Size};

		TArray<uint8> ArrayByte = ToArrayByte(Item);
		if (ArrayByte.Num() == 0)
		{
			return nullptr;
		}

		auto AbsPath = CompleteFilenameToAbsolute(ConvertKeyToFilename(Key));
		bool bIsSuccess = FFileHelper::SaveArrayToFile(ArrayByte, *AbsPath);
		
		if (!bIsSuccess) return nullptr;
		int Index = DerivedChunkArray.Add(Result);
		CurrentFileCount += 1;
		CurrentFileSizeBytes += Result.Length;

		return &DerivedChunkArray[Index];
	}

	inline bool InsertPrerequisiteOkay() 
	{
		if (CurrentFileCount >= MaxFileCount)
		{
			return false;
		}
		if (CurrentFileSizeBytes >= this->MaxFileSizeBytes)
		{
			return false;
		}
		return true;
	}
	
	inline const TArray<uint8> ToArrayByte(T& Item) { return TArray<uint8>(); }
	inline TSharedPtr<T> FromFString(const FString& Content) { return nullptr; };

	inline TSharedPtr<T> GetTheValueFromChunkArray(int Index) override
	{
		FAccelByteCacheWrapper<T>& ChunkInfo = this->ArrayGetIndex(Index);
		FString Key = ChunkInfo.Key.ToString();

		TArray<uint8> ArrayByte;
		auto AbsPath = CompleteFilenameToAbsolute(ConvertKeyToFilename(Key));
		bool bIsOK = FFileHelper::LoadFileToArray(ArrayByte, *AbsPath);
		if (!bIsOK || ArrayByte.Num() == 0) return nullptr;

		FString StringValue = FAccelByteArrayByteFStringConverter::BytesToFString(ArrayByte, false);

		TSharedPtr<T> Output = FromFString(StringValue);
		if (Output.IsValid())
		{
			ChunkInfo.Data = Output;
			return ChunkInfo.Data;
		}
		else
		{
			return nullptr;
		}
	}
};

#pragma region OVERRIDE_FString
template<>
inline const TArray<uint8> FAccelByteLRUCacheFile<FString>::ToArrayByte(FString& CacheItem)
{
	return FAccelByteArrayByteFStringConverter::FStringToBytes(CacheItem);
}

template<>
inline TSharedPtr<FString> FAccelByteLRUCacheFile<FString>::FromFString(const FString& Content) { return MakeShareable<FString>(new FString(Content)); };
#pragma endregion

// Override specific for AccelByteHttpCacheUsage
#pragma region OVERRIDE_FAccelByteHttpCacheItem
template<>
inline const TArray<uint8> FAccelByteLRUCacheFile<FAccelByteHttpCacheItem>::ToArrayByte(FAccelByteHttpCacheItem& CacheItem)
{
	FHttpRequestPtr RequestPtr = CacheItem.Request;
	TArray<uint8> Output;

	if (!RequestPtr.IsValid()) { return Output; }

	FAccelByteLRUHttpStruct Struct;
	Struct.SetMember(
		RequestPtr->GetAllHeaders(),
		RequestPtr->GetResponse()->GetAllHeaders(),
		RequestPtr->GetResponse()->GetResponseCode(),
		RequestPtr->GetURL(),
		RequestPtr->GetResponse()->GetContent(),
		CacheItem.ExpireTime);

	//TODO
	//switch (headers[mime-type)):
	//...
	//...

	FString SerializedText;
	if (!FJsonObjectConverter::UStructToJsonObjectString<FAccelByteLRUHttpStruct>(Struct, SerializedText))
	{
		return Output;
	}
	Output = FAccelByteArrayByteFStringConverter::FStringToBytes(SerializedText);
	return Output;
}

// Override specific for AccelByteHttpCacheUsage
template<>
inline TSharedPtr<FAccelByteHttpCacheItem> FAccelByteLRUCacheFile<FAccelByteHttpCacheItem>::FromFString(const FString& Content)
{
	FAccelByteLRUHttpStruct SerializeableHttpStruct;
	if (FJsonObjectConverter::JsonObjectStringToUStruct<FAccelByteLRUHttpStruct>(Content, &SerializeableHttpStruct, 0, 0) == false)
	{
		return nullptr;
	}

	auto HeaderSplitter = [this](const TArray<FString>& HeaderAndValuePair, TDelegate<void(FString /*Header*/, FString /*Value*/)> ActionSetHeader)
	{
		for (int i = 0 ; i < HeaderAndValuePair.Num(); i++)
		{
			const FString& HeaderAndValue = HeaderAndValuePair[i];
			FString Header, Value;
			HeaderAndValue.Split(":", &Header, &Value);

			Header = Header.TrimStartAndEnd();
			Value = Value.TrimStartAndEnd();

			ActionSetHeader.Execute(Header, Value);
		}
	};

	FHttpRequestPtr Request = MakeShareable<IHttpRequest>(FGenericPlatformHttp::ConstructRequest());
	// Cannot set response header at all
	FHttpResponsePtr Response = nullptr;

	Request->SetURL(SerializeableHttpStruct.RequestURL);
	HeaderSplitter(SerializeableHttpStruct.RequestHeaders, TDelegate<void(FString, FString)>::CreateLambda([&](FString Header, FString Value) { Request->SetHeader(Header, Value); }));

	TSharedPtr<FAccelByteHttpCacheItem> Output = MakeShareable<FAccelByteHttpCacheItem>(new FAccelByteHttpCacheItem);
	Output->SerializableRequestAndResponse = SerializeableHttpStruct;
	Output->Request = Request;
	Output->ExpireTime = double(FCString::Atof(*SerializeableHttpStruct.ExpireTime));

	return Output;
}
#pragma endregion

}
}
