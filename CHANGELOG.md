# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

## [16.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/16.0.0%0D15.1.2) (2022-08-15)


### ⚠ BREAKING CHANGES

* group API models change affect function signature

### Features

* adjust code to following new method ([a4cdf85](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a4cdf8545f9664631c1e95b54a79acd81a669871))
* adjust the code and bring back the code that made some test error ([b71ca6f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b71ca6fd6aa0834170cbb3d8dc27582b1edf2cbe))
* **blueprints:** add utilities class ([2104ba6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2104ba611be1a08be68c26b32896fe78280be4d6))
* **blueprints:** add utilities to get and modify fjsonobjectwrapper ([24ff88e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/24ff88e4590ec9605bd937b14ada944809e02d75))
* changed a value on struct related statistic end point to floating number type ([0497802](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/04978025862f9eae474726eb082047e41bca9091))
* expose statistic v2 endpoints ([0a70ae7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0a70ae7dd27a87904ee54f2d4f6dcf7d16a8f269))
* expose store Id and list public store ([5e18dce](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5e18dce082e0317f3d424a6934e52d35e9a0631f))
* fixed typo and comments, wrong param and some struct ([c897cc1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c897cc1ddfc5897dc881d0516e61dd43604e1bb0))
* **iam:** EA Origin Authentication Integration ([63e92f6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/63e92f6a06da7e412b56c384aacd588763ccb501))
* **item:** get Item Dynamic Data ([ae7ad68](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ae7ad6855c221c1e0e801e01030d0d44f1d3c9dd))
* New item type for get items like method, option box config. New API call method, get entitlement ownership by item ids. OPTIONBOX ItemType. ([42873cc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/42873ccfb0f0633bb8b9c7b99ad07c280fbb015b))
* **party:** add party connect, and disconnect notification ([a6200a2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a6200a200d999ee1ad113147269e16a81c7d3e2b))
* Purchasing Requirements ([649841b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/649841b955028592a4ffa71002fbc3b70fb94530))
* **qos:** get ping timeout from GConfig ([fc8c79f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fc8c79f79a6c5769d612af9286e7c641751388b5))
* **qos:** reduce udpecho timeout from 10s to 0.6s ([d549bfa](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d549bfad3e3c5bbac8aacb30622668ba4234b5b4))
* SDK Implementation for UGC service - "like' a content ([8b5c6c1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8b5c6c100933472e9cde20fe8ed284a74bf9552b))
* **sessionbrowser:** add get game sessions by user ids ([6a21472](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6a2147283a97e206dd97474d6896695e2f130f38))
* support searching entitlement by feature ([d66fc9e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d66fc9ec6d333403a2e3d65ff86a53d94865f0dc))


### Bug Fixes

* Group blueprint function, JSON request creation, Group Member Roles model ([5aea352](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5aea3523de59a4f4c3245bd7825af45e16c9b2a3))
* **party:** change all all function, delegate or variable by using PartyMember. Also adding some description on the function. ([856337b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/856337bc3e2fdf50d5afd7ec81ed7481ad1b7b33))
* **party:** fix typo on partyMemberLeaveNotice and FPartyMemberDisconnectNotif ([01c3a2b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/01c3a2b808a4bc4ddde5d461ab3db0eefbf6b1c1))
* **party:** Set Deprecation Variable, Function, and Delegate for PartyLeaveNotif. ([72283eb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/72283eb57dac1ac7c0904080689f6953bfbe436a))
* SortBy in GetItemByCriteria and UrlEncoded in Query Param Array ([3a15458](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3a1545846e21f7badd09434a024681cb38116021))
* **statistic:** wrong query params in GetUserStatItems ([b5a1395](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b5a139579e9e6a0abacda2f16b5f095539676758))

### [15.1.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/15.1.2%0D15.1.1) (2022-08-05)


### Bug Fixes

* **lobby:** unbind delegates on Lobby destructor ([daa6af1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/daa6af13907a89a844cd9b4838a27ac02694a453))
* **party:** Party leave notif is not triggered ([5e8458f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5e8458f4070ba19cbe6cbeba9511ba4757f051d1))

### [15.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/15.1.1%0D15.1.0) (2022-08-04)

## [15.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/15.1.0%0D15.0.0) (2022-08-01)


### Features

* **party:** add party connect, and disconnect notification ([bee9d18](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bee9d181a3d9f6ecf7628c8e4a47399382fd0510))


### Bug Fixes

* **party:** change all all function, delegate or variable by using PartyMember. Also adding some description on the function. ([6189926](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6189926b8e4a767fa3adf61e1e043bade49df630))
* **party:** fix typo on partyMemberLeaveNotice and FPartyMemberDisconnectNotif ([b28a515](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b28a515b78434d76b038475056fc4fabe8cac801))
* **party:** Set Deprecation Variable, Function, and Delegate for PartyLeaveNotif. ([ab4d428](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ab4d42860e26b8c26ffbef79a73a6a0fdd8f87ba))

## [15.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/15.0.0%0D14.0.0) (2022-07-18)


### ⚠ BREAKING CHANGES

* **reporting:** Changed AdditionalInfo datatype from dictionary to a struct / JsonObject later

### Features

* add setby from server response function GetGameRecord ([280a5d0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/280a5d0d16e17c37582e4564459456f8b7b376b9))
* Adding SortBy parameter of AccelByteModelsItemCriteria to call GetItemsByCriteria blueprint function ([5c51226](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5c512263c27bb125fecdc4d19832a7979adc1243))
* **basic:** added category (optional) param in upload generate URL for user content ([9dcb1af](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9dcb1af412310a303efbab0b1d1b0b540d40d689))
* **basic:** Expose generate upload URL ([2ab1517](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2ab15174509b88331907f586e014688cfb302b87))
* **ecommerce:** Support to sync Durable items from Epic Game Store ([d2aae0b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d2aae0bb3bee7117b1f633fce2cf2d877fa7b11e))
* Implement query game record method for ServerCloudSave ([2e9441a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2e9441aa4e19060113556f474513ca7c0f5e6606))
* **item:** get Item Dynamic Data ([8343831](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8343831c6012f192c65c21fc1b8d1f57497776a9))
* Purchasing Requirements ([c331c94](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c331c944c409350d087c5df18591a4b23bcd0aac))
* **reporting:** Reporting Signature Adjustment ([5e00a2a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5e00a2a9f1725d23297e3b8c4fb5ada6e90287d9))
* sdk adjustment for avoiding breaking changes and revisioning/reverting of phase 2 cleanup legacy Avatar/User Profile implementation ([27a16c6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/27a16c62d7ecb071f1330be100540e94c709d213))
* Set the currencyType for Currency.GetCurrencyList ([30067e7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/30067e7f6fcda6a135cc33b38735376512596820))
* **User:** add country and DoB on FAccountUserData ([96acefc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/96acefcfb8672b5196ec84d3b32627e203a88a9d))


### Bug Fixes

* add UMETA to the Environment enum class to fix compile errors ([7c64000](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7c64000b059617e54d2f6b3c7420c73e3d476125))
* **blueprints:** add missing category for any class exposed to Blueprints which use UPROPERTY ([c9e477b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c9e477b489813e93c81a5cbacfd6960defc5ddf7))
* **blueprints:** wrong function name for Fulfillment to redeem code ([8a59361](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8a59361493a8cc188b00a155ebdb6b504a1dd11c))
* **item:** wrong query parameters creation on GetItemsByCriteria ([fb4c584](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fb4c58408fca6aaf3535c68696318bbf5efac859))

## [14.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/14.0.0%0D13.0.0) (2022-07-04)


### ⚠ BREAKING CHANGES

* **Lobby:** - Remove EAccelByteGeneralUserStatus
- Renamed Availability to EAvailability
- Renamed Availability::Availabe to EAvailability::Online
* **iam:** Refactor/removal AvatarURL/User Profile related Implementation

### Features

* **auth:** Add SnapChatAuth Support ([929fe07](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/929fe07c4eb725494eb9441b527be760571ad09b))
* **iam:** Login with OIDC in SDK ([5f576e4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5f576e430eb7a4569263d64a8b74b3940d1a8901))
* **iam:** Refactor Avatar/User Profile Implementation ([0b494f6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0b494f6897bb3adb367e81ce79f6b76c441e8b32))
* **iam:** SDK Changes for BE Refactor/removal AvatarURL/User Profile related Implementation ([102dc9c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/102dc9caa52944bd3401a2b468d9d7f4d57a3801))
* **lobby:** add Leader field in lobby models ([fdfd5b5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fdfd5b508c7011149e7afafc627963655c4c0b72))
* **repoting:** add new enum of USER for reporting player or user ([09e9e4e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/09e9e4efe1a8e5fd6256ad6fd9c299407e5f9e6e))
* sdk adjustment for avoiding breaking changes and revisioning/reverting of phase 2 cleanup legacy Avatar/User Profile implementation ([b886deb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b886debd51d16e064db3a868a65204b0975d796a))
* **Tests:** remove tests from module directory ([2129e63](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2129e63efca57f9d0f7d5de3ec90700fd89f5023))
* **user:** fix query search users encode twice ([db2ea07](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/db2ea071e298d5fa99ad19326c24a762cdc1a2d9))


### Bug Fixes

* create ticker alias to support both UE4 FTicker and UE5 FTSTicker ([bf91b2f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bf91b2fa32fd70689229e9f65d62ec057c3a10ef))
* wrong implementation causing errors ([b0ad23e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b0ad23efe21f07c3ef8987dde94fba7701822d39))


### Tests

* add default value to test models and remove unused variables ([bc87073](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bc870737ea17a47a1dd3e4aa1fd3fb031d639b49))


### Refactors

* **Lobby:** Change availability type in presence model ([f246a35](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f246a35599830b184a94371c75d5c0980f6526fd))

## [13.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/13.0.0%0D12.2.1) (2022-06-20)


### ⚠ BREAKING CHANGES

