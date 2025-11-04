// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteUGCApi.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteApiClient.h"
#include "Core/AccelByteInstance.h"
#include "Core/AccelByteUtilities.h"
#include "Misc/Base64.h"

namespace AccelByte
{
namespace Api
{

UGC::UGC(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, TSharedPtr<AccelByte::FApiClient, ESPMode::ThreadSafe> const& InApiClient)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InApiClient)
{
}

UGC::UGC(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetrySchedulerBase& InHttpRef
	, FAccelBytePlatformPtr const& InAccelBytePlatform)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef, InAccelBytePlatform)
{
}

UGC::~UGC()
{
}

FAccelByteTaskWPtr UGC::CreateContent(FString const& ChannelId
	, FAccelByteModelsUGCRequest const& CreateRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId));

	FAccelByteModelsUGCRequest Request = CreateRequest;
	if (Request.ContentType.IsEmpty())
	{
		Request.ContentType = TEXT("application/octet-stream");
	}

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::CreateContent(FString const& ChannelId
	, FString const& Name
	, FString const& Type
	, FString const& SubType
	, TArray<FString> const& Tags
	, TArray<uint8> const& Preview
	, FString const& FileExtension
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& ContentType)
{
	FReport::Log(FString(__FUNCTION__));
	
	FAccelByteModelsUGCRequest Req;
	Req.Name = Name;
	Req.Type = Type;
	Req.SubType = SubType;
	Req.Tags = Tags;
	Req.Preview = FBase64::Encode(Preview);
	Req.FileExtension = FileExtension;
	Req.ContentType = ContentType;

	return CreateContent(ChannelId, Req, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::ModifyContent(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	FAccelByteModelsUGCUpdateRequest Request = ModifyRequest;
	if (Request.ContentType.IsEmpty())
	{
		Request.ContentType = TEXT("application/octet-stream");
	}

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::ModifyContent(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCRequest const& ModifyRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bUpdateContent)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), TEXT("The API might be removed without notice, please use ModifyContent(.., FAccelByteModelsUGCUpdateRequest const& ModifyRequest, ..) instead!!"));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	FAccelByteModelsUGCUpdateRequest Req;
	Req.Name = ModifyRequest.Name;
	Req.Type = ModifyRequest.Type;
	Req.SubType = ModifyRequest.SubType;
	Req.Tags = ModifyRequest.Tags;
	Req.Preview = FBase64::Encode(ModifyRequest.Preview);
	Req.FileExtension = ModifyRequest.FileExtension;
	Req.ContentType = ModifyRequest.ContentType;
	Req.PreviewMetadata = ModifyRequest.PreviewMetadata;
	Req.UpdateContentFile = bUpdateContent;
	Req.CustomAttributes = ModifyRequest.CustomAttributes;

	return ModifyContent(ChannelId, ContentId, Req, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::ModifyContent(FString const& ChannelId
	, FString const& ContentId
	, FString const& Name
	, FString const& Type
	, FString const& SubType
	, TArray<FString> const& Tags
	, TArray<uint8> const& Preview
	, FString const& FileExtension
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& ContentType
	, bool bUpdateContent)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), TEXT("The API might be removed without notice, please use ModifyContent(.., FAccelByteModelsUGCUpdateRequest const& ModifyRequest, ..) instead!!"));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	FAccelByteModelsUGCRequest Req;
	Req.Name = Name;
	Req.Type = Type;
	Req.SubType = SubType;
	Req.Tags = Tags;
	Req.Preview = FBase64::Encode(Preview);
	Req.FileExtension = FileExtension;
	Req.ContentType = ContentType;

	return ModifyContent(ChannelId, ContentId, Req, OnSuccess, OnError, bUpdateContent);
}

