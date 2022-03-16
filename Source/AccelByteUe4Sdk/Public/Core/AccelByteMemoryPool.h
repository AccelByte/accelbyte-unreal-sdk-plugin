// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "HttpManager.h"
#include "Interfaces/IHttpResponse.h"

namespace AccelByte
{
namespace Core
{

template<typename T>
class FAccelByteMemory;
template<typename T>
class FAccelByteMemoryPoolAllocation;
template<typename T>
class FAccelByteMemoryDynamicAllocation;


template<typename T>
struct FChunkInfo
{
public:
	FName Key;
	T Data;
	size_t Length;
	FAccelByteMemory<T>* Pool;
};


enum class MemoryMethod
{
	PoolAllocation,
	Dynamic
};


struct MemoryConstructionParameter
{
	MemoryMethod Method;
	size_t PoolSize;
	int32 ChunkCount;
};


template<typename T>
class FAccelByteMemory
{
public:

	/**
	* @brief Reset the storage and counters of the Memory class
	*/
	virtual void RemoveAll() = 0;

	/**
	* @brief Remove a specific data from the Memory class
	*
	* @param Key Identifier of the data
	*/
	virtual void Remove(const FName& Key) = 0;

	/**
	* @brief Insert data to the Memory class based on the specific key.
	* If the Key already exist, it will be replaced.
	* If the space is not enough or prerequisite doesn't met, return nullptr.
	*
	* @param Data The reference to the object itself
	* @param Key Identifier of the data
	* @return The pointer of the ChunkInfo if success, otherwise nullptr will be returned
	*/
	virtual const FChunkInfo<T>* Insert(T& Data, const FName& Key) = 0;

	/**
	* @brief Get the stored data in the Memory class
	*
	* @param Key Identifier of the data
	* @return The pointer of the data if found, otherwise nullptr will be returned
	*/
	virtual const T* Get(const FName& Key) = 0;
	
	/**
	* @brief Approximate the size of the data that will be stored or held by the Memory class
	*
	* @param Data Reference to the data
	* @return Size of the data
	*/
	inline const size_t GetRequiredMemorySize(T& Data)
	{
		return sizeof(Data);
	}

	inline const size_t GetCurrentMemoryPoolSize() { return CurrentMemoryPoolSize; }
	inline const size_t GetMemoryPoolLeft() { return MemoryParameter.PoolSize - CurrentMemoryPoolSize; }
	inline const int32 GetCurrentChunkCount() { return CurrentChunkCount; }

protected:
	
	/**
	* @brief Check wheter is it possible to make an insertion to the memory.
	*
	* @return true based on checking result
	*/
	virtual bool InsertPrerequisiteOkay() = 0;
	virtual ~FAccelByteMemory() {};

	/**
	* @brief Find the index of the ChunkList based on the storage Key
	*
	* @param Key Identifier of the data
	* @return Index number if it exists, otherwise return -1.
	*/
	inline const int32 FindIndexFromChunkList(const FName& Key)
	{
		int Index = -1;
		for (int i = 0; i < ChunkList.Num(); i++)
		{
			if (ChunkList[i].Key == Key)
			{
				Index = i;
			}
		}

		return Index;
	}

	MemoryConstructionParameter MemoryParameter;
	size_t CurrentMemoryPoolSize = 0;
	int32 CurrentChunkCount = 0;
	
	TArray<FChunkInfo<T>> ChunkList;
};

// Override specific for HTTP response size
template<>
inline const size_t FAccelByteMemory<FHttpRequestPtr>::GetRequiredMemorySize(FHttpRequestPtr& Data)
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
inline const size_t FAccelByteMemory<FString>::GetRequiredMemorySize(FString& Data)
{
	return Data.GetAllocatedSize();
}


template<typename T>
class FAccelByteMemoryFactory
{
public:
	inline static TSharedPtr<FAccelByteMemory<T>> Create(MemoryConstructionParameter Param)
	{
		switch (Param.Method)
		{
		case MemoryMethod::PoolAllocation:
			return MakeShareable(new FAccelByteMemoryPoolAllocation<T>(Param.PoolSize, Param.ChunkCount));
		case MemoryMethod::Dynamic:
			return MakeShareable(new FAccelByteMemoryDynamicAllocation<T>(Param.PoolSize, Param.ChunkCount));
		default:
			return nullptr;
		}
	}
};


template<typename T>
class FAccelByteMemoryPoolAllocation : public FAccelByteMemory<T>
{
public:
	FAccelByteMemoryPoolAllocation(size_t PoolSize_, int32 ChunkCount_)
	{
		this->MemoryParameter.Method = MemoryMethod::PoolAllocation;
		this->MemoryParameter.PoolSize = PoolSize_;
		this->MemoryParameter.ChunkCount = ChunkCount_;
		ReservedMemory = std::malloc(PoolSize_);
	}

	~FAccelByteMemoryPoolAllocation()
	{
		free(ReservedMemory);
		this->ChunkList.Empty();
	}

	inline void RemoveAll() override
	{
		this->CurrentChunkCount = 0;
		this->CurrentMemoryPoolSize = 0;
		this->ChunkList.Empty();
	}

	inline const FChunkInfo<T>* Insert(T& Data, const FName& Key) override
	{
		if (this->ChunkList.Num() >= this->MemoryParameter.ChunkCount)
		{
			return nullptr;
		}
		// Store to the reserved memory 
		return nullptr;
	}

	inline void Remove(const FName& Key) override
	{
		auto Index = this->FindIndexFromChunkList(Key);
		if (Index >= 0)
		{
			// deal with raw memory immediately
			this->ChunkList.RemoveAt(Index);
		}
	}

	inline const T* Get(const FName& Key) override
	{
		auto Index = this->FindIndexFromChunkList(Key);
		if (Index >= 0)
		{
			return &this->ChunkList[Index].Data;
		}
		return nullptr;
	}

private:
	inline bool InsertPrerequisiteOkay() override
	{
		if (this->ChunkList.Num() >= this->MemoryParameter.ChunkCount)
		{
			return false;
		}
		// if (ReservedMemory is full)

		return true;
	}

	//TODO void DefragmentMemory() = 0;

	// Constructor don't forget to reserve based on the PoolSize byte
	// Makeshareable for ChunkInfo

	void* ReservedMemory;
};


template<typename T>
class FAccelByteMemoryDynamicAllocation : public FAccelByteMemory<T>
{
public:
	FAccelByteMemoryDynamicAllocation(size_t PoolSize_, int32 ChunkCount_)
	{
		this->MemoryParameter.Method = MemoryMethod::Dynamic;
		this->MemoryParameter.PoolSize = PoolSize_;
		this->MemoryParameter.ChunkCount = ChunkCount_;
	}

	~FAccelByteMemoryDynamicAllocation()
	{
		this->ChunkList.Empty();
	}

	inline void RemoveAll() override
	{
		this->CurrentChunkCount = 0;
		this->CurrentMemoryPoolSize = 0;
		this->ChunkList.Empty();
	}

	inline const FChunkInfo<T>* Insert(T& Data, const FName& Key) override
	{
		if (!InsertPrerequisiteOkay())
		{
			return nullptr;
		}

		FChunkInfo<T> Result{ Key, Data, this->GetRequiredMemorySize(Data), this };
		int Index = this->ChunkList.Add(Result);
		Result.Length = this->GetRequiredMemorySize(this->ChunkList[Index].Data);

		this->CurrentChunkCount += 1;
		this->CurrentMemoryPoolSize += Result.Length;

		return &this->ChunkList[Index];
	}

	inline void Remove(const FName& Key) override
	{
		auto Index = this->FindIndexFromChunkList(Key);
		if (Index >= 0)
		{
			this->CurrentChunkCount -= 1;
			this->CurrentMemoryPoolSize -= this->ChunkList[Index].Length;
			this->ChunkList.RemoveAt(Index);
		}
	}

	inline const T* Get(const FName& Key) override
	{
		auto Index = this->FindIndexFromChunkList(Key);
		if (Index >= 0)
		{
			return &this->ChunkList[Index].Data;
		}
		return nullptr;
	}

private:
	inline bool InsertPrerequisiteOkay() override
	{
		if (this->GetCurrentChunkCount() >= this->MemoryParameter.ChunkCount)
		{
			return false;
		}
		if (this->GetCurrentMemoryPoolSize() >= this->MemoryParameter.PoolSize)
		{
			return false;
		}
		return true;
	}
};

}
}
