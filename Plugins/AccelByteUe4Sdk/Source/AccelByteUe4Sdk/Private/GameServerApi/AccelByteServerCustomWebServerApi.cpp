// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerCustomWebServerApi.h"
#include "GameDelegates.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteRegistry.h"

namespace AccelByte
{
	namespace GameServerApi
	{

		void ServerCustomWebServer::Start(const uint16 Port)
		{
			SetPort(Port);
			FGameDelegates::Get().GetWebServerActionDelegate() = FWebServerActionDelegate::CreateLambda([this](int32 UserIndex, const FString& Action, const FString& URL, const TMap<FString, FString>& Params, TMap<FString, FString>& Response)
			{
				UE_LOG(LogTemp, Log, TEXT("Got a Request"));
				FString ContentType = "application/json; charset=utf-8";
				FString Body = "";
				FString Code = "404";
				static const TCHAR MessageFormat[] = TEXT(R"sz({"code": %d, "message": "%s"})sz");

				if(URL == "/claim" && Action == "POST")
				{
					FAccelByteModelsDSMMessage Result;
					bool bSuccess = FJsonObjectConverter::JsonObjectStringToUStruct(Params["Body"], &Result, 0, 0);
					if (bSuccess)
					{
						Code = "200";
						Body = FString::Printf(MessageFormat, 0, TEXT("Success"));
						ServerCustomWebServer::DSClaimDelegate.ExecuteIfBound(Result);
						FRegistry::ServerCredentials.SetMatchId(Result.Message.Session_id);
					}
					else
					{
						Code = "400";
						Body = FString::Printf(MessageFormat, 1003, TEXT("Wrong JSON format"));
						ServerCustomWebServer::ErrorDelegate.ExecuteIfBound(-1, FString::Printf(TEXT("Error cannot parse response %s, Raw: %s"), *URL, *Params["Body"]));
					}
				}
				else
				{
				Body = FString::Printf(MessageFormat, 404, TEXT("Function not found"));
				ServerCustomWebServer::ErrorDelegate.ExecuteIfBound(-1, FString::Printf(TEXT("Function not found, Url %s, Body: %s"), *URL, *Params["Body"]));
				}

				Response.Add(TEXT("Content-Type"), ContentType);
				Response.Add(TEXT("Content-Length"), FString::FromInt(Body.Len()));
				Response.Add(TEXT("Body"), Body);
				Response.Add(TEXT("Code"), Code);
			});
			ServerInstance->Start(Port);
		}

		void ServerCustomWebServer::Stop()
		{
			UnbindEvents();
		}

		void ServerCustomWebServer::UnbindEvents()
		{
			DSClaimDelegate.Unbind();
			ErrorDelegate.Unbind();
			FGameDelegates::Get().GetWebServerActionDelegate().Unbind();
			ServerInstance->Unbind();
		}

		ServerCustomWebServer::ServerCustomWebServer()
		{
			ServerInstance = MakeShared<FAccelByteCustomWebServer>();
		}

		ServerCustomWebServer::~ServerCustomWebServer()
		{
			Stop();
		}

	} // Namespace GameServerApi
} // Namespace AccelByte
