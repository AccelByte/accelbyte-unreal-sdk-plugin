// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteUserBlueprints.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Api::User;

void UBPUser::LoginWithUsername(const FString& Username, const FString& Password, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.LoginWithUsername(Username, Password, FVoidHandler::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UBPUser::LoginWithOtherPlatform(EAccelBytePlatformType PlatformId, const FString& Token, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.LoginWithOtherPlatform(PlatformId, Token, FVoidHandler::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UBPUser::LoginWithDeviceId(const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UBPUser::ForgetAllCredentials()
{
	FRegistry::User.ForgetAllCredentials();
}

void UBPUser::Register(const FString& Username, const FString& Password, const FString& DisplayName, const FString& Country, const FString& DateOfBirth, const FDUserRegisterHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.Register(
		Username, Password, DisplayName, Country, DateOfBirth,
		THandler<FRegisterResponse>::CreateLambda([OnSuccess](const FRegisterResponse& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}


void UBPUser::Upgrade(const FString& Username, const FString& Password, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.Upgrade(
		Username, Password,
		THandler<FUserData>::CreateLambda([OnSuccess](const FUserData& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::SendUpgradeVerificationCode(const FString & Username, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.SendUpgradeVerificationCode(
		Username,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.UpgradeAndVerify(
		Username, Password, VerificationCode,
		THandler<FUserData>::CreateLambda([OnSuccess](const FUserData& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::SendVerificationCode(const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.SendVerificationCode(
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::Verify(const FString& VerificationCode, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	
	FRegistry::User.Verify(
		VerificationCode,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::SendResetPasswordCode(const FString& Username, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	
	FRegistry::User.SendResetPasswordCode(
		Username, 
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::ResetPassword(const FString& VerificationCode, const FString& Username, const FString& NewPassword, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.ResetPassword(
		VerificationCode, Username, NewPassword,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::GetPlatformLinks(const FDPlatformLinksHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.GetPlatformLinks(
		THandler<FPagedPlatformLinks>::CreateLambda([OnSuccess](const FPagedPlatformLinks& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::LinkOtherPlatform(const FString& PlatformId, const FString& Ticket, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.LinkOtherPlatform(
		PlatformId, Ticket,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}
	
void UBPUser::UnlinkOtherPlatform(const FString& PlatformId, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	
	FRegistry::User.UnlinkOtherPlatform(
		PlatformId,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}
