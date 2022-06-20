// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteOauth2Models.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteDefines.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "AccelByteCredentials.generated.h"

namespace AccelByte
{

// forward declaration
class FHttpRetryScheduler;
	
/**
 * @brief Singleston class for storing credentials.
 */
class ACCELBYTEUE4SDK_API Credentials
{
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnLoginSuccessDelegate, const FOauth2Token& /*Response*/);
	DECLARE_MULTICAST_DELEGATE(FRefreshTokenAdditionalActions);
	DECLARE_EVENT(Credentials, FTokenRefreshedEvent);

public:
	enum class ESessionState
	{
		Invalid,
		Expired,
		Refreshing,
		Valid,
	};

public:
	Credentials();
	~Credentials();

	/** @brief The user was just authed: At this point, Credential auth tokens are already set. */
	FOnLoginSuccessDelegate& OnLoginSuccess();
	
	/** @brief Forgets post-auth info, but pre-auth (such as setting email) will remain. */
	void ForgetAll();
	void SetClientCredentials(const FString& InClientId, const FString& InClientSecret);
	void SetClientCredentials(const ESettingsEnvironment Environment);
	void SetAuthToken(const FOauth2Token NewAuthToken, float CurrentTime);
	void SetUserEmailAddress(const FString& EmailAddress);
	void PollRefreshToken(double CurrentTime);
	void ScheduleRefreshToken(double NextRefreshTime);
	void SetBearerAuthRejectedHandler(FHttpRetryScheduler& InHttpRef);
	void SetErrorOAuth(const FErrorOauthInfo ErrorOAuthInfo);

	FTokenRefreshedEvent& OnTokenRefreshed();

	const FString& GetOAuthClientId() const;
	const FString& GetOAuthClientSecret() const;

	const FOauth2Token& GetAuthToken() const;
	const FString& GetRefreshToken() const;
	const FString& GetAccessToken() const;
	const FString& GetUserId() const;
	const FString& GetPlatformUserId() const;
	const FString& GetUserDisplayName() const;
	const FString& GetNamespace() const;
	const FString& GetUserEmailAddress() const;
	const FString& GetLinkingToken() const; 
	
	ESessionState GetSessionState() const;
	bool IsSessionValid() const;
	bool IsComply() const;

	void Startup();
	void Shutdown();

private:
	FString ClientId;
	FString ClientSecret;
	FOauth2Token AuthToken;
	FErrorOauthInfo ErrorOAuth; 
	
	double UserSessionExpire;
	FString UserEmailAddress;
	ESessionState UserSessionState;

	double UserRefreshTime;
	double UserExpiredTime;
	double UserRefreshBackoff;

	FDelegateHandleAlias PollRefreshTokenHandle;
	FRefreshTokenAdditionalActions RefreshTokenAdditionalActions;
	FTokenRefreshedEvent TokenRefreshedEvent;
	FOnLoginSuccessDelegate LoginSuccessDelegate;

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

