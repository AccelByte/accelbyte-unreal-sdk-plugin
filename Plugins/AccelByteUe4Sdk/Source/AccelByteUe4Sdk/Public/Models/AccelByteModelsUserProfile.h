// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteModelsUserProfile.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString FirstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString LastName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString AvatarSmallUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString AvatarUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString AvatarLargeUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString Email;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString Status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString Language;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString Timezone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfo")
	FString DateOfBirth;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserCreateResponse
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserCreateResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserCreateResponse")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserCreateResponse")
		FString AuthType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserCreateResponse")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserCreateResponse")
		FString LoginId;
};

USTRUCT(BlueprintType)
struct FAccelByteModelsUserProfileInfoUpdate
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString FirstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString LastName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString AvatarSmallUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString AvatarUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString AvatarLargeUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString Email;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString Language;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString Timezone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileInfoUpdate")
	FString DateOfBirth;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString FirstName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString LastName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString Language;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString AvatarSmallUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString AvatarUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString AvatarLargeUrl;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString Email;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString Timezone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Models | UserProfile | UserProfileRequest")
		FString DateOfBirth;
};

