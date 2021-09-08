// Copyright (c) 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "UserTestAdmin.h"

#include "Api/AccelByteUserApi.h"

void AdminDeleteUser(const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserMapSuccess = false;
	FUserMapResponse userMap;
	AdminGetUserMap(UserId,
		THandler<FUserMapResponse>::CreateLambda([&userMap, &bGetUserMapSuccess](const FUserMapResponse& Result)
			{
				userMap = Result;
				bGetUserMapSuccess = true;
			}), OnError);
	WaitUntil(bGetUserMapSuccess, "Wait for getting user map data...");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s"), *GetAdminBaseUrl(), *userMap.Namespace, *userMap.userId);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminDeleteUserProfile(const FString& Namespace, const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/basic/v1/admin/namespaces/%s/users/%s/profiles"), *GetAdminBaseUrl(), *Namespace, *UserId);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetUserMap(const FString& userId, const THandler<FUserMapResponse>& OnSuccess, const FErrorHandler& OnError)
{
	check(!userId.IsEmpty());
	UE_LOG(LogAccelByteTest, Log, TEXT("-----------------USER ID: %s---------------------"), *userId);
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/justice/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *userId, *GetPublisherNamespace());
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetUserVerificationCode(const FString& userId, const THandler<FVerificationCode>& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserMapSuccess = false;
	FUserMapResponse userMap;
	AdminGetUserMap(userId,
		THandler<FUserMapResponse>::CreateLambda([&userMap, &bGetUserMapSuccess](const FUserMapResponse& Result)
			{
				userMap = Result;
				bGetUserMapSuccess = true;
			}), OnError);
	WaitUntil(bGetUserMapSuccess, "Wait for getting user map data...");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/v3/admin/namespaces/%s/users/%s/codes"), *GetAdminBaseUrl(), *userMap.Namespace, *userMap.userId);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminGetUserByEmailAddress(const FString& EmailAddress, const THandler<FUserResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/v3/admin/namespaces/%s/users/search?query=%s&limit=2"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *FGenericPlatformHttp::UrlEncode(EmailAddress));
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(THandler<FUserSearchResponse>::CreateLambda([OnSuccess, OnError](FUserSearchResponse Users) {
		if (Users.Data.Num() == 1)
		{
			OnSuccess.ExecuteIfBound(Users.Data[0]);
		}
		else
		{
			OnError.ExecuteIfBound((int32)ErrorCodes::StatusNotFound, TEXT("User not found"));
		}
		}), OnError), FPlatformTime::Seconds());
}

void AdminDeleteUserByEmailAddress(const FString& EmailAddress, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserSuccess = false;
	bool bGetUserDone = false;
	FUserResponse userData;
	AdminGetUserByEmailAddress(EmailAddress,
		THandler<FUserResponse>::CreateLambda([&userData, &bGetUserSuccess, &bGetUserDone](const FUserResponse& Result)
			{
				userData = Result;
				bGetUserDone = true;
				bGetUserSuccess = true;
			}),
		FErrorHandler::CreateLambda([OnError, &bGetUserDone](int32 ErrorCode, const FString& Message) {
				bGetUserDone = true;
				OnError.Execute(ErrorCode, Message);
			}));
	WaitUntil(bGetUserDone, "Wait for getting user map data...");

	if (bGetUserSuccess)
	{
		AdminDeleteUser(userData.UserId, OnSuccess, OnError);
	}
}

void AdminBanUser(const FString& userId, const FBanRequest& requestBody, const THandler<FBanResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/v3/admin/namespaces/%s/users/%s/bans"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *userId);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(requestBody, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void AdminBanUserChangeStatus(const FString& userId, const FString& banId, bool enabled, const THandler<FBanResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/v3/admin/namespaces/%s/users/%s/bans/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *userId, *banId);
	FString Content = FString::Printf(TEXT("{\"enabled\": %s}"), enabled ? TEXT("true") : TEXT("false"));

	AB_HTTP_PATCH(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

bool RegisterTestUser(const FTestUser& InTestUser)
{
	bool bIsDone = false;
	bool bIsOk = false;

	FRegistry::User.Register(
		InTestUser.Email,
		InTestUser.Password,
		InTestUser.DisplayName,
		InTestUser.Country,
		InTestUser.DateOfBirth,
		THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
			{
				bIsOk = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](int32 Code, FString Message)
			{
				if (static_cast<ErrorCodes>(Code) == ErrorCodes::UserEmailAlreadyUsedException)
				{
					bIsOk = true;
				}
				bIsDone = true;
			}));

	WaitUntil(bIsDone, TEXT("Waiting ..."));

	return bIsOk;
}

bool LoginTestUser(FTestUser& TestUser)
{
	AccelByte::Api::User UserApi(
		TestUser.Credentials,
		FRegistry::Settings,
		FRegistry::HttpRetryScheduler);
	
	bool bIsDone = false;
	bool bIsOk = false;
	
	UserApi.LoginWithUsername(
		TestUser.Email,
		TestUser.Password,
		FVoidHandler::CreateLambda([&]()
			{
				bIsOk = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](int32 ErrorCode, const FString& ErrorMessage)
			{
				bIsDone = true;
			}));

	WaitUntil(bIsDone, TEXT("Waiting ..."));

	return bIsOk;
}

bool DeleteTestUser(FTestUser& TestUser)
{
	bool bIsDone = false;
	bool bIsOk = false;
		
	AdminDeleteUser(TestUser.Credentials.GetUserId(),
		FSimpleDelegate::CreateLambda([&]()
			{
				bIsOk = true;
				bIsDone = true;
			}),
		FErrorHandler::CreateLambda([&](int32 ErrorCode, const FString& ErrorMessage)
			{
				bIsDone = true;
			}));
	WaitUntil(bIsDone, TEXT("Waiting ..."));
		
	TestUser.Credentials.ForgetAll();

	return bIsOk;
}

void GetUserMyAccountData(const FString& JsonWebToken, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *JsonWebToken);
	FString Url = FString::Printf(TEXT("%s/iam/v3/public/users/me"), *FRegistry::Settings.BaseUrl);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

bool SetupTestUsers(const int32 InNumOfTestUsers, TArray<FTestUser>& OutTestUsers)
{
	OutTestUsers.Empty();

	for (int i = 0; i < InNumOfTestUsers; i++)
	{
		OutTestUsers.Add(FTestUser(i));
	}

	for (int i = 0; i < InNumOfTestUsers; i++)
	{
		if (!RegisterTestUser(OutTestUsers[i]))
		{
			return false; // Stop immediately on the first register error
		}
	}

	for (int i = 0; i < InNumOfTestUsers; i++)
	{
		if (!LoginTestUser(OutTestUsers[i]))
		{
			return false; // Stop immediately on the first login error
		}
	}

	return true;
}

bool TeardownTestUsers(TArray<FTestUser>& InTestUsers)
{
	bool bIsOk = true;
	
	for (int i = 0; i < InTestUsers.Num(); i++)
	{
		if (!DeleteTestUser(InTestUsers[i]))
		{
			bIsOk = false; // One or more errors has happened but try to continue deleting the remaining test users
		}
	}

	return bIsOk;
}