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

#pragma region Utils

FString UGC::ConvertLikedContentSortByToString(const EAccelByteLikedContentSortBy& SortBy)
{
	switch (SortBy)
	{
	case EAccelByteLikedContentSortBy::NAME:
		return TEXT("name");
	case EAccelByteLikedContentSortBy::DATE:
		return TEXT("date");
	case EAccelByteLikedContentSortBy::DOWNLOAD:
		return TEXT("download");
	case EAccelByteLikedContentSortBy::LIKE:
		return TEXT("like");
	default:
		return TEXT("");
	}
}

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
	case EAccelByteUgcSortBy::UPDATED_TIME:
		return TEXT("updatedTime");
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

FString UGC::ConvertGetUGContentsSortByToString(const EAccelByteUGCContentSortByV2& SortBy)
{
	switch (SortBy)
	{
	case EAccelByteUGCContentSortByV2::NAME:
		return TEXT("name");
	case EAccelByteUGCContentSortByV2::NAME_ASC:
		return TEXT("name:asc");
	case EAccelByteUGCContentSortByV2::NAME_DESC:
		return TEXT("name:desc");
	case EAccelByteUGCContentSortByV2::DOWNLOAD:
		return TEXT("download");
	case EAccelByteUGCContentSortByV2::DOWNLOAD_ASC:
		return TEXT("download:asc");
	case EAccelByteUGCContentSortByV2::DOWNLOAD_DESC:
		return TEXT("download:desc");
	case EAccelByteUGCContentSortByV2::LIKE:
		return TEXT("like");
	case EAccelByteUGCContentSortByV2::LIKE_ASC:
		return TEXT("like:asc");
	case EAccelByteUGCContentSortByV2::LIKE_DESC:
		return TEXT("like:desc");
	case EAccelByteUGCContentSortByV2::CREATED_TIME:
		return TEXT("createdTime");
	case EAccelByteUGCContentSortByV2::CREATED_TIME_ASC:
		return TEXT("createdTime:asc");
	case EAccelByteUGCContentSortByV2::CREATED_TIME_DESC:
		return TEXT("createdTime:desc");
	default:
		return TEXT("");
	}
}

FString UGC::ConvertUGCUtilitiesSortByToString(const EAccelByteUGCContentUtilitiesSortByV2& SortBy)
{
	switch (SortBy)
	{
	case EAccelByteUGCContentUtilitiesSortByV2::CREATED_TIME:
		return TEXT("createdTime");
	case EAccelByteUGCContentUtilitiesSortByV2::CREATED_TIME_ASC:
		return TEXT("createdTime:asc");
	case EAccelByteUGCContentUtilitiesSortByV2::CREATED_TIME_DESC:
		return TEXT("createdTime:desc");
	default:
		return TEXT("");
	}
}

#pragma endregion Utils

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
	, FErrorHandler const& OnError
	, bool bUpdateContent)
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
	Req.UpdateContentFile = bUpdateContent;
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
	, FString ContentType
	, bool bUpdateContent)
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

	ModifyContent(ChannelId, ContentId, Req, OnSuccess, OnError, bUpdateContent);
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

	TMultiMap<FString, FString> QueryParams;
	
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

	TMultiMap<FString, FString> QueryParams;
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
	, int32 Offset
	, FString const& ChannelName)
{
	FReport::Log(FString(__FUNCTION__));

	GetChannels(CredentialsRef.GetUserId(), OnSuccess, OnError, Limit, Offset, ChannelName);
}

void UGC::GetChannels(FString const& UserId
	, THandler<FAccelByteModelsUGCChannelsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, FString const& ChannelName)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMultiMap<FString, FString> QueryParams{
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("name"), ChannelName.IsEmpty() ? TEXT("") : ChannelName }
	};

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

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("sortby"), ConvertUGCSortByToString(SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(OrderBy) },
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

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void UGC::SearchContents(FAccelByteModelsUGCSearchContentsRequest const& Request
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("sortby"), ConvertUGCSortByToString(Request.SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(Request.OrderBy)  },
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

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
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

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/followers")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMultiMap<FString, FString> QueryParams;
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

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

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

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("sortby"), ConvertUGCSortByToString(SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(OrderBy) },
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

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}
	
