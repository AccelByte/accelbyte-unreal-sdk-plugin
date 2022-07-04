// Copyright (c) 2018-2022 AccelByte Inc. All Rights Reserved.
// This is licensed software from AccelByte Inc, for limitations
// and restrictions contact your company contract manager.

#pragma once

#include "CoreMinimal.h"
#include "Core/AccelByteError.h"
#include "Core/AccelByteHttpRetryScheduler.h"
#include "Core/AccelByteSettings.h"
#include "Models/AccelByteUserModels.h"
#include "Core/AccelByteApiBase.h"


namespace AccelByte
{
	namespace Api
	{
		class Entitlement;
		class Item;
		/**
		 * @brief User management API for creating user, verifying user, and resetting password.
		 */
		class ACCELBYTEUE4SDK_API User : public FApiBase
		{
		public:
			User(Credentials& Credentials, Settings& Settings, FHttpRetryScheduler& InHttpRef);
			~User();
		private:
			FHttpRetryScheduler& HttpRef;
			Credentials& CredentialsRef;
			Settings const& SettingsRef;
		public:
			/**
			* @brief delegate for handling upgrade headless account notification.
			*/
			DECLARE_DELEGATE(FUpgradeNotif);

			/**
			 * @brief These events happen before every login event, such as clearing CredentialRef post-auth info.
			 * - Call this last, just before the actual login call.
			 */
			void FinalPreLoginEvents() const;
			

			#pragma region Login Methods
			/**
			 * @brief Log in with email/username account.
			 *
			 * @param Username User email address or username.
			 * @param Password Password.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithUsername(
				const FString& Username,
				const FString& Password,
				const FVoidHandler& OnSuccess,
				const FErrorHandler& OnError) const;

			/**
			* @brief Log in with email/username account with 2FA enable
			*
			* @param Username User email address or username.
			* @param Password Password.
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			*/
			void LoginWithUsername(
				const FString& Username,
				const FString& Password,
				const FVoidHandler& OnSuccess,
				const FCustomErrorHandler& OnError) const;

			/**
			* @brief Log in with email/username account using v3 endpoint.
			*
			* @param Username User email address or username.
			* @param Password Password.
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			* @param bRememberMe This will use for refresh token expiration extension, default value is false. 
			*/
			void LoginWithUsernameV3(
				const FString& Username,
				const FString& Password,
				const FVoidHandler& OnSuccess,
				const FErrorHandler& OnError,
				const bool bRememberMe = false) const;

			/**
			* @brief Log in with email/username account using v3 endpoint with 2FA enable
			*
			* @param Username User email address or username.
			* @param Password Password.
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			* @param bRememberMe This will use for refresh token expiration extension, default value is false. 
			*/
			void LoginWithUsernameV3(
				const FString& Username,
				const FString& Password,
				const FVoidHandler& OnSuccess,
				const FCustomErrorHandler& OnError,
				const bool bRememberMe = false) const;

			/**
			 * @brief Log in with another platform account e.g. Steam, Google, Facebook, Twitch, etc.
			 *
			 * @param PlatformType Specify platform type that chosen by user to log in.
			 * @param PlatformToken Authentication code that provided by another platform.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithOtherPlatform(
				EAccelBytePlatformType PlatformType,
				const FString& PlatformToken,
				const FVoidHandler& OnSuccess,
				const FErrorHandler& OnError) const;

			/**
			* @brief Log in with another platform account e.g. Steam, Google, Facebook, Twitch, etc. with 2FA enable
			*
			* @param PlatformType Specify platform type that chosen by user to log in.
			* @param PlatformToken Authentication code that provided by another platform.
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			* @param bCreateHeadless If directly create new account when not linked yet, default value is true 
			*/
			void LoginWithOtherPlatform(
				EAccelBytePlatformType PlatformType,
				const FString& PlatformToken,
				const FVoidHandler& OnSuccess,
				const FCustomErrorHandler& OnError,
				bool bCreateHeadless = true) const;

			/**
			* @brief Log in with another platform Id account e.g. Steam, Google, Twitch, etc especially to support OIDC (with 2FA enable)
			*
			* @param PlatformId Specify platform type, string type of this field makes support OpenID Connect (OIDC)  
			* @param PlatformToken Authentication code that provided by another platform.
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			* @param bCreateHeadless If directly create new account when not linked yet, default value is true 
			*/
			void LoginWithOtherPlatformId(
				const FString& PlatformId,
				const FString& PlatformToken,
				const FVoidHandler& OnSuccess,
				const FCustomErrorHandler& OnError,
				bool bCreateHeadless = true) const;
			
