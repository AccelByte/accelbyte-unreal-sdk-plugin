// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
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
#include "Core/AccelByteOauth2Api.h"
#include "Api/AccelByteQos.h"
#include "Core/AccelByteUtilities.h"
#include "Core/IAccelByteDataStorage.h"
#include "Core/AccelByteDataStorageBinaryFile.h"
#include "AccelByteUe4SdkModule.h"


DECLARE_LOG_CATEGORY_EXTERN(LogAccelByteUser, Log, All);
DEFINE_LOG_CATEGORY(LogAccelByteUser);

using AccelByte::Api::Oauth2;

namespace AccelByte
{
namespace Api
{

User::User(Credentials& InCredentialsRef
	, Settings& InSettingsRef
	, FHttpRetryScheduler& InHttpRef)
	: FApiBase(InCredentialsRef, InSettingsRef, InHttpRef)
	, UserCredentialsRef{InCredentialsRef}
{
}

User::~User()
{}

static FString SearchStrings[] =
{
	TEXT(""),
	TEXT("displayName"),
	TEXT("username"),
};

void User::FinalPreLoginEvents()
{
	if (UserCredentialsRef.GetSessionState() == Credentials::ESessionState::Valid)
	{
		UserCredentialsRef.ForgetAll();
	}
}
	

#pragma region Login Methods
	
void User::LoginWithUsername(const FString& Username
	, const FString& Password
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	LoginWithUsername(Username, Password, OnSuccess, OnErrorHandler);
}

void User::LoginWithUsername(const FString& Username
	, const FString& Password
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
    	
	Oauth2::GetTokenWithPasswordCredentials(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, Username
		, Password
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);
			})
		, OnError
		, SettingsRef.IamServerUrl);
	
	UserCredentialsRef.SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithUsernameV3(const FString& Username
	, const FString& Password
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError
	, const bool bRememberMe)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	LoginWithUsernameV3(Username, Password, OnSuccess, OnErrorHandler, bRememberMe);
}

void User::LoginWithUsernameV3(const FString& Username
	, const FString& Password
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
	
	Oauth2::GetTokenWithPasswordCredentialsV3(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, Username
		, Password
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);
			})
		, OnError
		, bRememberMe
		, SettingsRef.IamServerUrl);

	UserCredentialsRef.SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithDeviceId(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	LoginWithDeviceId(OnSuccess, OnErrorHandler);
} 

void User::LoginWithDeviceId(const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{ 
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
	
	Oauth2::GetTokenWithDeviceId(CredentialsRef.GetOAuthClientId()
		, CredentialsRef.GetOAuthClientSecret()
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess](const FOauth2Token& Result)
			{
				OnLoginSuccess(OnSuccess, Result); // Curry to general handler	
			})
		, OnError
		, SettingsRef.IamServerUrl);

	UserCredentialsRef.SetBearerAuthRejectedHandler(HttpRef);
}
	
void User::LoginWithOtherPlatform(EAccelBytePlatformType PlatformType
	, const FString& PlatformToken
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	LoginWithOtherPlatform(PlatformType, PlatformToken, OnSuccess, OnErrorHandler, true);
}

void User::LoginWithOtherPlatform(EAccelBytePlatformType PlatformType
	, const FString& PlatformToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError
	, bool bCreateHeadless)
{
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
	
	Oauth2::GetTokenWithOtherPlatformToken(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, FAccelByteUtilities::GetPlatformString(PlatformType)
		, PlatformToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);
			})
		, FOAuthErrorHandler::CreateLambda(
			[this, OnError](const int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef.SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
		, bCreateHeadless
		, SettingsRef.IamServerUrl);

	UserCredentialsRef.SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithOtherPlatformId(const FString& PlatformId
	, const FString& PlatformToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError
	, bool bCreateHeadless)
{
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
	
	Oauth2::GetTokenWithOtherPlatformToken(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, PlatformId
		, PlatformToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);
			})
		, FOAuthErrorHandler::CreateLambda(
			[this, OnError](const int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef.SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
		, bCreateHeadless
		, SettingsRef.IamServerUrl);

	UserCredentialsRef.SetBearerAuthRejectedHandler(HttpRef);
}