void UGC::SearchContentsSpecificToChannel(FString const& ChannelId
	, FAccelByteModelsUGCSearchContentsRequest const& Request
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/channels/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ChannelId);

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("sortby"), ConvertUGCSortByToString(Request.SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(Request.OrderBy)  },
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

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
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

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMultiMap<FString, FString> QueryParams;
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

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

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

	TMultiMap<FString, FString> QueryParams;
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

	TMultiMap<FString, FString> QueryParams;
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

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("name"), Name },
		{ TEXT("type"), Type },
		{ TEXT("subtype"), Subtype },
		{ TEXT("isofficial"), IsOfficial ? TEXT("true") : TEXT("false")},
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Limit >= 0 ? FString::FromInt(Offset) : TEXT("")  },
		{ TEXT("sortby"), ConvertUGCSortByToString(SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(OrderBy)  },
	};

	for (const auto& Tag : Tags)
	{
		QueryParams.AddUnique(TEXT("tags"), Tag);
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
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
	, EAccelByteLikedContentSortBy SortBy
	, EAccelByteUgcOrderBy OrderBy)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/liked")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	TMultiMap<FString, FString> QueryParams {
		{ TEXT("name"), Name },
		{ TEXT("type"), Type },
		{ TEXT("subtype"), Subtype },
		{ TEXT("isofficial"), IsOfficial ? TEXT("true") : TEXT("false") },
		{ TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("offset"), Limit >= 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("sortby"), ConvertLikedContentSortByToString(SortBy) },
		{ TEXT("orderby"), ConvertUGCOrderByToString(OrderBy) },
	};

	for (const auto& Tag : Tags)
	{
		QueryParams.AddUnique(TEXT("tags"), Tag);
	}

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void UGC::GetCreator(const FString& UserId
	, THandler<FAccelByteModelsUGCGetListFollowersResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

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

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/groups")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMultiMap<FString, FString> QueryParams;
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

void UGC::BulkGetContentByShareCode(const TArray<FString>& ShareCodes
		, THandler<TArray<FAccelByteModelsUGCContentResponse>> const& OnSuccess
		, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ShareCodes.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCodes is empty."));
		return;
	}
	if (ShareCodes.Num() > MAX_BULK_CONTENT_SHARECODES_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_CONTENT_SHARECODES_COUNT));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/sharecodes/bulk")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	FAccelByteModelsUGCBulkGetContentByShareCodeRequest ContentRequest;
	ContentRequest.ShareCodes = ShareCodes;

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, ContentRequest, OnSuccess, OnError);
}

#pragma region UGC V2 (Content)

void UGC::SearchContentsSpecificToChannelV2(FString const& ChannelId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/channels/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ChannelId);

	const TMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void UGC::SearchContentsV2(FAccelByteModelsUGCFilterRequestV2 const& Filter
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	FString TagValue = FString::Join(Filter.Tags, TEXT(","));

	const TMap<FString, FString> QueryParams = {
		{TEXT("name"), Filter.Name.IsEmpty() ? TEXT("") : Filter.Name},
		{TEXT("type"), Filter.Type.IsEmpty() ? TEXT("") : Filter.Type},
		{TEXT("subType"), Filter.SubType.IsEmpty() ? TEXT("") : Filter.SubType},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), ConvertGetUGContentsSortByToString(SortBy)},
		{TEXT("tags"), TagValue.IsEmpty() ? TEXT("") : TagValue}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void UGC::GetContentBulkByIdsV2(TArray<FString> const& ContentIds
	, THandler<TArray<FAccelByteModelsUGCContentResponseV2>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentIds.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentIds is empty."));
		return;
	}
	if (ContentIds.Num() > MAX_BULK_CONTENT_IDS_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_CONTENT_IDS_COUNT));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/bulk")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	FAccelByteModelsUGCGetContentBulkRequest ContentRequest;
	ContentRequest.ContentIds = ContentIds;

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, ContentRequest, OnSuccess, OnError);
}

