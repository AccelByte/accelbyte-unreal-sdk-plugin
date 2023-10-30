// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerSessionBrowserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerSessionBrowser::ServerSessionBrowser(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

ServerSessionBrowser::~ServerSessionBrowser()
{}

void ServerSessionBrowser::CreateGameSession(FString const& GameMode
	, FString const& GameMapName
	, FString const& GameVersion
	, uint32 BotCount
	, uint32 MaxPlayer
	, TSharedPtr<FJsonObject> OtherSettings
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	CreateGameSession(GameMode
		, GameMapName
		, GameVersion
		, BotCount
		, MaxPlayer
		, 0
		, FString()
		, OtherSettings
		, OnSuccess
		, OnError);
}

void ServerSessionBrowser::CreateGameSession(FString const& GameMode
	, FString const& GameMapName
	, FString const& GameVersion
	, uint32 BotCount
	, uint32 MaxPlayer
	, uint32 MaxSpectator
	, FString const& Password
	, TSharedPtr<FJsonObject> OtherSettings
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	CreateGameSession(EAccelByteSessionType::p2p
		, GameMode
		, GameMapName
		, GameVersion
		, BotCount
		, MaxPlayer
		, MaxSpectator
		, Password
		, OtherSettings
		, OnSuccess
		, OnError);
}

void ServerSessionBrowser::CreateGameSession(FString const& SessionTypeString
	, FString const& GameMode
	, FString const& GameMapName
	, FString const& GameVersion
	, uint32 BotCount
	, uint32 MaxPlayer
	, uint32 MaxSpectator
	, FString const& Password
	, TSharedPtr<FJsonObject> OtherSettings
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	EAccelByteSessionType SessionType = FAccelByteUtilities::GetUEnumValueFromString<EAccelByteSessionType>(SessionTypeString);
	CreateGameSession(SessionType
		, GameMode
		, GameMapName
		, GameVersion
		, BotCount
		, MaxPlayer
		, MaxSpectator
		, Password
		, OtherSettings
		, OnSuccess
		, OnError);
}

void ServerSessionBrowser::CreateGameSession(EAccelByteSessionType SessionType
	, FString const& GameMode
	, FString const& GameMapName
	, FString const& GameVersion
	, uint32 BotCount
	, uint32 MaxPlayer
	, uint32 MaxSpectator
	, FString const& Password
	, TSharedPtr<FJsonObject> OtherSettings
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FString SessionTypeString = FAccelByteUtilities::GetUEnumValueAsString(SessionType);

	FAccelByteModelsSessionBrowserCreateRequest NewGameSession;
	NewGameSession.Namespace = ServerCredentialsRef.GetClientNamespace();
	NewGameSession.Session_type = SessionTypeString;
	NewGameSession.Game_session_setting.Mode = GameMode;
	NewGameSession.Game_session_setting.Map_name = GameMapName;
	NewGameSession.Game_session_setting.Num_bot = BotCount;
	NewGameSession.Game_session_setting.Max_player = MaxPlayer;
	NewGameSession.Game_session_setting.Current_player = 1;
	NewGameSession.Game_session_setting.Max_internal_player = MaxSpectator;
	NewGameSession.Game_session_setting.Current_internal_player = 0;
	
	if(!Password.IsEmpty())
	{
		NewGameSession.Game_session_setting.Password = FMD5::HashAnsiString(*Password);
	}
	
	NewGameSession.Game_session_setting.Settings.JsonObject = OtherSettings;
	NewGameSession.Game_version = GameVersion;

	CreateGameSession(NewGameSession, OnSuccess, OnError);
}

void ServerSessionBrowser::CreateGameSession(FAccelByteModelsSessionBrowserCreateRequest const& CreateSessionRequest
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	EAccelByteSessionType SessionType = FAccelByteUtilities::GetUEnumValueFromString<EAccelByteSessionType>(CreateSessionRequest.Session_type);
	
	if (SessionType == EAccelByteSessionType::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Wrong session type"));
		return;
	}
	
	if (CreateSessionRequest.Game_session_setting.Max_player == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Max players must greather then 0"));
		return;
	}

	if (CreateSessionRequest.Game_session_setting.Mode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Game mode can't be empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession")
		, *ServerSettingsRef.SessionBrowserServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, CreateSessionRequest, OnSuccess, OnError);
}

void ServerSessionBrowser::UpdateGameSession(FString const& SessionId
	, uint32 MaxPlayer
	, uint32 CurrentPlayerCount
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FAccelByteModelsSessionBrowserUpdateRequest UpdateSessionRequest;
	UpdateSessionRequest.Game_max_player = MaxPlayer;
	UpdateSessionRequest.Game_current_player = CurrentPlayerCount;

	UpdateGameSession(SessionId, UpdateSessionRequest, OnSuccess, OnError);
}

