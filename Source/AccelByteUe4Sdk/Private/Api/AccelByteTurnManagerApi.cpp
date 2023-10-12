// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteTurnManagerApi.h"

#include "Icmp.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteReport.h"

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

void TurnManager::GetTurnServers(const THandler<FAccelByteModelsTurnServerList>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/turn"), *GetTurnManagerServerUrl());

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void TurnManager::GetClosestTurnServer(const THandler<FAccelByteModelsTurnServer>& OnSuccess
	, const FErrorHandler& OnError)
{
	GetTurnServers(THandler<FAccelByteModelsTurnServerList>::CreateLambda(
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
		                FUDPPing::UDPEcho(FString::Printf(TEXT("%s:%d"), *Server.Ip, Server.Qos_port)
							, 10.0f
							, FIcmpEchoResultDelegate::CreateLambda(
								[this, Server, Count, OnSuccess](FIcmpEchoResult PingResult)
								{
									Counter++;
									if(FastestPing > PingResult.Time)
									{
                    					FastestPing = PingResult.Time;
                    					ClosestServer = Server;
									}
									if(Counter == Count)
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

void TurnManager::GetTurnCredential(const FString &Region
	, const FString &Ip
	, int Port
	, const THandler<FAccelByteModelsTurnServerCredential>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/turn/secret/%s/%s/%d")
		, *GetTurnManagerServerUrl()
		, *Region
		, *Ip
		, Port);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FString TurnManager::GetTurnManagerServerUrl() const
{
	return SettingsRef.TurnManagerServerUrl.IsEmpty()
		? FString::Printf(TEXT("%s/turnmanager"), *SettingsRef.BaseUrl)
		: SettingsRef.TurnManagerServerUrl;
}

void TurnManager::SendMetric(const FString &SelectedTurnServerRegion, const EP2PConnectionType &P2PConnectionType,
	const FVoidHandler &OnSuccess,
	const FErrorHandler &OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/metrics/namespaces/%s/connected"),
		*GetTurnManagerServerUrl(),
		*SettingsRef.Namespace);

	FAccelByteModelsTurnManagerMetric Data;
	Data.Region = SelectedTurnServerRegion;
	Data.Type = FAccelByteUtilities::GetUEnumValueAsString(P2PConnectionType).ToLower();

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Data,  OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
