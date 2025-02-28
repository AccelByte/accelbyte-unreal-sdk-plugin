// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteCredentials.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteOauth2Api.h"
#include "Models/AccelByteOauth2Models.h"
#include "AccelByteUe4SdkModule.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteRegistry.h"

using namespace AccelByte;
using namespace AccelByte::Api;

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteCredentials, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteCredentials);


namespace AccelByte
{

Credentials::Credentials(FAccelByteMessagingSystem& MessagingRef)
	: AuthToken()
#if ENGINE_MAJOR_VERSION < 5
	, MessagingSystemWPtr(MessagingRef.AsShared())
#else
	, MessagingSystemWPtr(MessagingRef.AsWeak())
#endif
	, RefreshTokenTask(nullptr)
{
}

Credentials::~Credentials()
{
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

const FString Credentials::DefaultSection = TEXT("/Script/AccelByteUe4Sdk.AccelByteSettings");

void Credentials::ForgetAll()
{
	BaseCredentials::ForgetAll();
	AuthToken = {};
}

void Credentials::SetClientCredentials(const ESettingsEnvironment Environment)
{
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

	if (GConfig->GetString(*SectionPath, TEXT("ClientId"), ClientId, GEngineIni))
	{
		GConfig->GetString(*SectionPath, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
	else
	{
		GConfig->GetString(*DefaultSection, TEXT("ClientId"), ClientId, GEngineIni);
		GConfig->GetString(*DefaultSection, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
}


void Credentials::SetAuthToken(const FOauth2Token& NewAuthToken, float CurrentTime)
{
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
	BackoffCount = 0;
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
	AuthToken.Display_name = UserDisplayName;
}

const FString& Credentials::GetAccessToken() const
{
	return AuthToken.Access_token;
}

const FString& Credentials::GetRefreshToken() const
{
	return AuthToken.Refresh_token;
}

const FString& Credentials::GetNamespace() const
{
	return AuthToken.Namespace;
}

const FString& Credentials::GetPlatformUserId() const
{
	return AuthToken.Platform_user_id;
}

const FString& Credentials::GetSimultaneousPlatformId() const
{
	return AuthToken.Simultaneous_platform_id;
}

const FString& Credentials::GetSimultaneousPlatformUserId() const
{
	return AuthToken.Simultaneous_platform_user_id;
}

FString Credentials::GetSimultaneousPlatformUserIdByPlatformName(const FString& PlatformName) const
{
	FString OutPlatformUserId = TEXT("");
	if (AuthToken.Simultaneous_platform_id.Contains(PlatformName))
	{
		OutPlatformUserId = AuthToken.Simultaneous_platform_user_id;
	}
	return OutPlatformUserId;
}

bool Credentials::IsSessionValid() const
{
	return SessionState == ESessionState::Valid;
}

bool Credentials::IsComply() const
{
	return AuthToken.Is_comply;
}

void Credentials::Startup()
{
	PollRefreshTokenHandle = FTickerAlias::GetCoreTicker().AddTicker(
        FTickerDelegate::CreateThreadSafeSP(AsShared(), &Credentials::Tick),
        0.2f);
	IAccelByteUe4SdkModuleInterface& ABSDKModule = IAccelByteUe4SdkModuleInterface::Get();
	SetClientCredentials(ABSDKModule.GetSettingsEnvironment());
}

void Credentials::Shutdown()
{
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
	return AuthToken.User_id;
}

const FString& Credentials::GetUserDisplayName() const
{
	return AuthToken.Display_name;
}

const FString& Credentials::GetUserEmailAddress() const
{ 
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
	if (!AuthToken.Unique_display_name.IsEmpty())
	{
		return AuthToken.Unique_display_name;
	}
	return AuthToken.Display_name;
}

const FString& Credentials::GetLinkingToken() const
{
	return ErrorOAuth.LinkingToken;
}

const FAccountUserData& Credentials::GetAccountUserData() const
{
	return AccountUserData;
}

const TMap<FString, FThirdPartyPlatformTokenData>& Credentials::GetThridPartyPlatformTokenData() const
{
	return ThirdPartyPlatformTokenData;
}

void Credentials::PollRefreshToken(double CurrentTime)
{
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
			RefreshTokenTask = Oauth2::GetTokenWithRefreshToken(ClientId
				, ClientSecret
				, AuthToken.Refresh_token
				, THandler<FOauth2Token>::CreateThreadSafeSP(AsShared(), &Credentials::OnRefreshTokenSuccessful)
				, FErrorHandler::CreateThreadSafeSP(AsShared(), &Credentials::OnRefreshTokenFailed)
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
	RefreshTime = LRefreshTime;
}

const FOauth2Token& Credentials::GetAuthToken() const
{
	return AuthToken;
}

void Credentials::SetBearerAuthRejectedHandler(FHttpRetryScheduler& HttpRef)
{
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
	ErrorOAuth = NewErrorOAuthInfo;
}

void Credentials::SetAccountUserData(const FAccountUserData& InAccountUserData)
{
	AccountUserData = InAccountUserData;
}

void Credentials::SetThridPartyPlatformTokenData(const FString& PlatformId, const FThirdPartyPlatformTokenData& InThirdPartyPlatformTokenData)
{
	ThirdPartyPlatformTokenData.Emplace(PlatformId, InThirdPartyPlatformTokenData);
}
	
void Credentials::ClearThridPartyPlatformTokenData()
{
	ThirdPartyPlatformTokenData.Empty();
}

Credentials::FOnLoginSuccessDelegate& Credentials::OnLoginSuccess()
{
	return LoginSuccessDelegate;
}

Credentials::FOnLogoutSuccessDelegate& Credentials::OnLogoutSuccess()
{
	return LogoutSuccessDelegate;
}

void Credentials::OnBearerAuthRejected(FHttpRetrySchedulerWPtr HttpWPtr)
{
	auto HttpPtr = HttpWPtr.Pin();

	if (!HttpPtr.IsValid() || GetSessionState() == ESessionState::Refreshing)
	{
		return;
	}

	if (GetSessionState() == ESessionState::Invalid)
	{
		HttpPtr->ResumeBearerAuthRequest(GetAccessToken());
		return;
	}

	UE_LOG(LogAccelByteCredentials, Verbose, TEXT("OnBearerAuthRejected triggered"));
	HttpPtr->PauseBearerAuthRequest();
	SessionState = ESessionState::Rejected;

	RefreshTokenAdditionalActions.AddThreadSafeSP(AsShared(), &Credentials::OnBearerAuthRefreshed, HttpWPtr);

	ScheduleRefreshToken(FPlatformTime::Seconds());
}

void Credentials::OnBearerAuthRefreshed(bool bSuccessful, FHttpRetrySchedulerWPtr HttpWPtr)
{
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

void Credentials::OnRefreshTokenSuccessful(FOauth2Token const& Token)
{
	SetAuthToken(Token, FPlatformTime::Seconds());
	if (RefreshTokenAdditionalActions.IsBound())
	{
		RefreshTokenAdditionalActions.Broadcast(true);
		RefreshTokenAdditionalActions.Clear();
	}
	TokenRefreshedEvent.Broadcast(true);
}

void Credentials::OnRefreshTokenFailed(int32 ErrorCode, FString const& ErrorMessage)
{
	BackoffCount++;

	if (BackoffCount < MaxBackoffCount)
	{
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
		ScheduleRefreshToken(FPlatformTime::Seconds() + RefreshBackoff);
	}

	if (RefreshTokenAdditionalActions.IsBound())
	{
		RefreshTokenAdditionalActions.Broadcast(false);
		RefreshTokenAdditionalActions.Clear();
	}

	if (BackoffCount < MaxBackoffCount)
	{
		SessionState = ESessionState::Expired;
	}
	else
	{
		SessionState = ESessionState::Invalid;
	}
	TokenRefreshedEvent.Broadcast(false);
}

} // Namespace AccelByte

#include "Core/AccelByteRegistry.h"

FString UAccelByteBlueprintsCredentials::GetOAuthClientId()
{
	return FRegistry::Credentials.GetOAuthClientId();
}

FString UAccelByteBlueprintsCredentials::GetOAuthClientSecret()
{
	return FRegistry::Credentials.GetOAuthClientSecret();
}

FString UAccelByteBlueprintsCredentials::GetUserSessionId()
{
	return FRegistry::Credentials.GetAccessToken();
}

FString UAccelByteBlueprintsCredentials::GetUserId()
{
	return FRegistry::Credentials.GetUserId();
}

FString UAccelByteBlueprintsCredentials::GetUserDisplayName()
{
	return FRegistry::Credentials.GetUserDisplayName();
}

FString UAccelByteBlueprintsCredentials::GetUserNamespace()
{
	return FRegistry::Credentials.GetNamespace();
}

FString UAccelByteBlueprintsCredentials::GetUserEmailAddress()
{
	return FRegistry::Credentials.GetUserEmailAddress();
}

FString UAccelByteBlueprintsCredentials::GetUserName()
{
	return FRegistry::Credentials.GetUserName();
}

FString UAccelByteBlueprintsCredentials::GetUniqueDisplayName()
{
	return FRegistry::Credentials.GetUniqueDisplayName();
}
