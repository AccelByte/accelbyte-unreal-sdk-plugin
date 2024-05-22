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

FString Statistic::ConvertUserStatisticSortByToString(EAccelByteStatisticSortBy SortBy)
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

FAccelByteTaskWPtr Statistic::CreateUserStatItems(TArray<FString> const& StatCodes
	, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
	, FErrorHandler const& OnError)
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
	
	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::GetAllUserStatItems(THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStatisticSortBy SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	return GetUserStatItems({}, {}, OnSuccess, OnError, Limit, Offset, SortBy);
}

FAccelByteTaskWPtr Statistic::GetUserStatItems(TArray<FString> const& StatCodes
	, TArray<FString> const& Tags
	, THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStatisticSortBy SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	return GetUserStatItems(CredentialsRef->GetUserId(), StatCodes, Tags, OnSuccess, OnError, Limit, Offset, SortBy);
}

FAccelByteTaskWPtr Statistic::GetUserStatItems(FString const& UserId
	, TArray<FString> const& StatCodes
	, TArray<FString> const& Tags
	, THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStatisticSortBy SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserId.IsEmpty())
	{
		OnSuccess.ExecuteIfBound(FAccelByteModelsUserStatItemPagingSlicedResult{});
		return nullptr;
	}

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
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

	return HttpClient.ApiRequest("GET", Url, QueryParams, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::IncrementUserStatItems(TArray<FAccelByteModelsBulkStatItemInc> const& Data
	, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
	, FErrorHandler const& OnError)
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

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Contents, Headers, OnSuccess, OnError);
} 

FAccelByteTaskWPtr Statistic::ListUserStatItems(TArray<FString> const& StatCodes
	, TArray<FString> const& Tags
	, FString const& AdditionalKey 
	, THandler<TArray<FAccelByteModelsFetchUser>> const& OnSuccess
	, FErrorHandler const& OnError)
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
	
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::BulkUpdateUserStatItemsValue(FString const& AdditionalKey
	, TArray<FAccelByteModelsUpdateUserStatItemWithStatCode> const& BulkUpdateUserStatItems
	, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
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

	return HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::UpdateUserStatItemsValue(FString const& StatCode
	, FString const& AdditionalKey
	, FAccelByteModelsPublicUpdateUserStatItem const& UpdateUserStatItem
	, THandler<FAccelByteModelsUpdateUserStatItemValueResponse> const& OnSuccess
	, FErrorHandler const& OnError)
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

	return HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::BulkFetchStatItemsValue(FString const& StatCode
	, TArray<FString> const& UserIds
	, THandler<TArray<FAccelByteModelsStatItemValueResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (StatCode.IsEmpty())
	{
		OnSuccess.ExecuteIfBound(TArray<FAccelByteModelsStatItemValueResponse>{});
		return nullptr;
	}
	if (UserIds.Num() <= 0)
	{
		OnSuccess.ExecuteIfBound(TArray<FAccelByteModelsStatItemValueResponse>{});
		return nullptr;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statitems/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace());


	TMultiMap<FString, FString> QueryParams {
		{ TEXT("statCode"), StatCode },
		{ TEXT("userIds"), FString::Join(UserIds, TEXT(",")) },
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::ResetUserStatItemValue(FString const& StatCode
	, THandler<FAccelByteModelsUpdateUserStatItemValueResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/stats/%s/statitems/value/reset")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId()
		, *StatCode);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::GetGlobalStatItemsByStatCode(FString const& StatCode
	, THandler<FAccelByteModelsGlobalStatItemValueResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (StatCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. StatCode is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/globalstatitems/%s")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *StatCode);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::GetUserStatCycleItems(FString const& CycleId
	, THandler<FAccelByteModelsUserStatCycleItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, TArray<FString> const& StatCodes)
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::GetStatCycleConfig(FString const& CycleId
	, THandler<FAccelByteModelsStatCycleConfig> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statCycles/%s")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *CycleId);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::GetListStatCycleConfigs(EAccelByteCycle CycleType
	, THandler<FAccelByteModelsStatCycleConfigPagingResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr Statistic::GetMyStatItems(TArray<FString> const& StatCodes
	, TArray<FString> const& Tags
	, THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteStatisticSortBy SortBy
	, int32 Limit
	, int32 Offset)
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

	return HttpClient.ApiRequest(TEXT("GET"),Url,QueryParams,OnSuccess,OnError);
}

FAccelByteTaskWPtr Statistic::GetMyStatItemValues(TArray<FString> const& StatCodes
	, TArray<FString> const& Tags
	, FString const& AdditionalKey
	, THandler<TArray<FAccelByteModelsFetchUser>> const& OnSuccess
	, FErrorHandler const& OnError)
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

	return HttpClient.ApiRequest(TEXT("GET"),Url,QueryParams,OnSuccess,OnError);
}

FAccelByteTaskWPtr Statistic::GetMyStatCycleItems(FString const& CycleId
	, TArray<FString> const& StatCodes
	, THandler<FAccelByteModelsUserStatCycleItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteStatisticSortBy SortBy
	, int32 Limit
	, int32 Offset)
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

#if !UE_BUILD_SHIPPING
FAccelByteTaskWPtr Statistic::BulkResetMultipleUserStatItemsValue(TArray<FAccelByteModelsResetUserStatItemValue> const& UserStatItemValue
		, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
		, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/statitems/value/reset/bulk")
		, *SettingsRef.StatisticServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());
	FString Verb = TEXT("PUT");
	FString Content = TEXT(""); 
	FAccelByteUtilities::TArrayUStructToJsonString(UserStatItemValue, Content);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
#endif // !UE_BUILD_SHIPPING

} // Namespace Api
} // Namespace AccelByte
