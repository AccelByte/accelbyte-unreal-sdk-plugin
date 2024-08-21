// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteNotificationSender.h"
#include "Core/AccelByteDefines.h"
#include "Misc/Base64.h"

namespace AccelByte
{
	FString FAccelByteNotificationSenderUtility::ComposeMMv2Notification(const FString& Topic, const FString& JsonStringContent, const bool bPayloadEncoded)
	{
		/* Full message example is like the following as of january 22nd 2024
		 * ==========================================
		 * type: messageNotif // must be messageNotif
		 * id: b3cb71d768d3493ba6c1dbd10b816e62
		 * from: system
		 * to: 103f2296c3d241ca9b21afaac693595d
		 * topic: OnMatchFound
		 * payload: // base64 encoded json string
		 * sentAt: 2024-01-22T09:10:54Z
		 * sequenceID: 1705914633
		 * sequenceNumber: 2
		 * =========================================
		 * but we only need to fill in topic and payload field to be used as a MMv2 notification
		 */

		const FString Payload = bPayloadEncoded ? JsonStringContent : FBase64::Encode(JsonStringContent);
		FString LobbyMessage = FString::Printf(TEXT("%stype: messageNotif\ntopic: %s\npayload: %s%s"), *LobbyMessageEnvelopeStartContent, *Topic, *Payload, *LobbyMessageEnvelopeEndContent);
		
		return LobbyMessage;
	}

	FString FAccelByteNotificationSenderUtility::ComposeSessionNotification(const FString& Topic, const FString& JsonStringContent, const bool bPayloadEncoded)
	{
		/* Full message example is like the following as of january 22nd 2024
		 * ==========================================
		 * type: messageSessionNotif // must be messageSessionNotif
		 * topic: OnPartyMembersChanged
		 * payload: // base64 encoded json string
		 * sentAt: 2024-01-22T09:19:43ZLbE"
		 * =========================================
		 * but we only need to fill in topic and payload field to be used as a session notification
		 */

		const FString Payload = bPayloadEncoded ? JsonStringContent : FBase64::Encode(JsonStringContent);
		FString LobbyMessage = FString::Printf(TEXT("%stype: messageSessionNotif\ntopic: %s\npayload: %s%s"), *LobbyMessageEnvelopeStartContent, *Topic, *Payload, *LobbyMessageEnvelopeEndContent);

		return LobbyMessage;
	}

	FString FAccelByteNotificationSenderUtility::ComposeLobbyConnectedNotification(const FString& LobbySessionId, const FString& LoginType, const int32& ReconnectFromCode)
	{
		/* Full message example is like the following as of january 24th 2024
		 * ==========================================
		 * type: connectNotif // must be connectNotif
		 * loginType: NewRegister
		 * reconnectFromCode: 5000
		 * lobbySessionID: a7e156e63ece4ebb93085acdb8ef4ae7
		 * sequenceID: 1705900094
		 * sequenceNumber: 1
		 * =========================================
		 * but we only need to fill in loginType, reconnectFromCode and lobbySessionID to be used as a connected notification
		 */

		FString LobbyMessage = FString::Printf(TEXT("%stype: connectNotif\nloginType: %s\nreconnectFromCode: %d\nlobbySessionID: %s%s"), *LobbyMessageEnvelopeStartContent, *LoginType, ReconnectFromCode, *LobbySessionId, *LobbyMessageEnvelopeEndContent);

		return LobbyMessage;
	}

	FAccelByteNotificationSender::FAccelByteNotificationSender(FAccelByteMessagingSystem& InMessagingSystemRef)
#if ENGINE_MAJOR_VERSION < 5
		: MessagingSystemWPtr{InMessagingSystemRef.AsShared()}
#else
		: MessagingSystemWPtr{InMessagingSystemRef.AsWeak()}
#endif
	{
	}

	void FAccelByteNotificationSender::SendLobbyNotification(const FString& Message) const
	{
		auto MessagingSystemPtr = MessagingSystemWPtr.Pin();
		if (MessagingSystemPtr.IsValid())
		{
			MessagingSystemPtr->SendMessage(EAccelByteMessagingTopic::NotificationSenderLobby, Message);
		}
	}
}
