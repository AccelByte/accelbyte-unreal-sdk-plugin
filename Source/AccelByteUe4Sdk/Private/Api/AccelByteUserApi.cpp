// Copyright (c) 2018 - 2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.


#include "Api/AccelByteUserApi.h"
#include "AccelByteUe4SdkModule.h"
#include "Api/AccelByteEntitlementApi.h"
#include "Api/AccelByteItemApi.h"
#include "Core/AccelByteRegistry.h"
#include "Core/AccelByteReport.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteHttpListenerExtension.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteEnvironment.h"
#include "Core/AccelByteOauth2Api.h"
#include "Core/AccelByteUtilities.h"
#include "Core/IAccelByteDataStorage.h"

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
	, UserCredentialsRef{InCredentialsRef.AsShared()}
{
}

User::~User()
{}

static FString SearchStrings[] =
{
	TEXT(""),
	TEXT("displayName"),
	TEXT("username"),
	TEXT("thirdPartyPlatform"),
};

void User::FinalPreLoginEvents()
{
	if (UserCredentialsRef->GetSessionState() == Credentials::ESessionState::Valid)
	{
		UserCredentialsRef->ForgetAll();
	}
}

void User::TriggerInvalidRequestError(const FString& ErrorMessage
	, const FOAuthErrorHandler& OnError)
{
	FErrorOAuthInfo ErrorOAuth{};
	ErrorOAuth.ErrorCode = static_cast<int32>(ErrorCodes::InvalidRequest);
	ErrorOAuth.ErrorMessage = ErrorMessage;
	OnError.ExecuteIfBound(ErrorOAuth.ErrorCode, ErrorOAuth.ErrorMessage, ErrorOAuth);
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

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
    	
	Oauth2::GetTokenWithPasswordCredentials(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError, Username](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Username);
			})
		, OnError
		, SettingsRef.IamServerUrl);
	
	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
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

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
	
	Oauth2::GetTokenWithPasswordCredentialsV3(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError, Username](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Username);
			})
		, OnError
		, bRememberMe
		, SettingsRef.IamServerUrl);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithUsernameV4(const FString& Username, const FString& Password, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError, const bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return;
	}

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use email instead"));
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	Oauth2::GetTokenWithPasswordCredentialsV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError, Username](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					ProcessLoginResponse(Result, FVoidHandler::CreateLambda([OnSuccess]() 
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), OnError, Username);
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, OnError
				, bRememberMe
				, SettingsRef.IamServerUrl);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithDeviceId(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError
	, bool bCreateHeadless)
{
	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	LoginWithDeviceId(OnSuccess, OnErrorHandler, bCreateHeadless);
} 

void User::LoginWithDeviceId(const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError
	, bool bCreateHeadless)
{ 
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
	
	Oauth2::GetTokenWithDeviceId(CredentialsRef->GetOAuthClientId()
		, CredentialsRef->GetOAuthClientSecret()
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess](const FOauth2Token& Result)
			{
				OnLoginSuccess(OnSuccess, Result, Result.Platform_user_id); // Curry to general handler	
			})
		, OnError
		, SettingsRef.IamServerUrl
		, bCreateHeadless);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithDeviceIdV4(const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError, bool bCreateHeadless)
{
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	Oauth2::GetTokenWithDeviceIdV4(CredentialsRef->GetOAuthClientId()
		, CredentialsRef->GetOAuthClientSecret()
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					OnLoginSuccess(FVoidHandler::CreateLambda([OnSuccess]()
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), Result, Result.Platform_user_id); // Curry to general handler	
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, OnError
				, SettingsRef.IamServerUrl
				, bCreateHeadless);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
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

	LoginWithOtherPlatformId(FAccelByteUtilities::GetPlatformString(PlatformType)
		, PlatformToken
		, OnSuccess
		, OnError
		, bCreateHeadless);
}

