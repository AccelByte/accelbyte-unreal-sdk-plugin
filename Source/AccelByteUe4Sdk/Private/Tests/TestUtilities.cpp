// Copyright (c) 2018 - 2021 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "TestUtilities.h"
#include "HttpModule.h"
#include "HttpManager.h"
#include "Core/AccelByteReport.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteEnvironment.h"
#include "Api/AccelByteUserApi.h"
#include "Misc/Base64.h"

DEFINE_LOG_CATEGORY(LogAccelByteTest);

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Settings;
using AccelByte::Credentials;
using AccelByte::HandleHttpError;

void WaitUntil(const bool& bCondition, const FString Message, const double TimeoutSeconds)
{
	WaitUntil([&] { return bCondition; }, Message, TimeoutSeconds);
}

void WaitUntil(const TFunction<bool()> Condition, const FString Message, const double TimeoutSeconds)
{
	const double StartSeconds = FPlatformTime::Seconds();
	const double LimitSeconds = StartSeconds + TimeoutSeconds;
	double LastTickSeconds = StartSeconds;

	while (Condition && !Condition() && (FPlatformTime::Seconds() < LimitSeconds))
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("%s\t%s Elapsed %f s"),
			*Message, *FString(__FUNCTION__), FPlatformTime::Seconds() - StartSeconds);
		FTicker::GetCoreTicker().Tick(.2f);
		FHttpModule::Get().GetHttpManager().Tick(.2f);
		LastTickSeconds = FPlatformTime::Seconds();
		FPlatformProcess::Sleep(.2f);
	}

	if (Condition && !Condition() && (FPlatformTime::Seconds() > LimitSeconds))
	{
		UE_LOG(LogAccelByteTest, Error, TEXT("%s\t%s Timed Out"), *Message, *FString(__FUNCTION__));
	}
}

void DelaySeconds(double Seconds, const FString Message)
{
	const double StartTime = FPlatformTime::Seconds();
	const double LimitSeconds = StartTime + Seconds;
	double LastTickTime = StartTime;

	while ((FPlatformTime::Seconds() < LimitSeconds))
	{
		UE_LOG(LogAccelByteTest, Log, TEXT("%s\t%s Elapsed %f s"),
			*Message, *FString(__FUNCTION__), FPlatformTime::Seconds() - StartTime);
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
	WaitUntil(bGetTokenSuccess, "Waiting for get token...");

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
	WaitUntil(bIsDone, "Waiting ...");
	return bIsOk;
}

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

