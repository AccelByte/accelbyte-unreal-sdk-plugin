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
		: HttpRef{InHttpRef}
		, CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
	{}

	TurnManager::~TurnManager()
	{}

	void TurnManager::GetTurnServers(const THandler<FAccelByteModelsTurnServerList>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/public/turn"), *GetTurnManagerServerUrl());
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void TurnManager::GetClosestTurnServer(const THandler<FAccelByteModelsTurnServer>& OnSuccess, const FErrorHandler& OnError)
	{
		GetTurnServers(THandler<FAccelByteModelsTurnServerList>::CreateLambda([this, OnError, OnSuccess](const FAccelByteModelsTurnServerList& Result)
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
                    FUDPPing::UDPEcho(FString::Printf(TEXT("%s:%d"), *Server.Ip, Server.Qos_port), 10.00, FIcmpEchoResultDelegate::CreateLambda([this, Server, Count, OnSuccess](FIcmpEchoResult &PingResult)
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
                    }));
                }
			}
		}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}

	FString TurnManager::GetTurnManagerServerUrl() const
	{
		return SettingsRef.TurnManagerServerUrl.IsEmpty() ? FString::Printf(TEXT("%s/turnmanager"), *SettingsRef.BaseUrl) : SettingsRef.TurnManagerServerUrl;
	}
	
} // Namespace Api
} // Namespace AccelByte
