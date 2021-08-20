// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "TestUtilities.h"

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
