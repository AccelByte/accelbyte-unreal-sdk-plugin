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

FString Statistic::ConvertUserStatisticSortByToString(const EAccelByteStatisticSortBy& SortBy)
{
	switch (SortBy)
	{
	case EAccelByteStatisticSortBy::STAT_CODE:
		return TEXT("statCode");
	case EAccelByteStatisticSortBy::STAT_CODE_ASC:
		return TEXT("statCode:asc");
	case EAccelByteStatisticSortBy::STAT_CODE_DESC:
		return TEXT("statCode:desc");
	case EAccelByteStatisticSortBy::CREATED_AT:
		return TEXT("createdAt");
	case EAccelByteStatisticSortBy::CREATED_AT_ASC:
		return TEXT("createdAt:asc");
	case EAccelByteStatisticSortBy::CREATED_AT_DESC:
		return TEXT("createdAt:desc");
	case EAccelByteStatisticSortBy::UPDATED_AT:
		return TEXT("updatedAt");
	case EAccelByteStatisticSortBy::UPDATED_AT_ASC:
		return TEXT("updatedAt:asc");
	case EAccelByteStatisticSortBy::UPDATED_AT_DESC:
		return TEXT("updatedAt:desc");
	default:
		return TEXT("");
	}
}

void Statistic::CreateUserStatItems(const TArray<FString>& StatCodes
	, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

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

	TMap<FString, FString> Headers;
	Headers.Add(GHeaderABLogSquelch, TEXT("true"));
	
	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, Headers, OnSuccess, OnError);
}

void Statistic::GetAllUserStatItems(const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
	, const FErrorHandler& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStatisticSortBy SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	GetUserStatItems({}, {}, OnSuccess, OnError, Limit, Offset, SortBy);
}

void Statistic::GetUserStatItems(const TArray<FString>& StatCodes
	, const TArray<FString>& Tags
	, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
	, const FErrorHandler & OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStatisticSortBy SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	GetUserStatItems(CredentialsRef->GetUserId(), StatCodes, Tags, OnSuccess, OnError, Limit, Offset, SortBy);
}

void Statistic::GetUserStatItems(const FString& UserId
	, const TArray<FString>& StatCodes
	, const TArray<FString>& Tags
	, const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess
	, const FErrorHandler& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStatisticSortBy SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserId.IsEmpty())
	{
		OnSuccess.ExecuteIfBound(FAccelByteModelsUserStatItemPagingSlicedResult{});
		return;
	}

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	FString StatCodesValue = FString::Join(StatCodes, TEXT(","));
	FString TagsValue = FString::Join(Tags, TEXT(","));

	const TMultiMap<FString, FString> QueryParams { 
        { TEXT("statCodes"), StatCodesValue },
		{ TEXT("tags"), TagsValue },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("sortBy"), SortBy == EAccelByteStatisticSortBy::NONE ? TEXT("") : ConvertUserStatisticSortByToString(SortBy) },
	};
	
	TMap<FString, FString> Headers;
	Headers.Add(GHeaderABLogSquelch, TEXT("true"));

	HttpClient.ApiRequest("GET", Url, QueryParams, FString(), Headers, OnSuccess, OnError);
}

void Statistic::IncrementUserStatItems(const TArray<FAccelByteModelsBulkStatItemInc>& Data
	, const THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/statitems/value/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

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

	TMap<FString, FString> Headers;
	Headers.Add(GHeaderABLogSquelch, TEXT("true"));

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Contents, Headers, OnSuccess, OnError);
} 

