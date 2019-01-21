// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteOauth2Models.h"
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
	enum class ETokenState
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
	void SetUserToken(const FString& AccessToken, const FString& RefreshToken, double ExpiredTime, const FString& Id, const FString& DisplayName, const FString& Namespace);
	/**
	 * @brief Get stored access token.
	 */
	const FString& GetUserAccessToken() const;
	/**
	 * @brief Get stored refresh token; this is not set if you logged in with client credentials and you simply have to login with client credentials again to get new access token.
	 */
	const FString& GetUserRefreshToken() const;
	/**
	 * @brief Get access token expiration in UTC.
	 */
	const FString& GetClientAccessToken() const;
	const FString& GetClientNamespace() const;
	const FString& GetUserId() const;
	const FString& GetUserDisplayName() const;
	const FString& GetUserNamespace() const;
	ETokenState GetTokenState() const;
	
	void PollRefreshToken(double CurrentTime);
	void ScheduleRefreshToken(double NextRefreshTime);
	
	/**
	 * @brief Force to refresh access token.
	 */
	void ForceRefreshToken();

private:
	FString ClientId;
	FString ClientSecret;
	FString ClientAccessToken;
	FString ClientNamespace;
	
	FString UserAccessToken;
	FString UserRefreshToken;
	FString UserNamespace;
	FString UserId;
	FString UserDisplayName;		
	double UserRefreshTime;
	double UserExpiredTime;
	double UserRefreshBackoff;
	ETokenState UserTokenState;
};

} // Namespace AccelByte


UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsCredentials : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserAccessToken();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserDisplayName();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserNamespace();
};

