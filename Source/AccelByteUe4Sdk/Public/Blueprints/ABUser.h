#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Models/AccelByteUserModels.h"
#include "Models/AccelByteOauth2Models.h"
#include "Core/AccelByteApiClient.h"
#include "ABUser.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FDRegisterResponse, FRegisterResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccountUserDataResponse, FAccountUserData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPagedPlatformLinksResponse, FPagedPlatformLinks, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDPagedPublicUsersInfoResponse, FPagedPublicUsersInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDSimpleUserDataResponse, FSimpleUserData, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDBulkPlatformUserIdResponse, FBulkPlatformUserIdResponse, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDListBulkUserInfoResponse, FListBulkUserInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDCountryInfoResponse, FCountryInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDBooleanResponse, bool, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDAccountUserPlatformInfosResponse, FAccountUserPlatformInfosResponse, Response);

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FDAccountLinkConflictErrorJsonResponse, int32, ErrorCode, const FString&, ErrorMessage, FAccountLinkConflictErrorJson, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDLoginQueueInfoResponse, FAccelByteModelsLoginQueueTicketInfo, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FDUserInputValidationResponse, FUserInputValidationResponse, Response);

UCLASS(Blueprintable, BlueprintType)
class UABUser final : public UObject
{
	GENERATED_BODY()
public:
	void SetApiClient(AccelByte::FApiClientPtr const& NewApiClientPtr);

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
	void TryRelogin(FString const& PlatformUserID, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginByExchangeCodeForToken(FString const& Code, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithUsernameV4(FString const& Username, FString const& Password, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithOtherPlatformV4(EAccelBytePlatformType PlatformType, FString const& PlatformToken, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError, bool bCreateHeadless = true);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithOtherPlatformIdV4(FString const& PlatformId, FString const& PlatformToken, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError, bool bCreateHeadless = true);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithDeviceIdV4(FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithLauncherV4(FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginWithRefreshTokenV4(FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void TryReloginV4(FString const& PlatformUserID, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void LoginByExchangeCodeForTokenV4(FString const& Code, FDLoginQueueInfoResponse OnSuccess, FDOAuthErrorHandler OnError);

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
	void SendUpgradeVerificationCode(FString const& EmailAddress, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UpgradeAndVerify(FString const& Username, FString const& Password, FString const& VerificationCode, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

//Reset Password
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SendResetPasswordCode(FString const& EmailAddress, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void ResetPassword(FString const& VerificationCode, FString const& EmailAddress, FString const& NewPassword, FDHandler OnSuccess, FDErrorHandler OnError);

//Update
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SendUpdateEmailVerificationCode(FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UpdateEmail(FUpdateEmailRequest const& UpdateEmailRequest, FDHandler OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UpdateUser(FUserUpdateRequest const& UpdateRequest, FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

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

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UnlinkOtherPlatformWithCustomErrorHandler(EAccelBytePlatformType PlatformType, FDHandler OnSuccess, FDCustomErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UnlinkOtherPlatformId(const FString& PlatformId, FDHandler OnSuccess, FDCustomErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UnlinkAllOtherPlatform(EAccelBytePlatformType PlatformType, FDHandler OnSuccess, FDCustomErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void UnlinkAllOtherPlatformId(const FString& PlatformId, FDHandler OnSuccess, FDCustomErrorHandler OnError);

// Queries
// Blueprint Functions below are indeed related to queries since they involve querying for specific user data
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SearchUsers(FString const& Query, FDPagedPublicUsersInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SearchUsersByType(FString const& Query, EAccelByteSearchType By, FDPagedPublicUsersInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SearchUsersByOtherPlatformType(FString const& Query, EAccelBytePlatformType PlatformType, EAccelByteSearchPlatformType PlatformBy, FDPagedPublicUsersInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void SearchUsersByOtherPlatformId(FString const& Query, const FString& PlatformId, EAccelByteSearchPlatformType PlatformBy, FDPagedPublicUsersInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetUserByUserId(FString const& UserId, FDSimpleUserDataResponse OnSuccess, FDErrorHandler OnError);
	
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType, TArray<FString> const& OtherPlatformUserId, FDBulkPlatformUserIdResponse OnSuccess, FDErrorHandler OnError);

//Misc.
public:
	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetCountryFromIP(FDCountryInfoResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetUserEligibleToPlay(FDBooleanResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetData(FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetDataWithLinkedPlatform(FDAccountUserDataResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void GetUserOtherPlatformBasicPublicInfo(const FPlatformAccountInfoRequest& Request,FDAccountUserPlatformInfosResponse OnSuccess, FDErrorHandler OnError);

	UFUNCTION(BlueprintCallable, Category = "AccelByte | User | Api")
	void ValidateUserInput(FUserInputValidationRequest const& UserInputValidationRequest, FDUserInputValidationResponse OnSuccess, FDErrorHandler OnError);

private:
	AccelByte::FApiClientPtr ApiClientPtr;
};
