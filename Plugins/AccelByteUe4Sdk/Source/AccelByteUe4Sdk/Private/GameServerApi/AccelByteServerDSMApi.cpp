// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
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
#include "Core/AccelByteEnvironment.h"
#include "Http.h"
#include "Modules/ModuleManager.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "JsonUtilities.h"


namespace AccelByte
{
    namespace GameServerApi
    {

		void ServerDSM::RegisterServerToDSM(const int32 Port, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));
			ParseCommandParam();

			if (ServerType != EServerType::NONE)
			{
				OnError.ExecuteIfBound(409, TEXT("Server already registered."));
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
				ServerName = Environment::GetEnvironmentVariable("POD_NAME", 100);
				FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
				FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/register"), *FRegistry::ServerSettings.DSMControllerServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
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
						FAccelByteModelsServerInfo Result;
						if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
						{
							SetServerName(Result.Pod_name);
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
			if (ServerType == EServerType::LOCALSERVER)
			{
				OnError.ExecuteIfBound(409, TEXT("Server not registered as Cloud Server."));
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
				FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/shutdown"), *FRegistry::ServerSettings.DSMControllerServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
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
				ServerType = EServerType::NONE;
				FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
			}
		}

		void ServerDSM::RegisterLocalServerToDSM(const FString IPAddress, const int32 Port, const FString ServerName_, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));
			if (ServerType != EServerType::NONE)
			{
				OnError.ExecuteIfBound(409, TEXT("Server already registered."));
			}
			else
			{
				this->ServerName = ServerName_;
				FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
				FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/local/register"), *FRegistry::ServerSettings.DSMControllerServerUrl, *FRegistry::ServerCredentials.GetClientNamespace());
				FString Verb = TEXT("POST");
				FString ContentType = TEXT("application/json");
				FString Accept = TEXT("application/json");
				const FAccelByteModelsRegisterLocalServerRequest Register{
					IPAddress,
					ServerName_,
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

		void ServerDSM::DeregisterLocalServerFromDSM(const FString& ServerName_, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));
			if (ServerType == EServerType::CLOUDSERVER)
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
					ServerName_
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
				ServerType = EServerType::NONE;
				FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
			}
		}

		void ServerDSM::GetSessionId(const THandler<FAccelByteModelsServerSessionResponse>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));
			FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/namespaces/%s/servers/%s/session"), *FRegistry::ServerSettings.DSMControllerServerUrl, *FRegistry::ServerCredentials.GetClientNamespace(), *ServerName);
			FString Verb = TEXT("GET");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
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

		// Should not be called from constructor, FCommandLine::Get() is not ready.
		void ServerDSM::ParseCommandParam()
		{
			const auto CommandParams = FCommandLine::Get();
			TArray<FString> Tokens;
			TArray<FString> Switches;
			FCommandLine::Parse(CommandParams, Tokens, Switches);
			UE_LOG(LogTemp, Log, TEXT("Params: %s"), CommandParams);
			for (auto Param : Switches)
			{
				if (Param.Contains("provider"))
				{
					TArray<FString> ArraySplit;
					Param.ParseIntoArray(ArraySplit, TEXT("="), 1);
					Provider = ArraySplit[1];
				}
				else if (Param.Contains("game_version"))
				{
					TArray<FString> ArraySplit;
					Param.ParseIntoArray(ArraySplit, TEXT("="), 1);
					Game_version = ArraySplit[1];
				}
				else if (Param.Contains("region"))
				{
					TArray<FString> ArraySplit;
					Param.ParseIntoArray(ArraySplit, TEXT("="), 1);
					Region = ArraySplit[1];
				}
			}
		}

		ServerDSM::ServerDSM(const AccelByte::ServerCredentials& Credentials, const AccelByte::ServerSettings& Settings) {}

		ServerDSM::~ServerDSM() {}

    } // Namespace GameServerApi
} // Namespace AccelByte
