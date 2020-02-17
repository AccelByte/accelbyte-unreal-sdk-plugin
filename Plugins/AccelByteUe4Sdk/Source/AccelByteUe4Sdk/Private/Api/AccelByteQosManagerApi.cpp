// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteQosManagerApi.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

	QosManager::QosManager(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Settings) : Credentials(Credentials), Settings(Settings)
	{}

	QosManager::~QosManager()
	{}

	void QosManager::GetQosServers(const THandler<FAccelByteModelsQosServerList>& OnSuccess, const FErrorHandler& OnError)
	{
		Report report;
		report.GetFunctionLog(FString(__FUNCTION__));

		FString Url = FString::Printf(TEXT("%s/public/qos"), *FRegistry::Settings.QosManagerServerUrl);
		UE_LOG(LogTemp, Log, TEXT("%s"), *Url);
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

} // Namespace Api
} // Namespace AccelByte
