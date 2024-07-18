// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Containers/Queue.h"
#include "Containers/Ticker.h"

#include "AccelByteDefines.h"
#include "Models/AccelByteMessagingSystemModels.h"
#include "JsonObjectConverter.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteMessagingSystem, Log, All);
namespace AccelByte
{
DECLARE_DELEGATE_OneParam(FOnMessagingSystemReceivedMessage, const FString& /* Payload */)
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMessagingSystemReceivedMessageMulti, const FString& /* Payload */)

class ACCELBYTEUE4SDK_API FAccelByteMessagingSystem 
	: public TSharedFromThis<FAccelByteMessagingSystem>
{
public:
	FAccelByteMessagingSystem();
	~FAccelByteMessagingSystem();

	/**
	 * @brief Send message without payload to the messaging system which later will be notified to the subscribers.
	 *
	 * @param Topic The topic of the message
	 * @return true if message successfully queued
	 */
	bool SendMessage(const EAccelByteMessagingTopic Topic);

	/**
	 * @brief Send message with payload to the messaging system which later will be notified to the subscribers.
	 *
	 * @param Topic The topic of the message
	 * @param Payload The payload of the message. It should be USTRUCT type.
	 * @return true if message successfully queued
	 */
	template <typename T>
	bool SendMessage(const EAccelByteMessagingTopic Topic, const T& Payload)
	{
		if (Topic == EAccelByteMessagingTopic::None)
		{
			return false;
		}

		FAccelByteModelsMessagingSystemMessage Message;
		Message.Topic = Topic;
		Message.Payload = SerializeObject<T>(Payload);

		const bool bMessageAdded = Messages.Enqueue(MakeShared<FAccelByteModelsMessagingSystemMessage>(Message));

		return bMessageAdded;
	}

	/**
	 * @brief Send message with string payload to the messaging system which later will be notified to the subscribers.
	 *
	 * @param Topic The topic of the message
	 * @param Payload The payload of the message.
	 * @return true if message successfully queued
	 */
	bool SendMessage(const EAccelByteMessagingTopic Topic, const FString& Payload);

	/**
	 * @brief Subscribe to particular topic in messaging system
	 *
	 * @param Topic The topic to subscribe, list of available topic is in EAccelByteMessagingTopic
	 * @param Delegate The delegate that will be executed when new message for this topic arrived
	 *
	 * @return FString representing unique id for the subscriber and can be used to unsubscribe from the topic.
	 * Empty string returned if failed to subscribe.
	 */
	FDelegateHandle SubscribeToTopic(const EAccelByteMessagingTopic& Topic, const FOnMessagingSystemReceivedMessage& Delegate);

	/**
	 * @brief Unsubscribe to a topic in messaging system
	 *
	 * @param Topic The topic to unsubscribe
	 * @param DelegateHandle The delegate handle returned by the SubscribeToTopic method
	 * @return true if the SubscriberID found and successfully unsubscribe from topic
	 */
	bool UnsubscribeFromTopic(const EAccelByteMessagingTopic& Topic, const FDelegateHandle& DelegateHandle);

	/**
	 * @brief Unsubscribe all subscribers from messaging system.
	 */
	void UnsubscribeAll();

	/**
	 * @brief Get the number of all subscribers to all of the topics.
	 *
	 * @return The number of all subscribers to all of the topics.
	 */
	int32 GetAllSubscribersCount();

private:
	TQueue<TSharedPtr<FAccelByteModelsMessagingSystemMessage>, EQueueMode::Mpsc> Messages;

	mutable FCriticalSection SubscribersLock;
	TMap<EAccelByteMessagingTopic, FOnMessagingSystemReceivedMessageMulti> SubscribersDelegateMap;

	const float PollingIntervalSecs = 0.5f;
	FTickerDelegate TickerDelegate{};
	FDelegateHandleAlias PollHandle{};

	FThreadSafeCounter TotalSubscribers{0};

	bool PollMessages(float DeltaTime);

	template <typename T>
	static FString SerializeObject(T Object)
	{
		FString Payload;
		FJsonObjectConverter::UStructToJsonObjectString(Object, Payload);

		return Payload;
	}
};
}
