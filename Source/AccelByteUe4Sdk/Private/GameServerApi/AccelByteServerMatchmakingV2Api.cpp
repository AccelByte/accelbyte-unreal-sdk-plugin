// Copyright (c) 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerMatchmakingV2Api.h"
#include "Core/AccelByteReport.h"

namespace AccelByte
{
namespace GameServerApi
{
	ServerMatchmakingV2::ServerMatchmakingV2(
		ServerCredentials const& InCredentialsRef,
		ServerSettings const& InSettingRef,
		FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingRef, InHttpRef)
	{
	}

	ServerMatchmakingV2::~ServerMatchmakingV2()
	{
	}

	void ServerMatchmakingV2::AcceptBackfillProposal(
		const FString& BackfillTicketId,
		const FString& ProposalId,
		bool bStopBackfilling,
		const FVoidHandler& OnSuccess,
		const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		if (BackfillTicketId.IsEmpty())
		{
			OnError.ExecuteIfBound(400, TEXT("BackfillTicketId cannot be empty!"));
			return;
		}

		if (ProposalId.IsEmpty())
		{
			OnError.ExecuteIfBound(400, TEXT("ProposalId cannot be empty!"));
			return;
		}

		const FString Authorization = FString::Printf(TEXT("Bearer %s"), *ServerCredentialsRef.GetClientAccessToken());
		const FString Verb = TEXT("PUT");
		const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill/%s/proposal/accept"),
			*ServerSettingsRef.MatchmakingV2ServerUrl,
			*ServerCredentialsRef.GetClientNamespace(),
			*BackfillTicketId);

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

		const TSharedRef<FJsonObject> RequestObject = MakeShared<FJsonObject>();
		RequestObject->SetStringField(TEXT("proposalId"), ProposalId);
		RequestObject->SetBoolField(TEXT("stop"), bStopBackfilling);

		FString Content;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
		if (!FJsonSerializer::Serialize(RequestObject, Writer))
		{
			OnError.ExecuteIfBound(400, TEXT("Failed to convert request JSON object to string!"));
			return;
		}

		Request->SetContentAsString(Content);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerMatchmakingV2::RejectBackfillProposal(
		const FString& BackfillTicketId,
		const FString& ProposalId,
		bool bStopBackfilling,
		const FVoidHandler& OnSuccess,
		const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		if (BackfillTicketId.IsEmpty())
		{
			OnError.ExecuteIfBound(400, TEXT("BackfillTicketId cannot be empty!"));
			return;
		}

		const FString Authorization = FString::Printf(TEXT("Bearer %s"), *ServerCredentialsRef.GetClientAccessToken());
		const FString Verb = TEXT("PUT");
		const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/backfill/%s/proposal/reject"),
			*ServerSettingsRef.MatchmakingV2ServerUrl,
			*ServerCredentialsRef.GetClientNamespace(),
			*BackfillTicketId);

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
		Request->SetHeader(TEXT("Accept"), TEXT("application/json"));

		const TSharedRef<FJsonObject> RequestObject = MakeShared<FJsonObject>();
		RequestObject->SetBoolField(TEXT("stop"), bStopBackfilling);
		RequestObject->SetStringField(TEXT("proposalId"), ProposalId);

		FString Content;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
		if (!FJsonSerializer::Serialize(RequestObject, Writer))
		{
			OnError.ExecuteIfBound(400, TEXT("Failed to convert request JSON object to string!"));
			return;
		}

		Request->SetContentAsString(Content);

		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

}
}