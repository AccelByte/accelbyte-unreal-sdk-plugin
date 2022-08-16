// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerQosManagerApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

	ServerQosManager::ServerQosManager(ServerCredentials const& InCredentialsRef
		, ServerSettings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef)
		: CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
		, HttpRef{InHttpRef}
	{}

	ServerQosManager::~ServerQosManager()
	{}

	void ServerQosManager::GetQosServers(const THandler<FAccelByteModelsQosServerList>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Url = FString::Printf(TEXT("%s/public/qos"), *SettingsRef.QosManagerServerUrl);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerQosManager::GetActiveQosServers(const THandler<FAccelByteModelsQosServerList>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Url = FString::Printf(TEXT("%s/public/namespace/%s/qos"), *SettingsRef.QosManagerServerUrl, *SettingsRef.Namespace);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void ServerQosManager::GetServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnSuccess, const FErrorHandler& OnError)
	{
		Latencies.Empty();
		GetQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda([this, OnSuccess](const FAccelByteModelsQosServerList Result)
		{
			for (int i = 0; i < Result.Servers.Num(); i++)
			{
				auto Server = Result.Servers[i];
				int32 Count = Result.Servers.Num();
				FUDPPing::UDPEcho(FString::Printf(TEXT("%s:%d"), *Server.Ip, Server.Port), FRegistry::ServerSettings.QosPingTimeout, FIcmpEchoResultDelegate::CreateLambda([this, Server, OnSuccess, Count](FIcmpEchoResult &Result)
				{
					Latencies.Add(TPair<FString, float>(Server.Region, Result.Time * 1000));
					if (Count == Latencies.Num())
					{
						OnSuccess.ExecuteIfBound(Latencies);
					}
				}));
			}
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}

} // Namespace Api
} // Namespace AccelByte