* new param Source and tags in GrantExpToUser method

### Features

* **credentials:** add function to get IsComply ([b84625e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b84625e79e6cad8b517385eefdc3d9bb1ebf6af9))
* **precompileHeader:** Resolve compile when set BuildConfiguration UsePchFiles as false ([025fa48](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/025fa486b77e1d933b889c3b504ee3c58f684fc0))
* **seasonpass:** make new params (source & tags) as optional. ([7f46ba7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7f46ba7ecb1f025f27e28341c3e6fbe09a4a4a50))
* **user:** add login with refresh token param ([dffdaff](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/dffdaff6bf405b47d4474809355e37f0634c469d))
* **user:** fix query search users encode twice ([8c53ae1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8c53ae1e92a29e01a02044a0b8c049c95215542d))
* **user:** fix query search users encode twice ([1411838](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/14118389d1f5f5fe5600830780ed6f74d6da71be))


### Bug Fixes

* create ticker alias to support both UE4 FTicker and UE5 FTSTicker ([e4dca48](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e4dca4802d0055f7dc348270b4f186f5b7dae35f))
* wrong implementation causing errors ([a3242e5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a3242e56da934fe58e458308f1a6cedf879b24c3))


### seasonpass

* Granted Season Pass EXP Tracking ([4afa10c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4afa10c70e9c7b23809c84ab2a14ad1b998a2f78))


### Tests

* **reporting:** add missing object type when submitting report ([41295ec](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/41295ecc8d50246d6b227129863a135bacdacb99))

### [12.2.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/12.2.1%0D12.2.0) (2022-06-14)


### Bug Fixes

* OAuth client credentials not set when ApiClient is created without the MultiRegistry ([c727808](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c727808dd097f529fc336226c80b81bb20fafc7e))

## [12.2.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/12.2.0%0D12.1.2) (2022-06-06)


### Features

* **iam:** Account linking handling ([3bd4bb7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3bd4bb78bfbd794413e7d5710db5606f50b2a7ef))
* **iam:** Include platformUserIds field when bulk gets users' basic info ([9bcf223](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9bcf22340432a9c88b06a248c540b735cc518927))
* **iam:** Need an adjustment to the SDK for the updated Public Search User V3 API, adding offset and limit. ([7609c93](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7609c93c04597faca6b19e7b26b6b6db9ba275f0))
* **lobby:** implement add friend with public id ([9dc358e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9dc358e0e53154285079ebf2ba0a2b9aab77b77b))
* **lobby:** implement set party size limit ([80fa3d0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/80fa3d04d2ccc19f1e96ef433ccf3011a52a5302))


### Bug Fixes

* **blueprint:** missing Group blueprint object initialization in the ApiClient Blueprint ([a71e5ad](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a71e5adc3f88ddc398a7d329ba8c2de0767680fa))
* **blueprints:** resolve conflict for hotfix 12.1.1 ([8462e83](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8462e8328f5cde82cfa694d02ef3bac97a8214fa))
* Fix ambiguous function overloads ([c988024](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c98802461cd005779d22b3450020eaf97a28873a))
* remove pragma once from cpp files ([20ead97](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/20ead97b32b434017a7f6f2b8a99e15cb4c6b41d))
* **test:** add wait 60s for lobby to refresh mm game mode cache ([b504617](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b5046170711241be6ec3632c3e9468832c8ff9d6))
* **websocket:** properly clean up websocket object on destroy ([ba56887](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ba568873f6b2827dab83ac0eae93ae71172e855b))


### Tests

* **lobby:** commented lobby channel chat tests ([b40d5e4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b40d5e4097036c92854f7d24fdd865bb75608d11))
* **reporting:** make exception if objectId was empty and use dummy objectId following the UUID v4 without hyphen format ([a70a3f4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a70a3f48453023121b93df280539741b8a990239))

### [12.1.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/12.1.2%0D12.1.1) (2022-05-31)


### Features

* **DSM:** Add register game session in DSM ([ef0d18e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ef0d18edc61a0c6a9f0ff06312dea4a6a51818b4))
* **entitlement:** add SyncMobilePlatformPurchaseGooglePlay blueprint ([90545bb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/90545bb50e99af86861df968f702e209aa563645))


### Bug Fixes

* change the source of truth for OAuth client Id and Secret to Credentials class ([76da937](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/76da93736ab2f1d3efc1e14f75cd38c3eddc5224))
* compile errors when targeting Linux platform due to incorrect code implementation ([4bd2dd5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4bd2dd5a8fb799a98c412bfcf3f79e3128fa97f9))


### Tests

* update test cases to handle the changes on source of truth for OAuth client credentials ([0334121](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0334121698468d57e21ebcdcd8ac65ab1438393f))


### Refactors

* rename parameters in the constructor to use different name with the class fields ([e1be08a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e1be08ab210ad5ba54d5b4d64154ca432af65f32))

### [12.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/12.1.1%0D12.1.0) (2022-05-23)


### Bug Fixes

* **blueprints:** remove const from functions, change blueprintreadonly to blueprintreadwrite ([d6e85b4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d6e85b4e483c458d11a4694fb407aae8d48de0a9))

## [12.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/12.1.0%0D12.0.0) (2022-05-23)


### Features

* **iam:** Expose the Admin Ban User endpoint ([1dd1db2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1dd1db2cda2095be7b3f82b014a82a696a9c2444))


### Bug Fixes

* **lobby:** fix sdk still retry update party storage after retry limit is reached. ([df9d2c4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/df9d2c4ccb7a762abb12fdc5e2df179357e41e0a))
* **test:** add wait 60s for lobby to refresh mm game mode cache ([9920a6b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9920a6be70cda355e0fec166fd660078fc409e7a))
* **websocket:** properly clean up websocket object on destroy ([82d6f54](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/82d6f54ee46b0bac3a56f9c7c702afbf5539866e))


### Tests

* **lobby:** commented lobby channel chat tests ([8617606](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/861760663069db715bddc107fde5d80af7f4f094))

## [12.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/12.0.0%0D11.0.0) (2022-05-09)


### ⚠ BREAKING CHANGES

* **ecommerce:** To get total  balance could not be retrieved from credit wallet response (call via admin end point call) anymore because it’s be a current wallet balance. User should call again get use wallet balance public interface to get total balance.
* Game and user record struct utiization

### Features

* **blueprints:** add Group API blueprint to the API Client blueprint ([93880d4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/93880d4859fe11b6aa5309bbafe8aca839e241c9))
* **Credentials:** add method to set clientid and secret directly from gconfig ([6e52c35](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6e52c350c2faecf75a96f3ca5328ac56022f296b))
* **dsm:** Add custom attribute parameter in register DS and in DSNotice model. ([25a8a49](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/25a8a49e534c44d25fa13be6f140077ab81b46cd))
* **ecommerce:** add BulkGetItemsBySkus API ([b95155f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b95155f301ae1002c819076ffa9aa20da2501b7b))
* **ecommerce:** add initializer on the model, add integration test when sku is invalid ([86dfeb0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/86dfeb023a82ff98bb261b7b1319787620867068))
* **ecommerce:** Update Unit Test Code for New Behavior Cross Platform Wallet ([2b7b76f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2b7b76fa5aca955b17fa64f0bf16a4ce4786d391))
* **lobby:** enable trigger error/response callback when failed to deserialize lobby responses. ([e322aaf](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e322aaf294133f93ef188114ee7e97f92a901a31))
* **lobby:** Skip fetching lobby error messages in shipping build ([d37441d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d37441d1b151471d65a0b20b947085ad1bc99b4b))
* **models:** adding ItemSku & Currency into FAccelByteModelsSeasonPassReward ([f0806db](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f0806dbe70907304fa378f6b1ffae2b96de7a5fc))
* **serverecommerse:** fixed params query user entitlement in unit test to make it passes ([56c985a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/56c985a1b6af415aab891bf828ff2e00badb9a58))


### Bug Fixes

* change server refresh token to not pointed to FRegistry ([9d5c63a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9d5c63a19d1398a1ad62ad1ccb835f547eda18d2))
* **models:** add missing header file ([1d63123](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1d63123db045a6a4a0a8408e4379c8fbf8531039))


### Tests

* **lobby:** commented lobby multithread test ([abff2fb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/abff2fb31bc20388189a2bcb5957fe7cea4279f9))
* **lobby:** re-enable channel chat test ([6f76c9b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6f76c9b39c7a7393b3bf94dec0852c8823fdfe64))


* Refactor Redundancy of Game Record and User Record Cloud Save on Api and Blueprint class ([6fb817a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6fb817a4e646108ce03b57fe6ab8cfa79978ff96))

## [11.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/11.0.0%0D10.0.1) (2022-04-25)


### ⚠ BREAKING CHANGES

* **server lobby:** renamed leaderId field in FAccelByteModelsDataPartyResponse to leader
* **serverecommerse:** make breaking changes when using user record or game record on BP
* make breaking changes when using user record or game record on BP
* make breaking changes when using user record or game record on BP
* **lobby:** data model signature is changed
* Renamed User/Game Record Enum
* Renamed User/Game Record Enum
* Renamed User/Game Record Enum
* Enum User/Game Record renamed
* **cloudsave:** save, delete, update game record using admin endpoint; parameter order in cloudsave

feat(cloudsave): Change field Metadata in RecordRequest Input and game record behaviour
* **cloudsave:** save, delete, update game record using admin endpoint; parameter order in cloudsave
* **cloudsave:** save, delete, update game record using admin endpoint; parameter order in cloudsave
* **cloudsave:** save, delete, update game record using admin endpoint; parameter order in cloudsave

### Features

