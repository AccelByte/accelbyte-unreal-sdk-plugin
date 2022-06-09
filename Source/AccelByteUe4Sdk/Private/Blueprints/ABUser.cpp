#include "Blueprints/ABUser.h"
#include "JsonUtilities.h"

void UABUser::SetApiClient(FApiClientPtr NewApiClientPtr)
{
	ApiClientPtr = NewApiClientPtr;
}

#pragma region LOGIN_AND_LOGOUT

void UABUser::LoginWithUsername(
	FString const& Username,
	FString const& Password,
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.LoginWithUsername(
		Username,
		Password,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FCustomErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message, const FJsonObject& ErrorJson)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::LoginWithOtherPlatform(
	EAccelBytePlatformType const PlatformType,
	FString const& PlatformToken,
	FDHandler OnSuccess,
	FDErrorHandler OnError,
	bool bCreateHeadless)
{
	ApiClientPtr->User.LoginWithOtherPlatform(
		PlatformType,
		PlatformToken,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FCustomErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message, const FJsonObject& ErrorJson)
			{
				OnError.ExecuteIfBound(Code, Message);
			}), bCreateHeadless);
}

void UABUser::LoginWithOtherPlatformId(
	FString const& PlatformId,
	FString const& PlatformToken,
	FDHandler OnSuccess,
	FDErrorHandler OnError,
	bool bCreateHeadless)
{
	ApiClientPtr->User.LoginWithOtherPlatformId(
		PlatformId,
		PlatformToken,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FCustomErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message, const FJsonObject& ErrorJson)
			{
				OnError.ExecuteIfBound(Code, Message);
			}), bCreateHeadless);
}

