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

void User::TriggerInvalidRequestError(FString const& ErrorMessage
	, FOAuthErrorHandler const& OnError)
{
	FErrorOAuthInfo ErrorOAuth{};
	ErrorOAuth.ErrorCode = static_cast<int32>(ErrorCodes::InvalidRequest);
	ErrorOAuth.ErrorMessage = ErrorMessage;
	OnError.ExecuteIfBound(ErrorOAuth.ErrorCode, ErrorOAuth.ErrorMessage, ErrorOAuth);
}
	
#pragma region Login Methods
	
FAccelByteTaskWPtr User::LoginWithUsername(FString const& Username
	, FString const& Password
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	return LoginWithUsername(Username, Password, OnSuccess, OnErrorHandler);
}

FAccelByteTaskWPtr User::LoginWithUsername(FString const& Username
	, FString const& Password
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers
	
	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
    	
	return Oauth2::GetTokenWithPasswordCredentials(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError, Username](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Username);
			})
		, OnError
		, SettingsRef.IamServerUrl);
}

FAccelByteTaskWPtr User::LoginWithUsernameV3(FString const& Username
	, FString const& Password
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, bool bRememberMe)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	return LoginWithUsernameV3(Username, Password, OnSuccess, OnErrorHandler, bRememberMe);
}

FAccelByteTaskWPtr User::LoginWithUsernameV3(FString const& Username
	, FString const& Password
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
	
	return Oauth2::GetTokenWithPasswordCredentialsV3(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError, Username](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Username);
			})
		, OnError
		, bRememberMe
		, SettingsRef.IamServerUrl);
}

FAccelByteTaskWPtr User::LoginWithUsernameV4(FString const& Username
	, FString const& Password
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bRememberMe)
{
	FReport::Log(FString(__FUNCTION__));

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return nullptr;
	}

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use email instead"));
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);

	return Oauth2::GetTokenWithPasswordCredentialsV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError, Username](FOauth2TokenV4 const& Result)
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
}

FAccelByteTaskWPtr User::LoginWithDeviceId(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, bool bCreateHeadless)
{
	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	return LoginWithDeviceId(OnSuccess, OnErrorHandler, bCreateHeadless);
} 

FAccelByteTaskWPtr User::LoginWithDeviceId(FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bCreateHeadless)
{ 
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
	
	return Oauth2::GetTokenWithDeviceId(CredentialsRef->GetOAuthClientId()
		, CredentialsRef->GetOAuthClientSecret()
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess](FOauth2Token const& Result)
			{
				OnLoginSuccess(OnSuccess, Result, Result.Platform_user_id); // Curry to general handler	
			})
		, OnError
		, SettingsRef.IamServerUrl
		, bCreateHeadless);
}

FAccelByteTaskWPtr User::LoginWithDeviceIdV4(THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bCreateHeadless)
{
	FReport::Log(FString(__FUNCTION__));

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);

	return Oauth2::GetTokenWithDeviceIdV4(CredentialsRef->GetOAuthClientId()
		, CredentialsRef->GetOAuthClientSecret()
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess](FOauth2TokenV4 const& Result)
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
}
	
FAccelByteTaskWPtr User::LoginWithOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& PlatformToken
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	return LoginWithOtherPlatform(PlatformType, PlatformToken, OnSuccess, OnErrorHandler, true);
}

FAccelByteTaskWPtr User::LoginWithOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& PlatformToken
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bCreateHeadless
	, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams)
{
	FReport::Log(FString(__FUNCTION__));

	return LoginWithOtherPlatformId(FAccelByteUtilities::GetPlatformString(PlatformType)
		, PlatformToken
		, OnSuccess
		, OnError
		, bCreateHeadless
		, OptionalParams);
}

FAccelByteTaskWPtr User::LoginWithOtherPlatformId(FString const& PlatformId
	, FString const& PlatformToken
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bCreateHeadless
	, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams)
{
	FReport::Log(FString(__FUNCTION__));

	if (PlatformId.IsEmpty() || PlatformId == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid PlatformType or PlatformId"), OnError);
		return nullptr;
	}

	if (PlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Platform Token is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
	
	return Oauth2::GetTokenWithOtherPlatformToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, PlatformId
		, PlatformToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Result.Platform_user_id);
			})
		, FOAuthErrorHandler::CreateLambda(
			[this, OnError](const int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef->SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
		, bCreateHeadless
		, SettingsRef.IamServerUrl
		, {} //AdditionalHeaders
		, OptionalParams);
}

FAccelByteTaskWPtr User::LoginWithOtherPlatformV4(EAccelBytePlatformType PlatformType
	, FString const& PlatformToken
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bCreateHeadless
	, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams)
{
	return LoginWithOtherPlatformIdV4(FAccelByteUtilities::GetPlatformString(PlatformType), PlatformToken, OnSuccess, OnError, bCreateHeadless, OptionalParams);
}

FAccelByteTaskWPtr User::LoginWithOtherPlatformIdV4(FString const& PlatformId
	, FString const& PlatformToken
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bCreateHeadless
	, FAccelByteLoginWithOtherPlatformOptionalParameters OptionalParams)
{
	FReport::Log(FString(__FUNCTION__));

	if (PlatformId.IsEmpty() || PlatformId == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid PlatformType or PlatformId"), OnError);
		return nullptr;
	}

	if (PlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Platform Token is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);

	return Oauth2::GetTokenWithOtherPlatformTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, PlatformId
		, PlatformToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2TokenV4 const& Result)
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
			[this, OnError](const int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef->SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
		, bCreateHeadless
		, SettingsRef.IamServerUrl
		, {} //AdditionalHeaders 
		, OptionalParams);
}

