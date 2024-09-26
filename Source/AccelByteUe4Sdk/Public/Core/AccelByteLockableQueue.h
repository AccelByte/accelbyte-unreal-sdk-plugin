// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Models/AccelByteLobbyModels.h"
#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_CLASS(LogAccelByteLockableQueue, Log, All);

template<typename ItemType>
class FAccelByteLockableQueue
{
public:
	FAccelByteLockableQueue(){}
	
	bool Enqueue(const ItemType& Item)
	{
		UE_LOG(LogAccelByteLockableQueue, VeryVerbose, TEXT("Item enqueued to lockable queue"));
		return Queue.Enqueue(Item);
	}

	bool Dequeue(ItemType& Item)
	{
		if(IsLocked())
		{
			UE_LOG(LogAccelByteLockableQueue, VeryVerbose, TEXT("Item dequeue failed, queue is locked"));
			return false;
		}

		UE_LOG(LogAccelByteLockableQueue, VeryVerbose, TEXT("Item dequeued from lockable queue"));
		return Queue.Dequeue(Item);
	}

	bool IsEmpty() const
	{
		return Queue.IsEmpty();
	}

	TSharedPtr<FAccelByteKey> LockQueue()
	{
		TSharedPtr<FAccelByteKey> NewKey = Key.Pin();
		
		if(!Key.IsValid())
		{
			NewKey = MakeShared<FAccelByteKey>();
			Key = NewKey;
		}

		UE_LOG(LogAccelByteLockableQueue, VeryVerbose, TEXT("locking queue"));
		return NewKey;
	}

	bool IsLocked() const
	{
		return Key.IsValid();
	}

private:

	TQueue<ItemType> Queue;
	TWeakPtr<FAccelByteKey> Key;

	/** Hidden copy constructor. */
	FAccelByteLockableQueue(const FAccelByteLockableQueue&) = delete;

	/** Hidden assignment operator. */
	FAccelByteLockableQueue& operator=(const FAccelByteLockableQueue&) = delete;
};
