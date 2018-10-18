// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
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
	const static FString ServerBaseUrl;
	const static FString ClientId;
	const static FString ClientSecret;
	const static FString GameId;
private:
	Settings() = delete; // static class can't have instance
	Settings(Settings const&) = delete;
	Settings(Settings&&) = delete;
};

} // Namespace AccelByte

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsSettings : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetSettingsServerBaseUrl();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetSettingsClientId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetSettingsClientSecret();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetSettingsGameId();
};

