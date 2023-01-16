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
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

QosManager::~QosManager()
{}

void QosManager::GetQosServers(const THandler<FAccelByteModelsQosServerList>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/qos")
		, *SettingsRef.QosManagerServerUrl);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	HttpClient.Request(TEXT("GET"), Url, TEXT(""), Headers, OnSuccess, OnError);
}

void QosManager::GetActiveQosServers(const THandler<FAccelByteModelsQosServerList>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/qos?status=ACTIVE")
		, *SettingsRef.QosManagerServerUrl
		, *SettingsRef.Namespace);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	HttpClient.Request(TEXT("GET"), Url, TEXT(""), Headers, OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte
