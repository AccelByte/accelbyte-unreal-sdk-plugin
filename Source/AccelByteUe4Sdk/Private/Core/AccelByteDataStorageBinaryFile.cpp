// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteDataStorageBinaryFile.h"
#include "JsonUtilities.h"
#include "Misc/FileHelper.h"
#include "Containers/UnrealString.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteDataStorageBinaryFile, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteDataStorageBinaryFile);

namespace AccelByte
{
DataStorageBinaryFile::DataStorageBinaryFile(FString DirectoryPath)
{
	FDirectoryPath DirPath;
	DirPath.Path = FPaths::ConvertRelativePathToFull(DirectoryPath);
#if !(PLATFORM_WINDOWS)
	//This is the only directory that allow us to write. This hardcode is mandatory & unavoidable
	DirPath.Path = FPaths::ConvertRelativePathToFull(FPaths::ProjectLogDir());
#endif
	this->AbsoluteFileDirectory = DirPath;
}

void DataStorageBinaryFile::Reset(const THandler<bool>& Result, const FString & FileName)
{
	auto Value = TArray<uint8>();
	FString Path = CompleteAbsoluteFilePath(FileName);
	Result.ExecuteIfBound(FFileHelper::SaveArrayToFile(Value, *Path));
}

void DataStorageBinaryFile::DeleteItem(const FString & Key, const FVoidHandler OnDone, const FString & FileName)
{
	auto StructurePtr = ParseStructureFromFile(FileName);
	if (StructurePtr == nullptr)
	{
		OnDone.ExecuteIfBound();
	}

	StructurePtr->Segments.Remove(Key);

	FString SerializedText;
	FJsonObjectConverter::UStructToJsonObjectString<FBinaryFileStructure>(*StructurePtr.Get(), SerializedText);
	
	TArray<uint8> ByteArray;
	ByteArray.AddUninitialized(SerializedText.Len());
	StringToBytes(SerializedText, ByteArray.GetData(), SerializedText.Len());

	FString Path = CompleteAbsoluteFilePath(FileName);
	FFileHelper::SaveArrayToFile(ByteArray, *Path);
	OnDone.ExecuteIfBound();
}

void DataStorageBinaryFile::SaveItem(const FString & Key, const TArray<uint8>& Item, const THandler<bool>& OnDone, const FString & FileName)
{
	OnDone.ExecuteIfBound(SaveToFile(FileName, Key, Item));
}

void DataStorageBinaryFile::SaveItem(const FString & Key, const FString & Item, const THandler<bool>& OnDone, const FString & FileName)
{
	uint8* Output = new uint8[Item.Len()];
	TArray<uint8> ByteArray;
	ByteArray.AddUninitialized(Item.Len());
	StringToBytes(Item, ByteArray.GetData(), Item.Len());
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

	auto ValuePtr = StructurePtr->Segments.Find(Key);
	if (ValuePtr != nullptr && ValuePtr->Content.Num() > 0)
	{
		Result.Value = TArray<uint8>(ValuePtr->Content);
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

	auto ValuePtr = StructurePtr->Segments.Find(Key);
	if (ValuePtr != nullptr && ValuePtr->Content.Num() > 0)
	{
		Result.Value = BytesToString(&ValuePtr->Content[0], ValuePtr->Content.Num());
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

	auto ValuePtr = StructurePtr->Segments.Find(Key);
	if (ValuePtr != nullptr && ValuePtr->Content.Num() > 0)
	{
		FString Value = BytesToString(&ValuePtr->Content[0], ValuePtr->Content.Num());
		Result.Value.JsonObjectFromString(Value);
		Result.Key = Key;
	}

	OnDone.Execute(Result);
}

FString DataStorageBinaryFile::CompleteAbsoluteFilePath(const FString& FileName)
{
	return this->AbsoluteFileDirectory.Path + FileName;
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
	bool LoadOK = FFileHelper::LoadFileToArray(Result, *Path);
	if (LoadOK && Result.Num() > 0)
	{
		Output = BytesToString(&Result[0], Result.Num());
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
		StructurePtr = MakeShared<FBinaryFileStructure>();
		StructurePtr->Segments.Add(Key, FArrayByte(Value));
	}
	else 
	{
		//check existing key, modify if the key already exist
		auto ValuePtr = StructurePtr->Segments.Find(Key);
		if (ValuePtr == nullptr)
		{
			StructurePtr->Segments.Add(Key, FArrayByte(Value));
		}
		else
		{
			ValuePtr->Content = Value;
		}
	}

	FString SerializedText;
	FJsonObjectConverter::UStructToJsonObjectString<FBinaryFileStructure>(*StructurePtr.Get(), SerializedText);

	TArray<uint8> ByteArray;
	ByteArray.AddUninitialized(SerializedText.Len());
	StringToBytes(SerializedText, ByteArray.GetData(), SerializedText.Len());

	FString Path = CompleteAbsoluteFilePath(FileName);
	return FFileHelper::SaveArrayToFile(ByteArray, *Path);
}

TSharedPtr<FBinaryFileStructure> DataStorageBinaryFile::ParseStructureFromFile(const FString& FileName)
{
	TSharedPtr<FBinaryFileStructure> Collection = MakeShared<FBinaryFileStructure>();

	if (!IsFileExist(FileName))
	{
		return Collection;
	}

	auto LoadedString = LoadFromFile(FileName);
	if (!LoadedString.IsSet())
	{
		return Collection;
	}

	FJsonObjectConverter::JsonObjectStringToUStruct<FBinaryFileStructure>(LoadedString.GetValue(), Collection.Get(), 0, 0);
	return Collection;
}

bool DataStorageBinaryFile::IsFileExist(const FString& FileName)
{
	FString Path = CompleteAbsoluteFilePath(FileName);
	return FPaths::FileExists(Path);
}

}
