// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteDefines.h"
#include "Core/AccelByteServerApiBase.h"
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
class ACCELBYTEUE4SDK_API ServerDSM : public FServerApiBase
{
public:
	ServerDSM(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef
		, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient = nullptr);
	~ServerDSM();

	/**
	 * @brief send register request to DSM
	 *
	 * @param Port the port where your game server run.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
 	 * @param CustomAttribute A value that will be sent to client that join via armada matchmaking DSNotice event
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RegisterServerToDSM(int32 Port
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& CustomAttribute = TEXT(""));

	/**
	 * @brief send register request to DSM
	 *
	 * @param Port the port where your game server run.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param CustomAttribute A value that will be sent to client that join via armada matchmaking DSNotice event
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RegisterServerToDSM(int32 Port
		, THandler<FAccelByteModelsServerInfo> const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& CustomAttribute = TEXT(""));


	/**
	 * @brief send register local server to DSM
	 *
	 * @param IPAddress the machine's IP address this local DS is in.
	 * @param Port the port where your game server run.
	 * @param ServerName the server name of this local DS
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param CustomAttribute A value that will be sent to client that join via armada matchmaking DS Notice event
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RegisterLocalServerToDSM(FString const& IPAddress
    	, int32 Port
    	, FString const& ServerName
    	, FVoidHandler const& OnSuccess
    	, FErrorHandler const& OnError
    	, FString const& CustomAttribute = TEXT(""));

	/**
	 * @brief send register local server to DSM using public IP
	 * this method is using api.ipify.org to get the device's public IP
	 *
	 * @param Port the port where your game server run.
	 * @param ServerName the server name of this local DS.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * @param CustomAttribute A value that will be sent to client that join via armada matchmaking DSNotice event
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RegisterLocalServerToDSM(int32 Port
		, FString const& ServerName
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError
		, FString const& CustomAttribute = TEXT(""));

	/**
	 * @brief send shutdown request to DSM
	 *
	 * @param KillMe true if you want DSM to kill your DS. false if your DS already on shutting down state.
	 * @param MatchId Id that you get from DSM-MatchmakingInfoNotif.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed. 
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr SendShutdownToDSM(bool KillMe
		, FString const& MatchId
		, FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief deregister local server to DSM
	 *
	 * @param ServerName the server name of this local DS.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed. 
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr DeregisterLocalServerFromDSM(FString const& ServerName
    	, FVoidHandler const& OnSuccess
    	, FErrorHandler const& OnError);

	/**
	 * @brief Register server's game session to DSM
	 *
	 * @param Request the request body.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RegisterServerGameSession(FAccelByteModelsServerCreateSessionRequest const& Request
		, THandler<FAccelByteModelsServerCreateSessionResponse> const& OnSuccess
		, FErrorHandler const& OnError);
	
	/**
	 * @brief Register server's game session to DSM
	 *
	 * @param SessionId the Session ID of the server.
	 * @param GameMode the matchmaker's game mode the server will use.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr RegisterServerGameSession(FString const& SessionId
		, FString const& GameMode
		, THandler<FAccelByteModelsServerCreateSessionResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get Session ID of a claimed DS. Will return empty string OnSuccess if DS is not claimed yet.
	 *
	 * @param OnSuccess This will be called when the operation succeeded, The result is const FAccelByteModelsServerSessionResponse.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetSessionId(THandler<FAccelByteModelsServerSessionResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Get Server Info Will return server information.
	 *
	 * @param OnSuccess This will be called when the operation succeeded, The result is const FAccelByteModelsServerInfo.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetServerInfo(THandler<FAccelByteModelsServerInfo> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief GetServerTimeout
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr GetSessionTimeout(THandler<FAccelByteModelsServerTimeoutResponse> const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief ServerHeartbeat
	 *
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 * 
	 * @return AccelByteTask object to track and cancel the ongoing API operation.
	 */
	FAccelByteTaskWPtr ServerHeartbeat(FVoidHandler const& OnSuccess
		, FErrorHandler const& OnError);

