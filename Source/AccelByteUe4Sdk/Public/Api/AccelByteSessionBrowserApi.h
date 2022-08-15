// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Models/AccelByteSessionBrowserModels.h"

class FJsonObject;

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
* @brief SessionBrowser API to handle custom game session.
*/

class ACCELBYTEUE4SDK_API SessionBrowser 
{
public:
	SessionBrowser(Credentials const& InCredentialsRef, Settings const& InSettingsRef, FHttpRetryScheduler& InHttpRef);
	~SessionBrowser();
private:
	FHttpRetryScheduler& HttpRef;
	Credentials const& CredentialsRef;
	Settings const& SettingsRef;

public:
	/**
	* @brief Create public P2P game session to Session Browser.
	*
	* @param GameMode game mode of the game.
	* @param GameMapName map of the game.
	* @param GameVersion version of the played game.
	* @param BotCount number of bot.
	* @param MaxPlayer maximum number of player can join the session.
	* @param OtherSettings other setting in json format
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateGameSession(
		FString const& GameMode,
		FString const& GameMapName,
		FString const& GameVersion,
		uint32 BotCount,
		uint32 MaxPlayer,
		TSharedPtr<FJsonObject> OtherSettings,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);
	
	/**
	* @brief Create private P2P game session to Session Browser.
	*
	* @param GameMode game mode of the game.
	* @param GameMapName map of the game.
	* @param GameVersion version of the played game.
	* @param BotCount number of bot.
	* @param MaxPlayer maximum number of player can join the session.
	* @param Password password of the game session
	* @param MaxSpectator maximum of spectator
	* @param OtherSettings other setting in json format
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateGameSession(
		FString const& GameMode,
		FString const& GameMapName,
		FString const& GameVersion,
		uint32 BotCount,
		uint32 MaxPlayer,
		uint32 MaxSpectator,
		FString const& Password,
		TSharedPtr<FJsonObject> OtherSettings,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief Create custom game session to Session Browser specified by the type.
	*
	* @param SessionTypeString type of the session: p2p or dedicated
	* @param GameMode game mode of the game.
	* @param GameMapName map of the game.
	* @param GameVersion version of the played game.
	* @param BotCount number of bot.
	* @param MaxPlayer maximum number of player can join the session.
	* @param Password password of the game session
	* @param MaxSpectator maximum of spectator
	* @param OtherSettings other setting in json format
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateGameSession(
		FString const& SessionTypeString,
		FString const& GameMode,
		FString const& GameMapName,
		FString const& GameVersion,
		uint32 BotCount,
		uint32 MaxPlayer, 
		uint32 MaxSpectator, 
		FString const& Password,
		TSharedPtr<FJsonObject> OtherSettings,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief Create custom game session to Session Browser specified by the type.
	*
	* @param SessionType type of the session: p2p or dedicated
	* @param GameMode game mode of the game.
	* @param GameMapName map of the game.
	* @param GameVersion version of the played game.
	* @param BotCount number of bot.
	* @param MaxPlayer maximum number of player can join the session.
	* @param Password password of the game session
	* @param MaxSpectator maximum of spectator
	* @param OtherSettings other setting in json format
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateGameSession(
		EAccelByteSessionType SessionType,
		FString const& GameMode,
		FString const& GameMapName,
		FString const& GameVersion, 
		uint32 BotCount,
		uint32 MaxPlayer,
		uint32 MaxSpectator,
		FString const& Password,
		TSharedPtr<FJsonObject> OtherSettings,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief Create custom game session to Session Browser specified by the type.
	*
	* @param CreateSessionRequest create session parameters.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void CreateGameSession(
		FAccelByteModelsSessionBrowserCreateRequest const& CreateSessionRequest,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief Update the session to Session Browser.
	*
	* @param SessionId id of the session want to update.
	* @param CurrentPlayerCount current number of player in the session.
	* @param MaxPlayer maximum number of player can join the session.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void UpdateGameSession(
		FString const& SessionId,
		uint32 MaxPlayer,
		uint32 CurrentPlayerCount,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief Update the session to Session Browser.
	*
	* @param SessionId id of the session want to update.
	* @param UpdateSessionRequest update session parameters.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void UpdateGameSession(
		FString const& SessionId,
		FAccelByteModelsSessionBrowserUpdateRequest const& UpdateSessionRequest,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief Update the session to Session Browser.
	*
	* @param SessionId id of the session want to update.
	* @param Settings update settings parameters.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void UpdateGameSettings(
		FString const& SessionId,
		TMap<FString, FString> Settings,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief Update the session to Session Browser.
	*
	* @param SessionId id of the session want to update.
	* @param Settings update settings parameters.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void UpdateGameSettings(
		FString const& SessionId,
		TSharedPtr<FJsonObject> Settings,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief Remove the session to Session Browser.
	*
	* @param SessionId id of the session want to update.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void RemoveGameSession(
		FString const& SessionId,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief Query game session.
	*
	* @param SessionTypeString the session type, either "dedicated", opr "p2p".
	* @param GameMode the game mode of the session to query.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserGetResult.
	* @param OnError This will be called when the operation failed.
	* @param Offset offset of the query
	* @param Limit number of data to return
	*/
	void GetGameSessions(
		FString const& SessionTypeString,
		FString const& GameMode,
		THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess,
		FErrorHandler const& OnError,
		uint32 Offset = 0, 
		uint32 Limit = 10);

