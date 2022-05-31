// Copyright (c) 2018-2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Misc/DateTime.h"
#include "CoreMinimal.h"
#include "AccelByteGeneralModels.h"
#include "JsonObjectWrapper.h"
#include "AccelByteOauth2Models.generated.h"

// Emulate namespace with long class names

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FOauth2TokenPermission
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Permission")
	FString Resource{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Permission")
	int32 Action{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FOauth2TokenBan
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Ban")
	FString Ban{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token | Ban")
	FDateTime EndDate{0};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FOauth2Token
{
	// UE4 JSON parser is case-insensitive and we have to match the variable names against the backend's model.
	// But "namespace" is a reserved keyword so we capitalize the first character.
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Access_token{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Refresh_token{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Token_type{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	float Expires_in{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	bool Is_comply{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	TArray<FOauth2TokenPermission> Permissions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	TArray<FString> Roles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	TArray<FOauth2TokenBan> Bans{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString User_id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Display_name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Namespace{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Platform_id{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Token")
	FString Platform_user_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FOauth2Session
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Session")
	FString Session_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Session")
	int32 Expires_in{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | Oauth2Session")
	FString Refresh_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FJwkSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | JWKSet")
	TArray<FJsonObjectWrapper> keys{};
};