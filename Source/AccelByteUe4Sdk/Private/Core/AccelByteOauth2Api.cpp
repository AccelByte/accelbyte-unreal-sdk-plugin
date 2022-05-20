// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteOauth2Api.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "JsonUtilities.h"
#include "Core/AccelByteUtilities.h"
#include "Misc/Base64.h"

FString LocalDeviceId()
{
	FString NewDeviceId;

	//check then read file
	FString CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("DeviceIdHelper/DeviceId.txt"));
	CurrentDirectory.ReplaceInline(TEXT("/"), TEXT("\\"));
	FFileHelper::LoadFileToString(NewDeviceId, *CurrentDirectory);

	//generate one if still empty
	if(NewDeviceId.IsEmpty())
	{
		FString FilePath = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
		FilePath.Append("DeviceIdHelper/DeviceId.txt");

		TCHAR valid[] = {'0', '1', '2', '3', '4', '5', '6','7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', 'g',
						'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
		
		for(int i = 0; i < 30; i++)
		{
			NewDeviceId = NewDeviceId + valid[FMath::RandRange(0, 35)];
		}
		
		FFileHelper::SaveStringToFile(NewDeviceId, *FilePath);
	}
	return NewDeviceId;
}

FString GetDeviceId()
{
	FString DeviceId = FPlatformMisc::GetDeviceId();
	if (DeviceId.IsEmpty())
	{
		PRAGMA_DISABLE_DEPRECATION_WARNINGS
		const TArray<uint8> MacAddr = FPlatformMisc::GetMacAddress();
		PRAGMA_ENABLE_DEPRECATION_WARNINGS
		FString MacAddressString;
		for (TArray<uint8>::TConstIterator it(MacAddr);it;++it)
		{
			MacAddressString += FString::Printf(TEXT("%02x"),*it);
		}
		DeviceId = FMD5::HashAnsiString(*MacAddressString);

		if(DeviceId.IsEmpty())
		{
			DeviceId = *LocalDeviceId();			
		}
	}
	return  DeviceId;
}

namespace AccelByte
{
namespace Api
{
void Oauth2::GetTokenWithAuthorizationCode(const FString& ClientId, const FString& ClientSecret, const FString& AuthorizationCode, const FString& RedirectUri, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("grant_type=authorization_code&code=%s&redirect_uri=%s"), *AuthorizationCode, *RedirectUri));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentials(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("When 2FA enabled This method should change to Oauth2::GetTokenWithPasswordCredentials(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError)"));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	const TCHAR Format[] = TEXT("grant_type=password&username=%s&password=%s&device_id=%s");
	const FString EncodedUsername = FGenericPlatformHttp::UrlEncode(Username);
	const FString EncodedPassword =  FGenericPlatformHttp::UrlEncode(Password);
	const FString EncodedDeviceId = FGenericPlatformHttp::UrlEncode(GetDeviceId());
	Request->SetContentAsString(FString::Printf(Format, *EncodedUsername, *EncodedPassword, *EncodedDeviceId));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentials(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device_token=" + FGenericPlatformHttp::UrlEncode(GetDeviceId())));
	
	const TCHAR Format[] = TEXT("grant_type=password&username=%s&password=%s&device_id=%s");
	const FString EncodedUsername = FGenericPlatformHttp::UrlEncode(Username);
	const FString EncodedPassword =  FGenericPlatformHttp::UrlEncode(Password);
	const FString EncodedDeviceId = FGenericPlatformHttp::UrlEncode(GetDeviceId());
	Request->SetContentAsString(FString::Printf(Format, *EncodedUsername, *EncodedPassword, *EncodedDeviceId));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithClientCredentials(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("grant_type=client_credentials")));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

//
// Custom grant types 
//

void Oauth2::GetTokenWithDeviceId(const FString& ClientId, const FString& ClientSecret, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/platforms/device/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

	Request->SetContentAsString(FString::Printf(TEXT("device_id=%s"), *FGenericPlatformHttp::UrlEncode(*GetDeviceId())));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("When 2FA enabled This method should change to Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError)"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/platforms/%s/token"), *FRegistry::Settings.IamServerUrl, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("platform_token=%s"), *FGenericPlatformHttp::UrlEncode(PlatformToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
	
void Oauth2::GetTokenWithOtherPlatformToken(const FString& ClientId, const FString& ClientSecret, const FString& PlatformId, const FString& PlatformToken,
	const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool bCreateHeadless )
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/platforms/%s/token"), *FRegistry::Settings.IamServerUrl, *PlatformId));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device_token=" + FGenericPlatformHttp::UrlEncode(GetDeviceId())));
	Request->SetContentAsString(FString::Printf(TEXT("platform_token=%s&createHeadless=%s"), *FGenericPlatformHttp::UrlEncode(PlatformToken),
		bCreateHeadless ? TEXT("true") : TEXT("false")));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithRefreshToken(const FString& ClientId, const FString& ClientSecret, const FString& RefreshId, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("grant_type=refresh_token&refresh_token=%s"), *RefreshId));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::RevokeToken(const FString& AccessToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("This method uses wrong auth type and will be deprecated. Use RevokeUserToken instead!"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/revoke"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Bearer " + AccessToken));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(FVoidHandler::CreateLambda([OnSuccess]() { 
		OnSuccess.ExecuteIfBound();
	}), OnError), FPlatformTime::Seconds());
}

