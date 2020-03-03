// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteQos.h"
#include "Api/AccelByteQosManagerApi.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
namespace Api
{

	Qos::Qos()
	{}

	Qos::~Qos()
	{}

	void Qos::GetServerLatencies(const THandler<TArray<TPair<FString, float>>>& OnSuccess, const FErrorHandler& OnError)
	{
		Latencies.Empty();
		FRegistry::QosManager.GetQosServers(THandler<FAccelByteModelsQosServerList>::CreateLambda([this, OnSuccess](const FAccelByteModelsQosServerList Result)
		{
			for (int i=0;i< Result.Servers.Num();i++)
			{
				auto Server = Result.Servers[i];
				int32 Count = Result.Servers.Num();
				FUDPPing::UDPEcho(FString::Printf(TEXT("%s:%d"), *Server.Ip, Server.Port), 10.00, FIcmpEchoResultDelegate::CreateLambda([this, Server, OnSuccess, Count](FIcmpEchoResult &Result)
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
