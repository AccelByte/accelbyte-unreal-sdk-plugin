// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUserManagementModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserCreateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString AuthType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString LoginId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString Password;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserCreateResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserCreateResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserCreateResponse")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserCreateResponse")
		FString AuthType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserCreateResponse")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserCreateResponse")
		FString LoginId;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserUpdateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString Country; // Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString EmailAddress; // Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString LanguageTag; // Optional
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPasswordResetRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | ResetPasswordRequest")
		FString Code;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | ResetPasswordRequest")
		FString LoginId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | ResetPasswordRequest")
		FString NewPassword;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsLinkedPlatform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
		FString EmailAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
		FString LinkedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
		FString OriginNamespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
		FString PlatformId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
		FString PlatformUserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
		FString UserId;
};
