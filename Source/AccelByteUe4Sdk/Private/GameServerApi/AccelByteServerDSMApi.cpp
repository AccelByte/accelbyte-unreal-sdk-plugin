// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerDSMApi.h"
#include "GameServerApi/AccelByteServerQosManagerApi.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteUtilities.h"
#include "Http.h"


namespace AccelByte
{
    namespace GameServerApi
    {

		void ServerDSM::RegisterServerToDSM(const int32 Port, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
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
				FAccelByteNetUtilities::GetPublicIP(GetPubIpDelegate, OnError);
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
				FReport::Log(TEXT("Starting DSM Register Request..."));

				OnRegisterResponse.BindLambda([this, OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
				{
					if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
					{
						FTicker::GetCoreTicker().RemoveTicker(AutoShutdownDelegateHandle);
						FAccelByteModelsServerInfo Result;
						if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
						{
							RegisteredServerInfo = Result;
							SetServerName(Result.Pod_name);
						}
						if (CountdownTimeStart != -1)
						{
							AutoShutdownDelegateHandle = FTicker::GetCoreTicker().AddTicker(AutoShutdownDelegate, ShutdownTickSeconds);
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
			FReport::Log(FString(__FUNCTION__));
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
				FAccelByteNetUtilities::GetPublicIP(GetPubIpDelegate, OnError);
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
				FReport::Log(TEXT("Starting DSM Shutdown Request..."));
				FTicker::GetCoreTicker().RemoveTicker(AutoShutdownDelegateHandle);
				ServerType = EServerType::NONE;
				FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());

				RegisteredServerInfo = FAccelByteModelsServerInfo();
			}
		}

		void ServerDSM::RegisterLocalServerToDSM(const FString IPAddress, const int32 Port, const FString ServerName_, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
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
				FReport::Log(TEXT("Starting DSM Register Local Request..."));

				OnRegisterResponse.BindLambda([this, OnSuccess, OnError](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
				{
					if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
					{
						FTicker::GetCoreTicker().RemoveTicker(AutoShutdownDelegateHandle);
						FAccelByteModelsServerInfo Result;
						if (FJsonObjectConverter::JsonObjectStringToUStruct(Response->GetContentAsString(), &Result, 0, 0))
						{
							RegisteredServerInfo = Result;
						}
						if (CountdownTimeStart != -1)
						{
							AutoShutdownDelegateHandle = FTicker::GetCoreTicker().AddTicker(AutoShutdownDelegate, ShutdownTickSeconds);
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

		void ServerDSM::RegisterLocalServerToDSM(const int32 Port, const FString ServerName_, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			GetPubIpDelegate.BindLambda([this, Port, ServerName_, OnSuccess, OnError](const FAccelByteModelsPubIp& Result)
			{
				RegisterLocalServerToDSM(Result.Ip, Port, ServerName_, OnSuccess, OnError);
			});
			FAccelByteNetUtilities::GetPublicIP(GetPubIpDelegate, OnError);
		}

		void ServerDSM::DeregisterLocalServerFromDSM(const FString& ServerName_, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));
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
				FReport::Log(TEXT("Starting DSM Deregister Request..."));
				FTicker::GetCoreTicker().RemoveTicker(AutoShutdownDelegateHandle);
				ServerType = EServerType::NONE;
				FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());

				RegisteredServerInfo = FAccelByteModelsServerInfo();
			}
		}

		void ServerDSM::GetServerInfo(const THandler<FAccelByteModelsServerInfo>& OnSuccess, const FErrorHandler& OnError)
		{
			if (!RegisteredServerInfo.Pod_name.IsEmpty())
			{
				OnSuccess.ExecuteIfBound(RegisteredServerInfo);
			}
			else
			{
				OnError.ExecuteIfBound(400, TEXT("No Server Registered."));
			}
		}

		void ServerDSM::GetSessionId(const THandler<FAccelByteModelsServerSessionResponse>& OnSuccess, const FErrorHandler& OnError)
		{
			FReport::Log(FString(__FUNCTION__));

			if (ServerType == EServerType::NONE)
			{
				OnError.ExecuteIfBound(400, TEXT("Server not Registered."));
			}
			else
			{
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

				FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(THandler<FAccelByteModelsServerSessionResponse>::CreateLambda([OnSuccess, this](const FAccelByteModelsServerSessionResponse& Result)
					{
						if (!Result.Session_id.IsEmpty())
						{
							bIsDSClaimed = true;
						}
						OnSuccess.ExecuteIfBound(Result);
					}), OnError), FPlatformTime::Seconds());
			}
		}

		bool ServerDSM::ShutdownTick(float DeltaTime)
		{
			FReport::Log(FString(__FUNCTION__));

			if (CountdownTimeStart != -1 && bIsDSClaimed && (ServerType == EServerType::CLOUDSERVER))
			{
				if (ShutdownCountdown <= 0)
				{
					SendShutdownToDSM(true, "", OnAutoShutdown, OnAutoShutdownError);
				}
				else
				{
					if (GetPlayerNum() == 0)
					{
						ShutdownCountdown -= ShutdownTickSeconds;
					}
					else
					{
						ShutdownCountdown = CountdownTimeStart;
					}
				}
			}
			return true;
		}

		void ServerDSM::ConfigureAutoShutdown(uint32 TickSeconds, int CountdownStart)
		{
			if (CountdownStart >= -1)
			{
				CountdownTimeStart = CountdownStart;
				ShutdownCountdown = CountdownStart;
				ShutdownTickSeconds = TickSeconds;
			}
			else
			{
				CountdownTimeStart = -1;
				ShutdownCountdown = -1;
			}
		}

		void ServerDSM::SetOnAutoShutdownResponse(FVoidHandler OnAutoShutdown_)
		{
			OnAutoShutdown = OnAutoShutdown_;
		}

		void ServerDSM::SetOnAutoShutdownErrorDelegate(const FErrorHandler& OnError)
		{
			OnAutoShutdownError = OnError;
		}

		// Should not be called from constructor, FCommandLine::Get() is not ready.
		void ServerDSM::ParseCommandParam()
		{
			const auto CommandParams = FCommandLine::Get();
			TArray<FString> Tokens;
			TArray<FString> Switches;
			FCommandLine::Parse(CommandParams, Tokens, Switches);
			FReport::Log(FString::Printf(TEXT("Params: %s"), CommandParams));
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

		int32 ServerDSM::GetPlayerNum()
		{
			UGameEngine* GameEngine = CastChecked<UGameEngine>(GEngine);
			if (GameEngine)
			{
				UWorld* World = GameEngine->GetGameWorld();
				if (World)
				{
					AGameStateBase* GameState = World->GetGameState();
					if (GameState)
					{
						auto PlayerArray = GameState->PlayerArray;
						return PlayerArray.Num();
					}
					else
					{
						return 0;
					}
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}

		ServerDSM::ServerDSM(const AccelByte::ServerCredentials& Credentials, const AccelByte::ServerSettings& Settings)
		{
			AutoShutdownDelegate = FTickerDelegate::CreateRaw(this, &ServerDSM::ShutdownTick);
		}

		ServerDSM::~ServerDSM() {}

    } // Namespace GameServerApi
} // Namespace AccelByte