* **AccelByteUe4SdkModule:** add environment changed delegate ([146662a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/146662ab9f4386bf6cedc4618537188e995aa459))
* **cloudsave:** Change field Metadata in RecordRequest Input and game record behaviour ([aa6824b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/aa6824b76c35845142a438c9b3f0787f788b3378))
* **cloudsave:** Change field Metadata in RecordRequest Input and game record behaviour ([1ca34a5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1ca34a5f9faf90b709c185d681ca913cd674e0d7))
* **cloudsave:** Change field Metadata in RecordRequest Input and game record behaviour ([37d9f49](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/37d9f492642da62687f643050011474d8a43027c))
* **cloudsave:** updated signature couldsave record in BP ([584372d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/584372d4e0152f8aae82d017be09c4803a84487e))
* **credentials:** add method to check if session is valid ([1a48fa4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1a48fa463ce7fb389c034fad985f499ca3c3d617))
* **lobby:** add websocket reconnect to lobby if receive close code 4000 ([b15604c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b15604cfc8c64812a1e7a97d93052c35255b6c91))
* **MultiRegistry:** Change RemoveApi to bool. NOT BREAKING CHANGE: new API ([9d078aa](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9d078aa2c9db6e91dca62178d0268bebbd043d7a))
* **serverecommerse:** fixed params query user entitlement in unit test to make it passes ([6fccdc8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6fccdc8c349338614e025e834e3e89fdd4a9d2e3))
* **serverecommerse:** fixed Proper Query Params for ServerEcommerce::QueryUserEntitlements Url ([d2d8954](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d2d8954cedfbbd97d9ac815fcfd40c760b34b067))
* **ServerSettings:** multiple environment support on server settings ([ced158b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ced158bfea18d3f4d00e2b19130e6b500d1d428e))
* **Settings:** change way to retrieve settings, from GetDefaut to GConfig ([c621a5e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c621a5e9e9ecd1131a57433ab94eb01c46c4400d))
* **Wallet:** add ListWalletTransactionsByCurrencyCode ([d22c755](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d22c755cbee2a72857794bde5f65af3c00d49e60))


### Bug Fixes

* **lobby:** Fix typo on model name ([d37845a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d37845a15a54c414ba0fa3c32a07a1db6ef22111))
* **lobby:** fixed empty session upgrade headers on reconnect ([90370b6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/90370b6844446851804385282d2e8f78ca30c248))
* **lobby:** remove refresh token delegate when conenction closed. ([406e5a0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/406e5a0c65f80642574d529f04957d4f2a62cf07))
* **multiregistry:** Implement input from Ben (SIMS). NOT BREAKING CHANGE: no implementation breaking api ([20cc71d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/20cc71d6ffd150ef762ed0bbb9b7742cc65500f4))
* **qos:** calculate latencies data even when the scheduler is activated ([8b4d908](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8b4d9085211fde32d5c838c758e45964b2e9ac4d))
* **qos:** wrong ping regions on success mechanism ([3938ea4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3938ea44f864f7081e07baa89d94a794193003c2))
* **server lobby:** fix wrong field name in FAccelByteModelsDataPartyResponse ([da29c50](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/da29c501ab4c885825eccedc5d5fe61dae92da43))
* **setting:** remove set min latency polling in setting. ([c02cc6f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c02cc6feedf01dc60e1e993acdba4f7a2eb2d23a))
* **user:** change logout to revoke user token instead ([cd266ed](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/cd266edcc00057145c9c7651c67e6b61e71e627d))


* Merge branch 'master' into feature/OS-6137-Signature-Record-CloudSave-on-BP ([05c9e15](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/05c9e15ed614f62278ea5a528734a949ae5cb018))
* Merge branch 'master' into feature/OS-6137-Signature-Record-CloudSave-on-BP ([9a565fb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9a565fba466febb3e18718606cb220d47927f8b5))
* Merge branch 'feature/OS-6137-Metadata-Field-and-Game-Record' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-6137-Metadata-Field-and-Game-Record ([9cf7fff](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9cf7fff7f03a95f364b9bc6519f583531ecf3888))
* Merge branch 'master' into feature/OS-6137-Metadata-Field-and-Game-Record ([bf76d82](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bf76d82d5b1ab0037436916d167e950d5e81dd80))
* Merge branch 'feature/OS-6137-Metadata-Field-and-Game-Record' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-6137-Metadata-Field-and-Game-Record ([e1f36a7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e1f36a7e91fbee88be28d425631fabc64b9d77d3))
* Merge branch 'feature/OS-6137-Metadata-Field-and-Game-Record' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-6137-Metadata-Field-and-Game-Record ([1caaa2d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1caaa2d7db206fbbaa888d2fbb4774104988ae33))


### Tests

* disabled test because of major changes in the lobby backend ([9560421](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/95604213aa89083f75b8f132c8279678c8e5e447))
* **qos:** Commented GetActiveLatencies test ([39e7954](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/39e79548a7e4509f09e2d859b001b3e0b8eb366a))

### [10.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/10.0.1%0D10.0.0) (2022-04-20)

## [10.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/10.0.0%0D9.1.2) (2022-04-11)


### ⚠ BREAKING CHANGES

* **oauth:** Get DeviceId from FPlatformMisc, FCustomErrorHandler

### Features

* **dbAdapter:** add db adapter interface and sqlite adapter implementation ([8b7bd35](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8b7bd354859d28957d80780077f0f866dd25f572))
* IAM Input validation ([beb5947](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/beb5947af586df053549afcdb5dc5c5498052bdd))
* **lua:** lobby missing blueprint API added along with the functional test ([a1cbcf8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a1cbcf8345f89c88a9f27fb91a0053ba5b4e793a))
* **oauth:** bring back old login flow ([c90d0aa](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c90d0aa436c4f0f649a9d59b6774b48e439102b4))
* **oauth:** handle verify 2FA code when user enable 2FA from player portal ([a017f07](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a017f07be0d472236c2e7329e55a4fd94b25140b))
* **plugin:** change cisqlite3 plugin to false as default ([ae02242](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ae02242efa1ebaf169aeb710793d224d70f9734d))
* **test:** create agreement admin blueprint ([8631ff4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8631ff47a34adb0479f0b655af3d9fa3130cb7ad))


### Bug Fixes

* disabling getItembyAppId test ([73e3f9d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/73e3f9df83b6091201dc47d100ac4f47ef038965))
* **lobby:** Fix lobby reconnecting on multiple user connected. ([e0fbfaf](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e0fbfafdb916ebf38821ba13334743b97e3368a4))
* missing header #include "JsonObjectWrapper.h" ([1ffa843](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1ffa843803e4122dceef2afda6c55be5d5d95784))
* **TokenGenerator:** fix header dir ([21e4497](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/21e44978ff9be00ca04ee759bb66aa5d5016b286))


### Refactors

* **dbadapter:** Refactor DB adapter and SQLite3 adapter ([53aadae](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/53aadae4f76b03d245d03560f755e958bedf44fb))


### Tests

* **lobby:** fix concurrent test to be more reliable when failing. ([4d14103](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4d14103d0e316afa606e60eed98d347922fbacd5))
* **qos:** Commented GetActiveLatencies test ([a2ced73](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a2ced7368a39a0abf8d398433fc66d56b84376ae))

### [9.1.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/9.1.2%0D9.1.1) (2022-04-07)


### Features

* **qos:** Add get active qos server and latencies ([1901e6f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1901e6fa3acc17dfaf2b5e1b355c42dbcec43db8))

### [9.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/9.1.1%0D9.1.0) (2022-04-06)


### Bug Fixes

* **blueprint:** update GetApiClient method to be static so it can be used in the Blueprint ([923bf82](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/923bf82f9c12641d3bfcf94932aa310f92d60caa))
* disabling getItembyAppId test ([a254c74](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a254c74d3c45e5518411625531281a5937cae5ac))
* incorrect header path on some files ([4f329c4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4f329c4e0fd53ff1ef5750f1fb43a913dfff6d5b))

## [9.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/9.1.0%0D9.0.2) (2022-03-28)


### Features

* **entitlement:** add get entitlement ownership token & token only function ([b3990ed](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b3990ed478dab7e984a01a4eb2344b822e4c0fb6))
* **httpretry:** add functionality to specify custom handlers when receiving certain response code ([b8fcfc8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b8fcfc8cc46730ba28b742e56947f44ed185010c))
* **JwtWrapper:** Create JWT wrapper class ([e87eca9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e87eca95e0372cee7e0c3e343af614da327c09ae))
* **lobby:** add lobby connect using Token Generator ([5d301a6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5d301a6d5a7022ae7a4746a6c6b051c1b1fd289d))
* server validation for cloud save ([29e798c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/29e798c4823720a2921472fb99cbc42d45895fec))
* **serverApiClient:** Add shared server credential ([c349aea](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c349aea4df0ccfcc85206876a3d4c5cc539ab627))
* **serverApiClient:** Add shared shared server credential ([4e76eff](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4e76eff3261e70d26b7231393b5cb00d361787c8))
* **serverApiClient:** Add shared shared server credential. Resolve inline copyright year ([d2b29a7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d2b29a7434c0366b749a59d67a61c0a65fd74984))
* **serverApiClient:** Rebase to master branch ([92ef43d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/92ef43d40065c580fd91d526b760287858a537ee))
* **TokenGenerator:** add EntitlementTokenGenerator ([75a5df7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/75a5df7e3309dce951b30efc80df809d97c0fe4e))


### Bug Fixes

* add Purchasable field on ItemInfo ([e9dea52](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e9dea52356465f8ffedc99ec5e81e98ff8390cf6))
* **Error:** provide correct ErrorCodes to handle user relate Error ([5e14a18](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5e14a18cd8e06a360b931ad798305bb00fac0608))
* missing EmailAddress field on FAccountUserData model ([dde1699](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/dde16994ed771f3ad8dda19aebb86eff448c777f))


### Tests

* **httpretry:** update tests implementation to use automation spec ([685a68e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/685a68ece55f988f6b05eab9506d80608349e6b6))

### [9.0.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/9.0.2%0D9.0.1) (2022-03-23)


### Bug Fixes

* missing header file in Commerce models ([78de603](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/78de603f19023308c8f82849ae1c50c2ec19bd0c))

