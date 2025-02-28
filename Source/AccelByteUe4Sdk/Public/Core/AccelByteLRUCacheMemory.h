// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/List.h"
#include "Core/AccelByteLRUCache.h"
#include "Core/AccelByteMemoryPool.h"

namespace AccelByte
{
namespace Core
{

template <typename T>
class FAccelByteLRUCacheMemory : public FAccelByteLRUCache<T>
{
public:
	inline FAccelByteLRUCacheMemory()
	{
		InitializeMemory();
	}

	inline FAccelByteLRUCacheMemory(MemoryConstructionParameter Param)
	{
		MemoryParameter = Param;
		FAccelByteLRUCacheMemory();
	}

	inline ~FAccelByteLRUCacheMemory()
	{
		FreeCache();
	}

private:
	/**
	* @brief Initialize the Memory class
	*/
	inline void InitializeMemory()
	{
		this->Memory = FAccelByteMemoryFactory<T>::Create(MemoryParameter);
		this->bIsInitialized = true;
	}

	inline void FreeCache() override 
	{
		if (Memory == nullptr) { InitializeMemory(); }
		Memory->RemoveAll();
	}

	inline void RemoveCache(const FName& Key) override
	{
		if (Memory == nullptr) { InitializeMemory(); }
		Memory->Remove(Key);
	}

	inline bool FreeCacheBeforeInsertion(T& Item) override
	{
		if (Memory == nullptr) { InitializeMemory(); }

		auto Required = this->GetRequiredSize(Item);
		size_t Left = Memory->GetMemoryPoolLeft();

		bool bChunkCountIsSafe = this->ArrayGetNum() < MemoryParameter.ChunkCount;

		if (Left >= Required && bChunkCountIsSafe)
		{
			return true;
		}

		auto Node = this->DLLGetTail();
		while (Node != nullptr && (Required > Left || this->ArrayGetNum() >= MemoryParameter.ChunkCount))
		{
			size_t TailSize = Node->GetValue().Length;
			this->Remove(this->DLLGetTail()->GetValue().Key);
			Left += TailSize;
			Node = this->DLLGetTail();
		}

		return (this->GetRequiredSize(Item) <= Memory->GetMemoryPoolLeft()) && (this->ArrayGetNum() < MemoryParameter.ChunkCount);
	}

	inline const FAccelByteCacheWrapper<T>* InsertToCache(T& Item, const FName& Key) override
	{
		if (Memory == nullptr) { InitializeMemory(); }
		const FChunkInfo<T>* InsertResult = (Memory->Insert(Item, Key));
		return static_cast<const FAccelByteCacheWrapper<T>*>(InsertResult);
	}

	// Not applicable
	inline bool ForcefullyPopulateChunkArrayFromStorage() override
	{
		return false;
	}

	inline TSharedPtr<T> GetTheValueFromChunkArray(int Index) override { return this->ArrayGetIndex(Index).Data; }

	TSharedPtr<FAccelByteMemory<T>> Memory;
	MemoryConstructionParameter MemoryParameter = { MemoryMethod::Dynamic, this->MAX_HTTP_LRU_CACHE_SIZE, this->MAX_HTTP_LRU_CACHE_COUNT };
};
}
}