	/**
	 * @brief Configure automatic shutdown, server will send shutdown request to DSM when there is no player on the DS for some periodical
	 *		time after the DS is claimed. Must be called before calling RegisterServerToDSM or RegisterLocalServerToDSM
	 *
	 * @param TickSeconds Tick Second to check the auto shutdown, not bigger than CountdownStart
	 * @param CountdownStart Countdown for DS Auto Shutdown, set to -1 to disable the Auto Shutdown
	 */
	void ConfigureAutoShutdown(uint32 TickSeconds = 5
		, int CountdownStart = 30);

	/**
	 * @brief Set handler delegate for OnAutoShutdown event when DS send shutdown request after no player activity
	 *
	 * @param OnAutoShutdown_ This delegate will be called if Auto Shutdown is successful
	 */
	void SetOnAutoShutdownResponse(FVoidHandler const& OnAutoShutdown_);

	/**
	 * @brief Set handler delegate for OnAutoShutdownError event when DS failed do Shutting down from auto shutdown
	 *
	 * @param OnError This delegate will be called if DS failed do Shutting down from auto shutdown
	 */
	void SetOnAutoShutdownErrorDelegate(FErrorHandler const& OnError);

	/**
	 * @brief Set handler delegate for OnHeartbeatSuccess event
	 *
	 * @param OnHeartbeatSuccess This delegate will be called if the DS heartbeat is sent
	 */
	void SetOnHeartbeatSuccessDelegate(FVoidHandler const& OnHeartbeatSuccess);

	/**
	 * @brief Set handler delegate for OnHeartbeatError event
	 *
	 * @param OnHeartbeatError This delegate will be called if the DS heartbeat is not sent
	 */
	void SetOnHeartbeatErrorDelegate(FErrorHandler const& OnHeartbeatError);

	/**
	 * @brief Start sending heartbeat at interval.
	 *
	 * @param IntervalSeconds
	 */
	void StartHeartbeat(uint32 IntervalSeconds);

	/**
	 * @brief Stop Heartbeat.
	 */
	void StopHeartbeat();

	void SetServerName(FString const& Name) { ServerName = Name; };
	void SetServerType(EServerType Type) { ServerType = Type; };
	FString GetServerName() { return ServerName; };
	int32 GetPlayerNum();

	FString DSMServerUrl = TEXT("");
	FString DSPubIp = TEXT("");

	/**
	 * @brief Update values from command line arguments. Used for tests.
	 */
	void ParseCommandParam();

private:
	ServerDSM(ServerDSM const&) = delete; // Copy constructor
	ServerDSM(ServerDSM&&) = delete; // Move constructor
	ServerDSM& operator=(ServerDSM const&) = delete; // Copy assignment operator
	ServerDSM& operator=(ServerDSM &&) = delete; // Move assignment operator

	bool ShutdownTick(float DeltaTime);
	bool PeriodicHeartbeat(float DeltaTime);

	FString ServerName = TEXT("");
	FString Provider = TEXT("");
	FString Region = TEXT("");
	FString GameVersion = TEXT("");
	EServerType ServerType = EServerType::NONE;
	FHttpRequestCompleteDelegate OnRegisterResponse;
	FVoidHandler OnAutoShutdown;
	FVoidHandler OnHeartbeatSuccess;
	bool bIsDSClaimed = false;
	uint32 ShutdownTickSeconds = 0;
	uint32 HeartbeatTickSeconds = 0;
	int ShutdownCountdown = 0;
	int CountdownTimeStart = -1;
	FErrorHandler OnAutoShutdownError;
	FErrorHandler OnHeartbeatError;
	FTickerDelegate AutoShutdownDelegate;
	FTickerDelegate HeartbeatDelegate;
	FDelegateHandleAlias AutoShutdownDelegateHandle;
	FDelegateHandleAlias HeartbeatDelegateHandle;
	THandler<FAccelByteModelsPubIp> GetPubIpDelegate;
	FAccelByteModelsServerInfo RegisteredServerInfo;
};

} // Namespace GameServerApi
} // Namespace AccelByte
