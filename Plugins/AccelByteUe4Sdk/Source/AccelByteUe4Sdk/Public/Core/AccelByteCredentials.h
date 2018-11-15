// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteOauth2Models.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteCredentials.generated.h"

namespace AccelByte
{

/**
 * @brief Singleston class for storing credentials.
 */
class ACCELBYTEUE4SDK_API Credentials
{
public:
	static Credentials& Get();
	void ForgetAll();
	void SetUserToken(const FString& AccessToken, const FString& RefreshToken, const FDateTime& ExpirationUtc, const FString& Id, const FString& DisplayName, const FString& Namespace);
	void SetClientToken(const FString& AccessToken, const FDateTime& ExpirationUtc, const FString& Namespace);
	/**
	 * @brief Get stored access token.
	 */
	FString GetUserAccessToken() const;
	/**
	 * @brief Get stored refresh token; this is not set if you logged in with client credentials and you simply have to login with client credentials again to get new access token.
	 */
	FString GetUserRefreshToken() const;
	/**
	 * @brief Get access token expiration in UTC.
	 */
	FString GetClientAccessToken() const;
	FString GetClientNamespace() const;
	FDateTime GetUserAccessTokenExpirationUtc() const;
	FString GetUserId() const;
	FString GetUserDisplayName() const;
	FString GetUserNamespace() const;
private:
	Credentials(Credentials const&) = delete; // Copy constructor
	Credentials(Credentials&&) = delete; // Move constructor
	Credentials& operator=(Credentials const&) = delete; // Copy assignment operator
	Credentials& operator=(Credentials &&) = delete; // Move assignment operator

	FString ClientAccessToken;
	FString ClientNamespace;
	FString UserAccessToken;
	FString UserRefreshToken;
	FDateTime UserAccessTokenExpirationUtc;
	FString UserNamespace;
	FString UserId;
	FString UserDisplayName;
protected:
	Credentials();
	~Credentials();
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
	static FString GetUserRefreshToken();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FDateTime GetUserAccessTokenExpirationUtc();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserDisplayName();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Credentials")
	static FString GetUserNamespace();
};

