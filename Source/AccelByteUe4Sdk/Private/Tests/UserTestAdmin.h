// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"
#include "UserTestAdmin.generated.h"

USTRUCT(BlueprintType)
struct FUserMapResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserMapResponse")
		FString userId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserMapResponse")
		FString Namespace;
};

USTRUCT(BlueprintType)
struct FVerificationCode
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | VerificationCode")
		FString accountRegistration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | VerificationCode")
		FString accountUpgrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | VerificationCode")
		FString passwordReset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | VerificationCode")
		FString updateEmail;
};

USTRUCT(BlueprintType)
struct FUserResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString Country;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString DateOfBirth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		bool DeletionStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString EmailAddress; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		bool EmailVerified;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		bool Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		TArray<FString> Roles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserResponse")
		FString UserId;
};

USTRUCT(BlueprintType)
struct FUserSearchResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserSearchResponse")
		TArray<FUserResponse> Data;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | User | UserSearchResponse")
		FAccelByteModelsPaging Paging;
};

struct FTestUser
{
	FString FirstName = TEXT("John");
	FString LastName = TEXT("Doe");
	FString DateOfBirth;
	FString Country = TEXT("US");
	FString Language = TEXT("en");
	FString Timezone = TEXT("Etc/UTC");
	FString DisplayName;
	FString Email;
	FString Password = TEXT("Password123!");
	FString AvatarSmallUrl = TEXT("http://example.com/avatar/small.jpg");
	FString AvatarUrl = TEXT("http://example.com/avatar/normal.jpg");
	FString AvatarLargeUrl = TEXT("http://example.com/avatar/large.jpg");
	FString UserId;

	FTestUser(const int32 UserIndex = 0) : FTestUser(FGuid::NewGuid().ToString(EGuidFormats::Digits).Left(8) // Random unique ID
		, UserIndex)
	{}

	FTestUser(const FString& UniqueID, const int32 UserIndex = 0)
		: DateOfBirth((FDateTime::Today() - FTimespan::FromDays(365 * 21 + 7))
								.ToIso8601().Left(10)) // 21 years old as of today, date only
		, DisplayName(FString::Printf(TEXT("%s%s-%s-%02d"), 
								*FirstName, *LastName, *UniqueID, UserIndex).ToLower())
		, Email(FString::Printf(TEXT("%s-%s-%02d@example.com"), 
								TEXT("justice-unreal-sdk"), *UniqueID, UserIndex).ToLower())
	{}
};

USTRUCT(BlueprintType)
struct FTestAcceptedPolicy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsAccepted{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LocalizedPolicyVersionId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PolicyId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PolicyVersionId{};
};

USTRUCT(BlueprintType)
struct FTestUserV4
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FTestAcceptedPolicy> AcceptedPolicies{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AuthType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Country{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DateOfBirth{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DisplayName{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EmailAddress{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Password{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString PasswordMD5Sum{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Username{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Verified{};
};

USTRUCT(BlueprintType)
struct FCreateTestUserResponseV4
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString AuthType{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Country{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString DateOfBirth{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Displayname{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EmailAddress{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Namespace{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UserId{};
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Username{};
};

void AdminGetUserMap(const FString& UserId, const THandler<FUserMapResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminGetUserVerificationCode(const FString& UserId, const THandler<FVerificationCode>& OnSuccess, const FErrorHandler& OnError);
void AdminGetUserByEmailAddress(const FString& EmailAddress, const THandler<FUserResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUser(const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUserByEmailAddress(const FString& EmailAddress, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUserProfile(const FString& UserId,const FString& Namespace,const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminBanUserChangeStatus(const FString& userId, const FString& banId, bool enabled, const THandler<FBanUserResponse>& OnSuccess, const FErrorHandler& OnError);

void GetUserMyAccountData(const FString& JsonWebToken, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

bool RegisterTestUser(const FTestUser& InTestUser);
void RegisterTestUserV4(const FTestUserV4& InTestUser, const THandler<FCreateTestUserResponseV4>& OnSuccess, const FErrorHandler& OnError);
bool LoginTestUser(FTestUser& TestUser);
bool DeleteTestUser(FTestUser& InTestUser);

bool SetupTestUsers(const int32 InNumOfTestUsers, TArray<FTestUser>& OutTestUsers);
bool SetupTestUser(FTestUser& InTestUser);
bool TeardownTestUsers(TArray<FTestUser>& InTestUsers);
bool TeardownTestUser(FTestUser& InTestUser);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDCreateTestUserResponseV4, FCreateTestUserResponseV4, Response);
UCLASS(Blueprintable, BlueprintType)
class ACCELBYTEUE4SDK_API UUserTestAdminFunctions final : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void RegisterTestUserV4BP(const FTestUserV4& InTestUser, FDCreateTestUserResponseV4 OnSuccess, FDErrorHandler OnError);
};