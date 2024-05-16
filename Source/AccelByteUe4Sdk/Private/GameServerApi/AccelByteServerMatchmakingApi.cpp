// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerMatchmaking::ServerMatchmaking(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	, bStatusPollingActive{false}
{
	StatusPollingDelegate = FTickerDelegate::CreateRaw(this, &ServerMatchmaking::StatusPollingTick);
	OnStatusPollingResponse.BindLambda(
		[this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
		{
			FReport::LogHttpResponse(Request, Response);

			if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
			{
				HandleHttpResultOk(Response, TArray<uint8>(), OnStatusPollingResponseSuccess);
				return;
			}
			int32 Code;
			FString Message;
			HandleHttpError(Request, Response, Code, Message);
			OnStatusPollingResponseError.ExecuteIfBound(Code, Message);
		});
}

void ServerMatchmaking::QuerySessionStatus(const FString MatchId
	, const  THandler<FAccelByteModelsMatchmakingResult>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions/%s/status")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *MatchId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerMatchmaking::EnqueueJoinableSession(const FAccelByteModelsMatchmakingResult& MatchmakingResult
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, MatchmakingResult, OnSuccess, OnError);
}

void ServerMatchmaking::DequeueJoinableSession(const FString& MatchId
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions/dequeue")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const FString Contents = FString::Printf(TEXT("{\"match_id\":\"%s\"}"), *MatchId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, OnSuccess, OnError);
}

void ServerMatchmaking::AddUserToSession(const FString& ChannelName
	, const FString& MatchId
	, const FString& UserId
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError
	, const FString& PartyId)
{
	FReport::Log(FString(__FUNCTION__));

	const FAccelByteModelsAddUserIntoSessionRequest Body = FAccelByteModelsAddUserIntoSessionRequest
	{
		UserId,
		PartyId
	};

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/channels/%s/sessions/%s")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *ChannelName
		, *MatchId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Body, OnSuccess, OnError);
}

void ServerMatchmaking::RemoveUserFromSession(const FString& ChannelName
	, const FString& MatchId
	, const FString& UserId
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError
	, const FAccelByteModelsMatchmakingResult& Body)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/channels/%s/sessions/%s/users/%s")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *ChannelName
		, *MatchId
		, *UserId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, Body, OnSuccess, OnError);
}

void ServerMatchmaking::RebalanceMatchmakingBasedOnMMR(const FString& MatchId
	, const THandler<FAccelByteModelsMatchmakingResult>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/rebalance")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	FString Contents = FString::Printf(TEXT("{\"match_id\":\"%s\"}"), *MatchId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, OnSuccess, OnError);
}

void ServerMatchmaking::ActivateSessionStatusPolling(const FString& MatchId
	, const THandler<FAccelByteModelsMatchmakingResult>& OnSuccess
	, const FErrorHandler& OnError
	, uint32 IntervalSec)
{
	if (!bStatusPollingActive)
	{
		StatusPollingMatchId = MatchId;
		OnStatusPollingResponseSuccess = OnSuccess;
		OnStatusPollingResponseError = OnError;
		StatusPollingDelegateHandle = FTickerAlias::GetCoreTicker().AddTicker(StatusPollingDelegate, IntervalSec);
		bStatusPollingActive = true;
	}
}

void ServerMatchmaking::DeactivateStatusPolling()
{
	if (UObjectInitialized() && (bStatusPollingActive || StatusPollingDelegateHandle.IsValid()))
	{
		FTickerAlias::GetCoreTicker().RemoveTicker(StatusPollingDelegateHandle);
		bStatusPollingActive = false;
	}
}

bool ServerMatchmaking::StatusPollingTick(float DeltaTime)
{
	FReport::Log(FString(__FUNCTION__));
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