void UGC::GetContentByShareCodeV2(FString const& ShareCode
	, THandler<FAccelByteModelsUGCContentResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ShareCode);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::GetContentByContentIdV2(FString const& ContentId
	, THandler<FAccelByteModelsUGCContentResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ContentId);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::CreateContentV2(FString const& ChannelId
	, FAccelByteModelsCreateUGCRequestV2 const& CreateRequest
	, THandler<FAccelByteModelsUGCCreateUGCResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, CreateRequest, OnSuccess, OnError);
}

void UGC::DeleteContentV2(FString const& ChannelId
	, FString const& ContentId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}
	if (ContentId.IsEmpty())
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
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ContentId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::ModifyContentV2(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsModifyUGCRequestV2 const& ModifyRequest
	, THandler<FAccelByteModelsUGCModifyUGCResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}
	if (ContentId.IsEmpty())
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
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ContentId);

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, ModifyRequest, OnSuccess, OnError);
}

void UGC::GenerateUploadContentURLV2(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUploadContentURLRequestV2 const& UploadRequest
	, THandler<FAccelByteModelsUGCUploadContentURLResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}
	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (UploadRequest.FileExtension.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, FileExtension is empty."));
		return;
	}
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s/uploadUrl")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ContentId);

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, UploadRequest, OnSuccess, OnError);
}

void UGC::UpdateContentFileLocationV2(FString const& ChannelId
	, FString const& ContentId
	, FString const& FileExtension
	, FString const& S3Key
	, THandler<FAccelByteModelsUGCUpdateContentFileLocationResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}
	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (FileExtension.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, FileExtension is empty."));
		return;
	}
	if (S3Key.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, S3Key is empty."));
		return;
	}
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s/fileLocation")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ContentId);

	FAccelByteModelsUGCUpdateContentFileLocationRequestV2 Request;
	Request.FileExtension = FileExtension;
	Request.FileLocation = S3Key;

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, Request, OnSuccess, OnError);
}

void UGC::GetUserContentsV2(FString const& UserId
	, THandler<FAccelByteModelsUGCSearchContentsPagingResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (UserId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, UserId is empty."));
		return;
	}
	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/contents")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	const TMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void UGC::UpdateContentScreenshotV2(FString const& ContentId
	, FAccelByteModelsUGCUpdateScreenshotsV2 const& ScreenshotsRequest
	, THandler<FAccelByteModelsUGCUpdateScreenshotsV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/contents/%s/screenshots")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ContentId);

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, ScreenshotsRequest, OnSuccess, OnError);
}

void UGC::UploadContentScreenshotV2(FString const& ContentId
	, FAccelByteModelsUGCUploadScreenshotsRequestV2 const& ScreenshotsRequest
	, THandler<FAccelByteModelsUGCUpdateContentScreenshotResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (ScreenshotsRequest.Screenshots.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, Screenshots is empty."));
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/contents/%s/screenshots")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ContentId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, ScreenshotsRequest, OnSuccess, OnError);
}

void UGC::DeleteContentScreenshotV2(FString const& ContentId
	, FString const& ScreenshotId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (ScreenshotId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ScreenshotId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}
	if (!ValidateAccelByteId(ScreenshotId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetScreenshotIdInvalidMessage(ScreenshotId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/contents/%s/screenshots/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ContentId
		, *ScreenshotId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::ModifyContentByShareCode(FString const& ChannelId
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

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ShareCode);

	FAccelByteModelsUGCUpdateRequest Request = ModifyRequest;
	if (Request.ContentType.IsEmpty())
	{
		Request.ContentType = TEXT("application/octet-stream");
	}

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Request, OnSuccess, OnError);
}

void UGC::ModifyContentShareCode(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCModifyContentShareCodeRequest const& ModifyContentShareCodeRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}

	if (ModifyContentShareCodeRequest.ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return;
	}

	if (ModifyContentShareCodeRequest.ShareCode.Len() > 7)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode length exceeds the maximum of 7 characters."));
		return;
	}
	
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}

	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/%s/sharecode")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ContentId);

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, ModifyContentShareCodeRequest, OnSuccess, OnError);
}	

