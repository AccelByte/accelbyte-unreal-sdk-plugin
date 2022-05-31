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
		: CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
		, HttpRef{InHttpRef}
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
		NewGameSession.Namespace = CredentialsRef.GetClientNamespace();
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
			OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Wrong session type"));
			return;
		}
		
		if (CreateSessionRequest.Game_session_setting.Max_player == 0)
		{
			OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Max players must greather then 0"));
			return;
		}

		if (CreateSessionRequest.Game_session_setting.Mode.IsEmpty())
		{
			OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Game mode can't be empty!"));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession"), *SettingsRef.SessionBrowserServerUrl, *CredentialsRef.GetClientNamespace());
		FString Verb = TEXT("POST");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FString Content;
		FJsonObjectConverter::UStructToJsonObjectString(CreateSessionRequest, Content);
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
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
		, FAccelByteModelsSessionBrowserUpdateRequest const& UpdateSessionRequest
		, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
		, FErrorHandler const& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		if (SessionId.IsEmpty())
		{
			OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
			return;
		}

		if (UpdateSessionRequest.Game_max_player == 0)
		{
			OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Max player must greather then 0"));
			return;
		}
		
		if (UpdateSessionRequest.Game_max_player < UpdateSessionRequest.Game_current_player)
		{
			OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Max players should NOT be less than current players"));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s"), *SettingsRef.SessionBrowserServerUrl, *CredentialsRef.GetClientNamespace(), *SessionId);
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FString Content;
		FJsonObjectConverter::UStructToJsonObjectString(UpdateSessionRequest, Content);
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerSessionBrowser::RemoveGameSession(FString const& SessionId
		, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
		, FErrorHandler const& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		if (SessionId.IsEmpty())
		{
			OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s"), *SettingsRef.SessionBrowserServerUrl, *CredentialsRef.GetClientNamespace(), *SessionId);
		FString Verb = TEXT("DELETE");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
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
			OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Wrong session type"));
			return;
		}

		FString SessionTypeString = FAccelByteUtilities::GetUEnumValueAsString(SessionType);
		
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession?session_type=%s&game_mode=%s&joinable=true&match_exist=%s&limit=%d&offset=%d"), *SettingsRef.SessionBrowserServerUrl, *CredentialsRef.GetClientNamespace(), *SessionTypeString, *GameMode, *MatchExist, Limit, Offset);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerSessionBrowser::GetGameSessionBySessionId(FString const& SessionId
		, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
		, FErrorHandler const& OnError)
	{
		FReport::Log(FString(__FUNCTION__));
		
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s"), *SettingsRef.SessionBrowserServerUrl, *CredentialsRef.GetClientNamespace(), *SessionId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
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

		const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
        const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/player"), *SettingsRef.SessionBrowserServerUrl, *CredentialsRef.GetClientNamespace(), *SessionId);
        const FString Verb = TEXT("POST");
        const FString ContentType = TEXT("application/json");
        const FString Accept = TEXT("application/json");

		FAccelByteModelsSessionBrowserAddPlayerRequest AddPlayerRequest;
		AddPlayerRequest.User_id = PlayerToAdd;
		AddPlayerRequest.As_spectator = AsSpectator;
		FString Content;
		FJsonObjectConverter::UStructToJsonObjectString(AddPlayerRequest, Content);
		
        FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
        Request->SetURL(Url);
        Request->SetHeader(TEXT("Authorization"), Authorization);
        Request->SetVerb(Verb);
        Request->SetHeader(TEXT("Content-Type"), ContentType);
        Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
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

		const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/player/%s"), *SettingsRef.SessionBrowserServerUrl, *CredentialsRef.GetClientNamespace(), *SessionId, *PlayerToRemove);
		const FString Verb = TEXT("DELETE");
		const FString ContentType = TEXT("application/json");
		const FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerSessionBrowser::GetRecentPlayer(FString const& UserId
		, THandler<FAccelByteModelsSessionBrowserRecentPlayerGetResult> const& OnSuccess
		, FErrorHandler const& OnError
		, uint32 Offset
		, uint32 Limit)
	{
		FReport::Log(FString(__FUNCTION__));

		if (UserId.IsEmpty())
		{
			OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("User id is empty"));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/recentplayer/%s?limit=%d&offset=%d"), *SettingsRef.SessionBrowserServerUrl, *CredentialsRef.GetClientNamespace(), *UserId, Limit, Offset);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
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

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/join"), *SettingsRef.SessionBrowserServerUrl, *CredentialsRef.GetClientNamespace(), *SessionId);
		FString Verb = TEXT("POST");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FAccelByteModelsSessionBrowserJoinSessionRequest JoinRequest;
		if(!Password.IsEmpty())
		{
			JoinRequest.Password = FMD5::HashAnsiString(*Password);			
		}		
		FString Content;
		FJsonObjectConverter::UStructToJsonObjectString(JoinRequest, Content);
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}
}
}