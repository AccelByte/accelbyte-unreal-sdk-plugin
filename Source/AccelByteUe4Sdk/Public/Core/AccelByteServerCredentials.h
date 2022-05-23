// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteOauth2Models.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteDefines.h"
#include "Containers/Ticker.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine.h"
#include "AccelByteServerCredentials.generated.h"

namespace AccelByte
{

/**
 * @brief Singleston class for storing server credentials.
 */
class ACCELBYTEUE4SDK_API ServerCredentials
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
	ServerCredentials();

	void ForgetAll();
	void SetClientCredentials(const FString& InClientId, const FString& InClientSecret);
	void SetClientCredentials(const ESettingsEnvironment Environment);
	void SetClientToken(const FString& AccessToken, double ExpiresIn, const FString& Namespace);
	void ScheduleRefreshToken(double RefreshTime);
	void SetMatchId(const FString& GivenMatchId);

	void Startup();
	void Shutdown();
	void PollRefreshToken(double CurrentTime);

	const FString& GetOAuthClientId() const;
	const FString& GetOAuthClientSecret() const;
	
	/**
	 * @brief Get access token expiration in UTC.
	 */
	const FString& GetClientAccessToken() const;
	const FString& GetClientNamespace() const;
	const double GetExpireTime() const;
	const double GetRefreshTime() const;
	const FString& GetMatchId() const;
	ESessionState GetSessionState() const;

private:
	FString ClientId;
	FString ClientSecret;
	FString ClientAccessToken;
	FString ClientNamespace;

	double ClientExpireTime;
	double ClientRefreshTime;
	double ClientRefreshBackoff;

	FString MatchId;
	
	ESessionState ClientSessionState;
	static const FString DefaultSection;

	FDelegateHandleAlias PollRefreshTokenHandle;
	void RemoveFromTicker(FDelegateHandleAlias& handle);
};

typedef TSharedRef<ServerCredentials, ESPMode::ThreadSafe> FServerCredentialsRef;
typedef TSharedPtr<ServerCredentials, ESPMode::ThreadSafe> FServerCredentialsPtr;

} // Namespace AccelByte


UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsServerCredentials : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Credentials")
	static FString GetClientAccessToken();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Credentials")
	static FString GetClientNamespace();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Server | Credentials")
	static FString GetMatchId();
};

