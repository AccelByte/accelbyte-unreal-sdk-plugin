// Copyright (c) 2021 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "UserTestAdmin.h"

#include "AccelByteUe4SdkModule.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteMultiRegistry.h"
#include "TestUtilities.h"

void AdminDeleteUser(const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserMapSuccess = false;
	bool bIsDone = false;
	FUserMapResponse userMap;
	
	AdminGetUserMap(UserId,
		THandler<FUserMapResponse>::CreateLambda(
			[&userMap, &bGetUserMapSuccess, &bIsDone](const FUserMapResponse& Result)
			{
				userMap = Result;
				bGetUserMapSuccess = true;
				bIsDone = true; // Stop waiting
			}), FErrorHandler::CreateLambda([&OnError, &bIsDone](const int32 ErrorCode, const FString& ErrorMessage)
			{
				bIsDone = true;
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
	
	WaitUntil(bIsDone, "Wait for getting user map data (AdminDeleteUser)...");

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
	if(userId.IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("User email address cannot be empty"));
		return;
	}
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
	WaitUntil(bGetUserMapSuccess, "Wait for getting user map data (AdminGetUserVerification)...");

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
	WaitUntil(bGetUserDone, "Wait for getting user map data (AdminDeleteUserByEmailAddress)...");

	if (bGetUserSuccess)
	{
		AdminDeleteUser(userData.UserId, OnSuccess, OnError);
	}
}

void AdminBanUserChangeStatus(const FString& userId, const FString& banId, bool enabled, const THandler<FBanUserResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/v3/admin/namespaces/%s/users/%s/bans/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *userId, *banId);
	FString Content = FString::Printf(TEXT("{\"enabled\": %s}"), enabled ? TEXT("true") : TEXT("false"));

	AB_HTTP_PATCH(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

bool RegisterTestUser(const FTestUser& InTestUser)
{
	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(InTestUser.Email);
	
	bool bIsDone = false;
	bool bIsOk = false;

	ApiClient->User.Register(InTestUser.Email
		, InTestUser.Password
		, InTestUser.DisplayName
		, InTestUser.Country
		, InTestUser.DateOfBirth
		, THandler<FRegisterResponse>::CreateLambda([&](const FRegisterResponse& Result)
		{
			bIsOk = true;
			bIsDone = true;
		})
		, FErrorHandler::CreateLambda([&](int32 Code, FString Message)
		{
			if (static_cast<ErrorCodes>(Code) == ErrorCodes::UserEmailAlreadyUsedException)
			{
				bIsOk = true;
			}
			bIsDone = true;
		})
	);

	WaitUntil(bIsDone, TEXT("Waiting to register user..."));

	return bIsOk;
}

void RegisterTestUserV4(const FTestUserV4& InTestUser, const THandler<FCreateTestUserResponseV4>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/v4/public/namespaces/%s/test_users"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace);
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(InTestUser, Content);
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

bool LoginTestUser(FTestUser& TestUser)
{
	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	
	bool bIsDone = false;
	bool bIsOk = false;
	
	ApiClient->User.LoginWithUsername(TestUser.Email
		, TestUser.Password
		, FVoidHandler::CreateLambda([&]()
		{ 
			bIsOk = true;
			bIsDone = true;
		})
		, FCustomErrorHandler::CreateLambda([&](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorJson)
		{
			bIsDone = true;
		})
	);

	WaitUntil(bIsDone, TEXT("Waiting to login with username ..."));
	
	TestUser.UserId = ApiClient->CredentialsRef->GetUserId();

	return bIsOk;
}

bool DeleteTestUser(FTestUser& TestUser)
{
	FApiClientPtr ApiClient = FMultiRegistry::GetApiClient(TestUser.Email);
	
	bool bIsDone = false;
	bool bIsOk = false;

	// No need to delete, if !valid UserId
	const FString UserId = ApiClient->CredentialsRef->GetUserId();
	if (UserId.IsEmpty())
	{
		// You probably want to know if it's a valid TestUser, but invalid UserId.
		UE_LOG(LogAccelByteTest, Display, TEXT("[DeleteTestUser] Valid TestUser, but invalid UserId"));
		
		ApiClient->CredentialsRef->ForgetAll();
		return true; // Technically, we did what we wanted to do!
	}	
	
	AdminDeleteUser(UserId,
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
		
	ApiClient->CredentialsRef->ForgetAll();

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
	bool bIsSuccess = true;
	OutTestUsers.Empty();

	for (int index = 0; index < InNumOfTestUsers; index++)
	{
		FTestUser TestUser{index};
		bIsSuccess = SetupTestUser(TestUser);
		OutTestUsers.Add(TestUser);
		
		if (!bIsSuccess)
		{
			break;
		}
	}

	return bIsSuccess;
}

bool SetupTestUser(FTestUser& InTestUser)
{
	if (!RegisterTestUser(InTestUser))
	{
		return false;
	}

	if (!LoginTestUser(InTestUser))
	{
		return false;
	}

	return true;
}

bool TeardownTestUsers(TArray<FTestUser>& InTestUsers)
{
	bool bIsOk = true;
	for (auto& TestUser : InTestUsers)
	{
		bIsOk = TeardownTestUser(TestUser);
	}

	return bIsOk;
}

bool TeardownTestUser(FTestUser& InTestUser)
{
	bool bIsOk = true;
	
	if (!DeleteTestUser(InTestUser))
	{
		bIsOk = false;
	}
	FMultiRegistry::RemoveApiClient(InTestUser.Email);
	
	return bIsOk;
}


void UUserTestAdminFunctions::RegisterTestUserV4BP(
	const FTestUserV4& InTestUser,
	FDCreateTestUserResponseV4 OnSuccess,
	FDErrorHandler OnError)
{
	RegisterTestUserV4(
		InTestUser,
		THandler<FCreateTestUserResponseV4>::CreateLambda(
			[OnSuccess](const FCreateTestUserResponseV4& Response){
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}
