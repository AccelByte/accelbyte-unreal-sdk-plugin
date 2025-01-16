#include "Blueprints/ABUser.h"
#include "JsonUtilities.h"

using namespace AccelByte;

void UABUser::SetApiClient(FApiClientPtr const& NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

#pragma region LOGIN_AND_LOGOUT

void UABUser::LoginWithUsername(FString const& Username
	, FString const& Password
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithUsername(Username
			, Password
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::LoginWithOtherPlatform(EAccelBytePlatformType const PlatformType
	, FString const& PlatformToken
	, FDHandler OnSuccess
	, FDErrorHandler OnError
	, bool bCreateHeadless)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithOtherPlatform(PlatformType
			, PlatformToken
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo&)
				{
					OnError.ExecuteIfBound(Code, Message);
				})
					, bCreateHeadless);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::LoginWithOtherPlatformId(FString const& PlatformId
	, FString const& PlatformToken
	, FDHandler OnSuccess
	, FDErrorHandler OnError
	, bool bCreateHeadless)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithOtherPlatformId(PlatformId
			, PlatformToken
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo&)
				{
					OnError.ExecuteIfBound(Code, Message);
				})
					, bCreateHeadless);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::LoginWithDeviceId(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithDeviceId(
			FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::LoginWithLauncher(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithLauncher(
			FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::LoginWithRefreshToken(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithRefreshToken(
			FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::TryRelogin(FString PlatformUserID, FDHandler OnSuccess, FDErrorHandler OnError)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->TryRelogin(
			PlatformUserID,
			FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::Logout(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->Logout(
			FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::LoginByExchangeCodeForToken(FString Code, FDHandler OnSuccess, FDErrorHandler OnError)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GenerateGameToken(Code
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo& Info)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::LoginWithUsernameV4(FString const& Username, FString const& Password, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithUsernameV4(Username
			, Password
			, THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
				[OnSuccess](const FAccelByteModelsLoginQueueTicketInfo& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo& ErrorInfo)
				{
					OnError.ExecuteIfBound(Code, Message, ErrorInfo);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FErrorOAuthInfo{});
	}
}

void UABUser::LoginWithOtherPlatformV4(EAccelBytePlatformType PlatformType, FString const& PlatformToken, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError, bool bCreateHeadless)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithOtherPlatformV4(PlatformType
			, PlatformToken
			, THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
				[OnSuccess](const FAccelByteModelsLoginQueueTicketInfo& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo& ErrorInfo)
				{
					OnError.ExecuteIfBound(Code, Message, ErrorInfo);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FErrorOAuthInfo{});
	}
}

void UABUser::LoginWithOtherPlatformIdV4(FString const& PlatformId, FString const& PlatformToken, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError, bool bCreateHeadless)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithOtherPlatformIdV4(PlatformId
			, PlatformToken
			, THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
				[OnSuccess](const FAccelByteModelsLoginQueueTicketInfo& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo& ErrorInfo)
				{
					OnError.ExecuteIfBound(Code, Message, ErrorInfo);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FErrorOAuthInfo{});
	}
}

void UABUser::LoginWithDeviceIdV4(FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithDeviceIdV4(THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
			[OnSuccess](const FAccelByteModelsLoginQueueTicketInfo& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo& ErrorInfo)
				{
					OnError.ExecuteIfBound(Code, Message, ErrorInfo);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FErrorOAuthInfo{});
	}
}

void UABUser::LoginWithLauncherV4(FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithLauncherV4(THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
			[OnSuccess](const FAccelByteModelsLoginQueueTicketInfo& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo& ErrorInfo)
				{
					OnError.ExecuteIfBound(Code, Message, ErrorInfo);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FErrorOAuthInfo{});
	}
}

void UABUser::LoginWithRefreshTokenV4(FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LoginWithRefreshTokenV4(THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
			[OnSuccess](const FAccelByteModelsLoginQueueTicketInfo& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo& ErrorInfo)
				{
					OnError.ExecuteIfBound(Code, Message, ErrorInfo);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FErrorOAuthInfo{});
	}
}

void UABUser::TryReloginV4(FString PlatformUserID, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->TryReloginV4(
			PlatformUserID,
			THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
				[OnSuccess](const FAccelByteModelsLoginQueueTicketInfo& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo& ErrorInfo)
				{
					OnError.ExecuteIfBound(Code, Message, ErrorInfo);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FErrorOAuthInfo{});
	}
}

void UABUser::LoginByExchangeCodeForTokenV4(FString Code, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GenerateGameTokenV4(Code
			, THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
				[OnSuccess](const FAccelByteModelsLoginQueueTicketInfo& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FOAuthErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, const FErrorOAuthInfo& Info)
				{
					OnError.ExecuteIfBound(Code, Message, Info);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FErrorOAuthInfo{});
	}
}

void UABUser::ForgetAllCredentials() 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->ForgetAllCredentials();
	}
}

#pragma endregion

#pragma region REGISTRATION

void UABUser::Register(FString const& Username
	, FString const& Password
	, FString const& DisplayName
	, FString const& Country
	, FString const& DateOfBirth
	, FDRegisterResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->Register(Username
			, Password
			, DisplayName
			, Country
			, DateOfBirth
			, THandler<FRegisterResponse>::CreateLambda(
				[OnSuccess](FRegisterResponse const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::Registerv2(FString const& EmailAddress
	, FString const& Username
	, FString const& Password
	, FString const& DisplayName
	, FString const& Country
	, FString const& DateOfBirth
	, FDRegisterResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->Registerv2(EmailAddress
			, Username
			, Password
			, DisplayName
			, Country
			, DateOfBirth
			, THandler<FRegisterResponse>::CreateLambda(
				[OnSuccess](FRegisterResponse const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::Registerv3(FRegisterRequestv3 const& RegisterRequest
	, FDRegisterResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->Registerv3(RegisterRequest
			, THandler<FRegisterResponse>::CreateLambda(
				[OnSuccess](FRegisterResponse const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

#pragma endregion

#pragma region UPGRADE_AND_VERIFY

void UABUser::Upgrade(FString const& Username
	, FString const& Password
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->Upgrade(Username
			, Password
			, THandler<FAccountUserData>::CreateLambda(
				[OnSuccess](FAccountUserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::Upgradev2(FString const& EmailAddress
	, FString const& Username
	, FString const& Password
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->Upgradev2(EmailAddress
			, Username
			, Password
			, THandler<FAccountUserData>::CreateLambda(
				[OnSuccess](FAccountUserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::SendVerificationCode(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->SendVerificationCode(FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::Verify(FString const& VerificationCode
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->Verify(VerificationCode
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::SendUpgradeVerificationCode(FString const& EmailAddress
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->SendUpgradeVerificationCode(EmailAddress
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::UpgradeAndVerify(FString const& Username
	, FString const& Password
	, FString const& VerificationCode
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->UpgradeAndVerify(Username
			, Password
			, VerificationCode
			, THandler<FAccountUserData>::CreateLambda(
				[OnSuccess](FAccountUserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

#pragma endregion

#pragma region RESET_PASSWORD

void UABUser::SendResetPasswordCode(FString const& EmailAddress
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->SendResetPasswordCode(EmailAddress
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::ResetPassword(FString const& VerificationCode
	, FString const& EmailAddress
	, FString const& NewPassword
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->ResetPassword(VerificationCode
			, EmailAddress
			, NewPassword
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

#pragma endregion

#pragma region UPDATE

void UABUser::SendUpdateEmailVerificationCode(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->SendUpdateEmailVerificationCode(FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::UpdateEmail(FUpdateEmailRequest UpdateEmailRequest
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->UpdateEmail(UpdateEmailRequest
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::UpdateUser(FUserUpdateRequest UpdateRequest
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->UpdateUser(UpdateRequest
			, THandler<FAccountUserData>::CreateLambda(
				[OnSuccess](FAccountUserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

#pragma endregion

#pragma region PLATFORM_LINK

void UABUser::GetPlatformLinks(FDPagedPlatformLinksResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GetPlatformLinks(THandler<FPagedPlatformLinks>::CreateLambda(
			[OnSuccess](FPagedPlatformLinks const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::LinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& Ticket
	, FDHandler OnSuccess
	, FDAccountLinkConflictErrorJsonResponse OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->LinkOtherPlatform(PlatformType
		, Ticket
		, FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			})
		, FCustomErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message, FJsonObject const& MessageVariable)
			{
				FString JsonString;
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
				TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(MessageVariable);
				FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
				FAccountLinkConflictErrorJson Result;
				FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &Result);
				OnError.ExecuteIfBound(Code, Message, Result);
			}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FAccountLinkConflictErrorJson{});
	}
}

void UABUser::ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& PlatformUserId
	, FDHandler OnSuccess
	, FDAccountLinkConflictErrorJsonResponse OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->ForcedLinkOtherPlatform(PlatformType
			, PlatformUserId
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FCustomErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message, FJsonObject const& MessageVariable)
				{
					FString JsonString;
					TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
					TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(MessageVariable);
					FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
					FAccountLinkConflictErrorJson Result;
					FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &Result);
					OnError.ExecuteIfBound(Code, Message, Result);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FAccountLinkConflictErrorJson{});
	}
}

void UABUser::UnlinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->UnlinkOtherPlatform(PlatformType
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::UnlinkOtherPlatformWithCustomErrorHandler(EAccelBytePlatformType PlatformType
	, FDHandler OnSuccess
	, FDCustomErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->UnlinkOtherPlatform(PlatformType
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FCustomErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorObject)
				{
					FString JsonString;
					TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
					TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ErrorObject);
					FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
					FJsonObjectWrapper ErrorObjectWrapper;
					FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorObjectWrapper);
					OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorObjectWrapper);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FJsonObjectWrapper{});
	}
}

void UABUser::UnlinkOtherPlatformId(const FString& PlatformId
	, FDHandler OnSuccess
	, FDCustomErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->UnlinkOtherPlatformId(PlatformId
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FCustomErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorObject)
				{
					FString JsonString;
					TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
					TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ErrorObject);
					FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
					FJsonObjectWrapper ErrorObjectWrapper;
					FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorObjectWrapper);
					OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorObjectWrapper);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FJsonObjectWrapper{});
	}
}

void UABUser::UnlinkAllOtherPlatform(EAccelBytePlatformType PlatformType
	, FDHandler OnSuccess
	, FDCustomErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->UnlinkAllOtherPlatform(PlatformType
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FCustomErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorObject)
				{
					FString JsonString;
					TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
					TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ErrorObject);
					FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
					FJsonObjectWrapper ErrorObjectWrapper;
					FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorObjectWrapper);
					OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorObjectWrapper);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FJsonObjectWrapper{});
	}
}

void UABUser::UnlinkAllOtherPlatformId(const FString& PlatformId
	, FDHandler OnSuccess
	, FDCustomErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->UnlinkAllOtherPlatformId(PlatformId
			, FVoidHandler::CreateLambda(
				[OnSuccess]()
				{
					OnSuccess.ExecuteIfBound();
				})
			, FCustomErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorObject)
				{
					FString JsonString;
					TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
					TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ErrorObject);
					FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
					FJsonObjectWrapper ErrorObjectWrapper;
					FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorObjectWrapper);
					OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorObjectWrapper);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"), FJsonObjectWrapper{});
	}
}

#pragma endregion

#pragma region Query

void UABUser::SearchUsers(FString const& Query
	, FDPagedPublicUsersInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->SearchUsers(Query
			, THandler<FPagedPublicUsersInfo>::CreateLambda(
				[OnSuccess](FPagedPublicUsersInfo const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::SearchUsersByType(FString const& Query
	, EAccelByteSearchType By
	, FDPagedPublicUsersInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->SearchUsers(Query
			, By
			, THandler<FPagedPublicUsersInfo>::CreateLambda(
				[OnSuccess](FPagedPublicUsersInfo const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::SearchUsersByOtherPlatformType(FString const& Query
	, EAccelBytePlatformType PlatformType
	, EAccelByteSearchPlatformType PlatformBy
	, FDPagedPublicUsersInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->SearchUsers(Query
			, PlatformType
			, PlatformBy
			, THandler<FPagedPublicUsersInfo>::CreateLambda(
				[OnSuccess](FPagedPublicUsersInfo const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}), 0, 100);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::SearchUsersByOtherPlatformId(FString const& Query
	, const FString& PlatformId
	, EAccelByteSearchPlatformType PlatformBy
	, FDPagedPublicUsersInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->SearchUsers(Query
			, PlatformId
			, PlatformBy
			, THandler<FPagedPublicUsersInfo>::CreateLambda(
				[OnSuccess](FPagedPublicUsersInfo const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}), 0, 100);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}


void UABUser::GetUserByUserId(FString const& UserId
	, FDSimpleUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GetUserByUserId(UserId
			, THandler<FSimpleUserData>::CreateLambda(
				[OnSuccess](FSimpleUserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::GetUserByOtherPlatformUserId(EAccelBytePlatformType PlatformType
	, FString const& OtherPlatformUserId
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GetUserByOtherPlatformUserId(PlatformType
			, OtherPlatformUserId
			, THandler<FAccountUserData>::CreateLambda(
				[OnSuccess](FAccountUserData const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType
	, TArray<FString> const& OtherPlatformUserId
	, FDBulkPlatformUserIdResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->BulkGetUserByOtherPlatformUserIds(PlatformType
			, OtherPlatformUserId
			, THandler<FBulkPlatformUserIdResponse>::CreateLambda(
				[OnSuccess](FBulkPlatformUserIdResponse const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::BulkGetUserInfo(TArray<FString> const& UserIds
	, FDListBulkUserInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->BulkGetUserInfo(UserIds
			, THandler<FListBulkUserInfo>::CreateLambda(
				[OnSuccess](FListBulkUserInfo const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

#pragma endregion

#pragma region MISCELLANEOUS

void UABUser::GetCountryFromIP(FDCountryInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GetCountryFromIP(THandler<FCountryInfo>::CreateLambda(
			[OnSuccess](FCountryInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::GetUserEligibleToPlay(FDBooleanResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GetUserEligibleToPlay(THandler<bool>::CreateLambda(
			[OnSuccess](bool const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::GetData(FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GetData(THandler<FAccountUserData>::CreateLambda(
			[OnSuccess](FAccountUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}), false);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::GetDataWithLinkedPlatform(FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GetData(THandler<FAccountUserData>::CreateLambda(
			[OnSuccess](FAccountUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}), true);
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::GetUserOtherPlatformBasicPublicInfo(const FPlatformAccountInfoRequest& Request
	, FDAccountUserPlatformInfosResponse OnSuccess
	, FDErrorHandler OnError) 
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->GetUserOtherPlatformBasicPublicInfo(Request
			, THandler<FAccountUserPlatformInfosResponse>::CreateLambda(
				[OnSuccess](FAccountUserPlatformInfosResponse const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

void UABUser::ValidateUserInput(FUserInputValidationRequest const& UserInputValidationRequest
	, FDUserInputValidationResponse OnSuccess
	, FDErrorHandler OnError)
{
	const auto UserPtr = ApiClientPtr->GetUserApi().Pin();
	if (UserPtr.IsValid())
	{
		UserPtr->ValidateUserInput(UserInputValidationRequest
			, THandler<FUserInputValidationResponse>::CreateLambda(
				[OnSuccess](FUserInputValidationResponse const& Response)
				{
					OnSuccess.ExecuteIfBound(Response);
				})
			, FErrorHandler::CreateLambda(
				[OnError](int Code, FString const& Message)
				{
					OnError.ExecuteIfBound(Code, Message);
				}));
	}
	else
	{
		OnError.ExecuteIfBound(static_cast<int32>(AccelByte::ErrorCodes::InvalidRequest), TEXT("Api already destroyed!"));
	}
}

#pragma endregion
