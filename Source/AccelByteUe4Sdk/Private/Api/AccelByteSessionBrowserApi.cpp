// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteSessionBrowserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{

SessionBrowser::SessionBrowser(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

SessionBrowser::~SessionBrowser()
{}

void SessionBrowser::CreateGameSession(FString const& GameMode
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

void SessionBrowser::CreateGameSession(FString const& GameMode
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

void SessionBrowser::CreateGameSession(FString const& SessionTypeString
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

void SessionBrowser::CreateGameSession(EAccelByteSessionType SessionType
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
	NewGameSession.Username = CredentialsRef.GetUserDisplayName();
	NewGameSession.Namespace = CredentialsRef.GetNamespace();
	NewGameSession.Session_type = SessionTypeString;
	NewGameSession.Game_session_setting.Mode = GameMode;
	NewGameSession.Game_session_setting.Map_name = GameMapName;
	NewGameSession.Game_session_setting.Num_bot = BotCount;
	NewGameSession.Game_session_setting.Max_player = MaxPlayer;
	NewGameSession.Game_session_setting.Current_player = 0;
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

void SessionBrowser::CreateGameSession(FAccelByteModelsSessionBrowserCreateRequest const& CreateSessionRequest
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const EAccelByteSessionType SessionType = FAccelByteUtilities::GetUEnumValueFromString<EAccelByteSessionType>(CreateSessionRequest.Session_type);
	
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

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession")
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, CreateSessionRequest, OnSuccess, OnError);
}

void SessionBrowser::UpdateGameSession(FString const& SessionId
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

void SessionBrowser::UpdateGameSession(FString const& SessionId
	, uint32 MaxPlayer
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FAccelByteModelsSessionBrowserUpdateRequest UpdateSessionRequest;
	UpdateSessionRequest.Game_max_player = MaxPlayer;

	UpdateGameSession(SessionId, UpdateSessionRequest, OnSuccess, OnError);
}

void SessionBrowser::UpdateGameSession(FString const& SessionId
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
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Max player must greather then 0"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s")
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace()
		, *SessionId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, UpdateSessionRequest, OnSuccess, OnError);
}

void SessionBrowser::UpdateGameSettings(FString const& SessionId
	, TMap<FString, FString> Settings
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	auto SettingJson = MakeShared<FJsonObject>();
	for (const auto& Set : Settings)
	{
		SettingJson->SetStringField(Set.Key, Set.Value);
	}

	UpdateGameSettings(SessionId, SettingJson, OnSuccess, OnError);
}

void SessionBrowser::UpdateGameSettings(FString const& SessionId
	, TSharedPtr<FJsonObject> Settings
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/settings")
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace()
		, *SessionId);

	FString Content;
	const TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Settings.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void SessionBrowser::RemoveGameSession(FString const& SessionId
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s")
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace()
		, *SessionId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void SessionBrowser::GetGameSessions(FString const& SessionTypeString
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

void SessionBrowser::GetGameSessions(EAccelByteSessionType SessionType
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

void SessionBrowser::GetGameSessions(FString const& SessionTypeString
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

void SessionBrowser::GetGameSessions(EAccelByteSessionType SessionType
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

	const FString SessionTypeString = FAccelByteUtilities::GetUEnumValueAsString(SessionType);

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession?session_type=%s&game_mode=%s&joinable=true&match_exist=%s&limit=%d&offset=%d")
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace()
		, *FGenericPlatformHttp::UrlEncode(SessionTypeString)
		, *FGenericPlatformHttp::UrlEncode(GameMode)
		, *FGenericPlatformHttp::UrlEncode(MatchExist)
		, Limit
		, Offset);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void SessionBrowser::GetGameSessionsByUserIds(const TArray<FString>& UserIds
	, THandler<FAccelByteModelsSessionBrowserGetByUserIdsResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	if (UserIds.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Empty userIds"));
		return;
	}

	TArray<FString> FilteredIds = UserIds;
	FilteredIds.RemoveAll([](const FString& Element)
	{
		return !FAccelByteUtilities::IsAccelByteIDValid(Element);
	});

	if (FilteredIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, User Id format is invalid"));
		return;
	}

	FString UserIdsQueryString = FString::Join(FilteredIds, TEXT(","));
	UserIdsQueryString = FGenericPlatformHttp::UrlEncode(UserIdsQueryString);

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/bulk?user_ids=%s")
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserIdsQueryString);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void SessionBrowser::RegisterPlayer(FString const& SessionId
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
    	, *SettingsRef.SessionBrowserServerUrl
    	, *CredentialsRef.GetNamespace()
    	, *SessionId);

	FAccelByteModelsSessionBrowserAddPlayerRequest AddPlayerRequest;
	AddPlayerRequest.User_id = PlayerToAdd;
	AddPlayerRequest.As_spectator = AsSpectator;

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, AddPlayerRequest, OnSuccess, OnError);
}

void SessionBrowser::UnregisterPlayer(FString const& SessionId
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
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace()
		, *SessionId
		, *PlayerToRemove);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void SessionBrowser::GetRecentPlayer(FString const& UserId
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

	if (!FAccelByteUtilities::IsAccelByteIDValid(*UserId))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, User Id format is invalid"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/recentplayer/%s?limit=%d&offset=%d")
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId
		, Limit
		, Offset);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void SessionBrowser::JoinSession(FString const& SessionId
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
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace()
		, *SessionId);

	FAccelByteModelsSessionBrowserJoinSessionRequest JoinRequest;
	if(!Password.IsEmpty())
	{
		JoinRequest.Password = FMD5::HashAnsiString(*Password);			
	}		

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, JoinRequest, OnSuccess, OnError);
}

void SessionBrowser::GetGameSession(FString const& SessionId
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s")
		, *SettingsRef.SessionBrowserServerUrl
		, *CredentialsRef.GetNamespace()
		, *SessionId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

}
}