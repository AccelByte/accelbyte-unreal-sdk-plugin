# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

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
