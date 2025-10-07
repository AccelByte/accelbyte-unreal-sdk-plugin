// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"
#include "Models/AccelByteChatModels.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteServerApiBase.h"

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{
	
/**
 * @brief Server APIs to access Chat service.
 */
class ACCELBYTEUE4SDK_API ServerChat : public FServerApiBase
{
public:
	ServerChat(const ServerCredentials& Credentials
		, const ServerSettings& Settings
		, FHttpRetrySchedulerBase& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerChat();

private:
	ServerChat() = delete;
	ServerChat(ServerChat const&) = delete;
	ServerChat(ServerChat&&) = delete;

#pragma region REQUESTS

public:

	/**
	 * @brief Create a chat topic
	 *
	 * @param MemberUserIds - UserIds to be added to the chat topic.
	 * @param AdminUserIds - UserIds to be added to the chat topic as admin.
	 * @param TopicName - Friendly name of the chat topic (alias).
	 * @param OnSuccess - Callback for successful chat topic creation.
	 * @param OnError - Callback for failed chat topic creation
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr CreateChannelChat(TSet<FString> const& MemberUserIds
		, TSet<FString> const& AdminUserIds
		, FString const& TopicName
		, THandler<FAccelByteModelsChatActionCreateTopicServerResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Delete a chat topic
	 *
	 * @param TopicId - Topic Id to be deleted.
	 * @param OnSuccess - Callback for successful chat topic deletion.
	 * @param OnError - Callback for failed chat topic deletion.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeleteChannelChat(FString const& TopicId
		, THandler<FAccelByteModelsChatActionTopicResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Add a user to chat topic
	 *
	 * @param TopicId - Topic Id to add user.
	 * @param UserId - User Id to be added to Topic.
	 * @param OnSuccess - Callback for successful user  addition.
	 * @param OnError - Callback for failed chat user addition.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr AddUserToChannelChat(FString const& TopicId
		, FString const& UserId
		, THandler<FAccelByteModelsChatActionUserTopicResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Remove a user from chat topic
	 *
	 * @param TopicId - Topic Id to remove user.
	 * @param UserId - User Id to be removed from Topic.
	 * @param OnSuccess - Callback for successful user removal.
	 * @param OnError - Callback for failed chat user removal.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RemoveUserFromChannelChat(FString const& TopicId
		, FString const& UserId
		, THandler<FAccelByteModelsChatActionUserTopicResponse> const& OnSuccess
		, FErrorHandler const& OnError);

private:

#pragma endregion
	
};
} // namespace GameServerApi
} // namespace AccelByte