### [9.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/9.0.1%0D9.0.0) (2022-03-22)


### Bug Fixes

* **commerce:** change Ext and LocalExt data types to FJsonObjectWrapper ([889da9b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/889da9be891e2033831f143439ac79aa8106e415))

## [9.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/9.0.0%0D8.6.0) (2022-03-14)


### ⚠ BREAKING CHANGES

* **user:** change EVerificationContext::UpgradeHeadlessAccount to
EVerificationContext::upgradeHeadlessAccount

### Features

* **cloudsave:** Server Validation for cloud save ([067d597](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/067d597ce0f2c56d5f1468b30681ead3199dd22e))
* **HttpCache:** add mechanism to cache http request and response ([e6ac073](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e6ac073a18b46fb95342123a1d5be1f1f6bfba25))
* **lua:** add lobby Presence blueprint API for lua test purpose ([2a3897c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2a3897c7b8a17979d73b17123da79301e673ab8f))
* **qos:** Implement Qos Scheduler & Init Trigger ([5a4ab6b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5a4ab6b51fa2a5daa0262aa33797cd17cd2f96e9))
* **telemetry:** add EventTimestamp automatically inside Send function. ([a3f2296](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a3f22964ce4f272e814187e45b74446df418a5bb))
* **user:** upgrade and verify headless account ([8cfbdb4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8cfbdb4b08b269fd1f0e838f117baefe84a0c206))


### Tests

* **lobby:** add integration test case for lobby usage in multithread scenario ([6f68d68](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6f68d683f221ce3234249413704fce0ad5035c03))
* **matchmaking:** refactored create matchmaking channel to setup step. ([86d4408](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/86d4408f802ff2afdd354b64d4848e2695a221c2))

## [8.6.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/8.6.0%0D8.5.0) (2022-03-01)


### Features

* **item:** get item by sku ([e14297b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e14297b6639c0f01272896cd4212a2ba81c88480))
* **sessionbrowser:** adding get session by session id on server API. ([9b21f6d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9b21f6d4ddf7d41c53af6eb33d7e5b9f6eea3d15))
* sync twitch drop entitlement api function and model ([f83acb5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f83acb5795cd71878b7df56986dc782558af0704))
* **user:** Login using password grant V3 ([4b3361d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4b3361d0e5d864583c64b165188cce5965ca5678))


### Bug Fixes

* **lobby:** fix order of Lobby::RequestDS parameter to be the same as in header ([ec35503](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ec35503564a6fb3af86c20d0cd24f78a5742cd6b))
* **oauth:** deprecate RevokeToken function and create new RevokeUserToken ([100005e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/100005e1314844de4383443d01321b145d04f85b))

## [8.5.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/8.5.0%0D8.4.0) (2022-02-14)


### Features

* **entitlement:** add new item sync with google play item function ([9bb71b7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9bb71b76c89b4f5824bbe792f60634a476526918))
* **group:** BP Group Member (individuals) ([db0e6f0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/db0e6f0743ee17456ff5c26fc5f3cc631e839e3a))
* **group:** BP group member (individuals), WIP ([87637cf](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/87637cfd81ad0d8299561ec13ce9c6ead69cb0db))
* **group:** BP Group Roles (permissions) ([e654ea2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e654ea2fb82303e12033abf6de959bd22dfe9e9b))
* **group:** BP multi member actions ([3a04c53](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3a04c538a02db1cd710c3a0c95809a146ece48d1))
* **group:** Implement group mgmt ([6cf1d7f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6cf1d7f6d771b15675c645d432bd4703f87dbab2))
* **group:** Vanilla BP, WIP ([4021f03](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4021f031641e6491af57c5e110991d03eb2a9669))
* **httpclient:** adding cancellation token mechanism ([072b0b6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/072b0b640e6166f2fd12a2b64e70e448517bda64))
* **lobby:** Add blocked & unblocked user ID on respective response ([f3d3357](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f3d33574080f74ae369512f4f7712c2ecfe04129))
* **lobby:** add more fields to MatchmakingNotif ([348689f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/348689f00cc0e680ac299a638202bea2554c7011))
* **lobby:** Add support for parsing array of json object field type in lobby ws parser. ([8508ce8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8508ce8a50c15cfa9b85d54c89aa1ddfb04a2fad))
* **lobby:** Add User Id in Invite party and kick response models ([d75e2e8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d75e2e8311e5521cca8848def2d0f03e4e1dec63))
* UE4 Group Mgmt (WIP); Unity parity ([7d0ac48](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7d0ac48b054a089294f4d1af7f51ec0b5cbd8a09))

## [8.4.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/8.4.0%0D8.3.0) (2022-01-31)


### Features

* **lobby:** Add blocked & unblocked user ID on respective response ([1caf5f7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1caf5f7ffb300f22efa40920425b57df7b411a1e))
* **lobby:** add more fields to MatchmakingNotif ([48e76be](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/48e76be199258f72af5316f5e4bcc00f22a6740f))
* **lobby:** Add support for parsing array of json object field type in lobby ws parser. ([8d17d2c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8d17d2c76d950530616ae61d37519b1730daa434))
* **lobby:** Add User Id in Invite party and kick response models ([e32b921](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e32b9210c49db9044cbc745875a95da2058f5dc5))
* **lua:** Add Server Statistic blueprint API ([6997160](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/69971602bbb1772264c75baaa4e2bead19ed6324))
* **lua:** server Achievement blueprint API ([0ee1e7d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0ee1e7d7b30904d04e420f26741841240ce3366f))
* **ServerSessionBrowser:** add Blueprint class from ServerSessionBrowser API ([86e7447](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/86e7447183b2a342bbbb18ec67a75b24c4ffcc8b))
* **UGC:** add UGC API Blueprint from UGC API ([a3f6ad3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a3f6ad3bf0e234a1017d4272bf1935ccb072805c))


### Bug Fixes

* **ABServerSessionBrowser:** fix build error due to delegates name already used in other class ([72ffc39](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/72ffc39b5b20320795e46cd8855e3c88089047ac))
* failed compile on PS5 ([6d29bc3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6d29bc3cf842bdf977083ebe2c6bd52758111d9f))
* missing headers that was not detected when unity build is enabled ([38d3bf4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/38d3bf4a89eab79b2693d918ffabda1b6ef00b2d))
* **missing include:** add missing include on test classes to avoid compile error ([f0e1bb4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f0e1bb4beda6dc9a192ffa43e28eb5387d70c618))
* remove unnecessary code that causes compile error ([5cbbc08](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5cbbc080e096866846bdeca95676fd92888d460a))


### Refactors

* **AccelByteBPApiClient.cpp:** add missing line break before the end of a function ([3f99a92](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3f99a92e0e119edb3746e589a07a07de38b43cef))
* **CloudSaveTest.cpp:** remove duplicate "include" ([c7cb62a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c7cb62aca9dc90c9ec63d15d293d14a5d0743d7e))


### Tests

* **leaderboard:** add delay to make sure leaderboard is active when stat code item is updated ([322916f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/322916fd9fa2674414fc1d2f7e9ee6d55a655f53))
* **seasonpass:** change season type setup to be durable according to BE validation changes. ([303acd6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/303acd69842c78b25a937f0bfe37203aebfe77b0))

## [8.3.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/8.3.0%0D8.2.0) (2022-01-17)


### Features

* **Achievement:** add Achievement API Blueprint from Achievement API ([dbce33b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/dbce33b802b85ced729e298801ab804c59a5c0f3))
* **entitlement:** bringback Sync DLC item ([78aa2fa](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/78aa2fa3a6acbed3f0170a869e2708042e30c1e1))
* **Leaderboard:** add Leaderboard API Blueprint from Leaderboard API ([3d87793](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3d87793d1a10427a9dcf4703cbabb397cb7d4958))
* **Leaderboard:** add Leaderboard API Blueprint from Leaderboard API ([ac4e3f4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ac4e3f4e2714fea3e8aa47880a64a73cd44773b0))
* **SessionBrowser:** add SessionBrowser blueprint from SessionBrowser api ([c87a141](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c87a1412730da61ec3305999dd75fa94768eb126))
* **Statistic:** add Statistic blueprint from Statistic api ([e7e6945](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e7e6945676dcab0543532181e054fbc239d66285))


### Bug Fixes

* **lobby:** Remove redundant reconnect call on lobby OnClosed function. ([76119f8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/76119f8597588ab5e8d0f5e4657bd1fb40000ccb))


### Tests

* **joinable test:** add more delay for waiting for dsNotif ([df9080a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/df9080acbeb5c7503650970c34dc2ca22d65f2d5))
* **lobby:** fix cleanup and makes some tests more isolated. ([f9a0800](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f9a0800118054e1a4e16040055e49c34f6af6895))


### Refactors

* **AccelByteBPApiClient.cpp:** add missing line break to a function to match the format of the rest ([dff76ae](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/dff76ae14188fbb812dbbead17805bc1403e7b9c))
* **SessionBrowser:** fix typo on ufunction macro category ([5ce9bdb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5ce9bdb6cb9ef3ac5eaedbdd2d0ea8bedbe1ab1d))
* **SessionBrowser:** removed testing purpose code ([7f4b31c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7f4b31c0445b8dd922dcae3c45a0faaa79b3f57f))
* **settings:** move environment settings enumeration to appropriate file ([8d2df0c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8d2df0c1e7411074f887edce91a59c028e82fbf7))

## [8.2.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/8.2.0%0D8.1.0) (2022-01-04)


### Features

* **blueprint:** agreement blueprint api ([f2f67c8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f2f67c8d58e25bd17d0a687dce2ed84686f24997)), closes [#142](https://accelbyte.atlassian.net/browse/142)
* **cloudsave:** added cloudsave blueprint from cloudsave api ([2625e87](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2625e879d7e551f206a079f42db6b6a8fdd7e6fe))
* **lobby:** add sub_game_mode in optional param struct for lobby SendStartMatchmaking. ([f282f9e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f282f9eb204fb94fa035e1c4b934ab757861540f))
* **reward:** add reward blueprint from reward api ([472d67b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/472d67bc9988937ed4b36bc8be44232af1fc838f))
* **SeasonPass:** add SeasonPass blueprint from SeasonPass api ([17f730e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/17f730e0796eab0cc861ee67154a7e9a95b1d5f7))
* **serverDSM:** Add the blueprint API along with the Server BP multiregistry ([5c3a47a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5c3a47a1b004c34aaaed42b72b82f22f899d2b9f))
* **serverLobby:** add blueprint for serverLobby API ([ff213a8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ff213a8a24c905a45664180dd27ecdfa1dfd47cd))
* **ServerMatchmaking:** add blueprint class and function for Dedicated Server Matchmaking API ([8d51d14](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8d51d140c1b85e730e0923c9edf9bff52d97f0df))
* **serverOauth:** add blueprint class for ServerOAuth2 API ([0ec04ab](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0ec04ab4a9d0105989e507a275bd8849eb4d1dea))


### Bug Fixes

* **cloudsave blueprint:** fixed FJsonObjectWrapper not properly converted to FJsonObject ([8274eb6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8274eb66ace0dd6296b9473b70822ab32e512b20))
* **deprecation:** change deprecation indicator from macro to log warning ([a36e3b4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a36e3b459dd69e43cb40553471dd61fa6f09180c))
* **lobby:** fix lobby violation error when destroying lobby while websocket still have events in queue. ([7d878dc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7d878dc6666f3ae1b1a3ff824d8bf501242b00e1))
* **reporting:** missing #include header file ([5d75ebf](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5d75ebf32d982c90b902c0175b34cc22b98c735d))
* **reward blueprint:** add missing macro class specifier ([9c03b87](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9c03b87d25d635fc77e1c2a338db6f9d5f7950d2))


### Reverts

* **agreement:** re-add agreement blueprint ([3fe8f3a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3fe8f3a75880e56301fc8c62ef348c21bbaea66e))


### Refactors

* **cloudsave blueprint:** rename FDJsonObject to FDPayloadJsonObject to avoid confusion ([de136b7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/de136b75318a0b2d4fc291b2a92aeaaa4f98b076))


### Tests

* **lobby:** add set general lobby rate limit in lobby test setup. ([d9536fc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d9536fc029c2d807de22b0d4d53aabc146f20afc))
* **user:** add additional admin function to create user for testing purpose ([820b5ae](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/820b5aed72a9fb2ea8fc58224cbecf3e05d88d6e))

## [8.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/8.1.0%0D8.0.1) (2021-12-20)


### Features

* **blueprint:** catalog, order, entitlement, currency, & wallet blueprint API ([2a772b7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2a772b78ade5449a84505075e11029452bb655f9))
* **shadowban:** Add Delegate Handler for task refresh function and delete it to prevent memory leak ([3e829e2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3e829e2f3c6555518aba6961e183de12830a5fdc))
* **shadowban:** Add Trim and Ignore case on Authorization check, to make sure catch various Bearer typing possibilities. ([fcb942e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fcb942e9dd292b33e5148531bff1336350ae2643))
* **shadowban:** exclude shadowban trigger from server build ([78eb173](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/78eb1730e3194d4319a7aa4a499270cd3392569e))
* **shadowban:** Fix Linux error compile, constructor has wrong order variable definition ([8319fdd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8319fdd7b6835d075e9becd30344b49d8cdb6c90))
* **shadowban:** switch order of bearer token refresh handle definition, make sure its assigned before refresh token triggered ([e5894de](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e5894dec66166d2404ee6f80e9dab59b09b53cef))


### Bug Fixes

* **lobby:** add missing field to several models, allow lobby MessageNotification handle incoming JSON object (curly bracket) ([42c74d6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/42c74d6603ae4eb4063688769dcf54ceef778aee))
* Merge back UTF-8 BOM removal and UGC UploadTo ([4800b58](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4800b58e74cf06be7687d52a6705f74221a20ff1))
* Merge tag 8.0.0 for clean cutoff release ([26b8417](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/26b8417d4f1d9842228a47a800218cf8eacf8c30))


### Tests

* **lobby:** fix InviteRejected test case not setting leave party delegate ([e0dde98](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e0dde98bb16fecdc9f78f1a5aa28c2f0068272bc))
* **serverlobby:** make sure lobby connected before requesting leave party, add more wait time for writing party storage concurrently ([04b9110](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/04b9110e8638be8fc3cd8caf77bdd8dc82d2c736))

### [8.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/8.0.1%0D8.0.0) (2021-12-10)


### Bug Fixes

* **blueprint:** rename blueprint classes to have AccelByte prefix to avoid conflicts with other classes ([6417423](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/64174236beb0b182bbab12afd5cbc00a150279c7))

## [8.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/8.0.0%0D7.2.1) (2021-12-06)


### ⚠ BREAKING CHANGES

* **user:** User Blueprint API modified
* **serverDSM)!: Revert "feat(serverDSM:** Revert "feat(serverDSM): add support for DSMC Multi Allocation Server Registration."

### Features

* **friends:** add lobby's friend blueprint API ([566fc46](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/566fc46f2a85c49a2b356b3efbb63ab8ab3d220e))
* **Jwt:** Add Utilities functions to Validate JWT using JWK ([807cc0f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/807cc0f2c25638551b0b85080234f55e48c0c980))
* **ServerUserApi:** Game Server api to provide DS searchUserOtherPlatform ([ddc0ca3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ddc0ca36b9ce407abc3c2bda169aaebcae9372af))
* **SyncIOSandApple:** sync purchased item from mobile platform Apple/Google ([2fb4b47](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2fb4b474ad880dfb99350936f7fce2fbe1932a25))
* **user:** complete user blueprint API & fix missing UENUM ([fa9709b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fa9709b4380b4e3ccc90c84051d2663922947b1f))


### Bug Fixes

* change encoding of uplugin file to UTF-8 without BOM ([5e6075e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5e6075e0c202795fffd86587d2b7cde877b6e8aa))
* **httpscheduler:** cancel request will trigger callback ([8736881](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8736881b8407f3d3d1410faef76a71bff86cc7d1))
* **httpscheduler:** check empty response when request status succeeded ([6e35d35](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6e35d355c15b95a007fb5bbd0f0ee4f9b20b8fca))
* **httpscheduler:** wrong break placement when getting denied status ([24b4a6f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/24b4a6f686bcc70367b11280f419ece7c1d4feed))
* **lobby:** fix Uninitialized NewSessionOnly value. ([45dde75](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/45dde75e649ab33ca3f559a36a7f6f0af5a10fe8))
* **lobby:** Handling ws message parsing if an array value have empty string as element ([f0eac5f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f0eac5f2383322dbf0969796c5e81dcbdc86e47a))
* **UGC:** fix adjustment for backend behavior changes ([394565c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/394565c7690544458074b02ad27cb1f094917c94))


### Reverts

* Revert "chore(release): 8.0.0 - commited and tagged by Jenkins" ([55ec3fd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/55ec3fd01a8dcf424356827727120a98fb9bdb38))


### Tests

* **httpscheduler:** update http retry tests to check cancelled state ([5bba5c1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5bba5c1b93fb9bfcd5c500eb69aa5f58b76701b9))
* **lobby:** add wait ds notif to 60 sec ([8650d6b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8650d6b568c8e4466dd3b1c32be2e75ecc60b63e))
* **lobby:** LobbyMatchmakingAllParam case cleanup add wait 10 sec for mmNotif to account for transport delay. ([107ffd9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/107ffd9f87f01c145972073681f6a2d52d75734e))


### Refactors

* **serverDSM)!: Revert "feat(serverDSM:** add support for DSMC Multi Allocation Server Registration." ([d974a97](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d974a97837a74cb5a567c6181528619a54af9d66))

### [7.2.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/7.2.1%0D7.2.0) (2021-11-24)


### Bug Fixes

* **telemetry:** add method to flush pending events without waiting for the next batch in GameTelemetry ([bf69c63](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bf69c63b9797f6785306b680864b3fdd7910f6c9))
* **telemetry:** add method to flush pending events without waiting for the next batch in ServerGameTelemetry ([a1bfe10](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a1bfe1096802d5460dfb25b7d94cb36bc63d0320))

## [7.2.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/7.2.0%0D7.1.1) (2021-11-22)


### Features

* **Lobby BP:** Add non-static Lobby blueprint ([a9ba570](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a9ba5706165c728e6c9340756cc36ea0a566bf00))
* **Lobby:** add start matchmaking with optional param struct, also add NewSessionOnly in matchmaking optional param ([dd3d0b9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/dd3d0b96c4cf30a23381ed867ea50e56797e3cc3))
* **Matchmaking BP:** Add matchmaking blueprints API ([641974f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/641974f407aa333a99c26c2a8f9a9493bd97554b))
* **serverDSM:** add support for DSMC Multi Allocation Server Registration. ([24f5ac5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/24f5ac5b21825b2705aa0162a6642507ae927cfa))
* **User:** get bulk userinfo ([fbcc2e4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fbcc2e49416561ee975f24dd71c50c01e50776e3))


### Bug Fixes

* **lobby:** fix Uninitialized NewSessionOnly value. ([e7025e9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e7025e96e65de463fafb7add60937cbcc8c2220b))
* **lobby:** Handling ws message parsing if an array value have empty string as element ([de21aab](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/de21aaba7c9844424428ea7028a602c6d007a1db))


### Refactors

* **Lobby blueprint:** rename functions to resemble their message types ([4ab83d8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4ab83d8f6b39d17507277100384b4cb7e99e0162))
* **lobby:** refactor lobby API ([43df064](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/43df064c78b594a21ca34d0bb3fb1fd9be36b951))
* **serverDSM): Revert "feat(serverDSM:** add support for DSMC Multi Allocation Server Registration." ([1d48827](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1d4882738efc8e2ab7b2b141ce28739068a7d68f))

### [7.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/7.1.1%0D7.1.0) (2021-11-12)


### Bug Fixes

* **websocketclient:** uninitialized variables cause undefined behavior ([1bba21f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1bba21f1346e0014105fdaaa19588c6795034c4d))

## [7.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/7.1.0%0D7.0.3) (2021-11-08)


### Features

* **GetCurrentUserProgression:** add GetCurrentUserProgression ([22b27b9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/22b27b94894da5a66ab6334bcddaac5860063c8f))
* **lobby:** add API to request DS for custom game session ([d9c0ef3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d9c0ef37fb45a2993dfe110197b53f66b8e60638))
* **sessionbrowser:** add APIs to manage custom game session ([75d0c64](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/75d0c640316afe7e41a6aa34b7cf5ef8024220c4))
* **utilities:** add UEnum to FString conversion and vice versa ([7e346d2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7e346d2c1a9f6998d30d58811b39919ba0a6aea5))
* **websocket:** add scheduler to make sure websocket messages execute in gamethread. ([083545e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/083545ec28071d76356b7e7d0bd119458c2e00c2))


### Bug Fixes

* **MultiRegistry:** Fix double Free calls from ApiClient when using shared credential ([83aeddb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/83aeddb2e97687a7852c96bf4f9a4004f1b6a17b))
* **settings:** Resolve compile warning in AccelByte Setting in ResetSettings ([007b9d2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/007b9d2be12fc49b3656566a27ae36c9f4516be8))
* **websocket:** attempt to clear all ws event before disconnecting & clear prev ws events on connect ([71211fb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/71211fb96ebad66dc18eb9826427fc8847dfeb11))
* **websocket:** make sure OnConnect event is called before disconnecting. ([1e05e7c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1e05e7c3b49aab3f8caaddd3f3b71c14bc997274))
* **websocket:** solve IsConnected returned true before connect event is triggered ([f41e207](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f41e2073807a19adeb2b48eeae426dfc3cb4fec6))


### Refactors

* **lobby:** message id is now taken into account when triggering a esponse delegate ([549d7af](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/549d7aff1df1d288a13f3c1171b4afdd38c29f07))
* **lobby:** moving to switch from if conditionals ([99e8d43](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/99e8d43c336937f60167ee993bea32c4d3c48e5e))
* **lobby:** Update some LobbyResponse string format, add const string for "suffix" namespace ([906c3d6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/906c3d6666eb0348b09ac43142683dabef12b4b4))
* **websocket:** extract websocket management from lobby to it's own class ([25348b1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/25348b10a4fde62dfaf3e44658d863a46f1e49d3))


### Tests

* **lobby:** add simple test AccelByte.Tests.Lobby.B.MessageIdCachedResponse ([cdc215f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/cdc215f629e796c9217078b8010478d661c320fa))
* **lobby:** fix matchmaking random timeout after StartMatchmakingAllParameters Test & increase wait matchmaking notif timeout ([902bacd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/902bacd540727b2155171dbe62577430e206d277))
* **lobby:** Increase timeout for Matchmaking and ReadyConsent notif as original value is too low ([791b655](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/791b655d5fe5ed1b5f30a3fa1b017afbc0dc9c11))
* **sessionbrowser:** add integration tests for custom game session ([4548248](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/454824840e872589eaa880620cea2c48b3225bc3))

### [7.0.3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/7.0.3%0D7.0.2) (2021-10-27)


### Refactors

* **compatibility:** add flag to ignore comparing patch number in the version ([4ea2a02](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4ea2a02abccda60181c4034a69f3ab9aceafa38b))
* **compatibility:** use LogWarning when version mismatch happened for less disruptive process ([36ce5bd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/36ce5bddab8aead5a1946005d30f95d22ddf5b1c))


### Tests

* **compatibility:** add new test cases for ignoring patch number ([d53a905](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d53a9059ee9e8df26666e8cfc8b2120f5ec220d1))

### [7.0.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/7.0.2%0D7.0.1) (2021-10-26)


### Tests

* **lobby:** fix matchmaking random timeout after StartMatchmakingAllParameters Test & increase wait matchmaking notif timeout ([bb41277](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bb41277bf0610449c2b4e74aaa1c2a5455f22f68))


### Refactors

* **ServerDSM:** Remove fetching public IP when registering cloud server ([988f5c0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/988f5c05ff70d452baa81031605be23b38b0e9fe))

### [7.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/7.0.1%0D7.0.0) (2021-10-25)


### Bug Fixes

* **lobby:** return invalidRequest when UserIds empty on bulk user presence ([a09c35f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a09c35f95651086f610dc7aec58c28534ae17578))


### Refactors

* **apiclient:** default ApiClient will use Credentials and HttpRetryScheduler instances from FRegistry ([00f73cb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/00f73cb2ebb28c51d50b01a47e237bdb1f49cc53))

## [7.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/7.0.0%0D6.1.1) (2021-10-11)


### ⚠ BREAKING CHANGES

* **ServerLobby:** remove GetActiveParties method

### Features

* **DSM:** Save server info when registered into variable and add Getter function ([cccd3b9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/cccd3b964eb526863064b2533db8535763821579))
* **ecommerce:** Add GetUserEntitlementOwnershipByItemId ([395bb97](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/395bb97a3953c212a40803c28556467bdd821b74))
* **ecommerce:** remove returnURL on CreateNewOrder if empty ([d39cfbc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d39cfbcec7b270e49098042f37a42bc5fcbcde95))
* implement qos turn manager to get the closest TURN server ([eeeab32](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/eeeab32995d95e82a25ccec6a18c5f5536cf7ef3))
* **reward:** Add new Reward Endpoint and Integration Test ([5ad088f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5ad088f371e1da90f963a2784c718f706eac6e83))
* **settings:** change ResetSettings param from FString to enum ESettingsEnvironment ([b6f7948](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b6f79489e438c41f1ce47e927d801698bbdf31a6))


### Bug Fixes

* **CheckServicesCompatibility:** Only check compatibility for non shipping build ([e2c78c5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e2c78c5c3acfd1d320ffe7aafc0b8edb8d394974))
* **lobby:** linux build failure due to werror on unreal engine > 4.25 ([fd12ddb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fd12ddb0d6a02f3530a1eeb57bc7f4d1745ad822))
* **sdkModule:** remove GetDefaultUrl, change it to call from Settings instead ([138acc3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/138acc34c2f91dd3e18b783b97ed16ed53e95a3a))
* **seasonpass:** season pass tier duplicates when there are more than 1 pass codes ([547b32b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/547b32bc974b859de360d009efd909f6507d51a5))
* **User:** change UpdateUser verb from PUT to PATCH ([ff6ca90](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ff6ca90c8ea7ce78c390d7233ba2a56aa8de6741))


### Tests

* **ParseErrorTest:** add unit test for service error parser ([c53be56](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c53be5697afac1f3e8104ecef3564ea0d01ed589))


### Refactors

* **ServerLobby:** remove GetActiveParties method ([4dc4d8f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4dc4d8f304b4d666472086ed2e4425315df6ebba))

### [6.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/6.1.1%0D6.1.0) (2021-10-01)


### Tests

* **Reporting:** add missing include ([326d54e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/326d54e7deea4a57b741c28c70036d70dc259d67))

## [6.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/6.1.0%0D6.0.1) (2021-09-27)


### Features

* **ecommerce:** add epicgames entitlement on SyncPlatformPurchase ([b78d9a5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b78d9a59e6c79c6d8b334b7e966457bfd647e481))
* **ecommerce:** add Media Item type ([f17b815](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f17b815e8ee7312cf67168c0dc46b4b808ed0b0e))
* **lobby:** add lobby ws request error handler and error message ([0eabf1d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0eabf1d2d77004789d6b83e93c274ff73449dca0))
* **misc:** add GetServerCurrentTime ([c3afc90](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c3afc9072a27e2ffaac704f06159a13e23a7de35))
* **shadowban:** Add refresh token when token revoked. Pause task and resume after new token retrieved ([e16d45a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e16d45a62971b8a8fcf8c3401a455f31de51202d))
* **Version:** Ensure version is compatible ([41b9b73](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/41b9b73d53bf6899a9be1822276f9b9c0e913894))


### Tests

* **agreement:** revision ([e481ab0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e481ab0d3d1c35e056500c815317f265bca6b4f3))

### [6.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/6.0.1%0D6.0.0) (2021-09-16)


### Bug Fixes

* **User:** change UpdateUser verb from PUT to PATCH ([d0babcf](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d0babcf7ad8c2e60fa1d6c1f59c095c09f94a9b0))

## [6.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/6.0.0%0D5.0.0) (2021-09-13)


### ⚠ BREAKING CHANGES

* **httpclient:** change methods to return FAccelByteTaskPtr so that http request can be cancelled

### Features

* **ecommerce:** new currency list and cancel order endpoint ([1568489](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/15684898e89e3b5f2b985dc1d21871db00a5a4c6))
* **ecommerce:** New CurrencyList and CancelOrder endpoint ([02cdd53](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/02cdd53b661bb62dd5b637867590e8b0979e49ba))
* **ecommerce:** New CurrencyList and CancelOrder endpoint ([940a819](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/940a8194d253c086985e9046b5cfa03ccf60049d))
* **ecommerce:** New CurrencyList and CancelOrder endpoint ([2b75447](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2b75447a24ae723030defc76d2fb39e86070ffc8))
* **ecommerce:** New CurrencyList and CancelOrder endpoint ([f883dca](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f883dca9eec3fff37a600054b86e1e104f4c520d))
* **FHttpClient:** make building url easier with namespace and user id token replacement with values in credentials ([f132fab](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f132fabe9b2d8a67b58c96edea12c111ded43481))
* **httpclient:** change methods to return FAccelByteTaskPtr so that http request can be cancelled ([6e46b25](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6e46b25a67d0156e1628db07e071690bbc1a145c))
* **lobby:** add unbind event per lobby scope ([ec76f4f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ec76f4fe2089a2ee0e8569b30c689e66e9680797))
* **lobby:** add unbind event per lobby scope ([c446e76](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c446e76e81f37b4038c50a3307917e2eb80802ba))
* **MultiRegistry:** Add "default" key parameter to GetClientApi static method ([2d7a9b8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2d7a9b8ab068a1b7f32aead64aef6b7a1235b927))
* **MultiRegistry:** Add"default" key parameter to GetClientApi static method (again) ([85c8afb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/85c8afb1967d4f8e8e6b490a9f5fb873b583178f))
* **reporting:** re-add reporting SDK ([fc509f6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fc509f62b496faceab420b40a61c6400c3b8f0f9))
* **User:** Add LoginWithRefreshToken ([5f5b47c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5f5b47c28489770a544ef8bdd5db8397e69f7009))


