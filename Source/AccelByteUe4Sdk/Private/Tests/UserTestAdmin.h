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

struct FTestUser
{
	FString FirstName = TEXT("John");
	FString LastName = TEXT("Doe");
	FString DateOfBirth = TEXT("2000-01-01");
	FString Country = TEXT("US");
	FString Language = TEXT("en");
	FString Timezone = TEXT("Etc/UTC");
	FString DisplayName;
	FString Email;
	FString Password = TEXT("Password123!");
	FString AvatarSmallUrl = TEXT("http://example.com/avatar/small.jpg");
	FString AvatarUrl = TEXT("http://example.com/avatar/normal.jpg");
	FString AvatarLargeUrl = TEXT("http://example.com/avatar/large.jpg");

	FTestUser(const FString& TestUID, const int32 UserIndex = 0) :
		DisplayName(FString::Printf(TEXT("%s%s-%s-%02d"), *FirstName, *LastName, *TestUID, UserIndex).ToLower()),
		Email(FString::Printf(TEXT("%s_%s_%s_%02d@example.com"), TEXT("justice-ue4-sdk"), TEXT("test"), *TestUID, UserIndex).ToLower())
	{}
};

void AdminGetUserMap(const FString& userId, const THandler<FUserMapResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminGetUserVerificationCode(const FString& userId, const THandler<FVerificationCode>& OnSuccess, const FErrorHandler& OnError);
void AdminGetUserByEmailAddress(const FString& EmailAddress, const THandler<FUserResponse>& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUser(const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUserByEmailAddress(const FString& EmailAddress, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);
void AdminDeleteUserProfile(const FString& UserId,const FString& Namespace,const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError);

bool SetupTestUsers(const FString& InTestUID, const int32 InNumOfUsers, TArray<TSharedPtr<FTestUser>>& OutUsers);
bool SetupTestUsers(const FString& InTestUID, const int32 InNumOfUsers, TArray<TSharedPtr<FTestUser>>& OutUsers, TArray<TSharedPtr<Credentials>>& OutCredentials);
bool CheckTestUsers(const TArray<TSharedPtr<FTestUser>>& InUsers, const TArray<TSharedPtr<Credentials>>& InCredentials);
bool TearDownTestUsers(TArray<TSharedPtr<Credentials>>& InCredentials);
void GetUserMyAccountData(const FString& JsonWebToken, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);
