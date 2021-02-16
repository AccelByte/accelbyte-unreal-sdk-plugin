// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerMatchmaking::ServerMatchmaking(const AccelByte::ServerCredentials& Credentials, const AccelByte::ServerSettings& Setting) : Credentials(Credentials), Settings(Setting)
{
	StatusPollingDelegate = FTickerDelegate::CreateRaw(this, &ServerMatchmaking::StatusPollingTick);
	OnStatusPollingResponse.BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
	{
		Report report;
		report.GetHttpResponse(Request, Response);

		if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
		{
			HandleHttpResultOk(Response, OnStatusPollingResponseSuccess);
			return;
		}
		int32 Code;
		FString Message;
		HandleHttpError(Request, Response, Code, Message);
		OnStatusPollingResponseError.ExecuteIfBound(Code, Message);
	});
}

void ServerMatchmaking::QuerySessionStatus(const FString MatchId, const  THandler<FAccelByteModelsMatchmakingResult>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions/%s/status"), *Settings.MatchmakingServerUrl,*Credentials.GetClientNamespace(), *MatchId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Contents;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Contents);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerMatchmaking::EnqueueJoinableSession(const FAccelByteModelsMatchmakingResult& MatchmakingResult, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions"), *Settings.MatchmakingServerUrl,*Credentials.GetClientNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Contents;
	FJsonObjectConverter::UStructToJsonObjectString(MatchmakingResult, Contents);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Contents);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerMatchmaking::DequeueJoinableSession(const FString& MatchId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions/dequeue"), *Settings.MatchmakingServerUrl, *Credentials.GetClientNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Contents = FString::Printf(TEXT("{\"match_id\":\"%s\"}"), *MatchId);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Contents);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerMatchmaking::AddUserToSession(const FString& ChannelName, const FString& MatchId, const FString& UserId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError, const FString& PartyId)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FAccelByteModelsAddUserIntoSessionRequest Body = FAccelByteModelsAddUserIntoSessionRequest
	{
		UserId,
		PartyId
	};

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/channels/%s/sessions/%s"), *Settings.MatchmakingServerUrl, *Credentials.GetClientNamespace(), *ChannelName, *MatchId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Contents;
	FJsonObjectConverter::UStructToJsonObjectString(Body, Contents);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Contents);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerMatchmaking::RemoveUserFromSession(const FString& ChannelName, const FString& MatchId, const FString& UserId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError, const FAccelByteModelsMatchmakingResult& Body)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/channels/%s/sessions/%s/users/%s"), *Settings.MatchmakingServerUrl, *Credentials.GetClientNamespace(), *ChannelName, *MatchId, *UserId);
	FString Verb = TEXT("DELETE");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Contents;
	FJsonObjectConverter::UStructToJsonObjectString(Body, Contents);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Contents);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerMatchmaking::ActivateSessionStatusPolling(const FString& MatchId, const THandler<FAccelByteModelsMatchmakingResult>& OnSuccess, const FErrorHandler& OnError, uint32 IntervalSec)
{
	if (!bStatusPollingActive)
	{
		StatusPollingMatchId = MatchId;
		OnStatusPollingResponseSuccess = OnSuccess;
		OnStatusPollingResponseError = OnError;
		StatusPollingDelegateHandle = FTicker::GetCoreTicker().AddTicker(StatusPollingDelegate, IntervalSec);
		bStatusPollingActive = true;
	}
}

void ServerMatchmaking::DeactivateStatusPolling()
{
	if (UObjectInitialized() && (bStatusPollingActive || StatusPollingDelegateHandle.IsValid()))
	{
		FTicker::GetCoreTicker().RemoveTicker(StatusPollingDelegateHandle);
		bStatusPollingActive = false;
	}
}

bool ServerMatchmaking::StatusPollingTick(float DeltaTime)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	QuerySessionStatus(StatusPollingMatchId, OnStatusPollingResponseSuccess, OnStatusPollingResponseError);
	return true;
}

ServerMatchmaking::~ServerMatchmaking()
{
	if (StatusPollingDelegateHandle.IsValid())
	{
		ServerMatchmaking::DeactivateStatusPolling();
		StatusPollingDelegateHandle.Reset();
	}
	OnStatusPollingResponse.Unbind();
	OnStatusPollingResponseSuccess.Unbind();
	OnStatusPollingResponseError.Unbind();
}

} // Namespace Api
} // Namespace AccelByte
