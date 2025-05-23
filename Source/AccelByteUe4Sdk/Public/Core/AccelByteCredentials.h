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
	virtual ~Credentials() override;
	
	/** @brief The user was just authed: At this point, Credential auth tokens are already set. */
	FOnLoginSuccessDelegate& OnLoginSuccess();

	/** @brief The user was just Logged out : At this point, Credential auth tokens are already removed. */
	FOnLogoutSuccessDelegate& OnLogoutSuccess();
	
	/** @brief Forgets post-auth info, but pre-auth (such as setting email) will remain. */
	virtual void ForgetAll() override;
	virtual void SetClientCredentials(const ESettingsEnvironment Environment) override;
	void SetAuthToken(const FOauth2Token& NewAuthToken, float CurrentTime);
	void SetUserEmailAddress(const FString& EmailAddress);
	void SetUserName(const FString& UserName); // DEPRECATED
	void SetUserDisplayName(const FString& UserDisplayName);
	virtual void PollRefreshToken(double CurrentTime) override;
	virtual void ScheduleRefreshToken(double NextRefreshTime) override;
	void SetBearerAuthRejectedHandler(FHttpRetryScheduler& InHttpRef);
	void SetErrorOAuth(const FErrorOAuthInfo& ErrorOAuthInfo);
	void SetAccountUserData(const FAccountUserData& InAccountUserData);
	void SetThridPartyPlatformTokenData(const FString& PlatformId, const FThirdPartyPlatformTokenData& ThirdPartyPlatformTokenData);
	void ClearThridPartyPlatformTokenData();
		
	const FOauth2Token& GetAuthToken() const;
	const FString& GetRefreshToken() const;
	virtual const FString& GetAccessToken() const override;
	virtual const FString& GetUserId() const override;
	const FString& GetPlatformUserId() const;
	const FString& GetSimultaneousPlatformId() const;
	const FString& GetSimultaneousPlatformUserId() const;
	FString GetSimultaneousPlatformUserIdByPlatformName(const FString& PlatformName) const;
	const FString& GetUserDisplayName() const;
	virtual const FString& GetNamespace() const override;
	const FString& GetUserEmailAddress() const;
	const FString& GetUserName() const; // DEPRECATED
	const FString& GetUniqueDisplayName() const;
	const FString& GetLinkingToken() const;
	const FAccountUserData& GetAccountUserData() const;
	const TMap<FString, FThirdPartyPlatformTokenData>& GetThridPartyPlatformTokenData() const;

	bool IsSessionValid() const;
	bool IsComply() const;

	virtual void Startup() override;
	virtual void Shutdown() override;

private:
	FOauth2Token AuthToken;
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

	void OnRefreshTokenSuccessful(FOauth2TokenV4 const& Token);
	void OnRefreshTokenFailed(int32 ErrorCode
		, FString const& ErrorMessage
		, FErrorOAuthInfo const& OAuthError);
};

typedef TSharedRef<Credentials, ESPMode::ThreadSafe> FCredentialsRef;
typedef TSharedPtr<Credentials, ESPMode::ThreadSafe> FCredentialsPtr;

} // Namespace AccelByte