FAccelByteTaskWPtr User::LoginWithSimultaneousPlatform(EAccelBytePlatformType NativePlatform
	, FString const& NativePlatformToken
	, EAccelBytePlatformType SecondaryPlatform
	, FString const& SecondaryPlatformToken
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	return LoginWithSimultaneousPlatform(FAccelByteUtilities::GetPlatformString(NativePlatform)
		, NativePlatformToken
		, FAccelByteUtilities::GetPlatformString(SecondaryPlatform)
		, SecondaryPlatformToken
		, OnSuccess
		, OnError);
}

FAccelByteTaskWPtr User::LoginWithSimultaneousPlatform(FString const& NativePlatform
	, FString const& NativePlatformToken
	, FString const& SecondaryPlatform
	, FString const& SecondaryPlatformToken
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (NativePlatform.IsEmpty() || NativePlatform == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid Native PlatformType or PlatformId"), OnError);
		return nullptr;
	}

	if (NativePlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Native Platform Token is empty!"), OnError);
		return nullptr;
	}

	if (SecondaryPlatform.IsEmpty() || SecondaryPlatform == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid Secondary PlatformType or PlatformId"), OnError);
		return nullptr;
	}

	if (SecondaryPlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Secondary Platform Token is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);
	
	return Oauth2::GetTokenWithSimultaneousPlatformToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, NativePlatform
		, NativePlatformToken
		, SecondaryPlatform
		, SecondaryPlatformToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Result.Platform_user_id);
			})
		, FOAuthErrorHandler::CreateLambda(
			[this, OnError](const int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef->SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
		, SettingsRef.IamServerUrl);
}

FAccelByteTaskWPtr User::LoginWithSimultaneousPlatformV4(EAccelBytePlatformType NativePlatform
	, FString const& NativePlatformToken
	, EAccelBytePlatformType SecondaryPlatform
	, FString const& SecondaryPlatformToken
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	return LoginWithSimultaneousPlatformV4(FAccelByteUtilities::GetPlatformString(NativePlatform)
		, NativePlatformToken
		, FAccelByteUtilities::GetPlatformString(SecondaryPlatform)
		, SecondaryPlatformToken
		, OnSuccess
		, OnError);
}

FAccelByteTaskWPtr User::LoginWithSimultaneousPlatformV4(FString const& NativePlatform
	, FString const& NativePlatformToken
	, FString const& SecondaryPlatform
	, FString const& SecondaryPlatformToken
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (NativePlatform.IsEmpty() || NativePlatform == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid Native PlatformType or PlatformId"), OnError);
		return nullptr;
	}

	if (NativePlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Native Platform Token is empty!"), OnError);
		return nullptr;
	}

	if (SecondaryPlatform.IsEmpty() || SecondaryPlatform == "unknown")
	{
		TriggerInvalidRequestError(TEXT("Please provide a valid Secondary PlatformType or PlatformId"), OnError);
		return nullptr;
	}

	if (SecondaryPlatformToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Secondary Platform Token is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);

	return Oauth2::GetTokenWithSimultaneousPlatformTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, NativePlatform
		, NativePlatformToken
		, SecondaryPlatform
		, SecondaryPlatformToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2TokenV4 const& Result)
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
			[this, OnError](const int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo)
			{
				UserCredentialsRef->SetErrorOAuth(ErrorOauthInfo);
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorOauthInfo);
			})
		, SettingsRef.IamServerUrl);
}

FAccelByteTaskWPtr User::VerifyLoginWithNewDevice2FAEnabled(FString const& MfaToken
	, EAccelByteLoginAuthFactorType AuthFactorType
	, FString const& Code
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bRememberDevice)
{
	FReport::Log(FString(__FUNCTION__));

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);

	return Oauth2::VerifyAndRememberNewDevice(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, MfaToken
		, AuthFactorType
		, Code
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
			})
		, OnError
		, bRememberDevice
		, SettingsRef.IamServerUrl);
}

FAccelByteTaskWPtr User::VerifyLoginWithNewDevice2FAEnabledV4(FString const& MfaToken
	, EAccelByteLoginAuthFactorType AuthFactorType
	, FString const& Code
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, bool bRememberDevice)
{
	FReport::Log(FString(__FUNCTION__));

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);

	return Oauth2::VerifyAndRememberNewDeviceV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, MfaToken
		, AuthFactorType
		, Code
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2TokenV4 const& Result)
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
}

FAccelByteTaskWPtr User::LoginWithLauncher(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	return LoginWithLauncher(OnSuccess, OnErrorHandler);
}
	
FAccelByteTaskWPtr User::LoginWithLauncher(FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString AuthorizationCode = FAccelByteUtilities::GetAuthorizationCode();

	if (AuthorizationCode.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Authorization code is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);

	if (FAccelByteUtilities::IsUsingExchangeCode())
	{
		return GenerateGameToken(AuthorizationCode, OnSuccess, OnError);
	}
	else
	{
		return Oauth2::GetTokenWithAuthorizationCode(UserCredentialsRef->GetOAuthClientId()
			, UserCredentialsRef->GetOAuthClientSecret()
			, AuthorizationCode
			, SettingsRef.RedirectURI
			, THandler<FOauth2Token>::CreateLambda(
				[this, OnSuccess, OnError](FOauth2Token const& Result)
				{
					ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
				})
			, OnError
			, SettingsRef.IamServerUrl);
	}
}

