// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteBaseCredentials.h"
#include "Models/AccelByteOauth2Models.h"
#include "Models/AccelByteErrorModels.h"
#include "Models/AccelByteUserModels.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteCredentials.generated.h"

namespace AccelByte
{

// forward declaration
class FHttpRetryScheduler;
	
/**
 * @brief Singleston class for storing credentials.
 */
class ACCELBYTEUE4SDK_API Credentials : public BaseCredentials
{
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoginSuccessDelegate, const FOauth2Token& /*Response*/);
	DECLARE_MULTICAST_DELEGATE_OneParam(FRefreshTokenAdditionalActions, bool);
	DECLARE_EVENT_OneParam(Credentials, FTokenRefreshedEvent, bool);

public:
	using BaseCredentials::SetClientCredentials;

	Credentials();
	virtual ~Credentials();

	/** @brief The user was just authed: At this point, Credential auth tokens are already set. */
	FOnLoginSuccessDelegate& OnLoginSuccess();
	
	/** @brief Forgets post-auth info, but pre-auth (such as setting email) will remain. */
	virtual void ForgetAll() override;
	virtual void SetClientCredentials(const ESettingsEnvironment Environment) override;
	void SetAuthToken(const FOauth2Token& NewAuthToken, float CurrentTime);
	void SetUserEmailAddress(const FString& EmailAddress);
	virtual void PollRefreshToken(double CurrentTime) override;
	virtual void ScheduleRefreshToken(double NextRefreshTime) override;
	void SetBearerAuthRejectedHandler(FHttpRetryScheduler& InHttpRef);
	void SetErrorOAuth(const FErrorOAuthInfo& ErrorOAuthInfo);
	void SetAccountUserData(const FAccountUserData& InAccountUserData);
	
	FTokenRefreshedEvent& OnTokenRefreshed();

	const FOauth2Token& GetAuthToken() const;
	const FString& GetRefreshToken() const;
	virtual const FString& GetAccessToken() const override;
	virtual const FString& GetUserId() const override;
	const FString& GetPlatformUserId() const;
	const FString& GetUserDisplayName() const;
	virtual const FString& GetNamespace() const override;
	const FString& GetUserEmailAddress() const;
	const FString& GetLinkingToken() const;
	const FAccountUserData& GetAccountUserData() const;
	
	bool IsSessionValid() const;
	bool IsComply() const;

	virtual void Startup() override;
	virtual void Shutdown() override;

private:
	FOauth2Token AuthToken;
	
	FRefreshTokenAdditionalActions RefreshTokenAdditionalActions;
	FTokenRefreshedEvent TokenRefreshedEvent;
	FOnLoginSuccessDelegate LoginSuccessDelegate;
	FAccountUserData AccountUserData;

	static const FString DefaultSection;

	void BearerAuthRejectedRefreshToken(FHttpRetryScheduler& InHttpRef);
};

typedef TSharedRef<Credentials, ESPMode::ThreadSafe> FCredentialsRef;
typedef TSharedPtr<Credentials, ESPMode::ThreadSafe> FCredentialsPtr;	

} // Namespace AccelByte


UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsCredentials : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetOAuthClientId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetOAuthClientSecret();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserSessionId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserDisplayName();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserNamespace();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserEmailAddress();
};

