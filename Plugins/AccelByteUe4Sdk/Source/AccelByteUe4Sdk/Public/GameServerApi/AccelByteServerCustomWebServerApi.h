// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteCustomWebServer.h"
#include "Models/AccelByteDSMModels.h"

namespace AccelByte
{

namespace GameServerApi
{

/**
 * @brief DSM API for communicating DS to DSM.
 * DSM server uses WebSocket (RFC 6455) to send matchmaking information to DS.
 */
class ACCELBYTEUE4SDK_API ServerCustomWebServer
{
public:
	ServerCustomWebServer();
	~ServerCustomWebServer();

	DECLARE_DELEGATE_OneParam(FDSClaim, const FAccelByteModelsDSMMessage&);

	void Start(const uint16 Port);
	void Stop();
	const uint16 GetPort(){ return Port; };
	void SetDSClaimDelegate(FDSClaim OnDSClaim){ DSClaimDelegate = OnDSClaim; };
	void SetErrorDelegate(FErrorHandler OnError){ ErrorDelegate = OnError; };
	void UnbindEvents();
private:
	ServerCustomWebServer(ServerCustomWebServer const&) = delete; // Copy constructor
	ServerCustomWebServer(ServerCustomWebServer&&) = delete; // Move constructor
	ServerCustomWebServer& operator=(ServerCustomWebServer const&) = delete; // Copy assignment operator
	ServerCustomWebServer& operator=(ServerCustomWebServer &&) = delete; // Move assignment operator

	uint16 Port;
	TSharedPtr<FAccelByteCustomWebServer> ServerInstance;
	void SetPort(const uint16 GivenPort){ Port = GivenPort; };

	FDSClaim DSClaimDelegate;
	FErrorHandler ErrorDelegate;
	
};

} // Namespace GameServerApi
} // Namespace AccelByte
