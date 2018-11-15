// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserManagementBlueprints.h"
#include "AccelByteUserManagementApi.h"

using AccelByte::Api::UserManagement;
using AccelByte::FErrorHandler;

void UAccelByteBlueprintsUserManagement::CreateUserAccount(const FString& Username, const FString& Password, const FString& DisplayName, FCreateUserAccountSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::CreateUserAccount(Username, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserCreateResponse& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::UpdateUserAccount(const FAccelByteModelsUserUpdateRequest& UpdateRequest, const FUpdateUserAccountSuccess& OnSuccess, const FBlueprintErrorHandler & OnError)
{
	UserManagement::UpdateUserAccount(UpdateRequest, UserManagement::FUpdateUserAccountSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserResponse& Result)
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
	UserManagement::UpgradeHeadlessAccount(Username, Password, UserManagement::FUpgradeHeadlessAccountSuccess::CreateLambda([OnSuccess]()
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
	
	UserManagement::SendUserAccountVerificationCode(Request, UserManagement::FSendUserAccountVerificationCodeSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::VerifyUserAccount(const FString& VerificationCode, FVerifyUserAccountSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	
	UserManagement::VerifyUserAccount(VerificationCode, UserManagement::FVerifyUserAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::SendPasswordResetCode(const FString& Username, FSendPasswordResetCodeSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	
	UserManagement::SendPasswordResetCode(Username, UserManagement::FSendPasswordResetCodeSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::ResetPassword(const FString& Username, const FString& VerificationCode, const FString& NewPassword, FResetPasswordSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::ResetPassword(Username, VerificationCode, NewPassword, UserManagement::FResetPasswordSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::GetLinkedUserAccounts(FGetLinkedUserAccountsSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::GetLinkedUserAccounts(UserManagement::FGetLinkedUserAccountsSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsLinkedPlatform>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::LinkUserAccounts(const FString& PlatformId, const FString& Ticket, FLinkUserAccountsSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	UserManagement::LinkUserAccounts(PlatformId, Ticket, UserManagement::FLinkUserAccountsSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
	
void UAccelByteBlueprintsUserManagement::UnlinkUserAccounts(const FString& PlatformId, FUnlinkUserAccountsSuccess OnSuccess, FBlueprintErrorHandler OnError)
{
	
	UserManagement::UnlinkUserAccounts(PlatformId, UserManagement::FUnlinkUserAccountsSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorHandler::CreateLambda([OnError](int32 ErrorCode, const FString& ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
