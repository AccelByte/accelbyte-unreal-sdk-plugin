// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteStatisticApi.h"
#include "ModuleManager.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "AccelByteRegistry.h"
#include "AccelByteHttpRetryScheduler.h"
#include "AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

Statistic::Statistic(const Credentials& Credentials, const AccelByte::Settings& Setting) : StatisticCredentials(Credentials), StatisticSettings(Setting){}

Statistic::~Statistic() {}

void Statistic::GetAllStatItems(const FString& ProfileId, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *StatisticCredentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s/statitems"), *StatisticSettings.StatisticServerUrl, *StatisticCredentials.GetUserNamespace(), *StatisticCredentials.GetUserId(), *ProfileId);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Statistic::GetStatItemsByStatCodes(const FString& ProfileId, TArray<FString> StatCodes, const THandler<TArray<FAccelByteModelsUserStatItemInfo>>& OnSuccess, const FErrorHandler& OnError)
{
	if (StatCodes.Num() < 1)
	{
		OnError.ExecuteIfBound(EHttpResponseCodes::NotFound, TEXT("Please provide at least one stat code."));
	}
	else
	{
		FString Authorization = FString::Printf(TEXT("Bearer %s"), *StatisticCredentials.GetUserAccessToken());
		FString Url = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s/statitems/byStatCodes"), *StatisticSettings.StatisticServerUrl, *StatisticCredentials.GetUserNamespace(), *StatisticCredentials.GetUserId(), *ProfileId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content;

		for (int i = 0; i < StatCodes.Num(); i++)
		{
			Url.Append((i == 0) ? TEXT("?statCodes=") : TEXT(",")).Append(StatCodes[i]);
		}

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}
}

} // Namespace Api
} // Namespace AccelByte
