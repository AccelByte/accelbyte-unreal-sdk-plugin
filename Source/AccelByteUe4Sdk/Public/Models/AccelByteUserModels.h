// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Models/AccelByteGeneralModels.h"
#include "AccelByteUserModels.generated.h"

/** @brief Supported 2FA Authenticator Type by AccelByte IAM. */
UENUM(BlueprintType)
enum class EAccelByteLoginAuthFactorType : uint8
{
	Authenticator,
	BackupCode
};

/** @brief Supported platforms by AccelByte IAM. */
UENUM(BlueprintType)
enum class EAccelBytePlatformType : uint8
{
	Steam,
	PS4,
	PS4CrossGen,
	PS5,
	Live,
	Google,
	Apple,
	Facebook,
	Android,
	iOS,
	Device,
	Twitch,
	Oculus,
	Twitter,
	EpicGames,
	Stadia,
	AwsCognito,
	Discord,
	Nintendo,
	SnapChat,
	EAOrigin
};

UENUM(BlueprintType)
enum class EAccelByteSearchType : uint8
{
	ALL, DISPLAYNAME, USERNAME
};

UENUM(BlueprintType)
enum class EBanType : uint8
{
	LOGIN,
	CHAT_SEND,
	CHAT_ALL,
	ORDER_AND_PAYMENT,
	STATISTIC,
	LEADERBOARD,
	MATCHMAKING,
	UGC_CREATE_UPDATE
};

