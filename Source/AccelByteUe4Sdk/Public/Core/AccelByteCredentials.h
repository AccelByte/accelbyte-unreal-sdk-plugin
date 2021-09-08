// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteOauth2Models.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
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
	DECLARE_MULTICAST_DELEGATE(FRefreshTokenAdditionalActions);

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
	void SetAuthToken(const FOauth2Token NewAuthToken, float CurrentTime);
	void SetUserEmailAddress(const FString& EmailAddress);
	void PollRefreshToken(double CurrentTime);
	void ScheduleRefreshToken(double NextRefreshTime);
	void SetBearerAuthRejectedHandler(FHttpRetryScheduler& HttpRef);

	const FOauth2Token& GetAuthToken() const;
	const FString& GetRefreshToken() const;
	const FString& GetAccessToken() const;
	const FString& GetUserId() const;
	const FString& GetPlatformUserId() const;
	const FString& GetUserDisplayName() const;
	const FString& GetNamespace() const;
	const FString& GetUserEmailAddress() const;
	ESessionState GetSessionState() const;

	void Startup();
	void Shutdown();

private:
	FString ClientId;
	FString ClientSecret;
	FOauth2Token AuthToken;
	
	double UserSessionExpire;
	FString UserEmailAddress;
	ESessionState UserSessionState;

	double UserRefreshTime;
	double UserExpiredTime;
	double UserRefreshBackoff;

	FDelegateHandle PollRefreshTokenHandle;
	FRefreshTokenAdditionalActions RefreshTokenAdditionalActions;

	void BearerAuthRejectedRefreshToken(FHttpRetryScheduler& HttpRef);
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

