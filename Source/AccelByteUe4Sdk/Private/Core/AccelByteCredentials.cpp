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
	: AuthToken()
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
	AuthToken = {};
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

void Credentials::SetAuthToken(const FOauth2Token& NewAuthToken, float CurrentTime)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);

	ExpireTime = NewAuthToken.Expires_in;
	UserSessionExpire = CurrentTime + (NewAuthToken.Expires_in * FMath::FRandRange(0.7, 0.9));
	
	double BanExpire = DBL_MAX;
	const int64 UtcNowTimestamp = FDateTime::UtcNow().ToUnixTimestamp();
	for (auto& Ban : NewAuthToken.Bans)
	{
		const int64 BanEndTimestamp = Ban.EndDate.ToUnixTimestamp();
		if (!Ban.Enabled || BanEndTimestamp < UtcNowTimestamp)
		{
			continue;
		}
		
		const double BanDuration = CurrentTime + (BanEndTimestamp - UtcNowTimestamp);
		if (BanDuration < BanExpire)
		{
			BanExpire = BanDuration;
		}
	}
	
	if (UserSessionExpire >= BanExpire)
	{
		const double BanSpanTime = BanExpire - CurrentTime;
		ExpireTime = BanSpanTime;
	}

	if (ExpireTime > MaxBackoffTime)
	{
		RefreshTime = CurrentTime + (ExpireTime - MaxBackoffTime);
	}
	else
	{
		RefreshTime = CurrentTime + (ExpireTime * BackoffRatio);
	}

	AuthToken = NewAuthToken;
	BackoffCount.Reset();
	SessionState = ESessionState::Valid;

	auto MessagingSystemPtr = MessagingSystemWPtr.Pin();

	if (MessagingSystemPtr.IsValid())
	{
		MessagingSystemPtr->SendMessage<FOauth2Token>(EAccelByteMessagingTopic::AuthTokenSet, NewAuthToken);
	}
}

void Credentials::SetUserEmailAddress(const FString& EmailAddress)
{
	AccelByte::FReport::LogDeprecated( FString(__FUNCTION__),
		TEXT("Deprecated, this method could not be used to set email address, use GetData instead."));
}

void Credentials::SetUserName(const FString& Name)
{
	AccelByte::FReport::LogDeprecated( FString(__FUNCTION__),
		TEXT("Deprecated, please use email address instead"));
	UserName = Name;
}

void Credentials::SetUserDisplayName(const FString& UserDisplayName)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	AuthToken.Display_name = UserDisplayName;
}

const FString& Credentials::GetAccessToken() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken.Access_token;
}

const FString& Credentials::GetRefreshToken() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken.Refresh_token;
}

const FString& Credentials::GetNamespace() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken.Namespace;
}

const FString& Credentials::GetPlatformUserId() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken.Platform_user_id;
}

const FString& Credentials::GetSimultaneousPlatformId() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken.Simultaneous_platform_id;
}

const FString& Credentials::GetSimultaneousPlatformUserId() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken.Simultaneous_platform_user_id;
}

FString Credentials::GetSimultaneousPlatformUserIdByPlatformName(const FString& PlatformName) const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	if (AuthToken.Simultaneous_platform_id.Contains(PlatformName))
	{
		return AuthToken.Simultaneous_platform_user_id;
	}
	else
	{
		return TEXT("");
	}
}

bool Credentials::IsSessionValid() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return SessionState == ESessionState::Valid;
}

bool Credentials::IsComply() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken.Is_comply;
}

void Credentials::Startup()
{
	{
		FWriteScopeLock WriteLock(DelegateLock);
		PollRefreshTokenHandle = FTickerAlias::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateThreadSafeSP(AsShared(), &Credentials::Tick),
			0.2f);
	}

	IAccelByteUe4SdkModuleInterface& ABSDKModule = IAccelByteUe4SdkModuleInterface::Get();
	SetClientCredentials(ABSDKModule.GetSettingsEnvironment());
}

