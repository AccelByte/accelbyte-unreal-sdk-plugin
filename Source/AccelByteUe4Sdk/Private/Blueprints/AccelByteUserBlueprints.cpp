// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "Blueprints/AccelByteUserBlueprints.h"
#include "Api/AccelByteUserApi.h"
#include "Core/AccelByteRegistry.h"
#include "Models/AccelByteErrorModels.h"

using namespace AccelByte;
using AccelByte::THandler;
using AccelByte::FVoidHandler;
using AccelByte::FErrorHandler;
using AccelByte::Api::User;

void UBPUser::LoginWithUsername(const FString& Username, const FString& Password, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.LoginWithUsername(Username
		, Password
		, FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			})
		, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
}

void UBPUser::LoginWithOtherPlatform(EAccelBytePlatformType PlatformType, const FString& Token, const FDHandler& OnSuccess, const FDErrorHandler& OnError, bool bCreateHeadless)
{
	FRegistry::User.LoginWithOtherPlatform(PlatformType
		, Token
		, FVoidHandler::CreateLambda(
			[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			})
		, FOAuthErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo&)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			})
		, bCreateHeadless);
}

void UBPUser::LoginWithDeviceId(const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda(
		[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			})
		, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage)
			{
				OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
			}));
}

void UBPUser::Logout(const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.Logout(FVoidHandler::CreateLambda(
		[OnSuccess]()
			{
				OnSuccess.ExecuteIfBound();
			})
		, FErrorHandler::CreateLambda(
			[OnError](int32 ErrorCode, const FString& ErrorMessage)
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
		THandler<FAccountUserData>::CreateLambda([OnSuccess](const FAccountUserData& Result) { OnSuccess.ExecuteIfBound(Result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::SendUpgradeVerificationCode(const FString & EmailAddress, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.SendUpgradeVerificationCode(
		EmailAddress,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.UpgradeAndVerify(
		Username, Password, VerificationCode,
		THandler<FAccountUserData>::CreateLambda([OnSuccess](const FAccountUserData& Result) { OnSuccess.ExecuteIfBound(Result); }),
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

void UBPUser::SendResetPasswordCode(const FString& EmailAddress, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	
	FRegistry::User.SendResetPasswordCode(
		EmailAddress, 
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::ResetPassword(const FString& VerificationCode, const FString& EmailAddress, const FString& NewPassword, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.ResetPassword(
		VerificationCode, EmailAddress, NewPassword,
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

void UBPUser::LinkOtherPlatform(EAccelBytePlatformType PlatformType, const FString& Ticket, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.LinkOtherPlatform(PlatformType
		, Ticket
		, FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FOAuthErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage, const FErrorOAuthInfo& ErrorOauthInfo) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}
	
void UBPUser::UnlinkOtherPlatform(EAccelBytePlatformType PlatformType, const FDHandler& OnSuccess, const FDErrorHandler& OnError)
{
	
	FRegistry::User.UnlinkOtherPlatform(
		PlatformType,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}

void UBPUser::UnlinkOtherPlatformWithCustomErrorHandler(EAccelBytePlatformType PlatformType, const FDHandler& OnSuccess, const FDCustomErrorHandler& OnError)
{
	
	FRegistry::User.UnlinkOtherPlatform(
		PlatformType,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FCustomErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorObject)
		{
			FString JsonString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
			TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ErrorObject);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			FJsonObjectWrapper ErrorObjectWrapper;
			FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorObjectWrapper);
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorObjectWrapper);
		})
	);
}

void UBPUser::UnlinkOtherPlatformId(const FString& PlatformId, const FDHandler& OnSuccess, const FDCustomErrorHandler& OnError)
{
	
	FRegistry::User.UnlinkOtherPlatformId(
		PlatformId,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FCustomErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorObject)
		{
			FString JsonString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
			TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ErrorObject);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			FJsonObjectWrapper ErrorObjectWrapper;
			FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorObjectWrapper);
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorObjectWrapper);
		})
	);
}

void UBPUser::UnlinkAllOtherPlatform(EAccelBytePlatformType PlatformType, const FDHandler& OnSuccess, const FDCustomErrorHandler& OnError)
{
	
	FRegistry::User.UnlinkAllOtherPlatform(
		PlatformType,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FCustomErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorObject)
		{
			FString JsonString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
			TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ErrorObject);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			FJsonObjectWrapper ErrorObjectWrapper;
			FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorObjectWrapper);
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorObjectWrapper);
		})
	);
}

void UBPUser::UnlinkAllOtherPlatformId(const FString& PlatformId, const FDHandler& OnSuccess, const FDCustomErrorHandler& OnError)
{
	
	FRegistry::User.UnlinkAllOtherPlatformId(
		PlatformId,
		FVoidHandler::CreateLambda([OnSuccess]() { OnSuccess.ExecuteIfBound(); }),
		FCustomErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage, const FJsonObject& ErrorObject)
		{
			FString JsonString;
			TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
			TSharedPtr<FJsonObject> JsonObject = MakeShared<FJsonObject>(ErrorObject);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);
			FJsonObjectWrapper ErrorObjectWrapper;
			FAccelByteJsonConverter::JsonObjectStringToUStruct(JsonString, &ErrorObjectWrapper);
			OnError.ExecuteIfBound(ErrorCode, ErrorMessage, ErrorObjectWrapper);
		})
	);
}

void UBPUser::GetUserEligibleToPlay(const FDUserEligiblePlayHandler& OnSuccess, const FDErrorHandler& OnError)
{
	FRegistry::User.GetUserEligibleToPlay(
		THandler<bool>::CreateLambda([OnSuccess](bool result) {OnSuccess.ExecuteIfBound(result); }),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage) { OnError.ExecuteIfBound(ErrorCode, ErrorMessage); })
	);
}
