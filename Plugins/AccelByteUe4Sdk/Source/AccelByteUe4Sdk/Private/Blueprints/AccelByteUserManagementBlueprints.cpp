// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#include "AccelByteUserManagementBlueprints.h"
#include "AccelByteUserManagementApi.h"

using AccelByte::Api::UserManagement;
using AccelByte::FErrorDelegate;

void UAccelByteBlueprintsUserManagement::CreateUserAccountEasy(FString Username, FString Password, FString DisplayName, FCreateUserAccountSuccess OnSuccess, FBlueprintError OnError)
{
	UserManagement::CreateUserAccountEasy(Username, Password, DisplayName, UserManagement::FCreateUserAccountSuccess::CreateLambda([OnSuccess](const FAccelByteModelsUserCreateResponse& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}),
		FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}


void UAccelByteBlueprintsUserManagement::AddUsernameAndPasswordEasy(FString Username, FString Password, FAddUsernameAndPasswordSuccess OnSuccess, FBlueprintError OnError)
{
	UserManagement::AddUsernameAndPasswordEasy(Username, Password, UserManagement::FAddUsernameAndPasswordSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::SendUserAccountVerificationCode(FString Username, FSendUserAccountVerificationCodeSuccess OnSuccess, FBlueprintError OnError)
{
	UserManagement::SendUserAccountVerificationCodeEasy(Username, UserManagement::FSendUserAccountVerificationCodeSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::VerifyUserAccountEasy(FString VerificationCode, FVerifyUserAccountSuccess OnSuccess, FBlueprintError OnError)
{
	
	UserManagement::VerifyUserAccountEasy(VerificationCode, UserManagement::FVerifyUserAccountSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}),
		FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::SendPasswordResetCodeEasy(FString Username, FSendPasswordResetCodeSuccess OnSuccess, FBlueprintError OnError)
{
	
	UserManagement::SendPasswordResetCodeEasy(Username, UserManagement::FSendPasswordResetCodeSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::ResetPasswordEasy(FString Username, FString VerificationCode, FString NewPassword, FResetPasswordSuccess OnSuccess, FBlueprintError OnError)
{
	UserManagement::ResetPasswordEasy(Username, VerificationCode, NewPassword, UserManagement::FResetPasswordSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::GetLinkedUserAccountsEasy(FGetLinkedUserAccountsSuccess OnSuccess, FBlueprintError OnError)
{
	UserManagement::GetLinkedUserAccountsEasy(UserManagement::FGetLinkedUserAccountsSuccess::CreateLambda([OnSuccess](const TArray<FAccelByteModelsLinkedPlatform>& Result)
	{
		OnSuccess.ExecuteIfBound(Result);
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}

void UAccelByteBlueprintsUserManagement::LinkUserAccountsEasy(FString PlatformId, FString Ticket, FLinkUserAccountsSuccess OnSuccess, FBlueprintError OnError)
{
	UserManagement::LinkUserAccountsEasy(PlatformId, Ticket, UserManagement::FLinkUserAccountsSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
	
void UAccelByteBlueprintsUserManagement::UnlinkUserAccountsEasy(FString PlatformId, FUnlinkUserAccountsSuccess OnSuccess, FBlueprintError OnError)
{
	
	UserManagement::UnlinkUserAccountsEasy(PlatformId, UserManagement::FUnlinkUserAccountsSuccess::CreateLambda([OnSuccess]()
	{
		OnSuccess.ExecuteIfBound();
	}), FErrorDelegate::CreateLambda([OnError](int32 ErrorCode, FString ErrorMessage)
	{
		OnError.ExecuteIfBound(ErrorCode, ErrorMessage);
	}));
}
