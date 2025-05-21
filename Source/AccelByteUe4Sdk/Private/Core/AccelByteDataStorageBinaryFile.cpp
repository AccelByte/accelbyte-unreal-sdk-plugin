// Copyright (c) 2022-2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteDataStorageBinaryFile.h"
#include "Core/AccelByteUtilities.h"
#include "JsonUtilities.h"
#include "Misc/FileHelper.h"
#include "Containers/UnrealString.h"
#if PLATFORM_SWITCH
#include "SwitchFileSystem.h"
#include "SwitchPathManager.h"
#endif

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteDataStorageBinaryFile, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteDataStorageBinaryFile);

namespace AccelByte
{
	
bool DataStorageBinaryFile::CopyFile(const FString& FileName, const FString& DestinationAbsoluteDirPath, const FString& SourceAbsoluteDirPath)
{
	const FString SourceFilePath = SourceAbsoluteDirPath + FileName;
	const FString DestFilePath = DestinationAbsoluteDirPath + FileName;

	// make sure no file in new dir, and exist file in old dir
	if(!FPaths::FileExists(DestFilePath) && FPaths::FileExists(SourceFilePath))
	{
		TArray64<uint8> FileContent;
		bool bLoadSuccess = false; 
		
		{
			FReadScopeLock ReadLock(FileAccessLock);
			bLoadSuccess = FFileHelper::LoadFileToArray(FileContent, *SourceFilePath);
		}

		if (bLoadSuccess)
		{
			FWriteScopeLock WriteLock(FileAccessLock);
			bool bSuccess = FFileHelper::SaveArrayToFile(FileContent, *DestFilePath);
			return bSuccess;
		}
	}

	return false;
}

bool DataStorageBinaryFile::MigrateStorageFiles(const FDirectoryPath& SourceDirectory, const FDirectoryPath& DestinationDirectory)
{
	auto DestinedABGeneralCachePath = DestinationDirectory.Path + FAccelByteUtilities::GetCacheFilenameGeneralPurpose();
	auto bCacheHasBeenMigrated = FPaths::FileExists(DestinedABGeneralCachePath);

	// If the AccelByteGeneralCache already EXIST in the destined directory, NO need to migrate
	if (bCacheHasBeenMigrated)
	{
		return true;
	}

	auto ABGeneralCacheOld = SourceDirectory.Path + FAccelByteUtilities::GetCacheFilenameGeneralPurpose();
	auto bSourceCacheExist = FPaths::FileExists(ABGeneralCacheOld);

	// If the source cache does not exist, the no need to migrate. Straightaway create new cache
	if (!bSourceCacheExist)
	{
		return true;
	}

	const FString SourceAbsoluteDirPath = FPaths::ConvertRelativePathToFull(SourceDirectory.Path);
	const FString DestinationAbsoluteDirPath = FPaths::ConvertRelativePathToFull(DestinationDirectory.Path);

	const FString GeneralFileName = FAccelByteUtilities::GetCacheFilenameGeneralPurpose();
	const FString TelemetryFileName = FAccelByteUtilities::GetCacheFilenameTelemetry();
	
	CopyFile(GeneralFileName, DestinationAbsoluteDirPath, SourceAbsoluteDirPath);
	CopyFile(TelemetryFileName, DestinationAbsoluteDirPath, SourceAbsoluteDirPath);

	auto MigratedResultFilePath = DestinationDirectory.Path + GeneralFileName;
	return FPaths::FileExists(MigratedResultFilePath);
}

DataStorageBinaryFile::DataStorageBinaryFile(FString DirectoryPath)
{
	FDirectoryPath DirPath;
	DirPath.Path = DirectoryPath;
	this->RelativeFileDirectory = DirPath;
#if PLATFORM_SWITCH
	if(!FSwitchFileSystem::IsCacheStorageMounted())
	{
		FSwitchFileSystem::MountCacheStorage();
	}
#endif

	// Cache migration performed for Editor-only
#if defined(UE_EDITOR) && UE_EDITOR

	FDirectoryPath ProjectContentDirectory;
	ProjectContentDirectory.Path = FPaths::ProjectContentDir();
	
	FDirectoryPath TargetDirectory = GetAbsoluteFileDirectory();

	bool bMigrateOk = MigrateStorageFiles(ProjectContentDirectory, TargetDirectory);

	// If not success, FALLBACK/revert to use the old content directory
	if (!bMigrateOk)
	{
		this->RelativeFileDirectory = ProjectContentDirectory;
	}
#endif
}

void DataStorageBinaryFile::Reset(const THandler<bool>& Result, const FString & FileName)
{
	auto Value = TArray<uint8>();
	FString Path = CompleteAbsoluteFilePath(FileName);

	{
		FWriteScopeLock WriteLock(FileAccessLock);
		Result.ExecuteIfBound(FFileHelper::SaveArrayToFile(Value, *Path));
	}
}

FString DataStorageBinaryFile::FABBinaryFileStructureToString(FABBinaryFileStructure* Structure)
{
	if (Structure == nullptr)
	{
		return "";
	}
	FString Value{};
	for (int i = 0; i < Structure->Num(); i++)
	{
		FString SerializedText{};
		FJsonObjectConverter::UStructToJsonObjectString<FBinaryContentIndependentSegment>(Structure->operator[](i), SerializedText, 0, 0, 0, nullptr, false);
		Value += SerializedText + "\n";
	}
	Value.TrimEndInline();
	return Value;
}

void DataStorageBinaryFile::DeleteItem(const FString & Key, const FVoidHandler OnDone, const FString & FileName)
{
	auto StructurePtr = ParseStructureFromFile(FileName);
	if (StructurePtr == nullptr)
	{
		OnDone.ExecuteIfBound();
	}

	auto Index = StructurePtr->IndexOfByPredicate([&](FBinaryContentIndependentSegment Segment) { return Segment.Key == Key; });
	if (Index < 0)
	{
		OnDone.ExecuteIfBound();
		return;
	}
	StructurePtr->RemoveAt(Index);

	FString SerializedText = FABBinaryFileStructureToString(StructurePtr.Get());
	
	TArray<uint8> ByteArray = FAccelByteArrayByteFStringConverter::FStringToBytes(SerializedText);

	FString Path = CompleteAbsoluteFilePath(FileName);

	{
		FWriteScopeLock WriteLock(FileAccessLock);
		FFileHelper::SaveArrayToFile(ByteArray, *Path);
	}

	OnDone.ExecuteIfBound();
}

//Specific for telemery optimization
void DataStorageBinaryFile::SaveItemOverwiteEntireFile(FString Key, FString Item, const THandler<bool>& OnDone, const FString& FileName)
{
	TArray<uint8> ContentByteArray = FAccelByteArrayByteFStringConverter::FStringToBytes(Item);

	FABBinaryFileStructure FileCacheStructure = FABBinaryFileStructure();
	FileCacheStructure.Add({ Key, ContentByteArray });

	FString SerializedText{};
	{
		//Manual serialization
		FString ArrayByteAsString{};
		for (int i = 0; i < ContentByteArray.Num(); i++)
		{
			ArrayByteAsString += FString::Printf(TEXT("%d"), ContentByteArray[i]);
			if (i < ContentByteArray.Num() - 1)
			{
				ArrayByteAsString += ",";
			}
		}
		SerializedText = FString::Printf(TEXT("{\"Key\": \"%s\", \"ArrayByte\": [%s]}"), *Key, *ArrayByteAsString);
	}

	TArray<uint8> FileByteArray = FAccelByteArrayByteFStringConverter::FStringToBytes(SerializedText);

	FString Path = CompleteAbsoluteFilePath(FileName);

	bool bSuccess = false;
	{
		FWriteScopeLock WriteLock(FileAccessLock);
		bSuccess = FFileHelper::SaveArrayToFile(FileByteArray, *Path);
	}

	OnDone.ExecuteIfBound(bSuccess);
}

void DataStorageBinaryFile::SaveItem(const FString & Key, const TArray<uint8>& Item, const THandler<bool>& OnDone, const FString & FileName)
{
	OnDone.ExecuteIfBound(SaveToFile(FileName, Key, Item));
}

void DataStorageBinaryFile::SaveItem(const FString & Key, const FString & Item, const THandler<bool>& OnDone, const FString & FileName)
{
	TArray<uint8> ByteArray = FAccelByteArrayByteFStringConverter::FStringToBytes(Item);
	OnDone.ExecuteIfBound(SaveToFile(FileName, Key, ByteArray));
}

void DataStorageBinaryFile::SaveItem(const FString & Key, const FJsonObjectWrapper & Item, const THandler<bool>& OnDone, const FString & FileName)
{
	FString ItemAsString = Item.JsonString;

	if (Item.JsonString.IsEmpty() && !Item.JsonObjectToString(ItemAsString))
	{
		OnDone.ExecuteIfBound(false);
		return;
	}

	SaveItem(Key, ItemAsString, OnDone, FileName);
}

void DataStorageBinaryFile::GetItem(const FString & Key, const THandler<TPair<FString, TArray<uint8>>>& OnDone, const FString & FileName)
{
	TPair<FString, TArray<uint8>> Result;

	auto StructurePtr = ParseStructureFromFile(FileName);
	if (StructurePtr == nullptr)
	{
		OnDone.Execute(Result);
	}

	auto ValuePtr = StructurePtr->FindByPredicate([&](FBinaryContentIndependentSegment Segment) { return Segment.Key == Key; });
	if (ValuePtr != nullptr && ValuePtr->ArrayByte.Num() > 0)
	{
		Result.Value = TArray<uint8>(ValuePtr->ArrayByte);
		Result.Key = Key;
	}
	OnDone.Execute(Result);
}

void DataStorageBinaryFile::GetItem(const FString & Key, const THandler<TPair<FString, FString>>& OnDone, const FString & FileName)
{
	TPair<FString, FString> Result;

	auto StructurePtr = ParseStructureFromFile(FileName);
	if (StructurePtr == nullptr)
	{
		OnDone.Execute(Result);
	}

	auto ValuePtr = StructurePtr->FindByPredicate([&](FBinaryContentIndependentSegment Segment) { return Segment.Key == Key; });
	if (ValuePtr != nullptr && ValuePtr->ArrayByte.Num() > 0)
	{
		Result.Value = FAccelByteArrayByteFStringConverter::BytesToFString(ValuePtr->ArrayByte, false);
		Result.Key = Key;
	}
	OnDone.Execute(Result);
}

void DataStorageBinaryFile::GetItem(const FString & Key, const THandler<TPair<FString, FJsonObjectWrapper>>& OnDone, const FString & FileName)
{
	TPair<FString, FJsonObjectWrapper> Result;

	auto StructurePtr = ParseStructureFromFile(FileName);
	if (StructurePtr == nullptr)
	{
		OnDone.Execute(Result);
	}

	auto ValuePtr = StructurePtr->FindByPredicate([&](FBinaryContentIndependentSegment Segment) { return Segment.Key == Key; });
	if (ValuePtr != nullptr && ValuePtr->ArrayByte.Num() > 0)
	{
		FString Value = FAccelByteArrayByteFStringConverter::BytesToFString(ValuePtr->ArrayByte, false);
		Result.Value.JsonObjectFromString(Value);
		Result.Key = Key;
	}

	OnDone.Execute(Result);
}

FDirectoryPath DataStorageBinaryFile::GetAbsoluteFileDirectory()
{
	FDirectoryPath DirPath;
#if PLATFORM_SWITCH
	DirPath.Path = this->RelativeFileDirectory.Path;
	if(!FSwitchPathManager::DoesPathIncludeMountPoint(*DirPath.Path))
	{
		FString MountPoint = TEXT("cache:");
		DirPath.Path = MountPoint / DirPath.Path;
	}
#elif !(PLATFORM_WINDOWS) || UE_BUILD_SHIPPING
	//This is the only directory that allow us to write. This hardcode is mandatory & unavoidable
	DirPath.Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectLogDir());
#else
	DirPath.Path = FPaths::ConvertRelativePathToFull(this->RelativeFileDirectory.Path);
#endif
	return DirPath;
}