void Oauth2::RevokeUserToken(const FString& ClientId, const FString& ClientSecret, const FString& AccessToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/revoke"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("token=%s"), *FGenericPlatformHttp::UrlEncode(*AccessToken)));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(FVoidHandler::CreateLambda([OnSuccess]() { 
	OnSuccess.ExecuteIfBound();
	}), OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FErrorHandler& OnError, bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(
		FString(__FUNCTION__),
		TEXT("When 2FA enabled This method should change to Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool RememberMe)"));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));

	const TCHAR Format[] = TEXT("grant_type=password&username=%s&password=%s&namespace=%s&extend_exp=%s");
	const FString EncodedUsername = FGenericPlatformHttp::UrlEncode(Username);
	const FString EncodedPassword =  FGenericPlatformHttp::UrlEncode(Password);
	const FString Namespace =  FGenericPlatformHttp::UrlEncode(*FRegistry::Settings.Namespace);
	const FString Extend_exp = bRememberMe ? FString::Printf(TEXT("true")) : FString::Printf(TEXT("false"));
	
	Request->SetContentAsString(FString::Printf(Format, *EncodedUsername, *EncodedPassword, *Namespace, *Extend_exp));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::GetTokenWithPasswordCredentialsV3(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device_token=" + FGenericPlatformHttp::UrlEncode(GetDeviceId())));

	const TCHAR Format[] = TEXT("grant_type=password&username=%s&password=%s&namespace=%s&extend_exp=%s");
	const FString EncodedUsername = FGenericPlatformHttp::UrlEncode(Username);
	const FString EncodedPassword =  FGenericPlatformHttp::UrlEncode(Password);
	const FString Namespace =  FGenericPlatformHttp::UrlEncode(*FRegistry::Settings.Namespace);
	const FString Extend_exp = bRememberMe ? FString::Printf(TEXT("true")) : FString::Printf(TEXT("false"));
	
	Request->SetContentAsString(FString::Printf(Format, *EncodedUsername, *EncodedPassword, *Namespace, *Extend_exp));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::VerifyAndRememberNewDevice(const FString& ClientId, const FString& ClientSecret, const FString& MfaToken, EAccelByteLoginAuthFactorType AuthFactorType, const FString& Code, 
	const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError, bool bRememberDevice)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Factor = FAccelByteUtilities::GetAuthenticatorString(AuthFactorType);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/oauth/mfa/verify"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetHeader(TEXT("cookie"), TEXT("device_token=" + FGenericPlatformHttp::UrlEncode(GetDeviceId())));
	const FString RememberThisDevice = bRememberDevice ? FString::Printf(TEXT("true")) : FString::Printf(TEXT("false"));
	Request->SetContentAsString(FString::Printf(TEXT("mfaToken=%s&factor=%s&code=%s&rememberDevice=%s"), *MfaToken, *Factor, *Code, *RememberThisDevice));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void Oauth2::CreateHeadlessAccountAndResponseToken(const FString& ClientId, const FString& ClientSecret, const FString& LinkingToken, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST")); 
	Request->SetURL(FString::Printf(TEXT("%s/v3/headless/token"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("linkingToken=%s&client_id=%s"), *LinkingToken, *ClientId));
	
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds()); 
}

void Oauth2::AuthenticationWithPlatformLink(const FString& ClientId, const FString& ClientSecret, const FString& Username, const FString& Password, const FString& LinkingToken, const THandler<FOauth2Token>& OnSuccess, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(TEXT("POST"));
	Request->SetURL(FString::Printf(TEXT("%s/v3/authenticateWithLink"), *FRegistry::Settings.IamServerUrl));
	Request->SetHeader(TEXT("Authorization"), TEXT("Basic " + FBase64::Encode(ClientId + ":" + ClientSecret)));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
	Request->SetContentAsString(FString::Printf(TEXT("username=%s&password=%s&linkingToken=%s&client_id=%s"),
		*FGenericPlatformHttp::UrlEncode(Username),
		*FGenericPlatformHttp::UrlEncode(Password),
		*LinkingToken, *ClientId));

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

} // Namespace Api
} // Namespace AccelByte
