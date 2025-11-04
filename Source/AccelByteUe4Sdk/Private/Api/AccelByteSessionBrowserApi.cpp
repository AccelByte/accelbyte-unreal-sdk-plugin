// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#if 1 // MMv1 Deprecation
#include "Api/AccelByteSessionBrowserApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteInstance.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{

SessionBrowser::SessionBrowser(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));
}

SessionBrowser::SessionBrowser(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, FAccelBytePlatformPtr const& InAccelBytePlatform)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InAccelBytePlatform)
{
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));
}

SessionBrowser::~SessionBrowser()
{}

FAccelByteTaskWPtr SessionBrowser::CreateGameSession(FString const& GameMode
	, FString const& GameMapName
	, FString const& GameVersion
	, uint32 BotCount
	, uint32 MaxPlayer
	, TSharedPtr<FJsonObject> OtherSettings
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	return CreateGameSession(GameMode
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

FAccelByteTaskWPtr SessionBrowser::CreateGameSession(FString const& GameMode
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
	return CreateGameSession(EAccelByteSessionType::p2p
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

FAccelByteTaskWPtr SessionBrowser::CreateGameSession(FString const& SessionTypeString
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
	return CreateGameSession(SessionType
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

FAccelByteTaskWPtr SessionBrowser::CreateGameSession(EAccelByteSessionType SessionType
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
	NewGameSession.Username = CredentialsRef->GetUserDisplayName();
	NewGameSession.Namespace = CredentialsRef->GetNamespace();
	NewGameSession.Session_type = SessionTypeString;
	NewGameSession.Game_session_setting.Mode = GameMode;
	NewGameSession.Game_session_setting.Map_name = GameMapName;
	NewGameSession.Game_session_setting.Num_bot = BotCount;
	NewGameSession.Game_session_setting.Max_player = MaxPlayer;
	NewGameSession.Game_session_setting.Current_player = 0;
	NewGameSession.Game_session_setting.Max_internal_player = MaxSpectator;
	NewGameSession.Game_session_setting.Current_internal_player = 0;
	
	if (!Password.IsEmpty())
	{
		NewGameSession.Game_session_setting.Password = FMD5::HashAnsiString(*Password);
	}
	
	NewGameSession.Game_session_setting.Settings.JsonObject = OtherSettings;
	NewGameSession.Game_version = GameVersion;

	return CreateGameSession(NewGameSession, OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::CreateGameSession(FAccelByteModelsSessionBrowserCreateRequest const& CreateSessionRequest
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));

	const EAccelByteSessionType SessionType = FAccelByteUtilities::GetUEnumValueFromString<EAccelByteSessionType>(CreateSessionRequest.Session_type);
	
	if (SessionType == EAccelByteSessionType::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Wrong session type"));
		return nullptr;
	}
	
	if (CreateSessionRequest.Game_session_setting.Max_player == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Max players must greather then 0"));
		return nullptr;
	}

	if (CreateSessionRequest.Game_session_setting.Mode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Game mode can't be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, CreateSessionRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::UpdateGameSession(FString const& SessionId
	, uint32 MaxPlayer
	, uint32 CurrentPlayerCount
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FAccelByteModelsSessionBrowserUpdateRequest UpdateSessionRequest;
	UpdateSessionRequest.Game_max_player = MaxPlayer;
	UpdateSessionRequest.Game_current_player = CurrentPlayerCount;

	return UpdateGameSession(SessionId, UpdateSessionRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::UpdateGameSession(FString const& SessionId
	, uint32 MaxPlayer
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FAccelByteModelsSessionBrowserUpdateRequest UpdateSessionRequest;
	UpdateSessionRequest.Game_max_player = MaxPlayer;

	return UpdateGameSession(SessionId, UpdateSessionRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::UpdateGameSession(FString const& SessionId
	, FAccelByteModelsSessionBrowserUpdateRequest const& UpdateSessionRequest
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));

	if (UpdateSessionRequest.Game_current_player != 0)
	{
		FReport::LogDeprecated(FString(__FUNCTION__),
			TEXT("Update player count deprecated from UpdateGameSession. Use register/unregister player to update player count"));
	}

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return nullptr;
	}

	if (UpdateSessionRequest.Game_max_player == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Max player must greather then 0"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(SessionId));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, UpdateSessionRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::UpdateGameSettings(FString const& SessionId
	, TMap<FString, FString> Settings
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	auto SettingJson = MakeShared<FJsonObject>();
	for (const auto& Set : Settings)
	{
		SettingJson->SetStringField(Set.Key, Set.Value);
	}

	return UpdateGameSettings(SessionId, SettingJson, OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::UpdateGameSettings(FString const& SessionId
	, TSharedPtr<FJsonObject> Settings
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/settings")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(SessionId));

	FString Content;
	const TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Settings.ToSharedRef(), Writer);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::RemoveGameSession(FString const& SessionId
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));
	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(SessionId));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::GetGameSessions(FString const& SessionTypeString
	, FString const& GameMode
	, THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	return GetGameSessions(SessionTypeString
		, GameMode
		, FString()
		, OnSuccess
		, OnError
		, Offset
		, Limit);
}

FAccelByteTaskWPtr SessionBrowser::GetGameSessions(EAccelByteSessionType SessionType
	, FString const& GameMode
	, THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	return GetGameSessions(SessionType
		, GameMode
		, FString()
		, OnSuccess
		, OnError
		, Offset
		, Limit);
}

FAccelByteTaskWPtr SessionBrowser::GetGameSessions(FString const& SessionTypeString
	, FString const& GameMode
	, FString const& MatchExist
	, THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	EAccelByteSessionType SessionType = FAccelByteUtilities::GetUEnumValueFromString<EAccelByteSessionType>(SessionTypeString);
	return GetGameSessions(SessionType
		, GameMode
		, MatchExist
		, OnSuccess
		, OnError
		, Offset
		, Limit);
}

FAccelByteTaskWPtr SessionBrowser::GetGameSessions(EAccelByteSessionType SessionType
	, FString const& GameMode
	, FString const& MatchExist
	, THandler<FAccelByteModelsSessionBrowserGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	FReport::Log(FString(__FUNCTION__));
	
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));
	
	if (SessionType == EAccelByteSessionType::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Wrong session type"));
		return nullptr;
	}

	const FString SessionTypeString = FAccelByteUtilities::GetUEnumValueAsString(SessionType);

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession?session_type=%s&game_mode=%s&joinable=true&match_exist=%s&limit=%d&offset=%d")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(SessionTypeString)
		, *FGenericPlatformHttp::UrlEncode(GameMode)
		, *FGenericPlatformHttp::UrlEncode(MatchExist)
		, Limit
		, Offset);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::GetGameSessionsByUserIds(const TArray<FString>& UserIds
	, THandler<FAccelByteModelsSessionBrowserGetByUserIdsResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));

	if (UserIds.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Empty userIds"));
		return nullptr;
	}

	FString UserIdsQueryString = FString::Join(UserIds, TEXT(","));
	UserIdsQueryString = FGenericPlatformHttp::UrlEncode(UserIdsQueryString);

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/bulk?user_ids=%s")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *UserIdsQueryString);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::RegisterPlayer(FString const& SessionId
	, FString const& PlayerToAdd
	, bool AsSpectator
	, THandler<FAccelByteModelsSessionBrowserAddPlayerResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));

	if (!ValidateAccelByteId(PlayerToAdd, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(PlayerToAdd)
		, OnError))
	{
		return nullptr;
	}

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return nullptr;
	}

    const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/player")
    	, *SettingsRef.SessionBrowserServerUrl
    	, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
    	, *FGenericPlatformHttp::UrlEncode(SessionId));

	FAccelByteModelsSessionBrowserAddPlayerRequest AddPlayerRequest;
	AddPlayerRequest.User_id = PlayerToAdd;
	AddPlayerRequest.As_spectator = AsSpectator;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, AddPlayerRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::UnregisterPlayer(FString const& SessionId
	, FString const& PlayerToRemove
	, THandler<FAccelByteModelsSessionBrowserAddPlayerResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));
	
	if (!ValidateAccelByteId(PlayerToRemove, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(PlayerToRemove)
		, OnError))
	{
		return nullptr;
	}

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/player/%s")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(SessionId)
		, *FGenericPlatformHttp::UrlEncode(PlayerToRemove));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::GetRecentPlayer(FString const& UserId
	, THandler<FAccelByteModelsSessionBrowserRecentPlayerGetResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 Offset
	, uint32 Limit)
{
	FReport::Log(FString(__FUNCTION__));

	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/recentplayer/%s?limit=%d&offset=%d")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, Limit
		, Offset);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::JoinSession(FString const& SessionId
	, FString const& Password
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s/join")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(SessionId));

	FAccelByteModelsSessionBrowserJoinSessionRequest JoinRequest;
	if (!Password.IsEmpty())
	{
		JoinRequest.Password = FMD5::HashAnsiString(*Password);			
	}		

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, JoinRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr SessionBrowser::GetGameSession(FString const& SessionId
	, THandler<FAccelByteModelsSessionBrowserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("The session browser is deprecated and replaced by game sessions. For more information, see https://docs.accelbyte.io/gaming-services/services/play/peer-to-peer-via-relay-server/"));
	
	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Session id is empty"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s")
		, *SettingsRef.SessionBrowserServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(SessionId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}
} // Namespace Api
} // Namespace AccelByte
#endif
