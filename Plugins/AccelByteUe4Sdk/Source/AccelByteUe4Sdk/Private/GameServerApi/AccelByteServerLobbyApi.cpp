// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "GameServerApi/AccelByteServerLobby.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"
#include "Models/AccelByteLobbyModels.h"

namespace AccelByte
{
namespace GameServerApi
{
	ServerLobby::ServerLobby(const ServerCredentials& Credentials, const ServerSettings& Settings) : Credentials(Credentials), Settings(Settings)
	{}

	ServerLobby::~ServerLobby()
	{}

	void ServerLobby::GetPartyDataByUserId(const FString & UserId, const THandler<FAccelByteModelsDataPartyResponse> OnSuccess, const FErrorHandler & OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		if (UserId.IsEmpty())
		{
			OnError.ExecuteIfBound(404, TEXT("Url is invalid. User Id is empty."));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/admin/party/namespaces/%s/users/%s/party"), *Settings.LobbyServerUrl, *Credentials.GetClientNamespace(), *UserId);
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
} // namespace GameServerApi
} // namespace AccelByte