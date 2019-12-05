// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteStatisticApi.h"
#include "Modules/ModuleManager.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
	namespace Api
	{

		Statistic::Statistic(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting)
		{}

		Statistic::~Statistic()
		{}

		void Statistic::GetAllUserStatItems(const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			Statistic::GetUserStatItems({}, {}, OnSuccess, OnError);
		}

		void Statistic::GetUserStatItemsByStatCodes(const TArray<FString>& StatCodes, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			Statistic::GetUserStatItems(StatCodes, {}, OnSuccess, OnError);
		}

		void Statistic::GetUserStatItemsByTags(const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			Statistic::GetUserStatItems({}, Tags, OnSuccess, OnError);
		}

		void Statistic::GetUserStatItems(const TArray<FString>& StatCodes, const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
			FString Url             = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems"), *Settings.StatisticServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId());
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

		void Statistic::BulkAddStatItemValue(const TArray<FAccelByteModelsBulkUserStatItemInc>& Data, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
			FString Url             = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statitems/value/bulk"), *Settings.StatisticServerUrl, *Credentials.GetUserNamespace());
			FString Verb            = TEXT("PUT");
			FString ContentType     = TEXT("application/json");
			FString Accept          = TEXT("application/json");
			FString Contents        = "[";
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

		void Statistic::BulkAddUserStatItemValue(const TArray<FAccelByteModelsBulkStatItemInc>& Data, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
			FString Url             = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems/value/bulk"), *Settings.StatisticServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId());
			FString Verb            = TEXT("PUT");
			FString ContentType     = TEXT("application/json");
			FString Accept          = TEXT("application/json");
			FString Contents        = "[";
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

		void Statistic::AddUserStatItemValue(const FString& StatCode, const float& Inc, const THandler<FAccelByteModelsStatItemIncResult>& OnSuccess, const FErrorHandler& OnError)
		{
			Report report;
			report.GetFunctionLog(FString(__FUNCTION__));

			FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
			FString Url             = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/stats/%s/statitems/value"), *Settings.StatisticServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *StatCode);
			FString Verb            = TEXT("PUT");
			FString ContentType     = TEXT("application/json");
			FString Accept          = TEXT("application/json");
			FString Content         = FString::Printf(TEXT("{\"inc\":%f}"), Inc);

			FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
			Request->SetURL(Url);
			Request->SetHeader(TEXT("Authorization"), Authorization);
			Request->SetVerb(Verb);
			Request->SetHeader(TEXT("Content-Type"), ContentType);
			Request->SetHeader(TEXT("Accept"), Accept);
			Request->SetContentAsString(Content);
			FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
		}

	} // Namespace Api
} // Namespace AccelByte
