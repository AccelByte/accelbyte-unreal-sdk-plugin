// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteDefines.h"
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
	ServerDSM(ServerCredentials const& InCredentialsRef, ServerSettings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~ServerDSM();

	/*
	 * @brief send register request to DSM
	 *
	 * @param Port the port where your game server run.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
 	 * @param CustomAttribute A value that will be sent to client that join via armada matchmaking DSNotice event
	*/
	void RegisterServerToDSM(const int32 Port, const FVoidHandler& OnSuccess, const FErrorHandler& OnError,
		const FString& CustomAttribute = "");


    /*
	 * @brief send register local server to DSM
	 *
	 * @param IPAddress the machine's IP address this local DS is in.
	 * @param Port the port where your game server run.
	 * @param ServerName the server name of this local DS
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param CustomAttribute A value that will be sent to client that join via armada matchmaking DS Notice event
	*/
    void RegisterLocalServerToDSM(const FString IPAddress, const int32 Port, const FString ServerName,
    	const FVoidHandler& OnSuccess, const FErrorHandler& OnError, const FString& CustomAttribute = "");

	/*
	 * @brief send register local server to DSM using public IP
	 * this method is using api.ipify.org to get the device's public IP
	 *
	 * @param Port the port where your game server run.
	 * @param ServerName the server name of this local DS
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param CustomAttribute A value that will be sent to client that join via armada matchmaking DSNotice event
	*/
	void RegisterLocalServerToDSM(const int32 Port, const FString ServerName, const FVoidHandler& OnSuccess,
		const FErrorHandler& OnError, const FString& CustomAttribute = "");

	/*
	 * @brief send shutdown request to DSM
	 *
	 * @param KillMe true if you want DSM to kill your DS. false if your DS already on shutting down state.
	 * @param MatchId Id that you get from DSM-MatchmakingInfoNotif.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed. 
	*/
	void SendShutdownToDSM(const bool KillMe, const FString& MatchId, const FVoidHandler& OnSuccess,
		const FErrorHandler& OnError);


	/*
	 * @brief deregister local server to DSM
	 *
	 * @param KillMe true if you want DSM to kill your DS. false if your DS already on shutting down state.
	 * @param MatchId Id that you get from DSM-MatchmakingInfoNotif.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed. 
	*/
    void DeregisterLocalServerFromDSM(const FString& ServerName, const FVoidHandler& OnSuccess,
    	const FErrorHandler& OnError);

	/*
	* @brief Register server's game session to DSM
	*
	* @param Request the request body.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed. 
	*/
	void RegisterServerGameSession(const FAccelByteModelsServerCreateSessionRequest& Request,
		const THandler<FAccelByteModelsServerCreateSessionResponse>& OnSuccess, const FErrorHandler& OnError);
	
	/*
	* @brief Register server's game session to DSM
	*
	* @param SessionId the Session ID of the server.
	* @param GameMode the matchmaker's game mode the server will use.
	* @param OnSuccess This will be called when the operation succeeded.
	* @param OnError This will be called when the operation failed. 
	*/
	void RegisterServerGameSession(const FString& SessionId, const FString& GameMode,
		const THandler<FAccelByteModelsServerCreateSessionResponse>& OnSuccess, const FErrorHandler& OnError);

	/*
	 * @brief Get Session ID of a claimed DS. Will return empty string OnSucess if DS is not claimed yet.
	 *
	 * @param OnSuccess This will be called when the operation succeeded, The result is const FAccelByteModelsServerSessionResponse.
	 * @param OnError This will be called when the operation failed.
	*/
	void GetSessionId(const THandler<FAccelByteModelsServerSessionResponse>& OnSuccess, const FErrorHandler& OnError);

	/*
	 * @brief Get Server Info Will return server information.
	 *
	 * @param OnSuccess This will be called when the operation succeeded, The result is const FAccelByteModelsServerInfo.
	 * @param OnError This will be called when the operation failed.
	*/
	void GetServerInfo(const THandler<FAccelByteModelsServerInfo>& OnSuccess, const FErrorHandler& OnError);

	/*
	 * @brief Configure automatic shutdown, server will send shutdown request to DSM when there is no player on the DS for some periodical time after the DS is claimed. Must be called before calling RegisterServerToDSM or RegisterLocalServerToDSM
	 *
	 * @param TickSeconds Tick Second to check the auto shutdown, not bigger than CountdownStart
	 * @param CountdownStart Countdown for DS Auto Shutdown, set to -1 to disable the Auto Shutdown
	*/
	void ConfigureAutoShutdown(uint32 TickSeconds = 5, int CountdownStart = 30);

	/*
	 * @brief Set handler delegate for OnAutoShutdown event when DS send shutdown request after no player activity
	 *
	 * @param OnAutoShutdown This delegate will be called if Auto Shutdown is successful
	*/
	void SetOnAutoShutdownResponse(FVoidHandler OnAutoShutdown_);

	/*
	 * @brief Set handler delegate for OnAutoShutdownError event when DS failed do Shutting down from auto shutdown
	 *
	 * @param OnAutoShutdownError This delegate will be called if DS failed do Shutting down from auto shutdown
	*/
	void SetOnAutoShutdownErrorDelegate(const FErrorHandler& OnError);

	void SetServerName(const FString Name) { ServerName = Name; };
	void SetServerType(EServerType Type) { ServerType = Type; };
	int32 GetPlayerNum();

	FString DSMServerUrl;
	FString DSPubIp = "";

	/*
	* @brief Update values from command line arguments. Used for tests.
	*/
	void ParseCommandParam();

private:
	ServerCredentials const& CredentialsRef;
	ServerSettings const& SettingsRef;
	FHttpRetryScheduler& HttpRef;

	ServerDSM(ServerDSM const&) = delete; // Copy constructor
	ServerDSM(ServerDSM&&) = delete; // Move constructor
	ServerDSM& operator=(ServerDSM const&) = delete; // Copy assignment operator
	ServerDSM& operator=(ServerDSM &&) = delete; // Move assignment operator

	bool ShutdownTick(float DeltaTime);

	FString ServerName = "";
	FString Provider = "";
	FString Region = "";
	FString Game_version = "";
	EServerType ServerType = EServerType::NONE;
	FHttpRequestCompleteDelegate OnRegisterResponse;
	FVoidHandler OnAutoShutdown;
	bool bIsDSClaimed = false;
	uint32 ShutdownTickSeconds = 0;
	int ShutdownCountdown = 0;
	int CountdownTimeStart = -1;
	FErrorHandler OnAutoShutdownError;
	FTickerDelegate AutoShutdownDelegate;
	FDelegateHandleAlias AutoShutdownDelegateHandle;
	THandler<FAccelByteModelsPubIp> GetPubIpDelegate;
	FAccelByteModelsServerInfo RegisteredServerInfo;
};

	} // Namespace GameServerApi
} // Namespace AccelByte
