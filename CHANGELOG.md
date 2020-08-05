# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

## [2.19.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.19.0%0D2.18.6) (2020-08-05)


### Features

* game telemetry scheduler ([e2bebe6](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e2bebe69196d27c044414c91e8537039ff1c492d))


### Bug Fixes

* **cloudstorage:** Fix CloudStorage API documentation. ([b030ce0](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/b030ce0aa02a7f45af3bf1777e2f50b254b8fe2e))
* **cloudstorage:** Mark Status field as DEPRECATED and add Size field in the FAccelByteModelsSlot model. ([64a0a6f](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/64a0a6f6b4d6fa22a94619aec201ce25e531e5ca))
* **cloudstorage:** Mark UpdateSlotMetadata function with filename  parameter as deprecated ([22f2df3](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/22f2df3ac8bac90f2fc2c8f2305384e8f21931ef))
* **cloudstorage:** Overload UpdateSlotMetdata()  function. ([7aeea01](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/7aeea01466a753e49539b86940078acd089103eb))
* **config:** remove leaderboard server URL from server SDK config ([7b25fce](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/7b25fceec67171efa82b2223095c01dc9b869c01))
* **jenkins:** add SERVER_STATISTIC_PREFIX for jenkins config ([f09f161](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f09f161a00d521447841c08e945045857d09e30e))
* **statistic:** Update statistic error codes. ([f0c050c](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f0c050c1146de38749a870234597c5c47cb748f5))


### CI

* **jenkins:** temporary social service env flag ([41ec02d](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/41ec02d7114e1a5889ed6919467c691963bbe85f))

### [2.18.6](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.18.6%0D2.18.5) (2020-07-22)


### Refactors

* **envar:** clean up warning and refactor envar ([ee90f12](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/ee90f1275334ea78f3b4234951db4359820febd6))

### [2.18.5](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.18.5%0D2.18.4) (2020-07-08)

## 2.18.4 (2020-07-03)
### Changed
- Agreement
    - GetLegalPolicies
        - Add policyType and tags param
    - GetLegalPoliciesByCountry
        - Add policyType and tags param

## 2.18.3 (2020-07-03)
### Fixed
- Lobby Api Blueprint
  - Fix delegate notif not executed

## 2.18.2 (2020-06-30)
### Fixed
- Telemetry Api
  - #include header path not complete
- Server DSM Api
  - Agones preprocessor
  - ServerDSMApi warning C4458 signature hides class member

## 2.18.1 (2020-06-25)
### Fixed
- Server DSM Api
  - Move args parsing from constructor

## 2.18.0 (2020-06-22)
### Added
- CloudSave
      - Add SaveUserRecord
      - Add ReplaceUserRecord
      - Add GetUserRecord
      - Add GetPublicUserRecord
      - Add DeleteUserRecord
      - Add SaveGameRecord
      - Add ReplaceGameRecord
      - Add GetGameRecord
      - Add DeleteGameRecord
      
## 2.17.0 (2020-06-19)
### Added
- Agones SDK UE4 plugin
- Register server using Agones provider

## 2.16.0 (2020-06-16)
### Added
- Agreement Api
### Fixed
- Fix third party platform string

## 2.15.0 (2020-06-03)
### Added
- Game Client SDK
  - User
    - Add UpgradeWithPlayerPortal 
    - Add GetCountryFromIP
  - Entitlement
    - Add CreateDistributionReceiver
    - Add GetDistributionReceiver
    - Add DeleteDistributionReceiver
    - Add UpdateDistributionReceiver

## 2.14.0 (2020-05-29)
### Added
- ServerTelemetry Api

## 2.13.0 (2020-05-29)
### Added
- GameTelemetry Api

## 2.12.1 (2020-05-28)
### Added
- User
    - added `ps4` and xbox `live` enum for 3rd party login

## 2.12.0 (2020-05-20)
### Added
- Lobby
    - Adding Bulk Friend Request
- User
    - Adding BulkGetUserByOtherPlatformId function

## 2.11.0 (2020-04-29)
### Added
- Leaderboard Api

## 2.10.0 (2020-04-08)
### Added
- UserProfile Api
    - GetCustomAttributes
    - UpdateCustomAttributes
### Changed
- UserProfile Api
    - UpdateUserProfile success result handler
- UserProfile Blueprints
    - UpdateUserProfile success result handler

## 2.9.0 (2020-04-06)
### Added
- UserProfile Api
    - GetCustomAttributes
    - UpdateCustomAttributes
- ServerQosManager
    - GetServerLatencies
    - GetQosServers
- ServerDSM
    - GetRegionDSMUrl
    - GetPubIp
### Changed
- Game Client SDK 
  - Retry network error request
  - Changed FAccelByteModelsSlot DateAccessed, DateCreated, DateModified from float to FDateTime
- Lobby
    - Fix lobby reconnect backoff retry, reconnect timeout, ping delay
