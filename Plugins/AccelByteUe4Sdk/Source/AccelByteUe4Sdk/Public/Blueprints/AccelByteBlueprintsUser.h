// Copyright (c) 2017-2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AccelByteServicesUser.h"
#include "AccelByteModelsUserProfile.h"
#include "AccelByteBlueprintsUser.generated.h"

UENUM(BlueprintType)
enum class EAccelBytePlatformType : uint8
{
	Steam,
	Google,
	Facebook,
	Android,
	iOS,
	Device,
	Twitch,
	Oculus,
	Twitter
};

UCLASS(Blueprintable, BlueprintType)
class UAccelByteBlueprintsUser : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	DECLARE_DYNAMIC_DELEGATE_TwoParams(FBlueprintError, int32, ErrorCode, FString, ErrorMessage);
	
	DECLARE_DYNAMIC_DELEGATE(FClientLoginSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
    static void ClientLogin(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FClientLoginSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
    static void ClientLoginEasy(FClientLoginSuccess OnSuccess, FBlueprintError OnError);
	
	DECLARE_DYNAMIC_DELEGATE(FLoginWithOtherPlatformAccountSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
    static void LoginWithOtherPlatformAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, EAccelBytePlatformType PlatformId, FString Token, FLoginWithOtherPlatformAccountSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
    static void LoginWithOtherPlatformAccountEasy(EAccelBytePlatformType PlatformId, FString Token, FLoginWithOtherPlatformAccountSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FLoginWithEmailAccountSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void LoginWithEmailAccount(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Email, FString Password, FLoginWithEmailAccountSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void LoginWithEmailAccountEasy(FString Email, FString Password, FLoginWithEmailAccountSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FLoginWithDeviceIdSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void LoginWithDeviceId(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FLoginWithDeviceIdSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void LoginWithDeviceIdEasy(FLoginWithDeviceIdSuccess OnSuccess, FBlueprintError OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void ResetCredentials();

	DECLARE_DYNAMIC_DELEGATE(FUpgradeHeadlessAccountSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void UpgradeHeadlessAccount(FString ServerBaseUrl, FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void UpgradeHeadlessAccounEasy(FString Email, FString Password, FUpgradeHeadlessAccountSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FCreateEmailAccountSuccess, const FAccelByteModelsUserCreateResponse&, Result);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void CreateEmailAccount(FString ServerBaseUrl, FString Namespace, FString Email, FString Password, FString DisplayName, FCreateEmailAccountSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void CreateEmailAccountEasy(FString Email, FString Password, FString DisplayName, FCreateEmailAccountSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FVerifyEmailAccountSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void VerifyEmailAccount(FString ServerBaseUrl, FString Namespace, FString UserId, FString VerificationCode, FVerifyEmailAccountSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void VerifyEmailAccountEasy(FString UserId, FString VerificationCode, FVerifyEmailAccountSuccess OnSuccess, FBlueprintError OnError);
	
	DECLARE_DYNAMIC_DELEGATE(FRequestPasswordResetSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void RequestPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString Email, FRequestPasswordResetSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void RequestPasswordResetEasy(FString Email, FRequestPasswordResetSuccess OnSuccess, FBlueprintError OnError);
	
	DECLARE_DYNAMIC_DELEGATE(FVerifyPasswordResetSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void VerifyPasswordReset(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString Namespace, FString VerificationCode, FString Email, FString NewPassword, FVerifyPasswordResetSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void VerifyPasswordResetEasy(FString VerificationCode, FString Email, FString NewPassword, FVerifyPasswordResetSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FLoginFromLauncherSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void LoginFromLauncher(FString ServerBaseUrl, FString ClientId, FString ClientSecret, FString RedirectUri, FLoginFromLauncherSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void LoginFromLauncherEasy(FString RedirectUri, FLoginFromLauncherSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE_OneParam(FGetProfileSuccess, const FAccelByteModelsUserProfileInfo&, UserProfileInfo);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void GetProfile(FString ServerBaseUrl, FGetProfileSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void GetProfileEasy(FGetProfileSuccess OnSuccess, FBlueprintError OnError);

	DECLARE_DYNAMIC_DELEGATE(FUpdateProfileSuccess);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void UpdateProfile(FString ServerBaseUrl, const FAccelByteModelsUserProfileInfoUpdate& UpdateProfile, FUpdateProfileSuccess OnSuccess, FBlueprintError OnError);
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Services | User")
	static void UpdateProfileEasy(const FAccelByteModelsUserProfileInfoUpdate& UpdateProfile, FUpdateProfileSuccess OnSuccess, FBlueprintError OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CredentialStore")
	static FString GetUserAccessToken();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CredentialStore")
	static FString GetUserRefreshToken();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CredentialStore")
	static FDateTime GetUserAccessTokenExpirationUtc();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CredentialStore")
	static FString GetUserId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CredentialStore")
	static FString GetUserDisplayName();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | CredentialStore")
	static FString GetUserNamespace();
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetSettingsServerBaseUrl();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetSettingsClientId();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetSettingsClientSecret();
	UFUNCTION(BlueprintCallable, Category = "AccelByte | Settings")
	static FString GetSettingsNamespace();
};
