// Copyright (c) 2023 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerUGCApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteUtilities.h"
#include "Core/AccelByteServerSettings.h"

namespace AccelByte
{
namespace GameServerApi
{

ServerUGC::ServerUGC(ServerCredentials const& InCredentialsRef
	, ServerSettings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FServerApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

ServerUGC::~ServerUGC()
{}

#pragma region Utilities

FString ServerUGC::ConvertUGCSortByToString(const EAccelByteUgcSortBy& SortBy)
{
	switch (SortBy)
	{
	case EAccelByteUgcSortBy::NAME:
		return TEXT("name");
	case EAccelByteUgcSortBy::DATE:
		return TEXT("date");
	case EAccelByteUgcSortBy::DOWNLOAD:
		return TEXT("download");
	case EAccelByteUgcSortBy::LIKE:
		return TEXT("like");
	case EAccelByteUgcSortBy::UPDATED_TIME:
		return TEXT("updatedTime");
	default:
		return TEXT("");
	}
}

FString ServerUGC::ConvertUGCOrderByToString(const EAccelByteUgcOrderBy& OrderBy)
{
	switch (OrderBy)
	{
	case EAccelByteUgcOrderBy::ASC:
		return TEXT("asc");
	case EAccelByteUgcOrderBy::DESC:
		return TEXT("desc");
	default:
		return TEXT("");
	}
}

#pragma endregion

void ServerUGC::SearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/contents/search")
		, *ServerSettingsRef.UGCServerUrl
		, *ServerCredentialsRef.GetNamespace());

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("sortby"), ConvertUGCSortByToString(Request.SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(Request.OrderBy) },
		{ TEXT("name"), Request.Name },
		{ TEXT("creator"), Request.Creator },
		{ TEXT("type"), Request.Type },
		{ TEXT("subtype"), Request.Subtype },
		{ TEXT("isofficial"), Request.bIsOfficial ? TEXT("true") : TEXT("false") },
		{ TEXT("ishidden"), Request.bIsHidden ? TEXT("true") : TEXT("false") },
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("userId"), Request.UserId }
	};

	for (const auto& Tag : Request.Tags)
	{
		QueryParams.AddUnique(TEXT("tags"), Tag);
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerUGC::SearchContentsSpecificToChannel(FString const& ChannelId
	, FAccelByteModelsUGCSearchContentsRequest const& Request
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}

	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/channels/%s/contents/search")
		, *ServerSettingsRef.UGCServerUrl
		, *ServerCredentialsRef.GetNamespace()
		, *ChannelId);

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("sortby"), ConvertUGCSortByToString(Request.SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(Request.OrderBy) },
		{ TEXT("name"), Request.Name },
		{ TEXT("creator"), Request.Creator },
		{ TEXT("type"), Request.Type },
		{ TEXT("subtype"), Request.Subtype },
		{ TEXT("isofficial"), Request.bIsOfficial ? TEXT("true") : TEXT("false") },
		{ TEXT("ishidden"), Request.bIsHidden ? TEXT("true") : TEXT("false") },
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("userId"), Request.UserId }
	};

	for (const auto& Tag : Request.Tags)
	{
		QueryParams.AddUnique(TEXT("tags"), Tag);
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void ServerUGC::ModifyContentByShareCode(FString const& UserId
	, FString const& ChannelId
	, FString const& ShareCode
	, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}

	if (ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return;
	}
	
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}

	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/channels/%s/contents/s3/sharecodes/%s")
		, *ServerSettingsRef.UGCServerUrl
		, *ServerCredentialsRef.GetNamespace()
		, *UserId
		, *ChannelId
		, *ShareCode);

	FAccelByteModelsUGCUpdateRequest Request = ModifyRequest;
	if (Request.ContentType.IsEmpty())
	{
		Request.ContentType = TEXT("application/octet-stream");
	}

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Request, OnSuccess, OnError);
}

void ServerUGC::DeleteContentByShareCode(FString const& UserId
	, FString const& ChannelId
	, FString const& ShareCode
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}

	if (ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return;
	}
	
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}

	FString Url = FString::Printf(TEXT("%s/v1/admin/namespaces/%s/users/%s/channels/%s/contents/sharecodes/%s")
		, *ServerSettingsRef.UGCServerUrl
		, *ServerCredentialsRef.GetNamespace()
		, *UserId
		, *ChannelId
		, *ShareCode);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

} // Namespace GameServerApi
} // Namespace AccelByte
