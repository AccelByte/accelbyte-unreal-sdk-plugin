// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteError.h"
#include "AccelByteUserModels.h"
#include "AccelByteUserBlueprints.generated.h"

UCLASS(Blueprintable, BlueprintType)
class UBPUser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE(FDHandler);
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FDErrorHandler, int32, ErrorCode, const FString&, ErrorMessage);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FDUserDataHandler, const FUserData&, Result);
	DECLARE_DYNAMIC_DELEGATE_OneParam(FDPlatformLinksHandler, const TArray<FPlatformLink>&, PlatformLinks);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void Register(const FString& Username, const FString& Password, const FString& DisplayName, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void Update(const FUserUpdateRequest& UpdateRequest, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void Upgrade(const FString& Username, const FString& Password, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void SendVerificationCode(const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void Verify(const FString& VerificationCode, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void SendResetPasswordCode(const FString& Username, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void ResetPassword(const FString& VerificationCode, const FString& Username, const FString& NewPassword, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void SendUpgradeVerificationCode(const FString& Email, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void GetPlatformLinks(const FDPlatformLinksHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void LinkOtherPlatform(const FString& PlatformId, const FString& Ticket, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | UserManagement | Api")
	static void UnlinkOtherPlatform(const FString& PlatformId, const FDHandler& OnSuccess, const FDErrorHandler& OnError);
};