UENUM(BlueprintType)
enum class EBanReason : uint8
{
	VIOLENCE,
	HARASSMENT,
	HATEFUL_CONDUCT,
	OFFENSIVE_USERNAME,
	IMPERSONATION,
	MALICIOUS_CONTENT,
	SEXUALLY_SUGGESTIVE,
	SEXUAL_VIOLENCE,
	EXTREME_VIOLENCE,
	UNDERAGE_USER,
	CHEATING,
	TOS_VIOLATION
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAcceptedPolicies
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
	bool IsAccepted{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
	FString LocalizedPolicyVersionId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
	FString PolicyVersionId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Agreement | Models | AcceptAgreementRequest")
	FString PolicyId{};
	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FRegisterRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString AuthType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString EmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString Password{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString Country{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString DateOfBirth{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FRegisterRequestv2
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString AuthType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString EmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString Password{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString Country{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString DateOfBirth{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString Username{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FRegisterRequestv3
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	TArray<FAcceptedPolicies> AcceptedPolicies{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString AuthType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString EmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString Username{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString Password{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString Country{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateRequest")
	FString DateOfBirth{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FRegisterResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
	FString AuthType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
	FString EmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
	FString Country{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
	FString DateOfBirth{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserCreateResponse")
	FString Username{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUserUpdateRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
	FString Country{}; // Optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
	FString DateOfBirth{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
	FString DisplayName{}; // Optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
	FString EmailAddress{}; // Optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
	FString LanguageTag{}; // Optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
	FString Username{}; // Optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | UserUpdateRequest")
	FString AvatarUrl{}; // Optional
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBan
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Ban")
	FString Ban{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Ban")
	FString BanId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Ban")
	FString EndDate{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPermission
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
	int32 Action{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
	FString Resource{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
	int32 SchedAction{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
	FString SchedCron{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | Permission")
	TArray<FString> SchedRange{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccountUserData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString AuthType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	TArray<FBan> Bans{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString Country{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString CreatedAt{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString DateOfBirth{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	bool EmailVerified{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	bool Enabled{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString LastEnabledChangedTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString LoginId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	TArray<FPermission> Permissions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	bool PhoneVerified{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString PlatformId{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString PlatformUserId{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	TArray<FString> Roles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString Username{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString EmailAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString AvatarUrl{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FSimpleUserData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserProfile | Models | AccountUserData")
	FString Username{}; //optional
};

// backward compatibility with previous AccelByte SDK codes
// remove this if you are using FUserData from Unreal PhysicsCore
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION < 26
using FUserData = FAccountUserData{};
#endif

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FResetPasswordRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | ResetPasswordRequest")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | ResetPasswordRequest")
	FString EmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | ResetPasswordRequest")
	FString NewPassword{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPlatformLink
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
	FString EmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
	FString LinkedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
	FString OriginNamespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
	FString PlatformId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
	FString PlatformUserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkedPlatform")
	FString AccountGroup{};
};

UENUM(BlueprintType)
enum class EVerificationContext : uint8
{
	UserAccountRegistration,
	UpdateEmailAddress,
	upgradeHeadlessAccount //this is BE restriction. We should use small `u` in the beginning of this enum value
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FVerificationCodeRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | SendVerificationCodeRequest")
	EVerificationContext Context{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | SendVerificationCodeRequest")
	FString LanguageTag{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | SendVerificationCodeRequest")
	FString EmailAddress{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUpgradeAndVerifyRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
	FString Country{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
	FString DateOfBirth{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
	FString DisplayName{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
	FString EmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
	FString Password{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
	bool ReachMinimumAge{ true }; //optional. If user input DOB, BE will not check this field 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
	FString Username{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeHeadlessAccountWithVerificationCodeRequest")
	bool ValidateOnly{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPublicUserInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | GetPublicUserInfoRequest")
	FString UserName{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPagedPublicUsersInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | PagedPublicUsersInfoRequest")
	TArray<FPublicUserInfo> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | Oauth2 | Models | PagedPublicUsersInfoRequest")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPagedPlatformLinks
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | PagedLinkedPlatform")
	TArray<FPlatformLink> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | PagedLinkedPlatform")
	FAccelByteModelsPaging Paging{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBulkPlatformUserIdRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkPlatformUserIdRequest")
	TArray<FString> PlatformUserIDs{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPlatformUserIdMap
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | PlatformUserIdMap")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | PlatformUserIdMap")
	FString PlatformUserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | PlatformUserIdMap")
	FString PlatformId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBulkPlatformUserIdResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkPlatformUserIdResponse")
	TArray<FPlatformUserIdMap> UserIdPlatforms{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FCountryInfo
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | CountryInfo")
	FString CountryCode{};
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | CountryInfo")
	FString CountryName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | CountryInfo")
	FString State{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | CountryInfo")
	FString City{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUpgradeUserRequest
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeUserRequest")
	FString Temporary_session_id{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpgradeUserRequest")
	int32 Expires_in{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccountLinkedPlatform
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkedPlatform")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkedPlatform")
	FString PlatformUserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccountLinkPublisherAccount
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkPublisherAccount")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkPublisherAccount")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkPublisherAccount")
	TArray<FAccountLinkedPlatform> LinkedPlatforms{};	
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccountLinkConflictMessageVariables
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkConfictMessageVariables")
	FString PlatformUserID{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkConfictMessageVariables")
	TArray<FAccountLinkPublisherAccount> PublisherAccounts{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FAccountLinkConflictErrorJson
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkConflictErrorJson")
	FString ErrorCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkConflictErrorJson")
	FString ErrorMessage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | AccountLinkConflictErrorJson")
	FAccountLinkConflictMessageVariables MessageVariables;
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FLinkPlatformAccountRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkPlatformAccountRequest")
	FString PlatformId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | LinkPlatformAccountRequest")
	FString PlatformUserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUpdateEmailRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpdateEmailRequest")
	FString Code{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpdateEmailRequest")
	FString EmailAddress{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FJsonWebTokenResponse
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpdateEmailRequest")
	FString Jwt_token{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UpdateEmailRequest")
	FString Session_id{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FListBulkUserInfoRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkUserInfo")
	TArray<FString> UserIds{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBaseUserInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkUserInfo")
	FString AvatarUrl{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkUserInfo")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkUserInfo")
	TMap<FString, FString> PlatformUserIds{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkUserInfo")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkUserInfo")
	FString PublisherAvatarUrl{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FListBulkUserInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BulkUserInfo")
	TArray<FBaseUserInfo> Data{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FNamespaceRole
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | NamespaceRole")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | NamespaceRole")
	FString RoleId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUserOtherPlatformInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString AuthType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	TArray<FBan> Bans{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString Country{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString DateOfBirth{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	bool DeletionStatus{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString EmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	bool EmailVerified{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	bool Enabled{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString LastDateOfBirthChangedTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString LastEnabledChangedTime{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	TArray<FNamespaceRole> NamespaceRoles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString NewEmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString OldEmailAddress{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	TArray<FPermission> Permissions{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString PhoneNumber{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	bool PhoneVerified{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString PlatformDisplayName{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString PlatformId{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString PlatformUserId{}; //optional

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	TArray<FString> Roles{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString UserId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | GetUserOtherPlatform")
	FString Username{}; //optional
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FPagedUserOtherPlatformInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserOtherPlatformData")
	TArray<FUserOtherPlatformInfo> Data{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserOtherPlatformData")
	FAccelByteModelsPaging Paging{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | UserOtherPlatformData")
	int32 TotalData{};
};

USTRUCT(BlueprintType)
struct FValidationDescription 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	FString Language{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	TArray<FString> Message{};
};

USTRUCT(BlueprintType)
struct FValidation
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	bool AllowDigit{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	bool AllowLetter{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	bool AllowSpace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	bool AllowUnicode{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	FValidationDescription Description{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	bool IsCustomRegex{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	FString LetterCase{};	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	int32 MaxLength{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	int32 MaxRepeatingAlphaNum{};	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	int32 MaxRepeatingSpecialCharacter{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	int32 MinCharType{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	int32 MinLength{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	FString Regex{}; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	FString SpecialCharacterLocation{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	TArray<FString> SpecialCharacters{}; 	
};

USTRUCT(BlueprintType)
struct FDataInputValidation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	FString Field{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	FValidation Validation{};
};

USTRUCT(BlueprintType)
struct FInputValidation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	TArray<FDataInputValidation> Data{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | InputValidation")
	int32 Version{};
};


USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUser2FaBackupCode
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | User2FaBackupCode")
	int64 GeneratedAt{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | User2FaBackupCode")
	TArray<FString> InvalidCodes{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | User2FaBackupCode")
	TArray<FString> ValidCodes{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUser2FaMethod
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | User2FaMethod")
	FString Default{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | User2FaMethod")
	TArray<FString> Enabled{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FUser2FaSecretKey
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | User2FaSecretKey")
	FString SecretKey{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | User2FaSecretKey")
	FString Uri{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBanUserRequest
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUser")
	EBanType Ban{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUser")
	FString Comment{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUser")
	FString EndDate{}; // ISO 8601 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUser")
	EBanReason Reason{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUser")
	bool SkipNotif{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBannedBy
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BannedBy")
	FString DisplayName{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BannedBy")
	FString UserId{};
};

USTRUCT(BlueprintType)
struct ACCELBYTEUE4SDK_API FBanUserResponse 
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	FString Ban{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	FString BanId{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	FBannedBy BannedBy{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	FString Comment{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	FString CreatedAt{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	FString DisableDate{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	bool Enabled{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	FString EndDate{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	FString Namespace{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	EBanReason Reason{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AccelByte | UserManagement | Models | BanUserInfo")
	FString UserId{};
};
