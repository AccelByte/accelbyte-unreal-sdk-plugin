// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "UGCTestAdmin.h"
#include "TestUtilities.h"

void UGCCreateType(const FString& Type, const TArray<FString>& SubType, const THandler<FAccelByteModelsUGCTypeResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/ugc/v1/admin/namespaces/%s/types"), *BaseUrl, *FRegistry::ServerSettings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FString SubTypeString;
	for(const FString& Stype : SubType)
	{
		if(!SubTypeString.IsEmpty())
			SubTypeString += ",";
		SubTypeString += "\"" + Stype + "\"" ;
	}
	
	FString Content = FString::Printf(TEXT("{\"subtype\": [%s], \"type\": \"%s\"}"), *SubTypeString, *Type);
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGCCreateTags(const FString& Tag, const THandler<FAccelByteModelsUGCTagResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/ugc/v1/admin/namespaces/%s/tags"), *BaseUrl, *FRegistry::ServerSettings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{\"tag\": \"%s\"}"), *Tag);
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGCDeleteType(const FString& TypeId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/ugc/v1/admin/namespaces/%s/types/%s"), *BaseUrl, *FRegistry::ServerSettings.Namespace, *TypeId);
	FString Verb = TEXT("DELETE");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void UGCDeleteTag(const FString& TagId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString BaseUrl = GetAdminBaseUrl();
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminAccessToken());
	FString Url = FString::Printf(TEXT("%s/ugc/v1/admin/namespaces/%s/tags/%s"), *BaseUrl, *FRegistry::ServerSettings.Namespace, *TagId);
	FString Verb = TEXT("DELETE");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
