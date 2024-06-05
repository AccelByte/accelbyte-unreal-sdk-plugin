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
	ApiClientPtr->User.LoginWithUsername(Username
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

void UABUser::LoginWithOtherPlatform(EAccelBytePlatformType const PlatformType
	, FString const& PlatformToken
	, FDHandler OnSuccess
	, FDErrorHandler OnError
	, bool bCreateHeadless)
{
	ApiClientPtr->User.LoginWithOtherPlatform(PlatformType
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

void UABUser::LoginWithOtherPlatformId(FString const& PlatformId
	, FString const& PlatformToken
	, FDHandler OnSuccess
	, FDErrorHandler OnError
	, bool bCreateHeadless)
{
	ApiClientPtr->User.LoginWithOtherPlatformId(PlatformId
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

void UABUser::LoginWithDeviceId(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.LoginWithDeviceId(
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

void UABUser::LoginWithLauncher(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.LoginWithLauncher(
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

void UABUser::LoginWithRefreshToken(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.LoginWithRefreshToken(
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

void UABUser::TryRelogin(FString PlatformUserID, FDHandler OnSuccess, FDErrorHandler OnError)
{
	ApiClientPtr->User.TryRelogin(
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

void UABUser::Logout(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.Logout(
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

void UABUser::LoginByExchangeCodeForToken(FString Code, FDHandler OnSuccess, FDErrorHandler OnError)
{
	ApiClientPtr->User.GenerateGameToken(Code
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

void UABUser::LoginWithUsernameV4(FString const& Username, FString const& Password, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	ApiClientPtr->User.LoginWithUsernameV4(Username
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

void UABUser::LoginWithOtherPlatformV4(EAccelBytePlatformType PlatformType, FString const& PlatformToken, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError, bool bCreateHeadless)
{
	ApiClientPtr->User.LoginWithOtherPlatformV4(PlatformType
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

void UABUser::LoginWithOtherPlatformIdV4(FString const& PlatformId, FString const& PlatformToken, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError, bool bCreateHeadless)
{
	ApiClientPtr->User.LoginWithOtherPlatformIdV4(PlatformId
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

void UABUser::LoginWithDeviceIdV4(FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	ApiClientPtr->User.LoginWithDeviceIdV4(THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
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

void UABUser::LoginWithLauncherV4(FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	ApiClientPtr->User.LoginWithLauncherV4(THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
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

void UABUser::LoginWithRefreshTokenV4(FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	ApiClientPtr->User.LoginWithRefreshTokenV4(THandler<FAccelByteModelsLoginQueueTicketInfo>::CreateLambda(
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

void UABUser::TryReloginV4(FString PlatformUserID, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	ApiClientPtr->User.TryReloginV4(
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

void UABUser::LoginByExchangeCodeForTokenV4(FString Code, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError)
{
	ApiClientPtr->User.GenerateGameTokenV4(Code
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

void UABUser::ForgetAllCredentials() 
{
	ApiClientPtr->User.ForgetAllCredentials();
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
	ApiClientPtr->User.Register(Username
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

void UABUser::Registerv2(FString const& EmailAddress
	, FString const& Username
	, FString const& Password
	, FString const& DisplayName
	, FString const& Country
	, FString const& DateOfBirth
	, FDRegisterResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.Registerv2(EmailAddress
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

void UABUser::Registerv3(FRegisterRequestv3 const& RegisterRequest
	, FDRegisterResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.Registerv3(RegisterRequest
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

#pragma endregion

#pragma region UPGRADE_AND_VERIFY

void UABUser::Upgrade(FString const& Username
	, FString const& Password
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.Upgrade(Username
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

void UABUser::Upgradev2(FString const& EmailAddress
	, FString const& Username
	, FString const& Password
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.Upgradev2(EmailAddress
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

void UABUser::SendVerificationCode(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.SendVerificationCode(FVoidHandler::CreateLambda(
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

void UABUser::Verify(FString const& VerificationCode
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.Verify(VerificationCode
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

void UABUser::SendUpgradeVerificationCode(FString const& EmailAddress
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.SendUpgradeVerificationCode(EmailAddress
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

void UABUser::UpgradeAndVerify(FString const& Username
	, FString const& Password
	, FString const& VerificationCode
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.UpgradeAndVerify(Username
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

#pragma endregion

#pragma region RESET_PASSWORD

void UABUser::SendResetPasswordCode(FString const& EmailAddress
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.SendResetPasswordCode(EmailAddress
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

void UABUser::ResetPassword(FString const& VerificationCode
	, FString const& EmailAddress
	, FString const& NewPassword
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.ResetPassword(VerificationCode
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

#pragma endregion

#pragma region UPDATE

void UABUser::SendUpdateEmailVerificationCode(FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.SendUpdateEmailVerificationCode(FVoidHandler::CreateLambda(
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

void UABUser::UpdateEmail(FUpdateEmailRequest UpdateEmailRequest
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.UpdateEmail(UpdateEmailRequest
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

void UABUser::UpdateUser(FUserUpdateRequest UpdateRequest
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.UpdateUser(UpdateRequest
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

#pragma endregion

#pragma region PLATFORM_LINK

void UABUser::GetPlatformLinks(FDPagedPlatformLinksResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetPlatformLinks(THandler<FPagedPlatformLinks>::CreateLambda(
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

void UABUser::LinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& Ticket
	, FDHandler OnSuccess
	, FDAccountLinkConflictErrorJsonResponse OnError) 
{
	ApiClientPtr->User.LinkOtherPlatform(PlatformType
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

void UABUser::ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FString const& PlatformUserId
	, FDHandler OnSuccess
	, FDAccountLinkConflictErrorJsonResponse OnError) 
{
	ApiClientPtr->User.ForcedLinkOtherPlatform(PlatformType
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

void UABUser::UnlinkOtherPlatform(EAccelBytePlatformType PlatformType
	, FDHandler OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.UnlinkOtherPlatform(PlatformType
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

void UABUser::UnlinkOtherPlatformWithCustomErrorHandler(EAccelBytePlatformType PlatformType
	, FDHandler OnSuccess
	, FDCustomErrorHandler OnError) 
{
	ApiClientPtr->User.UnlinkOtherPlatform(PlatformType
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

void UABUser::UnlinkOtherPlatformId(const FString& PlatformId
	, FDHandler OnSuccess
	, FDCustomErrorHandler OnError) 
{
	ApiClientPtr->User.UnlinkOtherPlatformId(PlatformId
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

void UABUser::UnlinkAllOtherPlatform(EAccelBytePlatformType PlatformType
	, FDHandler OnSuccess
	, FDCustomErrorHandler OnError) 
{
	ApiClientPtr->User.UnlinkAllOtherPlatform(PlatformType
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

void UABUser::UnlinkAllOtherPlatformId(const FString& PlatformId
	, FDHandler OnSuccess
	, FDCustomErrorHandler OnError) 
{
	ApiClientPtr->User.UnlinkAllOtherPlatformId(PlatformId
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

#pragma endregion

#pragma region Query

void UABUser::SearchUsers(FString const& Query
	, FDPagedPublicUsersInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.SearchUsers(Query
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

void UABUser::SearchUsersByType(FString const& Query
	, EAccelByteSearchType By
	, FDPagedPublicUsersInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.SearchUsers(Query
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

void UABUser::SearchUsersByOtherPlatformType(FString const& Query
	, EAccelBytePlatformType PlatformType
	, EAccelByteSearchPlatformType PlatformBy
	, FDPagedPublicUsersInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.SearchUsers(Query
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

void UABUser::SearchUsersByOtherPlatformId(FString const& Query
	, const FString& PlatformId
	, EAccelByteSearchPlatformType PlatformBy
	, FDPagedPublicUsersInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.SearchUsers(Query
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


void UABUser::GetUserByUserId(FString const& UserId
	, FDSimpleUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetUserByUserId(UserId
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

void UABUser::GetUserByOtherPlatformUserId(EAccelBytePlatformType PlatformType
	, FString const& OtherPlatformUserId
	, FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetUserByOtherPlatformUserId(PlatformType
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

void UABUser::BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType
	, TArray<FString> const& OtherPlatformUserId
	, FDBulkPlatformUserIdResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.BulkGetUserByOtherPlatformUserIds(PlatformType
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

void UABUser::BulkGetUserInfo(TArray<FString> const& UserIds
	, FDListBulkUserInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.BulkGetUserInfo(UserIds
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

#pragma endregion

#pragma region MISCELLANEOUS

void UABUser::GetCountryFromIP(FDCountryInfoResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetCountryFromIP(THandler<FCountryInfo>::CreateLambda(
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

void UABUser::GetUserEligibleToPlay(FDBooleanResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetUserEligibleToPlay(THandler<bool>::CreateLambda(
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

void UABUser::GetData(FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetData(THandler<FAccountUserData>::CreateLambda(
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

void UABUser::GetDataWithLinkedPlatform(FDAccountUserDataResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetData(THandler<FAccountUserData>::CreateLambda(
			[OnSuccess](FAccountUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			})
		, FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}),true);
}

void UABUser::GetUserOtherPlatformBasicPublicInfo(const FPlatformAccountInfoRequest& Request
	, FDAccountUserPlatformInfosResponse OnSuccess
	, FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetUserOtherPlatformBasicPublicInfo(Request
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

void UABUser::ValidateUserInput(FUserInputValidationRequest const& UserInputValidationRequest
	, FDUserInputValidationResponse OnSuccess
	, FDErrorHandler OnError)
{
	ApiClientPtr->User.ValidateUserInput(UserInputValidationRequest
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

#pragma endregion