FAccelByteTaskWPtr UGC::DeleteContent(FString const& ChannelId
	, FString const& ContentId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetContentByContentId(FString const& ContentId
	, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicGetContentByContentId(FString const& ContentId
	, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.Request(TEXT("GET"), Url, {}, FString(), GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetContentByShareCode(FString const& ShareCode
	, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ShareCode));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicGetContentByShareCode(FString const& ShareCode
	, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ShareCode));

	return HttpClient.Request(TEXT("GET"), Url, {}, FString(), GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetContentPreview(FString const& ContentId
	, THandler<FAccelByteModelsUGCPreview> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s/preview")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetContentPreview(FString const& ContentId
	, THandler<TArray<uint8>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	return GetContentPreview(ContentId, THandler<FAccelByteModelsUGCPreview>::CreateLambda(
			[OnSuccess](FAccelByteModelsUGCPreview const& Response)
			{
				TArray<uint8> Bytes;
				if(FBase64::Decode(Response.Preview, Bytes))
					OnSuccess.ExecuteIfBound(Bytes);
				else
					UE_LOG(LogAccelByte, Warning, TEXT("Cannot decode preview string : %s"), *Response.Preview);
			})
		, OnError);
}

FAccelByteTaskWPtr UGC::GetTags(THandler<FAccelByteModelsUGCTagsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/tags")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	TMultiMap<FString, FString> QueryParams;
	
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetTypes(THandler<FAccelByteModelsUGCTypesPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/types")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	TMultiMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::CreateChannel(FString const& ChannelName
	, THandler<FAccelByteModelsUGCChannelResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId()));

	const FString Content = FString::Printf(TEXT("{\"name\": \"%s\"}"), *ChannelName);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::UpdateChannel(FString const& ChannelId
	, FString const& ChannelName
	, THandler<FAccelByteModelsUGCChannelResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId));

	const FString Content = FString::Printf(TEXT("{\"name\": \"%s\"}"), *ChannelName);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetChannels(THandler<FAccelByteModelsUGCChannelsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, FString const& ChannelName)
{
	FReport::Log(FString(__FUNCTION__));

	return GetChannels(CredentialsRef->GetUserId(), OnSuccess, OnError, Limit, Offset, ChannelName);
}

FAccelByteTaskWPtr UGC::GetChannels(FString const& UserId
	, THandler<FAccelByteModelsUGCChannelsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, FString const& ChannelName)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
		, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	TMultiMap<FString, FString> QueryParams{
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("name"), ChannelName.IsEmpty() ? TEXT("") : ChannelName }
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::DeleteChannel(FString const& ChannelId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::InternalSearchContents(FString const& Name
	, FString const& Creator
	, FString const& Type
	, FString const& Subtype
	, TArray<FString> const& Tags
	, bool IsOfficial
	, FString const& UserId
	, EAccelByteUgcSortBy SortBy
	, EAccelByteUgcOrderBy OrderBy
	, int32 Limit
	, int32 Offset
	, FString& OutUrl
	, TMultiMap<FString, FString>& OutQueryParams)
{
	OutUrl = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace()));

	OutQueryParams = TMultiMap<FString, FString>{
		{ TEXT("sortby"), FAccelByteUGCUtilities::ConvertUGCSortByToString(SortBy) },
		{ TEXT("orderby"), FAccelByteUGCUtilities::ConvertUGCOrderByToString(OrderBy) },
		{ TEXT("name"), Name },
		{ TEXT("creator"), Creator },
		{ TEXT("type"), Type },
		{ TEXT("subtype"), Subtype },
		{ TEXT("isofficial"), IsOfficial ? TEXT("true") : TEXT("false") },
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("userId"), UserId }
	};

	for (const auto& Tag : Tags)
	{
		OutQueryParams.AddUnique(TEXT("tags"), Tag);
	}
}

