// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "TestUtilities.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteEnvironment.h"
#include "Api/AccelByteOauth2Api.h"
#include "Misc/Base64.h"

DEFINE_LOG_CATEGORY(LogAccelByteTest);

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Settings;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

void Waiting(bool& bCondition, FString Message, double TimeoutDelay)
{
	const double StartTime = FPlatformTime::Seconds();
	const double TimeoutSeconds = StartTime + TimeoutDelay;
	double LastTickTime = StartTime;

	while (!bCondition && (FPlatformTime::Seconds() < TimeoutSeconds))
	{
		FPlatformProcess::Sleep(.2f);
		UE_LOG(LogAccelByteTest, Log, TEXT("%s"), *Message);
		LastTickTime = FPlatformTime::Seconds();
		FHttpModule::Get().GetHttpManager().Tick(.2f);
		FTicker::GetCoreTicker().Tick(.2f);
	}

	if (FPlatformTime::Seconds() >= TimeoutSeconds)
	{
		UE_LOG(LogAccelByteTest, Error, TEXT("Waiting timed out, message %s"), *Message);
	}
}

void WaitUntil(TFunction<bool()> Condition, double TimeoutSeconds, const FString Message)
{
	const double StartTime = FPlatformTime::Seconds();
	TimeoutSeconds = StartTime + TimeoutSeconds;
	double LastTickTime = StartTime;

	while (Condition && !Condition() && (FPlatformTime::Seconds() < TimeoutSeconds))
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("%s. Elapsed: %f"), *Message, FPlatformTime::Seconds() - StartTime);
		FTicker::GetCoreTicker().Tick(FPlatformTime::Seconds() - LastTickTime);
		FHttpModule::Get().GetHttpManager().Tick(FPlatformTime::Seconds() - LastTickTime);
		LastTickTime = FPlatformTime::Seconds();
		FPlatformProcess::Sleep(.2f);
	}

	if (Condition && !Condition() && (FPlatformTime::Seconds() > TimeoutSeconds))
	{
		UE_LOG(LogAccelByteTest, Error, TEXT("WaitUntil timed out, message %s"), *Message);
	}
}

void WaitSecond(double WaitTime, const FString Message)
{
	const double StartTime = FPlatformTime::Seconds();
	WaitTime = StartTime + WaitTime;
	double LastTickTime = StartTime;

	while ((FPlatformTime::Seconds() < WaitTime))
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("%s. Elapsed: %f"), *Message, FPlatformTime::Seconds() - StartTime);
		FTicker::GetCoreTicker().Tick(FPlatformTime::Seconds() - LastTickTime);
		FHttpModule::Get().GetHttpManager().Tick(FPlatformTime::Seconds() - LastTickTime);
		LastTickTime = FPlatformTime::Seconds();
		FPlatformProcess::Sleep(.2f);
	}
}

FString GetEnvironmentVariable(const FString Name, const int32 MaxValueLength = 100)
{
	FString Value = Environment::GetEnvironmentVariable(Name, MaxValueLength + 1);

	if (Value.Len() > MaxValueLength)
	{
		UE_LOG(LogAccelByteTest, Fatal, TEXT("Environment variable value is too long: %s"), *Name);
	}

	return FString::Printf(TEXT("%s"), *Value);
}

FString GetMandatoryEnvironmentVariable(const FString Name, const int32 MaxValueLength = 100)
{
	FString Value = GetEnvironmentVariable(Name, MaxValueLength);

	if (Value.IsEmpty())
	{
		UE_LOG(LogAccelByteTest, Fatal, TEXT("Missing mandatory environment variable: %s"), *Name);
	}

	return FString::Printf(TEXT("%s"), *Value);
}

TOptional<FString> GetOptionalEnvironmentVariable(const FString Name, const int32 MaxValueLength = 100)
{
	TOptional<FString> Value;
	FString TempValue = GetEnvironmentVariable(Name, MaxValueLength);

	if (!TempValue.IsEmpty())
	{
		Value = TempValue;
	}

	return Value;
}

FString GetAdminBaseUrl()
{
	return GetMandatoryEnvironmentVariable(TEXT("ADMIN_BASE_URL"));
}

FString GetPublisherNamespace()
{
	return GetMandatoryEnvironmentVariable(TEXT("PUBLISHER_NAMESPACE"));
}

FString AdminUserTokenCache;
double AdminUserTokenExpiry;

