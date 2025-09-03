// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteOauth2Api.h"
#include "Models/AccelByteOauth2Models.h"
#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteReport.h"


using namespace AccelByte;
using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCredentials);

namespace AccelByte
{

Credentials::Credentials(FHttpRetryScheduler& InHttpRef, FAccelByteMessagingSystem& MessagingRef, FString const& InIamServerUrl)
	: BaseCredentials()
	, Oauth(InHttpRef, InIamServerUrl)
#if ENGINE_MAJOR_VERSION < 5
	, MessagingSystemWPtr(MessagingRef.AsShared())
#else
	, MessagingSystemWPtr(MessagingRef.AsWeak())
#endif
	, RefreshTokenTask(nullptr)
	, IamServerUrl(InIamServerUrl)
{
}

Credentials::~Credentials()
{
	FWriteScopeLock WriteLock(DelegateLock);

	LoginSuccessDelegate.Clear();
	LogoutSuccessDelegate.Clear();

	auto RefreshTokenTaskPtr = RefreshTokenTask.Pin();
	if (RefreshTokenTaskPtr.IsValid())
	{
		RefreshTokenTaskPtr->Cancel();
	}

	if (BearerAuthRejectedHandle.IsValid())
	{
		BearerAuthRejectedHandle.Reset();
	}
}

TCHAR const* Credentials::DefaultSection = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettings");

void Credentials::ForgetAll()
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	BaseCredentials::ForgetAll();
}

void Credentials::SetClientCredentials(const ESettingsEnvironment Environment)
{
	//Synchronization is handled by BaseCredentials::SetClientCredentials() member method

	FString SectionPath;
	switch (Environment)
	{
	case ESettingsEnvironment::Development:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsDev");
		break;
	case ESettingsEnvironment::Certification:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsCert");
		break;
	case ESettingsEnvironment::Production:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsProd");
		break;
	case ESettingsEnvironment::Sandbox:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsSandbox");
		break;
	case ESettingsEnvironment::Integration:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsIntegration");
		break;
	case ESettingsEnvironment::QA:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettingsQA");
		break;
	case ESettingsEnvironment::Default:
	default:
		SectionPath = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettings");
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

bool Credentials::SetAuthToken(const FOauth2Token& InAuthToken, float CurrentTime)
{
	bool IsSuccess = BaseCredentials::SetAuthToken(InAuthToken, CurrentTime);
	
	double BanDuration = DBL_MAX;
	const int64 UtcNowTimestamp = FDateTime::UtcNow().ToUnixTimestamp();
	for (auto& Ban : InAuthToken.Bans)
	{
		const int64 BanEndTimestamp = Ban.EndDate.ToUnixTimestamp();
		if (!Ban.Enabled || BanEndTimestamp < UtcNowTimestamp)
		{
			continue;
		}
		
		const double TempBanDuration = BanEndTimestamp - UtcNowTimestamp;
		if (TempBanDuration < BanDuration)
		{
			BanDuration = TempBanDuration;
		}
	}
	
	if (GetExpireDuration() >= BanDuration)
	{
		SetExpireDuration(BanDuration);
	}

	CalculateBackoffRetry(CurrentTime);

	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();

	if (MessagingSystemPtr.IsValid())
	{
		MessagingSystemPtr->SendMessage<FOauth2Token>(EAccelByteMessagingTopic::AuthTokenSet, GetAuthToken());
	}

	return IsSuccess;
}

void Credentials::SetUserEmailAddress(const FString& EmailAddress)
{
	AccelByte::FReport::LogDeprecated( FString(__FUNCTION__),
		TEXT("Deprecated, this method is planned to be decomissioned. Please use UpdateUser method in UserAPI class to modify the email address"));
}

void Credentials::SetUserName(const FString& Name)
{
	AccelByte::FReport::LogDeprecated( FString(__FUNCTION__),
		TEXT("Deprecated, this method is planned to be decomissioned. User name is not available anymore, please use User display name."));
}

void Credentials::SetUserDisplayName(const FString& UserDisplayName)
{
	AccelByte::FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Deprecated, this method is planned to be decomissioned. Please use UpdateUser method in UserAPI class to modify the display name."));
}

bool Credentials::IsSessionValid() const
{
	return GetSessionState() == ESessionState::Valid;
}

bool Credentials::IsComply() const
{
	FOauth2Token TempToken = GetAuthToken();
	return TempToken.Is_comply;
}

void Credentials::Startup()
{
	BaseCredentials::Startup();

	SetTickerDelegate(FTickerDelegate::CreateThreadSafeSP(AsShared(), &Credentials::Tick));
	IAccelByteUe4SdkModuleInterface& ABSDKModule = IAccelByteUe4SdkModuleInterface::Get();
	SetClientCredentials(ABSDKModule.GetSettingsEnvironment());
}

FString Credentials::GetUserDisplayName() const
{
	return GetDisplayName();
}

FString Credentials::GetUserEmailAddress() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AccountUserData.EmailAddress;
}

FString Credentials::GetUserName() const
{
	AccelByte::FReport::LogDeprecated( FString(__FUNCTION__),
		TEXT("Deprecated, please use email address instead"));
	return UserName;
}

FAccountUserData Credentials::GetAccountUserData() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AccountUserData;
}