FAccelByteTaskWPtr UGC::SearchContents(FString const& Name
	, FString const& Creator
	, FString const& Type
	, FString const& Subtype
	, TArray<FString> const& Tags
	, bool IsOfficial
	, FString const& UserId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteUgcSortBy SortBy
	, EAccelByteUgcOrderBy OrderBy
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url;
	TMultiMap<FString, FString> QueryParams;
	InternalSearchContents(Name, Creator, Type, Subtype, Tags, IsOfficial, UserId, SortBy, OrderBy, Limit, Offset, Url, QueryParams);

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::SearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url;
	TMultiMap<FString, FString> QueryParams;
	InternalSearchContents(Request.Name, Request.Creator, Request.Type, Request.Subtype, Request.Tags, Request.bIsOfficial, Request.UserId, Request.SortBy, Request.OrderBy, Limit, Offset, Url, QueryParams);

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicSearchContents(FString const& Name
	, FString const& Creator
	, FString const& Type
	, FString const& Subtype
	, TArray<FString> const& Tags
	, bool IsOfficial
	, FString const& UserId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteUgcSortBy SortBy
	, EAccelByteUgcOrderBy OrderBy
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url;
	TMultiMap<FString, FString> QueryParams;
	InternalSearchContents(Name, Creator, Type, Subtype, Tags, IsOfficial, UserId, SortBy, OrderBy, Limit, Offset, Url, QueryParams);

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicSearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url;
	TMultiMap<FString, FString> QueryParams;
	InternalSearchContents(Request.Name, Request.Creator, Request.Type, Request.Subtype, Request.Tags, Request.bIsOfficial, Request.UserId, Request.SortBy, Request.OrderBy, Limit, Offset, Url, QueryParams);

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::UpdateLikeStatusToContent(FString const& ContentId
	, bool bLikeStatus
	, THandler<FAccelByteModelsUGCUpdateLikeStatusToContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s/like")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	const FString Content = FString::Printf(TEXT("{\"likeStatus\": %s}"), bLikeStatus ? TEXT("true") : TEXT("false"));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr UGC::GetListFollowers(FString const& UserId
	, THandler<FAccelByteModelsUGCGetListFollowersPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
		, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/followers")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	TMultiMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::UpdateFollowStatusToUser(FString const& UserId
    , bool bFollowStatus
	, THandler<FAccelByteModelsUGCUpdateFollowStatusToUserResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
		, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/follow")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	const FString Content = FString::Printf(TEXT("{\"followStatus\": %s}"), bFollowStatus ? TEXT("true") : TEXT("false"));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr UGC::SearchContentsSpecificToChannel(FString const& ChannelId
	, FString const& Name
	, FString const& Creator
	, FString const& Type
	, FString const& Subtype
	, TArray<FString> const& Tags
	, bool IsOfficial
	, FString const& UserId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteUgcSortBy SortBy
	, EAccelByteUgcOrderBy OrderBy
	, int32 Limit
	, int32 Offset
)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError))
	{
		return nullptr;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/channels/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ChannelId));

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("sortby"), FAccelByteUGCUtilities::ConvertUGCSortByToString(SortBy) },
		{ TEXT("orderby"), FAccelByteUGCUtilities::ConvertUGCOrderByToString(OrderBy) },
		{ TEXT("name"), Name },
		{ TEXT("creator"), Creator },
		{ TEXT("type"), Type },
		{ TEXT("subtype"), Subtype },
		{ TEXT("isofficial"), IsOfficial ? TEXT("true") : TEXT("false") },
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("userId"), UserId }
	};

	for (const auto& Tag : Tags)
	{
		QueryParams.AddUnique(TEXT("tags"), Tag);
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr UGC::SearchContentsSpecificToChannel(FString const& ChannelId
	, FAccelByteModelsUGCSearchContentsRequest const& Request
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
		, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return nullptr;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/channels/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ChannelId));

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("sortby"), FAccelByteUGCUtilities::ConvertUGCSortByToString(Request.SortBy) },
		{ TEXT("orderby"), FAccelByteUGCUtilities::ConvertUGCOrderByToString(Request.OrderBy)  },
		{ TEXT("name"), Request.Name },
		{ TEXT("creator"), Request.Creator },
		{ TEXT("type"), Request.Type },
		{ TEXT("subtype"), Request.Subtype },
		{ TEXT("isofficial"), Request.bIsOfficial ? TEXT("true") : TEXT("false")},
		{ TEXT("ishidden"), Request.bIsHidden ? TEXT("true") : TEXT("false")},
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")  },
		{ TEXT("userId"), Request.UserId }
	};

	for (const auto& Tag : Request.Tags)
	{
		QueryParams.AddUnique(TEXT("tags"), Tag);
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetContentBulk(TArray<FString> const& ContentIds
	, THandler<TArray<FAccelByteModelsUGCContentResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/bulk")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	FAccelByteModelsUGCGetContentBulkRequest ContentRequest;
	ContentRequest.ContentIds = ContentIds;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ContentRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicGetContentBulk(TArray<FString> const& ContentIds
	, THandler<TArray<FAccelByteModelsUGCContentResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/bulk")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace()));

	FAccelByteModelsUGCGetContentBulkRequest ContentRequest;
	ContentRequest.ContentIds = ContentIds;

	FString ContentString;
	FJsonObjectConverter::UStructToJsonObjectString(ContentRequest, ContentString);

	return HttpClient.Request(TEXT("POST"), Url, ContentString, GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetUserContent(FString const& UserId
	, THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
	, FErrorHandler const& OnError 
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	TMultiMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicGetUserContent(FString const& UserId
	, THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	TMultiMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::UploadContentScreenshot(FString const& ContentId
	, FString const& UserId 
	, FAccelByteModelsUGCScreenshotsRequest ScreenshotsRequest
	, THandler<FAccelByteModelsUGCUpdateContentScreenshotResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/contents/%s/screenshots")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId)
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ScreenshotsRequest, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr UGC::GetFollowedContent(THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/followed")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	TMultiMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr UGC::GetFollowedUsers(THandler<FAccelByteModelsUGCFollowedUsersResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/followed")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	TMultiMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetLikedContent(TArray<FString> const& Tags
	, FString const& Name
	, FString const& Type
	, FString const& Subtype
	, bool IsOfficial 
	, THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUgcSortBy SortBy
	, EAccelByteUgcOrderBy OrderBy)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/liked")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("name"), Name },
		{ TEXT("type"), Type },
		{ TEXT("subtype"), Subtype },
		{ TEXT("isofficial"), IsOfficial ? TEXT("true") : TEXT("false")},
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Limit >= 0 ? FString::FromInt(Offset) : TEXT("")  },
		{ TEXT("sortby"), FAccelByteUGCUtilities::ConvertUGCSortByToString(SortBy) },
		{ TEXT("orderby"), FAccelByteUGCUtilities::ConvertUGCOrderByToString(OrderBy)  },
	};

	for (const auto& Tag : Tags)
	{
		QueryParams.AddUnique(TEXT("tags"), Tag);
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetLikedContent(TArray<FString> const& Tags
	, FString const& Name
	, FString const& Type
	, FString const& Subtype
	, bool IsOfficial
	, THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteLikedContentSortBy SortBy
	, EAccelByteUgcOrderBy OrderBy)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/liked")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("name"), Name },
		{ TEXT("type"), Type },
		{ TEXT("subtype"), Subtype },
		{ TEXT("isofficial"), IsOfficial ? TEXT("true") : TEXT("false") },
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Limit >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("sortby"), FAccelByteUGCUtilities::ConvertLikedContentSortByToString(SortBy) },
		{ TEXT("orderby"), FAccelByteUGCUtilities::ConvertUGCOrderByToString(OrderBy) },
	};

	for (const auto& Tag : Tags)
	{
		QueryParams.AddUnique(TEXT("tags"), Tag);
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetCreator(FString const& UserId
	, THandler<FAccelByteModelsUGCGetListFollowersResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr UGC::GetGroups(FString const& UserId
	, THandler<FAccelByteModelsUGCGetGroupsResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/groups")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	TMultiMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::BulkGetContentByShareCode(TArray<FString> const& ShareCodes
		, THandler<TArray<FAccelByteModelsUGCContentResponse>> const& OnSuccess
		, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ShareCodes.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCodes is empty."));
		return nullptr;
	}
	if (ShareCodes.Num() > MAX_BULK_CONTENT_SHARECODES_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_CONTENT_SHARECODES_COUNT));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/sharecodes/bulk")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	FAccelByteModelsUGCBulkGetContentByShareCodeRequest ContentRequest;
	ContentRequest.ShareCodes = ShareCodes;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ContentRequest, OnSuccess, OnError);
}

#pragma region UGC V2 (Content)

FAccelByteTaskWPtr UGC::SearchContentsSpecificToChannelV2(FString const& ChannelId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
		, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/channels/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ChannelId));

	const TMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : FAccelByteUGCUtilities::ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicSearchContentsSpecificToChannelV2(FString const& ChannelId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/channels/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ChannelId));

	const TMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : FAccelByteUGCUtilities::ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, GetDefaultHeaders(), OnSuccess, OnError);
}

void UGC::InternalSearchContentsV2(FAccelByteModelsUGCFilterRequestV2 const& Filter
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentSortByV2 SortBy
	, FString& OutUrl
	, TMap<FString, FString>& OutQueryParams)
{
	OutUrl = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace()));

	FString TagValue = FString::Join(Filter.Tags, TEXT(","));

	OutQueryParams = TMap<FString, FString>{
		{TEXT("name"), Filter.Name.IsEmpty() ? TEXT("") : Filter.Name},
		{TEXT("type"), Filter.Type.IsEmpty() ? TEXT("") : Filter.Type},
		{TEXT("subType"), Filter.SubType.IsEmpty() ? TEXT("") : Filter.SubType},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), FAccelByteUGCUtilities::ConvertGetUGContentsSortByToString(SortBy)},
		{TEXT("tags"), TagValue.IsEmpty() ? TEXT("") : TagValue}
	};
}

const FString UGC::GetNamespace()
{
	if (CredentialsRef->IsSessionValid())
	{
		return CredentialsRef->GetNamespace();
	}
	return SettingsRef.Namespace;
}

const TMap<FString, FString> UGC::GetDefaultHeaders()
{
	auto Headers = CredentialsRef->GetAuthHeader();
	Headers.Add(TEXT("Content-Type"), TEXT("application/json"));

	return Headers;
}

