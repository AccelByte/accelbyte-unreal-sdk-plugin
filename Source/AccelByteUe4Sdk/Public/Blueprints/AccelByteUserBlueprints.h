// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "AccelByteUe4Sdk/Public/Models/AccelByteUserModels.h"
#include "AccelByteUserBlueprints.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FDUserDataHandler, const FAccountUserData&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDUserRegisterHandler, const FRegisterResponse&, Result);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDPlatformLinksHandler, const FPagedPlatformLinks&, PlatformLinks);
DECLARE_DYNAMIC_DELEGATE_OneParam(FDUserEligiblePlayHandler, const bool, Result);

UCLASS(Blueprintable, BlueprintType)
class UBPUser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api ")
	static void LoginWithUsername(const FString& Username, const FString& Password, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api ")
	static void LoginWithOtherPlatform(EAccelBytePlatformType PlatformType, const FString& Token, const FDHandler& OnSuccess, const FDErrorHandler& OnError, bool bCreateHeadless = true);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api ")
	static void LoginWithDeviceId(const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api ")
	static void Logout(const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api ")
	static void ForgetAllCredentials();

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void Register(const FString& Username, const FString& Password, const FString& DisplayName, const FString& Country, const FString& DateOfBirth, const FDUserRegisterHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void Upgrade(const FString& Username, const FString& Password, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void SendVerificationCode(const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void Verify(const FString& VerificationCode, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void SendResetPasswordCode(const FString& Username, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void ResetPassword(const FString& VerificationCode, const FString& Username, const FString& NewPassword, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void SendUpgradeVerificationCode(const FString& Email, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const FDUserDataHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void GetPlatformLinks(const FDPlatformLinksHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void LinkOtherPlatform(EAccelBytePlatformType PlatformType, const FString& Ticket, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void UnlinkOtherPlatform(EAccelBytePlatformType PlatformType, const FDHandler& OnSuccess, const FDErrorHandler& OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	static void GetUserEligibleToPlay(const FDUserEligiblePlayHandler& OnSuccess, const FDErrorHandler& OnError);
};
