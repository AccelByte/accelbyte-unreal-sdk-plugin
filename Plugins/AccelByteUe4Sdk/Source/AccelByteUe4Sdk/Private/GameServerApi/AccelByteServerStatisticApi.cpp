// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
	namespace GameServerApi
	{

		ServerStatistic::ServerStatistic(const AccelByte::ServerCredentials& Credentials, const AccelByte::ServerSettings& Setting) : Credentials(Credentials), Settings(Setting)
		{}

		ServerStatistic::~ServerStatistic()
		{}

		void ServerStatistic::GetAllUserStatItems(const FString& UserId, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			ServerStatistic::GetUserStatItems(UserId, {}, {}, OnSuccess, OnError);
		}

		void ServerStatistic::GetUserStatItemsByStatCodes(const FString& UserId, const TArray<FString>& StatCodes, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			ServerStatistic::GetUserStatItems(UserId, StatCodes, {}, OnSuccess, OnError);
		}

		void ServerStatistic::GetUserStatItemsByTags(const FString& UserId, const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			ServerStatistic::GetUserStatItems(UserId, {}, Tags, OnSuccess, OnError);
		}

		void ServerStatistic::GetUserStatItems(const FString& UserId, const TArray<FString>& StatCodes, const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
			FString Url             = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/statitems"), *Settings.StatisticServerUrl, *Credentials.GetClientNamespace(), *UserId);
			FString Verb            = TEXT("GET");
			FString ContentType     = TEXT("application/json");
			FString Accept          = TEXT("application/json");
			FString Content;

			for (int i = 0; i < StatCodes.Num(); i++)
			{
				Url.Append((i == 0) ? TEXT("?statCodes=") : TEXT(",")).Append(StatCodes[i]);
			}

			for (int i = 0; i < Tags.Num(); i++)
			{
				Url.Append((i == 0) ? TEXT("?tags=") : TEXT(",")).Append(Tags[i]);
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


		void ServerStatistic::CreateUserStatItem(const FString& UserId, const FString& StatCode, const THandler< FAccelByteModelsUserStatItemInfo>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/stats/%s/statitems"), *Settings.StatisticServerUrl, *Credentials.GetClientNamespace(), *UserId, *StatCode);
			FString Verb = TEXT("POST");
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

		void ServerStatistic::BulkCreateUserStatItems(const FString& UserId, const TArray<FAccelByteModelsBulkStatItemCreate>& Data, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetClientAccessToken());
			FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/statitems/bulk"), *Settings.StatisticServerUrl, *Credentials.GetClientNamespace(), *UserId);
			FString Verb = TEXT("POST");
			FString ContentType = TEXT("application/json");
			FString Accept = TEXT("application/json");
			FString Contents = "[";
			FString Content;
			for (int i = 0; i < Data.Num(); i++)
			{
				FJsonObjectConverter::UStructToJsonObjectString(Data[i], Content);
				Contents += Content;
				if (i < Data.Num() - 1)
				{
					Contents += ",";
				}
				else
				{
					Contents += "]";
				}
			}

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Contents);

			FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}

	} // Namespace Api
} // Namespace AccelByte
