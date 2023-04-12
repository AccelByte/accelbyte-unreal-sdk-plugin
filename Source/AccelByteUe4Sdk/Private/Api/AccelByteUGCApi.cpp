// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteUGCApi.h"

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

#include "Misc/Base64.h"

namespace AccelByte
{
namespace Api
{

UGC::UGC(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
{}

UGC::~UGC(){}

FString UGC::ConvertUGCSortByToString(const EAccelByteUgcSortBy& SortBy)
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
	default:
		return TEXT("");
	}
}

FString UGC::ConvertUGCOrderByToString(const EAccelByteUgcOrderBy& OrderBy)
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

void UGC::CreateContent(FString const& ChannelId
	, FAccelByteModelsUGCRequest const& CreateRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId);

	FAccelByteModelsUGCRequest Request = CreateRequest;
	if (Request.ContentType.IsEmpty())
	{
		Request.ContentType = TEXT("application/octet-stream");
	}

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

void UGC::CreateContent(FString const& ChannelId
	, FString const& Name
	, FString const& Type
	, FString const& SubType
	, TArray<FString> const& Tags
	, TArray<uint8> const& Preview
	, FString const& FileExtension
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString ContentType)
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

	CreateContent(ChannelId, Req, OnSuccess, OnError);
}

void UGC::ModifyContent(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCUpdateRequest const& ModifyRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ContentId);

	FAccelByteModelsUGCUpdateRequest Request = ModifyRequest;
	if (Request.ContentType.IsEmpty())
	{
		Request.ContentType = TEXT("application/octet-stream");
	}

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Request, OnSuccess, OnError);
}

void UGC::ModifyContent(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCRequest const& ModifyRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), TEXT("The API might be removed without notice, please use ModifyContent(.., FAccelByteModelsUGCUpdateRequest const& ModifyRequest, ..) instead!!"));

	FAccelByteModelsUGCUpdateRequest Req;
	Req.Name = ModifyRequest.Name;
	Req.Type = ModifyRequest.Type;
	Req.SubType = ModifyRequest.SubType;
	Req.Tags = ModifyRequest.Tags;
	Req.Preview = FBase64::Encode(ModifyRequest.Preview);
	Req.FileExtension = ModifyRequest.FileExtension;
	Req.ContentType = ModifyRequest.ContentType;
	Req.PreviewMetadata = ModifyRequest.PreviewMetadata;
	Req.UpdateContentFile = true;
	Req.CustomAttributes = ModifyRequest.CustomAttributes;

	ModifyContent(ChannelId, ContentId, Req, OnSuccess, OnError);
}

void UGC::ModifyContent(FString const& ChannelId
	, FString const& ContentId
	, FString const& Name
	, FString const& Type
	, FString const& SubType
	, TArray<FString> const& Tags
	, TArray<uint8> const& Preview
	, FString const& FileExtension
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString ContentType)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__), TEXT("The API might be removed without notice, please use ModifyContent(.., FAccelByteModelsUGCUpdateRequest const& ModifyRequest, ..) instead!!"));

	FAccelByteModelsUGCRequest Req;
	Req.Name = Name;
	Req.Type = Type;
	Req.SubType = SubType;
	Req.Tags = Tags;
	Req.Preview = FBase64::Encode(Preview);
	Req.FileExtension = FileExtension;
	Req.ContentType = ContentType;

	ModifyContent(ChannelId, ContentId, Req, OnSuccess, OnError);
}

