// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteSettings.generated.h"

namespace AccelByte
{

class ACCELBYTEUE4SDK_API Settings
{
public:
	static FString ClientId;
	/**
	 * @brief Secret key!
	 * IETF does not recommend that you put your client secrets in "the the source code of the application or an associated resource bundle". Reference: RFC 6819 Section 5.2.3.1.
	 * @todo IAM, remove this.
	 */
	static FString ClientSecret;
	static FString GameId;
	static FString PublisherId;
	static FString IamServerUrl;
	static FString PlatformServerUrl;
	static FString LobbyServerUrl;
private:
	Settings() = delete; // Static class can't have instance
	Settings(Settings const&) = delete;
	Settings(Settings&&) = delete;
};

} // Namespace AccelByte

/**
 * @brief UObject for storing settings into configuration file.
 */
UCLASS(Config = Engine)
class ACCELBYTEUE4SDK_API UAccelByteSettings : public UObject
{
	GENERATED_BODY()
public:
	UAccelByteSettings();

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString ClientId;
	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString ClientSecret;
	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString GameId;
	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString PublisherId;
	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString IamServerUrl;
	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString PlatformServerUrl;
	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString LobbyServerUrl;
};


/**
 * @brief Get or update settings via blueprint.
 */
UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsSettings : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetClientId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetClientSecret();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetGameId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetPublisherId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetIamServerUrl();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetPlatformServerUrl();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetLobbyServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetClientId(const FString& ClientId);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetClientSecret(const FString& ClientSecret);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetGameId(const FString& GameId);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetPublisherId(const FString& PublisherId);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetIamServerUrl(const FString& IamServerUrl);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetPlatformServerUrl(const FString& PlatformServerUrl);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetLobbyServerUrl(const FString& LobbyServerUrl);
};

