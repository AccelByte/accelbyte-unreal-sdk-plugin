// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

namespace AccelByte
{
namespace Core
{

template <typename T>
struct FAccelByteCacheWrapper
{
	FName Key{};
	TSharedPtr<T> Data = nullptr;
	size_t Length = 0;
};

template <typename T>
class FAccelByteLRUCache
{
protected:
	size_t MAX_HTTP_LRU_CACHE_SIZE = 20 * 1024 * 1024;
	int32 MAX_HTTP_LRU_CACHE_COUNT = 100;

	bool bIsInitialized = false;

	TArray<FAccelByteCacheWrapper<T>> ChunkArray;
	TDoubleLinkedList<FAccelByteCacheWrapper<T>> ChunkDll;

#pragma region DOUBLE_LINKED_LIST
public:
	typename TDoubleLinkedList<FAccelByteCacheWrapper<T>>::TDoubleLinkedListNode* DLLGetTail() { return ChunkDll.GetTail(); }
	typename TDoubleLinkedList<FAccelByteCacheWrapper<T>>::TDoubleLinkedListNode* DLLGetHead() { return ChunkDll.GetHead(); }
private:
	void DLLSetEmpty() { ChunkDll.Empty(); }
	int DLLGetSize() { return ChunkDll.Num(); }
	void DLLAddHead(FAccelByteCacheWrapper<T> NewHead) { ChunkDll.AddHead(NewHead); }
	void DLLRemoveNode(typename TDoubleLinkedList<FAccelByteCacheWrapper<T>>::TDoubleLinkedListNode* Removed) { ChunkDll.RemoveNode(Removed); }
	void DLLRemoveNode(typename TDoubleLinkedList<FAccelByteCacheWrapper<T>>::TDoubleLinkedListNode* Removed, bool bDeleteNode) { ChunkDll.RemoveNode(Removed, bDeleteNode); }
#pragma endregion

#pragma region ARRAY_CONTAINER
protected:
	int ArrayGetNum() { return ChunkArray.Num(); }
	FAccelByteCacheWrapper<T>& ArrayGetIndex(int Index) { return ChunkArray[Index]; }
private:
	void ArraySetEmpty() { ChunkArray.Empty(); }
	void ArrayRemoveAt(int Index) { ChunkArray.RemoveAt(Index); }
	void ArrayAdd(const FAccelByteCacheWrapper<T>* Entry)
	{
		if (Entry == nullptr)
		{
			return;
		}
		ChunkArray.Add(*Entry);
	};
#pragma endregion

/**
* @brief Private function needs to be overriden
*/
protected:
#pragma region AbstractFunction
	/**
	* @brief LRUMemory: Clean the Memory class
	* @brief LRUFileCache: Cleanup the stored files
	*/
	virtual void FreeCache() = 0;
	
	/**
	* @brief LRUMemory: Clean memory specific key
	* @brief LRUFileCache: Cleanup specific file for this key
	*/
	virtual void RemoveCache(const FName& Key) = 0;

	/**
	* @brief Ensure that we can free up the space before we insert to cache
	* @return Possible to do insert operation
	*/
	virtual inline bool FreeCacheBeforeInsertion(T& Item) = 0;

	/**
	* @brief LRUMemory: Insert/replace to MemoryPool directly
	* @brief LRUFileCache: Insert/rewrite to the file directly
	* 
	* @return Pointer to the FAccelByteCacheWrapper (LRU Memory)
	*/
	virtual inline const FAccelByteCacheWrapper<T>* InsertToCache(T& Item, const FName& Key) = 0;

#pragma endregion

/**
* @brief Public function and accessed by HttpCache
*/
public:
	virtual ~FAccelByteLRUCache() {}

	/**
	* @brief Check if the LRU class stores an item with specified key
	*
	* @param Key Identifier of the data
	* @return True if the key is found
	*/
	inline bool Contains(const FName& Key)
	{
		return FindIndex(Key) >= 0;
	}

	/**
	* @brief Cleanup the LRU storage and the Memory class
	*/
	inline void Empty()
	{
		this->bIsInitialized = false;
		FreeCache();
		ArraySetEmpty();
		DLLSetEmpty();
	}

