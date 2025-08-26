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
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: FServerApiBase(InCredentialsRef, InSettingRef, InHttpRef, InServerApiClient)
{
}

ServerMatchmakingV2::~ServerMatchmakingV2()
{
}

FAccelByteTaskWPtr ServerMatchmakingV2::AcceptBackfillProposal(FString const& BackfillTicketId
	, FString const& ProposalId
	, bool bStopBackfilling
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
			FString(__FUNCTION__),
			TEXT("Use ServerMatchmakingV2::AcceptBackfillProposal with FAccelByteModelsV2GameSession info on success callback"));

	return AcceptBackfillProposal(BackfillTicketId, ProposalId, bStopBackfilling,
		THandler<FAccelByteModelsV2GameSession>::CreateLambda([OnSuccess](const FAccelByteModelsV2GameSession& GameSessionInfo)
		{
			OnSuccess.ExecuteIfBound();
		}), OnError);
}

FAccelByteTaskWPtr ServerMatchmakingV2::AcceptBackfillProposal(FString const& BackfillTicketId
	, FString const& ProposalId
	, bool bStopBackfilling
	, THandler<FAccelByteModelsV2GameSession> const& OnSuccess
	, FErrorHandler const& OnError
	, FAccelByteModelsV2MatchmakingBackfillAcceptanceOptionalParam const& OptionalParameter)
{
	FReport::Log(FString(__FUNCTION__));

	if (BackfillTicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("BackfillTicketId cannot be empty!"));
		return nullptr;
	}

	if (ProposalId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("ProposalId cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill/%s/proposal/accept")
		, *ServerSettingsRef.MatchmakingV2ServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *BackfillTicketId);

	const TSharedRef<FJsonObject> RequestObject = MakeShared<FJsonObject>();
	RequestObject->SetStringField(TEXT("proposalId"), ProposalId);
	RequestObject->SetBoolField(TEXT("stop"), bStopBackfilling);
	TArray<TSharedPtr<FJsonValue>> AcceptedTicketIdsArray{};
	if (OptionalParameter.AcceptedTicketIDs.Num() > 0)
	{
		for (const FString& Item : OptionalParameter.AcceptedTicketIDs)
		{
			AcceptedTicketIdsArray.Add(MakeShareable(new FJsonValueString(Item)));
		}
		RequestObject->SetArrayField(TEXT("acceptedTicketIds"), AcceptedTicketIdsArray);
	}

	FString Content;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	if (!FJsonSerializer::Serialize(RequestObject, Writer))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to convert request JSON object to string!"));
		return nullptr;
	}

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerMatchmakingV2::RejectBackfillProposal(FString const& BackfillTicketId
	, FString const& ProposalId
	, bool bStopBackfilling
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (BackfillTicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("BackfillTicketId cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill/%s/proposal/reject")
		, *ServerSettingsRef.MatchmakingV2ServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *BackfillTicketId);

	const TSharedRef<FJsonObject> RequestObject = MakeShared<FJsonObject>();
	RequestObject->SetBoolField(TEXT("stop"), bStopBackfilling);
	RequestObject->SetStringField(TEXT("proposalId"), ProposalId);

	FString Content;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	if (!FJsonSerializer::Serialize(RequestObject, Writer))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to convert request JSON object to string!"));
		return nullptr;
	}

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerMatchmakingV2::CreateBackfillTicket(FString const& MatchPool
	, FString const& SessionId
	, THandler<FAccelByteModelsV2MatchmakingCreateBackfillTicketResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (MatchPool.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("MatchPool cannot be empty!"));
		return nullptr;
	}

	if (SessionId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("SessionId cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill")
		, *ServerSettingsRef.MatchmakingV2ServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	const TSharedRef<FJsonObject> RequestObject = MakeShared<FJsonObject>();
	RequestObject->SetStringField(TEXT("matchPool"), MatchPool);
	RequestObject->SetStringField(TEXT("sessionId"), SessionId);

	FString Content;
	const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	if (!FJsonSerializer::Serialize(RequestObject, Writer))
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Failed to convert request JSON object to string!"));
		return nullptr;
	}

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerMatchmakingV2::DeleteBackfillTicket(FString const& BackfillTicketId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (BackfillTicketId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("BackfillTicketId cannot be empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill/%s")
		, *ServerSettingsRef.MatchmakingV2ServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *BackfillTicketId);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte