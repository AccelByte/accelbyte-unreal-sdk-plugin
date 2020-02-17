// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteDSMModels.h"

class IWebSocket;

namespace AccelByte
{
class ServerCredentials;
class ServerSettings;

namespace GameServerApi
{

/**
 * @brief DSM API for communicating DS to DSM.
 * DSM server uses WebSocket (RFC 6455) to send matchmaking information to DS.
 */
class ACCELBYTEUE4SDK_API ServerDSM
{
public:
	ServerDSM(const ServerCredentials& Credentials, const ServerSettings& Settings);
	~ServerDSM();
	
public:

	/*
	 * @brief send register request to DSM
	 *
	 * @param Port the port where your game server run.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	*/
	void RegisterServerToDSM(const int32 Port, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);


    /*
	 * @brief send register local server to DSM
	 *
	 * @param Port the port where your game server run.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	*/
    void RegisterLocalServerToDSM(const FString IPAddress, const int32 Port, const FString ServerName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/*
	 * @brief send shutdown request to DSM
	 *
	 * @param KillMe true if you want DSM to kill your DS. false if your DS already on shutting down state.
	 * @param MatchId Id that you get from DSM-MatchmakingInfoNotif.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed. 
	*/
	void SendShutdownToDSM(const bool KillMe, const FString& MatchId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);


	/*
	 * @brief deregister local server to DSM
	 *
	 * @param KillMe true if you want DSM to kill your DS. false if your DS already on shutting down state.
	 * @param MatchId Id that you get from DSM-MatchmakingInfoNotif.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed. 
	*/
    void DeregisterLocalServerFromDSM(const FString& ServerName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

	/*
	 * @brief Configure automatic heart beat on register. Must be called before calling RegisterServerToDSM or RegisterLocalServerToDSM
	 *
	 * @param bIsAutomatic true if you want heartbeat triggered automatically, false if you want poll heartbeat manually all the time.
	 * @param TimeoutSeconds Timeout in seconds when heartbeat automatically triggered.
	*/
	void ConfigureHeartBeat(bool bIsAutomatic = true, int TimeoutSeconds = 5);

	/*
	 * @brief Poll heartbeat manually. It will raise OnMatchRequest event if DSM send match request data in heartbeat response
	*/
	void PollHeartBeat();

	/*
	 * @brief Set handler delegate for OnMatchRequest event when DSM send match request data in heartbeat response
	 *
	 * @param OnMatchRequest This delegate will be called if DSM send match request data in heartbeat response
	*/
	void SetOnMatchRequest(THandler<FAccelByteModelsMatchRequest> OnMatchRequest);

private:
	ServerDSM(ServerDSM const&) = delete; // Copy constructor
	ServerDSM(ServerDSM&&) = delete; // Move constructor
	ServerDSM& operator=(ServerDSM const&) = delete; // Copy assignment operator
	ServerDSM& operator=(ServerDSM &&) = delete; // Move assignment operator

	bool HeartBeatTick(float DeltaTime);

	FString ServerName = "";
	FHttpRequestCompleteDelegate OnRegisterResponse;
	FHttpRequestCompleteDelegate OnHeartBeatResponse;
	THandler<FAccelByteModelsMatchRequest> OnMatchRequest;
	bool bHeartbeatIsAutomatic = false;
	int HeartBeatTimeoutSeconds = 0;
	FErrorHandler OnHeartBeatError;
	FTickerDelegate HeartBeatDelegate;
	FDelegateHandle HeartBeatDelegateHandle;
};

} // Namespace GameServerApi
} // Namespace AccelByte
