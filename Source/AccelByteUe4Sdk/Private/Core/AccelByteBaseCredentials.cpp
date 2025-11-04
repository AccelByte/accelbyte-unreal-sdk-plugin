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
		FWriteScopeLock WriteLock(TokenRefreshedEventMtx);
		TokenRefreshedEvent.Clear();
	}

	void BaseCredentials::ForgetAll()
	{
		{
			FWriteScopeLock WriteLock(AuthTokenMtx);		
			AuthToken = {};
		}
		ExpireDuration.store(0.0f, std::memory_order_release);
		RefreshTime.store(0.0f, std::memory_order_release);
		ExpireTime.store(0.0f, std::memory_order_release);
		RefreshWindow.store(0.0f, std::memory_order_release);
		RefreshBackoff.store(0.0f, std::memory_order_release);
		SessionState.store(ESessionState::Invalid, std::memory_order_release);
	}

	void BaseCredentials::SetClientCredentials(const FString& InClientId, const FString& InClientSecret)
	{
		{
			FWriteScopeLock WriteLock(ClientIdMtx);
			ClientId = InClientId;
		}
		{
			FWriteScopeLock WriteLock(ClientSecretMtx);
			ClientSecret = InClientSecret;
		}
	}

	BaseCredentials::FTokenRefreshedEvent& BaseCredentials::OnTokenRefreshed()
	{
		FReadScopeLock ReadLock(TokenRefreshedEventMtx);
		return TokenRefreshedEvent;
	}

	void BaseCredentials::Startup()
	{
		ForgetAll();
	}

	void BaseCredentials::Shutdown()
	{
		FWriteScopeLock WriteLock(PollRefreshTokenHandleMtx);
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
		FReadScopeLock ReadLock(ClientIdMtx);
		return ClientId;
	}

	const FString& BaseCredentials::GetOAuthClientSecret() const
	{
		FReadScopeLock ReadLock(ClientSecretMtx);
		return ClientSecret;
	}

	const FOauth2Token& BaseCredentials::GetAuthToken() const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
		return AuthToken;
	}

	const FString& BaseCredentials::GetAccessToken() const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
		return AuthToken.Access_token;
	}

	const FString& BaseCredentials::GetNamespace() const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
		return AuthToken.Namespace;
	}

	BaseCredentials::ESessionState BaseCredentials::GetSessionState() const
	{
		return SessionState.load(std::memory_order_acquire);
	}

	const FString& BaseCredentials::GetUserId() const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
		return AuthToken.User_id;
	}

	const TMap<FString, FString> BaseCredentials::GetAuthHeader() const
	{
		FString const AccessToken = GetAccessToken();
		
		if (SessionState.load(std::memory_order_acquire) == ESessionState::Valid)
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
		FReadScopeLock ReadLock(ErrorOAuthMtx);
		return ErrorOAuth;
	}

	void BaseCredentials::SetErrorOAuth(const FErrorOAuthInfo& NewErrorOAuthInfo)
	{
		FWriteScopeLock WriteLock(ErrorOAuthMtx);
		ErrorOAuth = NewErrorOAuthInfo;
	}

	FString BaseCredentials::GetRefreshToken() const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
		return AuthToken.Refresh_token;
	}

	FString BaseCredentials::GetPlatformUserId() const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
		return AuthToken.Platform_user_id;
	}

	FString BaseCredentials::GetSimultaneousPlatformId() const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
		return AuthToken.Simultaneous_platform_id;
	}

	FString BaseCredentials::GetSimultaneousPlatformUserId() const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
		return AuthToken.Simultaneous_platform_user_id;
	}

	FString BaseCredentials::GetSimultaneousPlatformUserIdByPlatformName(const FString& PlatformName) const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
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
		FReadScopeLock ReadLock(AuthTokenMtx);
		return AuthToken.Display_name;
	}

	FString BaseCredentials::GetUniqueDisplayName() const
	{
		FReadScopeLock ReadLock(AuthTokenMtx);
		if (!AuthToken.Unique_display_name.IsEmpty())
		{
			return AuthToken.Unique_display_name;
		}
		return AuthToken.Display_name;
	}

	FString BaseCredentials::GetLinkingToken() const
	{
		FReadScopeLock ReadLock(ErrorOAuthMtx);
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
			FWriteScopeLock WriteLock(AuthTokenMtx);
			AuthToken = InAuthToken;
		}
		ExpireDuration.store(AuthToken.Expires_in, std::memory_order_release);

		SetSessionState(ESessionState::Valid);

		CalculateBackoffRetry(CurrentTime);
		return true;
	}

	void BaseCredentials::CalculateBackoffRetry(float CurrentTime)
	{
		double ExpireDurationBuf = ExpireDuration.load(std::memory_order_acquire); 
		double RefreshWindowBuf = ExpireDurationBuf * GetRefreshWindowPercentage();

		double MinWindowTime = GetMinRefreshWindow();
		double MaxWindowTime = GetMaxRefreshWindow();
		if (RefreshWindowBuf <= MinWindowTime)
		{
			RefreshWindowBuf = MinWindowTime;
		}
		else if (RefreshWindowBuf >= MaxWindowTime)
		{
			RefreshWindowBuf = MaxWindowTime;
		}

		ExpireTime.store(CurrentTime + ExpireDurationBuf, std::memory_order_release);
		RefreshTime.store(ExpireTime.load(std::memory_order_acquire) - RefreshWindowBuf, std::memory_order_release);
		RefreshBackoff.store(RefreshWindowBuf * InitialBackoffPercentage, std::memory_order_release);
		RefreshWindow.store(RefreshWindowBuf, std::memory_order_release);
	}
	
	void BaseCredentials::PollRefreshToken(double CurrentTime)
	{
		ESessionState CurrentState = SessionState.load(std::memory_order_acquire);
#ifdef ACCELBYTE_ACTIVATE_PROFILER
		TRACE_CPUPROFILER_EVENT_SCOPE_STR(TEXT("AccelBytePollRefreshToken"));
#endif
		switch (CurrentState)
		{
		case ESessionState::Expired:
		case ESessionState::Rejected:
		case ESessionState::Valid:
			if (RefreshTime.load(std::memory_order_acquire) <= CurrentTime)
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
		double RefreshTimeBuf = NextRefreshTime;
		double ExpireTimeBuf = ExpireTime.load(std::memory_order_acquire);

		if (RefreshTimeBuf > ExpireTimeBuf)
		{
			RefreshTime.store(ExpireTimeBuf, std::memory_order_release);
		}
		else
		{
			RefreshTime.store(RefreshTimeBuf, std::memory_order_release);
		}
		SetSessionState(ESessionState::Expired);
	}

	void BaseCredentials::CalculateNextRefreshToken()
	{
		double CurrentTime = FPlatformTime::Seconds();
		double RefreshBackoffBuf = RefreshBackoff.load(std::memory_order_acquire);
		double NextRefreshTime = CurrentTime + RefreshBackoffBuf;
		if (NextRefreshTime < ExpireTime.load(std::memory_order_acquire) + GetExpireTimeBuffer())
		{
			ScheduleRefreshToken(NextRefreshTime);
			RefreshBackoffBuf = RefreshBackoffBuf * BackoffMultiplier;
			RefreshBackoff.store(RefreshBackoffBuf, std::memory_order_release);
		}
		else
		{
			SetSessionState(ESessionState::Invalid);
		}
	}

	void BaseCredentials::SetSessionState(ESessionState NewState)
	{
		SessionState.store(NewState, std::memory_order_release);
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
		return ExpireTime.load(std::memory_order_acquire);
	}

	void BaseCredentials::SetExpireTime(double InExpireTime)
	{
		ExpireTime.store(InExpireTime, std::memory_order_release);
	}

	double BaseCredentials::GetRefreshTime() const
	{
		return RefreshTime.load(std::memory_order_acquire);
	}

	void BaseCredentials::SetRefreshTime(double InRefreshTime)
	{
		RefreshTime.store(InRefreshTime, std::memory_order_release);
	}

	double BaseCredentials::GetExpireDuration() const
	{
		return ExpireDuration.load(std::memory_order_acquire);
	}

	void BaseCredentials::SetExpireDuration(double InExpireDuration)
	{
		ExpireDuration.store(InExpireDuration, std::memory_order_release);
	}

	double BaseCredentials::GetRefreshWindow() const
	{
		return RefreshWindow.load(std::memory_order_acquire);
	}

	double BaseCredentials::GetRefreshBackoffTime() const
	{
		return RefreshBackoff.load(std::memory_order_acquire);
	}

	void BaseCredentials::SetTickerDelegate(FTickerDelegate const& InDelegate)
	{
		FWriteScopeLock WriteLock(PollRefreshTokenHandleMtx);
		if (!PollRefreshTokenHandle.IsValid())
		{
			PollRefreshTokenHandle = FTickerAlias::GetCoreTicker().AddTicker(InDelegate, 0.2f);
		}
	}
}