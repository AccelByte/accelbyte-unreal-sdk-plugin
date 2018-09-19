// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "JusticeUser.h"
#include "JusticeUserBlueprints.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRegisterWithEmailOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API URegisterWithEmail : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FRegisterWithEmailOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FRegisterWithEmailOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static URegisterWithEmail* RegisterWithEmail(FString Email, FString Password, FString DisplayName);
	virtual void Activate() override;
private:
	FString Email;
	FString Password;
	FString DisplayName;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginWithEmailOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API ULoginWithEmail : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FLoginWithEmailOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FLoginWithEmailOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static ULoginWithEmail* LoginWithEmail(FString Email, FString Password);
	virtual void Activate() override;
private:
	FString Email;
	FString Password;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVerifyEmailOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UVerifyEmail : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FVerifyEmailOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FVerifyEmailOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static UVerifyEmail* VerifyEmail(FString VerificationCode);
	virtual void Activate() override;
private:
	FString VerificationCode;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FForgotPasswordOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UForgotPassword : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FForgotPasswordOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FForgotPasswordOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static UForgotPassword* ForgotPassword(FString Email);
	virtual void Activate() override;
private:
	FString Email;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FResetPasswordOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UResetPassword : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FResetPasswordOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FResetPasswordOutputPin OnFailed;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static UResetPassword* ResetPassword(FString VerificationCode, FString Email, FString NewPassword);
	virtual void Activate() override;
private:
	FString VerificationCode;
	FString Email;
	FString NewPassword;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginFromLauncherOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API ULoginFromLauncher : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FLoginFromLauncherOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FLoginFromLauncherOutputPin OnFail;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static ULoginFromLauncher* LoginFromLauncher();
	virtual void Activate() override;
};

UENUM(BlueprintType)
enum class UPlatformType : uint8
{
	Steam = 0		UMETA(DisplayName = "Steam"),
	Google = 1		UMETA(DisplayName = "Google"),
	Facebook = 2	UMETA(DisplayName = "Facebook"),
	Android = 3		UMETA(DisplayName = "Android"),
	iOS = 4			UMETA(DisplayName = "iOS"),
	Device = 5		UMETA(DisplayName = "Device"),
	Twitch = 6		UMETA(DisplayName = "Twitch"),
	Oculus = 7		UMETA(DisplayName = "Oculus"),
	Twitter = 8		UMETA(DisplayName = "Twitter")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginWithPlatformOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API ULoginWithPlatform : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FLoginWithPlatformOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FLoginWithPlatformOutputPin OnFail;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static ULoginWithPlatform* LoginWithPlatform(UPlatformType PlatformType, FString PlatformToken);
	virtual void Activate() override;
private:
	EPlatformType PlatformType;
	FString PlatformToken;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLoginWithDeviceIdOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API ULoginWithDeviceId : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FLoginWithDeviceIdOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FLoginWithDeviceIdOutputPin OnFail;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static ULoginWithDeviceId* LoginWithDeviceId();
	virtual void Activate() override;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpgradeHeadlessAccountOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UUpgradeHeadlessAccount : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FUpgradeHeadlessAccountOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FUpgradeHeadlessAccountOutputPin OnFail;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static UUpgradeHeadlessAccount* UpgradeHeadlessAccount(FString Email, FString Password);
	virtual void Activate() override;
private:
	FString Email;
	FString Password;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGetProfileOutputPin, FString, ErrorMessage, UUserProfileJustice*, UserProfile);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UGetProfile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FGetProfileOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FGetProfileOutputPin OnFail;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static UGetProfile* GetProfile();
	virtual void Activate() override;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateProfileOutputPin, FString, ErrorMessage);
UCLASS(Blueprintable, BlueprintType)
class JUSTICESDK_API UUpdateProfile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
		FUpdateProfileOutputPin OnSuccess;
	UPROPERTY(BlueprintAssignable)
		FUpdateProfileOutputPin OnFail;
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true", Category = "JusticeSDK | User"))
		static UUpdateProfile* UpdateProfile(FString DisplayName, FString FirstName, FString LastName, FString AvatarSmallUrl, FString AvatarUrl, FString AvatarLargeUrl, FString Email, FString Language, FString Timezone, FString DateOfBirth, FString Country, TMap<FString, FString> CustomAttributes);
	virtual void Activate() override;
private:
	FString                 DisplayName;
	FString                 FirstName;
	FString                 LastName;
	FString                 AvatarSmallUrl;
	FString                 AvatarUrl;
	FString                 AvatarLargeUrl;
	FString                 Email;
	FString                 Language;
	FString                 Timezone;
	FString                 DateOfBirth;
	FString                 Country;
	TMap<FString, FString>  CustomAttributes;
};
