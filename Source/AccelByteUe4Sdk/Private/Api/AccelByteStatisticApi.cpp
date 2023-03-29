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
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

Statistic::~Statistic()
{}

void Statistic::CreateUserStatItems(const TArray<FString>& StatCodes
	, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

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

void Statistic::GetAllUserStatItems(const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
	, const FErrorHandler& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	GetUserStatItems({}, {}, OnSuccess, OnError, Limit, Offset);
}

void Statistic::GetUserStatItems(const TArray<FString>& StatCodes
	, const TArray<FString>& Tags
	, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
	, const FErrorHandler & OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	GetUserStatItems(CredentialsRef.GetUserId(), StatCodes, Tags, OnSuccess, OnError, Limit, Offset);
}

void Statistic::GetUserStatItems(const FString& UserId
	, const TArray<FString>& StatCodes
	, const TArray<FString>& Tags
	, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
	, const FErrorHandler& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	FString QueryParams = FAccelByteUtilities::CreateQueryParams({ 
            { TEXT("statCodes"), FString::Join(StatCodes, TEXT(",")) },
			{ TEXT("tags"), 	FString::Join(Tags, TEXT(",")) }, 
			{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
			{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")  },
		});
	Url.Append(QueryParams); 

	HttpClient.ApiRequest("GET", Url, {}, FString(), OnSuccess, OnError);
}

void Statistic::IncrementUserStatItems(const TArray<FAccelByteModelsBulkStatItemInc>& Data
	, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems/value/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

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

void Statistic::ListUserStatItems(const TArray<FString>& StatCodes
	, const TArray<FString>& Tags
	, const FString& AdditionalKey 
	, const THandler<TArray<FAccelByteModelsFetchUser>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/statitems/value/bulk")
		, *SettingsRef.StatisticServerUrl, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	FString QueryParam = FAccelByteUtilities::CreateQueryParams({
		{ "statCodes", FString::Join(StatCodes, TEXT("&statCodes=")) },
		{ "tags", FString::Join(Tags, TEXT("&tags=")) },
		{ TEXT("additionalKey"), AdditionalKey },
	});

	Url.Append(QueryParam);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Statistic::BulkUpdateUserStatItemsValue(const FString& AdditionalKey
	, const TArray<FAccelByteModelsUpdateUserStatItemWithStatCode>& BulkUpdateUserStatItems
	, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/statitems/value/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUpdateUserStatItemWithStatCode>(BulkUpdateUserStatItems, Content);

	const TMap<FString, FString> QueryParams = {
		{TEXT("additionalKey"), AdditionalKey }
	};

	HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnError);
}

void Statistic::UpdateUserStatItemsValue(const FString& StatCode
	, const FString& AdditionalKey
	, const FAccelByteModelsPublicUpdateUserStatItem& UpdateUserStatItem
	, const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/stats/%s/statitems/value")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *StatCode);

	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(UpdateUserStatItem, Content); 

	const TMap<FString, FString> QueryParams = {
		{TEXT("additionalKey"), AdditionalKey}
	};

	HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnError);
}

void Statistic::BulkFetchStatItemsValue(const FString StatCode
	, const TArray<FString>& UserIds
	, const THandler<TArray<FAccelByteModelsStatItemValueResponse>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statitems/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> QueryParams;
	if (!StatCode.IsEmpty())
	{
		QueryParams.Add(TEXT("statCode"), StatCode);
	}
	if (UserIds.Num() != 0)
	{
		QueryParams.Add(TEXT("userIds"), FString::Join(UserIds, TEXT(",")));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Statistic::ResetUserStatItemValue(const FString& StatCode
	, const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/stats/%s/statitems/value/reset")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *StatCode);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

void Statistic::GetGlobalStatItemsByStatCode(const FString& StatCode
	, const THandler<FAccelByteModelsGlobalStatItemValueResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (StatCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. StatCode is empty."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/globalstatitems/%s")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *StatCode);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Statistic::GetUserStatCycleItems(
	const FString& CycleId,
	const THandler<FAccelByteModelsUserStatCycleItemPagingSlicedResult>& OnSuccess,
	const FErrorHandler& OnError,
	const int32 Limit,
	const int32 Offset,
	const TArray<FString> StatCodes)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statCycles/%s/statCycleitems")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *CycleId);

	TMap<FString, FString> QueryParams = {
		{TEXT("limit"), FString::FromInt(Limit)},
		{TEXT("offset"), FString::FromInt(Offset)},
	};

	if(StatCodes.Num() > 0)
	{
		QueryParams.Add(TEXT("statCodes"), FString::Join(StatCodes, TEXT(",")));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Statistic::GetStatCycleConfig(const FString& CycleId
	,const THandler<FAccelByteModelsStatCycleConfig>& OnSuccess
	,const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statCycles/%s")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *CycleId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void Statistic::GetListStatCycleConfigs(const EAccelByteCycle& CycleType,
	const THandler<FAccelByteModelsStatCycleConfigPagingResult>& OnSuccess,
	const FErrorHandler& OnError,
	const int32 Limit,
	const int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statCycles")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> QueryParams = {
		{TEXT("limit"), FString::FromInt(Limit)},
		{TEXT("offset"), FString::FromInt(Offset)},
	};

	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteCycle"), true);
	if (EnumPtr)
	{
		FString EnumText = EnumPtr->GetNameStringByIndex(static_cast<int32>(CycleType));

		QueryParams.Add(TEXT("cycleType"), EnumText.ToUpper());
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

#if !UE_BUILD_SHIPPING
void Statistic::BulkResetMultipleUserStatItemsValue(const TArray<FAccelByteModelsResetUserStatItemValue>& UserStatItemValue
		, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
		, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statitems/value/reset/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());
	FString Verb = TEXT("PUT");
	FString Content = TEXT(""); 
	FAccelByteUtilities::TArrayUStructToJsonString(UserStatItemValue, Content);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
#endif // !UE_BUILD_SHIPPING

} // Namespace Api
} // Namespace AccelByte