void UGC::DeleteContent(FString const& ChannelId
	, FString const& ContentId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ContentId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::GetContentByContentId(FString const& ContentId
	, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(404, TEXT("Url is invalid. ContentId is empty."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ContentId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::GetContentByShareCode(FString const& ShareCode
	, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ShareCode);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::GetContentPreview(FString const& ContentId
	, THandler<FAccelByteModelsUGCPreview> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s/preview")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ContentId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::GetContentPreview(FString const& ContentId
	, THandler<TArray<uint8>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	GetContentPreview(ContentId, THandler<FAccelByteModelsUGCPreview>::CreateLambda(
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

void UGC::GetTags(THandler<FAccelByteModelsUGCTagsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/tags")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

void UGC::GetTypes(THandler<FAccelByteModelsUGCTypesPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/types")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

void UGC::CreateChannel(FString const& ChannelName
	, THandler<FAccelByteModelsUGCChannelResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	const FString Content = FString::Printf(TEXT("{\"name\": \"%s\"}"), *ChannelName);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void UGC::UpdateChannel(FString const& ChannelId
	, FString const& ChannelName
	, THandler<FAccelByteModelsUGCChannelResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace(), *CredentialsRef.GetUserId(), *ChannelId);

	const FString Content = FString::Printf(TEXT("{\"name\": \"%s\"}"), *ChannelName);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

void UGC::GetChannels(THandler<FAccelByteModelsUGCChannelsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	GetChannels(CredentialsRef.GetUserId(), OnSuccess, OnError, Limit, Offset);
}

void UGC::GetChannels(FString const& UserId
	, THandler<FAccelByteModelsUGCChannelsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

void UGC::DeleteChannel(FString const& ChannelId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::SearchContents(const FString& Name
	, const FString& Creator
	, const FString& Type
	, const FString& Subtype
	, const TArray<FString>& Tags
	, bool IsOfficial
	, const FString& UserId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteUgcSortBy SortBy
	, EAccelByteUgcOrderBy OrderBy
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("sortby"), ConvertUGCSortByToString(SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(OrderBy)  },
		{ TEXT("name"), FGenericPlatformHttp::UrlEncode(Name) },
		{ TEXT("creator"), FGenericPlatformHttp::UrlEncode(Creator) },
		{ TEXT("type"), FGenericPlatformHttp::UrlEncode(Type) },
		{ TEXT("subtype"), FGenericPlatformHttp::UrlEncode(Subtype) },
		{ TEXT("tags"), 	FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(Tags, TEXT("&Tags=")) },
		{ TEXT("isofficial"), IsOfficial ? TEXT("true") : TEXT("false")},
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")  },
		{ TEXT("userId"), UserId }
	});
	Url.Append(QueryParams);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::UpdateLikeStatusToContent(const FString& ContentId, bool bLikeStatus
	, THandler<FAccelByteModelsUGCUpdateLikeStatusToContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s/like")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ContentId);

	const FString Content = FString::Printf(TEXT("{\"likeStatus\": %s}"), bLikeStatus ? TEXT("true") : TEXT("false"));

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
	
void UGC::GetListFollowers(const FString& UserId
	, THandler<FAccelByteModelsUGCGetListFollowersPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/followers")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

void UGC::UpdateFollowStatusToUser(const FString& UserId
    , bool bFollowStatus
	, THandler<FAccelByteModelsUGCUpdateFollowStatusToUserResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/follow")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	const FString Content = FString::Printf(TEXT("{\"followStatus\": %s}"), bFollowStatus ? TEXT("true") : TEXT("false"));

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}
	
void UGC::SearchContentsSpecificToChannel(const FString& ChannelId 
	, const FString& Name
	, const FString& Creator
	, const FString& Type
	, const FString& Subtype
	, const TArray<FString>& Tags
	, bool IsOfficial
	, const FString& UserId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, EAccelByteUgcSortBy SortBy
	, EAccelByteUgcOrderBy OrderBy
	, int32 Limit
	, int32 Offset
	)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/channels/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ChannelId);

	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("sortby"), ConvertUGCSortByToString(SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(OrderBy)  },
		{ TEXT("name"), FGenericPlatformHttp::UrlEncode(Name) },
		{ TEXT("creator"), FGenericPlatformHttp::UrlEncode(Creator) },
		{ TEXT("type"), FGenericPlatformHttp::UrlEncode(Type) },
		{ TEXT("subtype"), FGenericPlatformHttp::UrlEncode(Subtype) },
		{ TEXT("tags"), 	FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(Tags, TEXT("&Tags=")) },
		{ TEXT("isofficial"), IsOfficial ? TEXT("true") : TEXT("false")},
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")  },
		{ TEXT("userId"), UserId }
	});
	Url.Append(QueryParams);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::GetContentBulk(const TArray<FString>& ContentIds
	, THandler<TArray<FAccelByteModelsUGCContentResponse>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/bulk")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	FAccelByteModelsUGCGetContentBulkRequest ContentRequest;
	ContentRequest.ContentIds = ContentIds;

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, ContentRequest, OnSuccess, OnError);
}

void UGC::GetUserContent(const FString& UserId
	, THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
	, FErrorHandler const& OnError 
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}


void UGC::UploadContentScreenshot(const FString& ContentId
	, const FString& UserId 
	, FAccelByteModelsUGCScreenshotsRequest ScreenshotsRequest
	, THandler<FAccelByteModelsUGCUpdateContentScreenshotResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/contents/%s/screenshots")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId
		, *ContentId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, ScreenshotsRequest, OnSuccess, OnError);
}
	
void UGC::GetFollowedContent(THandler<FAccelByteModelsUGCContentPageResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/followed")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}
	
void UGC::GetFollowedUsers(THandler<FAccelByteModelsUGCFollowedUsersResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/followed")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

void UGC::GetLikedContent(const TArray<FString>& Tags
	, const FString& Name
	, const FString& Type
	, const FString& Subtype
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
		, *CredentialsRef.GetNamespace());

	FString QueryParams = FAccelByteUtilities::CreateQueryParams({
		{ TEXT("tags"), 	FAccelByteUtilities::CreateQueryParamValueUrlEncodedFromArray(Tags, TEXT("&Tags=")) },
		{ TEXT("name"), FGenericPlatformHttp::UrlEncode(Name) },
		{ TEXT("type"), FGenericPlatformHttp::UrlEncode(Type) },
		{ TEXT("subtype"), FGenericPlatformHttp::UrlEncode(Subtype) },
		{ TEXT("isofficial"), IsOfficial ? TEXT("true") : TEXT("false")},
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Limit >= 0 ? FString::FromInt(Offset) : TEXT("")  },
		{ TEXT("sortby"), ConvertUGCSortByToString(SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(OrderBy)  },
		});
	Url.Append(QueryParams);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::GetCreator(const FString& UserId
	, THandler<FAccelByteModelsUGCGetListFollowersResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}
	
void UGC::GetGroups(const FString& UserId
	, THandler<FAccelByteModelsUGCGetGroupsResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/groups")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMap<FString, FString> QueryParams;
	if (Offset > 0)
	{
		QueryParams.Add(TEXT("offset"), FString::FromInt(Offset));
	}
	if (Limit > 0)
	{
		QueryParams.Add(TEXT("limit"), FString::FromInt(Limit));
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, OnSuccess, OnError);
}

}
}