FAccelByteTaskWPtr User::LoginWithLauncherV4(THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString AuthorizationCode = FAccelByteUtilities::GetAuthorizationCode();

	if (AuthorizationCode.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Authorization code is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	UserCredentialsRef->SetBearerAuthRejectedHandler(HttpRef);

	if (FAccelByteUtilities::IsUsingExchangeCode())
	{
		return GenerateGameTokenV4(AuthorizationCode, OnSuccess, OnError);
	}
	else
	{
		return Oauth2::GetTokenWithAuthorizationCodeV4(UserCredentialsRef->GetOAuthClientId()
			, UserCredentialsRef->GetOAuthClientSecret()
			, AuthorizationCode
			, SettingsRef.RedirectURI
			, THandler<FOauth2TokenV4>::CreateLambda(
				[this, OnSuccess, OnError](FOauth2TokenV4 const& Result)
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
}

FAccelByteTaskWPtr User::LoginWithRefreshToken(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	return LoginWithRefreshToken(UserCredentialsRef->GetRefreshToken(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::LoginWithRefreshToken(FString const& RefreshToken
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	UE_LOG(LogAccelByte, Warning, TEXT("When 2FA is enabled, please use %s with FOAuthErrorHandler instead."), *FString(__FUNCTION__));

	auto OnErrorHandler = FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, FString const& ErrorMessage, const FErrorOAuthInfo&)
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		});
	return LoginWithRefreshToken(RefreshToken, OnSuccess, OnErrorHandler);
}

FAccelByteTaskWPtr User::LoginWithRefreshToken(FString const& RefreshToken
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& PlatformUserId)
{
	FReport::Log(FString(__FUNCTION__));

	if (RefreshToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Refresh Token is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	return Oauth2::GetTokenWithRefreshToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, RefreshToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, PlatformUserId, OnSuccess, OnError](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, PlatformUserId);
			})
		, OnError
		, SettingsRef.IamServerUrl);
}

FAccelByteTaskWPtr User::LoginWithRefreshTokenV4(THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& RefreshToken
	, FString const& PlatformUserId)
{
	FReport::Log(FString(__FUNCTION__));

	const FString InRefreshToken = RefreshToken.IsEmpty() ? UserCredentialsRef->GetRefreshToken() : RefreshToken;

	if (InRefreshToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Refresh Token is empty!"), OnError);
		return nullptr;
	}

	FinalPreLoginEvents(); // Clears CredentialsRef post-auth info, inits schedulers

	return Oauth2::GetTokenWithRefreshTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, InRefreshToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, PlatformUserId, OnSuccess, OnError](FOauth2TokenV4 const& Result)
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

FAccelByteTaskWPtr User::RefreshPlatformToken(EAccelBytePlatformType NativePlatform
	, FString const& NativePlatformToken
	, THandler<FPlatformTokenRefreshResponse> const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	return RefreshPlatformToken(FAccelByteUtilities::GetPlatformString(NativePlatform)
		, NativePlatformToken
		, OnSuccess
		, OnError);
}

FAccelByteTaskWPtr User::RefreshPlatformToken(FString const& NativePlatform
	, FString const& NativePlatformToken
	, THandler<FPlatformTokenRefreshResponse> const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (NativePlatform.IsEmpty() || NativePlatform == "unknown")
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Please provide a valid Native PlatformType or PlatformId."), FErrorOAuthInfo{});
		return nullptr;
	}

	if (NativePlatformToken.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Native Platform Token is empty!"), FErrorOAuthInfo{});
		return nullptr;
	}

	return Oauth2::RefreshPlatformToken(UserCredentialsRef->GetOAuthClientId()
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

FAccelByteTaskWPtr User::TryRelogin(FString const& PlatformUserID
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	return TryRelogin(PlatformUserID, OnSuccess, FOAuthErrorHandler::CreateLambda(
		[OnError](int32 ErrorCode, FString const& ErrorMessage, FErrorOAuthInfo const& ErrorObject )
		{
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
		})); 
}

FAccelByteTaskWPtr User::TryRelogin(FString const& PlatformUserID
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
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
	return nullptr;
}

FAccelByteTaskWPtr User::TryReloginV4(FString const& PlatformUserID
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError)
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
	return nullptr;
}

FAccelByteTaskWPtr User::CreateHeadlessAccountAndLogin(FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	return CreateHeadlessAccountAndLogin(UserCredentialsRef->GetLinkingToken(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::CreateHeadlessAccountAndLogin(FString const& LinkingToken
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	if (LinkingToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Linking Token is empty!"), OnError);
		return nullptr;
	}

	return Oauth2::CreateHeadlessAccountAndResponseToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, LinkingToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
			})	
		, OnError
		, SettingsRef.IamServerUrl); 
}

FAccelByteTaskWPtr User::CreateHeadlessAccountAndLoginV4(THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& LinkingToken)
{
	FReport::Log(FString(__FUNCTION__));

	const FString InLinkingToken = LinkingToken.IsEmpty() ? UserCredentialsRef->GetLinkingToken() : LinkingToken;

	if (InLinkingToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Linking Token is empty!"), OnError);
		return nullptr;
	}

	return Oauth2::CreateHeadlessAccountAndResponseTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, InLinkingToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2TokenV4 const& Result)
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
	
FAccelByteTaskWPtr User::AuthenticationWithPlatformLinkAndLogin(FString const& Username
	, FString const& Password
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	return AuthenticationWithPlatformLinkAndLogin(Username, Password, UserCredentialsRef->GetLinkingToken(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr User::AuthenticationWithPlatformLinkAndLogin(FString const& Username
	, FString const& Password
	, FString const& LinkingToken
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return nullptr;
	}

	if (LinkingToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Linking Token is empty!"), OnError);
		return nullptr;
	}

	return Oauth2::AuthenticationWithPlatformLink(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, LinkingToken
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
			})		
		, OnError
		, SettingsRef.IamServerUrl);
}

FAccelByteTaskWPtr User::AuthenticationWithPlatformLinkAndLoginV4(FString const& Username
	, FString const& Password
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& LinkingToken)
{
	FReport::Log(FString(__FUNCTION__));

	const FString InLinkingToken = LinkingToken.IsEmpty() ? UserCredentialsRef->GetLinkingToken() : LinkingToken;

	if (Username.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("E-mail address or username is empty!"), OnError);
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Password is empty!"), OnError);
		return nullptr;
	}

	if (InLinkingToken.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Linking Token is empty!"), OnError);
		return nullptr;
	}

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use email instead"));
	}

	return Oauth2::AuthenticationWithPlatformLinkV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Username
		, Password
		, InLinkingToken
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2TokenV4 const& Result)
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