void User::VerifyLoginWithNewDevice2FAEnabled(const FString& MfaToken
	, EAccelByteLoginAuthFactorType AuthFactorType
	, const FString& Code
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError
	, bool bRememberDevice)
{
	FReport::Log(FString(__FUNCTION__));

	Oauth2::VerifyAndRememberNewDevice(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, MfaToken
		, AuthFactorType
		, Code
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);
			})
		, OnError
		, bRememberDevice
		, SettingsRef.IamServerUrl);

	UserCredentialsRef.SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithLauncher(const FVoidHandler& OnSuccess
	, const FErrorHandler & OnError)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	LoginWithLauncher(OnSuccess, OnErrorHandler);
}
	
void User::LoginWithLauncher(const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString AuthorizationCode = Environment::GetEnvironmentVariable(TEXT("JUSTICE_AUTHORIZATION_CODE"), 1000);

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	Oauth2::GetTokenWithAuthorizationCode(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, AuthorizationCode
		, SettingsRef.RedirectURI
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);			
			})
		, OnError
		, SettingsRef.IamServerUrl);

	UserCredentialsRef.SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithRefreshToken(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	LoginWithRefreshToken(UserCredentialsRef.GetRefreshToken(), OnSuccess, OnError);
}

void User::LoginWithRefreshToken(const FString& RefreshToken
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	LoginWithRefreshToken(RefreshToken, OnSuccess, OnErrorHandler);
}
void User::LoginWithRefreshToken(const FString& RefreshToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	Oauth2::GetTokenWithRefreshToken(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, RefreshToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);
			})
		, OnError
		, SettingsRef.IamServerUrl);
}

bool IsTokenExpired(FRefreshInfo RefreshInfo)
{
	return RefreshInfo.Expiration <= FDateTime::UtcNow();
}

void User::TryRelogin(const FString& PlatformUserID
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	TryRelogin(PlatformUserID, OnSuccess, FOAuthErrorHandler::CreateLambda([OnError]
		(int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo& ErrorObject )
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	})); 
}

void User::TryRelogin(const FString& PlatformUserID
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
#if PLATFORM_WINDOWS
	FReport::Log(FString(__FUNCTION__));

	IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->GetItem(PlatformUserID
		, THandler<TPair<FString, FString>>::CreateLambda(
			[this, OnSuccess, OnError](TPair<FString, FString> Pair)
			{
				if (Pair.Key.IsEmpty() || Pair.Value.IsEmpty())
				{
					OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::CachedTokenNotFound), TEXT("The cached token is not found. Cannot continue the previous login session. Please login again."), FErrorOAuthInfo{});
					return;
				}

				auto Decoded = FAccelByteUtilities::XOR(Pair.Value, FAccelByteUtilities::GetDeviceId());
				FRefreshInfo RefreshInfo;
				if (!FJsonObjectConverter::JsonObjectStringToUStruct<FRefreshInfo>(Decoded, &RefreshInfo, 0, 0))
				{
					OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::UnableToSerializeCachedToken), TEXT("The cached token can't be parsed. Cannot continue the previous login session. Please login again."), FErrorOAuthInfo{});
					return;
				}

				if (IsTokenExpired(RefreshInfo))
				{
					OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::CachedTokenExpired), TEXT("Your previous login session is expired. Please login again."), FErrorOAuthInfo{});
					return;
				}

				this->LoginWithRefreshToken(RefreshInfo.RefreshToken, OnSuccess, OnError);
			}));
#else
	OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::CachedTokenNotFound), TEXT("Cannot relogin using cached token on other platforms."), FErrorOAuthInfo{});
#endif
}

void User::CreateHeadlessAccountAndLogin(const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	CreateHeadlessAccountAndLogin(UserCredentialsRef.GetLinkingToken(), OnSuccess, OnError);
}

void User::CreateHeadlessAccountAndLogin(const FString& LinkingToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	Oauth2::CreateHeadlessAccountAndResponseToken(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, LinkingToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);
			})	
		, OnError
		, SettingsRef.IamServerUrl); 
}
	
void User::AuthenticationWithPlatformLinkAndLogin(const FString& Username
	, const FString& Password
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	AuthenticationWithPlatformLinkAndLogin(Username, Password, UserCredentialsRef.GetLinkingToken(), OnSuccess, OnError);
}
	
void User::AuthenticationWithPlatformLinkAndLogin(const FString& Username
	, const FString& Password
	, const FString& LinkingToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	Oauth2::AuthenticationWithPlatformLink(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, Username
		, Password
		, LinkingToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);
			})		
		, OnError
		, SettingsRef.IamServerUrl);
}

#pragma endregion /Login Methods

void User::ProcessLoginResponse(const FOauth2Token& Response
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	if (Response.Access_token.IsEmpty() || Response.Expires_in <= 0.0f)
	{
		FErrorOAuthInfo EmptyObject;
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidResponse), TEXT("Invalid Response. Access Token is empty"), EmptyObject);
	}
	else
	{
		OnLoginSuccess(OnSuccess, Response);
	}
}
	
void User::OnLoginSuccess(const FVoidHandler& OnSuccess
	, const FOauth2Token& Response)
{
	// Set auth token before anything: Set before anything to prevent race condition issues.
	UserCredentialsRef.SetAuthToken(Response, FPlatformTime::Seconds());

	if (!Response.Auth_Trust_Id.IsEmpty())
	{
		FAccelByteUtilities::SetAuthTrustId(Response.Auth_Trust_Id);
	}
	
	FVoidHandler CallbackFunction = FVoidHandler::CreateLambda(
		[this, OnSuccess, Response]()
		{
			UserCredentialsRef.OnLoginSuccess().Broadcast(Response); // Sets auth tokens, inits qosScheduler
			OnSuccess.ExecuteIfBound();
		});

	GetData(THandler<FAccountUserData>::CreateLambda(
			[CallbackFunction](const FAccountUserData& AccountUserData)
			{
				CallbackFunction.Execute();
			})
		, FErrorHandler::CreateLambda(
			[CallbackFunction](int ErrorCode, const FString& ErrorMessage)
			{
				FReport::Log(FString::Printf(TEXT("[AccelByte] Error GetData after Login Success, Error Code: %d Message: %s"), ErrorCode, *ErrorMessage));
				CallbackFunction.Execute();
			})
		);

	FHttpRetryScheduler::SetHeaderNamespace(Response.Namespace);

#ifndef PLATFORM_WINDOWS // the following code is working on Windows only at the moment
	return;
#endif

	if (Response.Platform_user_id.IsEmpty() || Response.Refresh_token.IsEmpty())
	{
		return;
	}

	// Store the refresh token
	FRefreshInfo Info;
	Info.RefreshToken = Response.Refresh_token;
	Info.Expiration = FDateTime::UtcNow() + FTimespan::FromSeconds(Response.Refresh_expires_in);
	FString SerializedInfo;
	FJsonObjectConverter::UStructToJsonObjectString(Info, SerializedInfo);

	auto XorInfo = FAccelByteUtilities::XOR(SerializedInfo, FAccelByteUtilities::GetDeviceId());
	IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->SaveItem(Response.Platform_user_id, XorInfo,
		THandler<bool>::CreateLambda([this, OnSuccess, Response](bool IsSuccess)
		{
			// On Save Refresh Token Success
		}));
}

void User::Logout(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	Oauth2::RevokeToken(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, UserCredentialsRef.GetAccessToken()
		, FVoidHandler::CreateLambda(
			[this, OnSuccess, OnError]() 
			{
				ForgetAllCredentials();
				OnSuccess.ExecuteIfBound();
			})
		, OnError
		, SettingsRef.IamServerUrl);
}

void User::ForgetAllCredentials()
{
	FReport::Log(FString(__FUNCTION__));

	UserCredentialsRef.ForgetAll();
}

void User::Register(const FString& Username
	, const FString& Password
	, const FString& DisplayName
	, const FString& Country
	, const FString& DateOfBirth
	, const THandler<FRegisterResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	FRegisterRequest NewUserRequest;
	NewUserRequest.DisplayName  = DisplayName;
	NewUserRequest.Password     = Password;
	NewUserRequest.EmailAddress = Username;
	NewUserRequest.AuthType     = TEXT("EMAILPASSWD");
	NewUserRequest.Country      = Country;
	NewUserRequest.DateOfBirth  = DateOfBirth;
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(NewUserRequest, Content);
	
	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

void User::Registerv2(const FString& EmailAddress
	, const FString& Username
	, const FString& Password
	, const FString& DisplayName
	, const FString& Country
	, const FString& DateOfBirth
	, const THandler<FRegisterResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	FRegisterRequestv2 NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.EmailAddress = EmailAddress;
	NewUserRequest.Username = Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");
	NewUserRequest.Country = Country;
	NewUserRequest.DateOfBirth = DateOfBirth;
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(NewUserRequest, Content);
	
	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

void User::Registerv3(const FRegisterRequestv3& RegisterRequest
	, const THandler<FRegisterResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	FRegisterRequestv3 NewUserRequest;
	NewUserRequest.AcceptedPolicies = RegisterRequest.AcceptedPolicies;
	NewUserRequest.DisplayName = RegisterRequest.DisplayName;
	NewUserRequest.Password = RegisterRequest.Password;
	NewUserRequest.EmailAddress = RegisterRequest.EmailAddress;
	NewUserRequest.Username = RegisterRequest.Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");
	NewUserRequest.Country = RegisterRequest.Country;
	NewUserRequest.DateOfBirth = RegisterRequest.DateOfBirth;
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(NewUserRequest, Content);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

void User::GetData(const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/users/me")
		, *SettingsRef.IamServerUrl);

	const TDelegate<void(FAccountUserData const&)> OnSuccessHttpClient =
		THandler<FAccountUserData>::CreateLambda(
			[this, OnSuccess, OnError](const FAccountUserData& AccountUserData)
			{
				UserCredentialsRef.SetAccountUserData(AccountUserData);
				OnSuccess.ExecuteIfBound(AccountUserData);
			});

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccessHttpClient, OnError);
}

void User::UpdateUser(FUserUpdateRequest UpdateRequest
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!UpdateRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("Cannot update user email using this function. Use UpdateEmail instead."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, UpdateRequest, OnSuccess, OnError);
}

void User::UpdateEmail(FUpdateEmailRequest UpdateEmailRequest
	, const FVoidHandler & OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/email")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, UpdateEmailRequest, OnSuccess, OnError);
}

void User::BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType
	, const TArray<FString>& OtherPlatformUserId
	, const THandler<FBulkPlatformUserIdResponse>& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformString = FAccelByteUtilities::GetPlatformString(PlatformType);
	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/platforms/%s/users")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace()
		, *PlatformString);

	const FBulkPlatformUserIdRequest UserIdRequests{ OtherPlatformUserId };

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, UserIdRequests, OnSuccess, OnError);
}

void User::SendVerificationCode(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef.GetUserEmailAddress().IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("User email address cannot be empty"));
	}

	FVerificationCodeRequest SendVerificationCodeRequest
	{
		EVerificationContext::UserAccountRegistration,
		TEXT(""),
		CredentialsRef.GetUserEmailAddress()
	};

	SendVerificationCode(SendVerificationCodeRequest, OnSuccess, OnError);
}

