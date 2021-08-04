// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteSessionBrowserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
namespace Api
{

	SessionBrowser::SessionBrowser(
		Credentials const& CredentialsRef,
		Settings const& SettingsRef,
		FHttpRetryScheduler& HttpRef)
		:
		HttpRef{HttpRef},
		CredentialsRef{CredentialsRef},
		SettingsRef{SettingsRef} {}

	SessionBrowser::~SessionBrowser()
	{}

	FString SessionBrowser::GetSessionBrowserUrl() {
		return SettingsRef.SessionBrowserServerUrl.IsEmpty() ? FString::Printf(TEXT("%s/sessionbrowser"), *SettingsRef.BaseUrl) : SettingsRef.SessionBrowserServerUrl;
	}

	void SessionBrowser::CreateGameSession(const FString& GameMode, const FString& GameMapName, const FString& GameVersion, uint32 BotCount, uint32 MaxPlayer, TSharedPtr<FJsonObject> OtherSettings, const THandler<FAccelByteModelsSessionBrowserData>& OnSuccess, const FErrorHandler& OnError) {
		FReport::Log(FString(__FUNCTION__));

		if (MaxPlayer == 0)
		{
			OnError.ExecuteIfBound(404, TEXT("Max player must greather then 0"));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString SessionBrowserServerUrl = GetSessionBrowserUrl();
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession"), *SessionBrowserServerUrl, *CredentialsRef.GetNamespace());
		FString Verb = TEXT("POST");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FAccelByteModelsSessionBrowserCreateRequest NewGameSession;
		NewGameSession.Username = CredentialsRef.GetUserDisplayName();
		NewGameSession.Namespace = CredentialsRef.GetNamespace();
		NewGameSession.Game_session_setting.Mode = GameMode;
		NewGameSession.Game_session_setting.Map_name = GameMapName;
		NewGameSession.Game_session_setting.Num_bot = BotCount;
		NewGameSession.Game_session_setting.Max_player = MaxPlayer;
		NewGameSession.Game_session_setting.Current_player = 1;
		NewGameSession.Game_session_setting.Settings.JsonObject = OtherSettings;
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
		FReport::Log(FString(__FUNCTION__));

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

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString SessionBrowserServerUrl = GetSessionBrowserUrl();
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s"), *SessionBrowserServerUrl, *CredentialsRef.GetNamespace(), *SessionId);
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");

		FAccelByteModelsSessionBrowserUpdateRequest NewGameSession;
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
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString SessionBrowserServerUrl = GetSessionBrowserUrl();
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s"), *SessionBrowserServerUrl, *CredentialsRef.GetNamespace(), *SessionId);
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
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString SessionBrowserServerUrl = GetSessionBrowserUrl();
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession?session_type=%s&game_mode=%s&limit=%d&offset=%d"), *SessionBrowserServerUrl, *CredentialsRef.GetNamespace(), *SessionType, *GameMode, Limit, Offset);
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

	void SessionBrowser::GetGameSession(const FString& SessionId, const THandler<FAccelByteModelsSessionBrowserData>& OnSuccess, const FErrorHandler& OnError){
		FReport::Log(FString(__FUNCTION__));
		
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString SessionBrowserServerUrl = GetSessionBrowserUrl();
		FString Url = FString::Printf(TEXT("%s/namespaces/%s/gamesession/%s"), *SessionBrowserServerUrl, *CredentialsRef.GetNamespace(), *SessionId);
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