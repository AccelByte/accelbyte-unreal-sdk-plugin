// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerQosManagerApi.h"

#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/Ping/AccelBytePing.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerQosManager::ServerQosManager(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InServerApiClient)
{}

ServerQosManager::~ServerQosManager()
{}

void ServerQosManager::GetQosServers(const THandler<FAccelByteModelsQosServerList>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/qos")
		, *ServerSettingsRef.QosManagerServerUrl);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerQosManager::GetActiveQosServers(const THandler<FAccelByteModelsQosServerList>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespace/%s/qos")
		, *ServerSettingsRef.QosManagerServerUrl
		, *FGenericPlatformHttp::UrlEncode(ServerCredentialsRef->GetNamespace()));

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void ServerQosManager::GetServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnSuccess
	, const FErrorHandler& OnError)
{
	Latencies.Empty();
	GetQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda(
			[this, OnSuccess](const FAccelByteModelsQosServerList Result)
			{
				for (int i = 0; i < Result.Servers.Num(); i++)
				{
					auto Server = Result.Servers[i];
					int32 Count = Result.Servers.Num();
					FAccelBytePing::SendUdpPing(*Server.Ip, Server.Port
						, ServerSettingsRef.QosPingTimeout
						, FPingCompleteDelegate::CreateLambda(
							[this, Server, OnSuccess, Count](FPingResult Result)
							{
								Latencies.Add(TPair<FString, float>(Server.Region, Result.AverageRoundTrip * 1000)); // convert to milliseconds
								if (Count == Latencies.Num())
								{
									OnSuccess.ExecuteIfBound(Latencies);
								}
							})
					);
				}
			})
		, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
}

} // Namespace Api
} // Namespace AccelByte