FString DataStorageBinaryFile::CompleteAbsoluteFilePath(const FString& FileName)
{
	return GetAbsoluteFileDirectory().Path + FileName;
}

TOptional<FString> DataStorageBinaryFile::LoadFromFile(const FString& FileName)
{
	TOptional<FString> Output;

	if (!IsFileExist(FileName))
	{
		return Output;
	}

	FString Path = CompleteAbsoluteFilePath(FileName);

	TArray<uint8> Result;
	bool LoadOK = false;
	{
		FReadScopeLock ReadLock(FileAccessLock);
		LoadOK = FFileHelper::LoadFileToArray(Result, *Path);
	}

	if (LoadOK && Result.Num() > 0)
	{
		Output = FAccelByteArrayByteFStringConverter::BytesToFString(Result, false);
	}
	return Output;
}

bool DataStorageBinaryFile::SaveToFile(const FString& FileName, const FString& Key, const TArray<uint8>& Value)
{
	auto StructurePtr = ParseStructureFromFile(FileName);

	//file not exists
	if (StructurePtr == nullptr) 
	{
		//create new structure and add Key:Value
		StructurePtr = MakeShared<FABBinaryFileStructure>();
		StructurePtr->Add({ Key, Value });
	}
	else 
	{
		auto ValuePtr = StructurePtr->FindByPredicate([&](FBinaryContentIndependentSegment Segment) { return Segment.Key == Key; });
		if (ValuePtr == nullptr)
		{
			StructurePtr->Add({ Key, Value });
		}
		else
		{
			ValuePtr->ArrayByte = Value;
		}
	}

	FString SerializedText = FABBinaryFileStructureToString(StructurePtr.Get());

	TArray<uint8> ByteArray = FAccelByteArrayByteFStringConverter::FStringToBytes(SerializedText);

	FString Path = CompleteAbsoluteFilePath(FileName);
	bool bSuccess = false;
	{
		FWriteScopeLock WriteLock(FileAccessLock);
		bSuccess = FFileHelper::SaveArrayToFile(ByteArray, *Path);
	}

	return bSuccess;
}

TSharedPtr<FABBinaryFileStructure> DataStorageBinaryFile::ParseStructureFromFile(const FString& FileName)
{
	TSharedPtr<FABBinaryFileStructure> Collection = MakeShared<FABBinaryFileStructure>();

	if (!IsFileExist(FileName))
	{
		return Collection;
	}

	auto LoadedString = LoadFromFile(FileName);
	if (!LoadedString.IsSet())
	{
		return Collection;
	}

	return ParseStructureOnly(LoadedString.GetValue());
}

TSharedPtr<FABBinaryFileStructure> DataStorageBinaryFile::ParseStructureOnly(const FString& LoadedString)
{
	TSharedPtr<FABBinaryFileStructure> Collection = MakeShared<FABBinaryFileStructure>();
	TArray<FString> ParsedAsArray{};
	auto Count = LoadedString.ParseIntoArray(ParsedAsArray, TEXT("\n"));
	for (int i = 0; i < Count; i++)
	{
		FBinaryContentIndependentSegment Segment{};
		if (FJsonObjectConverter::JsonObjectStringToUStruct<FBinaryContentIndependentSegment>(ParsedAsArray[i], &Segment, 0, 0))
		{
			Collection->Add(Segment);
		}
	}
	return Collection;
}

bool DataStorageBinaryFile::IsFileExist(const FString& FileName)
{
	FString Path = CompleteAbsoluteFilePath(FileName);
	return FPaths::FileExists(Path);
}