void User::LoginWithOtherPlatformId(const FString& PlatformId
	, const FString& PlatformToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError
	, bool bCreateHeadless)
{
	FReport::Log(FString(__FUNCTION__));

	if (PlatformId.IsEmpty() || PlatformId == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid PlatformType or PlatformId"), OnError);
		return;
	}

	if (PlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Platform Token is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
	
	Oauth2::GetTokenWithOtherPlatformToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, PlatformId
		, PlatformToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Result.Platform_user_id);
			})
		, FOAuthErrorHandler::CreateLambda(
			[this, OnError](const int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef->SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
		, bCreateHeadless
		, SettingsRef.IamServerUrl);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithOtherPlatformV4(EAccelBytePlatformType PlatformType, const FString& PlatformToken, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError, bool bCreateHeadless)
{
	LoginWithOtherPlatformIdV4(FAccelByteUtilities::GetPlatformString(PlatformType), PlatformToken, OnSuccess, OnError, bCreateHeadless);
}

void User::LoginWithOtherPlatformIdV4(const FString& PlatformId, const FString& PlatformToken, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError, bool bCreateHeadless)
{
	FReport::Log(FString(__FUNCTION__));

	if (PlatformId.IsEmpty() || PlatformId == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid PlatformType or PlatformId"), OnError);
		return;
	}

	if (PlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Platform Token is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	Oauth2::GetTokenWithOtherPlatformTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, PlatformId
		, PlatformToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					ProcessLoginResponse(Result, FVoidHandler::CreateLambda([OnSuccess]()
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), OnError, Result.Platform_user_id);
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, FOAuthErrorHandler::CreateLambda(
			[this, OnError](const int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef->SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
				, bCreateHeadless
				, SettingsRef.IamServerUrl);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithSimultaneousPlatform(EAccelBytePlatformType NativePlatform
	, const FString& NativePlatformToken
	, EAccelBytePlatformType SecondaryPlatform
	, const FString& SecondaryPlatformToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	LoginWithSimultaneousPlatform(FAccelByteUtilities::GetPlatformString(NativePlatform)
		, NativePlatformToken
		, FAccelByteUtilities::GetPlatformString(SecondaryPlatform)
		, SecondaryPlatformToken
		, OnSuccess
		, OnError);
}

void User::LoginWithSimultaneousPlatform(const FString& NativePlatform
	, const FString& NativePlatformToken
	, const FString& SecondaryPlatform
	, const FString& SecondaryPlatformToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (NativePlatform.IsEmpty() || NativePlatform == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid Native PlatformType or PlatformId"), OnError);
		return;
	}

	if (NativePlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Native Platform Token is empty!"), OnError);
		return;
	}

	if (SecondaryPlatform.IsEmpty() || SecondaryPlatform == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid Secondary PlatformType or PlatformId"), OnError);
		return;
	}

	if (SecondaryPlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Secondary Platform Token is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
	
	Oauth2::GetTokenWithSimultaneousPlatformToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, NativePlatform
		, NativePlatformToken
		, SecondaryPlatform
		, SecondaryPlatformToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Result.Platform_user_id);
			})
		, FOAuthErrorHandler::CreateLambda(
			[this, OnError](const int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef->SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
		, SettingsRef.IamServerUrl);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithSimultaneousPlatformV4(EAccelBytePlatformType NativePlatform, const FString& NativePlatformToken, const EAccelBytePlatformType& SecondaryPlatform, const FString& SecondaryPlatformToken, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	LoginWithSimultaneousPlatformV4(FAccelByteUtilities::GetPlatformString(NativePlatform)
		, NativePlatformToken
		, FAccelByteUtilities::GetPlatformString(SecondaryPlatform)
		, SecondaryPlatformToken
		, OnSuccess
		, OnError);
}

void User::LoginWithSimultaneousPlatformV4(const FString& NativePlatform, const FString& NativePlatformToken, const FString& SecondaryPlatform, const FString& SecondaryPlatformToken, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (NativePlatform.IsEmpty() || NativePlatform == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid Native PlatformType or PlatformId"), OnError);
		return;
	}

	if (NativePlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Native Platform Token is empty!"), OnError);
		return;
	}

	if (SecondaryPlatform.IsEmpty() || SecondaryPlatform == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid Secondary PlatformType or PlatformId"), OnError);
		return;
	}

	if (SecondaryPlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Secondary Platform Token is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	Oauth2::GetTokenWithSimultaneousPlatformTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, NativePlatform
		, NativePlatformToken
		, SecondaryPlatform
		, SecondaryPlatformToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					ProcessLoginResponse(Result, FVoidHandler::CreateLambda([OnSuccess]() 
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), OnError, Result.Platform_user_id);
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, FOAuthErrorHandler::CreateLambda(
			[this, OnError](const int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef->SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
				, SettingsRef.IamServerUrl);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::VerifyLoginWithNewDevice2FAEnabled(const FString& MfaToken
	, EAccelByteLoginAuthFactorType AuthFactorType
	, const FString& Code
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError
	, bool bRememberDevice)
{
	FReport::Log(FString(__FUNCTION__));

	Oauth2::VerifyAndRememberNewDevice(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, MfaToken
		, AuthFactorType
		, Code
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
			})
		, OnError
		, bRememberDevice
		, SettingsRef.IamServerUrl);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::VerifyLoginWithNewDevice2FAEnabledV4(const FString& MfaToken, EAccelByteLoginAuthFactorType AuthFactorType, const FString& Code, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError, bool bRememberDevice)
{
	FReport::Log(FString(__FUNCTION__));

	Oauth2::VerifyAndRememberNewDeviceV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, MfaToken
		, AuthFactorType
		, Code
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					ProcessLoginResponse(Result, FVoidHandler::CreateLambda([OnSuccess]()
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), OnError, TEXT(""));
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, OnError
				, bRememberDevice
				, SettingsRef.IamServerUrl);

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
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

	FString AuthorizationCode = FAccelByteUtilities::GetAuthorizationCode();

	if (AuthorizationCode.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Authorization code is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	if (FAccelByteUtilities::IsUsingExchangeCode())
	{
		GenerateGameToken(AuthorizationCode, OnSuccess, OnError);
	}
	else
	{
		Oauth2::GetTokenWithAuthorizationCode(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, AuthorizationCode
		, SettingsRef.RedirectURI
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
			})
		, OnError
		, SettingsRef.IamServerUrl);
	}

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithLauncherV4(const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString AuthorizationCode = FAccelByteUtilities::GetAuthorizationCode();

	if (AuthorizationCode.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Authorization code is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	if (FAccelByteUtilities::IsUsingExchangeCode())
	{
		GenerateGameTokenV4(AuthorizationCode, OnSuccess, OnError);
	}
	else
	{
		Oauth2::GetTokenWithAuthorizationCodeV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, AuthorizationCode
		, SettingsRef.RedirectURI
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					ProcessLoginResponse(Result, FVoidHandler::CreateLambda([OnSuccess]()
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), OnError, TEXT(""));
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, OnError
				, SettingsRef.IamServerUrl);
	}

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
}

void User::LoginWithRefreshToken(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	LoginWithRefreshToken(UserCredentialsRef->GetRefreshToken(), OnSuccess, OnError);
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
	, const FOAuthErrorHandler& OnError
	, const FString& PlatformUserId)
{
	FReport::Log(FString(__FUNCTION__));

	if (RefreshToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Refresh Token is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	Oauth2::GetTokenWithRefreshToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, RefreshToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, PlatformUserId, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, PlatformUserId);
			})
		, OnError
		, SettingsRef.IamServerUrl);
}

void User::LoginWithRefreshTokenV4(const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError, const FString& RefreshToken, const FString& PlatformUserId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString InRefreshToken = RefreshToken.IsEmpty() ? UserCredentialsRef->GetRefreshToken() : RefreshToken;

	if (InRefreshToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Refresh Token is empty!"), OnError);
		return;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	Oauth2::GetTokenWithRefreshTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, InRefreshToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, PlatformUserId, OnSuccess, OnError](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					ProcessLoginResponse(Result, FVoidHandler::CreateLambda([OnSuccess]()
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), OnError, PlatformUserId);
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, OnError
				, SettingsRef.IamServerUrl);
}

void User::RefreshPlatformToken(EAccelBytePlatformType NativePlatform
	, const FString& NativePlatformToken
	, const THandler<FPlatformTokenRefreshResponse>& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	RefreshPlatformToken(FAccelByteUtilities::GetPlatformString(NativePlatform)
		, NativePlatformToken
		, OnSuccess
		, OnError);
}

void User::RefreshPlatformToken(const FString& NativePlatform
	, const FString& NativePlatformToken
	, const THandler<FPlatformTokenRefreshResponse>& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (NativePlatform.IsEmpty() || NativePlatform == "unknown")
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Please provide a valid Native PlatformType or PlatformId."), FErrorOAuthInfo{});
		return;
	}

	if (NativePlatformToken.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Native Platform Token is empty!"), FErrorOAuthInfo{});
		return;
	}

	Oauth2::RefreshPlatformToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, NativePlatform
		, NativePlatformToken
		, OnSuccess
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
#if PLATFORM_WINDOWS || PLATFORM_LINUX || PLATFORM_MAC
	FReport::Log(FString(__FUNCTION__));

	IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->GetItem(PlatformUserID
		, THandler<TPair<FString, FString>>::CreateLambda(
			[this, PlatformUserID, OnSuccess, OnError](TPair<FString, FString> Pair)
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

				this->LoginWithRefreshToken(RefreshInfo.RefreshToken, OnSuccess, OnError, PlatformUserID);
			})
		, FAccelByteUtilities::GetCacheFilenameGeneralPurpose());
#else
	OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::CachedTokenNotFound), TEXT("Cannot relogin using cached token on other platforms."), FErrorOAuthInfo{});
