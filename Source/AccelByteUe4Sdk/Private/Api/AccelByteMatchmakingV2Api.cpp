// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteMatchmakingV2Api.h"

namespace AccelByte
{
namespace Api
{
MatchmakingV2::MatchmakingV2(Credentials const& InCredentialsRef
	, Settings const& InSettingRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingRef, InHttpRef)
{
}

MatchmakingV2::~MatchmakingV2()
{
}

void MatchmakingV2::CreateMatchTicket(const FString& MatchPool
	, const THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse>& OnSuccess
	, const FErrorHandler& OnError
	, const FAccelByteModelsV2MatchTicketOptionalParams& Optionals)
{
	if (MatchPool.IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("MatchPool cannot be empty!"));
		return;
	}

	const FString Verb = TEXT("POST");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-tickets")
		, *SettingsRef.MatchmakingV2ServerUrl
		, *CredentialsRef.GetNamespace());

	FAccelByteModelsV2MatchmakingCreateTicketRequest Request;
	Request.MatchPool = MatchPool;
	Request.SessionId = Optionals.SessionId;
	Request.Attributes = Optionals.Attributes;

	for(const TPair<FString, float>& Latency : Optionals.Latencies)
	{
		Request.Latencies.Emplace(Latency.Key, FGenericPlatformMath::FloorToInt(Latency.Value));
	}

	HttpClient.ApiRequest(Verb, Url, {},  Request, OnSuccess, OnError);
}

void MatchmakingV2::GetMatchTicketDetails(const FString& TicketId
	, const THandler<FAccelByteModelsV2MatchmakingGetTicketDetailsResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	if (TicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("TicketId cannot be empty!"));
		return;
	}

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-tickets/%s")
		, *SettingsRef.MatchmakingV2ServerUrl
		, *CredentialsRef.GetNamespace()
		, *TicketId);

	HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

void MatchmakingV2::DeleteMatchTicket(const FString& TicketId
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	if (TicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("TicketId cannot be empty!"));
		return;
	}

	const FString Verb = TEXT("DELETE");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-tickets/%s"),
		*SettingsRef.MatchmakingV2ServerUrl,
		*CredentialsRef.GetNamespace(),
		*TicketId);

	HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}
	
}
}