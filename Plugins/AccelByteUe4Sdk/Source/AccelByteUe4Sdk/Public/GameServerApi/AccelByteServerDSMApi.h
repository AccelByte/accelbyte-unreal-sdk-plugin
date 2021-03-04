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
enum class EServerType :uint8
{
	NONE,
	CLOUDSERVER,
	LOCALSERVER
};

/**
 * @brief DSM API for communicating DS to DSM.
 * DSM server uses WebSocket (RFC 6455) to send matchmaking information to DS.
 */
class ACCELBYTEUE4SDK_API ServerDSM
{
public:
	ServerDSM(const ServerCredentials& Credentials, const ServerSettings& Settings);
	~ServerDSM();

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
	 * @brief Get Session ID of a claimed DS. Will return empty string OnSucess if DS is not claimed yet.
	 *
	 * @param OnSuccess This will be called when the operation succeeded, The result is const FAccelByteModelsServerSessionResponse.
	 * @param OnError This will be called when the operation failed.
	*/
	void GetSessionId(const THandler<FAccelByteModelsServerSessionResponse>& OnSuccess, const FErrorHandler& OnError);

	void SetServerName(const FString Name){ ServerName = Name; };
	void SetServerType(EServerType Type){ ServerType = Type; };

	FString DSMServerUrl;
	FString DSPubIp;

	/*
	* @brief Update values from command line arguments. Used for tests.
	*/
	void ParseCommandParam();

private:
	ServerDSM(ServerDSM const&) = delete; // Copy constructor
	ServerDSM(ServerDSM&&) = delete; // Move constructor
	ServerDSM& operator=(ServerDSM const&) = delete; // Copy assignment operator
	ServerDSM& operator=(ServerDSM &&) = delete; // Move assignment operator

	void GetPubIp(const THandler<FAccelByteModelsPubIp>& OnSuccess, const FErrorHandler& OnError);

	FString ServerName = "";
	FString Provider = "";
	FString Region = "";
	FString Game_version = "";
	EServerType ServerType = EServerType::NONE;
	FHttpRequestCompleteDelegate OnRegisterResponse;
	THandler<FAccelByteModelsPubIp> GetPubIpDelegate;
};

	} // Namespace GameServerApi
} // Namespace AccelByte
