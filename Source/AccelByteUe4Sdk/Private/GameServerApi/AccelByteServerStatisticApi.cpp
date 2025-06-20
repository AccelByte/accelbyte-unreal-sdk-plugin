// Copyright (c) 2019 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerStatisticApi.h"

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
	, FHttpRetryScheduler& InHttpRef
	, TSharedPtr<FServerApiClient, ESPMode::ThreadSafe> InServerApiClient)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InServerApiClient)
{}

ServerStatistic::~ServerStatistic()
{}

FString ServerStatistic::ConvertUserStatisticSortByToString(const EAccelByteStatisticSortBy& SortBy)
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

FAccelByteTaskWPtr ServerStatistic::CreateUserStatItems(FString const& UserId
	, TArray<FString> const& StatCodes
	, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/statitems/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
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
	
	TMap<FString, FString> Headers;
	Headers.Add(GHeaderABLogSquelch, TEXT("true"));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::GetAllUserStatItems(FString const& UserId
	, THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStatisticSortBy SortBy) 
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	return GetUserStatItems(UserId, {}, {}, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::GetUserStatItems(FString const& UserId
	, TArray<FString> const& StatCodes
	, TArray<FString> const& Tags
	, THandler<FAccelByteModelsUserStatItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStatisticSortBy SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/statitems")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("statCodes"), FString::Join(StatCodes, TEXT(",")) },
		{ TEXT("tags"), FString::Join(Tags, TEXT(",")) },
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("sortBy"), SortBy == EAccelByteStatisticSortBy::NONE ? TEXT("") : FGenericPlatformHttp::UrlEncode(ConvertUserStatisticSortByToString(SortBy))},
	};
	
	TMap<FString, FString> Headers;
	Headers.Add(GHeaderABLogSquelch, TEXT("true"));

	return HttpClient.ApiRequest("GET", Url, QueryParams, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::IncrementManyUsersStatItems(TArray<FAccelByteModelsBulkUserStatItemInc> const& Data
	, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/statitems/value/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

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

FAccelByteTaskWPtr ServerStatistic::IncrementUserStatItems(FString const& UserId
	, TArray<FAccelByteModelsBulkStatItemInc> const& Data
	, THandler<TArray<FAccelByteModelsBulkStatItemOperationResult>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/statitems/value/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
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
	
	TMap<FString, FString> Headers;
	Headers.Add(GHeaderABLogSquelch, TEXT("true"));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Contents, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::BulkFetchUserStatItemValues(FString const& StatCode
	, TArray<FString> const& UserIds
	, FString const& AdditionalKey
	, THandler<TArray<FAccelByteModelsFetchUser>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/statitems/value/bulk/getOrDefault")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("statCode"), StatCode },
		{ TEXT("additionalKey"), AdditionalKey },
	};

	for (const auto& UserId : UserIds)
	{
		if (!UserId.IsEmpty())
		{
			QueryParams.AddUnique(TEXT("userIds"), UserId);
		}
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::BulkFetchStatItemsValue(FString const& StatCode
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

	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/statitems/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("statCode"), StatCode },
		{ TEXT("userIds"), FString::Join(UserIds, TEXT(",")) },
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::BulkUpdateMultipleUserStatItemsValue(TArray<FAccelByteModelsUpdateUserStatItem> const& BulkUpdateMultipleUserStatItems
	, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/statitems/value/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace());

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUpdateUserStatItem>(BulkUpdateMultipleUserStatItems, Content);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::BulkResetUserStatItemsValues(FString const& UserId
	, FString const& AdditionalKey
	, TArray<FAccelByteModelsUserStatItem> const& BulkUserStatItems
	, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/users/%s/statitems/value/reset/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUserStatItem>(BulkUserStatItems, Content);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("additionalKey"), AdditionalKey },
	};

	return HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::BulkResetMultipleUserStatItemsValue(TArray<FAccelByteModelsResetUserStatItemValue> const& UserStatItemValue
	, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/statitems/value/reset/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace());
	FString Content = TEXT("");
	FAccelByteUtilities::TArrayUStructToJsonString(UserStatItemValue, Content);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::BulkUpdateUserStatItemValue(FString const& UserId
	, FString const& AdditionalKey
	, TArray<FAccelByteModelsUpdateUserStatItemWithStatCode> const& BulkUpdateUserStatItems
	, THandler<TArray<FAccelByteModelsUpdateUserStatItemsResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/users/%s/statitems/value/bulk")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId);

	FString Content = TEXT("");
	FAccelByteUtilities::UStructArrayToJsonObjectString<FAccelByteModelsUpdateUserStatItemWithStatCode>(BulkUpdateUserStatItems, Content);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("additionalKey"), AdditionalKey },
	};

	return HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::UpdateUserStatItemValue(FString const& UserId
	, FString const& StatCode
	, FString const& AdditionalKey
	, FAccelByteModelsUpdateUserStatItem const& UpdateUserStatItemValue
	, THandler<FAccelByteModelsUpdateUserStatItemValueResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/users/%s/stats/%s/statitems/value")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *StatCode);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("additionalKey"), AdditionalKey },
	};

	return HttpClient.ApiRequest(TEXT("PUT"), Url, QueryParams, UpdateUserStatItemValue, OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::DeleteUserStatItems(FString const& UserId
	, FString const& StatCode
	, FString const& AdditionalKey
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/admin/namespaces/%s/users/%s/stats/%s/statitems")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *UserId
		, *StatCode);

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("additionalKey"), AdditionalKey },
	};

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::GetGlobalStatItemsByStatCode(FString const& StatCode
	, THandler<FAccelByteModelsGlobalStatItemValueResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (StatCode.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. StatCode is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/globalstatitems/%s")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetClientNamespace()
		, *StatCode);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr ServerStatistic::ListGlobalStatItems(THandler<FAccelByteModelsGlobalStatItemPagingSlicedResult> const& OnSuccess
	, FErrorHandler const& OnError
	, TArray<FString> const& StatCodes
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));
	const FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/globalstatitems")
		, *ServerSettingsRef.StatisticServerUrl
		, *ServerCredentialsRef->GetNamespace());

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("limit"), FString::FromInt(Limit) },
		{ TEXT("offset"), FString::FromInt(Offset) },
	};
	if(StatCodes.Num() > 0)
	{
		QueryParams.Emplace(TEXT("statCodes"), FString::Join(StatCodes, TEXT(",")));
	}
	else
	{
		// Don't add statCodes into the QueryParams
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte
