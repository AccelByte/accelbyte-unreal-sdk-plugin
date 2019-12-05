// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerDSMApi.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteServerCredentials.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteHttpRetryScheduler.h"

namespace AccelByte
{
namespace GameServerApi
{

void ServerDSM::RegisterServerToDSM(const int32 Port, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	const int32 length = 100;
	TCHAR Pod_name[length];
#if PLATFORM_WINDOWS
	FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"), Pod_name, 1000);
#elif PLATFORM_LINUX
	FLinuxPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"), Pod_name, 1000);
#elif PLATFORM_MAC
	FApplePlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"), Pod_name, 1000);
#endif
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/servers/register"), *Settings.DSMServerUrl, *Credentials.GetClientNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	const FAccelByteModelsRegisterServerRequest Register{
		Pod_name,
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
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerDSM::SendShutdownToDSM(const bool KillMe, const FString& MatchId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	const int32 length = 100;
	TCHAR Pod_name[length];
#if PLATFORM_WINDOWS
	FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"), Pod_name, 1000);
#elif PLATFORM_LINUX
	FLinuxPlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"), Pod_name, 1000);
#elif PLATFORM_MAC
	FApplePlatformMisc::GetEnvironmentVariable(TEXT("POD_NAME"), Pod_name, 1000);
#endif
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/servers/shutdown"), *Settings.DSMServerUrl, *Credentials.GetClientNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	const FAccelByteModelsShutdownServerRequest Shutdown{
		KillMe,
		Pod_name,
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
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

ServerDSM::ServerDSM(const AccelByte::ServerCredentials& Credentials, const AccelByte::ServerSettings& Settings) : Credentials(Credentials), Settings(Settings)
{}

ServerDSM::~ServerDSM(){}

} // Namespace GameServerApi
} // Namespace AccelByte
