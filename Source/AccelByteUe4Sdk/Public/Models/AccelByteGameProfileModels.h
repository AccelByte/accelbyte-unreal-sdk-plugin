// Copyright (c) 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteGameProfileModels.generated.h"

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicGameProfileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | PublicGameProfile")
	FString profileId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | PublicGameProfile")
	FString Namespace{}; // lower-case namespace keyword is reserved 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | PublicGameProfile")
	FString profileName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | PublicGameProfile")
	FString avatarUrl{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicGameProfile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | PublicGameProfile")
	TArray<FAccelByteModelsPublicGameProfileInfo> gameProfiles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | PublicGameProfile")
	FString userId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameProfile
{
    GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfile")
	FString profileId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfile")
	FString userId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfile")
	FString Namespace{}; // lower-case namespace keyword is reserved

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfile")
	FString profileName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfile")
	FString avatarUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfile")
	FString label{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfile")
	TArray<FString> tags{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfile")
	TMap<FString, FString> attributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameProfileRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfileRequest")
	TMap<FString, FString> attributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfileRequest")
	FString avatarUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfileRequest")
	FString label{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfileRequest")
	FString profileName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | GameProfileRequest")
	TArray<FString> tags{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsGameProfileAttribute
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | Attribute")
	FString name{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | GameProfile | Models | Attribute")
	FString value{};
};