FAccelByteTaskWPtr User::ClaimAccessToken(FString const& LoginTicket
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (LoginTicket.IsEmpty())
	{
		TriggerInvalidRequestError(TEXT("Login Ticket is empty!"), OnError);
		return nullptr;
	}

	return Oauth2::GetTokenWithLoginTicket(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, LoginTicket
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, Result.Platform_user_id);
			})		
		, OnError
		, SettingsRef.IamServerUrl);
}

#pragma endregion /Login Methods

void User::ProcessLoginResponse(FOauth2Token const& Response
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError
	, FString const& CachedTokenKey)
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
	
void User::OnLoginSuccess(FVoidHandler const& OnSuccess
	, FOauth2Token const& Response
	, FString const& CachedTokenKey)
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
			[CallbackFunction](int ErrorCode, FString const& ErrorMessage)
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

void User::SaveCachedTokenToLocalDataStorage(FString const& CachedTokenKey
	, FString const& RefreshToken
	, FDateTime const& ExpireDate)
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

FAccelByteTaskWPtr User::Logout(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Logout by directly revoking token is deprecated, please use LogoutV3."));

	return Oauth2::RevokeToken(UserCredentialsRef->GetOAuthClientId()
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

FAccelByteTaskWPtr User::LogoutV3(FVoidHandler const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/logout") , *SettingsRef.IamServerUrl);

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")},
		{TEXT("Authorization"), FString::Printf(TEXT("Bearer %s"), *CredentialsRef->GetAccessToken())}
	};

	return HttpClient.Request(TEXT("POST"), Url, FString(), Headers, FVoidHandler::CreateLambda(
			[OnSuccess, this]()
			{
				UserCredentialsRef->OnLogoutSuccess().Broadcast();
				UserCredentialsRef->ForgetAll();
				OnSuccess.ExecuteIfBound();
			}), OnError);
}

void User::ForgetAllCredentials()
{
	FReport::Log(FString(__FUNCTION__));

	UserCredentialsRef->ForgetAll();
}

FAccelByteTaskWPtr User::Register(FString const& Username
	, FString const& Password
	, FString const& DisplayName
	, FString const& Country
	, FString const& DateOfBirth
	, THandler<FRegisterResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& UniqueDisplayName
	, FString const& Code)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address or username is empty!"));
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return nullptr;
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
	NewUserRequest.Code = Code;
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
	
	return HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::Registerv2(FString const& EmailAddress
	, FString const& Username
	, FString const& Password
	, FString const& DisplayName
	, FString const& Country
	, FString const& DateOfBirth
	, THandler<FRegisterResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& UniqueDisplayName
	, FString const& Code)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return nullptr;
	}

	if (Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Username is empty!"));
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return nullptr;
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
	NewUserRequest.Code = Code;

	return Registerv3(NewUserRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::Registerv3(FRegisterRequestv3 const& RegisterRequest
	, THandler<FRegisterResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(RegisterRequest.Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (RegisterRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return nullptr;
	}

	if (RegisterRequest.Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Username is empty!"));
		return nullptr;
	}

	if (RegisterRequest.Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return nullptr;
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

	return HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetData(THandler<FAccountUserData> const& OnSuccess
	, FErrorHandler const& OnError
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccessHttpClient, OnError);
}

FAccelByteTaskWPtr User::UpdateUser(FUserUpdateRequest const& UpdateRequest
	, THandler<FAccountUserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!UpdateRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(400, TEXT("Cannot update user email using this function. Use UpdateEmail address instead."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, UpdateRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::UpdateEmail(FUpdateEmailRequest const& UpdateEmailRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (UpdateEmailRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return nullptr;
	}

	if (UpdateEmailRequest.Code.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Update Code is empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/email")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, UpdateEmailRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType
	, TArray<FString> const& OtherPlatformUserId
	, THandler<FBulkPlatformUserIdResponse> const& OnSuccess
	, FErrorHandler const& OnError
	, bool bRawPuid)
{
	FReport::Log(FString(__FUNCTION__));

	if(OtherPlatformUserId.Num() > MaximumQueryLimit)
	{
		FReport::LogDeprecated(__FUNCTION__, TEXT("The API to get more than 100 users information will be deprecated in AGS 3.79, please use BulkGetUserByOtherPlatformUserIdsV4 instead"));	// Log deprecation as warning
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

		return HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, UserIdRequests, OnSuccess, OnError);
	}

	return BulkGetUserByOtherPlatformUserIdsV4(PlatformType, OtherPlatformUserId, OnSuccess, OnError, bRawPuid);
}

FAccelByteTaskWPtr User::BulkGetUserByOtherPlatformUserIdsV4(EAccelBytePlatformType PlatformType
, TArray<FString> const& OtherPlatformUserId
, THandler<FBulkPlatformUserIdResponse> const& OnSuccess
, FErrorHandler const& OnError
, bool bRawPuid)
{
	FReport::Log(FString(__FUNCTION__));

	TArray<FString> UserIdToSend;

	if(OtherPlatformUserId.Num() > MaximumQueryLimit)
	{
		UE_LOG(LogAccelByte, Warning, TEXT("BulkGetUserByOtherPlatformUserIdsV4 contains more than 100 OtherPlatformUserId, only first 100 will be sent"));
		UserIdToSend.Append(&OtherPlatformUserId[0], MaximumQueryLimit);
	}
	else
	{
		UserIdToSend = OtherPlatformUserId;
	}

	const FString PlatformString = FAccelByteUtilities::GetPlatformString(PlatformType);
	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/platforms/%s/users")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformString);

	TMultiMap<FString, FString> QueryParams;
	if (bRawPuid)
	{
		QueryParams.Add(TEXT("rawPUID"), TEXT("true"));
	}

	const FBulkPlatformUserIdRequest UserIdRequests{ UserIdToSend };

	return HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, UserIdRequests, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SendVerificationCode(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetUserEmailAddress().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return nullptr;
	}

	FVerificationCodeRequest SendVerificationCodeRequest
	{
		EVerificationContext::UserAccountRegistration,
		TEXT(""),
		CredentialsRef->GetUserEmailAddress()
	};

	return SendVerificationCode(SendVerificationCodeRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SendUpdateEmailVerificationCode(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (CredentialsRef->GetUserEmailAddress().IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return nullptr;
	}

	FVerificationCodeRequest SendVerificationCodeRequest
	{
		EVerificationContext::UpdateEmailAddress,
		TEXT(""),
		CredentialsRef->GetUserEmailAddress()
	};

	return SendVerificationCode(SendVerificationCodeRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SendUpgradeVerificationCode(FString const& EmailAddress
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FVerificationCodeRequest SendUpgradeVerificationCodeRequest
	{
		EVerificationContext::upgradeHeadlessAccount,
		TEXT(""),
		EmailAddress
	};

	return SendVerificationCode(SendUpgradeVerificationCodeRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::UpgradeAndVerify(FString const& Username
	, FString const& Password
	, FString const& VerificationCode
	, THandler<FAccountUserData> const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& UniqueDisplayName)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address or Username is empty!"));
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return nullptr;
	}

	if (VerificationCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Verification Code is empty!"));
		return nullptr;
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

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::UpgradeAndVerify2(FUpgradeAndVerifyRequest const& UpgradeAndVerifyRequest
	, THandler<FAccountUserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (UpgradeAndVerifyRequest.EmailAddress.IsEmpty() || UpgradeAndVerifyRequest.Username.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address or Username is empty!"));
		return nullptr;
	}

	if (UpgradeAndVerifyRequest.Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return nullptr;
	}

	if (UpgradeAndVerifyRequest.Code.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Verification Code is empty!"));
		return nullptr;
	}
	
	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/headless/code/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());
	
	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, UpgradeAndVerifyRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::Upgrade(FString const& Username
	, FString const& Password
	, THandler<FAccountUserData> const& OnSuccess
	, FErrorHandler const& OnError
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
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return nullptr;
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
	
	return HttpClient.ApiRequest(TEXT("POST"), Url, QueryParams, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::Upgradev2(FString const& EmailAddress
	, FString const& Username
	, FString const& Password
	, THandler<FAccountUserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!FAccelByteUtilities::IsValidEmail(Username))
	{
		UE_LOG(LogAccelByte, Warning, TEXT("Username is plan to deprecated - please use E-mail address instead"));
	}

	if (EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address or Username is empty!"));
		return nullptr;
	}

	if (Password.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/headless/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	const FString Content = FString::Printf(TEXT("{ \"emailAddress\": \"%s\", \"password\": \"%s\", \"username\": \"%s\"}")
		, *EmailAddress
		, *Password
		, *Username);

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::Verify(FString const& VerificationCode
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (VerificationCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Verification Code is empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/code/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	const FString Content = FString::Printf(TEXT("{ \"Code\": \"%s\",\"ContactType\":\"%s\"}")
		, *VerificationCode
		, TEXT("email"));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SendResetPasswordCode(FString const& EmailAddress
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/forgot")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	const FString Content= FString::Printf(TEXT("{\"emailAddress\": \"%s\"}"), *EmailAddress);
	
	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::ResetPassword(FString const& VerificationCode
	, FString const& EmailAddress
	, FString const& NewPassword
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("E-mail address is empty!"));
		return nullptr;
	}

	if (NewPassword.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Password is empty!"));
		return nullptr;
	}

	if (VerificationCode.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Verification Code is empty!"));
		return nullptr;
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

	return HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetPlatformLinks(THandler<FPagedPlatformLinks> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/platforms")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *CredentialsRef->GetUserId());

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::LinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& Ticket
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	return LinkOtherPlatform(PlatformType
		, Ticket
		, OnSuccess
		, FCustomErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, const FJsonObject& Result)
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
	
FAccelByteTaskWPtr User::LinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& Ticket
	, FVoidHandler const& OnSuccess
	, FCustomErrorHandler const& OnError)
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
	
	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::LinkOtherPlatformId(FString const& PlatformId
	, FString const& Ticket
	, FVoidHandler const& OnSuccess
	, FCustomErrorHandler const& OnError)
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
	
	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& PlatformUserId
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	return ForcedLinkOtherPlatform(PlatformType
		, PlatformUserId
		, OnSuccess
		, FCustomErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMessage, const FJsonObject& Result)
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
	
FAccelByteTaskWPtr User::ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& PlatformUserId
	, FVoidHandler const& OnSuccess
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

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, LinkRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::UnlinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::UnlinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FVoidHandler const& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr User::UnlinkOtherPlatformId(FString const& PlatformId
	, FVoidHandler const& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::UnlinkAllOtherPlatform(EAccelBytePlatformType PlatformType
	, FVoidHandler const& OnSuccess
	, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s/all")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}
	
FAccelByteTaskWPtr User::UnlinkAllOtherPlatformId(FString const& PlatformId
		, FVoidHandler const& OnSuccess
		, const FCustomErrorHandler& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	
	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/platforms/%s/all")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *PlatformId);

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SendVerificationCode(FVerificationCodeRequest const& VerificationCodeRequest
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/code/request")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, VerificationCodeRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SearchUsers(FString const& Query
	, EAccelByteSearchType By
	, THandler<FPagedPublicUsersInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Offset
	, int32 Limit
	, FString const& PlatformId
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
		return nullptr;
	}

	const TMultiMap<FString, FString> QueryParams = {
		{ TEXT("query"), Query },
		{ TEXT("by"), SearchId },
		{ TEXT("offset"), Offset > 0 ? FString::FromInt(Offset) : TEXT("") },
		{ TEXT("limit"), Limit > 0 ? FString::FromInt(Limit) : TEXT("") },
		{ TEXT("platformId"), PlatformId },
		{ TEXT("platformBy"), SearchPlatformBy },
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SearchUsers(FString const& Query
	, THandler<FPagedPublicUsersInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	return SearchUsers(Query, EAccelByteSearchType::ALL, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SearchUsers(FString const& Query
	, int32 Offset
	, int32 Limit
	, THandler<FPagedPublicUsersInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	return SearchUsers(Query, EAccelByteSearchType::ALL, OnSuccess, OnError, Offset, Limit);
}

FAccelByteTaskWPtr User::SearchUsers(FString const& Query
	, EAccelBytePlatformType PlatformType
	, EAccelByteSearchPlatformType PlatformBy
	, THandler<FPagedPublicUsersInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Offset
	, int32 Limit)
{
	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);
	if (PlatformId.IsEmpty() && PlatformBy == EAccelByteSearchPlatformType::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest),
			TEXT("platformId and platformBy can not be empty, platformBy only supports value [platformDisplayName]"));
		return nullptr;
	}
	return SearchUsers(Query, EAccelByteSearchType::THIRD_PARTY_PLATFORM, OnSuccess, OnError, Offset, Limit, PlatformId, PlatformBy);
}

FAccelByteTaskWPtr User::SearchUsers(FString const& Query
	, FString const& PlatformId
	, EAccelByteSearchPlatformType PlatformBy
	, THandler<FPagedPublicUsersInfo> const& OnSuccess
	, FErrorHandler const& OnError
	, int32 Offset
	, int32 Limit)
{
	if (PlatformId.IsEmpty() && PlatformBy == EAccelByteSearchPlatformType::NONE)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest),
			TEXT("platformId and platformBy can not be empty, platformBy only supports value [platformDisplayName]"));
		return nullptr;
	}
	return SearchUsers(Query, EAccelByteSearchType::THIRD_PARTY_PLATFORM, OnSuccess, OnError, Offset, Limit, PlatformId, PlatformBy);
}	
	
FAccelByteTaskWPtr User::GetUserByUserId(FString const& UserID
	, THandler<FSimpleUserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("Get user public info V3 is deprecated & might be removed without notice - please use GetUserPublicInfoByUserId (V4) instead!!"));

	if (!ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserID);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetUserPublicInfoByUserId(FString const& UserID
	, THandler<FUserPublicInfoResponseV4> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserID, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserID)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/%s")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserID);

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetUserByOtherPlatformUserId(EAccelBytePlatformType PlatformType
	, FString const& OtherPlatformUserId
	, THandler<FAccountUserData> const& OnSuccess
	, FErrorHandler const& OnError)
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

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetCountryFromIP(THandler<FCountryInfo> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/location/country")
		, *SettingsRef.IamServerUrl);
	
	const TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("GET"), Url, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetUserEligibleToPlay(THandler<bool> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	auto onItemInfoGot = THandler<FAccelByteModelsItemInfo>::CreateLambda(
		[this, OnSuccess, OnError](FAccelByteModelsItemInfo const& itemInfoResult)
		{

			TArray<FString> itemIds;
			TArray<FString> skus = itemInfoResult.Features;
			TArray<FString> appIds;
			appIds.Init(*SettingsRef.AppId, 1);

			FRegistry::Entitlement.GetUserEntitlementOwnershipAny(itemIds, appIds, skus, THandler<FAccelByteModelsEntitlementOwnership>::CreateLambda([OnSuccess, OnError](FAccelByteModelsEntitlementOwnership ownership)
			{
				OnSuccess.ExecuteIfBound(ownership.Owned);
			}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, FString const& ErrorMsg)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMsg);
			}));
		});

	// #TODO: Change the FRegistry implementation with ApiClient
	return FRegistry::Item.GetItemByAppId(*SettingsRef.AppId
		, TEXT(""), TEXT(""), onItemInfoGot, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, FString const& ErrorMsg)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMsg);
			}));
}

FAccelByteTaskWPtr User::BulkGetUserInfo(TArray<FString> const& UserIds
	, THandler<FListBulkUserInfo> const& OnSuccess
	, FErrorHandler const& OnError) 
{
	FReport::Log(FString(__FUNCTION__));

	if (UserIds.Num() <= 0)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("UserIds cannot be empty!"));
		return nullptr;
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

	return HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetInputValidations(FString const& LanguageCode
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

	return HttpClient.Request(TEXT("GET"), Url, QueryParams, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::ValidateUserInput(FUserInputValidationRequest const& UserInputValidationRequest
	, THandler<FUserInputValidationResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/input/validation")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	const TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	FString Content;
	const TSharedPtr<FJsonObject> Json = FJsonObjectConverter::UStructToJsonObject(UserInputValidationRequest);
	FAccelByteUtilities::RemoveEmptyFieldsFromJson(Json);
	TSharedRef<TJsonWriter<>> const Writer = TJsonWriterFactory<>::Create(&Content);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);

	return HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}
	
FAccelByteTaskWPtr User::Enable2FaBackupCode(THandler<FUser2FaBackupCode> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/backupCode/enable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::Disable2FaBackupCode(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FDisableMfaBackupCodeRequest const& Request)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/backupCode/disable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, Request, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GenerateBackupCode(THandler<FUser2FaBackupCode> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/backupCode")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetBackupCode(THandler<FUser2FaBackupCode> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/backupCode")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::Enable2FaAuthenticator(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FString const& Code)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/authenticator/enable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	const TMap<FString, FString> Data = {
		{TEXT("code"), Code}
	};

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Data, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::Disable2FaAuthenticator(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FDisableMfaAuthenticatorRequest const& Request)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/authenticator/disable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.ApiRequest(TEXT("DELETE"), Url, {}, Request, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GenerateSecretKeyFor2FaAuthenticator(THandler<FUser2FaSecretKey> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/authenticator/key")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetEnabled2FaFactors(THandler<FUser2FaMethod> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/factor")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::MakeDefault2FaFactors(EAccelByteLoginAuthFactorType AuthFactorType
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
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

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Content, Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::UpdateUserV3(FUserUpdateRequest const& UpdateRequest
	, THandler<FAccountUserData> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	FReport::LogDeprecated(FString(__FUNCTION__),
		TEXT("UpdateUserV3 is deprecated & might be removed without notice - please use UpdateUser instead!!"));

	if (!UpdateRequest.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Cannot update user email using this function. Use UpdateE-mail address instead."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("PATCH"), Url, {}, UpdateRequest, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetPublisherUser(FString const& UserId
	, THandler<FGetPublisherUserResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/publisher")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::VerifyToken(FVoidHandler const& OnSuccess
	, const FErrorHandler & OnError)
{
	FReport::Log(FString(__FUNCTION__));

	return Oauth2::VerifyToken(UserCredentialsRef->GetOAuthClientId()
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

FAccelByteTaskWPtr User::GetUserInformation(FString const& UserId
	, THandler<FGetUserInformationResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (!ValidateAccelByteId(UserId, EAccelByteIdHypensRule::NO_HYPENS
		, FAccelByteIdValidator::GetUserIdInvalidMessage(UserId)
		, OnError))
	{
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/%s/information")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace()
		, *UserId);

	TMap<FString, FString> Headers = {
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.ApiRequest(TEXT("GET"), Url, {}, FString(), Headers, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GenerateOneTimeCode(EAccelBytePlatformType PlatformType
	, THandler<FGeneratedOneTimeCode> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 

	const FString PlatformString = FAccelByteUtilities::GetPlatformString(PlatformType); 	
	return Oauth2::GenerateOneTimeCode(UserCredentialsRef->GetAccessToken()
		, PlatformString
		, THandler<FGeneratedOneTimeCode>::CreateLambda(
			[this, OnSuccess, OnError](const FGeneratedOneTimeCode& Result) 
			{
				OnSuccess.ExecuteIfBound(Result);
			})
		, OnError
		, SettingsRef.IamServerUrl);
}

FAccelByteTaskWPtr User::GenerateGameToken(FString const& Code
	, FVoidHandler const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 
	return Oauth2::GenerateGameToken(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Code
		, THandler<FOauth2Token>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2Token const& Result)
			{
				ProcessLoginResponse(Result, OnSuccess, OnError, TEXT(""));
			})		
		, OnError
		, SettingsRef.IamServerUrl);
}

FAccelByteTaskWPtr User::GenerateGameTokenV4(FString const& Code
	, THandler<FAccelByteModelsLoginQueueTicketInfo> const& OnSuccess
	, FOAuthErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));
	return Oauth2::GenerateGameTokenV4(UserCredentialsRef->GetOAuthClientId()
		, UserCredentialsRef->GetOAuthClientSecret()
		, Code
		, THandler<FOauth2TokenV4>::CreateLambda(
			[this, OnSuccess, OnError](FOauth2TokenV4 const& Result)
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

FAccelByteTaskWPtr User::GenerateCodeForPublisherTokenExchange(FString const& PublisherClientID
	, THandler<FCodeForTokenExchangeResponse> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__)); 
	return Oauth2::GenerateCodeForPublisherTokenExchange(UserCredentialsRef->GetAccessToken()
		, SettingsRef.PublisherNamespace
		, PublisherClientID
		, OnSuccess
		, OnError);
}

FAccelByteTaskWPtr User::LinkHeadlessAccountToCurrentFullAccount(FLinkHeadlessAccountRequest const& Request
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));   
	const FString Url = FString::Printf(TEXT("%s/v3/public/users/me/headless/linkWithProgression")
	   , *SettingsRef.IamServerUrl); 

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetConflictResultWhenLinkHeadlessAccountToFullAccount(FString const& OneTimeLinkCode,
	THandler<FConflictLinkHeadlessAccountResult> const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));   
	const FString Url = FString::Printf(TEXT("%s/v3/public/users/me/headless/link/conflict")
	   , *SettingsRef.IamServerUrl );
   
	const TMultiMap<FString, FString> QueryParams ({
		{"oneTimeLinkCode", *OneTimeLinkCode}
	}); 
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::CheckUserAccountAvailability(FString const& DisplayName
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, bool bIsCheckUniqueDisplayName)
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
	return HttpClient.ApiRequest(TEXT("GET"), Url, QueryParams, FString(), OnSuccess,
		FErrorHandler::CreateLambda([this, OnError](int32 ErrorCode, FString const& ErrorMessage)
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

FAccelByteTaskWPtr User::RetrieveUserThirdPartyPlatformToken(EAccelBytePlatformType PlatformType
	, THandler<FThirdPartyPlatformTokenData> const& OnSuccess
	, FOAuthErrorHandler const& OnError)
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
		return nullptr;
	}

	const FString UserId = CredentialsRef->GetUserId();
	const FString PlatformId = FAccelByteUtilities::GetPlatformString(PlatformType);
	const FString Authorization = FString::Printf(TEXT("Bearer %s"), *CredentialsRef->GetAccessToken());
	return Oauth2::RetrieveUserThirdPartyPlatformToken(UserId, PlatformId, Authorization, 
		THandler<FThirdPartyPlatformTokenData>::CreateLambda(
			[this, PlatformId, OnSuccess](const FThirdPartyPlatformTokenData& Result)
			{
				UserCredentialsRef->SetThridPartyPlatformTokenData(PlatformId, Result);
				OnSuccess.ExecuteIfBound(Result); 
			}), OnError);
}

FAccelByteTaskWPtr User::GetUserOtherPlatformBasicPublicInfo(FPlatformAccountInfoRequest const& Request
	, THandler<FAccountUserPlatformInfosResponse> const& OnSuccess
	, FErrorHandler const& OnError)
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

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Contents, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetAccountConfigurationValue(EAccountConfiguration ConfigKey
	, THandler<bool> const& OnSuccess
	, FErrorHandler const& OnError)
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
	case EAccountConfiguration::MANDATORY_EMAIL_VERIFICATION_ENABLED:
		Config = TEXT("mandatoryEmailVerificationEnabled");
		break;
	}

	const auto OnSuccessDelegate = THandler<FJsonObjectWrapper>::CreateLambda([OnSuccess, OnError, Config](const FJsonObjectWrapper& Result)
	{
		//const TSharedPtr<FJsonObject> *OutObject;
		bool bOutConfigValue = false;
		bool bIsFoundResult = false;
		;
		const TSharedPtr<FJsonObject>* OutObject;
		if (Result.JsonObject->TryGetObjectField(TEXT("result"), OutObject))
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
	   , *SettingsRef.PublisherNamespace
	   , *Config);

	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};
	
	return HttpClient.Request(TEXT("GET"), Url, TEXT(""), Headers, OnSuccessDelegate, OnError);
}

FAccelByteTaskWPtr User::UpdatePassword(FUpdatePasswordRequest const& Request
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Request.OldPassword.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Old password can't be empty."));
		return nullptr;
	}

	if (Request.NewPassword.IsEmpty())
    {
    	OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("New password can't be empty."));
    	return nullptr;
    }

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/me/password")
		, *SettingsRef.IamServerUrl
		, *SettingsRef.Namespace);

	return HttpClient.ApiRequest(TEXT("PUT"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::EnableMfaEmail(FString const& Code
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Code.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Code can't be empty to enable Mfa email."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/email/enable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	const TMap<FString, FString> Data = {
		{TEXT("code"), Code}
	};

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Data, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::DisableMfaEmail(FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError
	, FDisableMfaEmailRequest const& Request)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/email/disable")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SendMfaCodeToEmail(const EAccelByteSendMfaEmailAction Action
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/email/code")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	const FString Request = FString::Printf(TEXT("action=%s"), *FAccelByteUtilities::GetUEnumValueAsString<EAccelByteSendMfaEmailAction>(Action));

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Request, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::GetMfaStatus(THandler<FMfaStatusResponse> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/status")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, FString(), OnSuccess, OnError);
}

FAccelByteTaskWPtr User::VerifyMfaCode(EAccelByteLoginAuthFactorType const& Factor, FString const& Code,
	THandler<FVerifyMfaResponse> const& OnSuccess, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Factor == EAccelByteLoginAuthFactorType::None)
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Factor type None is not supported."));
		return nullptr;
	}

	if (Code.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Code can't be empty to verify MFA code."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v4/public/namespaces/%s/users/me/mfa/challenge/verify")
		, *SettingsRef.IamServerUrl
		, *CredentialsRef->GetNamespace());

	const TMap<FString, FString> Data = {
		{TEXT("factor"), FAccelByteUtilities::GetAuthenticatorString(Factor)},
		{TEXT("code"), Code}
	};

	return HttpClient.ApiRequest(TEXT("POST"), Url, {}, Data, OnSuccess, OnError);
}

FAccelByteTaskWPtr User::SendVerificationCodeToNewUser(FSendVerificationCodeToNewUserRequest const& Request
	, FVoidHandler const& OnSuccess
	, FErrorHandler const& OnError)
{
	FReport::Log(FString(__FUNCTION__));

	if (Request.EmailAddress.IsEmpty())
	{
		OnError.ExecuteIfBound(static_cast<int32>(ErrorCodes::InvalidRequest), TEXT("Email address can't be empty."));
		return nullptr;
	}

	const FString Url = FString::Printf(TEXT("%s/v3/public/namespaces/%s/users/code/request")
	   , *SettingsRef.IamServerUrl
	   , *SettingsRef.PublisherNamespace);

	FString Content;
	FJsonObjectConverter::UStructToJsonObjectString(Request, Content);
	
	TMap<FString, FString> Headers = {
		{TEXT("Content-Type"), TEXT("application/json")},
		{TEXT("Accept"), TEXT("application/json")}
	};

	return HttpClient.Request(TEXT("POST"), Url, Content, Headers, OnSuccess, OnError);
}

} // Namespace Api
} // Namespace AccelByte
