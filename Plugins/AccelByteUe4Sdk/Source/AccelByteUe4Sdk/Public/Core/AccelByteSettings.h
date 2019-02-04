// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
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
	FString ClientId;
	FString ClientSecret;
	FString Namespace;
	FString PublisherNamespace;
    FString RedirectURI;
    FString BaseUrl;
	FString IamServerUrl;
	FString PlatformServerUrl;
	FString LobbyServerUrl;
	FString BasicServerUrl;
	FString CloudStorageServerUrl;
	FString GameProfileServerUrl;
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

    UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
    FString CloudStorageServerUrl;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString BasicServerUrl;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte | Settings")
	FString GameProfileServerUrl;
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
    static FString GetCloudStorageServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetBasicServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetGameProfileServerUrl();

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

    UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
    static void SetCloudStorageServerUrl(const FString& CloudStorageServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetBasicServerUrl(const FString& BasicServerUrl);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static void SetGameProfileServerUrl(const FString& GameProfileServerUrl);
};

