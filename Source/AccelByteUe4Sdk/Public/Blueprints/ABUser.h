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
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithUsername(FString const& Username, FString const& Password, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithOtherPlatform(EAccelBytePlatformType PlatformType, FString const& PlatformToken, FDHandler OnSuccess, FDErrorHandler OnError, bool bCreateHeadless = true);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithOtherPlatformId(FString const& PlatformId, FString const& PlatformToken, FDHandler OnSuccess, FDErrorHandler OnError, bool bCreateHeadless = true);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithDeviceId(FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithLauncher(FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithRefreshToken(FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void Logout(FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void ForgetAllCredentials();

//Registration
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void Register(FString const& Username, FString const& Password, FString const& DisplayName, FString const& Country, FString const& DateOfBirth, FDRegisterResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void Registerv2(FString const& EmailAddress, FString const& Username, FString const& Password, FString const& DisplayName, FString const& Country, FString const& DateOfBirth, FDRegisterResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void Registerv3(FRegisterRequestv3 const& RegisterRequest, FDRegisterResponse OnSuccess, FDErrorHandler OnError);

//Upgrade and Verify
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void Upgrade(FString const& Username, FString const& Password, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void Upgradev2(FString const& EmailAddress, FString const& Username, FString const& Password, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SendVerificationCode(FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void Verify(FString const& VerificationCode, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SendUpgradeVerificationCode(FString const& Username, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UpgradeAndVerify(FString const& Username, FString const& Password, FString const& VerificationCode, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

//Reset Password
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SendResetPasswordCode(FString const& Username, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void ResetPassword(FString const& VerificationCode, FString const& Username, FString const& NewPassword, FDHandler OnSuccess, FDErrorHandler OnError);

//Update
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SendUpdateEmailVerificationCode(FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UpdateEmail(FUpdateEmailRequest UpdateEmailRequest, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UpdateUser(FUserUpdateRequest UpdateRequest, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

//Platform Link
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetPlatformLinks(FDPagedPlatformLinksResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LinkOtherPlatform(EAccelBytePlatformType PlatformType, FString const& Ticket, FDHandler OnSuccess, FDAccountLinkConflictErrorJsonResponse OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType, FString const& PlatformUserId, FDHandler OnSuccess, FDAccountLinkConflictErrorJsonResponse OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UnlinkOtherPlatform(EAccelBytePlatformType PlatformType, FDHandler OnSuccess, FDErrorHandler OnError);

//Queries
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SearchUsers(FString const& Query, FDPagedPublicUsersInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SearchUsersByType(FString const& Query, EAccelByteSearchType By, FDPagedPublicUsersInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetUserByUserId(FString const& UserId, FDSimpleUserDataResponse OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetUserByOtherPlatformUserId(EAccelBytePlatformType PlatformType, FString const& OtherPlatformUserId, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType, TArray<FString> const& OtherPlatformUserId, FDBulkPlatformUserIdResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void BulkGetUserInfo(TArray<FString> const& UserIds, FDListBulkUserInfoResponse OnSuccess, FDErrorHandler OnError);

//Misc.
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetCountryFromIP(FDCountryInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetUserEligibleToPlay(FDBooleanResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetData(FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

private:
	FApiClientPtr ApiClientPtr;
};
