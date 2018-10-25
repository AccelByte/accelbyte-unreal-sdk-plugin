// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteUserManagementModels.h"
#include "AccelByteUserManagementBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsUserManagement : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintErrorHandler, int32, ErrorCode, FString, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateUserAccountSuccess, const FAccelByteModelsUserCreateResponse&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void CreateUserAccountEasy(const FString& Username, const FString& Password, const FString& DisplayName, FCreateUserAccountSuccess OnSuccess, FBlueprintErrorHandler OnError);
	
	DECLARE_DYNAMIC_DELEGATE(FUpdateUserAccountSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void UpdateUserAccountEasy(const FAccelByteModelsUserUpdateRequest& UpdateRequest, const FUpdateUserAccountSuccess& OnSuccess, const FBlueprintErrorHandler& OnError);

	DECLARE_DYNAMIC_DELEGATE(FAddUsernameAndPasswordSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void AddUsernameAndPasswordEasy(const FString& Username, const FString& Password, FAddUsernameAndPasswordSuccess OnSuccess, FBlueprintErrorHandler OnError);

	DECLARE_DYNAMIC_DELEGATE(FSendUserAccountVerificationCodeSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void SendUserAccountVerificationCode(const FString& Username, FSendUserAccountVerificationCodeSuccess OnSuccess, FBlueprintErrorHandler OnError);

	DECLARE_DYNAMIC_DELEGATE(FVerifyUserAccountSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void VerifyUserAccountEasy(const FString& VerificationCode, FVerifyUserAccountSuccess OnSuccess, FBlueprintErrorHandler OnError);

	DECLARE_DYNAMIC_DELEGATE(FSendPasswordResetCodeSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void SendPasswordResetCodeEasy(const FString& Username, FSendPasswordResetCodeSuccess OnSuccess, FBlueprintErrorHandler OnError);

	DECLARE_DYNAMIC_DELEGATE(FResetPasswordSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void ResetPasswordEasy(const FString& Username, const FString& VerificationCode, const FString& NewPassword, FResetPasswordSuccess OnSuccess, FBlueprintErrorHandler OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetLinkedUserAccountsSuccess, const TArray<FAccelByteModelsLinkedPlatform>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void GetLinkedUserAccountsEasy(FGetLinkedUserAccountsSuccess OnSuccess, FBlueprintErrorHandler OnError);

	DECLARE_DYNAMIC_DELEGATE(FLinkUserAccountsSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void LinkUserAccountsEasy(const FString& PlatformId, const FString& Ticket, FLinkUserAccountsSuccess OnSuccess, FBlueprintErrorHandler OnError);

	DECLARE_DYNAMIC_DELEGATE(FUnlinkUserAccountsSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void UnlinkUserAccountsEasy(const FString& PlatformId, FUnlinkUserAccountsSuccess OnSuccess, FBlueprintErrorHandler OnError);
};
