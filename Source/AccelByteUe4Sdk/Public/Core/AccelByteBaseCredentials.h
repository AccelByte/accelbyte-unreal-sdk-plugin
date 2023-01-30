// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteErrorModels.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteDefines.h"

namespace AccelByte
{

/**
 * @brief Singleton class for storing credentials.
 */
class ACCELBYTEUE4SDK_API BaseCredentials
{
public:
	enum class ESessionState
	{
		Invalid,
		Expired,
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

	virtual void PollRefreshToken(double CurrentTime) = 0;
	virtual void ScheduleRefreshToken(double NextRefreshTime) = 0;

	virtual void Startup() =0;
	virtual void Shutdown() = 0;

	const FString& GetOAuthClientId() const;
	const FString& GetOAuthClientSecret() const;
	virtual const FString& GetAccessToken() const = 0;
	virtual const FString& GetNamespace() const = 0;
	ESessionState GetSessionState() const;
	virtual const FString& GetUserId() const = 0;

protected:
	FString ClientId;
	FString ClientSecret;
	FErrorOAuthInfo ErrorOAuth; 
	
	double UserSessionExpire;
	ESessionState SessionState;

	double RefreshTime;
	double ExpireTime;
	double RefreshBackoff;

	FDelegateHandleAlias PollRefreshTokenHandle;
};

typedef TSharedRef<BaseCredentials, ESPMode::ThreadSafe> FBaseCredentialsRef;
typedef TSharedPtr<BaseCredentials, ESPMode::ThreadSafe> FBaseCredentialsPtr;	

} // Namespace AccelByte