			/**
			 * @brief Log in with device ID (anonymous log in).
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithDeviceId(const FVoidHandler& OnSuccess, const FErrorHandler& OnError) const;

			/**
			 * @brief Verify log in with new device when user enabled 2FA.
			 *
			 * @param MfaToken return from BE when user login with new device and 2FA enabled.
			 * @param AuthFactorType 2FA factor used. Could be "authenticator" or "backupCode". User should make sure what type used
			 * @param Code auth code from 3rd party authenticator or backupCode. 
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 * @param bRememberDevice
			 */
			void VerifyLoginWithNewDevice2FAEnabled(const FString& MfaToken, EAccelByteLoginAuthFactorType AuthFactorType, const FString& Code,
				const FVoidHandler& OnSuccess, const FCustomErrorHandler& OnError, bool bRememberDevice = false) const;

			/**
			 * @brief Log in from Accelbyte Launcher.
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithLauncher(const FVoidHandler& OnSuccess, const FErrorHandler& OnError) const;

			/**
			 * @brief login with refresh token
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithRefreshToken(const FVoidHandler& OnSuccess, const FErrorHandler& OnError) const;

			/**
			 * @brief login with refresh token
			 *
			 * @param RefreshToken the refresh token for login.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void LoginWithRefreshToken(const FString& RefreshToken, const FVoidHandler& OnSuccess, const FErrorHandler& OnError) const;

			/**
			 * @brief Create Headless Account And Login
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void CreateHeadlessAccountAndLogin(const FVoidHandler& OnSuccess, const FCustomErrorHandler& OnError) const;
			
			/**
			 * @brief Authentication With Platform Link And Login
			 *
			 * @param Username This is username's account exist
			 * @param Password This is password's account exist
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void AuthenticationWithPlatformLinkAndLogin(const FString& Username, const FString& Password, const FVoidHandler& OnSuccess, const FCustomErrorHandler& OnError) const;

			#pragma endregion /Login Methods
			
			/**
			 * @brief General handler for LoginWith* success; mostly a multicast callback handler.
			 * - Credentials: Inits Qos Latencies Scheduler.
			 * - Qos: Sets CredentialsRef Auth Token.
			 * @param OnSuccess
			 * @param Response 
			 */
			void OnLoginSuccess(const FVoidHandler& OnSuccess, const FOauth2Token& Response) const;

			/**
			 * @brief Log out current user session. Access tokens, user ID, and other credentials from memory will be removed.
			 *
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void Logout(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

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
			 * @brief This function will register a new user with email-based account and complete agreement.
			 *
			 * @param RegisterRequest invoke Accepted Policies, EmailAddress, Username, Password, Display Name, Country and Date of Birth
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void Registerv3(const FRegisterRequestv3& RegisterRequest, const THandler<FRegisterResponse>& OnSuccess, const FErrorHandler& OnError);
			
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
			 * @brief Verify user's email. User should login with email and password first to get access token.
			 *
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
			 * @param Username Email or phone number that will be used to upgrade the headless account.
			 * @param Password User's password.
			 * @param VerificationCode User's verification code that obtained from email.
			 * @param OnSuccess This will be called when the operation succeeded.
			 * @param OnError This will be called when the operation failed.
			 */
			void UpgradeAndVerify(const FString& Username, const FString& Password, const FString& VerificationCode, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);
			
