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
		TokenRefreshedEvent.Clear();
	}

	void BaseCredentials::ForgetAll()
	{
		UserSessionExpire = 0;
		RefreshBackoff = 0.0;
		RefreshTime = 0.0;
		ExpireTime = 0.0;
		SessionState = ESessionState::Invalid;
	}

	void BaseCredentials::SetClientCredentials(const FString& InClientId, const FString& InClientSecret)
	{
		ClientId = InClientId;
		ClientSecret = InClientSecret;
	}

	BaseCredentials::FTokenRefreshedEvent& BaseCredentials::OnTokenRefreshed()
	{
		return TokenRefreshedEvent;
	}

	const FString& BaseCredentials::GetOAuthClientId() const
	{
		return ClientId;
	}

	const FString& BaseCredentials::GetOAuthClientSecret() const
	{
		return ClientSecret;
	}

	BaseCredentials::ESessionState BaseCredentials::GetSessionState() const
	{
		return SessionState;
	}

	const TMap<FString, FString> BaseCredentials::GetAuthHeader() const
	{
		if (SessionState == ESessionState::Valid)
		{
			FString const AccessToken = GetAccessToken();
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