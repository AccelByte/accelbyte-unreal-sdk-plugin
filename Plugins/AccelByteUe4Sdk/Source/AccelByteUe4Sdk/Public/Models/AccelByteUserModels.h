// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteGeneralModels.h"
#include "AccelByteUserModels.generated.h"

UENUM(BlueprintType)
/**
 *  @brief Supported platforms by AccelByte IAM.
 */
enum class EAccelBytePlatformType : uint8
{
	Steam,
	Google,
	Facebook,
	Android,
	iOS,
	Device,
	Twitch,
	Oculus,
	Twitter
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FRegisterRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString AuthType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString EmailAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString Password;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString DateOfBirth;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FRegisterResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
		FString AuthType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
		FString EmailAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
		FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
		FString DateOfBirth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
		FString UserId;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUserUpdateRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString Country; // Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString DateOfBirth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString DisplayName; // Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString EmailAddress; // Optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString LanguageTag; // Optional
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBan
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Ban")
		FString Ban;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Ban")
		FString BanId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Ban")
		FString EndDate;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPermission
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
		int32 Action;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
		FString Resource;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
		int32 SchedAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
		FString SchedCron;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
		TArray<FString> SchedRange;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUserData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString AuthType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		TArray<FBan> Bans;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString DateOfBirth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		bool DeletionStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString EmailAddress; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		bool EmailVerified;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		bool Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString LastDateOfBirthChangedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString LastEnabledChangedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString NewEmailAddress; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString OldEmailAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		TArray<FPermission> Permissions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString PhoneNumber; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		bool PhoneVerified;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString PlatformId; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString PlatformUserId; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		TArray<FString> Roles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRespone")
		FString Username; //optional
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FResetPasswordRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | ResetPasswordRequest")
		FString Code;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | ResetPasswordRequest")
		FString EmailAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | ResetPasswordRequest")
		FString NewPassword;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPlatformLink
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

UENUM(BlueprintType)
enum class EVerificationContext : uint8
{
	UserAccountRegistration,
	UpdateEmailAddress,
	UpgradeHeadlessAccount
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FVerificationCodeRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | SendVerificationCodeRequest")
		EVerificationContext Context;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | SendVerificationCodeRequest")
		FString LanguageTag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | SendVerificationCodeRequest")
		FString LoginID;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUpgradeAndVerifyRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
		FString Code;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
		FString Password;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
		FString LoginId;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPublicUserInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
		FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
		FString DateOfBirth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
		FString PhoneNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
		FString EmailAddress;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPagedPublicUsersInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | PagedPublicUsersInfoRequest")
		TArray<FPublicUserInfo> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | PagedPublicUsersInfoRequest")
		FAccelByteModelsPaging Paging;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPagedPlatformLinks
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | PagedLinkedPlatform")
		TArray<FPlatformLink> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | PagedLinkedPlatform")
		FAccelByteModelsPaging Paging;
};