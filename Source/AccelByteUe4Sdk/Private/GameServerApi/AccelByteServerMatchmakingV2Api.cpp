// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerMatchmakingV2Api.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerMatchmakingV2::ServerMatchmakingV2(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingRef, InHttpRef)
{
}

ServerMatchmakingV2::~ServerMatchmakingV2()
{
}

void ServerMatchmakingV2::AcceptBackfillProposal(const FString& BackfillTicketId
	, const FString& ProposalId
	, bool bStopBackfilling
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
			FString(__FUNCTION__),
			TEXT("Use ServerMatchmakingV2::AcceptBackfillProposal with FAccelByteModelsV2GameSession info on success callback"));

	AcceptBackfillProposal(BackfillTicketId, ProposalId, bStopBackfilling,
		THandler<FAccelByteModelsV2GameSession>::CreateLambda([OnSuccess](const FAccelByteModelsV2GameSession& GameSessionInfo)
		{
			OnSuccess.ExecuteIfBound();
		}), OnError);
}

void ServerMatchmakingV2::AcceptBackfillProposal(const FString& BackfillTicketId
	, const FString& ProposalId
	, bool bStopBackfilling
	, const THandler<FAccelByteModelsV2GameSession>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (BackfillTicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("BackfillTicketId cannot be empty!"));
		return;
	}

	if (ProposalId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("ProposalId cannot be empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill/%s/proposal/accept")
		, *ServerSettingsRef.MatchmakingV2ServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *BackfillTicketId);

	const TSharedRef<FJsonObject> RequestObject = MakeShared<FJsonObject>();
	RequestObject->SetStringField(TEXT("proposalId"), ProposalId);
	RequestObject->SetBoolField(TEXT("stop"), bStopBackfilling);

	FString Content;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	if (!FJsonSerializer::Serialize(RequestObject, Writer))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to convert request JSON object to string!"));
		return;
	}

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void ServerMatchmakingV2::RejectBackfillProposal(const FString& BackfillTicketId
	, const FString& ProposalId
	, bool bStopBackfilling
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (BackfillTicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("BackfillTicketId cannot be empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill/%s/proposal/reject")
		, *ServerSettingsRef.MatchmakingV2ServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *BackfillTicketId);

	const TSharedRef<FJsonObject> RequestObject = MakeShared<FJsonObject>();
	RequestObject->SetBoolField(TEXT("stop"), bStopBackfilling);
	RequestObject->SetStringField(TEXT("proposalId"), ProposalId);

	FString Content;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	if (!FJsonSerializer::Serialize(RequestObject, Writer))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to convert request JSON object to string!"));
		return;
	}

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void ServerMatchmakingV2::CreateBackfillTicket(const FString& MatchPool
	, const FString& SessionId
	, const THandler<FAccelByteModelsV2MatchmakingCreateBackfillTicketResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (MatchPool.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("MatchPool cannot be empty!"));
		return;
	}

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("SessionId cannot be empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill")
		, *ServerSettingsRef.MatchmakingV2ServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	const TSharedRef<FJsonObject> RequestObject = MakeShared<FJsonObject>();
	RequestObject->SetStringField(TEXT("matchPool"), MatchPool);
	RequestObject->SetStringField(TEXT("sessionId"), SessionId);

	FString Content;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	if (!FJsonSerializer::Serialize(RequestObject, Writer))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to convert request JSON object to string!"));
		return;
	}

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void ServerMatchmakingV2::DeleteBackfillTicket(const FString& BackfillTicketId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (BackfillTicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("BackfillTicketId cannot be empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill/%s")
		, *ServerSettingsRef.MatchmakingV2ServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *BackfillTicketId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

}
}