#endif
}

void User::TryReloginV4(const FString& PlatformUserID, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError)
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX || PLATFORM_MAC
	FReport::Log(FString(__FUNCTION__));

	IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->GetItem(PlatformUserID
		, THandler<TPair<FString, FString>>::CreateLambda(
			[this, PlatformUserID, OnSuccess, OnError](TPair<FString, FString> Pair)
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

				this->LoginWithRefreshTokenV4(OnSuccess, OnError, RefreshInfo.RefreshToken, PlatformUserID);
			})
		, FAccelByteUtilities::GetCacheFilenameGeneralPurpose());
#else
	OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::CachedTokenNotFound), TEXT("Cannot relogin using cached token on other platforms."), FErrorOAuthInfo{});
#endif
}

void User::CreateHeadlessAccountAndLogin(const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	CreateHeadlessAccountAndLogin(UserCredentialsRef->GetLinkingToken(), OnSuccess, OnError);
}

void User::CreateHeadlessAccountAndLogin(const FString& LinkingToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	if (LinkingToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Linking Token is empty!"), OnError);
		return;
	}

	Oauth2::CreateHeadlessAccountAndResponseToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, LinkingToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
			})	
		, OnError
		, SettingsRef.IamServerUrl); 
}

void User::CreateHeadlessAccountAndLoginV4(const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError, const FString& LinkingToken)
{
	FReport::Log(FString(__FUNCTION__));

	const FString InLinkingToken = LinkingToken.IsEmpty() ? UserCredentialsRef->GetLinkingToken() : LinkingToken;

	if (InLinkingToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Linking Token is empty!"), OnError);
		return;
	}

	Oauth2::CreateHeadlessAccountAndResponseTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, InLinkingToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					ProcessLoginResponse(Result, FVoidHandler::CreateLambda([OnSuccess]()
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), OnError, TEXT(""));
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, OnError
				, SettingsRef.IamServerUrl);
}
	
