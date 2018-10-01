// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "Misc/DateTime.h"
#include "AccelByteModelsIdentity.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOAuthTokenPermission
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken | Permission")
		FString Resource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken | Permission")
		int32 Action;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOAuthTokenBan
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken | Ban")
		FString Ban;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken | Ban")
		FDateTime EndDate;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsOAuthToken
{
	// UE4 JSON parser is case-insensitive and we have to match the variable names against the backend's model.
	// But "namespace" is a reserved keyword so we capitalize the first character.
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		FString Access_token;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		FString Refresh_token;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		FString Token_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		float Expires_in;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		TArray<FAccelByteModelsOAuthTokenPermission> Permissions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		TArray<FString> Roles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		TArray<FAccelByteModelsOAuthTokenBan> Bans;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		FString User_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		FString Display_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | OAuthToken")
		FString Namespace;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | UserCreateRequest")
		FString AuthType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | UserCreateRequest")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | UserCreateRequest")
		FString LoginId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | UserCreateRequest")
		FString Password;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsResetPasswordRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | ResetPasswordRequest")
		FString Code;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | ResetPasswordRequest")
		FString LoginId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | ResetPasswordRequest")
		FString NewPassword;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLinkedPlatform
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | LinkedPlatform")
		FString PlatformId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | LinkedPlatform")
		FString PlatformUserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | LinkedPlatform")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | Identity | LinkedPlatform")
		FString UserId;
};
