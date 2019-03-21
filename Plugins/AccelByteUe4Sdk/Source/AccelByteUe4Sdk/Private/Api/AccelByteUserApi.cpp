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
	Oauth2::GetAccessTokenWithClientCredentialsGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, THandler<FOauth2Token>::CreateLambda([OnSuccess](const FOauth2Token& Result)
	{
		FRegistry::Credentials.SetClientToken(Result.Access_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithOtherPlatform(EAccelBytePlatformType PlatformId, const FString& PlatformToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithPlatformGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, PlatformStrings[static_cast<std::underlying_type<EAccelBytePlatformType>::type>(PlatformId)], PlatformToken, THandler<FOauth2Token>::CreateLambda([OnSuccess](const FOauth2Token& Result)
	{
		FRegistry::Credentials.SetUserToken(Result.Access_token, Result.Refresh_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.User_id, Result.Display_name, Result.Namespace);

		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithUsername(const FString& Username, const FString& Password, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	User::TempUsername = Username;

	Oauth2::GetAccessTokenWithPasswordGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, Username, Password, THandler<FOauth2Token>::CreateLambda([OnSuccess](const FOauth2Token& Result)
	{
		FRegistry::Credentials.SetUserToken(Result.Access_token, Result.Refresh_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.User_id, Result.Display_name, Result.Namespace);

		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::LoginWithDeviceId(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	Oauth2::GetAccessTokenWithDeviceGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, THandler<FOauth2Token>::CreateLambda([OnSuccess](const FOauth2Token& Result)
	{
		FRegistry::Credentials.SetUserToken(Result.Access_token, Result.Refresh_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.User_id, Result.Display_name, Result.Namespace);

		OnSuccess.ExecuteIfBound();
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
#endif



	Oauth2::GetAccessTokenWithAuthorizationCodeGrant(FRegistry::Settings.ClientId, FRegistry::Settings.ClientSecret, AuthorizationCode, FRegistry::Settings.RedirectURI, THandler<FOauth2Token>::CreateLambda([OnSuccess](const FOauth2Token& Result) {
		FRegistry::Credentials.SetUserToken(Result.Access_token, Result.Refresh_token, FPlatformTime::Seconds() + (Result.Expires_in*FMath::FRandRange(0.7, 0.9)), Result.User_id, Result.Display_name, Result.Namespace);
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) {
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void User::ForgetAllCredentials()
{
	FRegistry::Credentials.ForgetAll();
}

void User::Register(const FString& Username, const FString& Password, const FString& DisplayName, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FRegisterRequest NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.LoginId = Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");

	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(NewUserRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::GetData(const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
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

void User::Update(const FUserUpdateRequest& UpdateRequest, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
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

void User::UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/upgradeHeadlessAccountWithVerificationCode"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{ \"Code\": \"%s\", \"LoginId\": \"%s\", \"Password\": \"%s\"}"), *VerificationCode, *Username, *Password);

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
					FRegistry::Credentials.ForceRefreshToken();
					OnSuccess.ExecuteIfBound(UserData);
				}),
				OnError),
		FPlatformTime::Seconds());
}

void User::Upgrade(const FString& Username, const FString& Password, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/upgradeHeadlessAccount"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FRegistry::Credentials.GetUserId());
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
					FRegistry::Credentials.ForceRefreshToken();
					OnSuccess.ExecuteIfBound(UserData);
				}),
				OnError),
		FPlatformTime::Seconds());
}

void User::Verify(const FString& VerificationCode, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FString ContactType = TEXT("email");
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/verification"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace(), *FRegistry::Credentials.GetUserId());
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
	FString Authorization = TEXT("Basic " + FBase64::Encode(FRegistry::Settings.ClientId + ":" + FRegistry::Settings.ClientSecret));
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/forgotPassword"), *FRegistry::Settings.IamServerUrl, *FRegistry::Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content = FString::Printf(TEXT("{\"LoginId\": \"%s\"}"), *Username);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
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
	FString Authorization = TEXT("Basic " + FBase64::Encode(FRegistry::Settings.ClientId + ":" + FRegistry::Settings.ClientSecret));
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/resetPassword"), *FRegistry::Settings.IamServerUrl, *FRegistry::Settings.Namespace);
	FString Verb = TEXT("POST");
	FString ContentType = TEXT("application/json");
	FString Accept = TEXT("application/json");
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ResetPasswordRequest, Content);

	FHttpRequestPtr Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(Url);
	Request->SetHeader(TEXT("Authorization"), Authorization);
	Request->SetVerb(Verb);
	Request->SetHeader(TEXT("Content-Type"), ContentType);
	Request->SetHeader(TEXT("Accept"), Accept);
	Request->SetContentAsString(Content);

	FRegistry::HttpRetryScheduler.ProcessRequest(Request, CreateHttpResultHandler(OnSuccess, OnError), FPlatformTime::Seconds());
}

void User::GetPlatformLinks(const THandler<TArray<FPlatformLink>>& OnSuccess, const FErrorHandler& OnError)
{
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace(), *FRegistry::Credentials.GetUserId());
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
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms/%s/link"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace(), *FRegistry::Credentials.GetUserId(), *PlatformId);
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
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetClientAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/platforms/%s/unlink"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetClientNamespace(), *FRegistry::Credentials.GetUserId(), *PlatformId);
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
	
	Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	Namespace = FRegistry::Credentials.GetUserNamespace();
	
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s/verificationcode"), *FRegistry::Settings.IamServerUrl, *Namespace, *FRegistry::Credentials.GetUserId());
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
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/byLoginId?loginId=%s"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *FGenericPlatformHttp::UrlEncode(LoginId));
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
	FString Authorization = FString::Printf(TEXT("Bearer %s"), *FRegistry::Credentials.GetUserAccessToken());
	FString Url = FString::Printf(TEXT("%s/namespaces/%s/users/%s"), *FRegistry::Settings.IamServerUrl, *FRegistry::Credentials.GetUserNamespace(), *UserID);
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
