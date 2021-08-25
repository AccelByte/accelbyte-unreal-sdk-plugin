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

bool SetupTestUsers(const FString& InTestUID, const int32 InNumOfUsers, TArray<TSharedPtr<FTestUser>>& OutUsers)
{
	const auto Register = [](const FTestUser& InUser)
	{
		bool bIsDone = false;
		bool bIsOk = false;
		UE_LOG(LogAccelByteTest, Log, TEXT("%s: %s %s"), TEXT("SetupTestUsers"), TEXT("Registering"), *InUser.Email);
		FRegistry::User.Register(InUser.Email, InUser.Password, InUser.DisplayName, InUser.Country, InUser.DateOfBirth,
			THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
				{
					bIsOk = true;
					bIsDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 Code, FString Message)
				{
					if ((ErrorCodes)Code == ErrorCodes::UserEmailAlreadyUsedException)
					{
						bIsOk = true;
					}
					bIsDone = true;
				}));
		WaitUntil(bIsDone, TEXT("Waiting ..."));
		return bIsOk;
	};

	OutUsers.Empty();

	for (int i = 0; i < InNumOfUsers; i++)
	{
		OutUsers.Add(MakeShared<FTestUser>(InTestUID, i));
		if (!Register(*OutUsers[i]))
		{
			return false;
		}
	}

	return true;
}

bool SetupTestUsers(const FString& InTestUID, const int32 InNumOfUsers, TArray<TSharedPtr<FTestUser>>& OutUsers, TArray<TSharedPtr<Credentials>>& OutCredentials)
{
	const auto Login = [](const FTestUser& InUser, Credentials& OutCredentials)
	{
		bool bIsDone = false;
		bool bIsOk = false;
		UE_LOG(LogAccelByteTest, Log, TEXT("%s: %s"), TEXT("Logging in"), *InUser.Email);
		Api::User UserApi(OutCredentials, FRegistry::Settings, FRegistry::HttpRetryScheduler);
		UserApi.LoginWithUsername(InUser.Email, InUser.Password,
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
	};

	if (!SetupTestUsers(InTestUID, InNumOfUsers, OutUsers))
	{
		return false;
	}

	OutCredentials.Empty();

	for (int i = 0; i < InNumOfUsers; i++)
	{
		OutCredentials.Add(MakeShared<Credentials>());
		if (!Login(*OutUsers[i], *OutCredentials[i]))
		{
			return false;
		}
	}

	return true;
}

bool CheckTestUsers(const TArray<TSharedPtr<FTestUser>>& InUsers, const TArray<TSharedPtr<Credentials>>& InCredentials)
{
	bool bIsOk = true;

	if (InUsers.Num() == 0)
	{
		bIsOk = false;
	}

	if (InUsers.Num() != InCredentials.Num())
	{
		bIsOk = false;
	}

	for (const auto& Credentials : InCredentials)
	{
		if (Credentials->GetUserId().IsEmpty())
		{
			bIsOk = false;
		}
	}

	if (!bIsOk)
	{
		UE_LOG(LogAccelByteTest, Error, TEXT("%s: %s"), TEXT("CheckTestUsers"), TEXT("Test users are not ready. Maybe SetupTestUsers has not been invoked?"));
	}

	return bIsOk;
}

bool TearDownTestUsers(TArray<TSharedPtr<Credentials>>& InCredentials)
{
	const auto Delete = [](const Credentials& InCredentials)
	{
		bool bIsDone = false;
		bool bIsOk = false;
		UE_LOG(LogAccelByteTest, Log, TEXT("%s: %s %s"), TEXT("TearDownTestUsers"), TEXT("Deleting"), *InCredentials.GetUserId());
		AdminDeleteUser(InCredentials.GetUserId(),
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
		return bIsOk;
	};

	for (int i = 0; i < InCredentials.Num(); i++)
	{
		if (!Delete(*InCredentials[i]))
		{
			return false;
		}
		InCredentials[i]->ForgetAll();
	}

	return true;
}

void GetUserMyAccountData(const FString& JsonWebToken, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *JsonWebToken);
	FString Url = FString::Printf(TEXT("%s/iam/v3/public/users/me"), *FRegistry::Settings.BaseUrl);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
