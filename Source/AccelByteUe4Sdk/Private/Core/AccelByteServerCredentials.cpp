// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteServerCredentials.h"

#include "Core/AccelByteOauth2Api.h"
#include "Models/AccelByteOauth2Models.h"
#include "AccelByteUe4SdkModule.h"


using namespace AccelByte;
using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteServerCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteServerCredentials);


namespace AccelByte
{

ServerCredentials::ServerCredentials(FHttpRetrySchedulerBase& InHttpRef, FString const& InIamServerUrl)
	: BaseCredentials()
	, IamServerUrl(InIamServerUrl)
	, Oauth(InHttpRef, InIamServerUrl)
{
}

TCHAR const* ServerCredentials::DefaultSection = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettings");

void ServerCredentials::ForgetAll()
{
	BaseCredentials::ForgetAll();
}

void ServerCredentials::SetClientCredentials(const ESettingsEnvironment Environment)
{
	//Synchronization is handled by BaseCredentials::SetClientCredentials() member method

	FString SectionPath;
	switch (Environment)
	{
	case ESettingsEnvironment::Development:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsDev");
		break;
	case ESettingsEnvironment::Certification:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsCert");
		break;
	case ESettingsEnvironment::Production:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsProd");
		break;
	case ESettingsEnvironment::Sandbox:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsSandbox");
		break;
	case ESettingsEnvironment::Integration:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsIntegration");
		break;
	case ESettingsEnvironment::QA:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettingsQA");
		break;
	case ESettingsEnvironment::Default:
	default:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettings");
		break;
	}

	FString ClientIdFromConfig;
	FString ClientSecretFromCConfig;
	if (GConfig->GetString(*SectionPath, TEXT("ClientId"), ClientIdFromConfig, GEngineIni))
	{
		GConfig->GetString(*SectionPath, TEXT("ClientSecret"), ClientSecretFromCConfig, GEngineIni);
	}
	else
	{
		GConfig->GetString(DefaultSection, TEXT("ClientId"), ClientIdFromConfig, GEngineIni);
		GConfig->GetString(DefaultSection, TEXT("ClientSecret"), ClientSecretFromCConfig, GEngineIni);
	}
	SetClientCredentials(ClientIdFromConfig, ClientSecretFromCConfig);
}

void ServerCredentials::SetClientToken(const FString& InAccessToken, double ExpiresIn, const FString& InNamespace)
{
	FOauth2Token NewAuthToken;
	NewAuthToken.Access_token = InAccessToken;
	NewAuthToken.Expires_in = ExpiresIn;
	NewAuthToken.Namespace = InNamespace;

	SetAuthToken(NewAuthToken, FPlatformTime::Seconds());
}

void ServerCredentials::Startup()
{
	BaseCredentials::Startup();
	
	SetTickerDelegate(FTickerDelegate::CreateThreadSafeSP(AsShared(), &ServerCredentials::Tick));
	IAccelByteUe4SdkModuleInterface& ABSDKModule = IAccelByteUe4SdkModuleInterface::Get();
	SetClientCredentials(ABSDKModule.GetSettingsEnvironment());
}

void ServerCredentials::SendRefreshToken()
{
	FString TempClientId = GetOAuthClientId();
	FString TempClientSecret = GetOAuthClientSecret();

	Oauth.GetTokenWithClientCredentials(TempClientId
		, TempClientSecret
		, THandler<FOauth2Token>::CreateThreadSafeSP(this, &ServerCredentials::OnPollRefreshTokenResponseSuccess)
		, FErrorHandler::CreateThreadSafeSP(this, &ServerCredentials::OnPollRefreshTokenResponseError)
		, IamServerUrl);
}

void ServerCredentials::OnPollRefreshTokenResponseSuccess(const FOauth2Token& Result)
{
	SetAuthToken(Result, FPlatformTime::Seconds());

	auto RefreshedEvent = OnTokenRefreshed();
	RefreshedEvent.Broadcast(true);
}

void ServerCredentials::OnPollRefreshTokenResponseError(int32 Code, const FString& Message)
{
	CalculateNextRefreshToken();

	auto RefreshedEvent = OnTokenRefreshed();
	RefreshedEvent.Broadcast(false);
}

void ServerCredentials::SetMatchId(const FString& GivenMatchId)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	MatchId = GivenMatchId;
}

const FString& ServerCredentials::GetClientAccessToken() const
{
	return GetAccessToken();
}

const FString& ServerCredentials::GetClientNamespace() const
{
	return GetNamespace();
}

const FString& ServerCredentials::GetMatchId() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return MatchId;
}

} // Namespace AccelByte