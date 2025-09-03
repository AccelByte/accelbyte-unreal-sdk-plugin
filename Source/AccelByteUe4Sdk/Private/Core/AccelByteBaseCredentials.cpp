// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteBaseCredentials.h"

namespace AccelByte
{

	BaseCredentials::BaseCredentials()
		: ClientId()
		, ClientSecret()
		, SessionState(ESessionState::Invalid)
		, ExpireDuration(0.0f)
		, RefreshTime(0.0f)
		, ExpireTime(0.0f)
		, RefreshWindow(0.0f)
		, RefreshBackoff(0.0f)
	{
	}

	BaseCredentials::~BaseCredentials()
	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		TokenRefreshedEvent.Clear();
	}

	void BaseCredentials::ForgetAll()
	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		AuthToken = {};
		ExpireDuration = 0.0f;
		RefreshTime = 0.0f;
		ExpireTime = 0.0f;
		RefreshWindow = 0.0f;
		RefreshBackoff = 0.0f;
		SessionState = ESessionState::Invalid;
	}

	void BaseCredentials::SetClientCredentials(const FString& InClientId, const FString& InClientSecret)
	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		ClientId = InClientId;
		ClientSecret = InClientSecret;
	}

	BaseCredentials::FTokenRefreshedEvent& BaseCredentials::OnTokenRefreshed()
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return TokenRefreshedEvent;
	}

	void BaseCredentials::Startup()
	{
		ForgetAll();
	}

	void BaseCredentials::Shutdown()
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

	const FString& BaseCredentials::GetOAuthClientId() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return ClientId;
	}

	const FString& BaseCredentials::GetOAuthClientSecret() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return ClientSecret;
	}

	const FOauth2Token& BaseCredentials::GetAuthToken() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return AuthToken;
	}

	const FString& BaseCredentials::GetAccessToken() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return AuthToken.Access_token;
	}

	const FString& BaseCredentials::GetNamespace() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return AuthToken.Namespace;
	}

	BaseCredentials::ESessionState BaseCredentials::GetSessionState() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return SessionState;
	}

	const FString& BaseCredentials::GetUserId() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return AuthToken.User_id;
	}

	const TMap<FString, FString> BaseCredentials::GetAuthHeader() const
	{
		FString const AccessToken = GetAccessToken();
		
		FReadScopeLock ReadLock(CredentialAccessLock);
		if (SessionState == ESessionState::Valid)
		{
			if(!AccessToken.IsEmpty())
			{
				TMap<FString, FString> Headers{};
				Headers.Add(TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *AccessToken));
				return Headers;
			}
		}
		return TMap<FString, FString>{};
	}

	const FErrorOAuthInfo& BaseCredentials::GetErrorOAuth() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return ErrorOAuth;
	}

	void BaseCredentials::SetErrorOAuth(const FErrorOAuthInfo& NewErrorOAuthInfo)
	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		ErrorOAuth = NewErrorOAuthInfo;
	}

	FString BaseCredentials::GetRefreshToken() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return AuthToken.Refresh_token;
	}

	FString BaseCredentials::GetPlatformUserId() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return AuthToken.Platform_user_id;
	}

	FString BaseCredentials::GetSimultaneousPlatformId() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return AuthToken.Simultaneous_platform_id;
	}

	FString BaseCredentials::GetSimultaneousPlatformUserId() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return AuthToken.Simultaneous_platform_user_id;
	}

	FString BaseCredentials::GetSimultaneousPlatformUserIdByPlatformName(const FString& PlatformName) const
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

	FString BaseCredentials::GetDisplayName() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return AuthToken.Display_name;
	}

	FString BaseCredentials::GetUniqueDisplayName() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		if (!AuthToken.Unique_display_name.IsEmpty())
		{
			return AuthToken.Unique_display_name;
		}
		return AuthToken.Display_name;
	}

	FString BaseCredentials::GetLinkingToken() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return ErrorOAuth.LinkingToken;
	}

	bool BaseCredentials::Tick(float DeltaTime)
	{
		PollRefreshToken(FPlatformTime::Seconds());
		return true;
	}

	bool BaseCredentials::SetAuthToken(FOauth2Token const& InAuthToken, float CurrentTime)
	{
		if (InAuthToken.Expires_in < GetMinExpireDuration())
		{
			return false;
		}

		{
			FWriteScopeLock WriteLock(CredentialAccessLock);
			AuthToken = InAuthToken;
			ExpireDuration = AuthToken.Expires_in;
		}

		SetSessionState(ESessionState::Valid);

		CalculateBackoffRetry(CurrentTime);
		return true;
	}

	void BaseCredentials::CalculateBackoffRetry(float CurrentTime)
	{
		RefreshWindow = ExpireDuration * GetRefreshWindowPercentage();

		double MinWindowTime = GetMinRefreshWindow();
		double MaxWindowTime = GetMaxRefreshWindow();
		if (RefreshWindow <= MinWindowTime)
		{
			RefreshWindow = MinWindowTime;
		}
		else if (RefreshWindow >= MaxWindowTime)
		{
			RefreshWindow = MaxWindowTime;
		}

		ExpireTime = CurrentTime + ExpireDuration;
		RefreshTime = ExpireTime - RefreshWindow;
		RefreshBackoff = RefreshWindow * InitialBackoffPercentage;
	}
	
	void BaseCredentials::PollRefreshToken(double CurrentTime)
	{
		ESessionState CurrentState;
		{
			FReadScopeLock ReadLock(CredentialAccessLock);
			CurrentState = SessionState;
		}
#ifdef ACCELBYTE_ACTIVATE_PROFILER
		TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelBytePollRefreshToken"));
#endif
		switch (CurrentState)
		{
		case ESessionState::Expired:
		case ESessionState::Rejected:
		case ESessionState::Valid:
			if (RefreshTime <= CurrentTime)
			{
				SetSessionState(ESessionState::Refreshing);
				SendRefreshToken();
			}
			break;
		case ESessionState::Refreshing:
		case ESessionState::Invalid:
		default:
			break;
		}
	}

	void BaseCredentials::ScheduleRefreshToken(double NextRefreshTime)
	{
		RefreshTime = NextRefreshTime;

		if (RefreshTime > ExpireTime)
		{
			RefreshTime = ExpireTime.load();
		}

		SetSessionState(ESessionState::Expired);
	}

	void BaseCredentials::CalculateNextRefreshToken()
	{
		double CurrentTime = FPlatformTime::Seconds();
		double NextRefreshTime = CurrentTime + RefreshBackoff;

		if (NextRefreshTime < ExpireTime + GetExpireTimeBuffer())
		{
			ScheduleRefreshToken(NextRefreshTime);
			RefreshBackoff = RefreshBackoff * BackoffMultiplier;
		}
		else
		{
			SetSessionState(ESessionState::Invalid);
		}
	}

	void BaseCredentials::SetSessionState(ESessionState NewState)
	{
		FWriteScopeLock WriteLock(CredentialAccessLock);
		SessionState = NewState;
	}

	double BaseCredentials::GetMinExpireDuration() const
	{
		return MinExpireDuration;
	}

	double BaseCredentials::GetRefreshWindowPercentage() const
	{
		return RefreshWindowPercentage;
	}

	double BaseCredentials::GetMinRefreshWindow() const
	{
		return MinRefreshWindow;
	}

	double BaseCredentials::GetMaxRefreshWindow() const
	{
		return MaxRefreshWindow;
	}

	double BaseCredentials::GetExpireTimeBuffer() const
	{
		return ExpireTimeBuffer;
	}

	double BaseCredentials::GetExpireTime() const
	{
		return ExpireTime;
	}

	void BaseCredentials::SetExpireTime(double InExpireTime)
	{
		ExpireTime = InExpireTime;
	}

	double BaseCredentials::GetRefreshTime() const
	{
		return RefreshTime;
	}

	void BaseCredentials::SetRefreshTime(double InRefreshTime)
	{
		RefreshTime = InRefreshTime;
	}

	double BaseCredentials::GetExpireDuration() const
	{
		return ExpireDuration;
	}

	void BaseCredentials::SetExpireDuration(double InExpireDuration)
	{
		ExpireDuration = InExpireDuration;
	}

	double BaseCredentials::GetRefreshWindow() const
	{
		return RefreshWindow;
	}

	double BaseCredentials::GetRefreshBackoffTime() const
	{
		return RefreshBackoff;
	}

	void BaseCredentials::SetTickerDelegate(FTickerDelegate const& InDelegate)
	{
		FWriteScopeLock WriteLock(DelegateLock);
		if (!PollRefreshTokenHandle.IsValid())
		{
			PollRefreshTokenHandle = FTickerAlias::GetCoreTicker().AddTicker(InDelegate, 0.2f);
		}
	}
}