// Copyright (c) 2019 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "LobbyTestAdmin.h"

#include "TestUtilities.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteRegistry.h"

FString GetAdminBaseUrl();
FString GetAdminUserAccessToken();

void LobbyGetConfig(const THandler<FLobbyModelConfig>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/config/namespaces/%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void LobbySetConfig(const FLobbyModelConfig& Config, const THandler<FLobbyModelConfig>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/v1/admin/config/namespaces/%s"), *GetAdminBaseUrl(), *FRegistry::ServerSettings.Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Config, Content);
	AB_HTTP_PUT(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void DSMDeleteServer(const FString& podName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/dsmcontroller/admin/namespaces/%s/servers/local/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *podName);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void DSMGetConfig(const THandler<FDsmConfig>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/dsmcontroller/admin/namespaces/%s/configs"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void DSMSetConfig(const FDsmConfig& Body, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/dsmcontroller/admin/configs"), *GetAdminBaseUrl());
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(Body, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