void UABUser::LoginWithDeviceId(
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.LoginWithDeviceId(
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::LoginWithLauncher(
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.LoginWithLauncher(
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::LoginWithRefreshToken(
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.LoginWithRefreshToken(
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::Logout(
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.Logout(
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::ForgetAllCredentials() 
{
	ApiClientPtr->User.ForgetAllCredentials();
}

#pragma endregion

#pragma region REGISTRATION

void UABUser::Register(
	FString const& Username,
	FString const& Password,
	FString const& DisplayName,
	FString const& Country,
	FString const& DateOfBirth,
	FDRegisterResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.Register(
		Username,
		Password,
		DisplayName,
		Country,
		DateOfBirth,
		THandler<FRegisterResponse>::CreateLambda(
			[OnSuccess](FRegisterResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::Registerv2(
	FString const& EmailAddress,
	FString const& Username,
	FString const& Password,
	FString const& DisplayName,
	FString const& Country,
	FString const& DateOfBirth,
	FDRegisterResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.Registerv2(
		EmailAddress,
		Username,
		Password,
		DisplayName,
		Country,
		DateOfBirth,
		THandler<FRegisterResponse>::CreateLambda(
			[OnSuccess](FRegisterResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::Registerv3(
	FRegisterRequestv3 const& RegisterRequest,
	FDRegisterResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.Registerv3(
		RegisterRequest,
		THandler<FRegisterResponse>::CreateLambda(
			[OnSuccess](FRegisterResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

#pragma endregion

#pragma region UPGRADE_AND_VERIFY

void UABUser::Upgrade(
	FString const& Username,
	FString const& Password,
	FDAccountUserDataResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.Upgrade(
		Username,
		Password,
		THandler<FAccountUserData>::CreateLambda(
			[OnSuccess](FAccountUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::Upgradev2(
	FString const& EmailAddress,
	FString const& Username,
	FString const& Password,
	FDAccountUserDataResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.Upgradev2(
		EmailAddress,
		Username,
		Password,
		THandler<FAccountUserData>::CreateLambda(
			[OnSuccess](FAccountUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::SendVerificationCode(
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.SendVerificationCode(
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::Verify(
	FString const& VerificationCode,
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.Verify(
		VerificationCode,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::SendUpgradeVerificationCode(
	FString const& Username,
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.SendUpgradeVerificationCode(
		Username,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::UpgradeAndVerify(
	FString const& Username,
	FString const& Password,
	FString const& VerificationCode,
	FDAccountUserDataResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.UpgradeAndVerify(
		Username,
		Password,
		VerificationCode,
		THandler<FAccountUserData>::CreateLambda(
			[OnSuccess](FAccountUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

#pragma endregion

#pragma region RESET_PASSWORD

void UABUser::SendResetPasswordCode(
	FString const& Username,
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.SendResetPasswordCode(
		Username,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::ResetPassword(
	FString const& VerificationCode,
	FString const& Username,
	FString const& NewPassword,
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.ResetPassword(
		VerificationCode,
		Username,
		NewPassword,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

#pragma endregion

#pragma region UPDATE

void UABUser::SendUpdateEmailVerificationCode(
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.SendUpdateEmailVerificationCode(
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::UpdateEmail(
	FUpdateEmailRequest UpdateEmailRequest,
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.UpdateEmail(
		UpdateEmailRequest,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::UpdateUser(
	FUserUpdateRequest UpdateRequest,
	FDAccountUserDataResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.UpdateUser(
		UpdateRequest,
		THandler<FAccountUserData>::CreateLambda(
			[OnSuccess](FAccountUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

#pragma endregion

#pragma region PLATFORM_LINK

void UABUser::GetPlatformLinks(
	FDPagedPlatformLinksResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetPlatformLinks(
		THandler<FPagedPlatformLinks>::CreateLambda(
			[OnSuccess](FPagedPlatformLinks const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::LinkOtherPlatform(
	EAccelBytePlatformType PlatformType,
	FString const& Ticket,
	FDHandler OnSuccess,
	FDAccountLinkConflictErrorJsonResponse OnError) 
{
	ApiClientPtr->User.LinkOtherPlatform(
		PlatformType,
		Ticket,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FCustomErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message, FJsonObject const& MessageVariable)
			{
				FAccountLinkConflictErrorJson Result;
				TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(MessageVariable);
				FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &Result, 0, 0);
				OnError.ExecuteIfBound(Code, Message, Result);
			}));
}

void UABUser::ForcedLinkOtherPlatform(
	EAccelBytePlatformType PlatformType,
	FString const& PlatformUserId,
	FDHandler OnSuccess,
	FDAccountLinkConflictErrorJsonResponse OnError) 
{
	ApiClientPtr->User.ForcedLinkOtherPlatform(
		PlatformType,
		PlatformUserId,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FCustomErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message, FJsonObject const& MessageVariable)
			{
				FAccountLinkConflictErrorJson Result;
				TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(MessageVariable);
				FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &Result, 0, 0);
				OnError.ExecuteIfBound(Code, Message, Result);
			}));
}

void UABUser::UnlinkOtherPlatform(
	EAccelBytePlatformType PlatformType,
	FDHandler OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.UnlinkOtherPlatform(
		PlatformType,
		FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

#pragma endregion

#pragma region Query

void UABUser::SearchUsers(
	FString const& Query,
	FDPagedPublicUsersInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.SearchUsers(
		Query,
		THandler<FPagedPublicUsersInfo>::CreateLambda(
			[OnSuccess](FPagedPublicUsersInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::SearchUsersByType(
	FString const& Query,
	EAccelByteSearchType By,
	FDPagedPublicUsersInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.SearchUsers(
		Query,
		By,
		THandler<FPagedPublicUsersInfo>::CreateLambda(
			[OnSuccess](FPagedPublicUsersInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}


void UABUser::GetUserByUserId(
	FString const& UserId,
	FDSimpleUserDataResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetUserByUserId(
		UserId,
		THandler<FSimpleUserData>::CreateLambda(
			[OnSuccess](FSimpleUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::GetUserByOtherPlatformUserId(
	EAccelBytePlatformType PlatformType,
	FString const& OtherPlatformUserId,
	FDAccountUserDataResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetUserByOtherPlatformUserId(
		PlatformType,
		OtherPlatformUserId,
		THandler<FAccountUserData>::CreateLambda(
			[OnSuccess](FAccountUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::BulkGetUserByOtherPlatformUserIds(
	EAccelBytePlatformType PlatformType,
	TArray<FString> const& OtherPlatformUserId,
	FDBulkPlatformUserIdResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.BulkGetUserByOtherPlatformUserIds(
		PlatformType,
		OtherPlatformUserId,
		THandler<FBulkPlatformUserIdResponse>::CreateLambda(
			[OnSuccess](FBulkPlatformUserIdResponse const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::BulkGetUserInfo(
	TArray<FString> const& UserIds,
	FDListBulkUserInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.BulkGetUserInfo(
		UserIds,
		THandler<FListBulkUserInfo>::CreateLambda(
			[OnSuccess](FListBulkUserInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

#pragma endregion

#pragma region MISCELLANEOUS

void UABUser::GetCountryFromIP(
	FDCountryInfoResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetCountryFromIP(
		THandler<FCountryInfo>::CreateLambda(
			[OnSuccess](FCountryInfo const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::GetUserEligibleToPlay(
	FDBooleanResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetUserEligibleToPlay(
		THandler<bool>::CreateLambda(
			[OnSuccess](bool const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

void UABUser::GetData(
	FDAccountUserDataResponse OnSuccess,
	FDErrorHandler OnError) 
{
	ApiClientPtr->User.GetData(
		THandler<FAccountUserData>::CreateLambda(
			[OnSuccess](FAccountUserData const& Response)
			{
				OnSuccess.ExecuteIfBound(Response);
			}),
		FErrorHandler::CreateLambda(
			[OnError](int Code, FString const& Message)
			{
				OnError.ExecuteIfBound(Code, Message);
			}));
}

#pragma endregion

