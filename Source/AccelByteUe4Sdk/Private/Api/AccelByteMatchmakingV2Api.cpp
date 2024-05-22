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

FAccelByteTaskWPtr MatchmakingV2::CreateMatchTicket(FString const& MatchPool
	, THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FAccelByteModelsV2MatchTicketOptionalParams const& Optionals)
{
	const auto OnCreateTicketError = FCreateMatchmakingTicketErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorCreateMatchmakingTicketV2 const& ErrorDetails)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});

	return CreateMatchTicket(MatchPool, OnSuccess, OnCreateTicketError, Optionals);
}

FAccelByteTaskWPtr MatchmakingV2::CreateMatchTicket(FString const& MatchPool
	, THandler<FAccelByteModelsV2MatchmakingCreateTicketResponse> const& OnSuccess
	, FCreateMatchmakingTicketErrorHandler const& OnError
	, FAccelByteModelsV2MatchTicketOptionalParams const& Optionals)
{
	if (MatchPool.IsEmpty())
	{
		FErrorCreateMatchmakingTicketV2 CreateTicketError;
		CreateTicketError.ErrorCode = static_cast<int32>(ErrorCodes::InvalidRequest);
		CreateTicketError.ErrorMessage = TEXT("MatchPool cannot be empty!");
		OnError.ExecuteIfBound(CreateTicketError.ErrorCode, CreateTicketError.ErrorMessage, CreateTicketError);
		return nullptr;
	}

	const FString Verb = TEXT("POST");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-tickets")
		, *SettingsRef.MatchmakingV2ServerUrl
		, *CredentialsRef->GetNamespace());

	FAccelByteModelsV2MatchmakingCreateTicketRequest Request;
	Request.MatchPool = MatchPool;
	Request.SessionId = Optionals.SessionId;
	Request.Attributes = Optionals.Attributes;

	for(const TPair<FString, float>& Latency : Optionals.Latencies)
	{
		Request.Latencies.Emplace(Latency.Key, FGenericPlatformMath::FloorToInt(Latency.Value));
	}

	return HttpClient.ApiRequest(Verb, Url, {},  Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr MatchmakingV2::GetMatchTicketDetails(FString const& TicketId
	, THandler<FAccelByteModelsV2MatchmakingGetTicketDetailsResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (TicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("TicketId cannot be empty!"));
		return nullptr;
	}

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-tickets/%s")
		, *SettingsRef.MatchmakingV2ServerUrl
		, *CredentialsRef->GetNamespace()
		, *TicketId);

	return HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr MatchmakingV2::DeleteMatchTicket(FString const& TicketId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (TicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("TicketId cannot be empty!"));
		return nullptr;
	}

	const FString Verb = TEXT("DELETE");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-tickets/%s"),
		*SettingsRef.MatchmakingV2ServerUrl,
		*CredentialsRef->GetNamespace(),
		*TicketId);

	return HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr MatchmakingV2::GetMatchmakingMetrics(FString const& MatchPool
	, THandler<FAccelByteModelsV2MatchmakingMetrics> const& OnSuccess
	, FErrorHandler const& OnError)
{
	if (MatchPool.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("MatchPool cannot be empty!"));
		return nullptr;
	}

	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-pools/%s/metrics")
		, *SettingsRef.MatchmakingV2ServerUrl
		, *CredentialsRef->GetNamespace()
		, *MatchPool);

	return HttpClient.ApiRequest(Verb, Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr MatchmakingV2::GetMyMatchTickets(THandler<FAccelByteModelsV2MatchmakingTicketStatuses> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& MatchPool
	, int32 Limit
	, int32 Offset)
{
	const FString Verb = TEXT("GET");
	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/match-tickets/me")
		, *SettingsRef.MatchmakingV2ServerUrl
		, *CredentialsRef->GetNamespace());

	TMultiMap<FString, FString> QueryParams;
	if(!MatchPool.IsEmpty())
	{
		QueryParams.Emplace(TEXT("matchPool"), MatchPool);
	}
	QueryParams.Emplace(TEXT("offset"), FString::FromInt(Offset));
	QueryParams.Emplace(TEXT("limit"), FString::FromInt(Limit));
	
	return HttpClient.ApiRequest(Verb, Url, QueryParams, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte