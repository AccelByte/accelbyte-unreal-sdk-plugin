// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserBlueprints.h"
#include "AccelByteUserApi.h"

using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Api::User;

void UBPUser::LoginWithClientCredentials(const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::LoginWithClientCredentials(FVoidHandler::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UBPUser::LoginWithUsername(const FString& Username, const FString& Password, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::LoginWithUsername(Username, Password, FVoidHandler::CreateLambda([OnSuccess]()
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
	User::LoginWithOtherPlatform(PlatformId, Token, FVoidHandler::CreateLambda([OnSuccess]()
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
	User::LoginWithDeviceId(FVoidHandler::CreateLambda([OnSuccess]()
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
	User::ForgetAllCredentials();
}

void UBPUser::Register(const FString& Username, const FString& Password, const FString& DisplayName, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::Register(
		Username, Password, DisplayName,
		THandler<FUserData>::CreateLambda([OnSuccess](const FUserData& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::Update(const FUserUpdateRequest& UpdateRequest, const FDUserDataHandler& OnSuccess, const FDErrorHandler & OnError)
{
	User::Update(
		UpdateRequest,
		THandler<FUserData>::CreateLambda([OnSuccess](const FUserData& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}


void UBPUser::Upgrade(const FString& Username, const FString& Password, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::Upgrade(
		Username, Password,
		THandler<FUserData>::CreateLambda([OnSuccess](const FUserData& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::SendUpgradeVerificationCode(const FString & Username, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::SendUpgradeVerificationCode(
		Username,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::UpgradeAndVerify(
		Username, Password, VerificationCode,
		THandler<FUserData>::CreateLambda([OnSuccess](const FUserData& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::SendVerificationCode(const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::SendVerificationCode(
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::Verify(const FString& VerificationCode, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	
	User::Verify(
		VerificationCode,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::SendResetPasswordCode(const FString& Username, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	
	User::SendResetPasswordCode(
		Username, 
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::ResetPassword(const FString& VerificationCode, const FString& Username, const FString& NewPassword, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::ResetPassword(
		VerificationCode, Username, NewPassword,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::GetPlatformLinks(const FDPlatformLinksHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::GetPlatformLinks(
		THandler<TArray<FPlatformLink>>::CreateLambda([OnSuccess](const TArray<FPlatformLink>& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::LinkOtherPlatform(const FString& PlatformId, const FString& Ticket, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	User::LinkOtherPlatform(
		PlatformId, Ticket,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}
	
void UBPUser::UnlinkOtherPlatform(const FString& PlatformId, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	
	User::UnlinkOtherPlatform(
		PlatformId,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}
