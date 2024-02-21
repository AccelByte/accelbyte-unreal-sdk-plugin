// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once
#include "Core/AccelByteMessagingSystem.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteNotificationSender, Log, All);

namespace AccelByte
{

class ACCELBYTEUE4SDK_API FAccelByteNotificationSenderUtility
{
public:
	/**
	 * Compose a lobby notification format string for a given MMv2 notification.
	 * @param Topic Topic of the MMv2 notification.
	 * @param JsonStringContent Notification content in json string format.
	 * @param bPayloadEncoded [Optional] set to true if payload already Base64 encoded.
	 * @return Lobby notification format string.
	 * @see EV2MatchmakingNotifTopic
	 */
	static FString ComposeMMv2Notification(const FString& Topic, const FString& JsonStringContent, const bool bPayloadEncoded = false);

	/**
	 * Compose a lobby notification format string for a given session notification.
	 * @param Topic Topic of the Session notification. 
	 * @param JsonStringContent Notification content in json string format.
	 * @param bPayloadEncoded [Optional] set to true if payload already Base64 encoded.
	 * @return Lobby notification format string.
	 * @see EV2SessionNotifTopic
	 */
	static FString ComposeSessionNotification(const FString& Topic, const FString& JsonStringContent, const bool bPayloadEncoded = false);

	/**
	 * Compose a lobby notification format string for a given parameters.
	 * @param LobbySessionId SessionId of lobby.
	 * @param LoginType the type of login.
	 * @param ReconnectFromCode the reconnect from code.
	 * @return
	 */
	static FString ComposeLobbyConnectedNotification(const FString& LobbySessionId, const FString& LoginType, const int32& ReconnectFromCode);
};
	
class ACCELBYTEUE4SDK_API FAccelByteNotificationSender
{
public:
	explicit FAccelByteNotificationSender(FAccelByteMessagingSystem& MessagingSystem);

	void SendLobbyNotification(const FString& Message) const;
	
private:
	FAccelByteMessagingSystem& MessagingSystem;
	
};
}