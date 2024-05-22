// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteTurnManagerApi.h"

#include "Icmp.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteReport.h"
#include "Core/Ping/AccelBytePing.h"

namespace AccelByte
{
namespace Api
{

TurnManager::TurnManager(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	, FastestPing{.0f}
	, Counter{0}
{}

TurnManager::~TurnManager()
{}

FAccelByteTaskWPtr TurnManager::GetTurnServers(THandler<FAccelByteModelsTurnServerList> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/turn"), *GetTurnManagerServerUrl());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr TurnManager::GetClosestTurnServer(const THandler<FAccelByteModelsTurnServer>& OnSuccess
	, FErrorHandler const& OnError)
{
	return GetTurnServers(THandler<FAccelByteModelsTurnServerList>::CreateLambda(
			[this, OnError, OnSuccess](const FAccelByteModelsTurnServerList& Result)
			{
				if(Result.Servers.Num() == 0)
				{
					OnError.ExecuteIfBound(400, TEXT("turn server is empty"));
				}
				else
				{
					FastestPing = 1000;
					Counter = 0;
					for (int i=0;i< Result.Servers.Num();i++)
					{
						auto Server = Result.Servers[i];
						int Count = Result.Servers.Num();
						FAccelBytePing::SendUdpPing(*Server.Ip, Server.Qos_port
							, 10.0f
							, FPingCompleteDelegate::CreateLambda(
								[this, Server, Count, OnSuccess](FPingResult PingResult)
								{
									Counter++;
									if (FastestPing > PingResult.AverageRoundTrip)
									{
										FastestPing = PingResult.AverageRoundTrip;
										ClosestServer = Server;
									}
									if (Counter == Count)
									{
										OnSuccess.ExecuteIfBound(ClosestServer);
									}
								})
						);
					}
				}
			})
		, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
}

FAccelByteTaskWPtr TurnManager::GetTurnCredential(FString const& Region
	, FString const& Ip
	, int Port
	, THandler<FAccelByteModelsTurnServerCredential> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/turn/secret/%s/%s/%d")
		, *GetTurnManagerServerUrl()
		, *Region
		, *Ip
		, Port);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FString TurnManager::GetTurnManagerServerUrl() const
{
	return SettingsRef.TurnManagerServerUrl.IsEmpty()
		? FString::Printf(TEXT("%s/turnmanager"), *SettingsRef.BaseUrl)
		: SettingsRef.TurnManagerServerUrl;
}

FAccelByteTaskWPtr TurnManager::SendMetric(FString const& SelectedTurnServerRegion
	, EP2PConnectionType P2PConnectionType
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/metrics/namespaces/%s/connected"),
		*GetTurnManagerServerUrl(),
		*SettingsRef.Namespace);

	FAccelByteModelsTurnManagerMetric Data;
	Data.Region = SelectedTurnServerRegion;
	Data.Type = FAccelByteUtilities::GetUEnumValueAsString(P2PConnectionType).ToLower();

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Data,  OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
