// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteMessagingSystem.h"

#include "CoreMinimal.h"
#include "Misc/ScopeLock.h"

#include "Core/AccelByteDefines.h"
#include "Core/AccelByteUtilities.h"

using namespace AccelByte;

DEFINE_LOG_CATEGORY(LogAccelByteMessagingSystem);

FAccelByteMessagingSystem::FAccelByteMessagingSystem()
{
	TickerDelegate = FTickerDelegate::CreateRaw(this, &AccelByte::FAccelByteMessagingSystem::PollMessages);
	PollHandle = FTickerAlias::GetCoreTicker().AddTicker(TickerDelegate, PollingIntervalSecs);
}

FAccelByteMessagingSystem::~FAccelByteMessagingSystem()
{
	UnsubscribeAll();
	FTickerAlias::GetCoreTicker().RemoveTicker(PollHandle);
}

FDelegateHandle FAccelByteMessagingSystem::SubscribeToTopic(const EAccelByteMessagingTopic& Topic, const FOnMessagingSystemReceivedMessage& Delegate)
{
	if (Topic == EAccelByteMessagingTopic::None)
	{
		UE_LOG(LogAccelByteMessagingSystem, Warning, TEXT("Unable to subscribe the topic is empty"));
		return FDelegateHandle{};
	}

	FScopeLock Lock(&SubscribersLock);

	FDelegateHandle DelegateHandle;
	if (SubscribersDelegateMap.Contains(Topic))
	{
		DelegateHandle = SubscribersDelegateMap[Topic].Add(Delegate);
	}
	else
	{
		SubscribersDelegateMap.Add(Topic, FOnMessagingSystemReceivedMessageMulti{});
		DelegateHandle = SubscribersDelegateMap[Topic].Add(Delegate);
	}

	if (DelegateHandle.IsValid())
	{
		TotalSubscribers.Increment();
	}

	return DelegateHandle;
}

bool FAccelByteMessagingSystem::UnsubscribeFromTopic(const EAccelByteMessagingTopic& Topic, const FDelegateHandle& DelegateHandle)
{
	if (Topic == EAccelByteMessagingTopic::None)
	{
		UE_LOG(LogAccelByteMessagingSystem, Warning, TEXT("Unable to unsubscribe to empty topic"));
		return false;
	}

	if (!DelegateHandle.IsValid())
	{
		UE_LOG(LogAccelByteMessagingSystem, Warning, TEXT("Unable to unsubscribe from topic %d due to invalid delegate handle"), Topic);
		return false;
	}

	FScopeLock Lock(&SubscribersLock);

	if (SubscribersDelegateMap.Contains(Topic))
	{
		const bool bRemoved = SubscribersDelegateMap[Topic].Remove(DelegateHandle);

		if (bRemoved)
		{
			TotalSubscribers.Decrement();
		}

		return bRemoved;
	}

	UE_LOG(LogAccelByteMessagingSystem, Warning, TEXT("Unable to unsubscribe from topic %d no subscriber found"), Topic);
	return false;
}

void FAccelByteMessagingSystem::UnsubscribeAll()
{
	FScopeLock Lock(&SubscribersLock);
	SubscribersDelegateMap.Empty();
	TotalSubscribers.Reset();
}

int32 FAccelByteMessagingSystem::GetAllSubscribersCount()
{
	return TotalSubscribers.GetValue();
}

bool FAccelByteMessagingSystem::SendMessage(const EAccelByteMessagingTopic Topic)
{
	if (Topic == EAccelByteMessagingTopic::None)
	{
		return false;
	}

	FAccelByteModelsMessagingSystemMessage Message;
	Message.Topic = Topic;

	const bool bMessageAdded = Messages.Enqueue(MakeShared<FAccelByteModelsMessagingSystemMessage>(Message));

	return bMessageAdded;
}

bool FAccelByteMessagingSystem::SendMessage(const EAccelByteMessagingTopic Topic, const FString& Payload)
{
	if (Topic == EAccelByteMessagingTopic::None)
	{
		return false;
	}

	FAccelByteModelsMessagingSystemMessage Message;
	Message.Topic = Topic;
	Message.Payload = Payload;

	const bool bMessageAdded = Messages.Enqueue(MakeShared<FAccelByteModelsMessagingSystemMessage>(Message));

	return bMessageAdded;
}

bool FAccelByteMessagingSystem::PollMessages(float DeltaTime)
{
#ifdef ACCELBYTE_ACTIVATE_PROFILER
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelByteMessagingSystemPoll"));
#endif
	while (!Messages.IsEmpty())
	{
		TSharedPtr<FAccelByteModelsMessagingSystemMessage> Message;
		if (!Messages.Dequeue(Message))
		{
			continue;
		}

		if (SubscribersDelegateMap.Contains(Message->Topic))
		{
			SubscribersDelegateMap[Message->Topic].Broadcast(Message->Payload);
		}
	}

	return true;
}
