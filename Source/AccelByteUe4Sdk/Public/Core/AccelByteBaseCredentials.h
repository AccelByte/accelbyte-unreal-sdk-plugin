// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteErrorModels.h"
#include "Models/AccelByteOauth2Models.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteDefines.h"

// STL
#include <atomic>

namespace AccelByte
{
/**
 * @brief Singleton class for storing credentials.
 */
class ACCELBYTEUE4SDK_API BaseCredentials
{
public:
	DECLARE_EVENT_OneParam(Credentials, FTokenRefreshedEvent, bool);

	enum class ESessionState
	{
		Invalid,
		Expired,
		Rejected,
		Refreshing,
		Valid,
	};

public:
	BaseCredentials();
	virtual ~BaseCredentials();
		
	/** @brief Forgets post-auth info, but pre-auth (such as setting email) will remain. */
	virtual void ForgetAll();

	void SetClientCredentials(const FString& InClientId, const FString& InClientSecret);
	virtual void SetClientCredentials(const ESettingsEnvironment Environment) = 0;

	virtual void PollRefreshToken(double CurrentTime);
	virtual void ScheduleRefreshToken(double NextRefreshTime);

	FTokenRefreshedEvent& OnTokenRefreshed();

	virtual void Startup();
	virtual void Shutdown();

	const FString& GetOAuthClientId() const;
	const FString& GetOAuthClientSecret() const;

	virtual bool SetAuthToken(FOauth2Token const& InAuthToken, float CurrentTime);
	virtual const FOauth2Token& GetAuthToken() const;
	virtual const FString& GetAccessToken() const;
	virtual const FString& GetNamespace() const;
	ESessionState GetSessionState() const;
	virtual const FString& GetUserId() const;
	const TMap<FString, FString> GetAuthHeader() const;

	virtual const FErrorOAuthInfo& GetErrorOAuth() const;
	virtual void SetErrorOAuth(const FErrorOAuthInfo& ErrorOAuthInfo);
	virtual FString GetRefreshToken() const;
	virtual FString GetPlatformUserId() const;
	virtual FString GetSimultaneousPlatformId() const;
	virtual FString GetSimultaneousPlatformUserId() const;
	virtual FString GetSimultaneousPlatformUserIdByPlatformName(const FString& PlatformName) const;
	virtual FString GetDisplayName() const;
	virtual FString GetUniqueDisplayName() const;
	virtual FString GetLinkingToken() const;

	virtual double GetMinExpireDuration() const;
	virtual double GetRefreshWindowPercentage() const;
	virtual double GetMinRefreshWindow() const;
	virtual double GetMaxRefreshWindow() const;
	virtual double GetExpireTimeBuffer() const;

	virtual double GetExpireTime() const;
	virtual void SetExpireTime(double InExpireTime);
	
	virtual double GetRefreshTime() const;
	virtual void SetRefreshTime(double InRefreshTime);
	
	virtual double GetExpireDuration() const;
	virtual void SetExpireDuration(double InExpireDuration);
	
	virtual double GetRefreshWindow() const;
	virtual double GetRefreshBackoffTime() const;

#if !UE_BUILD_SHIPPING
	void SetClientId(const FString& InClientId)
	{
		FWriteScopeLock WriteLock(ClientIdMtx);
		ClientId = InClientId;
	}
#endif

private:
	mutable FRWLock ClientIdMtx;
	FString ClientId;
	mutable FRWLock ClientSecretMtx;
	FString ClientSecret;

	mutable FRWLock AuthTokenMtx;
	FOauth2Token AuthToken;

	mutable FRWLock ErrorOAuthMtx;
	FErrorOAuthInfo ErrorOAuth; 
	
	std::atomic<ESessionState> SessionState;

	std::atomic<double> ExpireDuration;
	std::atomic<double> RefreshTime;
	std::atomic<double> ExpireTime;
	
	/** @brief RefreshWindow time in seconds. */
	std::atomic<double> RefreshWindow;
	
	/** @brief RefreshBackoff time in seconds. */
	std::atomic<double> RefreshBackoff;

	FRWLock PollRefreshTokenHandleMtx;
	FDelegateHandleAlias PollRefreshTokenHandle;

	FRWLock TokenRefreshedEventMtx;
	FTokenRefreshedEvent TokenRefreshedEvent;

protected:
	// #NOTE: Automatic Token Refresh coefficients

	/** @brief The minimum ExpireDuration in seconds. Current: 300 seconds */
	const double MinExpireDuration = 300.0f;
	
	/** @brief The percentage for RefreshWindow based on ExpireDuration value. Current: 25% */
	const double RefreshWindowPercentage = 0.25f;

	/** @brief Minimum RefreshWindow cap limit. Current: 180 seconds */
	const double MinRefreshWindow = 180.0f;

	/** @brief Maximum RefreshWindow cap limit. Current: 600 seconds */
	const double MaxRefreshWindow = 600.0f;

	/** @brief Buffer for Expire time. Current: 10 seconds */
	const double ExpireTimeBuffer = 10.0f;

	// #NOTE: Retry Backoff coefficients

	/** @brief Percentage value of RefreshWindow to get initial Backoff time. Current: 5% */
	const double InitialBackoffPercentage = 0.05f;

	/** @brief Multiplier for Backofftime from the previous value. Current: 2 */
	const double BackoffMultiplier = 2.0f;

	virtual bool Tick(float DeltaTime);

	void SetSessionState(ESessionState NewState);
	virtual void CalculateBackoffRetry(float CurrentTime);
	virtual void CalculateNextRefreshToken();
	virtual void SendRefreshToken() = 0;

	virtual void SetTickerDelegate(FTickerDelegate const& InDelegate);
};

typedef TSharedRef<BaseCredentials, ESPMode::ThreadSafe> FBaseCredentialsRef;
typedef TSharedPtr<BaseCredentials, ESPMode::ThreadSafe> FBaseCredentialsPtr;
typedef TWeakPtr<BaseCredentials, ESPMode::ThreadSafe> FBaseCredentialsWPtr;

} // Namespace AccelByte