void UGC::DeleteContentByShareCode(FString const& ChannelId
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

	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/sharecodes/%s")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ShareCode);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::BulkGetContentByShareCodeV2(TArray<FString> const& ShareCodes
		, THandler<TArray<FAccelByteModelsUGCContentResponseV2>> const& OnSuccess
		, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ShareCodes.Num() == 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCodes is empty."));
		return;
	}
	if (ShareCodes.Num() > MAX_BULK_CONTENT_SHARECODES_COUNT)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), FString::Printf(TEXT("Keys cannot exceed %d!"), MAX_BULK_CONTENT_SHARECODES_COUNT));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/sharecodes/bulk")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace());

	FAccelByteModelsUGCBulkGetContentByShareCodeRequest ContentRequest;
	ContentRequest.ShareCodes = ShareCodes;

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, ContentRequest, OnSuccess, OnError);
}

void UGC::ModifyContentShareCodeV2(FString const& ChannelId
	, FString const& ContentId
	, FAccelByteModelsUGCModifyContentShareCodeRequest const& ModifyContentShareCodeRequest
	, THandler<FAccelByteModelsUGCCreateUGCResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ChannelId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ChannelId is empty."));
		return;
	}

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}

	if (ModifyContentShareCodeRequest.ShareCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode is empty."));
		return;
	}

	if (ModifyContentShareCodeRequest.ShareCode.Len() > 7)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ShareCode length exceeds the maximum of 7 characters."));
		return;
	}
	
	if (!ValidateAccelByteId(ChannelId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetChannelIdInvalidMessage(ChannelId)
		, OnError))
	{
		return;
	}

	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s/channels/%s/contents/%s/sharecode")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId()
		, *ChannelId
		, *ContentId);

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, ModifyContentShareCodeRequest, OnSuccess, OnError);
}	

#pragma endregion UGC V2 (Content)

#pragma region UGC V2 (Download Count)

void UGC::AddDownloadContentCountV2(FString const& ContentId
	, THandler<FAccelByteModelsUGCAddDownloadContentCountResponseV2> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s/downloadcount")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ContentId);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

void UGC::GetListContentDownloaderV2(FString const& ContentId
	, THandler<FAccelByteModelsUGCGetPaginatedContentDownloaderResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& UserId
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s/downloader")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ContentId);

	const TMap<FString, FString> QueryParams = {
		{TEXT("userId"), UserId.IsEmpty() ? TEXT("") : *UserId},
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

#pragma endregion UGC V2 (Download Count)

#pragma region UGC V2 (Like)

void UGC::GetListContentLikerV2(FString const& ContentId
	, THandler<FAccelByteModelsUGCGetPaginatedContentLikerResponseV2> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset
	, EAccelByteUGCContentUtilitiesSortByV2 SortBy)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s/like")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ContentId);

	const TMap<FString, FString> QueryParams = {
		{TEXT("offset"), Offset >= 0 ? FString::FromInt(Offset) : TEXT("")},
		{TEXT("limit"), Limit >= 0 ? FString::FromInt(Limit) : TEXT("")},
		{TEXT("sortBy"), SortBy == EAccelByteUGCContentUtilitiesSortByV2::NONE ? TEXT("") : ConvertUGCUtilitiesSortByToString(SortBy)}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void UGC::UpdateLikeStatusToContentV2(FString const& ContentId
	, bool bLikeStatus
	, THandler<FAccelByteModelsUGCUpdateLikeStatusToContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (ContentId.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Invalid request, ContentId is empty."));
		return;
	}
	if (!ValidateAccelByteId(ContentId, EAccelByteIdHypensRule::NO_RULE
		, FAccelByteIdValidator::GetContentIdInvalidMessage(ContentId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/contents/%s/like")
		, *SettingsRef.UGCServerUrl
		, *CredentialsRef.GetNamespace()
		, *ContentId);

	const FString Content = FString::Printf(TEXT("{\"likeStatus\": %s}"), bLikeStatus ? TEXT("true") : TEXT("false"));

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Content, OnSuccess, OnError);
}

#pragma endregion UGC V2 (Like)

}
}
