// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "AccelByteUserProfileModels.generated.h"

UENUM(BlueprintType)
enum class EAccelByteFileType : uint8
{
	JPEG,
	JPG,
	PNG,
	BMP,
	GIF,
	MP3,
	BIN,
	WEBP
};

UENUM(BlueprintType)
enum class EAccelByteUploadCategory : uint8
{
	DEFAULT,
	REPORTING
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString FirstName{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString LastName{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString AvatarSmallUrl{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString AvatarUrl{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString AvatarLargeUrl{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString Status{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString DateOfBirth{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString Timezone{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	TMap<FString, FJsonObjectWrapper> CustomAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString ZipCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString PublicId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	TMap<FString, FJsonObjectWrapper> PrivateCustomAttributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsPublicUserProfileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString AvatarSmallUrl{};


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString AvatarUrl{};


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString AvatarLargeUrl{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	TMap<FString, FJsonObjectWrapper> CustomAttributes{}; 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString PublicId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfo")
	FString TimeZone{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileUpdateRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	FString FirstName{};


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	FString LastName{};


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	FString AvatarSmallUrl{};


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	FString AvatarUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	FString AvatarLargeUrl{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	FString Timezone{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	FString DateOfBirth{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	FString ZipCode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	TMap<FString, FJsonObjectWrapper>  PrivateCustomAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileInfoUpdate")
	TMap<FString, FJsonObjectWrapper>  CustomAttributes{};

	FAccelByteModelsUserProfileUpdateRequest() = default;

	FAccelByteModelsUserProfileUpdateRequest(const FAccelByteModelsUserProfileInfo& UserProfileInfo)
	{
		FirstName = UserProfileInfo.FirstName;
		LastName = UserProfileInfo.LastName;
		AvatarUrl = UserProfileInfo.AvatarUrl;
		AvatarSmallUrl = UserProfileInfo.AvatarSmallUrl;
		AvatarLargeUrl = UserProfileInfo.AvatarLargeUrl;
		Language = UserProfileInfo.Language;
		Timezone = UserProfileInfo.Timezone;
		DateOfBirth = UserProfileInfo.DateOfBirth;
		ZipCode = UserProfileInfo.ZipCode;
		PrivateCustomAttributes = UserProfileInfo.PrivateCustomAttributes;
		CustomAttributes = UserProfileInfo.CustomAttributes;
	}
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileCreateRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	FString FirstName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	FString LastName{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	FString AvatarSmallUrl{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	FString AvatarUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	FString AvatarLargeUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	FString Timezone{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	FString DateOfBirth{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	TMap<FString, FJsonObjectWrapper> CustomAttributes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileRequest")
	TMap<FString, FJsonObjectWrapper> PrivateCustomAttributes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccelByteModelsUserProfileUploadURLResult
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileUploadURL")
	FString Url{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileUploadURL")
	FString AccessUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileUploadURL")
	FString Method{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserProfileUploadURL")
	FString ContentType{};
};