void User::AuthenticationWithPlatformLinkAndLogin(const FString& Username
	, const FString& Password
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	AuthenticationWithPlatformLinkAndLogin(Username, Password, UserCredentialsRef->GetLinkingToken(), OnSuccess, OnError);
}
	
void User::AuthenticationWithPlatformLinkAndLogin(const FString& Username
	, const FString& Password
	, const FString& LinkingToken
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return;
	}

	if (LinkingToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Linking Token is empty!"), OnError);
		return;
	}

	Oauth2::AuthenticationWithPlatformLink(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, LinkingToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
			})		
		, OnError
		, SettingsRef.IamServerUrl);
}

void User::AuthenticationWithPlatformLinkAndLoginV4(const FString& Username, const FString& Password, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError, const FString& LinkingToken)
{
	FReport::Log(FString(__FUNCTION__));

	const FString InLinkingToken = LinkingToken.IsEmpty() ? UserCredentialsRef->GetLinkingToken() : LinkingToken;

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return;
	}

	if (InLinkingToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Linking Token is empty!"), OnError);
		return;
	}

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use email instead"));
	}

	Oauth2::AuthenticationWithPlatformLinkV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, InLinkingToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					ProcessLoginResponse(Result, FVoidHandler::CreateLambda([OnSuccess]()
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), OnError, TEXT(""));
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, OnError
				, SettingsRef.IamServerUrl);
}

void User::ClaimAccessToken(const FString LoginTicket, const FVoidHandler& OnSuccess, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (LoginTicket.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Login Ticket is empty!"), OnError);
		return;
	}

	Oauth2::GetTokenWithLoginTicket(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, LoginTicket
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Result.Platform_user_id);
			})		
		, OnError
		, SettingsRef.IamServerUrl);
}

#pragma endregion /Login Methods

void User::ProcessLoginResponse(const FOauth2Token& Response
	, const FVoidHandler& OnSuccess
	, const FOAuthErrorHandler& OnError
	, const FString& CachedTokenKey)
{
	if (Response.Access_token.IsEmpty() || Response.Expires_in <= 0.0f)
	{
		FErrorOAuthInfo EmptyObject;
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidResponse), TEXT("Invalid Response. Access Token is empty"), EmptyObject);
	}
	else
	{
		OnLoginSuccess(OnSuccess, Response, CachedTokenKey);
	}
}
	
void User::OnLoginSuccess(const FVoidHandler& OnSuccess
	, const FOauth2Token& Response
	, const FString& CachedTokenKey)
{
	// Set auth token before anything: Set before anything to prevent race condition issues.
	UserCredentialsRef->SetAuthToken(Response, FPlatformTime::Seconds());

	if (!Response.Auth_Trust_Id.IsEmpty())
	{
		FAccelByteUtilities::SetAuthTrustId(Response.Auth_Trust_Id);
	}
	
	FVoidHandler CallbackFunction = FVoidHandler::CreateLambda(
		[this, OnSuccess, Response]()
		{
			UserCredentialsRef->OnLoginSuccess().Broadcast(Response); // Sets auth tokens, inits qosScheduler
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
			}),true
		);

	FHttpRetryScheduler::SetHeaderNamespace(Response.Namespace); 

	// Save Cached Token to local data storage.
	FDateTime ExpireDate = FDateTime::UtcNow() + FTimespan::FromSeconds(Response.Refresh_expires_in);
	SaveCachedTokenToLocalDataStorage(CachedTokenKey, Response.Refresh_token, ExpireDate);
}