void Statistic::ListUserStatItems(const TArray<FString>& StatCodes
	, const TArray<FString>& Tags
	, const FString& AdditionalKey 
	, const THandler<TArray<FAccelByteModelsFetchUser>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef->GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/statitems/value/bulk")
		, *SettingsRef.StatisticServerUrl, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("additionalKey"), AdditionalKey }
	};

	for (const auto& StatCode : StatCodes)
	{
		QueryParams.AddUnique(TEXT("statCodes"), StatCode);
	}
	
	for (const auto& Tag : Tags)
	{
		QueryParams.AddUnique(TEXT("tags"), Tag);
	}
	
	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Statistic::BulkUpdateUserStatItemsValue(const FString& AdditionalKey
	, const TArray<FAccelByteModelsUpdateUserStatItemWithStatCode>& BulkUpdateUserStatItems
	, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/statitems/value/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUpdateUserStatItemWithStatCode>(BulkUpdateUserStatItems, Content);

	const TMultiMap<FString, FString> QueryParams = {
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
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *StatCode);

	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(UpdateUserStatItem, Content); 

	const TMultiMap<FString, FString> QueryParams = {
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

	if (StatCode.IsEmpty())
	{
		OnSuccess.ExecuteIfBound(TArray<FAccelByteModelsStatItemValueResponse>{});
		return;
	}
	if (UserIds.Num() <= 0)
	{
		OnSuccess.ExecuteIfBound(TArray<FAccelByteModelsStatItemValueResponse>{});
		return;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statitems/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace());


	TMultiMap<FString, FString> QueryParams {
		{ TEXT("statCode"), StatCode },
		{ TEXT("userIds"), FString::Join(UserIds, TEXT(",")) },
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Statistic::ResetUserStatItemValue(const FString& StatCode
	, const THandler<FAccelByteModelsUpdateUserStatItemValueResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/stats/%s/statitems/value/reset")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
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
		, *CredentialsRef->GetNamespace()
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
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *CycleId);

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("statCodes"), FString::Join(StatCodes, TEXT(",")) },
		{ TEXT("limit"), FString::FromInt(Limit) },
		{ TEXT("offset"), FString::FromInt(Offset) },
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Statistic::GetStatCycleConfig(const FString& CycleId
	,const THandler<FAccelByteModelsStatCycleConfig>& OnSuccess
	,const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statCycles/%s")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
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
		, *CredentialsRef->GetNamespace());

	TMultiMap<FString, FString> QueryParams = {
		{TEXT("limit"), FString::FromInt(Limit)},
		{TEXT("offset"), FString::FromInt(Offset)},
	};

	FString EnumText = FAccelByteUtilities::GetUEnumValueAsString(CycleType);
	if (!EnumText.IsEmpty())
	{
		QueryParams.Add(TEXT("cycleType"), EnumText.ToUpper());
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void Statistic::GetMyStatItems(const TArray<FString>& StatCodes,
	const TArray<FString>& Tags,
	const THandler<FAccelByteModelsUserStatItemPagingSlicedResult>& OnSuccess,
	const FErrorHandler& OnError,
	const EAccelByteStatisticSortBy& SortBy,
	const int32 Limit,
	const int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/statitems")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace());

	FString StatCodesValue = FString::Join(StatCodes, TEXT("&statCodes="));
	FString TagsValue = FString::Join(Tags, TEXT("&tags="));

	TMultiMap<FString, FString> QueryParams = {
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("sortBy"), SortBy == EAccelByteStatisticSortBy::NONE ? TEXT("") : FGenericPlatformHttp::UrlEncode(ConvertUserStatisticSortByToString(SortBy))},
	};

	for (auto& StatCode : StatCodes)
	{
		QueryParams.AddUnique("statCodes", StatCode);
	}

	for(auto& Tag : Tags)
	{
		QueryParams.AddUnique("tags", Tag);
	}

	HttpClient.ApiRequest(TEXT("GET"),Url,QueryParams,OnSuccess,OnError);
}

void Statistic::GetMyStatItemValues(
	const TArray<FString>& StatCodes,
	const TArray<FString>& Tags,
	const FString& AdditionalKey,
	const THandler<TArray<FAccelByteModelsFetchUser>>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/statitems/value/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace());

	TMultiMap<FString, FString> QueryParams = {
		{ TEXT("additionalKey"), AdditionalKey }
	};

	for (auto& StatCode : StatCodes)
	{
		QueryParams.AddUnique("statCodes", StatCode);
	}

	for(auto& Tag : Tags)
	{
		QueryParams.AddUnique("tags", Tag);
	}

	HttpClient.ApiRequest(TEXT("GET"),Url,QueryParams,OnSuccess,OnError);
}

void Statistic::GetMyStatCycleItems(
	const FString& CycleId,
	const TArray<FString>& StatCodes,
	const THandler<FAccelByteModelsUserStatCycleItemPagingSlicedResult>& OnSuccess,
	const FErrorHandler& OnError,
	const EAccelByteStatisticSortBy& SortBy,
	const int32 Limit,
	const int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/me/statCycles/%s/statCycleitems")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *CycleId);

	TMultiMap<FString, FString> QueryParams = {
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("sortBy"), SortBy == EAccelByteStatisticSortBy::NONE ? TEXT("") : FGenericPlatformHttp::UrlEncode(ConvertUserStatisticSortByToString(SortBy))},
	};

	for (auto& StatCode : StatCodes)
	{
		QueryParams.AddUnique("statCodes", StatCode);
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

#if !UE_BUILD_SHIPPING
void Statistic::BulkResetMultipleUserStatItemsValue(const TArray<FAccelByteModelsResetUserStatItemValue>& UserStatItemValue
		, const THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>>& OnSuccess
		, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statitems/value/reset/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());
	FString Verb = TEXT("PUT");
	FString Content = TEXT(""); 
	FAccelByteUtilities::TArrayUStructToJsonString(UserStatItemValue, Content);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
#endif // !UE_BUILD_SHIPPING

} // Namespace Api
} // Namespace AccelByte
