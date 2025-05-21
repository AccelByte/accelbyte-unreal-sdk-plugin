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

ServerCredentials::ServerCredentials(FHttpRetryScheduler& InHttpRef, FString const& InIamServerUrl)
	: AccessToken()
	, Namespace()
	, UserId()
	, IamServerUrl(InIamServerUrl)
	, Oauth(InHttpRef, InIamServerUrl)
{
}

TCHAR const* ServerCredentials::DefaultSection = TEXT("/Script/AccelByteUe4Sdk.AccelByteServerSettings");

void ServerCredentials::ForgetAll()
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	BaseCredentials::ForgetAll();
	AccessToken = FString();
}

void ServerCredentials::SetClientCredentials(const ESettingsEnvironment Environment)
{
	//Only read ClietId & ClientSecret at the end of scope
	FReadScopeLock ReadLock(CredentialAccessLock);

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

	if (GConfig->GetString(*SectionPath, TEXT("ClientId"), ClientId, GEngineIni))
	{
		GConfig->GetString(*SectionPath, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
	else
	{
		GConfig->GetString(DefaultSection, TEXT("ClientId"), ClientId, GEngineIni);
		GConfig->GetString(DefaultSection, TEXT("ClientSecret"), ClientSecret, GEngineIni);
	}
}

void ServerCredentials::SetClientToken(const FString& InAccessToken, double ExpiresIn, const FString& InNamespace)
{
	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		AccessToken = InAccessToken;
		ExpireTime = ExpiresIn;
		Namespace = InNamespace;
		SessionState = ESessionState::Valid;
	}

	if (ExpiresIn > MaxBackoffTime)
	{
		ScheduleRefreshToken(ExpireTime - MaxBackoffTime);
	}
	else
	{
		ScheduleRefreshToken(ExpireTime*BackoffRatio);
	}

	{
		FWriteScopeLock WriteLock(DelegateLock);
		BackoffCount.Reset();
		if (!PollRefreshTokenHandle.IsValid()) {
			PollRefreshTokenHandle = FTickerAlias::GetCoreTicker().AddTicker(
				FTickerDelegate::CreateThreadSafeSP(AsShared(), &ServerCredentials::Tick),
				0.2f);
		}
	}
}

void ServerCredentials::Startup()
{
	IAccelByteUe4SdkModuleInterface& ABSDKModule = IAccelByteUe4SdkModuleInterface::Get();
	SetClientCredentials(ABSDKModule.GetSettingsEnvironment());
}

void ServerCredentials::Shutdown()
{
	// Required because this function is accessing PollRefreshTokenHandle
	FWriteScopeLock WriteLock(DelegateLock);
	RemoveFromTicker(PollRefreshTokenHandle);
}

void ServerCredentials::RemoveFromTicker(FDelegateHandleAlias& handle)
{
	if (handle.IsValid())
	{
		FTickerAlias::GetCoreTicker().RemoveTicker(handle);
		handle.Reset();
	}
}
void ServerCredentials::PollRefreshToken(double CurrentTime)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);

#ifdef ACCELBYTE_ACTIVATE_PROFILER
	TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelByteServerPollRefreshToken"));
#endif
	switch (SessionState)
	{
		case ESessionState::Expired:
		case ESessionState::Valid:
			if (RefreshTime <= CurrentTime)
			{
				Oauth.GetTokenWithClientCredentials(ClientId
					, ClientSecret
					, THandler<FOauth2Token>::CreateThreadSafeSP(this, &ServerCredentials::OnPollRefreshTokenResponseSuccess)
					, FErrorHandler::CreateThreadSafeSP(this, &ServerCredentials::OnPollRefreshTokenResponseError)
					, IamServerUrl);

				SessionState = ESessionState::Refreshing;
			}

			break;
		case ESessionState::Refreshing:
		case ESessionState::Invalid:
			break;
	}
}

void ServerCredentials::OnPollRefreshTokenResponseSuccess(const FOauth2Token& Result)
{
	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		SessionState = ESessionState::Valid;
	}

	SetClientToken(Result.Access_token, Result.Expires_in, Result.Namespace);

	{
		FWriteScopeLock WriteLock(DelegateLock);
		TokenRefreshedEvent.Broadcast(true); 
	}
}

void ServerCredentials::OnPollRefreshTokenResponseError(int32 Code, const FString& Message)
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

		ScheduleRefreshToken(RefreshBackoff);
	}
	else
	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		SessionState = ESessionState::Invalid;
	}

	{
		FWriteScopeLock WriteLock(DelegateLock);
		TokenRefreshedEvent.Broadcast(false);
	}
}

void ServerCredentials::ScheduleRefreshToken(double NextRefreshTime)
{
	FWriteScopeLock WriteLock(CredentialAccessLock);
	RefreshTime = FPlatformTime::Seconds() + NextRefreshTime;
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

const FString& ServerCredentials::GetAccessToken() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return AccessToken;
}

const FString& ServerCredentials::GetClientNamespace() const
{
	return GetNamespace();
}

const FString& ServerCredentials::GetNamespace() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return Namespace;
}

const double ServerCredentials::GetExpireTime() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return ExpireTime;
}

const double ServerCredentials::GetRefreshTime() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return RefreshTime;
}

const FString& ServerCredentials::GetMatchId() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return MatchId;
}

const FString& ServerCredentials::GetUserId() const
{
	FReadScopeLock ReadLock(CredentialAccessLock);
	return UserId;
}

} // Namespace AccelByte