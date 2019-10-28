// Copyright (c) 2018-2019 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "AccelByteUe4Sdk/Public/Models/AccelByteUserModels.h"
#include "AccelByteUe4Sdk/Public/Core/AccelByteError.h"


namespace AccelByte
{
	class Credentials;
	class Settings;
	namespace Api
	{

		/**
		 * @brief User management API for creating user, verifying user, and resetting password.
		 */
		class ACCELBYTEUE4SDK_API User
		{
		public:
			User(Credentials& Credentials, Settings& Settings);
			~User();
		private:
			Credentials& Credentials;
			Settings& Settings;
		public:
			/**
			 * @brief This is to get access token from `client_credentials` grant, then store the access token in memory.
			 * You shouldn't use this if your application is a client. It is intended for machine-to-machine (server).
			 * It is "required" for user management (create new user, reset password, etc).
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithClientCredentials(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief Log in with email/phone number account.
			 *
			 * @param Username User email address.
			 * @param Password Password.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithUsername(const FString& Username, const FString& Password, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief Log in with another platform account e.g. Steam, Google, Facebook, Twitch, etc.
			 *
			 * @param PlatformId Specify platform type that chosen by user to log in.
			 * @param PlatformToken Authentication code that provided by another platform.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithOtherPlatform(EAccelBytePlatformType PlatformId, const FString& PlatformToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief Log in with device ID (anonymous log in).
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithDeviceId(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief login from Accelbyte Launcher
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithLauncher(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief Remove access tokens, user ID, and other credentials from memory.
			 */
			void ForgetAllCredentials();

			/**
			 * @brief This function will register a new user with email-based account.
			 *
			 * @param Username User email address or phone number.
			 * @param Password The Password.
			 * @param DisplayName The DisplayName.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void Register(const FString& Username, const FString& Password, const FString& DisplayName, const FString& Country, const FString& DateOfBirth, const THandler<FRegisterResponse>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will get data of currently logged in user.
			 *
			 * @param OnSuccess This will be called when the operation succeeded. The result is FUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetData(const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError);

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
			void Upgrade(const FString& Username, const FString& Password, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief Verify user's email. User should login with email and password first to get access token.
			 *
			 * @param Username User email address or phone number.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void SendVerificationCode(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will verify the registered email **after** user receives verification code sent with ::SendUserAccountVerificationCode() to their email.
			 *
			 * @param VerificationCode The VerificationCode.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void Verify(const FString& VerificationCode, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief Send a request to reset user's password. For some reason the endpoint uses HTTP basic authentication instead of bearer, because you know... our IAM is very inconsistent.
			 *
			 * @param Username User email address or phone number.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void SendResetPasswordCode(const FString& Username, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

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
			void ResetPassword(const FString& VerificationCode, const FString& Username, const FString& NewPassword, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function should be done before user upgrade their headless account. After this function successfully called, obtain the verification code from the submitted email. Then call UpgradeHeadlessAccountWithVerificationCode function afterwards.
			 *
			 * @param LoginId Email or phone number that will be used to upgrade the headless account.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			*/
			void SendUpgradeVerificationCode(const FString& Username, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function should be called after you call SendUserUpgradeVerificationCode and obtain verification code.
			 *
			 * @param LoginId Email or phone number that will be used to upgrade the headless account.
			 * @param Password User's password.
			 * @param VerificationCode User's verification code that obtained from email.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function gets user's platform accounts linked to user’s account.
			 *
			 * @param OnSuccess This will be called when the operation succeeded. The result is TArray<FPlatformLink>.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetPlatformLinks(const THandler<FPagedPlatformLinks>& OnSuccess, const FErrorHandler& OnError);

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
			void LinkOtherPlatform(const FString& PlatformId, const FString& Ticket, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function links user's current account to their other account in other platform
			 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch).
			 * The browser will redirect the URL to a site with a code in form of parameter URL.
			 *
			 * @param PlatformId The PlatformId.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void UnlinkOtherPlatform(const FString& PlatformId, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will search user by their Email Address.
			 *
			 * @param EmailAddress Targeted user's Email Address.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FPagedPublicUsersInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetUserByEmailAddress(const FString& EmailAddress, const THandler<FPagedPublicUsersInfo>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will search user by userId.
			 *
			 * @param UserId Targeted user's ID.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetUserByUserId(const FString& UserId, const THandler<FUserData>& OnSuccess, const FErrorHandler& OnError);

		private:
			User() = delete;
			User(User const&) = delete;
			User(User&&) = delete;

			void SendVerificationCode(const FVerificationCodeRequest& Request, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			static FString TempUsername;
		};

	} // Namespace Api
} // Namespace AccelByte