void User::SaveCachedTokenToLocalDataStorage(const FString& CachedTokenKey, const FString& RefreshToken, FDateTime ExpireDate)
{
#if PLATFORM_WINDOWS || PLATFORM_LINUX || PLATFORM_MAC 

	// Username as Key should be used when the Player was login with Username Password/AccelByte account,
	// when the login happen the IAM service give empty string value for Platform_user_id.
	// Platform_user_id should be used when the Player was login with platform/including login with device-id.
	if (CachedTokenKey.IsEmpty())
	{
		FReport::Log(FString::Printf(TEXT("[AccelByte] Key for Cached Token can not be empty. ")));
		return;
	}

	// Store the refresh token
	FRefreshInfo Info;
	Info.RefreshToken = RefreshToken;
	Info.Expiration = ExpireDate;
	FString SerializedInfo;
	FJsonObjectConverter::UStructToJsonObjectString(Info, SerializedInfo);

	auto XorInfo = FAccelByteUtilities::XOR(SerializedInfo, FAccelByteUtilities::GetDeviceId());
	IAccelByteUe4SdkModuleInterface::Get().GetLocalDataStorage()->SaveItem(CachedTokenKey
		, XorInfo
		, THandler<bool>::CreateLambda(
			[this](bool IsSuccess)
			{
				FReport::Log(FString::Printf(TEXT("[AccelByte] Save Refresh Token %s. "),
					IsSuccess ? TEXT("Success") : TEXT("Fail")));
			})
		, FAccelByteUtilities::GetCacheFilenameGeneralPurpose());

#else 
	FReport::Log(FString::Printf(TEXT("[AccelByte] Save Cached Token to local storage is not support for this platform yet. ")));
#endif 
}

void User::Logout(const FVoidHandler& OnSuccess, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	Oauth2::RevokeToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, UserCredentialsRef->GetAccessToken()
		, FVoidHandler::CreateLambda(
			[OnSuccess, OnError, Copy = UserCredentialsRef]()
			{
				Copy->OnLogoutSuccess().Broadcast();	
				Copy->ForgetAll();
				OnSuccess.ExecuteIfBound();
			})
		, OnError
		, SettingsRef.IamServerUrl);
}

void User::ForgetAllCredentials()
{
	FReport::Log(FString(__FUNCTION__));

	UserCredentialsRef->ForgetAll();
}

void User::Register(const FString& Username
	, const FString& Password
	, const FString& DisplayName
	, const FString& Country
	, const FString& DateOfBirth
	, const THandler<FRegisterResponse>& OnSuccess
	, const FErrorHandler& OnError
	, const FString& UniqueDisplayName)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address or username is empty!"));
		return;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return;
	}

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
	if (!UniqueDisplayName.IsEmpty())
	{
		NewUserRequest.UniqueDisplayName = UniqueDisplayName;
	}
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
	, const FErrorHandler& OnError
	, const FString& UniqueDisplayName)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return;
	}

	if (Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Username is empty!"));
		return;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return;
	}

	FRegisterRequestv3 NewUserRequest;
	NewUserRequest.DisplayName = DisplayName;
	NewUserRequest.Password = Password;
	NewUserRequest.EmailAddress = EmailAddress;
	NewUserRequest.Username = Username;
	NewUserRequest.AuthType = TEXT("EMAILPASSWD");
	NewUserRequest.Country = Country;
	NewUserRequest.DateOfBirth = DateOfBirth;
	NewUserRequest.UniqueDisplayName = UniqueDisplayName;

	Registerv3(NewUserRequest, OnSuccess, OnError);
}

void User::Registerv3(const FRegisterRequestv3& RegisterRequest
	, const THandler<FRegisterResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(RegisterRequest.Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (RegisterRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return;
	}

	if (RegisterRequest.Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Username is empty!"));
		return;
	}

	if (RegisterRequest.Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(RegisterRequest, Content);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

void User::GetData(const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError
	, bool bIncludeAllPlatforms)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/users/me")
		, *SettingsRef.IamServerUrl);

	const TMultiMap<FString, FString> QueryParams ({
		{"includeAllPlatforms", bIncludeAllPlatforms ? TEXT("true") : TEXT("false")}
	}); 

	const TDelegate<void(FAccountUserData const&)> OnSuccessHttpClient =
		THandler<FAccountUserData>::CreateLambda(
			[this, OnSuccess, OnError](const FAccountUserData& AccountUserData)
			{
				UserCredentialsRef->SetAccountUserData(AccountUserData);
				OnSuccess.ExecuteIfBound(AccountUserData);
			});

	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccessHttpClient, OnError);
}

void User::UpdateUser(FUserUpdateRequest UpdateRequest
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!UpdateRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("Cannot update user email using this function. Use UpdateEmail address instead."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, UpdateRequest, OnSuccess, OnError);
}

void User::UpdateEmail(FUpdateEmailRequest UpdateEmailRequest
	, const FVoidHandler & OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (UpdateEmailRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return;
	}

	if (UpdateEmailRequest.Code.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Update Code is empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/email")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	HttpClient.ApiRequest(TEXT("PUT"), Url, {}, UpdateEmailRequest, OnSuccess, OnError);
}

void User::BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType
	, const TArray<FString>& OtherPlatformUserId
	, const THandler<FBulkPlatformUserIdResponse>& OnSuccess
	, const FErrorHandler & OnError
	, const bool bRawPuid)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformString = FAccelByteUtilities::GetPlatformString(PlatformType);
	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/platforms/%s/users")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformString);

	TMultiMap<FString, FString> QueryParams;
	if (bRawPuid)
	{
		QueryParams.Add(TEXT("rawPUID"), TEXT("true"));
	}

	const FBulkPlatformUserIdRequest UserIdRequests{ OtherPlatformUserId };

	HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, UserIdRequests, OnSuccess, OnError);
}

void User::SendVerificationCode(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetUserEmailAddress().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return;
	}

	FVerificationCodeRequest SendVerificationCodeRequest
	{
		EVerificationContext::UserAccountRegistration,
		TEXT(""),
		CredentialsRef->GetUserEmailAddress()
	};

	SendVerificationCode(SendVerificationCodeRequest, OnSuccess, OnError);
}

void User::SendUpdateEmailVerificationCode(const FVoidHandler& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetUserEmailAddress().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return;
	}

	FVerificationCodeRequest SendVerificationCodeRequest
	{
		EVerificationContext::UpdateEmailAddress,
		TEXT(""),
		CredentialsRef->GetUserEmailAddress()
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
	, const FErrorHandler& OnError
	, const FString& UniqueDisplayName)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address or Username is empty!"));
		return;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return;
	}

	if (VerificationCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Verification Code is empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/headless/code/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	const TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>();
	JsonObject->SetStringField("code", VerificationCode);
	JsonObject->SetStringField("emailAddress", Username);
	JsonObject->SetStringField("password", Password);
	if (!UniqueDisplayName.IsEmpty())
	{
		JsonObject->SetStringField("uniqueDisplayName", UniqueDisplayName);
	}

	FString Content;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

void User::UpgradeAndVerify2(const FUpgradeAndVerifyRequest& UpgradeAndVerifyRequest
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (UpgradeAndVerifyRequest.EmailAddress.IsEmpty() || UpgradeAndVerifyRequest.Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address or Username is empty!"));
		return;
	}

	if (UpgradeAndVerifyRequest.Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return;
	}

	if (UpgradeAndVerifyRequest.Code.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Verification Code is empty!"));
		return;
	}
	
	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/headless/code/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());
	
	HttpClient.ApiRequest(TEXT("POST"), Url, {}, UpgradeAndVerifyRequest, OnSuccess, OnError);
}

void User::Upgrade(const FString& Username
	, const FString& Password
	, const THandler<FAccountUserData>& OnSuccess
	, const FErrorHandler& OnError
	, bool bNeedVerificationCode)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address or Username is empty!"));
		return;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/headless/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

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

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address or Username is empty!"));
		return;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/headless/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

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

	if (VerificationCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Verification Code is empty!"));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/code/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

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

	if (EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return;
	}

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

	if (EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return;
	}

	if (NewPassword.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return;
	}

	if (VerificationCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Verification Code is empty!"));
		return;
	}

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
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

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
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	const FString Content = FString::Printf(TEXT("ticket=%s"), *FGenericPlatformHttp::UrlEncode(*Ticket));

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnError);
}

void User::LinkOtherPlatformId(const FString& PlatformId
	, const FString& Ticket
	, const FVoidHandler& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
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
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

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
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void User::UnlinkOtherPlatform(EAccelBytePlatformType PlatformType
	, const FVoidHandler& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}
	
void User::UnlinkOtherPlatformId(const FString& PlatformId
	, const FVoidHandler& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

void User::UnlinkAllOtherPlatform(EAccelBytePlatformType PlatformType
	, const FVoidHandler& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s/all")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}
	
void User::UnlinkAllOtherPlatformId(const FString& PlatformId
		, const FVoidHandler& OnSuccess
		, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s/all")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
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
		, *CredentialsRef->GetNamespace());

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, VerificationCodeRequest, OnSuccess, OnError);
}

void User::SearchUsers(const FString& Query
	, EAccelByteSearchType By
	, const THandler<FPagedPublicUsersInfo>& OnSuccess
	, const FErrorHandler& OnError
	, const int32& Offset
	, const int32& Limit
	, const FString& PlatformId
	, EAccelByteSearchPlatformType PlatformBy)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	FString SearchId;
	if (By != EAccelByteSearchType::ALL)
	{
		if (By == EAccelByteSearchType::USERNAME)
		{
			UE_LOG(LogAccelByte, Warning, TEXT("Search by username is plan to deprecated!"));
		}
		
		SearchId = SearchStrings[static_cast<std::underlying_type<EAccelByteSearchType>::type>(By)];
	}

	FString SearchPlatformBy = TEXT("");
	switch (PlatformBy)
	{
	case EAccelByteSearchPlatformType::NONE:
		SearchPlatformBy = TEXT("");
		break;
	case EAccelByteSearchPlatformType::PLATFORM_DISPLAY_NAME:
		SearchPlatformBy = TEXT("platformDisplayName");
		break;
	default:
		return;
	}

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("query"), Query },
		{ TEXT("by"), SearchId },
		{ TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("platformId"), PlatformId },
		{ TEXT("platformBy"), SearchPlatformBy },
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