	/**
	* @brief Query game session.
	*
	* @param SessionType the session type, either "dedicated", opr "p2p".
	* @param GameMode the game mode of the session to query.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserGetResult.
	* @param OnError This will be called when the operation failed.
	* @param Offset offset of the query
	* @param Limit number of data to return
	*/
	void GetGameSessions(
		EAccelByteSessionType SessionType,
		FString const& GameMode,
		THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess,
		FErrorHandler const& OnError,
		uint32 Offset = 0,
		uint32 Limit = 10);

	/**
	* @brief Query game session.
	*
	* @param SessionTypeString the session type, either "dedicated", opr "p2p".
	* @param GameMode the game mode of the session to query.
	* @param MatchExist value will be true, false or empty
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserGetResult.
	* @param OnError This will be called when the operation failed.
	* @param Offset offset of the query
	* @param Limit number of data to return
	*/
	void GetGameSessions(
		FString const& SessionTypeString,
		FString const& GameMode,
		FString const& MatchExist,
		THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess,
		FErrorHandler const& OnError,
		uint32 Offset = 0,
		uint32 Limit = 10);

	/**
	* @brief Query game session.
	*
	* @param SessionType the session type, either "dedicated", opr "p2p".
	* @param GameMode the game mode of the session to query.
	* @param MatchExist value will be true, false or empty
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserGetResult.
	* @param OnError This will be called when the operation failed.
	* @param Offset offset of the query
	* @param Limit number of data to return
	*/
	void GetGameSessions(
		EAccelByteSessionType SessionType,
		FString const& GameMode,
		FString const& MatchExist,
		THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess,
		FErrorHandler const& OnError,
		uint32 Offset = 0,
		uint32 Limit = 10);

	/**
	* @brief Query game session by user ids
	*
	* @param UserIds List of user id
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserGetByUserIdsResult.
	* @param OnError This will be called when the operation failed.
	*/
	void GetGameSessionsByUserIds(
		const TArray<FString>& UserIds,
		THandler<FAccelByteModelsSessionBrowserGetByUserIdsResult> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief RegisterPlayer to game session
	*
	* @param SessionId id of the session
	* @param PlayerToAdd player to add
	* @param AsSpectator indicates as spectator
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserAddPlayerResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void RegisterPlayer(
		FString const& SessionId,
		FString const& PlayerToAdd,
		bool AsSpectator,
		THandler<FAccelByteModelsSessionBrowserAddPlayerResponse> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief RegisterPlayer to game session
	*
	* @param SessionId id of the session
	* @param PlayerToRemove player to remove from game session
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserAddPlayerResponse.
	* @param OnError This will be called when the operation failed.
	*/
	void UnregisterPlayer(
		FString const& SessionId,
		FString const& PlayerToRemove, 
		THandler<FAccelByteModelsSessionBrowserAddPlayerResponse> const& OnSuccess,
		FErrorHandler const& OnError);

	/**
	* @brief GetRecentPlayer to game session
	*
	* @param UserId id of the user
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserRecentPlayerGetResult.
	* @param OnError This will be called when the operation failed.
	* @param Offset offset of the query
	* @param Limit number of data to return
	*/
	void GetRecentPlayer(
		FString const& UserId,
		THandler<FAccelByteModelsSessionBrowserRecentPlayerGetResult> const& OnSuccess,
		FErrorHandler const& OnError,
		uint32 Offset = 0,
		uint32 Limit = 10);

	/**
	* @brief JoinSession to game session
	*
	* @param SessionId id of the session
	* @param Password password to join if required
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void JoinSession(
		FString const& SessionId,
		FString const& Password,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess, 
		FErrorHandler const& OnError);

	/**
	* @brief Get game session data by session ID.
	*
	* @param SessionId id of the session
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void GetGameSession(
		FString const& SessionId,
		THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess,
		FErrorHandler const& OnError);

private:
	SessionBrowser() = delete;
	SessionBrowser(SessionBrowser const&) = delete;
	SessionBrowser(SessionBrowser&&) = delete;
};

} // Namespace Api
} // Namespace AccelByte