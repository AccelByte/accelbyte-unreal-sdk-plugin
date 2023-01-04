// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteStatisticApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{

	Statistic::Statistic(Credentials const& InCredentialsRef
		, Settings const& InSettingsRef
		, FHttpRetryScheduler& InHttpRef)
		: HttpRef{InHttpRef}
		, CredentialsRef{InCredentialsRef}
		, SettingsRef{InSettingsRef}
	{}

	Statistic::~Statistic()
	{}

	void Statistic::CreateUserStatItems(const TArray<FString>& StatCodes, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems/bulk"), *SettingsRef.StatisticServerUrl, *SettingsRef.Namespace, *CredentialsRef.GetUserId());
		FString Verb = TEXT("POST");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Contents = "[";
		FString Content;
		FAccelByteModelsBulkStatItemCreate StatItemCreate;

		for (int i = 0; i < StatCodes.Num(); i++)
		{
			StatItemCreate.StatCode = StatCodes[i];
			FJsonObjectConverter::UStructToJsonObjectString(StatItemCreate, Content);
			Contents += Content;

			if (i < StatCodes.Num() - 1)
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
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void Statistic::GetAllUserStatItems(const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError, int32 Limit, int32 Offset)
	{
		FReport::Log(FString(__FUNCTION__));

		Statistic::GetUserStatItems({}, {}, OnSuccess, OnError, Limit, Offset);
	}

	void Statistic::GetUserStatItems(const TArray<FString>& StatCodes, const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler & OnError,
		int32 Limit, int32 Offset)
	{
		FReport::Log(FString(__FUNCTION__));

		Statistic::GetUserStatItems(CredentialsRef.GetUserId(), StatCodes, Tags, OnSuccess, OnError, Limit, Offset);
	}

	void Statistic::GetUserStatItems(const FString& UserId, const TArray<FString>& StatCodes, const TArray<FString>& Tags, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess, const FErrorHandler& OnError,
		int32 Limit, int32 Offset)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems"), *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace(), *UserId);
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content;

		FString QueryParams = FAccelByteUtilities::CreateQueryParams({ 
                { TEXT("statCodes"), FString::Join(StatCodes, TEXT(",")) },
				{ TEXT("tags"), 	FString::Join(Tags, TEXT(",")) }, 
				{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
				{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")  },
			});
		Url.Append(QueryParams); 

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void Statistic::IncrementUserStatItems(const TArray<FAccelByteModelsBulkStatItemInc>& Data, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems/value/bulk"), *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
		FString Verb = TEXT("PUT");
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
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	} 
	
	void Statistic::ListUserStatItems(const TArray<FString>& StatCodes,  const TArray<FString>& Tags,  const FString& AdditionalKey, 
		const THandler<TArray<FAccelByteModelsFetchUser>>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/statitems/value/bulk"), *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content = TEXT(""); 
		FString QueryParam = FAccelByteUtilities::CreateQueryParams({
			{ "statCodes", FString::Join(StatCodes, TEXT("&statCodes=")) },
			{ "tags", FString::Join(Tags, TEXT("&tags=")) },
			{ TEXT("additionalKey"), AdditionalKey },
		});

		Url.Append(QueryParam);
	
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void Statistic::BulkUpdateUserStatItemsValue(const FString& AdditionalKey, const TArray<FAccelByteModelsUpdateUserStatItemWithStatCode>& BulkUpdateUserStatItems,
			const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/statitems/value/bulk"), *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content = TEXT("");
		FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUpdateUserStatItemWithStatCode>(BulkUpdateUserStatItems, Content);
		FString QueryParam = FAccelByteUtilities::CreateQueryParams({
				{ TEXT("additionalKey"), AdditionalKey },
			});
		Url.Append(QueryParam);
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void Statistic::UpdateUserStatItemsValue(const FString& StatCode, const FString& AdditionalKey, const FAccelByteModelsPublicUpdateUserStatItem& UpdateUserStatItem,
		const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/stats/%s/statitems/value"), *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *StatCode);
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content = TEXT("");
		FJsonObjectConverter::UStructToJsonObjectString(UpdateUserStatItem, Content); 
		FString QueryParam = FAccelByteUtilities::CreateQueryParams({
				{ TEXT("additionalKey"), AdditionalKey },
			});
		Url.Append(QueryParam);
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}
	
	void Statistic::BulkFetchStatItemsValue(const FString StatCode, const TArray<FString>& UserIds, const THandler<TArray<FAccelByteModelsStatItemValueResponse>>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statitems/bulk"), *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
		FString Verb = TEXT("GET");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content;		 

		FString QueryParams = FAccelByteUtilities::CreateQueryParams({
			{ TEXT("statCode"), StatCode },
			{ TEXT("userIds"), FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(UserIds)  },
			});
		Url.Append(QueryParams);

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void Statistic::ResetUserStatItemValue(const FString& StatCode, const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/stats/%s/statitems/value/reset"), *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *StatCode);
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Contents{};

		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Contents);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

	void Statistic::GetGlobalStatItemsByStatCode(const FString& StatCode, const THandler<FAccelByteModelsGlobalStatItemValueResponse>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		if (StatCode.IsEmpty())
		{
			OnError.ExecuteIfBound(404, TEXT("Url is invalid. StatCode is empty."));
			return;
		}

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/globalstatitems/%s"), *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace(), *StatCode);
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
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}

#if !UE_BUILD_SHIPPING
	void Statistic::BulkResetMultipleUserStatItemsValue(const TArray<FAccelByteModelsResetUserStatItemValue>& UserStatItemValue,
			const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess, const FErrorHandler& OnError)
	{
		FReport::Log(FString(__FUNCTION__));

		FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
		FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statitems/value/reset/bulk"), *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId());
		FString Verb = TEXT("PUT");
		FString ContentType = TEXT("application/json");
		FString Accept = TEXT("application/json");
		FString Content = TEXT(""); 
		FAccelByteUtilities::TArrayUStructToJsonString(UserStatItemValue, Content);
		
		FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
		Request->SetURL(Url);
		Request->SetHeader(TEXT("Authorization"), Authorization);
		Request->SetVerb(Verb);
		Request->SetHeader(TEXT("Content-Type"), ContentType);
		Request->SetHeader(TEXT("Accept"), Accept);
		Request->SetContentAsString(Content);
		HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
	}
#endif // !UE_BUILD_SHIPPING
	
} // Namespace Api
} // Namespace AccelByte