void ServerSessionBrowser::UpdateGameSession(FString const& SessionId
	, uint32 MaxPlayer
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FAccelByteModelsSessionBrowserUpdateRequest UpdateSessionRequest;
	UpdateSessionRequest.Game_max_player = MaxPlayer;

	UpdateGameSession(SessionId, UpdateSessionRequest, OnSuccess, OnError);
}

void ServerSessionBrowser::UpdateGameSession(FString const& SessionId
	, FAccelByteModelsSessionBrowserUpdateRequest const& UpdateSessionRequest
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	if(UpdateSessionRequest.Game_current_player != 0)
	{
		FReport::LogDeprecated(FString(__FUNCTION__),
			TEXT("Update player count deprecated from UpdateGameSession. Use register/unregister player to update player count"));
	}
	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return;
	}

	if (UpdateSessionRequest.Game_max_player == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Max player must greather then 0"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s")
		, *ServerSettingsRef.SessionBrowserServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *SessionId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, UpdateSessionRequest, OnSuccess, OnError);
}

void ServerSessionBrowser::RemoveGameSession(FString const& SessionId
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s")
		, *ServerSettingsRef.SessionBrowserServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *SessionId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerSessionBrowser::GetGameSessions(FString const& SessionTypeString
	, FString const& GameMode
	, THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	GetGameSessions(SessionTypeString
		, GameMode
		, FString()
		, OnSuccess
		, OnError
		, Offset
		, Limit);
}

void ServerSessionBrowser::GetGameSessions(EAccelByteSessionType SessionType
	, FString const& GameMode
	, THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	GetGameSessions(SessionType
		, GameMode
		, FString()
		, OnSuccess
		, OnError
		, Offset
		, Limit);
}

void ServerSessionBrowser::GetGameSessions(FString const& SessionTypeString
	, FString const& GameMode
	, FString const& MatchExist
	, THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	EAccelByteSessionType SessionType = FAccelByteUtilities::GetUEnumValueFromString<EAccelByteSessionType>(SessionTypeString);
	GetGameSessions(SessionType
		, GameMode
		, MatchExist
		, OnSuccess
		, OnError
		, Offset
		, Limit);
}

void ServerSessionBrowser::GetGameSessions(EAccelByteSessionType SessionType
	, FString const& GameMode
	, FString const& MatchExist
	, THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (SessionType == EAccelByteSessionType::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Wrong session type"));
		return;
	}

	const FString SessionTypeString = FAccelByteUtilities::GetUEnumValueAsString(SessionType);
	
	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession")
		, *ServerSettingsRef.SessionBrowserServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	TMultiMap<FString, FString> QueryParams = {
		{ TEXT("session_type"), *SessionTypeString },
		{ TEXT("game_mode"), *GameMode },
		{ TEXT("joinable"), TEXT("true") },
		{ TEXT("match_exist"), *MatchExist },
	};

	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerSessionBrowser::GetGameSessionBySessionId(FString const& SessionId
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s")
		, *ServerSettingsRef.SessionBrowserServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *SessionId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerSessionBrowser::RegisterPlayer(FString const& SessionId
	, FString const& PlayerToAdd
	, bool AsSpectator
	, THandler<FAccelByteModelsSessionBrowserAddPlayerResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return;
	}

    const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/player")
    	, *ServerSettingsRef.SessionBrowserServerUrl
    	, *ServerCredentialsRef.GetClientNamespace()
    	, *SessionId);

	FAccelByteModelsSessionBrowserAddPlayerRequest AddPlayerRequest;
	AddPlayerRequest.User_id = PlayerToAdd;
	AddPlayerRequest.As_spectator = AsSpectator;

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, AddPlayerRequest, OnSuccess, OnError);
}

void ServerSessionBrowser::UnregisterPlayer(FString const& SessionId
	, FString const& PlayerToRemove
	, THandler<FAccelByteModelsSessionBrowserAddPlayerResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/player/%s")
		, *ServerSettingsRef.SessionBrowserServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *SessionId
		, *PlayerToRemove);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerSessionBrowser::GetRecentPlayer(FString const& UserId
	, THandler<FAccelByteModelsSessionBrowserRecentPlayerGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/recentplayer/%s")
		, *ServerSettingsRef.SessionBrowserServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	TMultiMap<FString, FString> QueryParams;
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerSessionBrowser::JoinSession(FString const& SessionId
	, FString const& Password
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/join")
		, *ServerSettingsRef.SessionBrowserServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *SessionId);

	FAccelByteModelsSessionBrowserJoinSessionRequest JoinRequest;
	if(!Password.IsEmpty())
	{
		JoinRequest.Password = FMD5::HashAnsiString(*Password);			
	}

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, JoinRequest, OnSuccess, OnError);
}

}
}