const TMap<FString, FThirdPartyPlatformTokenData>& Credentials::GetThridPartyPlatformTokenData() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return ThirdPartyPlatformTokenData;
}

void Credentials::SendRefreshToken()
{
	FString TempClientId = GetOAuthClientId();
	FString TempClientSecret = GetOAuthClientSecret();
	FOauth2Token TempToken = GetAuthToken();
	FWriteScopeLock WriteLock(DelegateLock);
	RefreshTokenTask = Oauth.GetTokenWithRefreshTokenV4(TempClientId
		, TempClientSecret
		, TempToken.Refresh_token
		, THandler<FOauth2TokenV4>::CreateThreadSafeSP(AsShared(), &Credentials::OnRefreshTokenSuccessful)
		, FOAuthErrorHandler::CreateThreadSafeSP(AsShared(), &Credentials::OnRefreshTokenFailed)
		, IamServerUrl
	);
}

void Credentials::SetBearerAuthRejectedHandler(FHttpRetryScheduler& HttpRef)
{
	FWriteScopeLock WriteLock(DelegateLock);

	if (BearerAuthRejectedHandle.IsValid())
	{
		HttpRef.RemoveBearerAuthRejectedDelegate(BearerAuthRejectedHandle);
		BearerAuthRejectedHandle.Reset();
	}
	FHttpRetrySchedulerWPtr HttpWPtr = HttpRef.AsShared();
	BearerAuthRejectedHandle = HttpRef.AddBearerAuthRejectedDelegate(
		FHttpRetryScheduler::FBearerAuthRejected::CreateThreadSafeSP(AsShared(), &Credentials::OnBearerAuthRejected, HttpWPtr));
}

void Credentials::SetAccountUserData(const FAccountUserData& InAccountUserData)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	AccountUserData = InAccountUserData;
}

void Credentials::SetThridPartyPlatformTokenData(const FString& PlatformId, const FThirdPartyPlatformTokenData& InThirdPartyPlatformTokenData)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	ThirdPartyPlatformTokenData.Emplace(PlatformId, InThirdPartyPlatformTokenData);
}
	
void Credentials::ClearThridPartyPlatformTokenData()
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	ThirdPartyPlatformTokenData.Empty();
}

Credentials::FOnLoginSuccessDelegate& Credentials::OnLoginSuccess()
{
	FReadScopeLock ReadLock(DelegateLock);
	return LoginSuccessDelegate;
}

Credentials::FOnLogoutSuccessDelegate& Credentials::OnLogoutSuccess()
{
	FReadScopeLock ReadLock(DelegateLock);
	return LogoutSuccessDelegate;
}

void Credentials::OnBearerAuthRejected(FHttpRetrySchedulerWPtr HttpWPtr)
{
	ESessionState CopySessionState{};

	CopySessionState = GetSessionState();

	auto HttpPtr = HttpWPtr.Pin();

	if (!HttpPtr.IsValid() || CopySessionState == ESessionState::Refreshing)
	{
		return;
	}

	if (CopySessionState == ESessionState::Invalid)
	{
		HttpPtr->ResumeBearerAuthRequest(GetAccessToken());
		return;
	}

	UE_LOG(LogAccelByteCredentials, Verbose, TEXT("OnBearerAuthRejected triggered"));
	HttpPtr->PauseBearerAuthRequest();

	SetSessionState(ESessionState::Rejected);

	RefreshTokenAdditionalActions.AddThreadSafeSP(AsShared(), &Credentials::OnBearerAuthRefreshed, HttpWPtr);

	ScheduleRefreshToken(FPlatformTime::Seconds());
}

void Credentials::OnBearerAuthRefreshed(bool bSuccessful, FHttpRetrySchedulerWPtr HttpWPtr)
{
	// No need to use ScopeLock at the moment
	// There is no access to this->member directly, only rely to GetAccessToken() below

	auto HttpPtr = HttpWPtr.Pin();
	if (!HttpPtr.IsValid())
	{
		return;
	}

	FString UpdatedToken = TEXT("");
	if (bSuccessful)
	{
		UpdatedToken = GetAccessToken();
	}

	HttpPtr->ResumeBearerAuthRequest(UpdatedToken);
}

void Credentials::OnRefreshTokenSuccessful(FOauth2TokenV4 const& Token)
{
	SetAuthToken(Token, FPlatformTime::Seconds());

	FWriteScopeLock WriteLock(DelegateLock);
	if (RefreshTokenAdditionalActions.IsBound())
	{
		RefreshTokenAdditionalActions.Broadcast(true);
		RefreshTokenAdditionalActions.Clear();
	}

	auto RefreshedEvent = OnTokenRefreshed();
	RefreshedEvent.Broadcast(true);
}

void Credentials::OnRefreshTokenFailed(int32 ErrorCode
	, FString const& ErrorMessage
	, FErrorOAuthInfo const& OAuthError)
{
	CalculateNextRefreshToken();

	{
		FWriteScopeLock WriteLock(DelegateLock);

		if (RefreshTokenAdditionalActions.IsBound())
		{
			RefreshTokenAdditionalActions.Broadcast(false);
			RefreshTokenAdditionalActions.Clear();
		}

		auto RefreshedEvent = OnTokenRefreshed();
		RefreshedEvent.Broadcast(false);
	}
}

} // Namespace AccelByte