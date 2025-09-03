// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#if 1 // MMv1 Deprecation
#include "GameServerApi/AccelByteServerMatchmakingApi.h"
#include "Core/AccelByteReport.h"

#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerMatchmaking::ServerMatchmaking(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InServerApiClient)
	, bStatusPollingActive{false}
{
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Matchmaking V1 is deprecated and replaced by Matchmaking V2. For more information, see https://docs.accelbyte.io/gaming-services/services/play/matchmaking/matchmaking-version-comparison/"));
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

FAccelByteTaskWPtr ServerMatchmaking::QuerySessionStatus(FString const& MatchId
	, THandler<FAccelByteModelsMatchmakingResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions/%s/status")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *MatchId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerMatchmaking::EnqueueJoinableSession(FAccelByteModelsMatchmakingResult const& MatchmakingResult
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Matchmaking V1 is deprecated and replaced by Matchmaking V2. For more information, see https://docs.accelbyte.io/gaming-services/services/play/matchmaking/matchmaking-version-comparison/"));
	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, MatchmakingResult, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerMatchmaking::DequeueJoinableSession(FString const& MatchId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Matchmaking V1 is deprecated and replaced by Matchmaking V2. For more information, see https://docs.accelbyte.io/gaming-services/services/play/matchmaking/matchmaking-version-comparison/"));
	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/sessions/dequeue")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const FString Contents = FString::Printf(TEXT("{\"match_id\":\"%s\"}"), *MatchId);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerMatchmaking::AddUserToSession(FString const& ChannelName
	, FString const& MatchId
	, FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& PartyId)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Matchmaking V1 is deprecated and replaced by Matchmaking V2. For more information, see https://docs.accelbyte.io/gaming-services/services/play/matchmaking/matchmaking-version-comparison/"));
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

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Body, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerMatchmaking::RemoveUserFromSession(FString const& ChannelName
	, FString const& MatchId
	, FString const& UserId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FAccelByteModelsMatchmakingResult const& Body)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Matchmaking V1 is deprecated and replaced by Matchmaking V2. For more information, see https://docs.accelbyte.io/gaming-services/services/play/matchmaking/matchmaking-version-comparison/"));
	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/channels/%s/sessions/%s/users/%s")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *ChannelName
		, *MatchId
		, *UserId);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, Body, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerMatchmaking::RebalanceMatchmakingBasedOnMMR(FString const& MatchId
	, THandler<FAccelByteModelsMatchmakingResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Matchmaking V1 is deprecated and replaced by Matchmaking V2. For more information, see https://docs.accelbyte.io/gaming-services/services/play/matchmaking/matchmaking-version-comparison/"));
	const FString Url = FString::Printf(TEXT("%s/namespaces/%s/rebalance")
		, *ServerSettingsRef.MatchmakingServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	FString Contents = FString::Printf(TEXT("{\"match_id\":\"%s\"}"), *MatchId);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, OnSuccess, OnError);
}

void ServerMatchmaking::ActivateSessionStatusPolling(FString const& MatchId
	, THandler<FAccelByteModelsMatchmakingResult> const& OnSuccess
	, FErrorHandler const& OnError
	, uint32 IntervalSec)
{
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Matchmaking V1 is deprecated and replaced by Matchmaking V2. For more information, see https://docs.accelbyte.io/gaming-services/services/play/matchmaking/matchmaking-version-comparison/"));
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
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Matchmaking V1 is deprecated and replaced by Matchmaking V2. For more information, see https://docs.accelbyte.io/gaming-services/services/play/matchmaking/matchmaking-version-comparison/"));
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
#endif