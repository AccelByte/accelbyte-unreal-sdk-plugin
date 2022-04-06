// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"
#include "Core/AccelByteMemoryPool.h"

namespace AccelByte
{
namespace Core
{

template <typename T>
class FAccelByteLRUCache
{
	size_t MAX_HTTP_LRU_CACHE_SIZE = 20 * 1024 * 1024;
	int32 MAX_HTTP_LRU_CACHE_COUNT = 100;
public:
	inline FAccelByteLRUCache()
	{
		InitializeMemory();
	}

	inline FAccelByteLRUCache(MemoryConstructionParameter Param): MemoryParameter(Param)
	{
		InitializeMemory();
	}

	inline ~FAccelByteLRUCache()
	{
		FreeMemory();
	}

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
		FreeMemory();
		ChunkList.Empty();
		ChunkDll.Empty();
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
		auto* Node = FindNode(Key);

		if (Index >= 0 && Node != nullptr)
		{
			Memory->Remove(Key);
			ChunkList.RemoveAt(Index);
			ChunkDll.RemoveNode(Node, true);
			return true;
		}
		return false;
	}

	/**
	* @brief Check if the LRU class stores an item with specified key
	*
	* @param Key Identifier of the data
	* @return True if the key is found
	*/
	inline bool Emplace(const FName& Key, T& Item)
	{
		auto FreeMemoryBeforeInsertion = [this](T& Item)
		{
			auto Required = Memory->GetRequiredMemorySize(Item);
			size_t Left = Memory->GetMemoryPoolLeft();

			bool bChunkCountIsSafe = ChunkList.Num() < MemoryParameter.ChunkCount;

			if (Left >= Required && bChunkCountIsSafe)
			{
				return true;
			}

			auto Node = ChunkDll.GetTail();
			while (Node != nullptr && (Required > Left || ChunkList.Num() >= MemoryParameter.ChunkCount))
			{
				size_t TailSize = Node->GetValue().Length;
				Remove(ChunkDll.GetTail()->GetValue().Key);
				Left += TailSize;
				Node = ChunkDll.GetTail();
			}

			return (Memory->GetRequiredMemorySize(Item) <= Memory->GetMemoryPoolLeft()) && (ChunkList.Num() < MemoryParameter.ChunkCount);
		};

		auto AddToHead = [this](T& Item, const FName& Key)
		{
			auto Result = Memory->Insert(Item, Key);
			if (Result == nullptr)
			{
				return false;
			}
			ChunkList.Add(*Result);
			ChunkDll.AddHead(*Result);
			return true;
		};

		// Check and remove an existing key that will be overwritten
		for (int i = 0; i < ChunkList.Num(); i++)
		{
			if (ChunkList[i].Key.IsEqual(Key, ENameCase::CaseSensitive, true))
			{
				int Index = FindIndex(Key);
				auto* Node = FindNode(Key);
				if (Node == nullptr || Index == -1)
				{
					break;
				}
				ChunkDll.RemoveNode(Node, true);
				ChunkList.RemoveAt(Index);
			}
		}
		if (FreeMemoryBeforeInsertion(Item) == false)
		{
			return false;
		}
		if (AddToHead(Item, Key) == false)
		{
			return false;
		}
		return true;
	}

	/**
	* @brief Obtain and access the data from LRU class
	* This function will move the data to the 'head' as the latest/recently used data.
	*
	* @param Key Identifier of the data
	* @return Pointer to the data. Nullptr if the data is not found.
	*/
	inline T* Find(const FName& Key)
	{
		int Index = FindIndex(Key);
		auto* Node = FindNode(Key);
		if (Index >= 0 && Node != nullptr)
		{
			// Swap the position
			ChunkDll.RemoveNode(Node);
			ChunkDll.AddHead(ChunkList[Index]);

			return &ChunkList[Index].Data;
		}

		return nullptr;
	}

	inline T &operator[](const FName& Key){ return *Find(Key); }

	/**
	* @brief Check the value only, does not affect the order of the linked list
	*
	* @param Key Identifier of the data
	* @return Pointer to the data. Nullptr if the data is not found.
	*/
	inline T* Peek(const FName& Key)
	{
		int Index = FindIndex(Key);
		if (Index >= 0)
		{
			return &ChunkList[Index].Data;
		}
		return nullptr;
	}

	/**
	* @brief Get the pointer to the latest data in the list
	*/
	inline typename TDoubleLinkedList<FChunkInfo<T>>::TDoubleLinkedListNode* GetHead() { return ChunkDll.GetHead(); }
	
private:
	/**
	* @brief Initialize the Memory class
	*/
	inline void InitializeMemory()
	{
		Memory = FAccelByteMemoryFactory<T>::Create(MemoryParameter);
		bIsInitialized = true;
	}

	/**
	* @brief Clean the Memory class
	*/
	inline void FreeMemory()
	{
		bIsInitialized = false;
		Memory->RemoveAll();
	}

	/**
	* @brief Find the index of the ChunkList based on the storage Key
	*
	* @param Key Identifier of the data
	* @return Index number if it exists, otherwise return -1.
	*/
	inline int FindIndex(const FName& Key)
	{
		for (int i = 0; i < ChunkList.Num(); i++)
		{
			if (ChunkList[i].Key.IsEqual(Key, ENameCase::CaseSensitive, true))
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
	inline typename TDoubleLinkedList<FChunkInfo<T>>::TDoubleLinkedListNode* FindNode(const FName& Key)
	{
		auto* Node = ChunkDll.GetHead();

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


	MemoryConstructionParameter MemoryParameter = { MemoryMethod::Dynamic, MAX_HTTP_LRU_CACHE_SIZE, MAX_HTTP_LRU_CACHE_COUNT };
	TSharedPtr<FAccelByteMemory<T>> Memory;

	TArray<FChunkInfo<T>> ChunkList;
	TDoubleLinkedList<FChunkInfo<T>> ChunkDll;
	bool bIsInitialized = false;
};

}
}