			/**
			* @brief This function should be called after you call SendUpgradeVerificationCode and obtain verification code.
			*
			* @param UpgradeAndVerifyRequest contain user data will be used to upgrade the headless account.
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			*/
			void UpgradeAndVerify2(const FUpgradeAndVerifyRequest& UpgradeAndVerifyRequest, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

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
			 * @brief This function will search user by their Username or Display Name. The query will be used to find the user with the most approximate username or display name.
			 *
			 * @param Query Targeted user's Username or Display Name.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FPagedPublicUsersInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void SearchUsers(const FString& Query, const THandler<FPagedPublicUsersInfo>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will search user by their Username or Display Name. The query will be used to find the user with the most approximate username or display name.
			 *
			 * @param Query Targeted user's Username or Display Name.
			 * @param Offset Targeted offset query filter.
			 * @param Limit Targeted limit query filter.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FPagedPublicUsersInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void SearchUsers(const FString& Query, int32 Offset, int32 Limit, const THandler<FPagedPublicUsersInfo>& OnSuccess, const FErrorHandler& OnError);
			
			/**
			 * @brief This function will search user by their Username or Display Name. The query will be used to find the user with the most approximate username or display name.
			 *
			 * @param Query Targeted user's Username or Display Name.
			 * @param By Filter the responded PagedPublicUsersInfo by SearchType. Choose the SearchType.ALL if you want to be responded with all query type.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FPagedPublicUsersInfo.
			 * @param OnError This will be called when the operation failed.
			 * @param Offset Targeted offset query filter.
			 * @param Limit Targeted limit query filter.
			 */
			void SearchUsers(const FString& Query, EAccelByteSearchType By, const THandler<FPagedPublicUsersInfo>& OnSuccess, const FErrorHandler& OnError, const int32& Offset = 0, const int32& Limit = 100);

			/**
			 * @brief This function will search user by userId.
			 *
			 * @param UserId Targeted user's ID.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void GetUserByUserId(const FString& UserId, const THandler<FSimpleUserData>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will get user by other platform user id it linked to.
			 * Note : For Nintendo Login you have to add Environment ID Suffix behind platform id with PlatformID:EnvironmentID as format
			 * e.g csg123jskansdk:dd1
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
			* Note : For Nintendo Login you have to add Environment ID Suffix behind platform id with PlatformID:EnvironmentID as format
			 * e.g csg123jskansdk:dd1
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
			 * @brief This function will get user(s) information like user's DisplayName.
			 *
			 * @param UserIds List UserId(s) to get.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FListBulkUserInfo.
			 * @param OnError This will be called when the operation failed.
			 */
			void BulkGetUserInfo(const TArray<FString>& UserIds, const THandler<FListBulkUserInfo>& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function will get user input validation
			 * 
			 * @param  LanguageCode Targeted Language Code, using ISO-639 
			 * @param  OnSuccess This will be called when the operation succeeded. The result is FInputUserValidation.
			 * @param  OnError This will be called when the operation failed.
			 * @param  bDefaultOnEmpty Targeted DefaultOnEmpty. 
			 */
			void GetInputValidations(const FString& LanguageCode, THandler<FInputValidation> const& OnSuccess, FErrorHandler const& OnError, bool bDefaultOnEmpty = true);

			/**
			* @brief This function will enable 2FA with backupCode.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FUser2FaBackupCode.
			* @param OnError This will be called when the operation failed.
			*/
			void Enable2FaBackupCode(const THandler<FUser2FaBackupCode>& OnSuccess, const FErrorHandler& OnError);

			/**
			* @brief This function will disable 2FA with backupCode.
			*
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			*/
			void Disable2FaBackupCode(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			* @brief This function will generate backupCode.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FUser2FaBackupCode.
			* @param OnError This will be called when the operation failed.
			*/
			void GenerateBackupCode(const THandler<FUser2FaBackupCode>& OnSuccess, const FErrorHandler& OnError);

			/**
			* @brief This function will get backupCode.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FUser2FaBackupCode.
			* @param OnError This will be called when the operation failed.
			*/
			void GetBackupCode(const THandler<FUser2FaBackupCode>& OnSuccess, const FErrorHandler& OnError);

			/**
			* @brief This function will enable 3rd party authenticator.
			*
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			*/
			void Enable2FaAuthenticator(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			* @brief This function will disable 3rd party authenticator.
			*
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			*/
			void Disable2FaAuthenticator(const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			* @brief This function will generate secret key for linking AB 2fa with 3rd party authenticator apps.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FUser2FaSecretKey.
			* @param OnError This will be called when the operation failed.
			*/
			void GenerateSecretKeyFor2FaAuthenticator(const THandler<FUser2FaSecretKey>& OnSuccess, const FErrorHandler& OnError);

			/**
			* @brief This function will get user 2FA factor enabled.
			*
			* @param OnSuccess This will be called when the operation succeeded. The result is FUser2FaMethod.
			* @param OnError This will be called when the operation failed.
			*/
			void GetEnabled2FaFactors(const THandler<FUser2FaMethod>& OnSuccess, const FErrorHandler& OnError);

			/**
			* @brief This function will make default factor for 2FA.
			*
			* @param AuthFactorType AuthFactorType user want to set as default factor
			* @param OnSuccess This will be called when the operation succeeded.
			* @param OnError This will be called when the operation failed.
			*/
			void MakeDefault2FaFactors(EAccelByteLoginAuthFactorType AuthFactorType ,const FVoidHandler& OnSuccess, const FErrorHandler& OnError);

			/**
			 * @brief This function for update user account info within the game.
			 *
			 * @param UpdateRequest The data you want to update. for DateOfBirth, the format is YYYY-MM-DD.
			 * @param OnSuccess This will be called when the operation succeeded. The result is FAccountUserData.
			 * @param OnError This will be called when the operation failed.
			 */
			void UpdateUserV3(FUserUpdateRequest UpdateRequest, const THandler<FAccountUserData>& OnSuccess, const FErrorHandler& OnError);

		private:
			User() = delete;
			User(User const&) = delete;
			User(User&&) = delete;

			void SendVerificationCode(const FVerificationCodeRequest& Request, const FVoidHandler& OnSuccess, const FErrorHandler& OnError);
		};

	} // Namespace Api
} // Namespace AccelByte
