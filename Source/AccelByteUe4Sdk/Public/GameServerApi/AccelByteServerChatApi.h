// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteRegistry.h"
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
	ServerChat(const ServerCredentials& Credentials, const ServerSettings& Settings, FHttpRetryScheduler& InHttpRef);
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
	 */
	void CreateChannelChat(const TSet<FString> MemberUserIds
		, const TSet<FString> AdminUserIds
		, const FString& TopicName
		, const THandler<FAccelByteModelsChatActionCreateTopicServerResponse>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Delete a chat topic
	 *
	 * @param TopicId - Topic Id to be deleted.
	 * @param OnSuccess - Callback for successful chat topic deletion.
	 * @param OnError - Callback for failed chat topic deletion.
	 */
	void DeleteChannelChat(const FString& TopicId
		, const THandler<FAccelByteModelsChatActionTopicResponse>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Add a user to chat topic
	 *
	 * @param TopicId - Topic Id to add user.
	 * @param UserId - User Id to be added to Topic.
	 * @param OnSuccess - Callback for successful user  addition.
	 * @param OnError - Callback for failed chat user addition.
	 */
	void AddUserToChannelChat(const FString& TopicId
		, const FString& UserId
		, const THandler<FAccelByteModelsChatActionUserTopicResponse>& OnSuccess
		, const FErrorHandler& OnError);

	/**
	 * @brief Remove a user from chat topic
	 *
	 * @param TopicId - Topic Id to remove user.
	 * @param UserId - User Id to be removed from Topic.
	 * @param OnSuccess - Callback for successful user removal.
	 * @param OnError - Callback for failed chat user removal.
	 */
	void RemoveUserFromChannelChat(const FString& TopicId
		, const FString& UserId
		, const THandler<FAccelByteModelsChatActionUserTopicResponse>& OnSuccess
		, const FErrorHandler& OnError);

private:

#pragma endregion
	
};
} // namespace GameServerApi
} // namespace AccelByte