	/**
	* @brief Remove a data from the LRU class
	*
	* @param Key Identifier of the data
	* @return True if the deleted data exist
	* @return True if the removal success
	*/
	inline bool Remove(const FName& Key)
	{
		int Index = FindIndex(Key);
		auto* Node = DLLFindNode(Key);

		if (Index >= 0 && Node != nullptr)
		{
			RemoveCache(Key);
			ArrayRemoveAt(Index);
			DLLRemoveNode(Node, true);
			return true;
		}
		return false;
	}

	/**
	* @brief Write/replace an item with specified key
	*
	* @param Key Identifier of the data
	* @return True if the key is found
	*/
	inline bool Emplace(const FName& Key, T& Item)
	{
		//// Check and remove an existing key that will be overwritten
		Remove(Key);

		if (FreeCacheBeforeInsertion(Item) == false)
		{
			return false;
		}

		// After the existing removed, it can be added to the front
		const FAccelByteCacheWrapper<T>* Result = InsertToCache(Item, Key);
		if (Result == nullptr)
		{
			return false;
		}
		ArrayAdd(Result);
		DLLAddHead(*Result);
		return true;
	}

	/**
	* @brief Obtain and access the data from LRU class
	* This function will move the data to the 'head' as the latest/recently used data.
	*
	* @param Key Identifier of the data
	* @param bPeekOnly If TRUE, does not affect the order.
	* @return Pointer to the data. Nullptr if the data is not found.
	*/
	inline TSharedPtr<T> Find(const FName& Key, bool bPeekOnly = false)
	{
		int Index = FindIndex(Key);
		if (Index < 0) { return nullptr; }

		if (!bPeekOnly)
		{
			auto* Node = DLLFindNode(Key);
			if (Node == nullptr) { return nullptr; }

			// Swap the position
			DLLRemoveNode(Node);
			DLLAddHead(ArrayGetIndex(Index));
		}

		return GetTheValueFromChunkArray(Index);
	}

	inline TSharedPtr<T> operator[](const FName& Key)
	{
		return Find(Key);
	}

	//LRUCacheFile should override this function
	virtual inline TSharedPtr<T> GetTheValueFromChunkArray(int Index){ return ArrayGetIndex(Index).Data; }

	/**
	* @brief Check the value only, does not affect the order of the linked list
	*
	* @param Key Identifier of the data
	* @return Pointer to the data. Nullptr if the data is not found.
	*/
	inline TSharedPtr<T> Peek(const FName& Key) { return Find(Key, true); }

protected:

	/**
	* @brief Find the index of the ChunkArray based on the storage Key
	*
	* @param Key Identifier of the data
	* @return Index number if it exists, otherwise return -1.
	*/
	inline int FindIndex(const FName& Key)
	{
		int Num = ArrayGetNum();
		for (int i = 0; i < Num; i++)
		{
			if (ArrayGetIndex(i).Key.IsEqual(Key, ENameCase::CaseSensitive, true))
			{
				return i;
			}
		}
		return -1;
	}

	/**
	* @brief Find the pointer of the ChunkDLL's node based on the storage Key
	*
	* @param Key Identifier of the data
	* @return Pointer to node if it exists, otherwise return nullptr.
	*/
	virtual inline typename TDoubleLinkedList<FAccelByteCacheWrapper<T>>::TDoubleLinkedListNode* DLLFindNode(const FName& Key)
	{
		auto* Node = DLLGetHead();

		while (Node != nullptr)
		{
			if (Node->GetValue().Key == Key)
			{
				break;
			}

			Node = Node->GetNextNode();
		}

		return Node;
	}

public:

	inline static const size_t GetRequiredSize(T& Data) { return sizeof(Data); }
	inline static const size_t GetRequiredSize(T* Data) { return GetRequiredSize(*Data); }
};

// Override specific for HTTP response size
template<>
inline const size_t FAccelByteLRUCache<FHttpRequestPtr>::GetRequiredSize(FHttpRequestPtr& Data)
{
	size_t Output = 0;

	if (Data->GetResponse().IsValid())
	{
		Output += Data->GetResponse()->GetContentLength();
	}
	Output += Data->GetContentLength();
	return Output;
}

// Override specific for FString size
template<>
inline const size_t FAccelByteLRUCache<FString>::GetRequiredSize(FString& Data)
{
	FString TempCopy = Data;
	return TempCopy.GetAllocatedSize();
}

}
}

