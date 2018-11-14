// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserManagementBlueprints.h"
#include "AccelByteUserManagementApi.h"

using AccelByte::Api::UserManagement;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsUserManagement::CreateUserAccountEasy(const FString& Username, const FString& Password, const FString& DisplayName, FCreateUserAccountSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::CreateUserAccountEasy(Username, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserCreateResponse& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::UpdateUserAccountEasy(const FAccelByteModelsUserUpdateRequest& UpdateRequest, const FUpdateUserAccountSuccess& OnSuccess, const FBlueprintErrorHandler & OnError)
{
	UserManagement::UpdateUserAccountEasy(UpdateRequest, UserManagement::FUpdateUserAccountSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserResponse& Result)
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}


void UAccelByteBlueprintsUserManagement::UpgradeHeadlessAccount(const FString& Username, const FString& Password, FUpgradeHeadlessAccountSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::UpgradeHeadlessAccountEasy(Username, Password, UserManagement::FUpgradeHeadlessAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::SendUserUpgradeAccountVerificationCode(const FString & Email, const FString& LanguageTag, FSendUserUpgradeAccountVerificationCodeSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::SendUserUpgradeVerificationCode(Email, UserManagement::FSendUserUpgradeVerificationCodeSuccess::CreateLambda([OnSuccess]() 
	{
		OnSuccess.ExecuteIfBound();
	}), 
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}), LanguageTag);
}

void UAccelByteBlueprintsUserManagement::UpgradeHeadlessAccountWithVerificationCode(const FAccelByteModelsUpgradeHeadlessAccountWithVerificationCodeRequest & RequestObject, FUpgradeHeadlessAccountWithVerificationCodeSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::UpgradeHeadlessAccountWithVerificationCode(RequestObject, UserManagement::FUpgradeHeadlessAccountWithVerificationCodeSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserResponse& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::SendUserAccountVerificationCode(const FString& LoginID, FSendUserAccountVerificationCodeSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	FAccelByteModelsSendVerificationCodeRequest Request
	{
		EAccelByteVerificationCodeContext::UserAccountRegistration,
		FString(),
		LoginID
	};
	
	UserManagement::SendUserAccountVerificationCodeEasy(Request, UserManagement::FSendUserAccountVerificationCodeSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::VerifyUserAccountEasy(const FString& VerificationCode, FVerifyUserAccountSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	
	UserManagement::VerifyUserAccountEasy(VerificationCode, UserManagement::FVerifyUserAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::SendPasswordResetCodeEasy(const FString& Username, FSendPasswordResetCodeSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	
	UserManagement::SendPasswordResetCodeEasy(Username, UserManagement::FSendPasswordResetCodeSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::ResetPasswordEasy(const FString& Username, const FString& VerificationCode, const FString& NewPassword, FResetPasswordSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::ResetPasswordEasy(Username, VerificationCode, NewPassword, UserManagement::FResetPasswordSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::GetLinkedUserAccountsEasy(FGetLinkedUserAccountsSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::GetLinkedUserAccountsEasy(UserManagement::FGetLinkedUserAccountsSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsLinkedPlatform>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::LinkUserAccountsEasy(const FString& PlatformId, const FString& Ticket, FLinkUserAccountsSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::LinkUserAccountsEasy(PlatformId, Ticket, UserManagement::FLinkUserAccountsSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
	
void UAccelByteBlueprintsUserManagement::UnlinkUserAccountsEasy(const FString& PlatformId, FUnlinkUserAccountsSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	
	UserManagement::UnlinkUserAccountsEasy(PlatformId, UserManagement::FUnlinkUserAccountsSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
