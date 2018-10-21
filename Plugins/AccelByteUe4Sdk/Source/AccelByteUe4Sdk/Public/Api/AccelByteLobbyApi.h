// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteError.h"
#include "Models/AccelByteLobbyModels.h"

// Forward declarations
class IWebSocket;

namespace AccelByte
{
namespace Api
{
	
/**
 * @brief Lobby API uses WebSocket.
 */
class ACCELBYTEUE4SDK_API Lobby
{
public:
DECLARE_DELEGATE_ThreeParams(FConnectionClosed, int32, const FString&, bool);
DECLARE_DELEGATE(FConnectSuccess);

	static Lobby& Get();
	DECLARE_DELEGATE_ThreeParams(FConnectionClosed, int32, const FString&, bool);
	DECLARE_DELEGATE(FConnectSuccess);
	void Connect(const FConnectSuccess& OnSuccess, const FErrorHandler& OnError, const FConnectionClosed& OnConnectionClosed);
	void Disconnect();
	bool IsConnected() const;

	
	void SendPrivateMessage(const FString& UserId, const FString& Message);
	void SendPartyMessage(const FString& Message);
	void SendInfoPartyRequest();
	void SendCreatePartyRequest();
	void SendLeavePartyRequest();
	void SendInviteToPartyRequest(const FString& UserId);
	void SendAcceptInvitationRequest(const FString& PartyId, FString InvitationToken);

	DECLARE_DELEGATE_OneParam(FPrivateMessageNotice, const FAccelByteModelsPrivateMessageNotice&);
	DECLARE_DELEGATE_OneParam(FPartyMessageNotice, const FAccelByteModelsPartyMessageNotice&);
	DECLARE_DELEGATE_OneParam(FInfoPartyResponse, const FAccelByteModelsInfoPartyResponse&);
	DECLARE_DELEGATE_OneParam(FCreatePartyResponse, const FAccelByteModelsCreatePartyResponse&);
	DECLARE_DELEGATE_OneParam(FLeavePartyResponse, const FAccelByteModelsLeavePartyResponse&);
	DECLARE_DELEGATE_OneParam(FInviteToPartyResponse, const FAccelByteModelsInviteToPartyResponse&);
	DECLARE_DELEGATE_OneParam(FPartyInvitationNotice, const FAccelByteModelsPartyInvitationNotice&);
	DECLARE_DELEGATE_OneParam(FAcceptInvitationResponse, const FAccelByteModelsAcceptInvitationReponse&);
	DECLARE_DELEGATE_OneParam(FPartyInvitationAcceptanceNotice, const FAccelByteModelsPartyInvitationAcceptanceNotice&);

	void BindDelegates(
		const FPrivateMessageNotice& PrivateMessageNotice,
		const FPartyMessageNotice& PartyMessageNotice,
		const FInfoPartyResponse& InfoPartyResponse,
		const FCreatePartyResponse& CreatePartyResponse,
		const FLeavePartyResponse& LeavePartyResponse,
		const FInviteToPartyResponse& InviteToPartyResponse,
		const FPartyInvitationNotice& PartyInvitationNotice,
		const FAcceptInvitationResponse& AcceptInvitationResponse,
		const FPartyInvitationAcceptanceNotice& PartyInvitationAcceptanceNotice
	);
	void UnbindDelegates();

private:
	Lobby(Lobby const&) = delete; // Copy constructor
	Lobby(Lobby&&) = delete; // Move constructor
	Lobby& operator=(Lobby const&) = delete; // Copy assignment operator
	Lobby& operator=(Lobby &&) = delete; // Move assignment operator

	void OnConnected();
	void OnConnectionError(const FString& Error);
	void OnMessage(const FString& Message);
	void OnClosed(int32 StatusCode, const FString& Reason, bool WasClean);

	TSharedPtr<IWebSocket> WebSocket;
	FConnectSuccess ConnectSuccess;
	FErrorHandler ConnectError;
	FConnectionClosed ConnectionClosed;
	
	FPrivateMessageNotice PrivateMessageNotice;
	FPartyMessageNotice PartyMessageNotice;
	FInfoPartyResponse InfoPartyResponse;
	FCreatePartyResponse CreatePartyResponse;
	FLeavePartyResponse LeavePartyResponse;
	FInviteToPartyResponse InviteToPartyResponse;
	FPartyInvitationNotice PartyInvitationNotice;
	FAcceptInvitationResponse AcceptInvitationResponse;
	FPartyInvitationAcceptanceNotice PartyInvitationAcceptanceNotice;
protected:
	Lobby();
	~Lobby();
};

} // Namespace Api
} // Namespace AccelByte