void User::SearchUsers(const FString& Query
	, EAccelBytePlatformType PlatformType
	, EAccelByteSearchPlatformType PlatformBy
	, const THandler<FPagedPublicUsersInfo>& OnSuccess
	, const FErrorHandler& OnError
	, const int32 Offset
	, const int32 Limit)
{
	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);
	if (PlatformId.IsEmpty() && PlatformBy == EAccelByteSearchPlatformType::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest),
			TEXT("platformId and platformBy can not be empty, platformBy only supports value [platformDisplayName]"));
		return;
	}
	SearchUsers(Query, EAccelByteSearchType::THIRD_PARTY_PLATFORM, OnSuccess, OnError, Offset, Limit, PlatformId, PlatformBy);
}

void User::SearchUsers(const FString& Query
	, const FString& PlatformId
	, EAccelByteSearchPlatformType PlatformBy
	, const THandler<FPagedPublicUsersInfo>& OnSuccess
	, const FErrorHandler& OnError
	, const int32 Offset
	, const int32 Limit)
{
	if (PlatformId.IsEmpty() && PlatformBy == EAccelByteSearchPlatformType::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest),
			TEXT("platformId and platformBy can not be empty, platformBy only supports value [platformDisplayName]"));
		return;
	}
	SearchUsers(Query, EAccelByteSearchType::THIRD_PARTY_PLATFORM, OnSuccess, OnError, Offset, Limit, PlatformId, PlatformBy);
}	
	
void User::GetUserByUserId(const FString& UserID
	, const THandler<FSimpleUserData>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
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
		, *CredentialsRef->GetNamespace()
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
		, *CredentialsRef->GetNamespace());

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
		, *CredentialsRef->GetNamespace());

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
		, *CredentialsRef->GetNamespace());

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
		, *CredentialsRef->GetNamespace());

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
		, *CredentialsRef->GetNamespace());

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
		, *CredentialsRef->GetNamespace());

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
		, *CredentialsRef->GetNamespace());

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
		, *CredentialsRef->GetNamespace());

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
		, *CredentialsRef->GetNamespace());

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
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("UpdateUserV3 is deprecated & might be removed without notice - please use UpdateUser instead!!"));

	if (!UpdateRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Cannot update user email using this function. Use UpdateE-mail address instead."));
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, UpdateRequest, OnSuccess, OnError);
}

void User::GetPublisherUser(const FString& UserId
	, const THandler<FGetPublisherUserResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/publisher")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

void User::VerifyToken(const FVoidHandler& OnSuccess, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	Oauth2::VerifyToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, UserCredentialsRef->GetAccessToken()
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

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/information")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
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
	Oauth2::GenerateOneTimeCode(UserCredentialsRef->GetAccessToken()
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
	Oauth2::GenerateGameToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Code
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2Token& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
			})		
		, OnError
		, SettingsRef.IamServerUrl);
}

void User::GenerateGameTokenV4(const FString& Code, const THandler<FAccelByteModelsLoginQueueTicketInfo>& OnSuccess, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	Oauth2::GenerateGameTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Code
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](const FOauth2TokenV4& Result)
			{
				if (Result.Ticket.IsEmpty())
				{
					ProcessLoginResponse(Result, FVoidHandler::CreateLambda([OnSuccess]() 
						{
							OnSuccess.ExecuteIfBound(FAccelByteModelsLoginQueueTicketInfo{});
						}), OnError, TEXT(""));
				}
				else
				{
					OnSuccess.ExecuteIfBound(Result);
				}
			})
		, OnError
				, SettingsRef.IamServerUrl);
}

