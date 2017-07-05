// Copyright (c) 2017 AccelByte Inc. All Rights Reserved.

#include "JusticeIamClient.h"
#include "Runtime/Core/Public/Misc/Base64.h"
#include "OnlineSubsystemJusticeModule.h"

FJusticeIamClient::FJusticeIamClient()
{
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("BaseURL"), BaseURL, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing BaseURL= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	BaseURL.Append(TEXT("/iam"));

	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientId"), ClientId, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientId= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
	if (!GConfig->GetString(TEXT("OnlineSubsystemJustice"), TEXT("ClientSecret"), ClientSecret, GEngineIni))
	{
		UE_LOG_ONLINE(Error, TEXT("Missing ClientSecret= in [OnlineSubsystemJustice] of DefaultEngine.ini"));
	}
}

void FJusticeIamClient::ClientCredentialsTokenGrant()
{
	TSharedRef<IHttpRequest> Request = FJusticeHTTP::Get().CreateRequest();
	Request->SetURL(BaseURL + TEXT("/oauth/token"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Authorization"), FJusticeHTTP::BasicAuth(ClientId, ClientSecret));
	Request->SetHeader(TEXT("Content-Type"),  TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("Accept"),        TEXT("application/json"));
	Request->SetContentAsString(TEXT("grant_type=client_credentials"));
	Request->OnProcessRequestComplete().BindRaw(this, &FJusticeIamClient::ClientCredentialsTokenGrantComplete);
	Request->ProcessRequest();
}

void FJusticeIamClient::ClientCredentialsTokenGrantComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded)
{
	FString ResponseStr = Response->GetContentAsString();
	UE_LOG_ONLINE(Verbose, TEXT("Client token grant response. url=%s code=%d response=%s"),
				  *Request->GetURL(), Response->GetResponseCode(), *ResponseStr);

	if (!bSucceeded || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		UE_LOG_ONLINE(Error, TEXT("Client token grant request failed. url=%s code=%d response=%s"),
					  *Request->GetURL(), Response->GetResponseCode(), *ResponseStr);
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG_ONLINE(Error, TEXT("Malformed client token grant response. url=%s code=%d response=%s"),
					  *Request->GetURL(), Response->GetResponseCode(), *ResponseStr);
		return;
	}

	if (!JsonObject->TryGetStringField(TEXT("access_token"), ClientToken.AccessToken) ||
		!JsonObject->TryGetNumberField(TEXT("expires_in"),   ClientToken.ExpiresIn)   ||
		!JsonObject->TryGetStringField(TEXT("token_type"),   ClientToken.TokenType))
	{
		UE_LOG_ONLINE(Error, TEXT("Unexpected response. url=%s code=%d response=%s"),
					  *Request->GetURL(), Response->GetResponseCode(), *ResponseStr);
	}

}

void FJusticeIamClient::PasswordTokenGrant(const FString& Username, const FString& Password)
{
	TSharedRef<IHttpRequest> Request = FJusticeHTTP::Get().CreateRequest();
	Request->SetURL(BaseURL + TEXT("/oauth/token"));
	Request->SetVerb(TEXT("POST"));
	Request->SetHeader(TEXT("Authorization"), FJusticeHTTP::BasicAuth(ClientId, ClientSecret));
	Request->SetHeader(TEXT("Content-Type"),  TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("Accept"),        TEXT("application/json"));
	FString PasswordGrant = TEXT("grant_type=password") +
						    TEXT("&username=") + *FGenericPlatformHttp::UrlEncode(Username) +
						    TEXT("&password=") + *FGenericPlatformHttp::UrlEncode(Password);
	Request->SetContentAsString(PasswordGrant);
	Request->OnProcessRequestComplete().BindRaw(this, &FJusticeIamClient::PasswordTokenGrantComplete);
	Request->ProcessRequest();
}

void FJusticeIamClient::PasswordTokenGrantComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSucceeded)
{
	FString ResponseStr = Response->GetContentAsString();
	UE_LOG_ONLINE(Verbose, TEXT("Password token grant response. url=%s code=%d response=%s"),
				  *Request->GetURL(), Response->GetResponseCode(), *ResponseStr);
	
	if (!bSucceeded || !Response.IsValid() || !EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		UE_LOG_ONLINE(Error, TEXT("Password token grant request failed. url=%s code=%d response=%s"),
					  *Request->GetURL(), Response->GetResponseCode(), *ResponseStr);
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG_ONLINE(Error, TEXT("Malformed password token grant response. url=%s code=%d response=%s"),
					  *Request->GetURL(), Response->GetResponseCode(), *ResponseStr);
		return;
	}
	
	if (!JsonObject->TryGetStringField(TEXT("access_token"), PlayerToken.AccessToken)   ||
		!JsonObject->TryGetStringField(TEXT("refresh_token"),PlayerToken.RefreshToken)  ||
		!JsonObject->TryGetNumberField(TEXT("expires_in"),   PlayerToken.ExpiresIn)     ||
		!JsonObject->TryGetStringField(TEXT("token_type"),   PlayerToken.TokenType))
	{
		UE_LOG_ONLINE(Error, TEXT("Unexpected response. url=%s code=%d response=%s"),
					  *Request->GetURL(), Response->GetResponseCode(), *ResponseStr);
	}

}

