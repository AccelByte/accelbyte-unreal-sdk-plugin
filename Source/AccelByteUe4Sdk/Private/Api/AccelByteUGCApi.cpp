// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteUGCApi.h"

#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteSettings.h"
#include "Core/AccelByteUtilities.h"

namespace AccelByte
{
namespace Api
{

UGC::UGC(Credentials const& InCredentialsRef
	, Settings const& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: HttpRef{InHttpRef}
	, CredentialsRef{InCredentialsRef}
	, SettingsRef{InSettingsRef}
{}

UGC::~UGC(){}

void UGC::CreateContent(FString const& ChannelId
	, FAccelByteModelsUGCRequest const& CreateRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *CredentialsRef.GetUserId(), *ChannelId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FAccelByteModelsUGCRequest Req = CreateRequest;	
	if (Req.ContentType.IsEmpty())
	{
		Req.ContentType = TEXT("application/octet-stream");
	}

	FJsonObjectConverter::UStructToJsonObjectString(Req, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
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
	, FAccelByteModelsUGCRequest const& ModifyRequest
	, THandler<FAccelByteModelsUGCResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3/%s"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *CredentialsRef.GetUserId(), *ChannelId, *ContentId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;

	FAccelByteModelsUGCRequest Req = ModifyRequest;	
	if (Req.ContentType.IsEmpty())
	{
		Req.ContentType = TEXT("application/octet-stream");
	}
	
	FJsonObjectConverter::UStructToJsonObjectString(Req, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
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

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/%s"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *CredentialsRef.GetUserId(), *ChannelId, *ContentId);
	FString Verb = TEXT("DELETE");
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

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *ContentId);
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

void UGC::GetContentByShareCode(FString const& ShareCode
	, THandler<FAccelByteModelsUGCContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/sharecodes/%s"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *ShareCode);
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

void UGC::GetContentPreview(FString const& ContentId
	, THandler<FAccelByteModelsUGCPreview> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s/preview"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *ContentId);
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

void UGC::GetContentPreview(FString const& ContentId
	, THandler<TArray<uint8>> const& OnSuccess
	, FErrorHandler const& OnError)
{
	GetContentPreview(ContentId, THandler<FAccelByteModelsUGCPreview>::CreateLambda([OnSuccess](FAccelByteModelsUGCPreview const& Response)
	{
		TArray<uint8> Bytes;
		if(FBase64::Decode(Response.Preview, Bytes))
			OnSuccess.ExecuteIfBound(Bytes);
		else
			UE_LOG(LogAccelByte, Warning, TEXT("Cannot decode preview string : %s"), *Response.Preview);
	}), OnError);
}

void UGC::GetTags(THandler<FAccelByteModelsUGCTagsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/tags?limit=%d&offset=%d"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, Limit, Offset);
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

void UGC::GetTypes(THandler<FAccelByteModelsUGCTypesPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/types?limit=%d&offset=%d"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, Limit, Offset);
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

void UGC::CreateChannel(FString const& ChannelName
	, THandler<FAccelByteModelsUGCChannelResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *CredentialsRef.GetUserId());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{\"name\": \"%s\"}"), *ChannelName);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::GetChannels(THandler<FAccelByteModelsUGCChannelsPagingResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Limit
	, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels?limit=%d&offset=%d"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *CredentialsRef.GetUserId(), Limit, Offset);
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

void UGC::DeleteChannel(FString const& ChannelId
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *CredentialsRef.GetUserId(), *ChannelId);
	FString Verb = TEXT("DELETE");
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

void UGC::UpdateLikeStatusToContent(const FString& ContentId, bool bLikeStatus
	, THandler<FAccelByteModelsUGCUpdateLikeStatusToContentResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s/like"), *SettingsRef.UGCServerUrl, *SettingsRef.Namespace, *ContentId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json"); 
	FString Content = FString::Printf(TEXT("{\"likeStatus\": %s}"), bLikeStatus ? TEXT("true") : TEXT("false"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	HttpRef.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
}
}
