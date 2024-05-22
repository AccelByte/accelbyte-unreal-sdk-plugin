// Copyright (c) 2024 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "Api/AccelByteLoginQueueApi.h"
#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpListenerExtension.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteOauth2Api.h"
#include "Core/AccelByteUtilities.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteLoginQueue, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteLoginQueue);

using AccelByte::Api::Oauth2;

namespace AccelByte
{
namespace Api
{

LoginQueue::LoginQueue(Credentials& InCredentialsRef
	, Settings& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	, UserCredentialsRef{InCredentialsRef.AsShared()}
{
}

LoginQueue::~LoginQueue()
{}

FAccelByteTaskWPtr LoginQueue::RefreshTicket(FString const& Ticket
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& Namespace)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/ticket")
		, *SettingsRef.LoginQueueServerUrl
		, Namespace.IsEmpty() ? *SettingsRef.Namespace : *Namespace);

	TMap<FString, FString> Headers{};
	Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Ticket));

	return HttpClient.Request(TEXT("GET"), Url, {}, FString(), Headers
		, THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
			[Ticket, OnSuccess](FAccelByteModelsLoginQueueTicketInfo const& Response) 
			{
				FAccelByteModelsLoginQueueTicketInfo NewTicketInfo = Response;
				NewTicketInfo.Ticket = Ticket;
				OnSuccess.ExecuteIfBound(NewTicketInfo);
			})
		, OnError);
}

FAccelByteTaskWPtr LoginQueue::CancelTicket(FString const& Ticket
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& Namespace)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/namespaces/%s/ticket")
		, *SettingsRef.LoginQueueServerUrl
		, Namespace.IsEmpty() ? *SettingsRef.Namespace : *Namespace);

	TMap<FString, FString> Headers{};
	Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *Ticket));
	Headers.Add(TEXT("Accept"), TEXT("application/json"));
	Headers.Add(TEXT("Content-Type"), TEXT(""));

	return HttpClient.Request(TEXT("DELETE"), Url, {}, FString(), Headers, OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte
