#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"
#include "AccelByteUe4Sdk/Public/Models/AccelByteUserModels.h"
#include "Core/AccelByteMultiRegistry.h"
#include "ABUser.generated.h"

using namespace AccelByte;
using namespace AccelByte::Api;


DECLARE_DYNAMIC_DELEGATE_OneParam(FDRegisterResponse, FRegisterResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccountUserDataResponse, FAccountUserData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPagedPlatformLinksResponse, FPagedPlatformLinks, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPagedPublicUsersInfoResponse, FPagedPublicUsersInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDSimpleUserDataResponse, FSimpleUserData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDBulkPlatformUserIdResponse, FBulkPlatformUserIdResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDListBulkUserInfoResponse, FListBulkUserInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDCountryInfoResponse, FCountryInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDBooleanResponse, bool, Response);

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDAccountLinkConflictErrorJsonResponse, int32, ErrorCode, const FString&, ErrorMessage, FAccountLinkConflictErrorJson, Response);

UCLASS(Blueprintable, BlueprintType)
class UABUser final : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(FApiClientPtr NewApiClientPtr);

//Login & logout
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void LoginWithUsername(FString const& Username, FString const& Password, FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void LoginWithOtherPlatform(EAccelBytePlatformType PlatformType, FString const& PlatformToken, FDHandler OnSuccess, FDErrorHandler OnError, bool bCreateHeadless = true) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void LoginWithDeviceId(FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void LoginWithLauncher(FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void LoginWithRefreshToken(FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void Logout(FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void ForgetAllCredentials() const;

//Registration
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void Register(FString const& Username, FString const& Password, FString const& DisplayName, FString const& Country, FString const& DateOfBirth, FDRegisterResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void Registerv2(FString const& EmailAddress, FString const& Username, FString const& Password, FString const& DisplayName, FString const& Country, FString const& DateOfBirth, FDRegisterResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void Registerv3(FRegisterRequestv3 const& RegisterRequest, FDRegisterResponse OnSuccess, FDErrorHandler OnError) const;

//Upgrade and Verify
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void Upgrade(FString const& Username, FString const& Password, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void Upgradev2(FString const& EmailAddress, FString const& Username, FString const& Password, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void SendVerificationCode(FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void Verify(FString const& VerificationCode, FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void SendUpgradeVerificationCode(FString const& Username, FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void UpgradeAndVerify(FString const& Username, FString const& Password, FString const& VerificationCode, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError) const;

//Reset Password
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void SendResetPasswordCode(FString const& Username, FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void ResetPassword(FString const& VerificationCode, FString const& Username, FString const& NewPassword, FDHandler OnSuccess, FDErrorHandler OnError) const;

//Update
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void SendUpdateEmailVerificationCode(FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void UpdateEmail(FUpdateEmailRequest UpdateEmailRequest, FDHandler OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void UpdateUser(FUserUpdateRequest UpdateRequest, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError) const;

//Platform Link
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void GetPlatformLinks(FDPagedPlatformLinksResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void LinkOtherPlatform(EAccelBytePlatformType PlatformType, FString const& Ticket, FDHandler OnSuccess, FDAccountLinkConflictErrorJsonResponse OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType, FString const& PlatformUserId, FDHandler OnSuccess, FDAccountLinkConflictErrorJsonResponse OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void UnlinkOtherPlatform(EAccelBytePlatformType PlatformType, FDHandler OnSuccess, FDErrorHandler OnError) const;

//Queries
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void SearchUsers(FString const& Query, FDPagedPublicUsersInfoResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void SearchUsersByType(FString const& Query, EAccelByteSearchType By, FDPagedPublicUsersInfoResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void GetUserByUserId(FString const& UserId, FDSimpleUserDataResponse OnSuccess, FDErrorHandler OnError) const;
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void GetUserByOtherPlatformUserId(EAccelBytePlatformType PlatformType, FString const& OtherPlatformUserId, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType, TArray<FString> const& OtherPlatformUserId, FDBulkPlatformUserIdResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void BulkGetUserInfo(TArray<FString> const& UserIds, FDListBulkUserInfoResponse OnSuccess, FDErrorHandler OnError) const;

//Misc.
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void GetCountryFromIP(FDCountryInfoResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void GetUserEligibleToPlay(FDBooleanResponse OnSuccess, FDErrorHandler OnError) const;

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User")
	void GetData(FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError) const;

private:
	FApiClientPtr ApiClientPtr;
};