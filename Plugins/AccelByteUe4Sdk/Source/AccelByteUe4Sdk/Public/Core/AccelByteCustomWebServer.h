// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreUObject.h"
#include "Engine.h"

DECLARE_LOG_CATEGORY_EXTERN(LogWebServer, Log, All);

#include "Runtime/Networking/Public/Networking.h"
#include "Runtime/Networking/Public/Common/TcpListener.h"

// Default endpoint for the server
#define DEFAULT_ENDPOINT FIPv4Endpoint(FIPv4Address(0, 0, 0, 0), 8080)

/**
 * Server listening on a TCP socket
 */
class FAccelByteCustomWebServer
	: public FRunnable
{
public:
    FAccelByteCustomWebServer();

	~FAccelByteCustomWebServer();

	void Start(const uint16 Port);

	void Unbind();

	const uint16 GetPort(){ return Port; };

	/** FRunnable init */
	virtual bool Init() override;

	/** FRunnable loop */
	virtual uint32 Run() override;

	bool HandleListenerConnectionAccepted(class FSocket* ClientSocket, const FIPv4Endpoint& ClientEndpoint);

	/**
	* Checks whether the listener is listening for incoming connections.
	*
	* @return true if it is listening, false otherwise.
	*/
	bool IsActive() const
	{
		return (!Stopping);
	}

	virtual void Stop() override
	{
		Stopping = true;
	}

	virtual void Exit() override { }

private:
	class FTcpListener *Listener = NULL;

	/** Current clients and pending to get accepted clients */
	TQueue<class FSocket*, EQueueMode::Mpsc> PendingClients;
	TArray<class FSocket*> Clients;

	uint16 Port;

	/** Used to tell that the thread is stopping */
	bool Stopping;

	/** Connection thread, used to not block the editor when waiting for connections */
	FRunnableThread* Thread = NULL;

	/** Basic message handling, will get extended */
	FString HandleClientMessage(const FSocket *Socket, const FString& Message);
	bool HandleExecCommand(const FSocket *Socket, const FString& Command);
	void SetPort(const uint16 GivenPort){ Port = GivenPort; };
};