- ServerDSM
    - Register Server will check DSMUrl, Provider, and Public Ip first before send request.
    - Add ErrorRetry param on ConfigureHeartBeat
    - Will stop sending heartbeat after errorRetry number reached
### Fixed
- Game Client SDK 
  - Fix Lobby UnbindEvent
  - Fix Linux compilation error
  - Fix AccelByteReport response invalid handler
  - Fix credentials is not cleared after logout

## 2.8.0 (2020-03-26)
### Added
- Session Management
    - Automated session refresh

## 2.7.0 (2020-03-03)
## Added
- Game Server SDK
  - ServerCredentials: stores ClientAccessToken, ClientNamespace, and MatchId
  - ServerSettings: stores server's settings such as ClientId, ClientSecret, IAM URL, DSM URL, etc.
  - CustomWebServerApi: to open a WebServer for accept DSM claim request
  - ServerOauth2Api
    - LoginWithClientCredentials
  - ServerDSMApi
    - RegisterServerToDSM
    - SendShutdownToDSM
    - RegisterLocalServerToDSM
    - DeregisterLocalServerFromDSM
  - ServerStatisticApi
    - CreateUserStatItems
    - IncrementManyUsersStatItems
    - IncrementUserStatItems
    - GetAllUserStatItems
- Game Client SDK 
  - Statistic
    - CreateUserStatItems
    - IncrementUserStatItems
## Removed
    - GetUserStatItemsByStatCodes
    - GetUserStatItemsByTags
    - BulkAddStatItemValue
    - BulkAddUserStatItemValue
    - AddUserStatItemValue
    
  - ServerEcommerce
    - GetUserEntitlementById
    - GrantUserEntitlements
    - CreditUserWallet
- Game Client SDK 
  - Entitlement
    - GetUserEntitlementById
    - ConsumeUserEntitlement
  - Qos & QosM Api
    - GetServerLatencies
    - GetQosServers
## Changed
- Game Client SDK
  - Lobby
    - SendStartMatchmaking(FString GameMode) -> SendStartMatchmaking(FString GameMode, FString ServerName, FString ClientVersion, TArray<TPair<FString, float>> Latencies)
## Removed
  - CustomWebServer
  - CustomWebServerApi
## Fixed
- User Integration Test
  - Fix UpgradeHeadlessDeviceAccount

## 2.6.1 (2020-02-07)
## Fixed
- Game Client SDK
  - Fix LoginWithOtherPlatform PlatformType

## 2.6.0 (2020-01-29)
## Added
- Game Client SDK 
  - User Api
    - GetUserByOtherPlatformUserId

## Changed
- GetUserByEmailAddress -> SearchUser
  - const FString& EmailAddress -> const FString& Query
  
## 2.5.0 (2019-11-25)
## Fixed
- Platform
  - ErrorCode Refactor
  - Models
    - FAccelByteModelsFullCategoryInfo -> FAccelByteModelsCategoryInfo
      - FString CreatedAt -> FDateTime CreatedAt
      - FString UpdatedAt -> FDateTime UpdatedAt
    - FAccelByteModelsItemInfoRegionData -> FAccelByteModelsItemRegionDataItem
      - FString CurrencyType -> EAccelByteItemCurrencyType CurrencyType
      - FString PurchaseAt -> FDateTime PurchaseAt
      - FString ExpireAt -> FDateTime ExpireAt
      - FString DiscountPurchaseAt -> FDateTime DiscountPurchaseAt
      - FString DiscountExpireAt -> FDateTime DiscountExpireAt
    - FAccelByteModelsItemInfoImage -> FAccelByteModelsItemImage
      - FString As -> Added
    - FAccelByteModelsItemInfo
      - FString BaseAppId -> Added
      - FString EntitlementType -> EAccelByteEntitlementType EntitlementType
      - bool Stackable -> Added
      - FString Status -> EAccelByteItemStatus Status
      - FString ItemType -> EAccelByteItemType ItemType
      - FString ThumbnailUrl -> Added
      - FAccelByteModelsItemInfoImage ThumbnailImage -> Deleted
      - FString Clazz -> Added
      - FString BoothName -> Added
      - int32 DisplayOrder -> Added
      - FString Ext -> Added
      - FString LocalExt -> Added
    - FAccelByteModelsPopulatedItemInfo -> Added
    - FAccelByteModelsItemCriteria -> Added
    - FAccelByteModelsEntitlementItemSnapshot
      - FString BaseAppId -> Added
      - EAccelByteEntitlementType EntitlementType -> Added
      - bool Stackable -> Added
      - FString ThumbnailUrl -> Added
      - FAccelByteModelsItemInfoImage ThumbnailImage -> Deleted
      - FString TargetItemId -> Added
      - int32 MaxCount -> Added
      - FString BoothName -> Added
    - FAccelByteModelsEntitlementInfo
      - FString GrantedCode -> Added
      - FString BundleItemId -> Deleted
      - EAccelByteEntitlementSource Source -> Added
      - FString GrantedAt -> FDateTime GrantedAt
      - FString CreatedAt -> FDateTime CreatedAt
      - FString UpdatedAt -> FDateTime UpdatedAt
    - FAccelByteModelsOrderInfoCurrencySummary -> FAccelByteModelsOrderCurrencySummary
      - FString CurrencyType -> EAccelByteItemCurrencyType CurrencyType
    - FAccelByteModelsOrderInfo
      - FString PaymentOrderNo -> Added
      - bool bSandbox -> bool Sandbox
      - FAccelByteModelsOrderInfoPaymentUrl PaymentUrl -> Deleted
      - TArray<FAccelByteModelsOrderInfoTransaction> Transactions -> Deleted
      - TArray<FString> EntitlementIds > Deleted
      - FString Status -> EAccelByteOrderStatus Status
      - FAccelByteModelsEntitlementItemSnapshot ItemSnapshot -> Added
      - FString CreatedTime -> FDateTime CreatedTime
      - FString ChargedTime -> FDateTime ChargedTime
      - FString FulfilledTime -> FDateTime FulfilledTime
      - FString RefundedTime -> FDateTime RefundedTime
      - FDateTime ChargebackTime -> Added
      - FDateTime ChargebackReversedTime -> Added
      - FString CreatedAt -> FDateTime CreatedAt
      - FString UpdatedAt -> FDateTime UpdatedAt
    - FAccelByteModelsOrderHistoryInfo
      - FString Namespace -> Added
      - FString CreatedAt -> FDateTime CreatedAt
      - FString UpdatedAt -> FDateTime UpdatedAt
    - FAccelByteModelsOrderCreate
      - FString Region -> Added
      - FString Language -> Added
    - FAccelByteModelsWalletInfo
      - FString Status -> EAccelByteItemStatus Status
      - FString CreatedAt -> FDateTime CreatedAt
      - FString UpdatedAt -> FDateTime UpdatedAt
  - Api
    - Entitlement::QueryUserEntitlement()
      - Param: int32 Page -> const int32& Offset
      - Param: int32 Size -> const int32& Limit
    - Item::GetItemsByCriteria()
      - Param: const FAccelByteModelsItemCriteria& ItemCriteria -> Added
      - Param: const FString& Language -> Deleted
      - Param: const FString& Region -> Deleted
      - Param: const FString& CategoryPath -> Deleted
      - Param: const EAccelByteItemType& ItemType -> Deleted
      - Param: const EAccelByteItemStatus& Status -> Deleted
      - Param: int32 Page -> const int32& Offset
      - Param: int32 Size -> const int32& Limit
    - Item::SearchItem()
      - Param: int32 Page -> const int32& Offset
      - Param: int32 Size -> const int32& Limit
    - Wallet::GetWalletInfoByCurrencyCode()
      - Url: "%s/public/namespaces/%s/users/%s/wallets/%s" -> "%s/public/namespaces/%s/users/me/wallets/%s"
    - Order::FulfillOrder() -> Deleted
    
## 2.4.0 (2019-11-11)
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
  - Model:
    - FAccelByteModelsUserStatItemInfo
      - UPROPERTY change: profileId -> userId
    - FAccelByteModelsBulkUserStatItemInc
      - UPROPERTY change: profileId -> userId
- Statistic API Standardization
  -API:
    - GetAllStatItems() & GetStatItemsByStatCodes() -> GetUserStatItems()
      - URL change: `GET` "/public/namespaces/%s/users/%s/profiles/%s/statitems" -> `GET` "/v1/public/namespaces/%s/users/%s/statitems"
    - BulkAddStatItemValue()
      - URL change: `POST` "/public/namespaces/%s/statitems/bulk/inc" -> `PUT` "/v1/public/namespaces/%s/statitems/value/bulk"
    - BulkAddUserStatItemValue()
      - URL change: `POST` "/public/namespaces/%s/users/%s/profiles/%s/statitems/bulk/inc" -> `PUT` "/v1/public/namespaces/%s/users/%s/statitems/value/bulk"
    - AddUserStatItemValue()
      - URL change: `POST` "/public/namespaces/%s/users/%s/profiles/%s/stats/%s/statitems/inc" -> `PUT` "/v1/public/namespaces/%s/users/%s/stats/%s/statitems/value"
  - Model:
    - FAccelByteModelsUserStatItemInfo
      - Add member TArray<FString> Tags
## Added
- Log
  - Add HTTP Request/Response Log
  - Add Current Function Log
- Statistic
  - Add GetUserStatItemsByTags()
    - URL: `GET` "/v1/public/namespaces/{namespace}/users/{userId}/statitems"
    - Query param: "tags"
    
## 2.3.0 (2019-10-15)
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
## 2.0.0 (2019-09-06)
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

## 1.0.0-beta.1 (2019-03-13)
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
