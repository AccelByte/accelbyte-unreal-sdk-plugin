// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerDSMApi.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteError.h"
#include "Http.h"
#include "Modules/ModuleManager.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"


namespace AccelByte
{
    namespace GameServerApi
    {

        void ServerDSM::RegisterServerToDSM(const int32 Port, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
        {
            Report report;
            report.GetFunctionLog(FString(__FUNCTION__));
            FString PodName;

#if ENGINE_MINOR_VERSION > 20
#if PLATFORM_WINDOWS
            ServerName = FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"));
#elif PLATFORM_LINUX
			ServerName = FLinuxPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"));
#elif PLATFORM_MAC
            ServerName = FApplePlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"));
#endif
#else
			TCHAR data[100];
#if PLATFORM_WINDOWS
			FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"), data, 100);
#elif PLATFORM_LINUX
			FLinuxPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"), data, 100);
#elif PLATFORM_MAC
			FApplePlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"), data, 100)
#endif
			ServerName = FString::Printf(TEXT("%s"), data);
#endif
            FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
            FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/register"), *FRegistry::ServerSettings.DSMServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
            FString Verb = TEXT("POST");
            FString ContentType = TEXT("application/json");
            FString Accept = TEXT("application/json");
            const FAccelByteModelsRegisterServerRequest Register{
                PodName,
                Port
            };
            FString Contents;
            FJsonObjectConverter::UStructToJsonObjectString(Register, Contents);
            FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Contents);
			UE_LOG(LogTemp, Log, TEXT("Starting DSM Register Request..."));

			OnRegisterResponse.BindLambda([this, OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
			{
				Report report;
				report.GetHttpResponse(Request, Response);

				if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
				{
					FTicker::GetCoreTicker().RemoveTicker(HeartBeatDelegateHandle);

					if (bHeartbeatIsAutomatic)
					{
						HeartBeatDelegateHandle = FTicker::GetCoreTicker().AddTicker(HeartBeatDelegate, HeartBeatTimeoutSeconds);
					}
					HandleHttpResultOk(Response, OnSuccess);
					return;
				}

				int32 Code;
				FString Message;
				HandleHttpError(Request, Response, Code, Message);
				OnError.ExecuteIfBound(Code, Message);

			});

			FRegistry::HttpRetryScheduler.ProcessRequest(Request, OnRegisterResponse, FPlatformTime::Seconds());
		}

        void ServerDSM::SendShutdownToDSM(const bool KillMe, const FString& MatchId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
        {
            Report report;
            report.GetFunctionLog(FString(__FUNCTION__));
            const int32 length = 100;
            FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
            FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/shutdown"), *FRegistry::ServerSettings.DSMServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
            FString Verb = TEXT("POST");
            FString ContentType = TEXT("application/json");
            FString Accept = TEXT("application/json");
            const FAccelByteModelsShutdownServerRequest Shutdown{
                KillMe,
                ServerName,
                MatchId
            };
            FString Contents;
            FJsonObjectConverter::UStructToJsonObjectString(Shutdown, Contents);
            FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
            Request->SetURL(Url);
            Request->SetHeader(TEXT("Authorization"), Authorization);
            Request->SetVerb(Verb);
            Request->SetHeader(TEXT("Content-Type"), ContentType);
            Request->SetHeader(TEXT("Accept"), Accept);
            Request->SetContentAsString(Contents);
            UE_LOG(LogTemp, Log, TEXT("Starting DSM Shutdown Request..."));
			FTicker::GetCoreTicker().RemoveTicker(HeartBeatDelegateHandle);
			FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
        }

        void ServerDSM::RegisterLocalServerToDSM(const FString IPAddress, const int32 Port, const FString ServerName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
        {
            Report report;
            report.GetFunctionLog(FString(__FUNCTION__));
			this->ServerName = ServerName;
            FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
            FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/local/register"), *FRegistry::ServerSettings.DSMServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
			FString Verb = TEXT("POST");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			const FAccelByteModelsRegisterLocalServerRequest Register{
				IPAddress,
				ServerName,
				Port
			};
			FString Contents;
			FJsonObjectConverter::UStructToJsonObjectString(Register, Contents);
			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Contents);
			UE_LOG(LogTemp, Log, TEXT("Starting DSM Register Local Request..."));

			OnRegisterResponse.BindLambda([this, OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
			{
				Report report;
				report.GetHttpResponse(Request, Response);

				if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
				{
					FTicker::GetCoreTicker().RemoveTicker(HeartBeatDelegateHandle);

					if (bHeartbeatIsAutomatic)
					{
						HeartBeatDelegateHandle = FTicker::GetCoreTicker().AddTicker(HeartBeatDelegate, HeartBeatTimeoutSeconds);
					}
					HandleHttpResultOk(Response, OnSuccess);
					return;
				}

				int32 Code;
				FString Message;
				HandleHttpError(Request, Response, Code, Message);
				OnError.ExecuteIfBound(Code, Message);

			});

			FRegistry::HttpRetryScheduler.ProcessRequest(Request, OnRegisterResponse, FPlatformTime::Seconds());
        }

        void ServerDSM::DeregisterLocalServerFromDSM(const FString& ServerName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
        {
            Report report;
            report.GetFunctionLog(FString(__FUNCTION__));
            const int32 length = 100;
            FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
            FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/local/deregister"), *FRegistry::ServerSettings.DSMServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
            FString Verb = TEXT("POST");
            FString ContentType = TEXT("application/json");
            FString Accept = TEXT("application/json");
            const FAccelByteModelsDeregisterLocalServerRequest Deregister{
                ServerName,
            };
            FString Contents;
            FJsonObjectConverter::UStructToJsonObjectString(Deregister, Contents);
            FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
            Request->SetURL(Url);
            Request->SetHeader(TEXT("Authorization"), Authorization);
            Request->SetVerb(Verb);
            Request->SetHeader(TEXT("Content-Type"), ContentType);
            Request->SetHeader(TEXT("Accept"), Accept);
            Request->SetContentAsString(Contents);
            UE_LOG(LogTemp, Log, TEXT("Starting DSM Deregister Request..."));
			FTicker::GetCoreTicker().RemoveTicker(HeartBeatDelegateHandle);
			FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
        }

        bool ServerDSM::HeartBeatTick(float DeltaTime)
        {
            Report report;
            report.GetFunctionLog(FString(__FUNCTION__));
			PollHeartBeat();
            return true;
        }

		void ServerDSM::ConfigureHeartBeat(bool bIsAutomatic, int TimeoutSeconds)
		{
			bHeartbeatIsAutomatic = bIsAutomatic;
			HeartBeatTimeoutSeconds = TimeoutSeconds;
		}

		void ServerDSM::PollHeartBeat()
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));
			const int32 length = 100;
			FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/heartbeat"), *FRegistry::ServerSettings.DSMServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
			FString Verb = TEXT("POST");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			FString Contents = FString::Printf(TEXT("{\"name\":\"%s\"}"), *ServerName);

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Contents);
			UE_LOG(LogTemp, Log, TEXT("Starting DSM Deregister Request..."));
			FRegistry::HttpRetryScheduler.ProcessRequest(Request, OnHeartBeatResponse, FPlatformTime::Seconds());
		}

		void ServerDSM::SetOnMatchRequest(THandler<FAccelByteModelsMatchRequest> OnMatchRequest)
		{
			this->OnMatchRequest = OnMatchRequest;
		}

        ServerDSM::ServerDSM(const AccelByte::ServerCredentials& Credentials, const AccelByte::ServerSettings& Settings)
        {
			HeartBeatDelegate = FTickerDelegate::CreateRaw(this, &ServerDSM::HeartBeatTick);
			OnHeartBeatResponse.BindLambda([this](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
			{
				Report report;
				report.GetHttpResponse(Request, Response);

				if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
				{
					if (Response->GetContentLength() > 0)
					{
						HandleHttpResultOk(Response, OnMatchRequest);
					}

					return;
				}

				int32 Code;
				FString Message;
				HandleHttpError(Request, Response, Code, Message);
				OnHeartBeatError.ExecuteIfBound(Code, Message);

			});
		}

        ServerDSM::~ServerDSM() {}

    } // Namespace GameServerApi
} // Namespace AccelByte
