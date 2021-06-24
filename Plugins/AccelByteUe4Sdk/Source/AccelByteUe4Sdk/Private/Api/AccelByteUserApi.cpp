// Copyright (c) 2018 - 2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "Api/AccelByteUserApi.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteItemApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteHttpListenerExtension.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteEnvironment.h"
#include "Api/AccelByteOauth2Api.h"
#include "Runtime/Core/Public/Misc/Base64.h"

DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteUser, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteUser);

using AccelByte::Api::Oauth2;

namespace AccelByte
{
namespace Api
{

User::User(AccelByte::Credentials& Credentials, AccelByte::Settings& Setting) : Creds(Credentials), Settings(Setting)
{}

User::~User()
{}

static HttpListenerExtension ListenerExtension;
FString User::TempUsername;

static FString SearchStrings[] = {
	TEXT(""),
	TEXT("emailAddress"),
	TEXT("displayName"),
	TEXT("username")
};

static FString GetPlatformString(EAccelBytePlatformType PlatformType)
{
	switch (PlatformType)
	{
	case EAccelBytePlatformType::Steam:
		return "steam";
	case EAccelBytePlatformType::PS4:
		return "ps4";
	case EAccelBytePlatformType::PS4CrossGen:
	case EAccelBytePlatformType::PS5:
		return "ps5";
	case EAccelBytePlatformType::Live:
		return "live";
	case EAccelBytePlatformType::Google:
		return "google";
	case EAccelBytePlatformType::Facebook:
		return "facebook";
	case EAccelBytePlatformType::Android:
		return "android";
	case EAccelBytePlatformType::iOS:
		return "ios";
	case EAccelBytePlatformType::Device:
		return "device";
	case EAccelBytePlatformType::Twitch:
		return "twitch";
	case EAccelBytePlatformType::Oculus:
		return "oculus";
	case EAccelBytePlatformType::Twitter:
		return "twitter";
	case EAccelBytePlatformType::EpicGames:
		return "epicgames";
	case EAccelBytePlatformType::Stadia:
		return "stadia";
	default:
		return "unknown";
	}
}

void User::LoginWithOtherPlatform(EAccelBytePlatformType PlatformType, const FString& PlatformToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Creds.GetSessionState() == Credentials::ESessionState::Valid)
	{
		Creds.ForgetAll();
	}
	Oauth2::GetTokenWithOtherPlatformToken(Settings.ClientId, Settings.ClientSecret, GetPlatformString(PlatformType), PlatformToken, THandler<FOauth2Token>::CreateLambda([this, PlatformType, OnSuccess, OnError](const FOauth2Token& Result)
	{
		Creds.SetAuthToken(Result, FPlatformTime::Seconds());
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithUsername(const FString& Username, const FString& Password, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	TempUsername = Username;
	Creds.SetUserEmailAddress(Username);
	if (Creds.GetSessionState() == Credentials::ESessionState::Valid)
	{
		Creds.ForgetAll();
	}
	Oauth2::GetTokenWithPasswordCredentials(Settings.ClientId, Settings.ClientSecret, Username, Password, THandler<FOauth2Token>::CreateLambda([this, OnSuccess, OnError](const FOauth2Token& Result)
	{
		Creds.SetAuthToken(Result, FPlatformTime::Seconds());
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithDeviceId(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Creds.GetSessionState() == Credentials::ESessionState::Valid)
	{
		Creds.ForgetAll();
	}
	Oauth2::GetTokenWithDeviceId(Settings.ClientId, Settings.ClientSecret, THandler<FOauth2Token>::CreateLambda([this, OnSuccess, OnError](const FOauth2Token& Result)
	{
		Creds.SetAuthToken(Result, FPlatformTime::Seconds());
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithLauncher(const FVoidHandler& OnSuccess, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString AuthorizationCode = Environment::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), 1000);

	if (Creds.GetSessionState() == Credentials::ESessionState::Valid)
	{
		Creds.ForgetAll();
	}

	Oauth2::GetTokenWithAuthorizationCode(Settings.ClientId, Settings.ClientSecret, AuthorizationCode, Settings.RedirectURI, THandler<FOauth2Token>::CreateLambda([this, OnSuccess, OnError](const FOauth2Token& Result) {
		Creds.SetAuthToken(Result, FPlatformTime::Seconds());
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::ForgetAllCredentials()
{
	FReport::Log(FString(__FUNCTION__));

	Creds.ForgetAll();
}

void User::Register(const FString& Username, const FString& Password, const FString& DisplayName, const FString& Country, const FString& DateOfBirth, const THandler<FRegisterResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FRegisterRequest NewUserRequest;
	NewUserRequest.DisplayName  = DisplayName;
	NewUserRequest.Password     = Password;
	NewUserRequest.EmailAddress = Username;
	NewUserRequest.AuthType     = TEXT("EMAILPASSWD");
	NewUserRequest.Country      = Country;
	NewUserRequest.DateOfBirth  = DateOfBirth;

	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(NewUserRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::Registerv2(const FString& EmailAddress, const FString& Username, const FString& Password, const FString& DisplayName, const FString& Country, const FString& DateOfBirth, const THandler<FRegisterResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FRegisterRequestv2 NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.EmailAddress = EmailAddress;
	NewUserRequest.Username = Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");
	NewUserRequest.Country = Country;
	NewUserRequest.DateOfBirth = DateOfBirth;

	FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(NewUserRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::Registerv3(const FRegisterRequestv3& RegisterRequest, const THandler<FRegisterResponse>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FRegisterRequestv3 NewUserRequest;
	NewUserRequest.AcceptedPolicies = RegisterRequest.AcceptedPolicies;
	NewUserRequest.DisplayName = RegisterRequest.DisplayName;
	NewUserRequest.Password = RegisterRequest.Password;
	NewUserRequest.EmailAddress = RegisterRequest.EmailAddress;
	NewUserRequest.Username = RegisterRequest.Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");
	NewUserRequest.Country = RegisterRequest.Country;
	NewUserRequest.DateOfBirth = RegisterRequest.DateOfBirth;

	FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(NewUserRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::GetData(const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/users/me"), *Settings.IamServerUrl);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::UpdateUser(FUserUpdateRequest UpdateRequest, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!UpdateRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("Cannot update user email using this function. Use UpdateEmail instead."));
		return;
	}

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(UpdateRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::UpdateEmail(FUpdateEmailRequest UpdateEmailRequest, const FVoidHandler & OnSuccess, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/email"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb = TEXT("PUT");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(UpdateEmailRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType, const TArray<FString>& OtherPlatformUserId, const THandler<FBulkPlatformUserIdResponse>& OnSuccess, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformString = GetPlatformString(PlatformType);
	const FBulkPlatformUserIdRequest UserIdRequests{ OtherPlatformUserId };

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/platforms/%s/users"), *Settings.IamServerUrl, *Settings.Namespace, *PlatformString);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(UserIdRequests, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::SendVerificationCode(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Creds.GetUserEmailAddress().IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("User email address cannot be empty"));
	}

	FVerificationCodeRequest SendVerificationCodeRequest
	{
		EVerificationContext::UserAccountRegistration,
		TEXT(""),
		Creds.GetUserEmailAddress()
	};

	SendVerificationCode(SendVerificationCodeRequest, OnSuccess, OnError);
}

void User::SendUpdateEmailVerificationCode(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Creds.GetUserEmailAddress().IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("User email address cannot be empty"));
	}

	FVerificationCodeRequest SendVerificationCodeRequest
	{
		EVerificationContext::UpdateEmailAddress,
		TEXT(""),
		Creds.GetUserEmailAddress()
	};

	SendVerificationCode(SendVerificationCodeRequest, OnSuccess, OnError);
}

void User::SendUpgradeVerificationCode(const FString& Username, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FVerificationCodeRequest SendUpgradeVerificationCodeRequest
	{
		EVerificationContext::UpgradeHeadlessAccount,
		TEXT(""),
		Username
	};

	SendVerificationCode(SendUpgradeVerificationCodeRequest, OnSuccess, OnError);
}

void User::UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/headless/code/verify"), *Settings.IamServerUrl, *Creds.GetNamespace(), *Creds.GetUserId());
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("{ \"code\": \"%s\", \"emailAddress\": \"%s\", \"password\": \"%s\"}"), *VerificationCode, *Username, *Password);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(
		Request,
		CreateHttpResultHandler(
			THandler<FAccountUserData>::CreateLambda(
				[OnSuccess](const FAccountUserData& UserData)
				{
					OnSuccess.ExecuteIfBound(UserData);
				}),
			OnError),
		FPlatformTime::Seconds());
}

void User::Upgrade(const FString& Username, const FString& Password, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/headless/verify"), *Settings.IamServerUrl, *Creds.GetNamespace());
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("{ \"EmailAddress\": \"%s\", \"Password\": \"%s\"}"), *Username, *Password);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(
		Request,
		CreateHttpResultHandler(
			THandler<FAccountUserData>::CreateLambda(
				[OnSuccess](const FAccountUserData& UserData)
				{
					OnSuccess.ExecuteIfBound(UserData);
				}),
			OnError),
		FPlatformTime::Seconds());
}

void User::Upgradev2(const FString& EmailAddress, const FString& Username, const FString& Password, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/headless/verify"), *Settings.IamServerUrl, *Creds.GetNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"emailAddress\": \"%s\", \"password\": \"%s\", \"username\": \"%s\"}"), *EmailAddress, *Password, *Username);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(
		Request,
		CreateHttpResultHandler(
			THandler<FAccountUserData>::CreateLambda(
				[OnSuccess](const FAccountUserData& UserData)
				{
					OnSuccess.ExecuteIfBound(UserData);
				}),
			OnError),
		FPlatformTime::Seconds());
}

void User::Verify(const FString& VerificationCode, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString ContactType     = TEXT("email");
	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/code/verify"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("{ \"Code\": \"%s\",\"ContactType\":\"%s\"}"), *VerificationCode, *ContactType);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::SendResetPasswordCode(const FString& Username, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/forgot"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("{\"emailAddress\": \"%s\"}"), *Username);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::ResetPassword(const FString& VerificationCode, const FString& Username, const FString& NewPassword, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FResetPasswordRequest ResetPasswordRequest;
	ResetPasswordRequest.Code           = VerificationCode;
	ResetPasswordRequest.EmailAddress   = Username;
	ResetPasswordRequest.NewPassword    = NewPassword;
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/reset"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ResetPasswordRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::GetPlatformLinks(const THandler<FPagedPlatformLinks>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/platforms"), *Settings.IamServerUrl, *Creds.GetNamespace(), *Creds.GetUserId());
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::LinkOtherPlatform(EAccelBytePlatformType PlatformType, const FString& Ticket, const FVoidHandler& OnSuccess, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	auto PlatformId = GetPlatformString(PlatformType);

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s"), *Settings.IamServerUrl, *Creds.GetNamespace(), *PlatformId);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/x-www-form-urlencoded");
	FString Accept          = TEXT("application/json");
	FString Content         = FString::Printf(TEXT("ticket=%s"), *FGenericPlatformHttp::UrlEncode(*Ticket));

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType, const FString& PlatformUserId, const FVoidHandler& OnSuccess, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	auto PlatformId = GetPlatformString(PlatformType);

	FLinkPlatformAccountRequest linkRequest;
	linkRequest.PlatformId = PlatformId;
	linkRequest.PlatformUserId = PlatformUserId;

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/platforms/link"), *Settings.IamServerUrl, *Creds.GetNamespace(), *Creds.GetUserId());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(linkRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::UnlinkOtherPlatform(EAccelBytePlatformType PlatformType, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	auto PlatformId = GetPlatformString(PlatformType);

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s"), *Settings.IamServerUrl, *Creds.GetNamespace(), *PlatformId);
	FString Verb            = TEXT("DELETE");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::SendVerificationCode(const FVerificationCodeRequest& VerificationCodeRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = TEXT("");
	FString Namespace       = TEXT("");
	
	Authorization = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	Namespace = Creds.GetNamespace();
	
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/code/request"), *Settings.IamServerUrl, *Namespace);
	FString Verb            = TEXT("POST");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content         = TEXT("");
	FJsonObjectConverter::UStructToJsonObjectString(VerificationCodeRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::SearchUsers(const FString& Query, EAccelByteSearchType By, const THandler<FPagedPublicUsersInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users?query=%s"), *Settings.IamServerUrl, *Creds.GetNamespace(), *FGenericPlatformHttp::UrlEncode(Query));
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");
	FString Content;

	if (By != EAccelByteSearchType::ALL)
	{
		FString SearchId = SearchStrings[static_cast<std::underlying_type<EAccelByteSearchType>::type>(By)];
		Url.Append(FString::Printf(TEXT("&by=%s"), *SearchId));
	}

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::SearchUsers(const FString& Query, const THandler<FPagedPublicUsersInfo>& OnSuccess, const FErrorHandler& OnError)
{
	SearchUsers(Query, EAccelByteSearchType::ALL, OnSuccess, OnError);
}

void User::GetUserByUserId(const FString& UserID, const THandler<FSimpleUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s"), *Settings.IamServerUrl, *Settings.Namespace, *UserID);
	FString Verb            = TEXT("GET");
	FString ContentType     = TEXT("application/json");
	FString Accept          = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::GetUserByOtherPlatformUserId(EAccelBytePlatformType PlatformType, const FString& OtherPlatformUserId, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FString PlatformId      = GetPlatformString(PlatformType);

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/public/namespaces/%s/platforms/%s/users/%s"), *Settings.IamServerUrl, *Settings.Namespace, *PlatformId, *OtherPlatformUserId);
	FString Verb            = TEXT("GET");
	FString Accept          = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::GetCountryFromIP(const THandler<FCountryInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Authorization   = FString::Printf(TEXT("Bearer %s"), *Creds.GetAccessToken());
	FString Url             = FString::Printf(TEXT("%s/v3/location/country"), *Settings.IamServerUrl);
	FString Verb            = TEXT("GET");
	FString Accept          = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Accept"), Accept);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::GetUserEligibleToPlay(const THandler<bool>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	auto onItemInfoGot = THandler<FAccelByteModelsItemInfo>::CreateLambda([this, OnSuccess, OnError](const FAccelByteModelsItemInfo& itemInfoResult)
	{

		TArray<FString> itemIds;
		TArray<FString> skus = itemInfoResult.Features;
		TArray<FString> appIds;
		appIds.Init(*Settings.AppId, 1);

		FRegistry::Entitlement.GetUserEntitlementOwnershipAny(itemIds, appIds, skus, THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([OnSuccess, OnError](FAccelByteModelsEntitlementOwnership ownership)
		{
			OnSuccess.ExecuteIfBound(ownership.Owned);
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMsg)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMsg);
		}));
	});

	FRegistry::Item.GetItemByAppId(*Settings.AppId, "", "", onItemInfoGot, FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMsg)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMsg);
	}));
}


} // Namespace Api
} // Namespace AccelByte
