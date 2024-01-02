// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerDSHubApi.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteWebSocket.h"
#include "Core/FUnrealWebSocketFactory.h"
#include "Core/IWebSocketFactory.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteReport.h"

DEFINE_LOG_CATEGORY(LogAccelByteDSHub);

namespace AccelByte
{
namespace GameServerApi
{

// DS HUB TOPICS START
const FString DSMCServerClaimedTopic = TEXT("serverClaimed");
const FString MMV2BackfillProposalTopic = TEXT("BACKFILL_PROPOSAL");
const FString SessionMembersChangedTopic = TEXT("SESSION_MEMBER_CHANGED");
const FString SessionEndedTopic = TEXT("SESSION_ENDED_NOTIF");
const FString SessionSecretUpdateTopic = TEXT("SESSION_SERVER_SECRET");
// DS HUB TOPICS END

ServerDSHub::ServerDSHub(
	ServerCredentials const& InCredentialsRef,
	ServerSettings const& InSettingsRef,
	FHttpRetryScheduler& InHttpRef,
	float InPingDelay,
	float InInitialBackoffDelay,
	float InMaxBackoffDelay,
	float InTotalTimeout)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	, PingDelay(InPingDelay)
	, InitialBackoffDelay(InInitialBackoffDelay)
	, MaxBackoffDelay(InMaxBackoffDelay)
	, TotalTimeout(InTotalTimeout)
{
}

ServerDSHub::~ServerDSHub()
{
	// Only disconnect on destruct if the engine is still around
	if (UObjectInitialized())
	{
		Disconnect(true);
		UnbindDelegates();
	}
}

void ServerDSHub::Connect(const FString& InBoundServerName)
{
	FReport::Log(FString(__FUNCTION__));

	BoundServerName = InBoundServerName;

	if (!WebSocket.IsValid())
	{
		CreateWebSocket();
	}

	if (WebSocket->IsConnected())
	{
		return;
	}

	WebSocket->Connect();

	UE_LOG(LogAccelByteDSHub, Log, TEXT("Connecting to %s"), *ServerSettingsRef.DSHubServerUrl);
}

void ServerDSHub::Disconnect(bool bForceCleanup /*= false*/)
{
	FReport::Log(FString(__FUNCTION__));

	if (WebSocket.IsValid())
	{
		WebSocket->Disconnect(bForceCleanup);
	}

	UE_LOG(LogAccelByteDSHub, Log, TEXT("Disconnected"));
}

bool ServerDSHub::IsConnected() const
{
	FReport::Log(FString(__FUNCTION__));

	return WebSocket.IsValid() && WebSocket->IsConnected();
}

void ServerDSHub::SetOnServerClaimedNotificationDelegate(const FOnServerClaimedNotification& InDelegate)
{
	OnServerClaimedNotification = InDelegate;
}

void ServerDSHub::SetOnV2BackfillProposalNotificationDelegate(const FOnV2BackfillProposalNotification& InDelegate)
{
	OnV2BackfillProposalNotification = InDelegate;
}

void ServerDSHub::SetOnV2SessionMemberChangedNotificationDelegate(const FOnV2SessionMemberChangedNotification& InDelegate)
{
	OnV2SessionMemberChangedNotification = InDelegate;
}

void ServerDSHub::SetOnV2SessionEndedNotificationDelegate(const FOnV2SessionEndedNotification& InDelegate)
{
	OnV2SessionEndedNotification = InDelegate;
}

void ServerDSHub::SetOnV2SessionSecretUpdateNotification(const FOnV2SessionSecretUpdateNotification& InDelegate)
{
	OnV2SessionSecreteUpdateNotification = InDelegate;
}

void ServerDSHub::UnbindDelegates()
{
	OnConnectSuccessDelegate.Unbind();
	OnConnectErrorDelegate.Unbind();
	OnConnectionClosedDelegate.Unbind();
	
	OnServerClaimedNotification.Unbind();
	OnV2BackfillProposalNotification.Unbind();
	OnV2SessionMemberChangedNotification.Unbind();
	OnV2SessionEndedNotification.Unbind();
}

void ServerDSHub::CreateWebSocket()
{
	if (WebSocket.IsValid())
	{
		WebSocket.Reset();
	}

	TMap<FString, FString> Headers;
	Headers.Add(DSHubServerNameHeader, BoundServerName);
	FModuleManager::Get().LoadModuleChecked(FName(TEXT("WebSockets")));

	FAccelByteUtilities::AppendModulesVersionToMap(Headers);
	WebSocket = AccelByteWebSocket::Create(*ServerSettingsRef.DSHubServerUrl,
		TEXT("wss"),
		ServerCredentialsRef,
		Headers,
		TSharedRef<IWebSocketFactory>(new FUnrealWebSocketFactory()),
		PingDelay,
		InitialBackoffDelay,
		MaxBackoffDelay,
		TotalTimeout);

	WebSocket->OnConnected().AddRaw(this, &ServerDSHub::OnConnected);
	WebSocket->OnMessageReceived().AddRaw(this, &ServerDSHub::OnMessage);
	WebSocket->OnConnectionError().AddRaw(this, &ServerDSHub::OnConnectionError);
	WebSocket->OnConnectionClosed().AddRaw(this, &ServerDSHub::OnClosed);
}

void ServerDSHub::OnConnected()
{
	UE_LOG(LogAccelByteDSHub, Log, TEXT("Connected"));

	OnConnectSuccessDelegate.ExecuteIfBound();
}

void ServerDSHub::OnConnectionError(const FString& Error)
{
	UE_LOG(LogAccelByteDSHub, Log, TEXT("Error connecting: %s"), *Error);

	OnConnectErrorDelegate.ExecuteIfBound(Error);
}

void ServerDSHub::SetOnConnectSuccess(const FConnectSuccess& OnConnectSuccess)
{
	OnConnectSuccessDelegate = OnConnectSuccess;
}

void ServerDSHub::SetOnConnectError(const FConnectError& OnConnectError)
{
	OnConnectErrorDelegate = OnConnectError;
}

void ServerDSHub::SetOnConnectionClosed(const FConnectionClosed& OnConnectionClosed)
{
	OnConnectionClosedDelegate = OnConnectionClosed;
}

void ServerDSHub::OnMessage(const FString& Message)
{
	UE_LOG(LogAccelByteDSHub, Log, TEXT("New message received from DSHub:\n%s"), *Message);

	// First, try and parse the message to a JSON object
	TSharedPtr<FJsonObject> MessageObject;
	TSharedRef<TJsonReader<>> MessageReader = TJsonReaderFactory<>::Create(Message);
	if (!FJsonSerializer::Deserialize(MessageReader, MessageObject))
	{
		UE_LOG(LogAccelByteDSHub, Warning, TEXT("Failed to parse DS hub message as a JSON object!"));
		return;
	}

	const bool bHasTopicField = MessageObject->HasTypedField<EJson::String>(TEXT("topic"));
	const bool bHasPayloadField = MessageObject->HasTypedField<EJson::Object>(TEXT("payload"));
	if (!bHasTopicField || !bHasPayloadField)
	{
		UE_LOG(LogAccelByteDSHub, Warning, TEXT("Cannot handle received DS hub message as it does not contain a topic and/or a payload field!"));
		return;
	}

	const FString Topic = MessageObject->GetStringField(TEXT("topic"));
	const TSharedPtr<FJsonObject> PayloadObject = MessageObject->GetObjectField(TEXT("payload"));

	if (Topic.Equals(DSMCServerClaimedTopic))
	{
		FAccelByteModelsServerClaimedNotification NotificationPayload;
		if (!FJsonObjectConverter::JsonObjectToUStruct(PayloadObject.ToSharedRef(), &NotificationPayload))
		{
			UE_LOG(LogAccelByteDSHub, Warning, TEXT("Failed to convert payload for session claim topic to an FAccelByteModelsServerClaimedNotification instance!"));
			return;
		}

		OnServerClaimedNotification.ExecuteIfBound(NotificationPayload);
	}
	else if (Topic.Equals(MMV2BackfillProposalTopic))
	{
		FAccelByteModelsV2MatchmakingBackfillProposalNotif NotificationPayload;
		if (!FJsonObjectConverter::JsonObjectToUStruct(PayloadObject.ToSharedRef(), &NotificationPayload))
		{
			UE_LOG(LogAccelByteDSHub, Warning, TEXT("Failed to convert payload for backfill proposal topic to an FAccelByteModelsV2MatchmakingBackfillProposalNotif instance!"));
			return;
		}

		OnV2BackfillProposalNotification.ExecuteIfBound(NotificationPayload);
	}
	else if(Topic.Equals(SessionMembersChangedTopic))
	{
		FAccelByteModelsV2GameSession NotificationPayload;
		if (!FJsonObjectConverter::JsonObjectToUStruct(PayloadObject.ToSharedRef(), &NotificationPayload))
		{
			UE_LOG(LogAccelByteDSHub, Warning, TEXT("Failed to convert payload for session member changed topic to an FAccelByteModelsV2GameSession instance!"));
			return;
		}

		OnV2SessionMemberChangedNotification.ExecuteIfBound(NotificationPayload);
	}
	else if (Topic.Equals(SessionEndedTopic))
	{
		FAccelByteModelsSessionEndedNotification NotificationPayload;
		if (!FJsonObjectConverter::JsonObjectToUStruct(PayloadObject.ToSharedRef(), &NotificationPayload))
		{
			UE_LOG(LogAccelByteDSHub, Warning, TEXT("Failed to convert payload for session member changed topic to an FAccelByteModelsSessionEndedNotification instance!"));
			return;
		}
		OnV2SessionEndedNotification.ExecuteIfBound(NotificationPayload);
	}
	else if (Topic.Equals(SessionSecretUpdateTopic))
	{
		FAccelByteModelsSessionSecretUpdateNotification NotificationPayload;
		if (!FJsonObjectConverter::JsonObjectToUStruct(PayloadObject.ToSharedRef(), &NotificationPayload))
		{
			UE_LOG(LogAccelByteDSHub, Warning, TEXT("Failed to convert payload for session member changed topic to an FAccelByteModelsSessionSecretUpdateNotification instance!"));
			return;
		}
		OnV2SessionSecreteUpdateNotification.ExecuteIfBound(NotificationPayload);
	}
}

void ServerDSHub::OnClosed(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(LogAccelByteDSHub, Log, TEXT("Connection closed. Status code: %d; Reason: %s; Clean: %d"), StatusCode, *Reason, bWasClean);

	if (StatusCode > 4000)
	{
		Disconnect();
	}

	OnConnectionClosedDelegate.ExecuteIfBound(StatusCode, Reason, bWasClean);
}

}
}
