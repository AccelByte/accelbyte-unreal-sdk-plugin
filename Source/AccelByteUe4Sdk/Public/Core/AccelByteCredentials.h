// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteMessagingSystem.h"
#include "Core/AccelByteOauth2Api.h"
#include "Core/AccelByteBaseCredentials.h"
#include "Models/AccelByteOauth2Models.h"
#include "Models/AccelByteErrorModels.h"
#include "Models/AccelByteUserModels.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Kismet/BlueprintFunctionLibrary.h"

namespace AccelByte
{
	
// forward declaration
class FHttpRetryScheduler;
	
/**
 * @brief Singleston class for storing credentials.
 */
class ACCELBYTEUE4SDK_API Credentials
	: public BaseCredentials
	, public TSharedFromThis<Credentials, ESPMode::ThreadSafe>
{
private:
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoginSuccessDelegate, const FOauth2Token& /*Response*/);
	DECLARE_MULTICAST_DELEGATE_OneParam(FRefreshTokenAdditionalActions, bool);
	DECLARE_MULTICAST_DELEGATE(FOnLogoutSuccessDelegate);
	FRWLock mutable CredentialAccessLock{};
	FRWLock mutable DelegateLock{};

public:
	using BaseCredentials::SetClientCredentials;

	Credentials(FHttpRetryScheduler& InHttpRef, FAccelByteMessagingSystem& MessagingRef, FString const& IamServerUrl);
	virtual ~Credentials();
	
	/** @brief The user was just authed: At this point, Credential auth tokens are already set. */
	FOnLoginSuccessDelegate& OnLoginSuccess();

	/** @brief The user was just Logged out : At this point, Credential auth tokens are already removed. */
	FOnLogoutSuccessDelegate& OnLogoutSuccess();
	
	/** @brief Forgets post-auth info, but pre-auth (such as setting email) will remain. */
	virtual void ForgetAll() override;
	virtual void SetClientCredentials(const ESettingsEnvironment Environment) override;
	virtual bool SetAuthToken(const FOauth2Token& NewAuthToken, float CurrentTime) override;
	void SetUserEmailAddress(const FString& EmailAddress);
	void SetUserName(const FString& UserName); // DEPRECATED
	void SetUserDisplayName(const FString& UserDisplayName);
	void SetBearerAuthRejectedHandler(FHttpRetryScheduler& InHttpRef);
	void SetAccountUserData(const FAccountUserData& InAccountUserData);
	void SetThridPartyPlatformTokenData(const FString& PlatformId, const FThirdPartyPlatformTokenData& ThirdPartyPlatformTokenData);
	void ClearThridPartyPlatformTokenData();
		
	FString GetUserDisplayName() const;
	FString GetUserEmailAddress() const;
	FString GetUserName() const; // DEPRECATED
	FAccountUserData GetAccountUserData() const;
	const TMap<FString, FThirdPartyPlatformTokenData>& GetThridPartyPlatformTokenData() const;

	bool IsSessionValid() const;
	bool IsComply() const;

	virtual void Startup() override;

protected:
	Api::Oauth2 Oauth;
	
	FString UserName; // DEPRECATED
	FRefreshTokenAdditionalActions RefreshTokenAdditionalActions;
	FOnLoginSuccessDelegate LoginSuccessDelegate{};
	FOnLogoutSuccessDelegate LogoutSuccessDelegate{};
	FAccountUserData AccountUserData;
	TMap<FString, FThirdPartyPlatformTokenData> ThirdPartyPlatformTokenData;
	FAccelByteMessagingSystemWPtr MessagingSystemWPtr;

	FAccelByteTaskWPtr RefreshTokenTask;

	FDelegateHandle BearerAuthRejectedHandle;
	FString IamServerUrl;

	static TCHAR const* DefaultSection;

	void OnBearerAuthRejected(FHttpRetrySchedulerWPtr HttpWPtr);
	void OnBearerAuthRefreshed(bool bSuccessful, FHttpRetrySchedulerWPtr HttpWPtr);

	virtual void SendRefreshToken() override;
	void OnRefreshTokenSuccessful(FOauth2TokenV4 const& Token);
	void OnRefreshTokenFailed(int32 ErrorCode
		, FString const& ErrorMessage
		, FErrorOAuthInfo const& OAuthError);
};

typedef TSharedRef<Credentials, ESPMode::ThreadSafe> FCredentialsRef;
typedef TSharedPtr<Credentials, ESPMode::ThreadSafe> FCredentialsPtr;

} // Namespace AccelByte