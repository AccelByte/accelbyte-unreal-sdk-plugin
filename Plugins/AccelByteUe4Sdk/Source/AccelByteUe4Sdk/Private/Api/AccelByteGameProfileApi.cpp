// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Api/AccelByteGameProfileApi.h"
#include "Modules/ModuleManager.h"
#include "IWebSocket.h"
#include "WebSocketsModule.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"

namespace AccelByte
{
namespace Api
{

GameProfile::GameProfile(const AccelByte::Credentials& Credentials, const AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting)
{
}

GameProfile::~GameProfile() { }

void GameProfile::BatchGetPublicGameProfiles(TArray<FString> UserIds, const THandler<TArray<FAccelByteModelsPublicGameProfile>>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	if (UserIds.Num() < 1)
	{
		OnError.ExecuteIfBound(EHttpResponseCodes::NotFound, TEXT("Please provide at least one user ID."));
	}
	else
	{
		FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
		FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/profiles"), *Settings.GameProfileServerUrl, *Credentials.GetUserNamespace());
		FString Verb            = TEXT("GET");
		FString ContentType     = TEXT("application/json");
		FString Accept          = TEXT("application/json");
		FString Content;

		for (int i = 0 ; i < UserIds.Num() ; i++)
		{
			Url.Append((i == 0) ? TEXT("?") : TEXT("&")).Append(TEXT("userIds=")).Append(UserIds[i]);
		}

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

void GameProfile::GetAllGameProfiles(const THandler<TArray<FAccelByteModelsGameProfile>>& OnSuccess, const FErrorHandler & OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	
	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles"), *Settings.GameProfileServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId());
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
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

void GameProfile::CreateGameProfile(const FAccelByteModelsGameProfileRequest & GameProfileRequest, const THandler<FAccelByteModelsGameProfile>& OnSuccess, const FErrorHandler & OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles"), *Settings.GameProfileServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId());
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString<FAccelByteModelsGameProfileRequest>(GameProfileRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void GameProfile::GetGameProfile(const FString & ProfileId, const THandler<FAccelByteModelsGameProfile>& OnSuccess, const FErrorHandler & OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s"), *Settings.GameProfileServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *ProfileId);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
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

void GameProfile::UpdateGameProfile(const FString & ProfileId, const FAccelByteModelsGameProfileRequest & GameProfileRequest, const THandler<FAccelByteModelsGameProfile>& OnSuccess, const FErrorHandler & OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s"), *Settings.GameProfileServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *ProfileId);
	FString Verb            = TEXT("PUT");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(GameProfileRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void GameProfile::DeleteGameProfile(const FString & ProfileId, const FVoidHandler& OnSuccess, const FErrorHandler & OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s"), *Settings.GameProfileServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *ProfileId);
	FString Verb            = TEXT("DELETE");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
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

void GameProfile::GetGameProfileAttribute(const FString & ProfileId, const FString & AttributeName, const THandler<FAccelByteModelsGameProfileAttribute>& OnSuccess, const FErrorHandler & OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	
	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s/attributes/%s"), *Settings.GameProfileServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *ProfileId, *AttributeName);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
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

void GameProfile::UpdateGameProfileAttribute(const FString & ProfileId, const FAccelByteModelsGameProfileAttribute& Attribute, const THandler<FAccelByteModelsGameProfile>& OnSuccess, const FErrorHandler & OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));
	
	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url             = FString::Printf(TEXT("%s/public/namespaces/%s/users/%s/profiles/%s/attributes/%s"), *Settings.GameProfileServerUrl, *Credentials.GetUserNamespace(), *Credentials.GetUserId(), *ProfileId, *Attribute.name);
	FString Verb            = TEXT("PUT");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Attribute, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
