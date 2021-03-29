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
				ServerName = Environment::GetEnvironmentVariable("POD_NAME", 100);
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
			if (ServerType == EServerType::LOCALSERVER)
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

		void ServerDSM::SetOnMatchRequest(THandler<FAccelByteModelsMatchRequest> OnMatchRequest_)
		{
			this->OnMatchRequest = OnMatchRequest_;
		}

		void ServerDSM::SetOnHeartBeatErrorDelegate(const FErrorHandler& OnError)
		{
			this->OnHeartBeatError = OnError;
		}

		void ServerDSM::GetRegionDSMUrl(const THandler<FAccelByteModelsDSMClient>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));
			if (Region.IsEmpty() && FRegistry::ServerQosManager.Latencies.Num() == 0)
			{
				GetLatenciesDelegate.BindLambda([this, OnSuccess, OnError](const TArray<TPair<FString, float>>& Result)
				{
					GetRegionDSMUrl(OnSuccess, OnError);
				});
				FRegistry::ServerQosManager.GetServerLatencies(GetLatenciesDelegate, OnError);
			}
			else
			{
				if (Region.IsEmpty())
				{
					TPair<FString, float> RegionData{ "", 10000 };
					for (auto Latency : FRegistry::ServerQosManager.Latencies)
					{
						if (RegionData.Value > Latency.Value)
						{
							RegionData = Latency;
						}
					}
					Region = RegionData.Key;
				}
				FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::ServerCredentials.GetClientAccessToken());
				FString Url = FString::Printf(TEXT("%s/public/dsm?region=%s"), *FRegistry::ServerSettings.DSMControllerServerUrl, *Region);
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
				FRegistry::HttpRetryScheduler.ProcessRequest(Request, FHttpRequestCompleteDelegate::CreateLambda(
				[OnSuccess, OnError, this]
				(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessful)
				{
					Report report;
					report.GetHttpResponse(Request, Response);

					if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
					{
						// backward compatibility checks
						// old: single DSM client
						// new: array of DSM clients
						const FString JsonString = Response->GetContentAsString();
						TSharedPtr<FJsonValue> JsonParsed;
						const TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(JsonString);
						bool bSuccess = false;
						int32 ErrorCode = 0;
						FString ErrorMessage;
						if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
						{
							ErrorCode = (int32)ErrorCodes::JsonDeserializationFailed;
							const TArray<TSharedPtr<FJsonValue>>* JsonArray;
							const TSharedPtr<FJsonObject>* JsonObject;
							if (JsonParsed->TryGetArray(JsonArray))
							{
								TArray<FAccelByteModelsDSMClient> DsmClients;
								if (FJsonObjectConverter::JsonArrayToUStruct(*JsonArray, &DsmClients,0 ,0))
								{
									for (const auto& DsmClient : DsmClients)
									{
										if (DsmClient.Provider == Provider && DsmClient.Status == "HEALTHY")
										{
											bSuccess = true;
											OnSuccess.ExecuteIfBound(DsmClient);
											return;
										}
									}

									if (bSuccess == false)
									{
										ErrorCode = (int32)ErrorCodes::InvalidResponse;
										ErrorMessage = FString::Printf(TEXT("Cannot found healthy DSM for provider '%s' region '%s'"), *Provider, *Region);
									}
								}
							}
							else if (JsonParsed->TryGetObject(JsonObject))
							{
								FAccelByteModelsDSMClient DsmClient;
								if (FJsonObjectConverter::JsonObjectToUStruct(JsonObject->ToSharedRef(), &DsmClient, 0, 0))
								{
									bSuccess = true;
								   OnSuccess.ExecuteIfBound(DsmClient);
									return;
								}
							}
						}

						if (!bSuccess)
						{
							if (ErrorMessage.IsEmpty())
							{
								ErrorMessage = ErrorMessages::Default.at(ErrorCode);
							}
							OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
						}
					}
					else
					{
						int32 Code;
						FString Message;
						HandleHttpError(Request, Response, Code, Message);
						OnError.ExecuteIfBound(Code, Message);
					}

				}), FPlatformTime::Seconds());
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

		void RemoveMemberAttributeFromBackend(FJsonObject& jsonObject)
		{
			if (jsonObject.HasField("matching_allies"))
			{
				const TArray<TSharedPtr<FJsonValue>>* matching_allies;
				if (jsonObject.TryGetArrayField("matching_allies", matching_allies))
				{
					for (int i = 0; i < matching_allies->Num(); i++)
					{
						const TArray<TSharedPtr<FJsonValue>>* matching_parties;
						if ((*matching_allies)[i]->AsObject()->TryGetArrayField("matching_parties", matching_parties))
						{
							for (int j = 0; j < matching_parties->Num(); j++)
							{
								const TSharedPtr<FJsonObject>* matching_party;
								if ((*matching_parties)[j]->TryGetObject(matching_party))
								{
									const TSharedPtr<FJsonObject>* partyAttribute;
									if ((*matching_party)->TryGetObjectField("party_attributes", partyAttribute))
									{
										if ((*partyAttribute)->HasField("member_attributes"))
										{
											(*partyAttribute)->RemoveField("member_attributes");
										}
									}
								}
							}
						}
					}
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
					if (Response->GetContent().Num() > 0)
					{
						HandleHttpResultOk(Response, THandler<FJsonObject>::CreateLambda([OnSuccess = OnMatchRequest](const FJsonObject& jsonObject)
						{
							FJsonObject copyJsonObject = FJsonObject(jsonObject);
							RemoveMemberAttributeFromBackend(copyJsonObject);
							TSharedRef<FJsonObject> JsonObjectRef = MakeShared<FJsonObject>(copyJsonObject);
							FAccelByteModelsMatchRequest MatchRequest;
							bool bParseSuccess = FJsonObjectConverter::JsonObjectToUStruct<FAccelByteModelsMatchRequest>(JsonObjectRef, &MatchRequest, 0, 0);

							if (bParseSuccess)
							{
								OnSuccess.ExecuteIfBound(MatchRequest);
							}
						}));
					}

					return;
				}
				int32 Code;
				FString Message;
				HandleHttpError(Request, Response, Code, Message);
				if (((Code >= 400 && Code < 500) || Code == (int32)ErrorCodes::DedicatedServerNotFoundException))
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