void User::GenerateCodeForPublisherTokenExchange(const FString& PublisherClientID,
	const THandler<FCodeForTokenExchangeResponse>& OnSuccess,
	const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 
	Oauth2::GenerateCodeForPublisherTokenExchange(UserCredentialsRef->GetAccessToken()
		, SettingsRef.PublisherNamespace
		, PublisherClientID
		, OnSuccess
		, OnError);
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

void User::CheckUserAccountAvailability(const FString& DisplayName,
	const FVoidHandler& OnSuccess, const FErrorHandler& OnError, bool bIsCheckUniqueDisplayName)
{
	FReport::Log(FString(__FUNCTION__));   
	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/availability")
	   , *SettingsRef.IamServerUrl
	   , *CredentialsRef->GetNamespace());

	const FString Field = bIsCheckUniqueDisplayName ? TEXT("uniqueDisplayName") : TEXT("displayName");
	const TMultiMap<FString, FString> QueryParams ({
		{"field", *Field},
		{"query", *DisplayName}
	}); 
	HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess,
		FErrorHandler::CreateLambda([this, OnError](int32 ErrorCode, const FString& ErrorMessage)
		{
			FString Message = ErrorMessage;
			if (ErrorCode == EHttpResponseCodes::NotFound)
			{
				Message = TEXT("Account doesn't exist. If a new account is added with the defined display name, " 
						"the service will be able to perform the action.");
				OnError.ExecuteIfBound(EHttpResponseCodes::NotFound, Message);
			}
			else
			{
				OnError.ExecuteIfBound(ErrorCode, Message);
			}
		}));
}

void User::RetrieveUserThirdPartyPlatformToken(const EAccelBytePlatformType& PlatformType,
	const THandler<FThirdPartyPlatformTokenData>& OnSuccess, const FOAuthErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	switch (PlatformType)
	{
	case EAccelBytePlatformType::PS4Web:
	case EAccelBytePlatformType::PS4:
	case EAccelBytePlatformType::PS5:
	case EAccelBytePlatformType::Twitch:
	case EAccelBytePlatformType::EpicGames:
	case EAccelBytePlatformType::AwsCognito:
		break;
	default:
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::BadRequestException), TEXT("Platform Type is not supported"), {});
		return;
	}

	const FString UserId = CredentialsRef->GetUserId();
	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);
	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef->GetAccessToken());
	Oauth2::RetrieveUserThirdPartyPlatformToken(UserId, PlatformId, Authorization, 
		THandler<FThirdPartyPlatformTokenData>::CreateLambda(
			[this, PlatformId, OnSuccess](const FThirdPartyPlatformTokenData& Result)
			{
				UserCredentialsRef->SetThridPartyPlatformTokenData(PlatformId, Result);
				OnSuccess.ExecuteIfBound(Result); 
			}), OnError);
}

void User::GetUserOtherPlatformBasicPublicInfo(const FPlatformAccountInfoRequest& Request
	, const THandler<FAccountUserPlatformInfosResponse>& OnSuccess
	, const FErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/platforms")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	TSharedPtr<FJsonObject> JsonObj = FJsonObjectConverter::UStructToJsonObject(Request);
	FAccelByteUtilities::RemoveEmptyFieldsFromJson(JsonObj, FAccelByteUtilities::FieldRemovalFlagAll, { TEXT("userIds") });
	FString Contents;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Contents);
	FJsonSerializer::Serialize(JsonObj.ToSharedRef(), Writer);

	HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, OnSuccess, OnError);
}

void User::GetAccountConfigurationValue(EAccountConfiguration ConfigKey
	, const THandler<bool>& OnSuccess
	, const FErrorHandler& OnError)
{
	FString Config;
	switch (ConfigKey)
	{
	case EAccountConfiguration::UNIQUE_DISPLAY_NAME_ENABLED:
		Config = TEXT("uniqueDisplayNameEnabled");
		break;
	case EAccountConfiguration::USERNAME_DISABLED:
		Config = TEXT("usernameDisabled");
		break;
	}

	const auto OnSuccessDelegate = THandler<FJsonObjectWrapper>::CreateLambda([OnSuccess, OnError, Config](const FJsonObjectWrapper& Result)
	{
		//const TSharedPtr<FJsonObject> *OutObject;
		bool bOutConfigValue = false;
		bool bIsFoundResult = false;
		;
		const TSharedPtr<FJsonObject>* OutObject;
		if (Result.JsonObject->TryGetObjectField("result", OutObject))
		{
			if (OutObject->Get()->TryGetBoolField(Config, bOutConfigValue))
			bIsFoundResult = true;
		}

		if (bIsFoundResult)
		{
			OnSuccess.ExecuteIfBound(bOutConfigValue);
		}
		else
		{
			OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::UnknownError), TEXT("Config value not found on the response."));
		}
	});
	
	FReport::Log(FString(__FUNCTION__));   
	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/config/%s")
	   , *SettingsRef.IamServerUrl
	   , *CredentialsRef->GetNamespace()
	   , *Config);
   
	HttpClient.ApiRequest(TEXT("GET"), Url, OnSuccessDelegate, OnError);
}
	
} // Namespace Api
} // Namespace AccelByte