void Credentials::Shutdown()
{
	FWriteScopeLock WriteLock(DelegateLock);

	if (PollRefreshTokenHandle.IsValid())
	{
		// Core ticker by this point in engine shutdown has already been torn down - only remove ticker if this is not an engine shutdown
		if (!IsEngineExitRequested())
		{
			FTickerAlias::GetCoreTicker().RemoveTicker(PollRefreshTokenHandle);
		}
		PollRefreshTokenHandle.Reset();
	}
}

const FString& Credentials::GetUserId() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken.User_id;
}

const FString& Credentials::GetUserDisplayName() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken.Display_name;
}

const FString& Credentials::GetUserEmailAddress() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AccountUserData.EmailAddress;
}

const FString& Credentials::GetUserName() const
{
	AccelByte::FReport::LogDeprecated( FString(__FUNCTION__),
		TEXT("Deprecated, please use email address instead"));
	return UserName;
}

const FString& Credentials::GetUniqueDisplayName() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	if (!AuthToken.Unique_display_name.IsEmpty())
	{
		return AuthToken.Unique_display_name;
	}
	return AuthToken.Display_name;
}

const FString& Credentials::GetLinkingToken() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return ErrorOAuth.LinkingToken;
}

const FAccountUserData& Credentials::GetAccountUserData() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AccountUserData;
}

const TMap<FString, FThirdPartyPlatformTokenData>& Credentials::GetThridPartyPlatformTokenData() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return ThirdPartyPlatformTokenData;
}

void Credentials::PollRefreshToken(double CurrentTime)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);

#ifdef ACCELBYTE_ACTIVATE_PROFILER
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelBytePollRefreshToken"));
#endif
	switch (SessionState)
	{
	case ESessionState::Expired:
	case ESessionState::Rejected:
	case ESessionState::Valid:
		if (RefreshTime <= CurrentTime)
		{
			RefreshTokenTask = Oauth.GetTokenWithRefreshTokenV4(ClientId
				, ClientSecret
				, AuthToken.Refresh_token
				, THandler<FOauth2TokenV4>::CreateThreadSafeSP(AsShared(), &Credentials::OnRefreshTokenSuccessful)
				, FOAuthErrorHandler::CreateThreadSafeSP(AsShared(), &Credentials::OnRefreshTokenFailed)
				, IamServerUrl
			);

			SessionState = ESessionState::Refreshing;
		}

		break;
	case ESessionState::Refreshing:
	case ESessionState::Invalid:
	default:
		break;
	}
}

void Credentials::ScheduleRefreshToken(double LRefreshTime)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	RefreshTime = LRefreshTime;
}

const FOauth2Token& Credentials::GetAuthToken() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AuthToken;
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

void Credentials::SetErrorOAuth(const FErrorOAuthInfo& NewErrorOAuthInfo)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	ErrorOAuth = NewErrorOAuthInfo;
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

	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		SessionState = ESessionState::Rejected;
	}

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
	TokenRefreshedEvent.Broadcast(true);
}

void Credentials::OnRefreshTokenFailed(int32 ErrorCode
	, FString const& ErrorMessage
	, FErrorOAuthInfo const& OAuthError)
{
	BackoffCount.Increment();

	if (BackoffCount.GetValue() < MaxBackoffCount)
	{
		{
			// Separate scope to avoid locking with ScheduleRefreshToken
			FWriteScopeLock WriteLock(CredentialAccessLock);

			if (ExpireTime > MaxBackoffTime)
			{
				ExpireTime = MaxBackoffTime;
			}
			else if (ExpireTime <= MinBackoffTime)
			{
				ExpireTime = MinBackoffTime;
			}
			else
			{
				ExpireTime = ExpireTime * BackoffRatio;
			}

			RefreshBackoff = ExpireTime * BackoffRatio;
			SessionState = ESessionState::Expired;
		}

		ScheduleRefreshToken(FPlatformTime::Seconds() + RefreshBackoff);
	}
	else
	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		SessionState = ESessionState::Invalid;
	}

	{
		FWriteScopeLock WriteLock(DelegateLock);

		if (RefreshTokenAdditionalActions.IsBound())
		{
			RefreshTokenAdditionalActions.Broadcast(false);
			RefreshTokenAdditionalActions.Clear();
		}

		TokenRefreshedEvent.Broadcast(false);
	}
}

} // Namespace AccelByte