void User::SendUpdateEmailVerificationCode(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef.GetUserEmailAddress().IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("User email address cannot be empty"));
	}

	FVerificationCodeRequest SendVerificationCodeRequest
	{
		EVerificationContext::UpdateEmailAddress,
		TEXT(""),
		CredentialsRef.GetUserEmailAddress()
	};

	SendVerificationCode(SendVerificationCodeRequest, OnSuccess, OnError);
}

void User::SendUpgradeVerificationCode(const FString& EmailAddress
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FVerificationCodeRequest SendUpgradeVerificationCodeRequest
	{
		EVerificationContext::upgradeHeadlessAccount,
		TEXT(""),
		EmailAddress
	};

	SendVerificationCode(SendUpgradeVerificationCodeRequest, OnSuccess, OnError);
}

void User::UpgradeAndVerify(const FString& Username
	, const FString& Password
	, const FString& VerificationCode
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/headless/code/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	const FString Content = FString::Printf(TEXT("{ \"code\": \"%s\", \"emailAddress\": \"%s\", \"password\": \"%s\"}")
		, *VerificationCode
		, *Username
		, *Password);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void User::UpgradeAndVerify2(const FUpgradeAndVerifyRequest& UpgradeAndVerifyRequest
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/headless/code/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());
	
	HttpClient.ApiRequest(TEXT("POST"), Url, {}, UpgradeAndVerifyRequest, OnSuccess, OnError);
}

void User::Upgrade(const FString& Username
	, const FString& Password
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError
	, bool bNeedVerificationCode)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/headless/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	const FString Content = FString::Printf(TEXT("{ \"EmailAddress\": \"%s\", \"Password\": \"%s\"}")
		, *Username
		, *Password);

	TMultiMap<FString, FString> QueryParams ({
		{"needVerificationCode", bNeedVerificationCode ? TEXT("true") : TEXT("false")}
	});
	
	HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, Content, OnSuccess, OnError);
}

void User::Upgradev2(const FString& EmailAddress
	, const FString& Username
	, const FString& Password
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/headless/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	const FString Content = FString::Printf(TEXT("{ \"emailAddress\": \"%s\", \"password\": \"%s\", \"username\": \"%s\"}")
		, *EmailAddress
		, *Password
		, *Username);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void User::Verify(const FString& VerificationCode
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/code/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	const FString Content = FString::Printf(TEXT("{ \"Code\": \"%s\",\"ContactType\":\"%s\"}")
		, *VerificationCode
		, TEXT("email"));

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void User::SendResetPasswordCode(const FString& EmailAddress
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/forgot")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	const FString Content= FString::Printf(TEXT("{\"emailAddress\": \"%s\"}"), *EmailAddress);
	
	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

void User::ResetPassword(const FString& VerificationCode
	, const FString& EmailAddress
	, const FString& NewPassword
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/reset")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	FResetPasswordRequest ResetPasswordRequest;
	ResetPasswordRequest.Code           = VerificationCode;
	ResetPasswordRequest.EmailAddress   = EmailAddress;
	ResetPasswordRequest.NewPassword    = NewPassword;
	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(ResetPasswordRequest, Content);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

void User::GetPlatformLinks(const THandler<FPagedPlatformLinks>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/platforms")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void User::LinkOtherPlatform(EAccelBytePlatformType PlatformType
	, const FString& Ticket
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	LinkOtherPlatform(PlatformType
		, Ticket
		, OnSuccess
		, FCustomErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& Result)
			{
				FString JsonString;
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
				TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(Result);
				FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
				FErrorOAuthInfo ErrorOauthInfo;
				FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage,ErrorOauthInfo);
			}));
}
	
void User::LinkOtherPlatform(EAccelBytePlatformType PlatformType
	, const FString& Ticket
	, const FVoidHandler& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace()
		, *PlatformId);

	const FString Content = FString::Printf(TEXT("ticket=%s"), *FGenericPlatformHttp::UrlEncode(*Ticket));

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnError);
}

void User::ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType
	, const FString& PlatformUserId
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	ForcedLinkOtherPlatform(PlatformType
		, PlatformUserId
		, OnSuccess
		, FCustomErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& Result)
			{
				FString JsonString;
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
				TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(Result);
				FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
				FErrorOAuthInfo ErrorOauthInfo;
				FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage,ErrorOauthInfo);
			}));
}
	
void User::ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType
	, const FString& PlatformUserId
	, const FVoidHandler& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/platforms/link")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace()
		, *CredentialsRef.GetUserId());

	auto PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	FLinkPlatformAccountRequest LinkRequest;
	LinkRequest.PlatformId = PlatformId;
	LinkRequest.PlatformUserId = PlatformUserId;

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, LinkRequest, OnSuccess, OnError);
}

void User::UnlinkOtherPlatform(EAccelBytePlatformType PlatformType
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace()
		, *PlatformId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void User::SendVerificationCode(const FVerificationCodeRequest& VerificationCodeRequest
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/code/request")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, VerificationCodeRequest, OnSuccess, OnError);
}

void User::SearchUsers(const FString& Query
	, EAccelByteSearchType By
	, const THandler<FPagedPublicUsersInfo>& OnSuccess
	, const FErrorHandler& OnError
	, const int32& Offset
	, const int32& Limit)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	FString SearchId;
	if (By != EAccelByteSearchType::ALL)
	{
		SearchId = SearchStrings[static_cast<std::underlying_type<EAccelByteSearchType>::type>(By)];
	}

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("query"), Query },
		{ TEXT("by"), SearchId },
		{ TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

void User::SearchUsers(const FString& Query
	, const THandler<FPagedPublicUsersInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	SearchUsers(Query, EAccelByteSearchType::ALL, OnSuccess, OnError);
}

void User::SearchUsers(const FString& Query
	, int32 Offset
	, int32 Limit
	, const THandler<FPagedPublicUsersInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	SearchUsers(Query, EAccelByteSearchType::ALL, OnSuccess, OnError, Offset, Limit);
}
	
void User::GetUserByUserId(const FString& UserID
	, const THandler<FSimpleUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserID);

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

void User::GetUserByOtherPlatformUserId(EAccelBytePlatformType PlatformType
	, const FString& OtherPlatformUserId
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId      = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/platforms/%s/users/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace()
		, *PlatformId
		, *OtherPlatformUserId);

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::GetCountryFromIP(const THandler<FCountryInfo>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/location/country")
		, *SettingsRef.IamServerUrl);
	
	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("GET"), Url, FString(), Headers, OnSuccess, OnError);
}

void User::GetUserEligibleToPlay(const THandler<bool>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	auto onItemInfoGot = THandler<FAccelByteModelsItemInfo>::CreateLambda(
		[this, OnSuccess, OnError](const FAccelByteModelsItemInfo& itemInfoResult)
		{

			TArray<FString> itemIds;
			TArray<FString> skus = itemInfoResult.Features;
			TArray<FString> appIds;
			appIds.Init(*SettingsRef.AppId, 1);

			FRegistry::Entitlement.GetUserEntitlementOwnershipAny(itemIds, appIds, skus, THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([OnSuccess, OnError](FAccelByteModelsEntitlementOwnership ownership)
			{
				OnSuccess.ExecuteIfBound(ownership.Owned);
			}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMsg)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMsg);
			}));
		});

	// #TODO: Change the FRegistry implementation with ApiClient
	FRegistry::Item.GetItemByAppId(*SettingsRef.AppId
		, "", "", onItemInfoGot, FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMsg)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMsg);
	}));
}

void User::BulkGetUserInfo(const TArray<FString>& UserIds
	, const THandler<FListBulkUserInfo>& OnSuccess
	, const FErrorHandler& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	if (UserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserIds cannot be empty!"));
		return;
	}

	const FListBulkUserInfoRequest UserList{ UserIds };

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/bulk/basic")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(UserList, Content);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

void User::GetInputValidations(const FString& LanguageCode
	, THandler<FInputValidation> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bDefaultOnEmpty)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/inputValidations")
		, *SettingsRef.IamServerUrl);

	const TMultiMap<FString, FString> QueryParams ({
		{"languageCode", *LanguageCode},
		{"defaultOnEmpty", bDefaultOnEmpty ? TEXT("true") : TEXT("false")}
	});

	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("GET"), Url, QueryParams, Headers, OnSuccess, OnError);
}
	
