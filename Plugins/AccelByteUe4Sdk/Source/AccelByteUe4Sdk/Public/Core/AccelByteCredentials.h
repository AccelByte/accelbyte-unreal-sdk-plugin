// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteOauth2Models.h"
#include "Runtime/Core/Public/Containers/Ticker.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "AccelByteCredentials.generated.h"

namespace AccelByte
{

/**
 * @brief Singleston class for storing credentials.
 */
class ACCELBYTEUE4SDK_API Credentials
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
	Credentials();

	void ForgetAll();
	void SetClientCredentials(const FString& ClientId, const FString& ClientSecret);
	void SetClientToken(const FString& AccessToken, double ExpiresIn, const FString& Namespace);
	void SetUserSession(const FString& SessionId, double ExpiredTime, const FString& RefreshId);
	void SetUserLogin(const FString& Id, const FString& DisplayName, const FString& Namespace);
	void SetUserEmailAddress(const FString& EmailAddress);
	void SetPlatformInfo(const FString& PlatformUserId);
	void PollRefreshToken(double CurrentTime);
	void ScheduleRefreshToken(double NextRefreshTime);
	const FString& GetUserRefreshId() const;
	/**
	 * @brief Get stored session id.
	 */
	const FString& GetUserSessionId() const;
	/**
	 * @brief Get access token expiration in UTC.
	 */
	const FString& GetClientAccessToken() const;
	const FString& GetClientNamespace() const;
	const FString& GetUserId() const;
	const FString& GetPlatformBoundUserId() const;
	const FString& GetUserDisplayName() const;
	const FString& GetUserNamespace() const;
	const FString& GetUserEmailAddress() const;
	ESessionState GetSessionState() const;

	void Startup();
	void Shutdown();

private:
	FString ClientId;
	FString ClientSecret;
	FString ClientAccessToken;
	FString ClientNamespace;
	
	FString UserSessionId;
	double UserSessionExpire;
	FString UserNamespace;
	FString UserId;
	FString UserDisplayName;
	FString UserEmailAddress;
	ESessionState UserSessionState;

	FString UserRefreshId;
	double UserRefreshTime;
	double UserExpiredTime;
	double UserRefreshBackoff;

	FDelegateHandle PollRefreshTokenHandle;

	FString PlatformBoundUserId;
};

} // Namespace AccelByte


UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsCredentials : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
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

