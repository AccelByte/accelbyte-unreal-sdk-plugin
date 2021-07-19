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
UGC::UGC(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Settings) : Credentials(Credentials), Settings(Settings)
{
}
	
UGC::~UGC(){}

void UGC::CreateContent(const FString& ChannelId, const FAccelByteModelsUGCRequest& CreateRequest,
						const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3"), *Settings.UGCServerUrl, *Settings.Namespace, *Credentials.GetUserId(), *ChannelId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(CreateRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::CreateContent(const FString& ChannelId, const FString& Name, const FString& Type,
	const FString& SubType, const TArray<FString>& Tags, const TArray<uint8>& Preview, const FString& FileExtension,
	const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FAccelByteModelsUGCRequest Req;
	Req.Name = Name;
	Req.Type = Type;
	Req.SubType = SubType;
	Req.Tags = Tags;
	Req.Preview = FBase64::Encode(Preview);
	Req.FileExtension = FileExtension;

	CreateContent(ChannelId, Req, OnSuccess, OnError);
}

void UGC::ModifyContent(const FString& ChannelId, const FString& ContentId, const FAccelByteModelsUGCRequest& ModifyRequest,
	const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/s3/%s"), *Settings.UGCServerUrl, *Settings.Namespace, *Credentials.GetUserId(), *ChannelId, *ContentId);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ModifyRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::ModifyContent(const FString& ChannelId, const FString& ContentId, const FString& Name,
	const FString& Type, const FString& SubType, const TArray<FString>& Tags, const TArray<uint8>& Preview,
	const FString& FileExtension, const THandler<FAccelByteModelsUGCResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FAccelByteModelsUGCRequest Req;
	Req.Name = Name;
	Req.Type = Type;
	Req.SubType = SubType;
	Req.Tags = Tags;
	Req.Preview = FBase64::Encode(Preview);
	Req.FileExtension = FileExtension;

	ModifyContent(ChannelId, ContentId, Req, OnSuccess, OnError);
}

void UGC::DeleteContent(const FString& ChannelId, const FString& ContentId,	const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s/contents/%s"), *Settings.UGCServerUrl, *Settings.Namespace, *Credentials.GetUserId(), *ChannelId, *ContentId);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::GetContentByContentId(const FString& ContentId,
								const THandler<FAccelByteModelsUGCContentResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s"), *Settings.UGCServerUrl, *Settings.Namespace, *ContentId);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::GetContentByShareCode(const FString& ShareCode,
	const THandler<FAccelByteModelsUGCContentResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/sharecodes/%s"), *Settings.UGCServerUrl, *Settings.Namespace, *ShareCode);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::GetContentPreview(const FString& ContentId, const THandler<FAccelByteModelsUGCPreview>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/contents/%s/preview"), *Settings.UGCServerUrl, *Settings.Namespace, *ContentId);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());	
}

void UGC::GetContentPreview(const FString& ContentId, const THandler<TArray<uint8>>& OnSuccess,
	const FErrorHandler& OnError)
{
	GetContentPreview(ContentId, THandler<FAccelByteModelsUGCPreview>::CreateLambda([OnSuccess](const FAccelByteModelsUGCPreview& Response)
	{
		TArray<uint8> Bytes;
		if(FBase64::Decode(Response.Preview, Bytes))
			OnSuccess.ExecuteIfBound(Bytes);
		else
			UE_LOG(LogAccelByte, Warning, TEXT("Cannot decode preview string : %s"), *Response.Preview);
	}), OnError);
}

void UGC::GetTags(const THandler<FAccelByteModelsUGCTagsPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 Limit, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/tags?limit=%d&offset=%d"), *Settings.UGCServerUrl, *Settings.Namespace, Limit, Offset);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::GetTypes(const THandler<FAccelByteModelsUGCTypesPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 Limit, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/types?limit=%d&offset=%d"), *Settings.UGCServerUrl, *Settings.Namespace, Limit, Offset);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::CreateChannel(const FString& ChannelName, const THandler<FAccelByteModelsUGCChannelResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels"), *Settings.UGCServerUrl, *Settings.Namespace, *Credentials.GetUserId());
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::GetChannels(const THandler<FAccelByteModelsUGCChannelsPagingResponse>& OnSuccess, const FErrorHandler& OnError, int32 Limit, int32 Offset)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels?limit=%d&offset=%d"), *Settings.UGCServerUrl, *Settings.Namespace, *Credentials.GetUserId(), Limit, Offset);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGC::DeleteChannel(const FString& ChannelId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v1/public/namespaces/%s/users/%s/channels/%s"), *Settings.UGCServerUrl, *Settings.Namespace, *Credentials.GetUserId(), *ChannelId);
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

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
}
}