### Bug Fixes

* **lobby:** fixed PartyDataUpdateNotif Leader field typo. ([e78c4cb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e78c4cb2b09b5922da8ec56bf14482699cef3fa6))


### Refactors

* revert back changes that cause errors ([86be55c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/86be55cc63a43792d309649c15858654eed54c54))


### Tests

* **ecommerce:** update Ecommerce Test Admin to fix some crashes during Ecommerce Setup process ([469c088](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/469c088f5e2dacbbe0cd994155dd8657a10f704c))
* **ecommerce:** use valid uuid format for testing item that does not exist + move ServerEntitlementFulfillInvalidFailed to integration tests ([0e460dd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0e460dd2f459d436c1dd275fac8e36ad03a12f07))
* **lobby:** fix intermttent test error on rematchmaking ([bb41553](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bb41553f98fb5e6c1cdefcf6b1f4748bea5be69b))
* **serverlobby:** fix intermittent test error in get set session attribute ([d620ea7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d620ea7a3d2c4136ff4807d64b018b3b6bdc5097))

## [5.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/5.0.0%0D4.0.1) (2021-08-30)


### ⚠ BREAKING CHANGES

* **Achievement:** Change the type of GoalValue of FAccelByteModelsPublicAchievement and FAccelByteModelsMultiLanguageAchievement and model from int32 into float.

### Features

* **Error:** parse oauth error response ([c795d02](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c795d02849124226195caa971a965d6ae51af73e))
* **Error:** parse oauth error response ([3e2da7f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3e2da7f53f7a64a8a68751de2f1a93b641154516))
* **FApiClient:** add GetApi method template to support API extension ([6e6c247](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6e6c247dd7329ad4f4ec98ed52d44b81613a2b72))
* **FHttpClient:** implement http client to make api request easier ([4eb0821](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4eb08212cd861602504efc022ecabf36108fbd74))
* **User:** Add Logout api ([572758c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/572758cabadae8f90a14f49af5d44b503779b593))


### Bug Fixes

* **Achievement:** Fix the Achievement Api's Public Achievement and Multi Language Achievement model difference with the Backend response. Fix the Test Helper's Achievement Request and Achievement Response model difference with the Backend response. ([c701114](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c70111429e118870b94c7c7ac161423d669a0c4b))
* **Subscription:** Fix subscription integration test. ([b9bb250](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b9bb250dd966209156881ea49fe5c49bf191e0a0))


### Refactors

* **FApiClient and FRegistry:** Each API receive FHttpRetryScheduler reference from constructur instead of directly referring the singleton ([7d39e38](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7d39e38d6db25e09c1df17e35fb4023a9c911945))


### Tests

* **agreement:** make more resistant to dirty data ([ab9fa83](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ab9fa83ecd54abe58f3350d818f25fb5f1484f7f))
* **FHttpClient:** add core test for FHttpClient ([116e456](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/116e456178aa1b184ce8d3d62981e09ca346c4cc))
* **lobby:** fix LobbyTestStartMatchmakingTempPartyOfTwo_ReturnOk ([cc50770](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/cc50770684608dd41c8b518dff87e7b735e8844b))

### [4.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/4.0.1%0D4.0.0) (2021-08-16)

## [4.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/4.0.0%0D3.2.0) (2021-08-02)


### ⚠ BREAKING CHANGES

* **Achievement:** Change the type of LatestValue of FAccelByteModelsUserAchievement model from int32 into float.

### Features

