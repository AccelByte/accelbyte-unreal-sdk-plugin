# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

## [2.34.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.34.0%0D2.33.0) (2021-04-28)


### Features

* **cloudsave:** add Cloud Save APIs for Server SDK ([d050d14](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/d050d1469f0c29c535e5fabc93514d01a1b30c14))
* **ecommerce:** add bulk entitlement lookup ([e86769b](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e86769ba214f565893b5d2fb337f807139e9d1ba))
* **ecommerce:** add Debit wallet ([5355b33](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/5355b33a92dbd2a1fccbfb293d1ca07704e296ca))
* **lobby:** add custom port field in DSNotice lobby model. ([d7a2722](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/d7a27223776817022409482f9c7888b097a7ac5b)), closes [#288](https://accelbyte.atlassian.net/browse/288)
* **lobby:** add custom port field in DSNotice lobby model. ([faeef4b](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/faeef4bca4341cd2b69995fc5a94b8ac3e00d255))
* **server:** Re-Login mechanism to renew token on Server SDK ([295cd02](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/295cd0267696366c9e12e485f31432e15917a92f))


### Bug Fixes

* **ecommerce:** fix integration test for bulk query entitlement ([e3b8241](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e3b82414275cb1f8933f87c5feb25467a0bd2ccb))


### Tests

* **dsmconfig:** fix missing protocol field in dsmconfig ([cdf912b](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/cdf912b98137e59f9fafab8dbbb2a3b96c9aec8f)), closes [#294](https://accelbyte.atlassian.net/browse/294)

## [2.33.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.33.0%0D2.32.0) (2021-04-14)


### Features

* **entitlement:** add entitlement sync for 3rd party platform purchases ([aef37f1](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/aef37f1264049659e32aa8e8b907d9e539227bcb)), closes [#272](https://accelbyte.atlassian.net/browse/272)
* **iam:** add endpoint to retrieve JWT from particular session id on apigw ([8611590](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/861159011c16b012515a005e562a6c4716795fc7))
* **server:** remove agones ([3653a4f](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/3653a4f0798a68f544c36d96c8cb886215743a20))


### Bug Fixes

* **http:** fixed http flush on shutdown ([7521cf9](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/7521cf91406f08901fce45f0d1d588f96ec83182))
* **http:** Not started http requests needs to wait for timeout before return error ([7014eed](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/7014eedd6671cd8d36021ce7eaf31d4a7e2ba0fe))
* **lobby:** create a new websocket instance when connection state error ([8ceae30](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/8ceae30b4219dbc71f439dce1a95942c3131aa75))
* **lobby:** fixed websocket instance connection error state reconnecting ([5690f02](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/5690f02c2074237e043ba0b148387dbc7b696763))

## [2.32.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.32.0%0D2.31.0) (2021-03-17)


### Features

* Add ability to set AccelByteReportLog's verbosity ([94d07c0](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/94d07c0568c9c9bd97b6a0b1de3c26bf124a661d))
* add session browser api ([4f86f6e](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/4f86f6e2cfd9a024ca0d9d1b6bfc026bbb91886d)), closes [#270](https://accelbyte.atlassian.net/browse/270)
* add signaling p2p to lobby ([90ca01c](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/90ca01c08719d61f1b31d33279a56776733f561c)), closes [#263](https://accelbyte.atlassian.net/browse/263)
* **plugin:** remove platform whitelist ([5a52ebb](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/5a52ebb682ee8351e8e832a1835c99738061c15a))


### Bug Fixes

* **error:** check field before try parsing ([070bfc0](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/070bfc0e8a8e85ec1ecd9d4b4f243a8715a7b777))
* AccelByteUe4SdkDemo project blueprint ([5e68276](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/5e682765384cca02f4d9e1361a6912a3e68ebe0f))
* Remove #pragma once from cpp file, which can cause compilation failures on some platforms. ([4a85981](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/4a85981803fc6131d644126073620d557372964e))
* **error:** check field before try parsing ([e0caf24](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e0caf240e980852d92a1c78af1f03d32d4faea3e))


### Tests

* **joinable:** fix joinable test case failed because of persisting party between test session ([7f1c4db](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/7f1c4db434fea016cfad13db925bffc657a66d87))

## [2.31.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.31.0%0D2.30.0) (2021-03-03)


### Features

* **partyStorage:** add party storage client function ([d824de7](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/d824de7a4dbfc1f1b02be9d54baaf82400e18793))
* **serverLobby:** add party storage ([5ff594f](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/5ff594ffe5abae83e2f79ad49cc5d3ea8bd3a9e3))


### Bug Fixes

* **chore:** remove array ([fb1c9bc](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/fb1c9bcfe3a37782bbf9b2a8cc5643d1969373e6))
* **serverLobby:** simplify baseUrl ([929a221](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/929a221e836d6de8b594c52d1a2ade1d0648558d))
* fixed request content was not encoded for type x-www-form-urlencoded ([e3078cc](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e3078cc0ef453424e64a20cb96e839e2736169dd))

## [2.30.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.30.0%0D2.29.2) (2021-02-17)


### Features

* **config:** make service url config optional ([c8b1741](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/c8b1741a9df6582d1e1ab4346255f162a4d0fe02))
* **Matchmaking:** implement joinable session ([fc127fc](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/fc127fcead48d9039e8f6e6ba74c8284a029b608)), closes [#258](https://accelbyte.atlassian.net/browse/258)
* **Server:** Add query party by userId in server ([2f290c0](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/2f290c04873b77a6918cf06ce584b5cdd5a3b3ab)), closes [#242](https://accelbyte.atlassian.net/browse/242)
* **user:** add login with other platform ps5 enum ([bcc94c2](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/bcc94c28329ad5cdb528aa7fed6fc53a03878d87))


### Bug Fixes

* undefined ENGINE_MINOR_VERSION preprocessor on AccelByteUserModels.h ([1535744](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/1535744683d1dc0b6898e8c97aa16f5d8ceb2a13))
* **GetDeviceId:** Suppress deprecation warning for GetDeviceId ([e58f645](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e58f645734c9fbd4371bf1251b8fa5adbb8e2f10))

### [2.29.2](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.29.2%0D2.29.1) (2021-02-03)


### Bug Fixes

* **dsm:** fixed parse match request content length checking lead to uninvoked callback ([e4def25](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e4def254497115967253ffab05497dceffa16140))

## [2.29.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.29.0%0D2.28.1) (2021-01-20)


### Features

* **Automation Metric:** SDET-1300 Integrate SDK Test Result to Automation Metrics ([d002eca](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/d002eca6d98f2ddcf260582129db2743445e3f47)), closes [#237](https://accelbyte.atlassian.net/browse/237)
* **Automation Metric:** SDET-1300 Integrate SDK Test Result to Automation Metrics ([9b824e3](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/9b824e37af6589416d1a04a4cae48f615b1c6985))
* **core:** remove AccelByteCustomWebBrowserWidget ([82cbb1a](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/82cbb1ac8ad6ba85a9e1b604e15ec7b080d2a694))
* **core:** support UE 4.26 ([d2d4b22](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/d2d4b2232be2a368ef7abe7321479280dd78493f))


### Bug Fixes

* **config:** add config null check ([d56a651](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/d56a6513d7e8a53fbe387709458a61388d4a5fbb))

## [2.28.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.28.0%0D2.27.0) (2021-01-06)


### Features

* **dsm:** adjust get DSM region url, get region and provider value from run args, select DSM client based on region and provider (backward compatible) ([5cabf1a](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/5cabf1a0ab58d1af53732efd0eea07d7fcc7b45f))

## [2.27.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.27.0%0D2.26.0) (2020-12-10)


### Features

* **lobby:** parse object value, parse quoted array element, only show parse error for known messages ([f72578c](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f72578cb4e53f2cec4261baf4d82e0620cc590dc))


### Bug Fixes

* **lobby:** fixed lobby crash on exit ([b667537](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/b6675370b99c58d28ebd5594db32bbf36c154f3a))

## [2.26.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.26.0%0D2.25.0) (2020-11-25)


### Features

* **lobby:** add party attribute option for the StartMatchmaking request ([7a85dff](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/7a85dfff97ccbf7c2dd817774cfcb15f24581693))


### Bug Fixes

* **partyAttribute:** restrict the matchmaking party attribute to Map<string, string> and affect the sent request + heartbeat parsing at the server ([f0a26a7](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f0a26a761ced9184917e1c5a55d8e5c9fabd24bf))

## [2.25.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.25.0%0D2.24.0) (2020-10-28)


### Features

* **user:** add epicgames and stadia platformType ([533aa8a](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/533aa8a19866a69da00f58f81cedce54bf3470f4))


### Bug Fixes

* crash on RemoveTicker on game telemetry API ([2e0a5fd](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/2e0a5fd09e399c320aed8d61a4368534fdd3154b))
* GetPlatformLinks and UnlinkOtherPlatform namespace ([f0ab180](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f0ab180e3417a7a8ad4e6a1bcb23e0d2801316de))

## [2.24.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.24.0%0D2.23.0) (2020-10-14)


### Features

* **User:** add search filter param in SearchUser ([7565805](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/7565805425472096e94d21894a63c77dd3956fb4))


### Tests

* **Daily Test:** SDET-1166 Run SDK Automation Test Daily and Send report to slack channel ([3f7f34f](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/3f7f34f92ba5012fd310722b89a80e3b61c16a79))

## [2.23.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.23.0%0D2.22.0) (2020-10-01)


### Features

* **lobby:** implement global chat ([3773b11](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/3773b11f5bd5553fcc5b3806946c7a7f51ae88ad))
* **user:** add username parameter for register, upgrade, and search ([54b3ed7](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/54b3ed7a998a9f8723b3441d70feecf08cdf0028))

## [2.22.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.22.0%0D2.21.0) (2020-09-16)


### Features

* **Subscription:** add CheckEligibleUserCheck blueprint node ([fa0182c](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/fa0182cb7687bc58b1759f34308ec743069b811c))


### Bug Fixes

* **Subscription:** updated check flow to latest using check any entitlement ownership API ([7335ef8](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/7335ef813b7a655d9182e154e96833db9760ce18))


### Tests

* **Achievement:** fix achievement invalid ID test to comply to UUIDv4 format ([8a1ff0f](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/8a1ff0fdda5baf1e84679852fd2ef7bf8e3746e7))
* **ecommerce:** fix invalid case because not UUIDv4 format ([7ee52e2](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/7ee52e2ccbeb8f03ac59ddf0ca9f4992f7fd4f4d))
* **ecommerce:** wrong UUIDv4 format ([2217521](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/221752133149bc0c34487ad5abeb754ad3cce73b))
* **Ecommerce:** fix another invalid id error ([a3f7f0f](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/a3f7f0f9f923ce28f85f269ae35e9d2098417409))
* **sub:** changed sub test users ([b6de67e](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/b6de67ecf3c452129fa0251c297cdf3aa97da65e))
* **subscription:** make app id static in sub test ([b9c76d8](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/b9c76d8baa2a1b0805def56bc267b46598a3648a))

## [2.21.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.21.0%0D2.20.0) (2020-09-02)


### Features

* **User:** added `ps4` and xbox `live` enum for 3rd party login ([430b2e0](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/430b2e0635a8ceaf8e31a9bdc1c7b89030e29f24))


### Bug Fixes

* **config:** fix DefaultEngine.ini default server setting value ([9f868f2](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/9f868f2aadd95be027ec7d94c3bfda6d5be16a24))

## [2.20.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.20.0%0D2.19.0) (2020-08-19)


### Features

* **Achievement:** bringback achievement from VE ([1f3e6be](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/1f3e6be1b85b8a194d32fa28d9f56a987d19cff0))


### Bug Fixes

* **test:** additional incremental Achievement test ([71fba3a](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/71fba3a48e3c447992659c49230e24faf43ad147))
* fix ue 4.25 build ([9e2b24c](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/9e2b24c79913fe49335b5a23b8b299f6394ac277))


### Tests

* **gameTelemetry:** fix game telemetry test ([240df45](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/240df45e9699a35095abeb1c2eeeb8557daa0429))


### CI

* **jenkins:** add target parameters ([a9d6d4d](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/a9d6d4d0fede4e120a106e8ceb9908d2d32b2a6b))

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
