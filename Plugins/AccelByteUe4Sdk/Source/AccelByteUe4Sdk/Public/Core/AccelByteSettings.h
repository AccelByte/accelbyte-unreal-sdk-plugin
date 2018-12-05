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
	static FString ClientSecret;
	static FString Namespace;
	static FString PublisherNamespace;
    static FString RedirectURI;
    static FString BaseUrl;
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
	FString Namespace;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString PublisherNamespace;

    UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
    FString RedirectURI;

    UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
    FString BaseUrl;

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
	static FString GetNamespace();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetPublisherNamespace();

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
	static void SetNamespace(const FString& Namespace);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetPublisherNamespace(const FString& PublisherNamespace);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetIamServerUrl(const FString& IamServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetPlatformServerUrl(const FString& PlatformServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetLobbyServerUrl(const FString& LobbyServerUrl);
};

