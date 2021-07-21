// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteSessionBrowserModels.h"

class FJsonObject;

namespace AccelByte
{
class Credentials;
class Settings;
namespace Api
{

/**
* @brief Leaderboard API to obtain player's ranking in a namespace.
*/

class ACCELBYTEUE4SDK_API SessionBrowser 
{
public:
	SessionBrowser(const Credentials& Credentials, const Settings& Settings);
	~SessionBrowser();
private:
	const Credentials& Credentials;
	const Settings& Settings;

public:
	/**
	* @brief Create the session to Session Browser.
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
	void CreateGameSession(const FString &GameMode, const FString &GameMapName, const FString &GameVersion, uint32 BotCount, uint32 MaxPlayer, TSharedPtr<FJsonObject> OtherSettings, const THandler<FAccelByteModelsSessionBrowserData>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Update the session to Session Browser.
	*
	* @param SessionId id of the session want to update.
	* @param CurrentPlayerCount current number of player in the session.
	* @param MaxPlayer maximum number of player can join the session.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void UpdateGameSession(const FString& SessionId, uint32 MaxPlayer, uint32 CurrentPlayerCount, const THandler<FAccelByteModelsSessionBrowserData>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Remove the session to Session Browser.
	*
	* @param SessionId id of the session want to update.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	*/
	void RemoveGameSession(const FString& SessionId, const THandler<FAccelByteModelsSessionBrowserData>& OnSuccess, const FErrorHandler& OnError);

	/**
	* @brief Query game session.
	*
	* @param SessionType id of the session want to update.
	* @param GameMode id of the session want to update.
	* @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsSessionBrowserData.
	* @param OnError This will be called when the operation failed.
	* @param Offset offset of the query
	* @param Limit number of data to return
	*/
	void GetGameSessions(const FString &SessionType, const FString &GameMode, const THandler<FAccelByteModelsSessionBrowserGetResult>& OnSuccess, const FErrorHandler& OnError, uint32 Offset = 0, uint32 Limit = 50);

private:
	SessionBrowser() = delete;
	SessionBrowser(SessionBrowser const&) = delete;
	SessionBrowser(SessionBrowser&&) = delete;

	FString GetSessionBrowserUrl();
};

} // Namespace Api
} // Namespace AccelByte