// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteOauth2Api.h"
#include "Core/AccelByteBaseCredentials.h"
#include "Kismet/BlueprintFunctionLibrary.h"


namespace AccelByte
{
/**
 * @brief Singleton class for storing server credentials.
 */
class ACCELBYTEUE4SDK_API ServerCredentials
	: public BaseCredentials
	, public TSharedFromThis<ServerCredentials, ESPMode::ThreadSafe>
{
private:
	FRWLock mutable CredentialAccessLock{};
	FRWLock mutable DelegateLock{};

public:
	using BaseCredentials::SetClientCredentials;

	ServerCredentials(FHttpRetryScheduler& InHttpRef, FString const& InIamServerUrl);
	virtual void ForgetAll() override;
	void SetClientToken(const FString& AccessToken, double ExpiresIn, const FString& Namespace);
	void SetMatchId(const FString& GivenMatchId);

	virtual void SetClientCredentials(const ESettingsEnvironment Environment) override;
	virtual void ScheduleRefreshToken(double RefreshTime) override;
	virtual void PollRefreshToken(double CurrentTime) override;

	virtual void Startup() override;
	virtual void Shutdown() override;

	const FString& GetClientAccessToken() const;
	virtual const FString& GetAccessToken() const override;
	const FString& GetClientNamespace() const;
	virtual const FString& GetNamespace() const override;
	const double GetExpireTime() const;
	const double GetRefreshTime() const;
	const FString& GetMatchId() const;
	virtual const FString& GetUserId() const override;

private:
	FString AccessToken;
	FString Namespace;
	FString MatchId;
	FString UserId;
	
	FString IamServerUrl;

	Api::Oauth2 Oauth;

	static TCHAR const* DefaultSection;

	void RemoveFromTicker(FDelegateHandleAlias& handle);
	void OnPollRefreshTokenResponseSuccess(const FOauth2Token& Result);
	void OnPollRefreshTokenResponseError(int32 Code, const FString& Message);
};

typedef TSharedRef<ServerCredentials, ESPMode::ThreadSafe> FServerCredentialsRef;
typedef TSharedPtr<ServerCredentials, ESPMode::ThreadSafe> FServerCredentialsPtr;

} // Namespace AccelByte