// Copyright (c) 2018 - 2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserApi.h"

#include "AccelByteRegistry.h"
#include "AccelByteHttpRetryScheduler.h"
#include "AccelByteOauth2Api.h"
#include "Base64.h"

using AccelByte::Api::Oauth2;

namespace AccelByte
{
namespace Api
{

User::User(AccelByte::Credentials& Credentials, AccelByte::Settings& Setting) : Credentials(Credentials), Settings(Setting)
{}

User::~User()
{}

FString User::TempUsername;

static FString PlatformStrings[] = {
	TEXT("steam"),
	TEXT("google"),
	TEXT("facebook"),
	TEXT("android"),
	TEXT("ios"),
	TEXT("device"),
	TEXT("twitch"),
	TEXT("oculus"),
	TEXT("twitter"),
};

void User::LoginWithClientCredentials(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithClientCredentialsGrant(Settings.ClientId, Settings.ClientSecret, THandler<FOauth2Token>::CreateLambda([this, OnSuccess](const FOauth2Token& Result)
	{
		AccelByte::Api::User::Credentials.SetClientToken(Result.Access_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithOtherPlatform(EAccelBytePlatformType PlatformId, const FString& PlatformToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	if (Credentials.GetSessionState() == Credentials::ESessionState::Valid)
	{
		Oauth2::Logout(Credentials.GetUserSessionId(), FVoidHandler::CreateLambda([this]()
		{
			AccelByte::Api::User::Credentials.ForgetAll();
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	Oauth2::GetSessionIdWithPlatformGrant(Settings.ClientId, Settings.ClientSecret, PlatformStrings[static_cast<std::underlying_type<EAccelBytePlatformType>::type>(PlatformId)], PlatformToken, THandler<FOauth2Session>::CreateLambda([this, OnSuccess, OnError](const FOauth2Session& Result)
	{
		const FOauth2Session session = Result;
		AccelByte::Api::User::Credentials.SetUserSession(session.Session_id, FPlatformTime::Seconds() + (session.Expires_in*FMath::FRandRange(0.7, 0.9)));
		GetData(THandler<FUserData>::CreateLambda([this, OnSuccess, session](const FUserData& Result)
		{
			AccelByte::Api::User::Credentials.SetUserLogin(Result.UserId, Result.DisplayName, Result.Namespace);
			OnSuccess.ExecuteIfBound();
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithUsername(const FString& Username, const FString& Password, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	User::TempUsername = Username;
	if (Credentials.GetSessionState() == Credentials::ESessionState::Valid)
	{
		Oauth2::Logout(Credentials.GetUserSessionId(), FVoidHandler::CreateLambda([this]()
		{
			AccelByte::Api::User::Credentials.ForgetAll();
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	Oauth2::GetSessionIdWithPasswordGrant(Settings.ClientId, Settings.ClientSecret, Username, Password, THandler<FOauth2Session>::CreateLambda([this, OnSuccess, OnError](const FOauth2Session& Result)
	{
		const FOauth2Session session = Result;
		AccelByte::Api::User::Credentials.SetUserSession(session.Session_id, FPlatformTime::Seconds() + (session.Expires_in*FMath::FRandRange(0.7, 0.9)));
		GetData(THandler<FUserData>::CreateLambda([this, OnSuccess, session](const FUserData& Result)
		{
			AccelByte::Api::User::Credentials.SetUserLogin(Result.UserId, Result.DisplayName, Result.Namespace);
			OnSuccess.ExecuteIfBound();
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithDeviceId(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	if (Credentials.GetSessionState() == Credentials::ESessionState::Valid)
	{
		Oauth2::Logout(Credentials.GetUserSessionId(), FVoidHandler::CreateLambda([this]()
		{
			AccelByte::Api::User::Credentials.ForgetAll();
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}
	Oauth2::GetSessionIdWithDeviceGrant(Settings.ClientId, Settings.ClientSecret, THandler<FOauth2Session>::CreateLambda([this, OnSuccess, OnError](const FOauth2Session& Result)
	{
		const FOauth2Session session = Result;
		AccelByte::Api::User::Credentials.SetUserSession(session.Session_id, FPlatformTime::Seconds() + (session.Expires_in*FMath::FRandRange(0.7, 0.9)));
		GetData(THandler<FUserData>::CreateLambda([this, OnSuccess, session](const FUserData& Result)
		{
			AccelByte::Api::User::Credentials.SetUserLogin(Result.UserId, Result.DisplayName, Result.Namespace);
			OnSuccess.ExecuteIfBound();
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithLauncher(const FVoidHandler& OnSuccess, const FErrorHandler & OnError)
{
	TCHAR AuthorizationCode[1000];
	AuthorizationCode[0] = 0;
#if PLATFORM_WINDOWS
	FWindowsPlatformMisc::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), AuthorizationCode, 1000);
#elif PLATFORM_LINUX
	FLinuxPlatformMisc::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), AuthorizationCode, 1000);
#elif PLATFORM_MAC
	FApplePlatformMisc::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), AuthorizationCode, 1000);
#endif

	if (Credentials.GetSessionState() == Credentials::ESessionState::Valid)
	{
		Oauth2::Logout(Credentials.GetUserSessionId(), FVoidHandler::CreateLambda([this]()
		{
			AccelByte::Api::User::Credentials.ForgetAll();
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}

	Oauth2::GetSessionIdWithAuthorizationCodeGrant(Settings.ClientId, Settings.ClientSecret, AuthorizationCode, Settings.RedirectURI, THandler<FOauth2Session>::CreateLambda([this, OnSuccess, OnError](const FOauth2Session& Result) {
		const FOauth2Session session = Result;
		AccelByte::Api::User::Credentials.SetUserSession(session.Session_id, FPlatformTime::Seconds() + (session.Expires_in*FMath::FRandRange(0.7, 0.9)));
		GetData(THandler<FUserData>::CreateLambda([this, OnSuccess, session](const FUserData& Result)
		{
			AccelByte::Api::User::Credentials.SetUserLogin(Result.UserId, Result.DisplayName, Result.Namespace);
			OnSuccess.ExecuteIfBound();
		}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		}));
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) {
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::ForgetAllCredentials()
{
	Credentials.ForgetAll();
}

void User::Register(const FString& Username, const FString& Password, const FString& DisplayName, const FString& Country, const FString& DateOfBirth, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FRegisterRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginId = Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");
	NewUserRequest.Country = Country;
	NewUserRequest.DateOfBirth = DateOfBirth;

	FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users"), *Settings.IamServerUrl, *Credentials.GetClientNamespace());
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

void User::GetData(const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v2/public/users/me"), *Settings.IamServerUrl);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
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

/*! Commented because can't send PATCH request yet
void User::Update(const FUserUpdateRequest& UpdateRequest, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *UserCredentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v2/public/users/me"), *UserSettings.IamServerUrl);
	FString Verb = TEXT("PATCH");
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
*/

void User::SendVerificationCode(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FVerificationCodeRequest SendVerificationCodeRequest
	{
		EVerificationContext::UserAccountRegistration,
		TEXT(""),
		User::TempUsername
	};

	SendVerificationCode(SendVerificationCodeRequest, OnSuccess, OnError);
}

void User::SendUpgradeVerificationCode(const FString& Username, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FVerificationCodeRequest SendUpgradeVerificationCodeRequest
	{
		EVerificationContext::UpgradeHeadlessAccount,
		TEXT(""),
		Username
	};
	
	SendVerificationCode(SendUpgradeVerificationCodeRequest, OnSuccess, OnError);
}

//not implemented yet on api-gateway
//void User::UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
//{
//	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserSessionId());
//	FString Url = FString::Printf(TEXT("%s/v3/admin/namespaces/%s/users/%s/headless/code/verify"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
//	FString Verb = TEXT("POST");
//	FString ContentType = TEXT("application/json");
//	FString Accept = TEXT("application/json");
//	FString Content = FString::Printf(TEXT("{ \"Code\": \"%s\", \"LoginId\": \"%s\", \"Password\": \"%s\"}"), *VerificationCode, *Username, *Password);
//
//	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
//	Request->SetURL(Url);
//	Request->SetHeader(TEXT("Authorization"), Authorization);
//	Request->SetVerb(Verb);
//	Request->SetHeader(TEXT("Content-Type"), ContentType);
//	Request->SetHeader(TEXT("Accept"), Accept);
//	Request->SetContentAsString(Content);
//
//	FRegistry::HttpRetryScheduler.ProcessRequest(
//		Request,
//		CreateHttpResultHandler(
//			THandler<FUserData>::CreateLambda(
//				[OnSuccess](const FUserData& UserData)
//				{
//					OnSuccess.ExecuteIfBound(UserData);
//				}),
//				OnError),
//		FPlatformTime::Seconds());
//}

void User::Upgrade(const FString& Username, const FString& Password, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/headless/verify"), *Settings.IamServerUrl, *Credentials.GetUserNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"LoginId\": \"%s\", \"Password\": \"%s\"}"), *Username, *Password);

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
			THandler<FUserData>::CreateLambda(
				[OnSuccess](const FUserData& UserData)
				{
					OnSuccess.ExecuteIfBound(UserData);
				}),
				OnError),
		FPlatformTime::Seconds());
}

void User::Verify(const FString& VerificationCode, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString ContactType = TEXT("email");
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/code/verify"), *Settings.IamServerUrl, *Credentials.GetClientNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"Code\": \"%s\",\"ContactType\":\"%s\"}"), *VerificationCode, *ContactType);

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
	FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/forgotPassword"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{\"LoginId\": \"%s\"}"), *Username);

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
	FResetPasswordRequest ResetPasswordRequest;
	ResetPasswordRequest.Code = VerificationCode;
	ResetPasswordRequest.LoginId = Username;
	ResetPasswordRequest.NewPassword = NewPassword;
	FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/resetPassword"), *Settings.IamServerUrl, *Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
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

void User::GetPlatformLinks(const THandler<TArray<FPlatformLink>>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms"), *Settings.IamServerUrl, *Credentials.GetClientNamespace());
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
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

void User::LinkOtherPlatform(const FString& PlatformId, const FString& Ticket, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/me/platforms/%s/link"), *Settings.IamServerUrl, *Credentials.GetClientNamespace(), *PlatformId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/x-www-form-urlencoded");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("ticket=%s"), *Ticket);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::UnlinkOtherPlatform(const FString& PlatformId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/me/platforms/%s/link"), *Settings.IamServerUrl, *Credentials.GetClientNamespace(), *PlatformId);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
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
	FString Authorization = TEXT("");
	FString Namespace = TEXT("");
	
	Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	Namespace = Credentials.GetUserNamespace();
	
	FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/code/request"), *Settings.IamServerUrl, *Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = TEXT("");
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

void User::GetUserByLoginId(const FString& LoginId, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/byLoginId?loginId=%s"), *Settings.IamServerUrl, *Credentials.GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(LoginId));
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
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

void User::GetPublicUserInfo(const FString& UserID, const THandler<FPublicUserInfo>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *Credentials.GetUserSessionId());
	FString Url = FString::Printf(TEXT("%s/v2/public/namespaces/%s/users/%s"), *Settings.IamServerUrl, *Settings.Namespace, *UserID);
	FString Verb = TEXT("GET");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->OnProcessRequestComplete() = CreateHttpResultHandler(OnSuccess, OnError);
	Request->ProcessRequest();
}

} // Namespace Api
} // Namespace AccelByte
