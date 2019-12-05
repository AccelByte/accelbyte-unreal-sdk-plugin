// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "GameServerApi/AccelByteServerOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Runtime/Core/Public/Misc/Base64.h"

namespace AccelByte
{
namespace GameServerApi
{

void ServerOauth2::GetAccessTokenWithClientCredentialsGrant(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	FString Authorization   = TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret));
	FString Url             = FString::Printf(TEXT("%s/oauth/token"), *FRegistry::ServerSettings.IamServerUrl);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/x-www-form-urlencoded");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("grant_type=client_credentials"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void ServerOauth2::LoginWithClientCredentials(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	GetAccessTokenWithClientCredentialsGrant(Settings.ClientId, Settings.ClientSecret, THandler<FOauth2Token>::CreateLambda([OnSuccess](const FOauth2Token& Result)
	{
		FRegistry::ServerCredentials.SetClientToken(Result.Access_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void ServerOauth2::ForgetAllCredentials()
{
	Report report;
	report.GetFunctionLog(FString(__FUNCTION__));

	Credentials.ForgetAll();
}

ServerOauth2::ServerOauth2(AccelByte::ServerCredentials& Credentials, AccelByte::ServerSettings& Settings) : Credentials(Credentials), Settings(Settings)
{}

ServerOauth2::~ServerOauth2()
{}

} // Namespace Api
} // Namespace AccelByte