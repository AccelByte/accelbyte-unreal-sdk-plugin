// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerStatisticApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteServerSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerStatistic::ServerStatistic(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

ServerStatistic::~ServerStatistic()
{}

void ServerStatistic::CreateUserStatItems(const FString& UserId
	, const TArray<FString>& StatCodes
	, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/statitems/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	FString Contents = "[";
	FString Content;

	for (int i = 0; i < StatCodes.Num(); i++)
	{
		FAccelByteModelsBulkStatItemCreate StatItemCreate;
		StatItemCreate.StatCode = StatCodes[i];
		FJsonObjectConverter::UStructToJsonObjectString(StatItemCreate, Content);
		Contents += Content;
		if (i < StatCodes.Num() - 1)
		{
			Contents += ",";
		}
	}
	Contents += "]";

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, OnSuccess, OnError);
}

void ServerStatistic::GetAllUserStatItems(const FString& UserId
	, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	GetUserStatItems(UserId, {}, {}, OnSuccess, OnError);
}

void ServerStatistic::GetUserStatItems(const FString& UserId
	, const TArray<FString>& StatCodes
	, const TArray<FString>& Tags
	, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/statitems")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("statCodes"), FString::Join(StatCodes, TEXT(",")) },
		{ TEXT("tags"), FString::Join(Tags, TEXT(",")) },
	};

	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerStatistic::IncrementManyUsersStatItems(const TArray<FAccelByteModelsBulkUserStatItemInc>& Data
	, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/statitems/value/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

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
	}
	Contents += "]";

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Contents, OnSuccess, OnError);
}

void ServerStatistic::IncrementUserStatItems(const FString& UserId
	, const TArray<FAccelByteModelsBulkStatItemInc>& Data
	, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/statitems/value/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

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
	}
	Contents += "]";

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Contents, OnSuccess, OnError);
}

void ServerStatistic::BulkFetchUserStatItemValues(const FString& StatCode
	, const TArray<FString>& UserIds
	, const FString& AdditionalKey
	, const THandler<TArray<FAccelByteModelsFetchUser>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/statitems/value/bulk/getOrDefault")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("statCode"), StatCode },
		{ TEXT("additionalKey"), AdditionalKey },
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerStatistic::BulkUpdateMultipleUserStatItemsValue(const TArray<FAccelByteModelsUpdateUserStatItem>& BulkUpdateMultipleUserStatItems
	, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/statitems/value/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace());

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUpdateUserStatItem>(BulkUpdateMultipleUserStatItems, Content);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void ServerStatistic::BulkResetUserStatItemsValues(const FString& UserId
	, const FString& AdditionalKey
	, const TArray<FAccelByteModelsUserStatItem>& BulkUserStatItems
	, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/users/%s/statitems/value/reset/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUserStatItem>(BulkUserStatItems, Content);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("additionalKey"), AdditionalKey },
	};

	HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnError);
}

void ServerStatistic::BulkResetMultipleUserStatItemsValue(const TArray<FAccelByteModelsResetUserStatItemValue>& UserStatItemValue
	, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/statitems/value/reset/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace());
	FString Content = TEXT("");
	FAccelByteUtilities::TArrayUStructToJsonString(UserStatItemValue, Content);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void ServerStatistic::BulkUpdateUserStatItemValue(const FString& UserId
	, const FString& AdditionalKey
	, const TArray<FAccelByteModelsUpdateUserStatItemWithStatCode>& BulkUpdateUserStatItems
	, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/users/%s/statitems/value/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId);

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUpdateUserStatItemWithStatCode>(BulkUpdateUserStatItems, Content);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("additionalKey"), AdditionalKey },
	};

	HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnError);
}

void ServerStatistic::UpdateUserStatItemValue(const FString& UserId
	, const FString& StatCode
	, const FString& AdditionalKey
	, const FAccelByteModelsUpdateUserStatItem& UpdateUserStatItemValue
	, const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/users/%s/stats/%s/statitems/value")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *StatCode);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("additionalKey"), AdditionalKey },
	};

	HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, UpdateUserStatItemValue, OnSuccess, OnError);
}

void ServerStatistic::DeleteUserStatItems(const FString& UserId
	, const FString& StatCode
	, const FString& AdditionalKey
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/users/%s/stats/%s/statitems")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *UserId
		, *StatCode);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("additionalKey"), AdditionalKey },
	};

	HttpClient.ApiRequest(TEXT("DELETE"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerStatistic::GetGlobalStatItemsByStatCode(const FString& StatCode
	, const THandler<FAccelByteModelsGlobalStatItemValueResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (StatCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. StatCode is empty."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/globalstatitems/%s")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef.GetClientNamespace()
		, *StatCode);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte
