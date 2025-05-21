// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Core/AccelByteBaseCredentials.h"

namespace AccelByte
{

	BaseCredentials::BaseCredentials()
		: ClientId()
		, ClientSecret()
		, UserSessionExpire(0)
		, SessionState(ESessionState::Invalid)
		, RefreshTime(0.0)
		, ExpireTime(0.0)
		, RefreshBackoff(0.0)
		, BackoffCount(0)
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
		UserSessionExpire = 0;
		RefreshBackoff = 0.0;
		RefreshTime = 0.0;
		ExpireTime = 0.0;
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

	BaseCredentials::ESessionState BaseCredentials::GetSessionState() const
	{
		FReadScopeLock ReadLock(CredentialAccessLock);
		return SessionState;
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

	bool BaseCredentials::Tick(float DeltaTime)
	{
		PollRefreshToken(FPlatformTime::Seconds());
		return true;
	}
}