FAccelByteTaskWPtr UGC::SearchContentsV2(FAccelByteModelsUGCFilterRequestV2 const& Filter
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url;
	TMap<FString, FString> QueryParams;
	InternalSearchContentsV2(Filter, Limit, Offset, SortBy, Url, QueryParams);

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicSearchContentsV2(FAccelByteModelsUGCFilterRequestV2 const& Filter
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url;
	TMap<FString, FString> QueryParams;
	InternalSearchContentsV2(Filter, Limit, Offset, SortBy, Url, QueryParams);

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetContentBulkByIdsV2(TArray<FString> const& ContentIds
	, THandler<TArray<FAccelByteModelsUGCContentResponseV2>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentIds.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentIds is empty."));
		return nullptr;
	}
	if (ContentIds.Num() > MAX_BULK_CONTENT_IDS_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_CONTENT_IDS_COUNT));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/bulk")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	FAccelByteModelsUGCGetContentBulkRequest ContentRequest;
	ContentRequest.ContentIds = ContentIds;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ContentRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicGetContentBulkByIdsV2(TArray<FString> const& ContentIds
	, THandler<TArray<FAccelByteModelsUGCContentResponseV2>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentIds.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentIds is empty."));
		return nullptr;
	}
	if (ContentIds.Num() > MAX_BULK_CONTENT_IDS_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_CONTENT_IDS_COUNT));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/bulk")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace()));

	FAccelByteModelsUGCGetContentBulkRequest ContentRequest;
	ContentRequest.ContentIds = ContentIds;

	FString ContentString;
	FJsonObjectConverter::UStructToJsonObjectString(ContentRequest, ContentString);

	return HttpClient.Request(TEXT("POST"), Url, ContentString, GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetContentByShareCodeV2(FString const& ShareCode
	, THandler<FAccelByteModelsUGCContentResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ShareCode));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicGetContentByShareCodeV2(FString const& ShareCode
	, THandler<FAccelByteModelsUGCContentResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ShareCode));

	return HttpClient.Request(TEXT("GET"), Url, {}, FString(), GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetContentByContentIdV2(FString const& ContentId
	, THandler<FAccelByteModelsUGCContentResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicGetContentByContentIdV2(FString const& ContentId
	, THandler<FAccelByteModelsUGCContentResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.Request(TEXT("GET"), Url, {}, FString(), GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::CreateContentV2(FString const& ChannelId
	, FAccelByteModelsCreateUGCRequestV2 const& CreateRequest
	, THandler<FAccelByteModelsUGCCreateUGCResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return nullptr;
	}
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, CreateRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::DeleteContentV2(FString const& ChannelId
	, FString const& ContentId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::ModifyContentV2(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsModifyUGCRequestV2 const& ModifyRequest
	, THandler<FAccelByteModelsUGCModifyUGCResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, ModifyRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GenerateUploadContentURLV2(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUploadContentURLRequestV2 const& UploadRequest
	, THandler<FAccelByteModelsUGCUploadContentURLResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}
	if (UploadRequest.FileExtension.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, FileExtension is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s/uploadUrl")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, UploadRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::UpdateContentFileLocationV2(FString const& ChannelId
	, FString const& ContentId
	, FString const& FileExtension
	, FString const& S3Key
	, THandler<FAccelByteModelsUGCUpdateContentFileLocationResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
		, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	if (FileExtension.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, FileExtension is empty."));
		return nullptr;
	}
	if (S3Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, S3Key is empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s/fileLocation")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	FAccelByteModelsUGCUpdateContentFileLocationRequestV2 Request;
	Request.FileExtension = FileExtension;
	Request.FileLocation = S3Key;

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetUserContentsV2(FString const& UserId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	const TMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : FAccelByteUGCUtilities::ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::PublicGetUserContentsV2(FString const& UserId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(UserId));

	const TMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : FAccelByteUGCUtilities::ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, FString(), GetDefaultHeaders(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::UpdateContentScreenshotV2(FString const& ContentId
	, FAccelByteModelsUGCUpdateScreenshotsV2 const& ScreenshotsRequest
	, THandler<FAccelByteModelsUGCUpdateScreenshotsV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/contents/%s/screenshots")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, ScreenshotsRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::UploadContentScreenshotV2(FString const& ContentId
	, FAccelByteModelsUGCUploadScreenshotsRequestV2 const& ScreenshotsRequest
	, THandler<FAccelByteModelsUGCUpdateContentScreenshotResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	if (ScreenshotsRequest.Screenshots.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Screenshots is empty."));
		return nullptr;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/contents/%s/screenshots")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ScreenshotsRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::DeleteContentScreenshotV2(FString const& ContentId
	, FString const& ScreenshotId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ScreenshotId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetScreenshotIdInvalidMessage(ScreenshotId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/contents/%s/screenshots/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ContentId)
		, *FGenericPlatformHttp::UrlEncode(ScreenshotId));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::ModifyContentByShareCode(FString const& ChannelId
	, FString const& ShareCode
	, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
		, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return nullptr;
	}

	if (ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return nullptr;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ShareCode));

	FAccelByteModelsUGCUpdateRequest Request = ModifyRequest;
	if (Request.ContentType.IsEmpty())
	{
		Request.ContentType = TEXT("application/octet-stream");
	}

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::ModifyContentShareCode(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCModifyContentShareCodeRequest const& ModifyContentShareCodeRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	if (ModifyContentShareCodeRequest.ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return nullptr;
	}

	if (ModifyContentShareCodeRequest.ShareCode.Len() > 7)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode length exceeds the maximum of 7 characters."));
		return nullptr;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/%s/sharecode")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, ModifyContentShareCodeRequest, OnSuccess, OnError);
}	

FAccelByteTaskWPtr UGC::DeleteContentByShareCode(FString const& ChannelId
	, FString const& ShareCode
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError))
	{
		return nullptr;
	}

	if (ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return nullptr;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ShareCode));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::BulkGetContentByShareCodeV2(TArray<FString> const& ShareCodes
		, THandler<TArray<FAccelByteModelsUGCContentResponseV2>> const& OnSuccess
		, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ShareCodes.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCodes is empty."));
		return nullptr;
	}
	if (ShareCodes.Num() > MAX_BULK_CONTENT_SHARECODES_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_CONTENT_SHARECODES_COUNT));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/sharecodes/bulk")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace()));

	FAccelByteModelsUGCBulkGetContentByShareCodeRequest ContentRequest;
	ContentRequest.ShareCodes = ShareCodes;

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, ContentRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::ModifyContentShareCodeV2(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCModifyContentShareCodeRequest const& ModifyContentShareCodeRequest
	, THandler<FAccelByteModelsUGCCreateUGCResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ChannelId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
			, OnError) ||
		!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	if (ModifyContentShareCodeRequest.ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return nullptr;
	}

	if (ModifyContentShareCodeRequest.ShareCode.Len() > 7)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode length exceeds the maximum of 7 characters."));
		return nullptr;
	}

	FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s/sharecode")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ChannelId)
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, ModifyContentShareCodeRequest, OnSuccess, OnError);
}	

#pragma endregion UGC V2 (Content)

#pragma region UGC V2 (Download Count)

FAccelByteTaskWPtr UGC::AddDownloadContentCountV2(FString const& ContentId
	, THandler<FAccelByteModelsUGCAddDownloadContentCountResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s/downloadcount")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetListContentDownloaderV2(FString const& ContentId
	, THandler<FAccelByteModelsUGCGetPaginatedContentDownloaderResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& UserId
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s/downloader")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	const TMap<FString, FString> QueryParams = {
		{TEXT("userId"), UserId.IsEmpty() ? TEXT("") : *FGenericPlatformHttp::UrlEncode(UserId)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : FAccelByteUGCUtilities::ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

#pragma endregion UGC V2 (Download Count)

#pragma region UGC V2 (Like)

FAccelByteTaskWPtr UGC::GetListContentLikerV2(FString const& ContentId
	, THandler<FAccelByteModelsUGCGetPaginatedContentLikerResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s/like")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	const TMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : FAccelByteUGCUtilities::ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::UpdateLikeStatusToContentV2(FString const& ContentId
	, bool bLikeStatus
	, THandler<FAccelByteModelsUGCUpdateLikeStatusToContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s/like")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	const FString Content = FString::Printf(TEXT("{\"likeStatus\": %s}"), bLikeStatus ? TEXT("true") : TEXT("false"));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

#pragma endregion UGC V2 (Like)

#pragma region UGC Staging Content

FAccelByteTaskWPtr UGC::GetStagingContents(EStagingContentRequestStatus Status
	, THandler<FAccelByteModelsUGCPaginatedListStagingContentResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteStagingContentUtilitiesSortBy SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/staging-contents")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId()));

	const TMap<FString, FString> QueryParams = {
		{TEXT("status"), FAccelByteUtilities::GetUEnumValueAsString(Status)},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteStagingContentUtilitiesSortBy::NONE ? TEXT("") : FAccelByteUGCUtilities::ConvertStagingContentSortByToString(SortBy)}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::GetStagingContentById(FString const& ContentId
	, THandler<FAccelByteModelsUGCStagingContentResponse> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/staging-contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("GET"), Url, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::UpdateStagingContent(FString const& ContentId
	, FAccelByteModelsUGCUpdateContentFileLocationRequestV2 UpdateRequest
	, THandler<FAccelByteModelsUGCStagingContentResponse> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/staging-contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, UpdateRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr UGC::DeleteStagingContent(FString const& ContentId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(ContentId
			, EAccelByteIdHypensRule::NO_HYPENS
			, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
			, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/staging-contents/%s")
		, *SettingsRef.UGCServerUrl
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetNamespace())
		, *FGenericPlatformHttp::UrlEncode(CredentialsRef->GetUserId())
		, *FGenericPlatformHttp::UrlEncode(ContentId));

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

#pragma endregion UGC Staging Content

} // Namespace Api
} // Namespace AccelByte
