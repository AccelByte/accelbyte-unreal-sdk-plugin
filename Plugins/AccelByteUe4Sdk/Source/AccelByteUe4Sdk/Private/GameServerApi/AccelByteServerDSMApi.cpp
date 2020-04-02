// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerQosManagerApi.h"
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
			if (ServerType != EServerType::NONE)
			{
				OnError.ExecuteIfBound(409, TEXT("Server already registered."));
			}
			else if (DSMServerUrl.IsEmpty())
			{
				GetServerUrlDelegate.BindLambda([this, Port, OnSuccess, OnError](const FAccelByteModelsDSMClient& Result)
				{
					DSMServerUrl = Result.Host_address;
					RegisterServerToDSM(Port, OnSuccess, OnError);
				});
				GetRegionDSMUrl(GetServerUrlDelegate, OnError);
			}
			else if (DSPubIp.IsEmpty())
			{
				GetPubIpDelegate.BindLambda([this, Port, OnSuccess, OnError](const FAccelByteModelsPubIp& Result)
				{
					DSPubIp = Result.Ip;
					RegisterServerToDSM(Port, OnSuccess, OnError);
				});
				GetPubIp(GetPubIpDelegate, OnError);
			}
			else
			{
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
				ParseCommandParam();
				FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
				FString Url = FString::Printf(TEXT("%s/dsm/namespaces/%s/servers/register"), *DSMServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
				FString Verb = TEXT("POST");
				FString ContentType = TEXT("application/json");
				FString Accept = TEXT("application/json");
				const FAccelByteModelsRegisterServerRequest Register{
					Game_version,
					DSPubIp,
					ServerName,
					Port,
					Provider
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
						FAccelByteModelsServerInfo Result;
						if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
						{
							SetServerName(Result.Pod_name);
						}
						if (bHeartbeatIsAutomatic)
						{
							HeartBeatDelegateHandle = FTicker::GetCoreTicker().AddTicker(HeartBeatDelegate, HeartBeatTimeoutSeconds);
						}
						SetServerType(EServerType::CLOUDSERVER);
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
		}

        void ServerDSM::SendShutdownToDSM(const bool KillMe, const FString& MatchId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
        {
            Report report;
            report.GetFunctionLog(FString(__FUNCTION__));
			if (ServerType != EServerType::CLOUDSERVER)
			{
				OnError.ExecuteIfBound(409, TEXT("Server not registered as Cloud Server."));
			}
			if (DSMServerUrl.IsEmpty())
			{
				GetServerUrlDelegate.BindLambda([this, KillMe, MatchId, OnSuccess, OnError](const FAccelByteModelsDSMClient& Result)
				{
					DSMServerUrl = Result.Host_address;
					SendShutdownToDSM(KillMe, MatchId, OnSuccess, OnError);
				});
				GetRegionDSMUrl(GetServerUrlDelegate, OnError);
			}
			else if (DSPubIp.IsEmpty())
			{
				GetPubIpDelegate.BindLambda([this, KillMe, MatchId, OnSuccess, OnError](const FAccelByteModelsPubIp& Result)
				{
					DSPubIp = Result.Ip;
					SendShutdownToDSM(KillMe, MatchId, OnSuccess, OnError);
				});
				GetPubIp(GetPubIpDelegate, OnError);
			}
			else
			{
				FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
				FString Url = FString::Printf(TEXT("%s/dsm/namespaces/%s/servers/shutdown"), *DSMServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
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
				ServerType = EServerType::NONE;
				FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
			}
        }

        void ServerDSM::RegisterLocalServerToDSM(const FString IPAddress, const int32 Port, const FString ServerName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
        {
            Report report;
            report.GetFunctionLog(FString(__FUNCTION__));
			if (ServerType != EServerType::NONE)
			{
				OnError.ExecuteIfBound(409, TEXT("Server already registered."));
			}
			else
			{
				this->ServerName = ServerName;
				FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
				FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/local/register"), *FRegistry::ServerSettings.DSMControllerServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
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
						SetServerType(EServerType::LOCALSERVER);
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
        }

        void ServerDSM::DeregisterLocalServerFromDSM(const FString& ServerName, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
        {
            Report report;
            report.GetFunctionLog(FString(__FUNCTION__));
			if (ServerType != EServerType::LOCALSERVER)
			{
				OnError.ExecuteIfBound(409, TEXT("Server not registered as Local Server."));
			}
			else
			{
				FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
				FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/local/deregister"), *FRegistry::ServerSettings.DSMControllerServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
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
				ServerType = EServerType::NONE;
				FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
			}
        }

        bool ServerDSM::HeartBeatTick(float DeltaTime)
        {
            Report report;
            report.GetFunctionLog(FString(__FUNCTION__));
			PollHeartBeat();
            return true;
        }

		void ServerDSM::ConfigureHeartBeat(bool bIsAutomatic, int TimeoutSeconds, int ErrorRetry)
		{
			bHeartbeatIsAutomatic = bIsAutomatic;
			HeartBeatTimeoutSeconds = TimeoutSeconds;
			HeartBeatErrorRetry = ErrorRetry;
		}

		void ServerDSM::PollHeartBeat()
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));
			FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
			FString ServerUrl;
			switch (ServerType)
			{
				case EServerType::CLOUDSERVER: ServerUrl = DSMServerUrl + "/dsm"; break;
				case EServerType::LOCALSERVER:
				default: ServerUrl = FRegistry::ServerSettings.DSMControllerServerUrl; break;
			}
			FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/heartbeat"), *ServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
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
			FRegistry::HttpRetryScheduler.ProcessRequest(Request, OnHeartBeatResponse, FPlatformTime::Seconds());
		}

		void ServerDSM::SetOnMatchRequest(THandler<FAccelByteModelsMatchRequest> OnMatchRequest)
		{
			this->OnMatchRequest = OnMatchRequest;
		}

		void ServerDSM::SetOnHeartBeatErrorDelegate(const FErrorHandler& OnError)
		{
			this->OnHeartBeatError = OnError;
		}

		void ServerDSM::GetRegionDSMUrl(const THandler<FAccelByteModelsDSMClient>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));
			if (FRegistry::ServerQosManager.Latencies.Num() == 0)
			{
				GetLatenciesDelegate.BindLambda([this, OnSuccess, OnError](const TArray<TPair<FString, float>>& Result)
				{
					GetRegionDSMUrl(OnSuccess, OnError);
				});
				FRegistry::ServerQosManager.GetServerLatencies(GetLatenciesDelegate, OnError);
			}
			else
			{
				TPair<FString, float> Region{ "", 10000 };
				for (auto Latency : FRegistry::ServerQosManager.Latencies)
				{
					if (Region.Value > Latency.Value)
					{
						Region = Latency;
					}
				}
				FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
				FString Url = FString::Printf(TEXT("%s/public/dsm?region=%s"), *FRegistry::ServerSettings.DSMControllerServerUrl, *Region.Key);
				FString Verb = TEXT("GET");
				FString ContentType = TEXT("application/json");
				FString Accept = TEXT("application/json");
				FString Contents;

				FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
				Request->SetURL(Url);
				Request->SetHeader(TEXT("Authorization"), Authorization);
				Request->SetVerb(Verb);
				Request->SetHeader(TEXT("Content-Type"), ContentType);
				Request->SetHeader(TEXT("Accept"), Accept);
				Request->SetContentAsString(Contents);
				FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
			}
		}

		void ServerDSM::GetPubIp(const THandler<FAccelByteModelsPubIp>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));
			FString Url = FString::Printf(TEXT("https://api.ipify.org?format=json"));
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

		void ServerDSM::ParseCommandParam()
		{
			const auto CommandParams = FCommandLine::Get();
			TArray<FString> Tokens;
			TArray<FString> Switches;
			FCommandLine::Parse(CommandParams, Tokens, Switches);
			UE_LOG(LogTemp, Log, TEXT("Params: %s"), CommandParams);
			bool bIsProviderFound = false;
			bool bIsGameVersionFound = false;
			for (auto Param : Switches)
			{
				if (Param.Contains("provider"))
				{
					TArray<FString> ArraySplit;
					Param.ParseIntoArray(ArraySplit, TEXT("="), 1);
					Provider = ArraySplit[1];
					bIsProviderFound = true;
				}
				if (Param.Contains("game_version"))
				{
					TArray<FString> ArraySplit;
					Param.ParseIntoArray(ArraySplit, TEXT("="), 1);
					Game_version = ArraySplit[1];
					bIsGameVersionFound = true;
				}
				if (bIsProviderFound && bIsGameVersionFound)
				{
					break;
				}
			}
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

				if (Response->GetResponseCode() >= 400 && Response->GetResponseCode() < 500)
				{
					if (HeartBeatRetryCount <= HeartBeatErrorRetry)
					{
						HeartBeatRetryCount++;
					}
					else
					{
						OnHeartBeatError.ExecuteIfBound(Response->GetResponseCode(), Response->GetContentAsString());
						FTicker::GetCoreTicker().RemoveTicker(HeartBeatDelegateHandle);
					}
				}
				else
				{
					HeartBeatRetryCount = 0;
				}
			});
		}

        ServerDSM::~ServerDSM() {}

    } // Namespace GameServerApi
} // Namespace AccelByte
