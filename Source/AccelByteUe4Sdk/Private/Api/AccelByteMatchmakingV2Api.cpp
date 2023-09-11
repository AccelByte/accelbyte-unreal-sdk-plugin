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
	const auto OnCreateTicketError = FCreateMatchmakingTicketErrorHandler::CreateLambda(
		[&OnError](const int32 ErrorCode, const FString& ErrorMessage, const FErrorCreateMatchmakingTicketV2& ErrorDetails)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	});

	CreateMatchTicket(MatchPool, OnSuccess, OnCreateTicketError, Optionals);
}

void MatchmakingV2::CreateMatchTicket(const FString& MatchPool,
	const THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse>& OnSuccess,
	const FCreateMatchmakingTicketErrorHandler& OnError,
	const FAccelByteModelsV2MatchTicketOptionalParams& Optionals)
{
	if (MatchPool.IsEmpty())
	{
		FErrorCreateMatchmakingTicketV2 CreateTicketError;
		CreateTicketError.ErrorCode = static_cast<int32>(ErrorCodes::InvalidRequest);
		CreateTicketError.ErrorMessage = TEXT("MatchPool cannot be empty!");
		OnError.ExecuteIfBound(CreateTicketError.ErrorCode, CreateTicketError.ErrorMessage, CreateTicketError);
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
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("TicketId cannot be empty!"));
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
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("TicketId cannot be empty!"));
		return;
	}

	const FString Verb = TEXT("DELETE");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-tickets/%s"),
		*SettingsRef.MatchmakingV2ServerUrl,
		*CredentialsRef.GetNamespace(),
		*TicketId);

	HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

void MatchmakingV2::GetMatchmakingMetrics(const FString& MatchPool,
	const THandler<FAccelByteModelsV2MatchmakingMetrics>& OnSuccess, const FErrorHandler& OnError)
{
	if (MatchPool.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("MatchPool cannot be empty!"));
		return;
	}

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-pools/%s/metrics")
		, *SettingsRef.MatchmakingV2ServerUrl
		, *CredentialsRef.GetNamespace()
		, *MatchPool);

	HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

void MatchmakingV2::GetMyMatchTickets(const THandler<FAccelByteModelsV2MatchmakingTicketStatuses>& OnSuccess,
	const FErrorHandler& OnError, const FString& MatchPool, const int32& Limit, const int32& Offset)
{
	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-tickets/me")
		, *SettingsRef.MatchmakingV2ServerUrl
		, *CredentialsRef.GetNamespace());

	TMultiMap<FString, FString> QueryParams;
	if(!MatchPool.IsEmpty())
	{
		QueryParams.Emplace(TEXT("matchPool"), MatchPool);
	}
	QueryParams.Emplace(TEXT("offset"), FString::FromInt(Offset));
	QueryParams.Emplace(TEXT("limit"), FString::FromInt(Limit));
	
	HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}
}
}