FABBinaryFileStructure DataStorageBinaryFile::ConvertOldCacheToNewFormat(const FString& Input)
{
	FABBinaryFileStructure NewFormat{};
	FAccelByteBinaryFileStructureObsolete TemporaryOldCacheContainer{};
	bool bDeserializationSuccess = FJsonObjectConverter::JsonObjectStringToUStruct<FAccelByteBinaryFileStructureObsolete>(Input, &TemporaryOldCacheContainer);
	if (!bDeserializationSuccess)
	{
		return NewFormat;
	}

	TArray<FString> OldCachedKeys{};
	TemporaryOldCacheContainer.Segments.GetKeys(OldCachedKeys);
	for (int i = 0; i < OldCachedKeys.Num(); i++)
	{
		FString& CurrentKey = OldCachedKeys[i];
		if (!TemporaryOldCacheContainer.Segments.Contains(CurrentKey))
		{
			continue;
		}
		TArray<uint8>& CurrentValue = TemporaryOldCacheContainer.Segments[OldCachedKeys[i]].Content;
		if (CurrentValue.Num() == 0)
		{
			continue;
		}
		FBinaryContentIndependentSegment NewSegment(CurrentKey, CurrentValue);
		NewFormat.Add(NewSegment);
	}
	return NewFormat;
}

void DataStorageBinaryFile::MoveOldCacheToNewCacheFiles(const FABBinaryFileStructure& CacheContent, const FString& TelemetryCacheFilename, const FString& GenerelPurposeCacheFilename)
{
	for (int i = 0; i < CacheContent.Num(); i++)
	{
		//1.Extract DeviceID
		if (CacheContent[i].Key.Contains(FAccelByteUtilities::AccelByteStoredKeyDeviceId()))
		{
			SaveItem(CacheContent[i].Key, CacheContent[i].ArrayByte, THandler<bool>::CreateLambda([](const bool& Result){}),
				GenerelPurposeCacheFilename);
			continue;
		}

		//2.Extract Telemetry
		if (CacheContent[i].Key.Contains("TELEMETRY"))
		{
			SaveItem(CacheContent[i].Key, CacheContent[i].ArrayByte, THandler<bool>::CreateLambda([](const bool& Result){}),
				TelemetryCacheFilename);
			continue;
		}

		//else, might be PlatformUserID for token caching, leave it as it is in the original file
		SaveItem(CacheContent[i].Key, CacheContent[i].ArrayByte, THandler<bool>::CreateLambda([](const bool&) {}),
			GenerelPurposeCacheFilename);
		continue;
	}
}

void DataStorageBinaryFile::ConvertExistingCache(const FString& OldCacheFilename, const FString& NewCacheFilenameForTelemetry, const FString& NewCacheFilenameForGeneralPurpose)
{
	auto LoadedOldCache = LoadFromFile(OldCacheFilename);
	//If new cache format is found, no need to do a migration
	//If OldCache exist but new cache not found, then execute migration
	if (!LoadedOldCache.IsSet() || IsFileExist(NewCacheFilenameForGeneralPurpose))
	{
		return;
	}
	auto ParsedOldCache = ConvertOldCacheToNewFormat(LoadedOldCache.GetValue());
	if (ParsedOldCache.Num() == 0)
	{
		return;
	}
	MoveOldCacheToNewCacheFiles(ParsedOldCache, NewCacheFilenameForTelemetry, NewCacheFilenameForGeneralPurpose);
}
}