FString GetAdminUserAccessToken()
{
	double CurrentTime = FPlatformTime::Seconds();
	if (!AdminUserTokenCache.IsEmpty() && CurrentTime < AdminUserTokenExpiry)
	{
		return AdminUserTokenCache;
	}

	FString ClientId = GetMandatoryEnvironmentVariable(TEXT("ADMIN_CLIENT_ID"));
	FString ClientSecret = GetMandatoryEnvironmentVariable(TEXT("ADMIN_CLIENT_SECRET"));
	FString UserName = GetMandatoryEnvironmentVariable(TEXT("ADMIN_USER_NAME"));
	FString UserPass = GetMandatoryEnvironmentVariable(TEXT("ADMIN_USER_PASS"));

	FString Authorization = TEXT("Basic " + FBase64::Encode(FString::Printf(TEXT("%s:%s"), *ClientId, *ClientSecret)));
	FString Url = FString::Printf(TEXT("%s/iam/oauth/token"), *GetAdminBaseUrl());
	FString Content = FString::Printf(TEXT("grant_type=password&username=%s&password=%s"), *FGenericPlatformHttp::UrlEncode(UserName), *FGenericPlatformHttp::UrlEncode(UserPass));

	AB_HTTP_POST_FORM(Request, Url, Authorization, Content);

	bool bGetTokenSuccess = false;
	FOauth2Token TokenResult;
	THandler<FOauth2Token> OnSuccess;
	OnSuccess.BindLambda([&TokenResult, &bGetTokenSuccess](const FOauth2Token& Result)
		{
			TokenResult = Result;
			bGetTokenSuccess = true;
		});
	FErrorHandler OnError;
	OnError.BindLambda([](int32 ErrorCode, const FString& ErrorMessage)
		{
			UE_LOG(LogAccelByteTest, Fatal, TEXT("Error Code: %d, Message: %s"), ErrorCode, *ErrorMessage);
		});
	CurrentTime = FPlatformTime::Seconds();
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), CurrentTime);
	Waiting(bGetTokenSuccess, "Waiting for get token...");

	AdminUserTokenCache = TokenResult.Access_token;
	AdminUserTokenExpiry = CurrentTime + TokenResult.Expires_in;

	return TokenResult.Access_token;
}

FString GetSteamUserId()
{
	return GetMandatoryEnvironmentVariable(TEXT("STEAM_USER_ID"));
}

TOptional<FString> GetSteamKey()
{
	return GetOptionalEnvironmentVariable(TEXT("STEAM_KEY"));
}

FString GetSteamTicket()
{
	FString SteamTicket = TEXT("");
	FString CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("SteamHelper/steamticket.txt"));
	CurrentDirectory.ReplaceInline(TEXT("/"), TEXT("\\"));
	FFileHelper::LoadFileToString(SteamTicket, *CurrentDirectory);

	return SteamTicket;
}

FString GetSteamAppId()
{
	FString SteamAppId = TEXT("");
	FString CurrentDirectory = IFileManager::Get().ConvertToAbsolutePathForExternalAppForRead(*FPaths::ProjectDir());
	CurrentDirectory.Append(TEXT("SteamHelper/steam_appid.txt"));
	CurrentDirectory.ReplaceInline(TEXT("/"), TEXT("\\"));
	FFileHelper::LoadFileToString(SteamAppId, *CurrentDirectory);

	return SteamAppId;
}

bool CheckSteamTicket() {
	TOptional<FString> SteamKey = GetSteamKey();
	if (!SteamKey.IsSet())
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("Environment variable STEAM_KEY is not set, not checking Steam ticket"));
		return true;
	}
	FString Url = FString::Printf(
		TEXT("https://api.steampowered.com/ISteamUserAuth/AuthenticateUserTicket/v1/?key=%s&appid=%s&ticket=%s&format=json"),
		*SteamKey.GetValue(),
		*GetSteamAppId(),
		*GetSteamTicket());
	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(TEXT("GET"));
	Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
	bool bIsDone = false;
	bool bIsOk = false;
	FRegistry::HttpRetryScheduler.ProcessRequest(Request,
		CreateHttpResultHandler(
			THandler<FString>::CreateLambda([&](const FString& Result)
				{
					UE_LOG(LogAccelByteTest, Log, TEXT("%s: %s"), TEXT("Check Steam ticket"), *Result);
					bIsOk = Result.Contains(TEXT("OK"));
					bIsDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 ErrorCode, const FString& ErrorMessage) {
					bIsDone = true;
				})),
		FPlatformTime::Seconds());
	Waiting(bIsDone, "Waiting ...");
	return bIsOk;
};

