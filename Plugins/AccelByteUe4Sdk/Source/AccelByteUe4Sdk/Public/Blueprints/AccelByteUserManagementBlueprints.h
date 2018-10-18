// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
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
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateUserAccountSuccess, const FAccelByteModelsUserCreateResponse&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void CreateUserAccountEasy(FString Username, FString Password, FString DisplayName, FCreateUserAccountSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FAddUsernameAndPasswordSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void AddUsernameAndPasswordEasy(FString Username, FString Password, FAddUsernameAndPasswordSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FSendUserAccountVerificationCodeSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void SendUserAccountVerificationCode(FString Username, FSendUserAccountVerificationCodeSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FVerifyUserAccountSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void VerifyUserAccountEasy(FString VerificationCode, FVerifyUserAccountSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FSendPasswordResetCodeSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void SendPasswordResetCodeEasy(FString Username, FSendPasswordResetCodeSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FResetPasswordSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void ResetPasswordEasy(FString Username, FString VerificationCode, FString NewPassword, FResetPasswordSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetLinkedUserAccountsSuccess, const TArray<FAccelByteModelsLinkedPlatform>&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void GetLinkedUserAccountsEasy(FGetLinkedUserAccountsSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FLinkUserAccountsSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void LinkUserAccountsEasy(FString PlatformId, FString Ticket, FLinkUserAccountsSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FUnlinkUserAccountsSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void UnlinkUserAccountsEasy(FString PlatformId, FUnlinkUserAccountsSuccess OnSuccess, FBlueprintError OnError);
};