* add awscognito platform type ([0610ef2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0610ef28ab6907069be6402d1d887b87a155a487))
* Add digital signature check to plugin ([94e8843](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/94e8843875a10f41f6e976000198afef40e7e5d7))
* **core:** add scope lock-free mechanism on HttpRetryScheduler for thread-safety ([c731dcb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c731dcbbec20e44cfd207c36a3e7abdb064c4418)), closes [#2](https://accelbyte.atlassian.net/browse/2)
* **jenkins:** jenkinsfile - remove ue 4.20, 4.21 and add 4.26 ([a152ca6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a152ca62e85dbe4c9f04439f6b53e6d1defd8ee9))
* **jenkins:** make build linux & windows parallel + separate build editor and game to make ci faster ([3a98e60](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3a98e60f5840eb3c28f4c595a234aa952c71176a))
* **jenkins:** use the new justice-sdk-builder ([472995e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/472995ed380f6125a81b09d6412fed869dada6b3))
* **lobby:** add get & getAll session Attributes ws commands ([4208e56](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4208e56fb3dc7510714a624c4ca584fa188e185a))
* **lobby:** add GetOnlineFriendPresence, and marked OnGetAllUSerPresence obsolete. ([eaf18ef](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/eaf18ef389fd2ebd84f2ed3bb0a3753f0ff8968b))
* **lobby:** Generate party code ([2e04734](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2e04734c03b06a91e40646533899f4c3cb5f5b2d))
* **serverloby:** add Set, Get, GetAll user session attribute ([1d69966](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1d699664354896f936ddb1dc4fbaf04f50b3f486))
* **session browser:** add get session data by session id ([6982214](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6982214c52344b03b415286c56f8bd11c0e38e63))
* **ue5ea:** Jenkins - Build.ps1 - remove superficial option ue4exe ([d93e2d2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d93e2d28538a4f646f606568cd9ed042d6ce9f38))
* **ue5ea:** Jenkins - RunTests.ps1 - add alternate unreal editor exe for UE5 and UE4 ([2c9deca](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2c9deca8fcb2a40490651ae7370dc9fa14d91126))
* **ue5ea:** jenkins.config.json - add ue 5.0ea ([21bb301](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/21bb301287e4de1ffc059fd8091b9a8b1f1eda3e))
* **unrealengine:** remove support for ue < 4.25 ([a5b7791](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a5b7791fa7799f0773c6556f09a70a0e092f963f))


### Bug Fixes

* **Achievement:** Fix the User Achievement model difference with the Backend response. ([1581d57](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1581d57a4f34d73a10de47f127280d85c3b1a0ec))
* **files order:** Fix signature algorithm to ensure files and directories order consistent and update digital signature ([57f9688](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/57f968814d190d598b20a2b8f37734dcbb070790))
* **lobby:** Response.Code always empty ([134429c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/134429cefcabfb6e0f72ae33dd7dd4f0ad321ec7))
* **Oauth2:** Fix the GetTokenWithAuthorizationCode() using v1 endpoint ([1e642ae](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1e642ae06b83b08af9cb624bc3b94d1def4390d1))
* **Telemetry:** Change GetSeconds() to GetTotalSeconds ([c8b57de](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c8b57de210f1c91b5bc6979015d1eddb4f82d4f1))
* **UGC:** Fix the integration test naming ([889e2f6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/889e2f64393448fc3fd3e44fad55a4cc6ca0bf68))
* **UGC:** Rewrite the inline documentation: fixing from copy-paste mistake, and not standard writing. Fix the copyright. ([25b909a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/25b909a3281b5de4271e55c75d012655a4a84056))
* **uplugin:** remove unicode literal in PrebuildSteps ([ac08206](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ac082067a8a69c636cb421a4456e022860c6dd9b))


### Documentations

* add version.json, changelog, and readme ([5aa4cf2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5aa4cf2b662917f0383f159d69f5138791979daf))


### Tests

* **AUser:** fix test regression due to invalid partial query result assertion ([37e7a98](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/37e7a986670b9120e2bfa30942ce9b226ac335a2))
* **AUser:** make STEAM_KEY environment variable optional ([eb0d5c5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/eb0d5c526047600758fbb54471676053492789ef))
* **basic:** check steam ticket before testing with steam in AUser tests ([e1d06ae](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e1d06ae4a93df953fc41d2cb7d615c94962e08b4))
* **general:** fix incorrect token retrieval for the admin user in test utilities ([448d23c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/448d23c5b444c7a842976c984330b281313c3088)), closes [#19](https://accelbyte.atlassian.net/browse/19)
* **play:** add random element in lobby user test emails ([ccf71a1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ccf71a1e287a7e7632a50f4aec51394c7daabacd))
* **play:** fixed lobby, serverlobby, joinablesession & session browser tests ([c705f48](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c705f48bfab962dfeb9a90bdde222457f2360e73)), closes [#375](https://accelbyte.atlassian.net/browse/375)
* **subscription:** fix error categoryPath /game ([8ad19c8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8ad19c8a46d4034da4939418a4d0ab298305835c))

## [3.2.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/3.2.0%0D3.1.0) (2021-07-07)


### Features

* **Achievement:** remove redundant test code + fix identifier naming according to ue4 guideline ([4340c75](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/4340c759a78b5ac2b01408528078e91e47cea116))
* **jenkins:** Convert scripts to powershell ([d8c6faf](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/d8c6faf794b1e64696f094e2ad69cb2e6af1aaef))
* **jenkins:** Convert scripts to powershell ([973cd4a](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/973cd4a3006da5ddbced29a5bde52cb3976e4a78))
* **Json:** Remove empty string fields from FJSonObject ([176e210](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/176e210fc41928491052afb0474d3591e8fccb33))
* **lobby:** add errorNotif and move admin function into new file ([af5ddbc](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/af5ddbcc773d35810ca3db360ecc530dd7c0edcf))
* **lobby:** Add Send Notification to User ([4f46c75](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/4f46c756d3c8f590ef4f23e067572005543b4142))
* **ugc:** Add UGC into SDK ([8bb8e87](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/8bb8e87a55a85dce51675d25cdb2b629ae136e71))
* **UserProfile:** add BatchGetPublicUserProfileInfos ([759f4d5](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/759f4d52e853b9a6d6de2e8bb01fc96964ed903a))
* **UserProfile:** Get custom attribute other user id ([1dce0a5](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/1dce0a5659a63d128c9969b799e453b628813697))


### Bug Fixes

* **CloudSaveTest:** Disable concurrent record test ([02ed0a4](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/02ed0a4493c022f69b19995ab416b8bd71ff0aae))
* **CloudSaveTest:** Disable concurrent record test ([1aa8263](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/1aa82637d0dc335d51c71127546edb1e275c1e30))
* **Jenkins:** Revert unfinished work to convert scripts to powershell ([3cbcc1e](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/3cbcc1eb58c65a1d8c2924202e8c2999ae641b18))
* **User test:** test failed due to missing waiting ([faf6818](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/faf6818563c985d492532be37a6775e6abdade2f))


### Reverts

* Revert "test(Achievement): make test variable declarations static to keep it local + make the names shorter" ([f1e8785](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f1e8785b3ffffb3704709df4996e97779ffe0854))
* Revert "test(MultiRegistry): make test variable declarations static to keep it local + make the names shorter" ([bd0f2ea](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/bd0f2ea376c1cd464f96721f3c323b0bcf2d3e20))
* Revert "test(GameProfile): make test variable declarations static to keep it local + make the names shorter" ([e7c84bd](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e7c84bd24c04657de05bb1c57c4ed2f1c685c199))
* Revert "test(UserIntegration): make test variable declarations static to keep it local + make the names shorter" ([879f5ba](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/879f5bad209aaf0ab8a3561ed88a7fd45d6de92c))


### Tests

* **Achievement:** deduplicate QueryAchievement tests ([d5c8bf2](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/d5c8bf2b082d664689e37654c56c327e4c404f95))
* **Achievement:** deduplicate QueryAchievement tests (more) ([63daee4](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/63daee4b9a2efdab1a182f0b2448f9afbebce583))
* **Achievement:** remove unnecessary http flush ([b76e99a](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/b76e99afcf4bed5e559f6b9187b98b48717adc33))
* **Achievement:** rename identifiers according to ue4 guideline ([651e5ad](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/651e5ad92c730f0d16de99becf71b3a930e9b082))
* **Achievement:** setup - use reference on achievements loop ([9b070d9](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/9b070d9ec17e8ef608803feda03e91a70ed96153))
* **CloudSave & GameTelemetry:** change check macro with AB_TEST_XXXX that doesn't crash if failed ([93dff77](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/93dff775262ce1a3bcd88d3f2d51d4543e11f18a))
* **eCommerce:** Change check macro to AB_TEST_XXX to not crash test automation ([4c067a0](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/4c067a07e9081729900b3234dc630b566c0cd3ef))
* **Ecommerce:** fix sdk tests (max draft 1 be changes, remove unnecessary http flush, remove code redundancy) ([6742f8f](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/6742f8f109ec7b2d8164370d8de4cfe4492b44e2))
* **lobby:** fix lobby test stuck ([8744a3f](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/8744a3fde575fb7c4dea2182a6d6d021262c5a91)), closes [#364](https://accelbyte.atlassian.net/browse/364)
* **MultiRegistry:** use setup/teardown test users from utilities ([f98b2a5](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f98b2a5f8027acda604462a3b4e37239f15e9c10))
* **serverlobby:** fix serverlobby test cases stuck ([c7ecf6d](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/c7ecf6d2fd4e1aa9ed25199eaea0a42fd2f87435)), closes [#366](https://accelbyte.atlassian.net/browse/366)
* **Social:** change check macro into AB_TEST_XXX to not crash automated test ([469b04c](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/469b04c1fbc2e5fe33d87225154036f418915286))
* **UserIntegration:** BatchGetPublicUserProfileInfos - use setup/teardown test users from utilities ([1efcbad](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/1efcbadc2be96af8236dbddedbdef0c2f73e77d6))
* **UserIntegration:** BatchGetPublicUserProfileInfos - use setup/teardown test users from utilities (more) ([95667f3](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/95667f318d0c3d0a06ed5a2a40f30a91e222fb1d))
* **UserIntegration:** clean up ([31b0d58](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/31b0d58c5c753e81674bb59f577785973c54c5f8))
* **UserIntegration:** fix error due to missing waiting in GetUserByDisplayName, GetUserByEmailAddress, GetUserByUsername ([6f0bb58](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/6f0bb582c60ef7a020a8bf4d809fd4ead79d5236))
* **User:** Replace check macro with more descriptive TestXXX-based AB_TEST_XXX macros ([8643f1e](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/8643f1efeba008abb11c83d8ad2beed12467132c))
* **Utilities:** move LatenciesPredicate to its proper place ([57e8ad3](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/57e8ad3ceaa5c6a6b7ea0fef1828249543176768))
* **Utilities:** remove redundant get required environment variable logic ([0f52026](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/0f520268e56cdfdf8fd912819c1aed71ec35482b))

## [3.1.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/3.1.0%0D3.0.1) (2021-06-23)


### Features

* **apigw:** remove apigw and adjust endpoint, add Registerv3 accept legal policy when create new account ([db83fb6](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/db83fb6919fc3f082bb0b825d2caf6d59eacfd58))
* **FMultiRegistry,FApiClient:** allow multiple users to be signed in at one ([503d2c9](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/503d2c994ee0b104011e129d8c9332a1bf4566e8))
* **lobby:** Bulk get user presence ([e7fcfe4](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e7fcfe432379b379be27e2e25172b7e9d75caadb))
* **lobby:** Invite and Join Party via Code ([6c19a0e](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/6c19a0e52e539d43b65ac738d11307461a8a7330))
* **lobby:** Promote party leader request ([f2348b8](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f2348b8f33efa4bdf8a63f1b243e179103913a26))
* **lobby:** Reject party invitation ([98c016c](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/98c016cc23d0d44e524223d9ceb74ed51f0b9ff2))
* **Party:** Get party data by party id ([ac412e4](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/ac412e4f685cb47c1c50fca7a8e4a232b94318d6))


### Bug Fixes

* **Tests.AUserProfile:** remove redundant delete profile function ([b873b1c](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/b873b1c5af1c389cc75578e6ff81e65e2f2e5f46))
* **Tests.AUserProfile:** remove redundant delete profile function ([5d97c87](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/5d97c874b2182159ef3fec6d0b8f2fda4b987bc2))

## [3.0.1](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/3.0.1%0D3.0.0) (2021-06-09)


### Features

* **ecommerce:** add some entitlement endpoint to server sdk ([e284472](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/e28447288bd8a96816e3a63c69c140679cd210e8))
875da0fe172fc5120ed60ca9fc276d80302fd2cd))
* **lobby:** unfriend, cancel friend, and reject friend notification ([099489a](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/099489a0316f84be9caddf45198469e3f77782e9))

### Bug Fixes

* **user:** Fix login with other platform function ([f6624fa](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f6624fa749d924f739891dc1f53b068ec67e421e))
* **entitlement:** change appid to string ([78592c5](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/78592c5da61355a2534bdcb9fd24a91f797c3705)

## [3.0.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/3.0.0%0D2.35.0) (2021-06-09)


### ⚠ BREAKING CHANGES

* **AUser.Tests:** Fix breaking change caused by removal sensitive info
* **tests.AUser:** Fix breaking change caused by removal sensitive info

### Features

* **ecommerce:** redeem code feature from VE repo ([875da0f](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/875da0fe172fc5120ed60ca9fc276d80302fd2cd))
* **lobby:** add lobbySessionId to be able to reconnect using same token ([1bf75c8](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/1bf75c8e25f3538c90383b260540099378b1e932))


### Bug Fixes

* **AUser.Tests:** Fix breaking change caused by removal sensitive info ([4cecd53](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/4cecd53d8e310262e3eac360b84dc27197ecfe60))
* **log:** github sync, removed nonfunctional verbosity set functions. ([9528e97](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/9528e970be64ddd405af5c2f41ba7ab5323c7d54))
* **tests.AUser:** Fix breaking change caused by removal sensitive info ([f0a4d79](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/f0a4d7929948dcd2ced4ba0bc7580ad27b8fbfff))
* error compile need to include AccelByteError.h on file AccelByteUtilities.h ([3d18b10](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/3d18b10d66286147839fdb6abfb2d9361f24a268)), closes [#312](https://accelbyte.atlassian.net/browse/312)

## [2.35.0](https://bitbucket.org/accelbyte/justice-ue4-sdk/branches/compare/2.35.0%0D2.34.0) (2021-05-25)


### Features

* **dsm:** add RegisterLocalServer with public ip address & moved GetPublicIp to AccelByteUtilites ([62a2d53](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/62a2d53395e8a462ee0cc39ecaa10bb04e5c2711)), closes [#307](https://accelbyte.atlassian.net/browse/307)
* **lobby:** add SetSessionAttribute function & matchmaking with extra attribute field ([3f3dcdc](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/3f3dcdc69df550905a12cd2bdb3aeda8de18c677)), closes [#303](https://accelbyte.atlassian.net/browse/303)


### Bug Fixes

* **Utilities:** fix FAccelByteModelsPubIp undefined in UE4 4.20 ([8495c02](https://bitbucket.org/accelbyte/justice-ue4-sdk/commits/8495c02526ce04c5a7b9706b2956af24c1659658)), closes [#310](https://accelbyte.atlassian.net/browse/310)

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
