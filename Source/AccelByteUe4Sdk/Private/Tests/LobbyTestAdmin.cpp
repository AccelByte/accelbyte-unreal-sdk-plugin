// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "LobbyTestAdmin.h"

#include "TestUtilities.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteRegistry.h"
#include "Models/AccelByteMatchmakingModels.h"

void AdminGetLobbyConfig(const THandler<FLobbyModelConfig>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/config/namespaces/%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminSetLobbyConfig(const FLobbyModelConfig& Config, const THandler<FLobbyModelConfig>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/config/namespaces/%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Config, Content);
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminDeleteLobbyDSMServer(const FString& podName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/dsmcontroller/admin/namespaces/%s/servers/local/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *podName);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetLobbyDSMConfig(const THandler<FDsmConfig>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/dsmcontroller/admin/namespaces/%s/configs"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminSetLobbyDSMConfig(const FDsmConfig& Body, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/dsmcontroller/admin/configs"), *GetAdminBaseUrl());
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(Body, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminEnqueueSession(const FString& SessionId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/matchmaking/namespaces/%s/sessions/%s/status"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *SessionId);
	AB_HTTP_GET(Request, Url, Authorization);

	const THandler<FAccelByteModelsMatchmakingResult> GetSessionStatusSuccessHandler =
		THandler<FAccelByteModelsMatchmakingResult>::CreateLambda([Authorization, OnSuccess, OnError](const FAccelByteModelsMatchmakingResult& result)
		{
			FString UrlEnqueue = FString::Printf(TEXT("%s/matchmaking/namespaces/%s/sessions"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
			FString Content;
			FJsonObjectConverter::UStructToJsonObjectString(result, Content);
			AB_HTTP_POST(Request, UrlEnqueue, Authorization, Content);

			FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		});
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(GetSessionStatusSuccessHandler, OnError), FPlatformTime::Seconds());
}