void User::Enable2FaBackupCode(const THandler<FUser2FaBackupCode>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/backupCode/enable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::Disable2FaBackupCode(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/backupCode/disable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::GenerateBackupCode(const THandler<FUser2FaBackupCode>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/backupCode")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::GetBackupCode(const THandler<FUser2FaBackupCode>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/backupCode")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::Enable2FaAuthenticator(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/authenticator/enable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::Disable2FaAuthenticator(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/authenticator/disable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::GenerateSecretKeyFor2FaAuthenticator(const THandler<FUser2FaSecretKey>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/authenticator/key")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::GetEnabled2FaFactors(const THandler<FUser2FaMethod>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/factor")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::MakeDefault2FaFactors(EAccelByteLoginAuthFactorType AuthFactorType
	, const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Factor = FAccelByteUtilities::GetAuthenticatorString(AuthFactorType);

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/factor")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	const FString Content = FString::Printf(TEXT("factor=%s"), *Factor);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnError);
}

void User::UpdateUserV3(FUserUpdateRequest UpdateRequest
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!UpdateRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("Cannot update user email using this function. Use UpdateEmail instead."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace());

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, UpdateRequest, OnSuccess, OnError);
}

void User::GetPublisherUser(const FString& UserId
	, const THandler<FGetPublisherUserResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/publisher")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::VerifyToken(const FVoidHandler& OnSuccess, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	Oauth2::VerifyToken(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, UserCredentialsRef.GetAccessToken()
		, FVoidHandler::CreateLambda(
			[this, OnSuccess, OnError]() 
			{
				OnSuccess.ExecuteIfBound();
			})
		, OnError
		, SettingsRef.IamServerUrl);
}

void User::GetUserInformation(const FString& UserId
	, const THandler<FGetUserInformationResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/information")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef.GetNamespace()
		, *UserId);

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::GenerateOneTimeCode(EAccelBytePlatformType PlatformType
	, const THandler<FGeneratedOneTimeCode>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	const FString PlatformString = FAccelByteUtilities::GetPlatformString(PlatformType); 	
	Oauth2::GenerateOneTimeCode(UserCredentialsRef.GetAccessToken()
		, PlatformString
		, THandler<FGeneratedOneTimeCode>::CreateLambda(
			[this, OnSuccess, OnError](const FGeneratedOneTimeCode& Result) 
			{
				OnSuccess.ExecuteIfBound(Result);
			})
		, OnError
		, SettingsRef.IamServerUrl);
}

void User::GenerateGameToken(const FString& Code,
	const FVoidHandler& OnSuccess,
	const FOAuthErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__)); 
	Oauth2::GenerateGameToken(UserCredentialsRef.GetOAuthClientId()
		, UserCredentialsRef.GetOAuthClientSecret()
		, Code
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError);
			})		
		, OnError
		, SettingsRef.IamServerUrl);
}

void User::LinkHeadlessAccountToCurrentFullAccount(const FLinkHeadlessAccountRequest& Request, const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));   
	const FString Url = FString::Printf(TEXT("%s/v3/public/users/me/headless/linkWithProgression")
	   , *SettingsRef.IamServerUrl); 

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

void User::GetConflictResultWhenLinkHeadlessAccountToFullAccount(const FString& OneTimeLinkCode,
	const THandler<FConflictLinkHeadlessAccountResult>& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));   
	const FString Url = FString::Printf(TEXT("%s/v3/public/users/me/headless/link/conflict")
	   , *SettingsRef.IamServerUrl );
   
	const TMultiMap<FString, FString> QueryParams ({
		{"oneTimeLinkCode", *OneTimeLinkCode}
	}); 
	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte
