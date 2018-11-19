// Copyright (c) 2018 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUserManagementModels.h"
#include "AccelByteError.h"


namespace AccelByte
{
namespace Api
{

/**
 * @brief User management API for creating user, verifying user, and resetting password.
 */
class ACCELBYTEUE4SDK_API UserManagement
{
public:

	DECLARE_DELEGATE_OneParam(FCreateUserAccountSuccess, const FAccelByteModelsUserCreateResponse&);
	/**
	 * @brief This function will register a new user with email-based account.
	 * 
	 * @param Username User email address or phone number.
	 * @param Password The Password.
	 * @param DisplayName The DisplayName.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserCreateResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateUserAccount(const FString& Username, const FString& Password, const FString& DisplayName, const FCreateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError);
	
	DECLARE_DELEGATE_OneParam(FUpdateUserAccountSuccess, const FAccelByteModelsUserResponse&);
	/**
	* @brief This function will update user's account.
	*
	* @param UpdateRequest User's request.
	* @param OnSuccess This will be called when the operation succeeded. The result is FUpdateUserAccountSuccess.
	* @param OnError This will be called when the operation failed.
	*/
	static void UpdateUserAccount(const FAccelByteModelsUserUpdateRequest& UpdateRequest, const FUpdateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FSendUserUpgradeVerificationCodeSuccess)
	/**
	 * @brief This function should be done before user upgrade their headless account. After this function successfully called, obtain the verification code from the submitted email. Then call UpgradeHeadlessAccountWithVerificationCode function afterwards.
	 * 
	 * @param LoginId Email or phone number that will be used to upgrade the headless account.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	*/
	static void SendUserUpgradeVerificationCode(const FString & LoginId, const FSendUserUpgradeVerificationCodeSuccess & OnSuccess, const FErrorHandler & OnError, const FString& LanguageTag = TEXT(""));

	DECLARE_DELEGATE_OneParam(FUpgradeHeadlessAccountWithVerificationCodeSuccess, const FAccelByteModelsUserResponse&)
	/**
	 * @brief This function should be called after you call SendUserUpgradeVerificationCode and obtain verification code.
	 *
	 * @param LoginId Email or phone number that will be used to upgrade the headless account.
	 * @param Password User's password.
	 * @param VerificationCode User's verification code that obtained from email.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UpgradeHeadlessAccountWithVerificationCode(const FAccelByteModelsUpgradeHeadlessAccountWithVerificationCodeRequest & RequestObject, const FUpgradeHeadlessAccountWithVerificationCodeSuccess& OnSuccess, const FErrorHandler& OnError);
	
	DECLARE_DELEGATE(FUpgradeHeadlessAccountSuccess);
	/**
	 * @brief This function will upgrade user's headless account. You may call SendUserAccountVerificationCode afterwards.
	 * Headless account is an account that doesn't have an email and password.
	 * If user logs in with a device/platform and they cannot login with email-and-password, their account is considered as a headless account.
	 * Therefore, the function requests user’s Username and Password for parameters.
	 * 
	 * @param Username The Username.
	 * @param Password The Password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UpgradeHeadlessAccount(const FString& Username, const FString& Password, const FUpgradeHeadlessAccountSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FSendUserAccountVerificationCodeSuccess);
	/**
	 * @brief Verify user's email. User should login with email and password first to get access token.
	 * 
	 * @param Username User email address or phone number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void SendUserAccountVerificationCode(const FAccelByteModelsSendVerificationCodeRequest& Request, const FSendUserAccountVerificationCodeSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FVerifyUserAccountSuccess);
	/**
	 * @brief This function will verify the registered email **after** user receives verification code sent with ::SendUserAccountVerificationCode() to their email.
	 * 
	 * @param VerificationCode The VerificationCode.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void VerifyUserAccount(const FString& VerificationCode, const FVerifyUserAccountSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FSendPasswordResetCodeSuccess);
    /**
	 * @brief Send a request to reset user's password. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * 
	 * @param Username User email address or phone number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void SendPasswordResetCode(const FString& Username, const FSendPasswordResetCodeSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FResetPasswordSuccess);
	/**
	 * @brief Reset user's password with sent verification code.
	 * 
	 * @ref SendPasswordResetCode()
	 * @param Username User email address or phone number.
	 * @param VerificationCode Code sent to the email address.
	 * @param NewPassword The new password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void ResetPassword(const FString& Username, const FString& VerificationCode, const FString& NewPassword, const FResetPasswordSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetLinkedUserAccountsSuccess, const TArray<FAccelByteModelsLinkedPlatform>&);
	/**
	 * @brief This function gets user's platform accounts linked to user’s account.
	 * 
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsLinkedPlatform>.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetLinkedUserAccounts(const FGetLinkedUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FLinkUserAccountsSuccess);
    /**
     * @brief This function links user's current account to their other account in other platform.
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch platform).
	 * The browser will redirect the URL to a site with a code in form of parameter URL. 
	 * 
	 * @param PlatformId The PlatformId.
	 * @param Ticket The Ticket.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LinkUserAccounts(const FString& PlatformId, const FString& Ticket, const FLinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FUnlinkUserAccountsSuccess);
	/**
	 * @brief This function links user's current account to their other account in other platform
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch).
	 * The browser will redirect the URL to a site with a code in form of parameter URL.
	 * 
	 * @param PlatformId The PlatformId.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UnlinkUserAccounts(const FString& PlatformId, const FUnlinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError);

private:
	UserManagement() = delete; // static class can't have instance
	UserManagement(UserManagement const&) = delete;
	UserManagement(UserManagement&&) = delete;

	static void CreateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateUserAccountSuccess OnSuccess, FErrorHandler OnError);
	static void UpdateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpdateUserAccountSuccess OnSuccess, FErrorHandler OnError);
	static void AddUsernameAndPasswordResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpgradeHeadlessAccountSuccess OnSuccess, FErrorHandler OnError);
	static void SendUserAccountVerificationCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FSendUserAccountVerificationCodeSuccess OnSuccess, FErrorHandler OnError);
	static void VerifyUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FVerifyUserAccountSuccess OnSuccess, FErrorHandler OnError);
	static void SendPasswordResetCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FSendPasswordResetCodeSuccess OnSuccess, FErrorHandler OnError);
	static void ResetPasswordResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FResetPasswordSuccess OnSuccess, FErrorHandler OnError);
	static void GetLinkedUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetLinkedUserAccountsSuccess OnSuccess, FErrorHandler OnError);
	static void LinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FLinkUserAccountsSuccess OnSuccess, FErrorHandler OnError);
	static void UnlinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUnlinkUserAccountsSuccess OnSuccess, FErrorHandler OnError);
	static void UpgradeHeadlessAccountWithVerificationCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpgradeHeadlessAccountWithVerificationCodeSuccess OnSuccess, FErrorHandler OnError);
};

} // Namespace Api
} // Namespace AccelByte
