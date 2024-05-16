// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Core/AccelByteBaseCredentials.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteServerCredentials.generated.h"

namespace AccelByte
{

/**
 * @brief Singleston class for storing server credentials.
 */
class ACCELBYTEUE4SDK_API ServerCredentials
	: public BaseCredentials
	, public TSharedFromThis<ServerCredentials, ESPMode::ThreadSafe>
{
public:
	using BaseCredentials::SetClientCredentials;

	ServerCredentials();

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
	
	static const FString DefaultSection;

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

