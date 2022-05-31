// Copyright (c) 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteQosManagerApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

QosManager::QosManager(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

	QosManager::~QosManager()
	{}

	void QosManager::GetQosServers(
		const THandler<FAccelByteModelsQosServerList>& OnSuccess,
		const FErrorHandler& OnError) const
	{
		FReport::Log(FString(__FUNCTION__));

		const FString Url = FString::Printf(TEXT("%s/public/qos"), *SettingsRef.QosManagerServerUrl);
		const FString Verb = TEXT("GET");
		const FString ContentType = TEXT("application/json");
		const FString Accept = TEXT("application/json");
		
		const FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void QosManager::GetActiveQosServers(
		const THandler<FAccelByteModelsQosServerList>& OnSuccess,
		const FErrorHandler& OnError) const
	{
		FReport::Log(FString(__FUNCTION__));

		const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/qos?status=ACTIVE"), *SettingsRef.QosManagerServerUrl, *SettingsRef.Namespace);
		const FString Verb = TEXT("GET");
		const FString ContentType = TEXT("application/json");
		const FString Accept = TEXT("application/json");
			
		const FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}
	
} // Namespace Api
} // Namespace AccelByte
