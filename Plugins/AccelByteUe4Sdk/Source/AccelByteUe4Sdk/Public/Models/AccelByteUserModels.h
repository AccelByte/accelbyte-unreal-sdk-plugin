// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#include "AccelByteGeneralModels.h"
#include "AccelByteUserModels.generated.h"

UENUM(BlueprintType)
/**
 *  @brief Supported platforms by AccelByte IAM.
 */
enum class EAccelBytePlatformType : uint8
{
	Steam,
	PS4,
	PS4CrossGen,
	PS5,
	Live,
	Google,
	Facebook,
	Android,
	iOS,
	Device,
	Twitch,
	Oculus,
	Twitter,
	EpicGames,
	Stadia
};

enum class EAccelByteSearchType : uint8
{
	ALL, EMAILADDRESS, DISPLAYNAME, USERNAME
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
struct ACCELBYTEUE4SDK_API FRegisterRequestv2
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
		FString Username;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
		FString Username;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
		FString Username; // Optional
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
struct ACCELBYTEUE4SDK_API FAccountUserData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString AuthType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		TArray<FBan> Bans;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString DateOfBirth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		bool DeletionStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString EmailAddress; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		bool EmailVerified;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		bool Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString LastDateOfBirthChangedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString LastEnabledChangedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString NewEmailAddress; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString OldEmailAddress;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		TArray<FPermission> Permissions;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString PhoneNumber; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		bool PhoneVerified;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString PlatformId; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString PlatformUserId; //optional
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		TArray<FString> Roles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
		FString Username; //optional
};

// backward compatibility with previous AccelByte SDK codes
// remove this if you are using FUserData from Unreal PhysicsCore
#if ENGINE_MINOR_VERSION < 26
using FUserData = FAccountUserData;
#endif

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
		FString AccountGroup;
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
		FString EmailAddress;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
		FString UserName;
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

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBulkPlatformUserIdRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkPlatformUserIdRequest")
		TArray<FString> PlatformUserIDs;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPlatformUserIdMap
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | PlatformUserIdMap")
		FString UserId;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBulkPlatformUserIdResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkPlatformUserIdResponse")
		TArray<FPlatformUserIdMap> UserIdPlatforms;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FCountryInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | CountryInfo")
        FString CountryCode;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | CountryInfo")
        FString CountryName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | CountryInfo")
        FString State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | CountryInfo")
        FString City;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUpgradeUserRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeUserRequest")
		FString Temporary_session_id;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeUserRequest")
		int32 Expires_in;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccountLinkedPlatform
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkedPlatform")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkedPlatform")
		FString PlatformUserId;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccountLinkPublisherAccount
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkPublisherAccount")
		FString UserId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkPublisherAccount")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkPublisherAccount")
		TArray<FAccountLinkedPlatform> LinkedPlatforms;

};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccountLinkConflictMessageVariables
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkConfictMessageVariables")
		FString PlatformUserID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkConfictMessageVariables")
		TArray<FAccountLinkPublisherAccount> PublisherAccounts;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FLinkPlatformAccountRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkPlatformAccountRequest")
		FString PlatformId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkPlatformAccountRequest")
		FString PlatformUserId;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUpdateEmailRequest
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpdateEmailRequest")
		FString Code;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpdateEmailRequest")
		FString EmailAddress;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FJsonWebTokenResponse
{
	GENERATED_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpdateEmailRequest")
		FString Jwt_token;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpdateEmailRequest")
		FString Session_id;
};