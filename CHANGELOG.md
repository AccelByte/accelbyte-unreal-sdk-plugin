# Changelog

## Unreleased
## Fixed
- Basic
  - Follow API Standardization
    - Endpoint Url:
        - UserProfile::CreateUserProfile() "%s/public/namespaces/%s/users/me/profiles" -> "%s/v1/public/namespaces/%s/users/me/profiles"
        - UserProfile::GetPublicUserProfileInfo() "%s/public/namespaces/%s/users/%s/profiles/public" -> "%s/v1/public/namespaces/%s/users/%s/profiles/public"
        - UserProfile::UpdateUserProfile() "%s/public/namespaces/%s/users/me/profiles" -> "%s/v1/public/namespaces/%s/users/me/profiles"
        - UserProfile::GetUserProfile() "%s/public/namespaces/%s/users/me/profiles" -> "%s/v1/public/namespaces/%s/users/me/profiles"
  - Cleanup user profile on integration test
## Changed
- Statistic API Standardization
  - GetAllStatItems() & GetStatItemsByStatCodes() -> GetProfileStatItems()
    - URL change: `GET` "/public/namespaces/%s/users/%s/profiles/{profileId}/statitems" -> `GET` "/v1/public/namespaces/%s/users/%s/profiles/%s/statitems"
  - BulkAddStatItemValue()
    - URL change: `POST` "/public/namespaces/%s/statitems/bulk/inc" -> `PUT` "/v1/public/namespaces/%s/statitems/value/bulk"
  - BulkAddUserStatItemValue() -> BulkAddProfileStatItemValue()
    - URL change: `POST` "/public/namespaces/%s/users/%s/profiles/%s/statitems/bulk/inc" -> `PUT` "/v1/public/namespaces/%s/users/{userId}/profiles/%s/statitems/value/bulk"
  - AddUserStatItemValue() -> AddProfileStatItemValue()
    - URL change: `POST` "/public/namespaces/%s/users/%s/profiles/%s/stats/%s/statitems/inc" -> `PUT` "/v1/public/namespaces/%s/users/%s/profiles/%s/stats/%s/statitems/value"
## Added
- Statistic API Standardization
  - GetUserStatItems()
    - URL: `GET` "/v1/public/namespaces/%s/users/%s/statitems"
  - BulkAddUserStatItemValue()
    - URL: `PUT` "/v1/public/namespaces/%s/users/%s/statitems/value/bulk"
  - AddUserStatItemValue()
    - URL: `PUT` "/v1/public/namespaces/%s/users/%s/stats/%s/statitems/value"

## v2.1.0 (2019-10-04)
## Fixed
- Ecommerce Test
  - Change CurrencyCode from "Ue4SdkCoin" to "SDKC"
  - Change Store name separator from '_' to '-'
- Automation
  - Change package format from "AccelByteUe4SdkPackage-{GIT_SHA}-BUILD-{BUILD_NUMBER}.7z" to "AccelByteUe4SdkPackage-{GIT_SHA}-{SDK_VERSION}.7z"
- IAM
  - Follow API Standardization
    - Models:
        - FRegisterRequest.LoginId -> FRegisterRequest.EmailAddress
        - FResetPasswordRequest.LoginId -> FResetPasswordRequest.EmailAddress
    - Endpoint Url:
        - User::GetData(): "%s/v2/public/users/me" -> "%s/v3/public/users/me"
        - User::SendResetPasswordCode(): "%s/v2/public/namespaces/%s/users/forgotPassword" -> "%s/v3/public/namespaces/%s/users/forgot"
        - User::ResetPassword(): "%s/v2/public/namespaces/%s/users/resetPassword" -> "%s/v3/public/namespaces/%s/users/reset"
        - User::LinkOtherPlatform(): "%s/v2/public/namespaces/%s/users/me/platforms/%s/link" -> "%s/v3/public/namespaces/%s/users/me/platforms/%s"
        - User::UnlinkOtherPlatform(): "%s/v2/public/namespaces/%s/users/me/platforms/%s/link" -> "%s/v3/public/namespaces/%s/users/me/platforms/%s"
        - User::GetUserByLoginId(): "%s/namespaces/%s/users/byLoginId?loginId=%s" -> "%s/v3/public/namespaces/%s/users?emailAddress=%s"
        - User::GetPublicUserInfo(): "%s/v2/public/namespaces/%s/users/%s" -> "%s/iam/v3/public/namespaces/%s/users/%s"
    - New Endpoint:
        - User::UpgradeAndVerify(): "%s/v3/public/namespaces/%s/users/me/headless/code/verify"
    - Response Changes:
        - Register: FUserData -> FRegisterResponse
- Integration Test
  - Follow new password input restriction
  - Statistic: add test teardown
- Model
  - Add GeneralModels
  - Add FBan and FPermission on UserModels
  - Add FRegisterResponse
  - Update UserData
## v2.0.0 (2019-09-06)
## Added
- Session Management
  - Implementing API Gateway
- Supported Game Profile Service Features
  - Create Game Profile
  - Get Game Profile
  - Update Game Profile
  - Delete Game Profile
- Supported Statistic Service Features
  - Get StatItem
  - Update StatItem from Client
- Supported Lobby Service Features
  - Ready Consent
  - DS Notif
- Updated to v2.0.0 for Justice release v2.0.0 

## v1.0.0-beta.1 (2019-03-13)
### Added

- Supported IAM Service Features:
  - Login With Username and Password
  - Login With Other Platforms (Google, Facebook, and Steam)
  - Login With AccelByte Launcher
  - Login With Device ID
  - User Registration
  - User Verification (via email)
  - Upgrade device account and other platform account to proper AccelByte account with username and password
  - Update User Data
- Supported Basic Service Features
  - Create User Profile
  - Get User Profile
  - Update User Profile
  - Delete User Profile
- Supported Platform and E-Commerce Service Features:
  - Get Wallet Info
  - Create Order
  - Get Entitlements
  - Browse catalog (categories and items)
- Supported Lobby Service Features
  - Private Chat
  - Party
  - Friends
  - Matchmaking
  - Notification
