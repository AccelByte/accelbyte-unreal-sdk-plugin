// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
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

USTRUCT(BlueprintType)
struct FBanRequest
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanRequest")
		EBanType Ban;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanRequest")
		FString Comment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanRequest")
		FString EndDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanRequest")
		EBanReason Reason;
};

USTRUCT(BlueprintType)
struct FBannedBy
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FString DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FString UserId;
};

USTRUCT(BlueprintType)
struct FBanResponse
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		EBanType Ban;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FString BanId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FBannedBy BannedBy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FString Comment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FString CreatedAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FString DisabledAt;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		bool Enabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FString EndDate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FString Namespace;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		EBanReason Reason;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test | Ban | BanResponse")
		FString UserId;
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
	Credentials Credentials;

	FTestUser(const int32 UserIndex = 0) : FTestUser(
		FGuid::NewGuid().ToString(EGuidFormats::Digits).Left(8), // Random unique ID
		UserIndex
		)
	{}

	FTestUser(const FString& UniqueID, const int32 UserIndex = 0) :
		DateOfBirth((FDateTime::Today() - FTimespan::FromDays(365 * 21 + 7))
		            .ToIso8601().Left(10)), // 21 years old as of today, date only
		DisplayName(FString::Printf(TEXT("%s%s-%s-%02d"), 
		                            *FirstName, *LastName, *UniqueID, UserIndex).ToLower()),
		Email(FString::Printf(TEXT("%s-%s-%02d@example.com"), 
		                      TEXT("justice-unreal-sdk"), *UniqueID, UserIndex).ToLower())
	{}
};

void AdminGetUserMap(const FString& userId, const THandler<FUserMapResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminGetUserVerificationCode(const FString& userId, const THandler<FVerificationCode>& OnSuccess, const FErrorHandler& OnError);
void AdminGetUserByEmailAddress(const FString& EmailAddress, const THandler<FUserResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUser(const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUserByEmailAddress(const FString& EmailAddress, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUserProfile(const FString& UserId,const FString& Namespace,const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminBanUser(const FString& userId, const FBanRequest& requestBody, const THandler<FBanResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminBanUserChangeStatus(const FString& userId, const FString& banId, bool enabled, const THandler<FBanResponse>& OnSuccess, const FErrorHandler& OnError);

void GetUserMyAccountData(const FString& JsonWebToken, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

bool RegisterTestUser(const FTestUser& InTestUser);
bool LoginTestUser(FTestUser& TestUser);
bool DeleteTestUser(FTestUser& InTestUser);

bool SetupTestUsers(const int32 InNumOfTestUsers, TArray<FTestUser>& OutTestUsers);
bool TeardownTestUsers(TArray<FTestUser>& InTestUsers);