void UAccelByteBlueprintsTest::SendNotification(FString Message, bool bAsync, const UAccelByteBlueprintsTest::FSendNotificationSuccess& OnSuccess, const UAccelByteBlueprintsTest::FBlueprintErrorHandler& OnError)
{
	UAccelByteBlueprintsTest::SendNotif(FRegistry::Credentials.GetUserId(), Message, bAsync,
		FVoidHandler::CreateLambda([OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda([OnError](int32 Code, const FString& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UAccelByteBlueprintsTest::SendNotif(FString UserId, FString Message, bool bAsync, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/lobby/notification/namespaces/%s/users/%s/freeform"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *UserId);
	FString Content = FString::Printf(TEXT("{\"message\":\"%s\"}"), *Message);
	Url = Url.Replace(TEXT("wss"), TEXT("https")); //change protocol
	Url = Url.Replace(TEXT("lobby/"), TEXT("")); //no /lobby
	if (bAsync)
	{
		Url.Append(TEXT("?async=true"));
	}
	else
	{
		Url.Append(TEXT("?async=false"));
	}
	AB_HTTP_POST(Request, Url, Authorization, Content);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

FString UAccelByteBlueprintsTest::BytesToFString(TArray<uint8> Input)
{
	uint32 size = Input.Num();
	FString Data = TEXT("");
	Data = BytesToString(Input.GetData(), Input.Num());
	return Data;
}

TArray<uint8> UAccelByteBlueprintsTest::FStringToBytes(FString Input)
{
	uint8* Output = new uint8[Input.Len()];
	TArray<uint8> Return;
	Return.AddUninitialized(Input.Len());
	StringToBytes(Input, Return.GetData(), Input.Len());
	return Return;
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
		Waiting(bIsDone, TEXT("Waiting ..."));
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
		Waiting(bIsDone, TEXT("Waiting ..."));
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
		DeleteUserById(InCredentials.GetUserId(),
			FSimpleDelegate::CreateLambda([&]()
				{
					bIsOk = true;
					bIsDone = true;
				}),
			FErrorHandler::CreateLambda([&](int32 ErrorCode, const FString& ErrorMessage)
				{
					bIsDone = true;
				}));
		Waiting(bIsDone, TEXT("Waiting ..."));
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

void DeleteUserById(const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserMapSuccess = false;
	FUserMapResponse userMap;
	User_Get_User_Mapping(UserId,
		THandler<FUserMapResponse>::CreateLambda([&userMap, &bGetUserMapSuccess](const FUserMapResponse& Result)
			{
				userMap = Result;
				bGetUserMapSuccess = true;
			}), OnError);
	Waiting(bGetUserMapSuccess, "Wait for getting user map data...");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s"), *GetAdminBaseUrl(), *userMap.Namespace, *userMap.userId);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void DeleteUserProfile(const FString& Namespace, const FString& UserId, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/basic/v1/admin/namespaces/%s/users/%s/profiles"), *GetAdminBaseUrl(), *Namespace, *UserId);
	AB_HTTP_DELETE(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void FlushHttpRequests()
{
	double LastTickTime = FPlatformTime::Seconds();

	while (FRegistry::HttpRetryScheduler.PollRetry(FPlatformTime::Seconds()))
	{
		FHttpModule::Get().GetHttpManager().Tick(FPlatformTime::Seconds() - LastTickTime);
		LastTickTime = FPlatformTime::Seconds();
		FPlatformProcess::Sleep(0.5);
	};

	FHttpModule::Get().GetHttpManager().Flush(false);
}

void User_Get_User_Mapping(const FString& userId, const THandler<FUserMapResponse>& OnSuccess, const FErrorHandler& OnError)
{
	check(!userId.IsEmpty());
	UE_LOG(LogAccelByteTest, Log, TEXT("-----------------USER ID: %s---------------------"), *userId);
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/namespaces/%s/users/%s/platforms/justice/%s"), *GetAdminBaseUrl(), *FRegistry::Settings.Namespace, *userId, *GetPublisherNamespace());
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User_Get_Verification_Code(const FString& userId, const THandler<FVerificationCode>& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserMapSuccess = false;
	FUserMapResponse userMap;
	User_Get_User_Mapping(userId,
		THandler<FUserMapResponse>::CreateLambda([&userMap, &bGetUserMapSuccess](const FUserMapResponse& Result)
			{
				userMap = Result;
				bGetUserMapSuccess = true;
			}), OnError);
	Waiting(bGetUserMapSuccess, "Wait for getting user map data...");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *GetAdminUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/iam/v3/admin/namespaces/%s/users/%s/codes"), *GetAdminBaseUrl(), *userMap.Namespace, *userMap.userId);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User_Get_By_Email_Address(const FString& EmailAddress, const THandler<FUserResponse>& OnSuccess, const FErrorHandler& OnError)
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

void User_Delete_By_Email_Address(const FString& EmailAddress, const FSimpleDelegate& OnSuccess, const FErrorHandler& OnError)
{
	bool bGetUserSuccess = false;
	bool bGetUserDone = false;
	FUserResponse userData;
	User_Get_By_Email_Address(EmailAddress,
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
	Waiting(bGetUserDone, "Wait for getting user map data...");

	if (bGetUserSuccess)
	{
		DeleteUserById(userData.UserId, OnSuccess, OnError);
	}
}

void User_Get_MyData_Direct(const FString& JsonWebToken, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *JsonWebToken);
	FString Url = FString::Printf(TEXT("%s/iam/v3/public/users/me"), *FRegistry::Settings.BaseUrl);
	AB_HTTP_GET(Request, Url, Authorization);
	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}
