// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteServerSettings.generated.h"

namespace AccelByte
{

class ACCELBYTEUE4SDK_API ServerSettings
{
public:
	FString ClientId;
	FString ClientSecret;
	FString Namespace;
	FString PublisherNamespace;
    FString RedirectURI;
    FString BaseUrl;
	FString IamServerUrl;
	FString DSMServerUrl;
};

} // Namespace AccelByte

/**
 * @brief UObject for storing settings into configuration file.
 */
UCLASS(Config = Engine)
class ACCELBYTEUE4SDK_API UAccelByteServerSettings : public UObject
{
	GENERATED_BODY()
public:
	UAccelByteServerSettings();

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
	FString ClientId;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
	FString ClientSecret;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
	FString Namespace;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
	FString PublisherNamespace;

    UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
    FString RedirectURI;

    UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
    FString BaseUrl;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
	FString IamServerUrl;

	UPROPERTY(EditAnywhere, GlobalConfig, Category = "AccelByte Server | Settings")
	FString DSMServerUrl;
};


/**
 * @brief Get or update settings via blueprint.
 */
UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsServerSettings : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetClientId();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetClientSecret();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetNamespace();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetPublisherNamespace();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetIamServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static FString GetDSMServerUrl();

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetClientId(const FString& ClientId);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetClientSecret(const FString& ClientSecret);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetNamespace(const FString& Namespace);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetPublisherNamespace(const FString& PublisherNamespace);

	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetIamServerUrl(const FString& IamServerUrl);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte Server | Settings")
	static void SetDSMServerUrl(const FString& DSMServerUrl);
};

