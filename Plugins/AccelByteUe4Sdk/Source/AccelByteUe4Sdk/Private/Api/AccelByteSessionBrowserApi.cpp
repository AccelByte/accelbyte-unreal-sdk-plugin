// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteSessionBrowserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
namespace Api
{

	SessionBrowser::SessionBrowser(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting)
	{}

	SessionBrowser::~SessionBrowser()
	{}

	FString SessionBrowser::GetSessionBrowserUrl() {
		return Settings.SessionBrowserServerUrl.IsEmpty() ? FString::Printf(TEXT("%s/sessionbrowser"), *Settings.BaseUrl) : Settings.SessionBrowserServerUrl;
	}

	void SessionBrowser::CreateGameSession(const FString& SessionType, const FString& GameMode, const FString& GameMapName, const FString& GameVersion, uint32 BotCount, uint32 MaxPlayer, const THandler<FAccelByteModelsSessionBrowserData>& OnSuccess, const FErrorHandler& OnError) {
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		if (MaxPlayer == 0)
		{
			OnError.ExecuteIfBound(404, TEXT("Max player must greather then 0"));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
		FString SessionBrowserServerUrl = GetSessionBrowserUrl();
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession"), *SessionBrowserServerUrl, *Credentials.GetUserNamespace());
		FString Verb = TEXT("POST");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FAccelByteModelsSessionBrowserData NewGameSession;
		NewGameSession.User_id = Credentials.GetUserId();
		NewGameSession.User_name = Credentials.GetUserDisplayName();
		NewGameSession.Namespace = Credentials.GetUserNamespace();
		NewGameSession.Game_mode = GameMode;
		NewGameSession.Game_map_name = GameMapName;
		NewGameSession.Session_type = SessionType;
		NewGameSession.Game_num_bot = BotCount;
		NewGameSession.Game_max_player = MaxPlayer;
		NewGameSession.Game_current_player = 1;
		NewGameSession.Game_version = GameVersion;
		FString Content;
		FJsonObjectConverter::UStructToJsonObjectString(NewGameSession, Content);
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);

		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void SessionBrowser::UpdateGameSession(const FString &SessionId, uint32 MaxPlayer, uint32 CurrentPlayerCount, const THandler<FAccelByteModelsSessionBrowserData>& OnSuccess, const FErrorHandler& OnError) {
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		if (MaxPlayer == 0)
		{
			OnError.ExecuteIfBound(404, TEXT("Max player must greather then 0"));
			return;
		}
		if (MaxPlayer < CurrentPlayerCount)
		{
			OnError.ExecuteIfBound(404, TEXT("Current player count must less than max player"));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
		FString SessionBrowserServerUrl = GetSessionBrowserUrl();
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s"), *SessionBrowserServerUrl, *Credentials.GetUserNamespace(), *SessionId);
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FAccelByteModelsSessionBrowserData NewGameSession;
		NewGameSession.Game_max_player = MaxPlayer;
		NewGameSession.Game_current_player = CurrentPlayerCount;
		FString Content;
		FJsonObjectConverter::UStructToJsonObjectString(NewGameSession, Content);
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);

		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void SessionBrowser::RemoveGameSession(const FString& SessionId, const THandler<FAccelByteModelsSessionBrowserData>& OnSuccess, const FErrorHandler& OnError) {
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
		FString SessionBrowserServerUrl = GetSessionBrowserUrl();
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s"), *SessionBrowserServerUrl, *Credentials.GetUserNamespace(), *SessionId);
		FString Verb = TEXT("DELETE");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void SessionBrowser::GetGameSessions(const FString& SessionType, const FString& GameMode, const THandler<FAccelByteModelsSessionBrowserGetResult>& OnSuccess, const FErrorHandler& OnError, uint32 Offset, uint32 Limit) {
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
		FString SessionBrowserServerUrl = GetSessionBrowserUrl();
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession?session_type=%s&game_mode=%s&limit=%d&offset=%d"), *SessionBrowserServerUrl, *Credentials.GetUserNamespace(), *SessionType, *GameMode, Limit, Offset);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);

		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}
}
}