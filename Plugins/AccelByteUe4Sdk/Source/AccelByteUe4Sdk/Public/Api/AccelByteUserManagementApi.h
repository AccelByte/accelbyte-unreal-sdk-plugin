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
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The GameId.
	 * @param Username User email address or phone number.
	 * @param Password The Password.
	 * @param DisplayName The DisplayName.
	 * @param OnSuccess This will be called when the operation succeeded. The result is FAccelByteModelsUserCreateResponse.
	 * @param OnError This will be called when the operation failed.
	 */
	static void CreateUserAccount(const FString& AccessToken, const FString& Namespace, const FString& Username, const FString& Password, const FString& DisplayName, const FCreateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError);
	static void CreateUserAccountEasy(const FString& Username, const FString& Password, const FString& DisplayName, const FCreateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError);
	
	DECLARE_DELEGATE_OneParam(FUpdateUserAccountSuccess, const FAccelByteModelsUserUpdateResponse&);
	static void UpdateUserAccount(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FAccelByteModelsUserUpdateRequest& UpdateRequest, const FUpdateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError);
	static void UpdateUserAccountEasy(const FAccelByteModelsUserUpdateRequest& UpdateRequest, const FUpdateUserAccountSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FAddUsernameAndPasswordSuccess);
	/**
	 * @brief This function will upgrade user's headless account. You may call SendUserAccountVerificationCode afterwards.
	 * Headless account is an account that doesn't have an email and password.
	 * If user logs in with a device/platform and they cannot login with email-and-password, their account is considered as a headless account.
	 * Therefore, the function requests user’s Username and Password for parameters.
	 * 
	 * @param AccessToken User's access token.
	 * @param Namespace The GameId.
	 * @param UserId The UserId.
	 * @param Username The Username.
	 * @param Password The Password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void AddUsernameAndPassword(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& Username, const FString& Password, const FAddUsernameAndPasswordSuccess& OnSuccess, const FErrorHandler& OnError);
	static void AddUsernameAndPasswordEasy(const FString& Username, const FString& Password, const FAddUsernameAndPasswordSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FSendUserAccountVerificationCodeSuccess);
	/**
	 * @brief Verify user's email. User should login with email and password first to get access token.
	 * 
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The GameId.
	 * @param UserId Obtained from token response obtained after user login. 
	 * @param Username User email address or phone number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void SendUserAccountVerificationCode(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& Username, const FSendUserAccountVerificationCodeSuccess& OnSuccess, const FErrorHandler& OnError);
	static void SendUserAccountVerificationCodeEasy(const FString& Username, const FSendUserAccountVerificationCodeSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FVerifyUserAccountSuccess);
	/**
	 * @brief This function will verify the registered email **after** user receives verification code sent with ::SendUserAccountVerificationCode() to their email.
	 * 
	 * @param AccessToken Access token from client credentials grant.
	 * @param Namespace The GameId.
	 * @param UserId The user ID.
	 * @param VerificationCode The VerificationCode.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void VerifyUserAccount(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& VerificationCode, const FVerifyUserAccountSuccess& OnSuccess, const FErrorHandler& OnError);
	static void VerifyUserAccountEasy(const FString& VerificationCode, const FVerifyUserAccountSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FSendPasswordResetCodeSuccess);
    /**
	 * @brief Send a request to reset user's password. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * 
	 * @param ClientId Client credentials. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * @param ClientSecret Client credentials. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
	 * @param ClientNamespace The ClientNamespace.
	 * @param Username User email address or phone number.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void SendPasswordResetCode(const FString& ClientId, const FString& ClientSecret, const FString& ClientNamespace, const FString& Username, const FSendPasswordResetCodeSuccess& OnSuccess, const FErrorHandler& OnError);
	static void SendPasswordResetCodeEasy(const FString& Username, const FSendPasswordResetCodeSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FResetPasswordSuccess);
	/**
	 * @brief Reset user's password with sent verification code.
	 * 
	 * @ref SendPasswordResetCode()
	 * @param Namespace The GameId.
	 * @param Username User email address or phone number.
	 * @param VerificationCode Code sent to the email address.
	 * @param NewPassword The new password.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void ResetPassword(const FString& ClientId, const FString& ClientSecret, const FString& Namespace, const FString& Username, const FString& VerificationCode, const FString& NewPassword, const FResetPasswordSuccess& OnSuccess, const FErrorHandler& OnError);
	static void ResetPasswordEasy(const FString& Username, const FString& VerificationCode, const FString& NewPassword, const FResetPasswordSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE_OneParam(FGetLinkedUserAccountsSuccess, const TArray<FAccelByteModelsLinkedPlatform>&);
	/**
	 * @brief This function gets user's platform accounts linked to user’s account.
	 * 
	 * @param AccessToken Client token.
	 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FAccelByteModelsLinkedPlatform>.
	 * @param OnError This will be called when the operation failed.
	 */
	static void GetLinkedUserAccounts(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FGetLinkedUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError);
	static void GetLinkedUserAccountsEasy(const FGetLinkedUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FLinkUserAccountsSuccess);
    /**
     * @brief This function links user's current account to their other account in other platform.
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch platform).
	 * The browser will redirect the URL to a site with a code in form of parameter URL. 
	 * 
	 * @param AccessToken Client token. Required.
	 * @param Namespace The GameId.
	 * @param PlatformId The PlatformId.
	 * @param Ticket The Ticket.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void LinkUserAccounts(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& PlatformId, const FString& Ticket, const FLinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError);
	static void LinkUserAccountsEasy(const FString& PlatformId, const FString& Ticket, const FLinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError);

	DECLARE_DELEGATE(FUnlinkUserAccountsSuccess);
	/**
	 * @brief This function links user's current account to their other account in other platform
	 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch).
	 * The browser will redirect the URL to a site with a code in form of parameter URL.
	 * 
	 * @param AccessToken Client token. Required.
	 * @param Namespace The GameId.
	 * @param PlatformId The PlatformId.
	 * @param OnSuccess This will be called when the operation succeeded.
	 * @param OnError This will be called when the operation failed.
	 */
	static void UnlinkUserAccounts(const FString& AccessToken, const FString& Namespace, const FString& UserId, const FString& PlatformId, const FUnlinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError);
	static void UnlinkUserAccountsEasy(const FString& PlatformId, const FUnlinkUserAccountsSuccess& OnSuccess, const FErrorHandler& OnError);

private:
	UserManagement() = delete; // static class can't have instance
	UserManagement(UserManagement const&) = delete;
	UserManagement(UserManagement&&) = delete;

	static void CreateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FCreateUserAccountSuccess OnSuccess, FErrorHandler OnError);
	static void UpdateUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUpdateUserAccountSuccess OnSuccess, FErrorHandler OnError);
	static void AddUsernameAndPasswordResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FAddUsernameAndPasswordSuccess OnSuccess, FErrorHandler OnError);
	static void SendUserAccountVerificationCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FSendUserAccountVerificationCodeSuccess OnSuccess, FErrorHandler OnError);
	static void VerifyUserAccountResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FVerifyUserAccountSuccess OnSuccess, FErrorHandler OnError);
	static void SendPasswordResetCodeResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FSendPasswordResetCodeSuccess OnSuccess, FErrorHandler OnError);
	static void ResetPasswordResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FResetPasswordSuccess OnSuccess, FErrorHandler OnError);
	static void GetLinkedUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FGetLinkedUserAccountsSuccess OnSuccess, FErrorHandler OnError);
	static void LinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FLinkUserAccountsSuccess OnSuccess, FErrorHandler OnError);
	static void UnlinkUserAccountsResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool Successful, FUnlinkUserAccountsSuccess OnSuccess, FErrorHandler OnError);
};

} // Namespace Api
} // Namespace AccelByte
