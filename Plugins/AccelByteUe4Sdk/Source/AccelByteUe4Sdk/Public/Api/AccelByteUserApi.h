// Copyright (c) 2018-2020 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Models/AccelByteUserModels.h"
#include "Models/AccelByteEcommerceModels.h"
#include "Core/AccelByteError.h"


namespace AccelByte
{
	class Credentials;
	class Settings;
	namespace Api
	{
		class Entitlement;
		class Item;
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
			* @brief delegate for handling upgrade headless account notification.
			*/
			DECLARE_DELEGATE(FUpgradeNotif);

			/**
			 * @brief Log in with email/username account.
			 *
			 * @param Username User email address or username.
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
			void LoginWithOtherPlatform(EAccelBytePlatformType PlatformType, const FString& PlatformToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

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
			 * @param Username Email address of the user, can be used as login username.
			 * @param Password The Password.
			 * @param DisplayName The DisplayName.
			 * @param Country User's country, ISO3166-1 alpha-2 two letter, e.g. US.
			 * @param DateOfBirth User's date of birth, valid values are between 1905-01-01 until current date.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void Register(const FString& Username, const FString& Password, const FString& DisplayName, const FString& Country, const FString& DateOfBirth, const THandler<FRegisterResponse>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will register a new user with email-based account.
			 *
			 * @param EmailAddress Email address of the user, can be used as login username.
			 * @param Username The username can be used as login username, case insensitive, alphanumeric with allowed symbols underscore (_) and dot (.).
			 * @param Password The Password.
			 * @param DisplayName The DisplayName.
			 * @param Country User's country, ISO3166-1 alpha-2 two letter, e.g. US.
			 * @param DateOfBirth User's date of birth, valid values are between 1905-01-01 until current date.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void Registerv2(const FString& EmailAddress, const FString& Username, const FString& Password, const FString& DisplayName, const FString& Country, const FString& DateOfBirth, const THandler<FRegisterResponse>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will get data of currently logged in user.
			 *
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetData(const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will upgrade user's headless account. You may call SendUserAccountVerificationCode afterwards.
			 * Headless account is an account that doesn't have an email and password.
			 * If user logs in with a device/platform and they cannot login with email-and-password, their account is considered as a headless account.
			 * Therefore, the function requests user’s Username and Password for parameters.
			 *
			 * @param Username The EmailAddress of the user.
			 * @param Password The Password.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void Upgrade(const FString& Username, const FString& Password, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will upgrade user's headless account. You may call SendUserAccountVerificationCode afterwards.
			 * Headless account is an account that doesn't have an email and password.
			 * If user logs in with a device/platform and they cannot login with email-and-password, their account is considered as a headless account.
			 * Therefore, the function requests user’s Username and Password for parameters.
			 *
			 * @param EmailAddress The EmailAddress of the user.
			 * @param Username The Username.
			 * @param Password The Password.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void Upgradev2(const FString& EmailAddress, const FString& Username, const FString& Password, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will upgrade user's headless account. You may call SendUserAccountVerificationCode afterwards.
			 * Headless account is an account that doesn't have an email and password.
			 * If user logs in with a device/platform and they cannot login with email-and-password, their account is considered as a headless account.
			 *
			 * @param OnSuccess This will be called when the operation succeeded. Player Profile will be opened from default browser.
			 * @param OnError This will be called when the operation failed.
			 */
			void UpgradeWithPlayerPortal(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

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
			 * @brief Send request verification code with Change email context. User should login with email and password first to get access token.
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void SendUpdateEmailVerificationCode(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function should be called after you call SendUserUpgradeVerificationCode and obtain verification code.
			 *
			 * @param LoginId Email or phone number that will be used to upgrade the headless account.
			 * @param Password User's password.
			 * @param VerificationCode User's verification code that obtained from email.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

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
			 * @param PlatformType The PlatformType (Steam, PS4, Xbox, etc).
			 * @param Ticket The Ticket.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LinkOtherPlatform(EAccelBytePlatformType PlatformType, const FString& Ticket, const FVoidHandler& OnSuccess, const FCustomErrorHandler& OnError);

			/**
			 * @brief This function forced links user's current account to their other account in other platform. Use this only if the general LinkOtherPlatform get conflicted and getting confirmation from user.
			 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch platform).
			 * The browser will redirect the URL to a site with a code in form of parameter URL.
			 *
			 * @param PlatformType The PlatformType (Steam, PS4, Xbox, etc).
			 * @param PlatformUserId The UserId from the other platfrom you want to link.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void ForcedLinkOtherPlatform(EAccelBytePlatformType PlatformType, const FString& PlatformUserId, const FVoidHandler& OnSuccess, const FCustomErrorHandler& OnError);

			/**
			 * @brief This function links user's current account to their other account in other platform
			 * Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch).
			 * The browser will redirect the URL to a site with a code in form of parameter URL.
			 *
			 * @param PlatformType The PlatformType (Steam, PS4, Xbox, etc).
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void UnlinkOtherPlatform(EAccelBytePlatformType PlatformType, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will search user by their Email Address, Username, or Display Name.
			 *
			 * @param Query Targeted user's Email Address, Username, or Display Name.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FPagedPublicUsersInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void SearchUsers(const FString& Query, const THandler<FPagedPublicUsersInfo>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will search user by their Email Address, Username, or Display Name.
			 *
			 * @param Query Targeted user's Email Address, Username, or Display Name.
			 * @param By Filter the responded PagedPublicUsersInfo by SearchType. Choose the SearchType.ALL if you want to be responded with all query type.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FPagedPublicUsersInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void SearchUsers(const FString& Query, EAccelByteSearchType By, const THandler<FPagedPublicUsersInfo>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will search user by userId.
			 *
			 * @param UserId Targeted user's ID.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetUserByUserId(const FString& UserId, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will get user by other platform user id it linked to.
			 *
			 * @param PlatformType Other platform type .
			 * @param OtherPlatformUserId Targeted user's ID.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetUserByOtherPlatformUserId(EAccelBytePlatformType PlatformType, const FString& OtherPlatformUserId, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function for update user account info within the game.
			 *
			 * @param UpdateRequest The data you want to update. for DateOfBirth, the format is YYYY-MM-DD.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void UpdateUser(FUserUpdateRequest UpdateRequest, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function for update user account info within the game.
			 *
			 * @param UpdateEmailRequest Set verify code and new user email address.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void UpdateEmail(FUpdateEmailRequest UpdateEmailRequest, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will get user(s) by other platform user id(s) it linked to.
			 *
			 * @param PlatformType Other platform type .
			 * @param OtherPlatformUserId Targeted user's ID(s).
			 * @param OnSuccess This will be called when the operation succeeded. The result is FBulkPlatformUserIdResponse.
			 * @param OnError This will be called when the operation failed.
			 */
			void BulkGetUserByOtherPlatformUserIds(EAccelBytePlatformType PlatformType, const TArray<FString>& OtherPlatformUserId,
				const THandler<FBulkPlatformUserIdResponse>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will get country information from user's IP.
			 *
			 * @param OnSuccess This will be called when the operation succeeded. The result is FCountryInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetCountryFromIP(const THandler<FCountryInfo>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will check whether user can play the game by having it purchased or subscribed.
			 *
			 * @param OnSuccess This will be called when the operation succeeded. The result is boolean.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetUserEligibleToPlay(const THandler<bool>& OnSuccess, const FErrorHandler & OnError);

			/**
			* @brief Get JWT from specified Session Id.
			* he result is FAccelByteModelsOauth2Token.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FJsonWebTokeResponse.
			* @param OnError This will be called when the operation failed.
			*/
			void GetJsonWebToken(const THandler<FJsonWebTokenResponse>& OnSuccess, const FErrorHandler& OnError);

		private:
			User() = delete;
			User(User const&) = delete;
			User(User&&) = delete;

			void SendVerificationCode(const FVerificationCodeRequest& Request, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			void UpgradeWithPlayerPortalAsync(const FString& ReturnUrl, const THandler<FUpgradeUserRequest>& OnSuccess, const FErrorHandler& OnError);	
			
			void CheckUserSubEntitlement(const THandler<bool>& OnSuccess, const FErrorHandler & OnError);

			void CheckSubEntitlementFromItemInfo(const THandler<bool>& OnSuccess, const FErrorHandler& OnError, const FAccelByteModelsItemInfo& itemInfo);

			static FString TempUsername;
			FUpgradeNotif UpgradeNotif;

		public:
			void SetUpgradeNotifDelegate(const FUpgradeNotif& OnUpgradeNotif)
			{
				UpgradeNotif = OnUpgradeNotif;
			}
		};

	} // Namespace Api
} // Namespace AccelByte
