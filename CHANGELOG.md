# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

### [25.2.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/25.2.1%0D25.2.0) (2024-04-26)


### Bug Fixes

* **Config:** fix get config to not need auth token for request ([f5bfdf5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f5bfdf54d84cbc81ee6cd4b0246d810c5f693e6e))

## [25.2.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/25.2.0%0D25.1.1) (2024-04-05)


### Features

* **DSMC:** Implement RegisterServerToDSM with server info on success ([adefb3c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/adefb3c996d32b5510658cb278d03971f95e3496))
* **ecommerce:** add API for retrieving IAP items mapping ([f33aec8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f33aec81addc1e3a3b3797cddf4ca9c9996931d4))
* **ecommerce:** Add DebitUserWalletV2 and PaymentWithUserWallet ([b025d97](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b025d97ab0dd7f82085eed0d957f6eb10699b270))
* **lobby:** add entitlement token generation and check to lobby connection ([c9fedbc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c9fedbc2d1a8d58f16f611b3094e7819d66e0d36))
* **session:** add 'previousStatus' field to session member model ([7edafde](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7edafdecdec5e176edc060001d80426755d0e61b))


### Bug Fixes

* **Core:** add challenge api on registry ([69ed666](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/69ed666a61a071096d28e45578519f7872eb2d25))
* **FormatUrl:** fix format api url flow by using sdk setting if the session is not valid ([5e1074a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5e1074ab1dcd66a35f91c68ac0cffde8c8d37a63))
* **http:** fix construct key implementation by using hash key to prevent the fname character limitation ([4e76335](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4e76335403b768ef390586a240d4230ea22c6cc0))

### [25.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/25.1.1%0D25.1.0) (2024-03-28)


### Features

* **login:** add simultaneous user info to credentials object ([7abd0c0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7abd0c02df13c00ac1501c7b603fa50fd58e7aa9))
* **oauth:** add additional simultaneous login info on token model ([4800023](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4800023c0632f4b39a560eceafeb036c0ce247ba))


### Bug Fixes

* add platformUserId as cachedTokenKey on ClaimAccessToken ([d954274](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d954274dafc12f33cbbce9f222b4a17452ce8007))

## [25.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/25.1.0%0D25.0.0) (2024-03-24)


### Features

* **mpv2:** add support for friends only joinability in game and party session ([d29e8c6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d29e8c6b006d3ce51cdf77c3237e988d7be6f81e))
* **ttl:** expose ttl config on cloud save api ([7395030](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/73950303f60825a68da12ec203615e1e8a8ddd0a))


### Bug Fixes

* **chore:** change DefaultSection name to DefaultSectionPath to prevent linux shadow declaration error ([6a42e22](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6a42e226c846f973b04fd3e043feca0b86dabe2a))
* **launcher:** get the exchange code and call generate game token if using exchange code. ([92076d0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/92076d03d1983098a0b345d92dd72aa8e0793082))
* **MessagingSystem:** fix issue when messageSystem is not the same for api and registry when create a default apiClient ([2a38e87](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2a38e87df2b131b248acf0317b87ab7eafc796a9))
* **setting:** update override setting using AccelByte config injection ([751459c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/751459ca210cdc274a6deccb992ce4fe93d011b7))

## [25.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/25.0.0%0D24.11.0) (2024-03-08)


### ⚠ BREAKING CHANGES

* **CommerceBalance:** balance type on wallet is using int64 now

### Features

* add enablesecret field to response model in sdk ([93d1c6b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/93d1c6b3360ed7ad4e9b898e346a1cae453abc67))
* **backfill:** add new notification for backfill ticket expired ([b501d1b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b501d1bc6393ca2cac99a512b21bfefdd78e53b7))
* **challenge:** add client and server API methods for the challenge service ([83bb01e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/83bb01e677ee5ab514f49eda45526b3d27ed0e19))


### Bug Fixes

* **CommerceBalance:** fix wallet balance using int64 type ([58059ff](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/58059ffc4d6104e0002b312580e98d9c4c69c358))
* **ecommerce:** add missing field DisplayOrder in FAccelByteModelsSectionInfo ([9915723](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/99157237657a94df8f8d5a9ade6062fc073c64bb))

## [24.11.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.11.0%0D24.10.0) (2024-02-21)


### Features

* add common config API ([600021d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/600021d9bbe433a8bd9007e453e3b4fa06deb25c))
* add new autoCalcEstimatedPrice Param ([2abb5f2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2abb5f2e111b0579d7a609b0085051f46a537297))
* add TicketIDs field in game session model ([df9ed23](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/df9ed23f427999627d0a8ed997b2158b6077869f))
* Implement notification sender ([4f4702c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4f4702cccb0666a7660f3599c02b7518e3e7a2af))
* **lobby:** add friend REST API ([f7afd5e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f7afd5e88adbf8ea24c58a56f41e7507df9eb4ce))
* **lobby:** add handler for retrieving missing notifications ([732132f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/732132fe3596098c6acf7a11165b09bb9b37935c))
* **UGC:** add some public api that can be used without login in some environment ([4562ad7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4562ad7a8a26cf038acf6599348351167e2a68e3))


### Bug Fixes

* **account:** account profile revamp ([6164cee](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6164cee7671ed7f0afa5f7a929c028342696313b))
* add validation to User API ([d7cbc42](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d7cbc42342cb4b9a665f1f52673bf4db6baa5530))
* incorrect request body serialized in GetUserOtherPlatformBasicPublicInfo ([f20ab79](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f20ab7996fd1271d31a2309594fd22bff2706022))
* remove validation for platform information request ([38ef15a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/38ef15ab5181ccf0c1db21f09534795ac3542c9d))
* **session:** remove empty fields for storePlayerAttributes's request ([23543f9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/23543f9ca1c9ca1ca2673c127082740338b0038f))
* **support:** add Ext field for FAccelByteModelsSectionInfo model ([2b97448](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2b974482dd7928aeaceb78c77f664ace6bfe984b))
* **support:** add SEASONAL enum type on EAccelByteCycle ([9058182](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/90581827ed99b94ad53307e5cebfcffeb6b39571))
* **support:** repositioning the parameter on Region and Language inside ListActiveSectionContents function ([80e28c4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/80e28c44790bd63761f7834fe8b23ecf87835745))


### Refactors

* **PlatformHandler:** add dictionary as extra attributes for FetchPlatfromToken method ([f28b028](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f28b0283ad9f6033eccc54c11f326b56e2e8a011))


### Tests

* add define guard to avoid parallel tests to override stored value ([187770c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/187770c3acc7270dfd3b02f7edebd7149f723b64))

## [24.10.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.10.0%0D24.9.0) (2024-02-05)


### Features

* add AMS fleet command API ([0933fe2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0933fe20a59a171e1ccf70b18a89f49c77c7f7ef))
* add endpoint to get other user 3rd party platform information ([f08ed7e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f08ed7e72cc49d94ccdd56fac0b96d213355e0fc))
* add new endpoint of UGC staging content ([675adc9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/675adc9b05674c5b6ac2d848a0337e1869f746d8))
* **namespace:** encode namespace ([8da9113](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8da9113825fe53ab10978d9ba0b29154f62e77ad))
* **namespace:** REVERT namespace encode ([da5ee04](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/da5ee046e3f697278c6812e01b22cc3604417da4))
* **platformHandler:** add OnPresenceChanged delegate to notify sdk if there any presence changed from platform ([733529c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/733529cf7746c556a765ad2d2e03f2be67a4cf8e))


### Bug Fixes

* add missing FlightId header in Login with DeviceId functionality ([fcf9306](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fcf9306f15ce942aea2f320fa1d8723df9bd3ff7))
* change achievement blueprint header ([8a7541d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8a7541d23cf1904b1f8e3c9ceed76a9cd34260ca))
* **lobby:** store notification message delegate handle, replace instead of add new delegate ([4a47490](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4a47490a1e868239345bd72fd29a701b7595039a))
* move DisconnectFromExternalReconnect outside of the boundary ([cbe2e90](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/cbe2e902b1c225d123d550a4213db426b72cd3e7))
* remove validation for platform information request ([da1b7ce](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/da1b7ce9e0ce6f084a82015362606c36d4e59908))
* **settings:** ClientSecret is still using the old value when the key is not defined in the .ini file ([299a4f7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/299a4f75be0418e72ce0b540e07d0c149bbbfcde))

## [24.9.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.9.0%0D24.8.0) (2024-01-18)


### Features

* add messaging system for communicating between api and automatically send user region on lobby connected ([db2e764](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/db2e7648a8e721956fe9718824afffebf0939407))
* **HTTP:** automatically retry the request if the backend returns 429 response (too many request) ([1e98a05](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1e98a05213e45d1bc616681b55809fd626fdb475))
* implement message envelope in lobby ([57a9360](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/57a9360c363a26f92da7ddb15e52d82f1e64fe31))
* Implement network conditioner in chat and lobby apis ([c336c73](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c336c73db64a68509d21aba10b444a1039d96790))
* Implement NetworkConditioner class ([f7f6763](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f7f6763f1c21c8858bf46c896d86532c0c9c647e))
* **Login:** simultaneous login IAM function ([e660235](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e6602356fc3f67ca149c1fd2f279f9c7cb5cfe68))
* sdk adjustment to support returning explicit 3rd party platform info ([1eec4a7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1eec4a7e2e6e38ce8c70e5f7b0383974e594359a))
* send new access token to chat every auth token set and remove chat refresh token handlers ([442eacb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/442eacb3858eeebd07dced8a56ca52b9bbceb0e5))
* send new auth token to lobby every time it's set ([6543d70](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6543d70889789ff829481e94ce276a0471d080ac))
* **session:** Session Service Player Validation ([649240c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/649240cde4b227762a6dbbedbecb5abab3e270f3))
* **session:** Session Service Player Validation ([294135f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/294135f8fd828282b97ac47a2f264953d9f59446))
* SHA1 generator, update and set to lower case by default ([06ebd3d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/06ebd3d756594316256fd5fbb450b372fa114fe7))


### Bug Fixes

* add missing include header in AccelByteChatApi.cpp ([327b293](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/327b293a9ec9bb7e45f3dff9531c012eef0c5689))
* add more description on platform user id ([2bfd454](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2bfd45498e40e8a5c90adcec646a3f99c23f3d1f))
* add platform user id on login refresh token to cached when try to re-login ([e9d8aac](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e9d8aac598e881537bef2bb9e2a98f48695d1318))
* change queue to shared pointer and add delay in PingRegionsSetLatencies to avoid intermittent enqueue error in messaging system ([0fcf7ff](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0fcf7ffcaeef1f59a10e4b3bdf9a5e807a8348c8))
* change the parameter of platform user id position ([1ad5cec](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1ad5cec3897bcb494166e8dce1cfbcbe68dcd4fa))
* **DeviceID:** Device ID randomizer not completely random due to uninitialize seed number ([c1f12dc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c1f12dcca90993c28761dae17e44e3e7905eb9f8))
* fix LogPython same name warning ([440ecef](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/440ecefb251419166d93f78562a0657c2716e5d8))
* GEngine check cause compile errors in UE 5.2 ([f770158](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f770158d04ab810c848bdab90e8b922677ff753c))
* redo removing logphyton warning on editor startup ([0d39604](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0d396046f4ffb96a2ebfa8a419e40150c30f6a40))
* remove ApiClient Credentials intializer ([3830167](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3830167f781297443e1e67f35e515cfc4b98ecaf))
* remove the new function and add new platform user id optional parameter on the existing function ([b36074b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b36074b9203e0083cad382dca9d1cd7651b92b45))
* separate SendQosLatenciesMessage from UdpPing success delegate ([6504a92](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6504a92b97ba3049ffb68c23094c943483d6ad26))
* **utility:** ambiguous FMath::Pow ([60af48e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/60af48e93a10d01bfe52b5b42616522123cc72cd))


### Refactors

* remove lobby token refresh handlers ([9adb80a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9adb80afa4348d66b60a3eed056ce041a5b06d11))

## [24.8.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.8.0%0D24.7.1) (2024-01-09)


### Features

* add game session recent player endpoint ([1f3adea](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1f3adea47845d524c132f7ed571a4931356daf44))
* expose cloud save game binary records endpoints in server SDK ([1b50200](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1b5020058034e4f82642e0009288fcdfba32f23b))
* **gameTelemetry:** add feature to enable/disable game telemetry cache from Engine.ini setup ([4cdd848](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4cdd84879e73dbfe35352c884deaedc07e4c99f6))
* **IAM:** refresh native platform token ([c8d3ac1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c8d3ac1ea43dd4f8d92e81f50192f5fca0f62598))
* new lobby notification delegate for UE OSS ([d2ae140](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d2ae140975153167b224ac446ac1319137cf6d94))
* **oauth:** add namespaceRoles on Oauth2Token model ([5c748f3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5c748f3913021966cafd3f2a516b341d0fb582af))
* **ServerTime:** return back caculated ServerTime and sync ServerTime with the service every one hour. ([f6881af](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f6881af70e7aade36ef938f4f7273653b7778e9a))


### Bug Fixes

* GetPublicUserProfileInfo headers attached as queryParam ([c0c7622](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c0c7622dff79b587c28c78fdaa7bad2e92d39871))
* **presenceBroadcast:** mark StartHeartbeat and StopHeartbeat as deprecated since it not supposed to be used outside of the class ([fbc0ab7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fbc0ab77a931a6142718ac8ab4805c0a70a81bc6))

### [24.7.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.7.1%0D24.7.0) (2024-01-02)


### Bug Fixes

* hotfix get api client from oss ([f7fcf08](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f7fcf086a00f135535416b20519fc7c7972090cd))
* remove unnecessary return for a better code ([474b57e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/474b57e5f5801b689994e9e0c130199ba4e5e8a9))

## [24.7.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.7.0%0D24.6.0) (2023-12-14)


### Features

* Add SDK version in the connection headers of all the Websocket API. ([8d334dc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8d334dc522fba269a8554f2b2ead076c2977271c))
* adjust concurrent record endpoint behavior ([fb17510](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fb17510df9330af5cc8f733921513e2ae2bd0a38))
* **analytics:** refactor predefinedEvent, adding base analytics api ([c8b490d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c8b490df3b863eade265c218a46ecdff9de8574e))
* **chatv2:** add more information in FSystemMessageNotifMessage ([d7a34e3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d7a34e3eff9aaf972b9441b0f53662d621475266))
* expose public bulk get content by sharecode ([5437a8c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5437a8c9baf8d5931392e2292cf81d23246e3bcd))
* expose public endpoint for updating content share code ([0575ea4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0575ea4520eb6607ed69c6c82f213e4ce5d0768d))
* Expose UGC ModifyContent and DeleteContent by shareCode ([1b2551a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1b2551a71827989913d384e164b8123181dba4b2))
* Exposed RetrieveUserThirdPartyPlatformToken ([2d6ccc9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2d6ccc96d8a814aca22b55a11d8e51a183f60601))

## [24.6.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.6.0%0D24.5.0) (2023-11-28)


### Features

* add deprecated message for UpdateUserV3 ([b8aa29c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b8aa29c82f090ed48c667bd6aa6544188713f8b9))
* add several predefined events for storage scope ([1f80ba7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1f80ba77e33d7302bc9f0c629a03546636a752f6))
* expose new search hidden content endpoints ([ad57dcb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ad57dcbc07c649b881164e1a4b77e7bb3be0f6b9))
* **IAM:** generate game namespace code for publisher usage ([76d9cde](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/76d9cdeb739ba13626bff5a7877b878dea346297))
* **OAuth:** add CreateHeadless for LoginWithDeviceId ([59ad4f4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/59ad4f449e018ac9392d2d4f4f0271460d752978))


### Bug Fixes

* fixed cache token for windows platform ([e526034](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e526034e987665689712862d8fc866c76361a268))
* **predefinedEvent:** change event namespace to game namespace ([21b8877](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/21b8877acc376a34368d6ba6696f0530c1c90bd2))
* **Switch:** compile error on switch platform ([5d4705a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5d4705a891a8b4c92cb2b9bcdc26c3ad537a8c07))

## [24.5.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.5.0%0D24.4.1) (2023-11-13)


### Features

*  Expose CheckUserAvailability ([56d8042](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/56d8042915676498024cd20c307353f2aa75338f))
* add message field in send chat response model ([b72ecdd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b72ecdde3f985e300b1cbb43caff34e81fe5c3dc))
* adjust logic RemoveEmptyFieldsFromJson to allows removal of a fields with a value of 0 ([e062559](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e0625593b11bc204ee97d72d2d2ff673f95985e0))
* **CloudSave:** add cloud save admin game record API ([ec7d671](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ec7d67149b6f85ac004055fff007a30088812f6f))
* making param guard for ServerEcommerce::FulfillUserItem ([ec73975](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ec73975fa1e41c52c6179f3346d5f8a22b21ad69))
* Parity model for FulfillUserItem ([ba412db](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ba412dbdcc08a340f73cc4282a842e179834f9ab))
* **predefinedEvent:** add events for chatv2 scope ([e0974e1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e0974e1343cfef1f81b5f5d208483f228168c0fb))
* **predefinedEvent:** add some models on Engagement Leaderboard, Seasonpass, and Reward scope ([78c974d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/78c974d8279d5168d2e351352b88960fa29cf2a0))
* **predefinedEvent:** disabling predefinedEventApi ([3daa8bb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3daa8bb73cd39c9f18aee94656cd126bfc818cb8))
* **qosm:** allow the game client to specify the qosm URL change to AMS-qosm using DefaultEngine.ini ([79a366f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/79a366fd3831414d93c078b017ef4b761da11e35))
* **UGC:** new V2 UGC endpoints ([367c96c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/367c96cffc569806bf0e1079049863d36855abb4))


### Bug Fixes

* compile error on PlayStation build ([b9271bb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b9271bbffd0eb4f3f09dff6bdc66161785477fd0))
* **Ecommerce:** add some validations inside the grant user entitlements ([7ade9d8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7ade9d82f6f385b5a3e0089b0491cd9bf2b4e106))
* fixed cache token ([5c102fc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5c102fc4f8b0c52ecea733685f110ec7e2bc9e3d))
* fixed cache token ([7e64590](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7e645903b05d0564d4401830bae90d27631a8035))
* move the function from OSS to the utilities function ([f43c40a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f43c40a11041f3ee38796eb6bd23673b8939c4a7))
* **predefinedEvent:** change event namespace to game namespace ([2a11b13](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2a11b13425d38b05b6f6f9a39585ce629e45716c))
* remove delegate capture by reference in lambda capture ([d94b3b8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d94b3b88848837e0ca8f398f03a175e14e9ba88f))
* **serverEcommerce:** remove availableDate param when the value is default ([1c415d6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1c415d6c2bfb75103a3b309f4a3378d3e710400d))

### [24.4.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.4.1%0D24.4.0) (2023-11-13)


### Bug Fixes

* add allowed user id list for official user id ([f100bf4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f100bf4537daa25a5be41186ad2f5fefb37ed7ee))

## [24.4.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.4.0%0D24.3.0) (2023-10-30)


### Features

* add Flight ID across API and Web Socket calls ([228ff16](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/228ff16053955866a48d9be239aaf520556140ea))
* add upd ping pong async task ([1438283](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/14382834d76fee2c0ec91d8b1fd1b5b920dd22a0))
* **lobby:** add FriendId in friend interaction response ([ee83b7f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ee83b7f061a2bbf8f32fb19dabb037f2f78af043))
* **MPv2:** add API to send DS session ready to session service ([3c1b9f5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3c1b9f58762f4fcb5965b323ad800a5d8003889d))
* **predefinedEvent:** add some models on Engagement Achievement scope ([48ddf23](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/48ddf23f4e62ae5a39841d1a9cf793b59c67ca38))
* **predefinedEvent:** add some models on Engagement Group scope ([62b8114](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/62b8114b23b26511771f0561070cc2c8d9212416))
* **predefinedEvent:** disabling predefinedEventApi ([faef12a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/faef12af98af555735ab01ca37bdbb82809ea605))
* **predefinedEvent:** update and add some models on Play scope ([a9f9afc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a9f9afc6c73868c40415c13794071da7df0645cc))
* **qosm:** allow the game client to specify the qosm URL change to AMS-qosm using DefaultEngine.ini ([98518cd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/98518cd16d602c62928d961a5d518c0f6f31e6e4))
* **QoS:** use udp ping pong to get latencies ([4f88f48](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4f88f486879d7ecfc7cce4fcc6d5b2fc4f55507a))


### Bug Fixes

* change presence broadcast event namespace from publisher namespace to session namespace ([2c77a9e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2c77a9e30338d999359ba3a9a1edf22c21157c9f))
* compile error on PlayStation build ([9da8846](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9da884688c0c4d9af343cc0db1d590858835acdd))
* enable update session with empty teams ([3e019cd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3e019cdcdb5fe93dea87ba1d74a36e86b0cfd7cc))
* fix clang error in linux build ([7bfade5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7bfade52ea7bec3873cb6832758cdf6c48e8d59b))
* fix preprocessor checks for TEMPORARY_ENABLE_COMPAT_CHECK macro ([ab89159](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ab891596cb7b2504444aefa035aa3f681b131029))
* given new/proper name for Sellable in FAccelByteModelsItemInfo model ([7c939fe](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7c939fee49800bb27142e51cac864a6fb4be521a))
* Incorrect include paths ([e3f48be](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e3f48bea73f4fe4febf7e26c18885b77ddb96d5e))
* **predefinedEvent:** change event namespace to game namespace ([8c164ae](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8c164aea89f9770d137e3933b17d7878b79a8257))


### Refactors

* **compatibilityMap:** temporarily disable compatibility map checking ([8d59de3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8d59de32da6d49f6ebf3b44d34e0bbb248a9d68f))
* validate user id before calling API ([c3c5afd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c3c5afdb2f683f35c849f8c22407dafb155904c8))

## [24.3.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.3.0%0D24.2.1) (2023-10-12)


### Features

* add flight id to additional login data ([bae39d8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bae39d8d4b278b59141785794caf13478280b9f0))
* exposed latest query user orders ([af10369](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/af103697965b2d7e0f392ad13e68d650f6126d8d))
* Flexible bundle pricing ([702a448](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/702a44812f924df7c8197afe6265c45fbdb9451f))
* platform wrapper interface ([3df2503](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3df2503221eed0856b22b7ce9ff035cd8bc84895))
* **predefinedEvent:** add CampaignCodeRedeemedPayload model, add payment payload ([307ab85](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/307ab8509f2f8b00249b6067fe5c220cdf7518fa))
* **predefinedEvent:** add some event on social scope ([e675d67](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e675d670236a56a43b9ef79bd9d5c2f2a5fa0e0e))
* **predefinedEvent:** update and add some models on Play scope ([428da23](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/428da23b1434c954970e80cac38589998aea334f))


### Bug Fixes

* capitalization for itemId in FAccelByteModelsPriceDetail ([f811c51](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f811c5157871dde78e75ea15706fdc7576e768ae))
* compile error on UE5.1 ([1d5d329](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1d5d32988664a556f412ac899ad7e7e9a6a0f5d3))
* compile error on UE5.1 ([59ca43e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/59ca43ed823383ef12ca4b8633b09b3c435c4482))
* compile warnings when using monolithic header ([7567275](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7567275c08c3cebd61cff1b0cce2a3d620955f72))
* custom json policy not working on UE5 ([6767794](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6767794e9209e39a0b9c567e8a94d7aad0d35077))
* custom json policy not working on UE5 ([98ddda8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/98ddda88dbcc6ec1a144fa8107a71cbcb129d5e8))
* incorrect signature for the parameters used in the lambda function that causes compile errors ([6909d79](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6909d791cfe07f24ffd480067b80a008c762c58f))
* **lobby:** add missing platform field in FAccelByteModelsUserStatusNotif and FAccelByteModelsGetOnlineUsersResponse ([aa8707c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/aa8707c3bbe762458f4cdbd8c38b1106fe027121))
* **lobby:** ignore empty friendsId for BulkFriendRequest and don't send the request ([2193fc7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2193fc7e1b7a56b0d38e45e43861c1a9e2429cee))
* missing Base64 header in the Lobby Api ([0cf2527](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0cf2527e0b7aaf496af6cb9ce86f933759e45c6a))
* **mpv2:** add missing notification handler for OnSessionEnded ([afab9c9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/afab9c94ace245324c9b67a5323a01dfed23ec9a))
* **predefinedEvent:** remove UPROPERTY's params so that it won't shown on blueprint ([a4ff71a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a4ff71a4f8446f2b4d75b72efb13480d876f46fb))
* **UE:** allow compilation on UE5.2 ([7ef6b04](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7ef6b04012e0c2574342d92af465b8b5642ccc91))
* **UE:** allow compilation on UE5.2 ([0ee9b11](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0ee9b11e399a61facacdc7c3388aa8a858c18fe9))
* **UGC:** sync the variables for UGC response ([d6b85de](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d6b85de0c50ccc14c880ad6032f9afdd128cefd3))

### [24.2.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.2.1%0D24.2.0) (2023-09-27)


### Bug Fixes

* command line parser will have wrong value when input key contains other param key ([951facd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/951facde106f02553893d3384c50fabf451bfc67))

## [24.2.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.2.0%0D24.1.1) (2023-09-25)


### Features

* **QoS:** send multiple ping and calculate average round trip ([c04fe54](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c04fe548d31539c50e6900eed5140840b08a5237))
* **telemetry:** bringback caching improvement ([a614f50](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a614f50273456ee0d7e01cb1abae165acc04bc99))
* **Telemetry:** make some function protected to allow access by child class ([e48a2b1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e48a2b1077a6742322f0d34b1fc91904393730be))


### Bug Fixes

* **lobby:** add missing platform field in FAccelByteModelsUserStatusNotif and FAccelByteModelsGetOnlineUsersResponse ([1ac6792](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1ac6792675dfc15e1af14e4168843e69d14bb7e8))
* **predefinedEvent:** adjust predefinedEvent models on storage scope ([e49b4a1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e49b4a14f8b54029b9fc67273248636b5cf6d735))

### [24.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.1.1%0D24.1.0) (2023-09-15)


### Bug Fixes

* DefaultGame.ini is altered after build ([3ccdea0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3ccdea0b761da738940f8809ab6cc541194e10a9))

## [24.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.1.0%0D24.0.3) (2023-09-11)


### Features

* add predefinedEvent api ([6613202](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6613202bb854e030fd8a4bf836bb6d1e75430c47))
* **BinaryCloudSave:** added public binary cloud save APIs ([1a02de6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1a02de6581d161c36f9d9c12be529354838edeb1))
* check user id validity before calling API ([99aec64](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/99aec64fdb94cf79a555c0a5d2279a7f8c9afad3))
* **MMv1:** add Region in MMv1 matchmaking notice ([7ca8036](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7ca8036d8c5b3a4234659ae53aa40d40721750f4))
* **MPv2:** add new create matchmaking ticket with more info on error and deprecate the old one ([6ee106a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6ee106af4aaf878a2e687fde6565ade2d5bb98ba))
* **predefinedEvent:** add bool to enable/disable caching, add event timestamp as parameter ([18341fe](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/18341fef3ba298b91e866d5ed6abcc502f796bde))
* **predefinedEvent:** add payload with TelemetryBody, add send method with telemetryBody ([7a1aa89](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7a1aa89473830a6b0cfb23e4240b5501adc4c21c))
* **predefinedEvent:** change to clientTimestamp ([a24d826](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a24d826c834637902c4ab396c7fbc6d2e249c487))
* **Session:** add update session storage methods ([8b07b68](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8b07b6889f4e281c4db2e143a44e3e5d84b06e46))


### Bug Fixes

* **AMS:** default AMS config is empty ([ff2d2da](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ff2d2daae409ab2819c7edb29e77bd73bd381518))
* deprecation warning for IWYU support in Unreal Engine 5.2 ([9b336ff](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9b336fffa3eebbf4da6025da13f448a1f8540394))
* missing Category in UProperty causing an explicit category specifier issue ([36d0001](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/36d00017a061a1a55fbcf6ecff4c50b800c03d25))
* **predefinedEvent:** change UserAgreement_Rejected to UserAgreement_NotAccepted, change UserProfile payload from TArray<FString> to FJsonObjectWrapper ([6e6a3c5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6e6a3c530d1c71f7ed646cce7449daaaaa8a698e))
* **Statistic:** Adjust newest flow to expose which statistic is being updated ([a24a10f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a24a10f79be1ca5bee5940da5f1494bf385c1658))

### [24.0.3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.0.3%0D24.0.2) (2023-09-08)


### Bug Fixes

* deprecation warning for IWYU support in Unreal Engine 5.2 ([407f29b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/407f29b42e424769d8198f0b270b49097746b23b))

### [24.0.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.0.2%0D24.0.1) (2023-09-06)


### Bug Fixes

* **AMS:** DS not connecting to amssim when launched using editor command line ([6fdf909](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6fdf9092cfed1f940eab87c4362c4dab9c2ec6e9))
* **AMS:** settings are not loaded on module startup ([309b213](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/309b2138dec27ae4a3fdded27c75eaa2d7abee0e))

### [24.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.0.1%0D24.0.0) (2023-09-04)


### Bug Fixes

* unable to connect to lobby when logged in via 3rd party platform ([985ddfc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/985ddfc9627e5bd6f4c5b570650c80c88144dc7f))

## [24.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/24.0.0%0D23.7.1) (2023-08-28)


### ⚠ BREAKING CHANGES

* parsing behavior has been improved and handle quotation mark + backslash

### Features

* add lobby error code for max friend limit reached ([b4b25c8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b4b25c890a3456b334853dbaba113dd2316572c1))
* check user id validity before calling API ([5405de2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5405de22018f65f33ceeb9903bce71a7c7bcee91))
* Expose Fulfill Rewards ([ff1316e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ff1316e6dfdc9e4061d7c7171b884903198363e6))
* exposing missing psn sync entitlement multiple service ([a045dbc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a045dbcfd52727049d7c1875efcaba4b9b2bac09))
* **lobby:** send platform name to lobby when connecting ([e1e5546](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e1e5546166c99af1f4b6618521cd4483d599d69d))
* **MMv1:** add Region in MMv1 matchmaking notice ([df7a1a3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/df7a1a3b89b038aff9ab59b85b461c6f62c366ba))
* SetUserPresenceStatus support JSON ([d542a29](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d542a29a77288253784e4eb86fe25ca7674d4e5c))
* **telemetry:** add clientTimestamp ([8f25c0e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8f25c0e277cc9a993f3a49dbdd793f3b78097e57))
* **UGC:** add a new sort by value and devide liked content endpoint ([583e0a7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/583e0a73b531f225c8fc3018e2c2862aacb2f449))


### Bug Fixes

* change I64d to lli ([352be36](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/352be3699952b96fe065e51a10279afd6bc627b6))
* **Leaderboard:** Sync the leftover variables for get user rank endpoint ([fef82d0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fef82d0884bdc428c4b3b552c1fe1fb10f32a99d))
* lobby not triggering ConnectionClosed after user disconnected because of login ban. ([2c78a05](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2c78a0555c09ba76c7e253a942e579c0224b17fe))
* Prebuild script failed when path contain spaces ([b9988d1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b9988d13ae04026f8d604e7064b6bfd0858ff295))
* remove 'using namespace' in public header ([0ad5094](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0ad50942d3d24163b248e67e43e3b32057e519a6))
* **Statistic:** Adjust newest flow to expose which statistic is being updated ([3867dde](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3867dde44f29434a784e9b56cb2dbf294cbdb73e))
* **switch:** unable to read/write cache file ([1feee5e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1feee5efb62e594c37048c20e8517a3db4ac13c0))


### Refactors

* move IsValidUrl to AccelByte Net Utilities ([f91f18e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f91f18e8049e55d8b36eab2013eb6e18d284b067))

### [23.7.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.7.1%0D23.7.0) (2023-08-23)


### Bug Fixes

* Prebuild script failed when project path contain spaces ([0454259](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/04542596a3adf305e9ecaff97a4401e94f186fd3))

## [23.7.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.7.0%0D23.6.1) (2023-08-14)


### Features

* bring back oculus iap sync ([92b2eef](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/92b2eefdf35ea9b96c631ef7f155b7e29a7adeb0))
* **config:** omit server ClientId and ClientSecret from configuration file on shipping build ([5c2959c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5c2959cf78b34f1b9be4e4d1880ffe0a90e66a20))
* **launcher:** read command line arguments for Justice Auth Code ([b586a69](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b586a69fcad9959ec8aeea540ff8249f530d255f))
* **telemetry:** add clientTimestamp ([511a4eb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/511a4eb5eab3bd7801d3ec880d4efb7b5b2ac6e2))
* **UGC:** add a new sort by value and devide liked content endpoint ([ac14217](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ac14217f4b5acc188cde2e20794f8a741356aaa4))


### Bug Fixes

* **core:** add scopelock on availableToken to handle multi thread requests ([0e4202c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0e4202c3714f7879a833eb9aafd64bfdcf2a99c9))
* lobby not triggering ConnectionClosed after user disconnected because of login ban. ([536c5ac](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/536c5ac5365b312b4eca3dc98e3bb6598d511d4c))
* Prebuild script failed when path contain spaces ([ee7241c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ee7241c0fc9f22630852d6519afaadc0f24be548))
* removed encoding process in ConvertItemSortByToString utilites fucntion ([9358317](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9358317d6558ff5522542b075bc5417e5d0eba85))

### [23.6.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.6.1%0D23.6.0) (2023-08-03)


### Features

* **MPv2:** add teams setting model and auto join flag ([2f086d3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2f086d36581ab5c5a45d00a8c25883bb966f7b54))

## [23.6.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.6.0%0D23.5.2) (2023-07-31)


### Features

* Add GetMyMatchTickets API ([488cf7f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/488cf7fa3d6bc929ac496566c427eabfa7d44273))
* add raw platform user id support ([ea50a94](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ea50a945b8638cf333f29e04b38b1fbef034fb8d))
* add UnlinkOtherPlatform and UnlinkAllPlatform into blueprint ([a69e344](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a69e344e3250572116f6c81603c7789f9d7286f7))
* bring back oculus iap sync ([2e55f8a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2e55f8a2dd38e0535ebef9a78d950d6f42c07ced))
* Expose Oculus IAP Sync Endpoint ([d097105](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d09710527a2657d22368e9cf3a119f15c1593eca))
* **Lobby:** Add incoming and outgoing friend request with timestamp ([f857aac](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f857aac300224e7183d1b8b9d089e44176761dd1))
* **MPv2:** add accept backfill proposal API with game session info and deprecate the old one ([839de09](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/839de09716a0a3cc1ef9eb3813f592aa5991dc2e))
* revert oculus sync iap changes, will be a hotfix at justice 3.52 ([08f7b48](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/08f7b48cccfbe085336e194872d61672ce34edd0))


### Bug Fixes

* initialize USTRUCT member to default value ([7eb6745](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7eb67458fd8fc77465b1395e8ce95e0155f78de9))
* **Inventory:** Revert all inventory endpoints since BE isn't ready ([d0e92d0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d0e92d0b45fa7748dad2f8ce7e72d8c49987646b))
* trigger on connection close delegate when failed reconnecting to websocket ([8d642cb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8d642cbafd4cf3ca739b8a20d22e06b4dad20081))
* **user:** add missing fields in FAccountUserData ([d845c29](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d845c297dfb49d1d7d205ea48e6a0f6889ad58ca))

### [23.5.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.5.2%0D23.5.1) (2023-07-25)


### Features

* **launcher:** read command line arguments for Justice Auth Code ([2bd030f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2bd030f4584d317014f63ae6a6149a52de7efebc))

### [23.5.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.5.1%0D23.5.0) (2023-07-20)


### Bug Fixes

* **AMS:** launch param cannot be read properly ([786170e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/786170e5e05e64512c92d8b39f130a3b7aff5393))
* **chatV2:** update websocket chat session id after connected ([c83f2c2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c83f2c2eb4e822bd54a3db5bff8c2f4f781c6933))
* removed oculus iap sync API function due to incompatibility with AB service. ([10e5529](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/10e5529100166be7e62c24b7b69227a9fe477e07))

## [23.5.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.5.0%0D23.4.2) (2023-07-17)


### Features

* add custom print policy to fix locale issue ([48fd989](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/48fd989e20ff8965efa939aafb77773a2837958e))
* add raw platform user id support ([34d6bf2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/34d6bf22685f52a352b3464843d83efda65ca8cb))
* add UnlinkOtherPlatform and UnlinkAllPlatform into blueprint ([0b52c0a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0b52c0a023bbfc9400f59f4af471e5722ccb3bd6))
* **Error:** add sessionJoinFull error code ([95f3d8a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/95f3d8aecd52b19bd408fa4fb0ff5634532b5387))
* redirect readme to docs portal ([5273f6f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5273f6fab3be71838481129cef45d9bd0c1273c1))


### Bug Fixes

* initialize USTRUCT member to default value ([3ecf41e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3ecf41ede16242ef39da4e18b43b86c3bc8c8161))

### [23.4.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.4.2%0D23.4.1) (2023-07-14)


### Bug Fixes

* added sync IAP & DLC for oculus platform ([3ac6daa](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3ac6daafe85c0b0aff4421619b2657d61d7857fb))

### [23.4.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.4.1%0D23.4.0) (2023-07-05)


### Bug Fixes

* error when building Linux Editor ([bbe6f73](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bbe6f734b21b735020f7e4eef55069a3e45dc23a))
* Separate UnlinkOtherPlatform API functions for improved functionality and bug fixes ([e1ac377](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e1ac377ca97d0102fa2ae4df0cdd0378c8d6fb7b))

## [23.4.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.4.0%0D23.3.1) (2023-07-03)


### Features

* add GDPR Api to SDK ([4174ee9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4174ee95c98914948850395f88a0d8f7776878b5))
* Add missing fields in the DSM models ([987edb4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/987edb4116ca3f5904d7c45bb4c3cef108762299))
* add missing header in LinkOtherPlatformId ([7a12eb0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7a12eb0aeb001046d907d4cd0c1688979f5b02bd))
* add session code management for DS ([69d44e0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/69d44e0530c68dac97d2d624a76b125cb34cecba))
* Add Username in the Credentials class ([b5e955c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b5e955c9bde59b379db523ca73c3e33a2ba8d7ce))
* Add validity flag in the QoS API class to ensure the object is still valid when OnLoginSuccess triggered ([da08aa2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/da08aa2689cec87ef46b2bca11b027d1c47cea63))
* **DSHub:** add session member changed notif delegate ([98ebd81](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/98ebd81f2301c78a776543b51f28143d3538046a))
* extend Environment settings by adding sandbox, integration and qa ([6a55ce3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6a55ce35e3a06edd5860a25262cde513cf0c5488))
* implement presence boardcast event ([feec513](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/feec5130c5158056fc414343e6bac3bc0e6691b6))
* Logs squelching to hide some information in the HTTP request and response by adding the new HTTP header ([072cb54](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/072cb54be63391c01773f526c556c1ca9c81f29e))


### Bug Fixes

* change cache qos get latencies to get active latencies ([7dbe7e4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7dbe7e401e4cf223cec6d29a4210859768ae5f1e))
* **httpclient:** Fix undefined Content-Type and Accept headers in the HTTP request ([2c8591b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2c8591bb66eb47567c43cd5d59f374bb1bddcb23))
* inconsistent string name on GDPRApi Header ([4b3823b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4b3823b65c8cac762285a0e01d5d203a181d0905))


### Refactors

* **httpcache:** Change CachedItems initialization process as lazy initialization ([4e636b6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4e636b60fea28a1fd33397dda29327e801ae124c))


### Performance

* Use reference to iterate switches in the ServerDSM API class ([461610e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/461610edc75dd9aa874188a5a130475348ad7902))

### [23.3.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.3.1%0D23.3.0) (2023-06-23)


### Features

* Add game session code management endpoints ([d520d76](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d520d767696140cf7650ea3998a7fe2663339d36))


### Refactors

* Implement ApiRequest with TSharedRef<FJsonObject> json parameter ([4e69a6f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4e69a6fb9b608287e635428077f1d90983a848cc))

## [23.3.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.3.0%0D23.2.1) (2023-06-19)


### Features

* add AccelByte-specific storage file used in some features ([e8b1833](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e8b1833490268d0b75fd506e35dac8e6be2bc159))
* add missing header in LinkOtherPlatformId ([5edf70b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5edf70ba9ab47237a7ce6f1478300d6da86ac316))
* cache generated device id ([1fd182d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1fd182df246f268c310fe1dcde1605ea02ba1f55))
* **chat:** add event for muted and unmuted ([5887a9f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5887a9f7a4b88972b932117575c443248b208197))
* **chat:** add group chat moderator api ([6602d4f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6602d4f89471b77ada32ffbd1799e48eb625daa4))
* **credentials:** change refresh token retry flow ([48ac904](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/48ac9044e02cb51696ce66f812697e7104f203e9))
* **MPv2:** add error code for party and game session not found ([cb8b906](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/cb8b906f1014af34873ae6978ec4195ae13921b4))
* new statistic endpoints ([4b82172](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4b821729f29fc4c89130c847f289c69749947b1b))
* OIDC Unlink Features ([dd03101](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/dd031010061abe155efc52a0a072597d3540bbb2))
* **session:** add promote game session leader API for client and DS ([311099d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/311099db6005a9ef0b18df8934d26af342ae96c4))


### Bug Fixes

* **Chat:** add missing header for Chat API ([2ba536b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2ba536b07d9f52874c9559eb7567bf5930cc7c65))
* **log:** fix string format ([c9e6104](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c9e61042c3a817e208ebfde2d3b8414c1280bc01))


### Refactors

* **AMS:** change ServerWatchdog to ServerAMS ([1880bcc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1880bccb25a7ae6f22912b94fc02ec2b9d3c012f))
* **httpclient:** change query params to be TMultiMap ([0bec559](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0bec55932cd2f6f11bb68a96335410fe6e236d10))

### [23.2.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.2.1%0D23.2.0) (2023-06-05)


### Features

* **UGC:** add a new optional channel name parameter to query a specific channel ([28fe02c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/28fe02c724bc92444d62b9697bd327cb766a8b90))


### Refactors

* **HttpClient:** use Request instead of ApiRequest method for specified API function without authorization header check ([a655f0b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a655f0bfe27bcd4eef69588644d6e8e2a0550c21))

## [23.2.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.2.0%0D23.1.4) (2023-06-05)


### Features

* **DS:** add DSM heartbeat API ([0701ed3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0701ed30cbf2e8b2ecca93672fd75f39a42fe629))
* **session:** add query game sessions API for dedicated server ([262c8a9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/262c8a933964e7ca2e3d1eee55626046b2aaf3bb))

### [23.1.4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.1.4%0D23.1.3) (2023-05-26)


### Features

* **DS:** add DSM heartbeat API ([eb71bc8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/eb71bc8160d9c1a5a1f64a71a88de2c4b234903b))

### [23.1.3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.1.3%0D23.1.2) (2023-05-22)

### [23.1.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.1.2%0D23.1.1) (2023-05-17)


### Bug Fixes

* **UGC:** change the content update boolean to false by default ([36914b6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/36914b6c0575815ef9db954c667e92e44bf5a5f2))

### [23.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.1.1%0D23.1.0) (2023-05-09)


### Bug Fixes

* missing include headers when the project doesn't use shared precompiled headers ([bdbbf78](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bdbbf78a1fa67ee8bbc5768a47e61262532de72b))

## [23.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.1.0%0D23.0.1) (2023-05-08)


### Features

* **HttpClient:** change regex port to have 5 digits ([0b06db5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0b06db56fb004232e3ba6ce47f26bdde3acefb5b))
* leaderboard revamp ([8e0bcf4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8e0bcf426f401e51d19bb44ba207e8f126a7b5b7))
* **lobby:** Add Reconnecting delegate ([2332f3f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2332f3f69ef162e63d48b195af983bd095d31c7a))
* **presence:** limiting bulk User presence to 40 ([10da775](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/10da77528616863e647052f75a517f68e617924c))

### [23.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.0.1%0D23.0.0) (2023-05-04)


### Bug Fixes

* **lobby:** add missing data members in FAccelByteModelsUsersPresenceNotice ([93f39bd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/93f39bd4de61f1d51b15426bdbb8590f8ba23715))

## [23.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/23.0.0%0D22.0.4) (2023-04-26)


### ⚠ BREAKING CHANGES

* namespace restructurization

### Features

* add new field ([293ad19](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/293ad1997074fe409e9ac3753898cdefc9194c65))
* add SIGTERM catch for kill signal ([d77f512](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d77f512e4faa26049ecefac6ed9c17e7d995eeaf))
* add uparam macro so the parameter will be treated as input instead of output. ([a1a1c80](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a1a1c805593c37203442270ea27d3b00fce1db88))
* **MMv2:** add get matchmaking metrics endpoint and queueTime in create match ticket response ([d20457c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d20457c2e47520c982106f907531139377aaa46c))
* support build without PCHs ([72c3c13](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/72c3c13256781ce5023f3df35b84af487198942f))


### Bug Fixes

* **chat:** add missing header file when compiling oss ([aa2f36b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/aa2f36bdcce27f69580d165a159cd6346935ae63))
* remove using namespace accelbyte from header files ([7bcfe8f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7bcfe8f1a58773af666b85b45750a389b0ce1315))
* resolve conflict 22.0.2 to master branch ([ec19ba3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ec19ba372812b24d6ea81bf9743dca586ae44f02))


### Refactors

* **MMv2:** change error for invalid MM request to be ErrorCodes::InvalidRequest instaed of 400 ([01bcdfa](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/01bcdfae0a8f700499bc61ce5631af8a6059fe67))

### [22.0.4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/22.0.4%0D22.0.3) (2023-04-10)


### Bug Fixes

* resolve conflict 22.0.2 to release candidate branch ([3cc1c19](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3cc1c197f61baf51791fbbb7baf89382cf28a1c4))

### [22.0.3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/22.0.3%0D22.0.2) (2023-04-03)


### Bug Fixes

* **openssl:** missing include in openSSL ([abc0990](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/abc0990b44f7a1f0acfaa597e116745786992127))
* **statistic:** deprecation warning for ANY_PACKAGE in Unreal Engine 5.1 ([6549396](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6549396e1304af4c4b3c70d1ee440c3449e68f9a))

### [22.0.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/22.0.2%0D22.0.1) (2023-03-29)

### [22.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/22.0.1%0D22.0.0) (2023-03-29)


### Features

* add public statistic cycle endpoint ([372b4d8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/372b4d85c60a4f0995b110ced0414653f417abd8))
* **chat:** add get system message stats api ([5a62f1b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5a62f1b9f6e4308ca7ad77a1c2a28709ea78c34e))
* **DS:** sdk emits StatsD metrics ([dda9b7a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/dda9b7a7c9d77f331fff134c4e93b31f3a2bb58a))


### Bug Fixes

* **blueprint:** add mising login & credential function ([4bc3448](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4bc3448930842231d1affba7d18be366110f538d))
* **lobby:** keep lobby connected if refresh token failed and disconnect when token expired ([c73639d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c73639ddc960e55361f2006ae33bf27d9a552091))
* **UGC:** UGC missing CustomAttribute field assignment ([d90bbe5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d90bbe5ef5ecb624ea4d9e0797850916945dc500))

## [22.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/22.0.0%0D21.3.2) (2023-03-27)


### ⚠ BREAKING CHANGES

* **chat:** Optional bool enum members changed
* **session:** changed OnSuccess result of ServerSession::QueryPartySessions from FAccelByteModelsV2QueryPartiesResponse to FAccelByteModelsV2PaginatedPartyQueryResult

### Features

* **lobby:** lobby errorNotif will trigger response delegate when there is a request associated with it. ([1704af4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1704af409d9c8694803334bd9458247f73d9d36e))
* **session:** use paginated model in server query party sessions ([2ec636f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2ec636ff526b7abc9c09bc132043c69e532e590a))
* **UserProfile:** add bulk get user profile with array param ([06e53e5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/06e53e5d27da0ffea1997105a8113ea0f4fb09ca))


### Bug Fixes

* add missing blueprint function for lobby, party and chat v1 ([8435359](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/843535961dedfd68b98d9f9eda12d09530c40e9c))
* **chat:** refactor optional bool model to avoid compile error in mac ([afd4d50](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/afd4d5000f92e7ccfcbfdd0283fed5af7ad0227b))
* **HttpCache:** configurable HTTP caching mode (MEMORY & STORAGE) ([47041e6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/47041e662649a2dd23022ccd33750eefd6c3e9db))
* **HTTPCache:** move the free cache to the SDK module shutdown ([b24e036](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b24e036449933253c9900bae9d915b823fa3d5b3))
* **session:** keep previous ServerSession::QueryPartySessions to avoid breaking change ([002c0ad](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/002c0adef6879ba4ce0ffc74f68874f175d7ca7c))

### [21.3.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.3.2%0D21.3.1) (2023-03-17)


### Features

* **error:** add WS error code list based on RFC ([f3a54fe](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f3a54fe5495813b11eb7935b10e5526cb9f63c26))

### [21.3.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.3.1%0D21.3.0) (2023-03-15)


### Bug Fixes

* add missing blueprint function for lobby, party and chat v1 ([a786597](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a7865972d9935eb7c3f13d7f5746089449c98664))

## [21.3.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.3.0%0D21.2.0) (2023-03-13)


### Features

* **chat:** implement system inbox message apis ([b565030](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b565030dac5851cba0df879a7a9b6b20a7cc4eb2))
* **deviceID:** encode the mac address & override in development phase ([93bb689](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/93bb6898aa8c67f3b6be6016eae85eafe35f12bd))
* **MPv2:** add error codes for MMv2 ticket conflict and not found ([9e9903c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9e9903cd81f0be014e7d01a288ea3fe51a37ce51))
* **UserProfile:** always remove empty field from update request to prevent validation issue if the field left empty ([03b9ac5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/03b9ac52bf76ebb9ee32f09603b933eaf8ddd59a))


### Bug Fixes

* DS crash when sending message to watchdog ([bd02e28](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/bd02e283abb403c5be0b35df99e025c87e3a5adb))
* **MPv2:** fix joinType not removed from request when EMPTY, and fix request capitalization changes by unreal engine. ([15e7b9c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/15e7b9c229c32904ab0c60fd178416cd0272a74d))

## [21.2.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.2.0%0D21.1.1) (2023-02-28)


### Features

* **DS:** add watchdog API ([87fee12](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/87fee12240de08ced41e1c7bcf0d84c55cdfc2a6))
* **MMv2:** add API calls to manually queue for backfill ([921f39b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/921f39b1b80a5da34e005ee33430a6cd08831575))
* query user achievement by user id ([9e062c7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9e062c7a987b92d39106287f151e4ad5abe7a60c))

### [21.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.1.1%0D21.1.0) (2023-02-16)


### Features

* **MMv2:** add API calls to manually queue for backfill ([ec972b6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ec972b607b6871fc8c60d6314f2b4b587a7c935d))

## [21.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.1.0%0D21.0.3) (2023-02-13)


### Features

* add limit and offset parameter to GetAllUserStatItems and GetUserStatItems ([625048e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/625048e5c1427fcab32dd7bf5385c8bc104a027e))
* get game token by publisher token ([95b5d55](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/95b5d5527e56aeeff3e7a8b1ab05704479079426))
* **Group:** Add Group V2 ([36125a1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/36125a1a2869954a739c86b98dd2a4f55ba3d369))
* Remove some Stadia enumeration variable, because Google is shutting down Stadia. ([df94838](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/df94838255bae0f7df4fe6f3e2507353e8addc1d))
* **ugc:** additional custom attributes ([a79b6c2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a79b6c2ea8137678a7102980f2271ce18c87817b))


### Bug Fixes

* **MMv2:** fix validation for ticket ID and match pool not aborting requests ([eccdc98](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/eccdc985da79ba61de9292a0fc7e7763ee8717ee))

### [21.0.3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.0.3%0D21.0.2) (2023-02-06)


### Bug Fixes

* **LRUCache:** remove cache cleanup & it duplicates with the one from HTTP retry scheduler ([69d608a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/69d608a96a175f8960e6489f8f3c8b9086796ea8))

### [21.0.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.0.2%0D21.0.1) (2023-02-01)


### Bug Fixes

* **SessionBrowser:** set default create game session current_player to 0 ([83d385e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/83d385e6e53a2cbd436e42b16403bd350d938037))

### [21.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.0.1%0D21.0.0) (2023-01-31)


### Bug Fixes

* missing AES_BLOCK_SIZE when compiled in UE 5.1 ([88671f5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/88671f522d5cf801191cbc10c0f70b43c851abf6))

## [21.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/21.0.0%0D20.5.0) (2023-01-30)


### ⚠ BREAKING CHANGES

* **OAuth:** Error handler signature changed and IamUrl changed as optional
* The existing var type (TMap<FString, FJsonObjectWrapper> CustomAttributes{};) in a model (FAccelByteModelsPublicUserProfileInfo) is not compatible with server response, we need to adjust it (to FJsonObjectWrapper CustomAttributes{};) to appropriate with server response and also uniformity JsonObject SDK parsing model.
* adjusted JsonObject variable type in FAccelByteModelsPublicUserProfileInfo model to able give json value and uniformity
* adjusted variable type in a model to able give json value and  uniformity

### Features

* add response validation for Login APIs ([8bbad85](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8bbad857075b878c44293bcfc5801cdd3feee1cd))
* **engagement:** add blueprint function ([eb07175](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/eb0717595026f8e5d6fd9a51702cd94c02052ead))
* **engagement:** implement new endpoint bulk get user records and game records ([806557e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/806557e3115f43fab25db78f8fd0d5c805333e40))
* group api v2 ([c0c2086](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c0c20861891de0680fb39c96c192be4ace4a0158))
* handle Third Party Login and Banned Users and update other error login method related ([d10e698](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d10e6986d69d3807d4744a0f5767e80bcff946a0))
* Implement get party info from the session service ([ef10432](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ef1043290d7f2e30c14449bf4773e94e8540fdc1))
* implement HTTP Requests guard to prevent DDoS-ing ([0acc975](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0acc9752128e293ce92889bf6bfeae484e626e94))
* **module:** Change ABCredentials class to public ([2934c41](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2934c4164aa25e1f414e848323ee03d428b2fa61))
* provided CustomError for LoginWithDeviceId ([23279aa](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/23279aa6b636dded12620e74e6a80c75aa67900b))
* **session:** add parties field in FAccelByteModelsV2GameSessionTeam ([aa4510b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/aa4510b649e324d25f74423a9444ff42f2752e4e))
* **storage:** data storage for shipping build windows ([8857f96](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8857f961500b940f75fc1cfda999d418d14c802a))
* **turnmanager:** add api to send metric to turn manager ([53b9d3d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/53b9d3d39b550daa3e02b8090e48c7d819145279))


### Bug Fixes

* error when parsing unidentified enum in http result handler ([f61adfa](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f61adfa5d1a4d1c634cab7c2268323dc644c843c))
* fix missing includes ([6c8672b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6c8672b801051bc24fd3907317938cc9ad1467c0))
* Fixing build errors from merge with 5c5adf8 - feat: add ApiRequest methods with Headers in FHttpClient ([536626d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/536626dba0f5e5aac025f80d552b958389ddafda))
* Json case sensitive error ([d41748a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d41748a12f999e4f197b9b96d1c2d4225c44fe8c))
* missing includes cause compile errors when unity build is disabled ([ee7adde](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ee7adde3298ebca13d1445a69218906dd3a41ce6))
* missing includes in OAuth2 API which causes compile errors when unity build is set to false ([f7af417](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f7af417ee5dee81dff99b6f17c10566777c509d9))
* updating HttpClient.ApiReqest to HttpClient.Request ([ca6dea1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ca6dea1d7b534ec4c7b1a161a4ac64283f9d16c6))
* updating HttpClient.Request calls to HttpClient.ApiRequest ([8030d48](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8030d489061f0f187ae2f3283e8651b93afb7be9))


### Documentations

* Changed parameter comment mistake ([3fe4152](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3fe4152e193d033dca85d1cd8758dbdb4827ee09))


* Merge branch 'master' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-8327-logindeviceid-customerror ([3ed6fd7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3ed6fd76cbf656c445d5baa8436ab3e95a0a25cd))
* Merge branch 'master' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-8327-logindeviceid-customerror ([7cc3672](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7cc367259229e5c584d1e26319954327dc443733))
* Merge branch 'master' into feature/OS-8327-logindeviceid-customerror ([1022ad4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1022ad40f4f7019c9f883c01ea1c45b5b0593712))


### Refactors

* **OAuth:** Change CustomErrorHandler with OAuthErrorHandler to handle OAuth error specifically ([3e0d2d5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3e0d2d5d050f45d86ff36e36d79a8807f38639af))

## [20.5.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/20.5.0%0D20.4.0) (2023-01-16)


### Features

* achievement query using tag and new get public tag endpoint ([2ac2d8e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2ac2d8e8a3c6c6a9ded9187b836efd42c9604c33))
* add ApiRequest methods with Headers in FHttpClient ([5c5adf8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5c5adf8f8b228503f7a2661a91b4a073d4e7022a))
* Add validation in the HTTP API calls to check empty auth token and invalid url ([725d3f3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/725d3f372c9273d53be2864a984c9e9fe8bfc76c))
* **cache:** HTTP threadsafe ([7cf0270](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7cf0270c03d7ef8ff4d80fed92ecff6ffa6451c0))
* **HTTP:** result handler parsing request/response from cache ([dd3705b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/dd3705bd7ce700d414c94432316f1afca6e8724a))
* **lobby:** update all Lobby responses to inherit LobbyBaseResponse class ([d951b13](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d951b13c0e64c9138846949b8320f9260d15843f))
* **Reporting:** add chat reporting ([626a447](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/626a44765784b9ce748c770f5ce89e358d6cb2d3))
* **session:** add player and DS status V2 ([80e4c2a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/80e4c2a6dacda41e811a74aeae85b685f04aad78))


### Bug Fixes

* empty values in required configurations only shown as warnings ([641e62d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/641e62d3147d1bc6300601a91672370e1648cd59))
* failed GetCountryFromIP API because empty Bearer authorization header ([77224c4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/77224c4056ba98935859a5c9da47bedd6f21bc2a))
* **SessionBrowser:** add missing query param in ServerSessionBrowser::GetGameSessions ([ad65469](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ad6546934b0a632127470d5653776ec9301f2f62))

## [20.4.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/20.4.0%0D20.3.0) (2023-01-03)


### Features

* add handler for unidentified enum values in Json ([0ebfc58](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0ebfc583a19da5c16e88fc41e886bf9b8150a429))
* add userId to addToTopic and removeFromTopic event ([82cfecf](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/82cfecfee20da0c0e87e55c4f0534d9ee387d6b9))
* ecommerce display & section (Rotation Shop Items) ([57c6ccc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/57c6ccc3a5c63522c579c995e6d91f83dc9bf484))
* facilitate pagination param (offset and limit) for GetAllUserStatItems ([830fbf6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/830fbf650d009f38977818ae35ec346a5ea16472))
* **lobby:** add away availability in lobby presence ([efa9833](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/efa9833ad92d64824b292bcef71f2c899316369c))
* **lobby:** use custom json object string converter to handle unidentified enum ([7107ec9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7107ec9f28a1127d2c0c36d1c759e4209ea4d3b3))
* **matchmaking:** implement MMR Rebalance endpoint in server matchmaking ([5bc95de](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5bc95de3260f5af271c7eee1818950380f6b7517))
* **Statistic:** Add get globalstatitembystatcode ([35c6f9d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/35c6f9dacd14f4e76f69d36ad9f8f5f34ef8b0fb))
* **Statistic:** Add new struct for globalstat ([f96e942](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f96e942dca343babf5ffcf0ade792b6801a49338))
* **storage:** specify log directory as data storage destination for non-windows platform ([9c63703](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9c63703f3402e18e63dc30c876d1f033a7ef106f))
* **utilities:** bring back and add advance query ([b765f62](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b765f6247830eeaa62c1e7a905e94398a2bbf252))
* **utilities:** bring back and add advance query ([91b043b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/91b043b6b6b636593049b8e20e40bee1f5c2db10))
* **utilities:** bring back and add advance query ([20b88e7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/20b88e73b349d3865e4d5505ccc01dbb69383639))
* **utilities:** bring back and add advance query ([fa89ac6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fa89ac66064f40a8fdd72954aab5eb52e8ff7e65))
* **utilities:** bring back and add advance query ([42c30cf](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/42c30cf898c7491fb200d842d7de6c5277ac9b25))
* **utilities:** bring back and add advance query ([15d277c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/15d277cbb27919c149b8d440e917373e56c77599))
* **utilities:** bring back and add advance query ([fcebfb8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fcebfb846c50704e8c486cadbb63091cac5523ab))
* **utilities:** bring back and add advance query ([133befd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/133befd0319daf9fc6025b05cc47657bf88ea3fd))
* validate HTTP responses when expecting JSON ([6d6b207](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6d6b2078aca9a55b6e7299e54e64d0438b977f46))


### Bug Fixes

* **advanced query:** change data type to receive different data type ([1ba7fd4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1ba7fd4719b2a45e19c6ace64782bb2d1f3f5dcf))
* **advanced query:** change data type to receive different data type ([b44d955](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b44d9550f598a789e09d30dc84ba67571bf9f76e))
* **advanced query:** change data type to receive different data type ([314bca9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/314bca935a41445ffa01304e576b114f58bdbdc0))
* **advanced query:** change data type to receive different data type ([8fdf78f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8fdf78fb8c57f1cc783acffb6082eef36464815e))
* **advanced query:** change data type to receive different data type ([cf9f05d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/cf9f05d5f13d2e1ba0da47f7fde881503fbe2e1b))
* **advanced query:** change data type to receive different data type ([91bba7c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/91bba7c8bc88769474aaf3b6820b4efc6ac2d52a))
* **advanced query:** change data type to receive different data type ([b85534a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b85534ac1f27ace434b831cada95a8399bf79760))
* **advanced query:** change data type to receive different data type ([079308e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/079308e4731251d49d9733644c11ecccb9743afe))
* **advanced query:** change error log to warning ([8246505](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8246505dd1e4093bf62750e07a160496674365a4))
* **advanced query:** change error log to warning ([4bba2df](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4bba2df962ead37738365d0dbbb775b8827f05bb))
* **advanced query:** change error log to warning ([fd04cd1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fd04cd116ce033fd580f109af68fedb97e0d69cb))
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool ([1817a99](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1817a9998a5410ccdcc8dbae25f1d86e9bdee6e3))
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool ([c5d2710](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c5d2710a8d1eb26a43dcac292edac11a32554f4c))
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool ([f46f32c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f46f32c1cfeaa5fe3c908eeb6b877f0854373126))
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool ([2cfe9aa](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2cfe9aad4dd231f94e3e78e98e33b8ac1217dc9e))
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool ([930b24b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/930b24b609d65e542e30672a41e17743688043b5))
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool ([883ae03](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/883ae03b68d55b5ecbb89b42848dc7280c22f439))
* **advanced query:** change implementation flow and move it to new files ([ae9155e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ae9155ea856f43fbc740891466108a3ff3097a87))
* **advanced query:** change implementation flow and move it to new files ([0768ccd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0768ccd5fe66518255f927bd63ee1a8ccbcc7ddc))
* **advanced query:** change implementation flow and move it to new files ([f3b06bf](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f3b06bfbfa288c89999bc4a4751e7dce96eb04ad))
* **advanced query:** change implementation flow and move it to new files ([5385469](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5385469e466ccda182b384f4468e443800a73f70))
* **advanced query:** change implementation flow and move it to new files ([e71a2a1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e71a2a111fdbfc77621d98ec372d224abab0c97e))
* **advanced query:** change implementation flow and move it to new files ([7da7863](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7da7863ec7e93ba85789ce10ca7b9a3172f8fe85))
* **advanced query:** change implementation flow and move it to new files ([18822c4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/18822c4e840cc64a854fc09c0ff1551eb00b43a3))
* **advanced query:** change implementation flow and move it to new files ([3c1400a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3c1400a331f2f90c7a07ed2a21f2bbd20218c0d2))
* **advanced query:** change start function using sharedRef ([fd4b1cb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fd4b1cb7636bc681b86a48273dc63d0c752789e9))
* **advanced query:** change start function using sharedRef ([4eaedc0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4eaedc06c6a738fc8dfb6887951df05aa4493b6e))
* **advanced query:** change start function using sharedRef ([d84d6bd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d84d6bd55b13485eb3c6700c109da3f6c1ab7470))
* **advanced query:** change start function using sharedRef ([9c94b77](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9c94b77526951bc6d9f2be8eb59b542ae7ab1356))
* **advanced query:** change start function using sharedRef ([14c7277](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/14c72775e998290993320b67cbbd196277f803df))
* **advanced query:** change start function using sharedRef ([c0266f6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c0266f67a3de103552ecf14d913ab19a06474bdd))
* **advanced query:** change start function using sharedRef ([1462648](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/1462648217f2a6bd4972246ab6525eea7e05e6cf))
* **advanced query:** change start function using sharedRef ([74bcdb1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/74bcdb1b83215642ba523c07237ceffc075e2164))
* **advanced query:** clean up after rebasing from master ([4114d9f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4114d9f727e918bcafedd24441f8ec43c6c102ad))
* **advanced query:** clean up after rebasing from master ([20afff7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/20afff7df298ab9fefe2f889d72be538e02e5398))
* **advanced query:** clean up after rebasing from master ([319d005](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/319d005434e67aa96c90170bccc511e98e1c96e4))
* **advanced query:** clean up after rebasing from master ([44cb514](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/44cb514541b43647647a1a25679a54f9090a95a3))
* **advanced query:** clean up after rebasing from master ([0990868](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/09908687170193f656a9dc686549ea32b537012e))
* **advanced query:** rebasing from master again ([3dffe76](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3dffe76986b51a9f28f44f40d3b6b70b3da6add0))
* **crash:** fix crash on shutdown due to task graph and tickers being destroyed ([e87d52b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e87d52b3303f4bddd2f1191222106098b161992a))
* **entitlement:** unable to query user entitlements with multiple itemIds ([0361039](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/036103960cb7b148bff0f9e383fd31c9f46f92f4))
* **heartbeat:** fix compile errors with FRegistry::HeartBeat on Linux ([a0e260a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a0e260a703b13ae88ea4be56aca03e38b69aed30))
* remove unused namespace declare ([b4d7afe](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b4d7afe958e7e5e28fa02b8f3a5baebcf478ccb5))
* **Statistic:** Initialize FDateTime With 0 ([91d9767](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/91d9767458b37e7f73e4229aac38f3803dacaeea))


### Refactors

* **Statistic:** Change variable name ([5a99d59](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5a99d59416105f1f5be190ece28fbb52a2581af3))

## [20.3.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/20.3.0%0D20.2.2) (2022-12-19)


### Features

* Account linking with one-time code ([131d8de](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/131d8de158645aa2c74b445c34e473217b165958))
* Add new chat service APIs ([07335d4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/07335d44a286f46747987dd7b0ce9c573229509d))
* BulkResetMultipleUserStatItemsValue to support OSS ([d542574](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d5425744e1ad13124b3683a78b5c0fce7bd1515b))
* expose to support OSS, Reset stats from a specified user ([2098dcb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2098dcbf18e69a9eefb1b637fa10f71b6314f3e9))
* **session:** add ENDED and UNKNOWN status in EAccelByteV2GameSessionDsStatus ([9afdd28](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9afdd280735451c1a23262f1c2221988e2b68b83))
* **settings:** sanitize base url on load ([212f82d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/212f82d026ac6b21186407ba975c8e3e13ec6ed1))
* **Utilities:** disable jwttoken for nintendo switch ([411564e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/411564e6023bf1ee2261662bf2437af0571634b4))


### Bug Fixes

* disable HeartBeat in DS ([9de6efd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9de6efdb03fd1d4ff4336c054667bcdb8d767027))
* **heartbeat:** fix compile errors with FRegistry::HeartBeat on Linux ([fc0a0b6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fc0a0b62aa8417d8fdacdd26555c8c6023189305))
* **session:** remove TextChat field from FAccelByteModelsV2GameSessionBaseRequest. ([69a602e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/69a602e490ea2e6cb204a0431338e8e8c022180a))


### Documentations

* add initial documentation files ([392531d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/392531d8fd1bdd9f15f6e094b27176a75a7ecb95))
* implement script for automated build ([2e5a96a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2e5a96a611de81835c26b0eb7132e06cb92f1a42))

### [20.2.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/20.2.2%0D20.2.1) (2022-12-14)


### Bug Fixes

* incorrect IAM url used during refresh token ([ea27755](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ea277553daff6186faf51ef1858353dc798b556c))

### [20.2.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/20.2.1%0D20.2.0) (2022-12-09)


### Features

* **MPv2:** add new server calls to create game sessions and send invites to game sessions ([ae0418d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ae0418dd6afe3dd0044043cd559d0a4b26d027ea))

## [20.2.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/20.2.0%0D20.1.1) (2022-12-05)


### Features

* add heartbeat service ([4e4121d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4e4121df286e67d51d2d322be86d065a33104ee7))
* add missing header ([0569272](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0569272297e90f7e8ca42638a751623b6c557b6e))
* Exposing Get Item By Criteria in DS ([93642a8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/93642a87cd287e69fe441f65efb3be962f2cdcdf))
* Make the PSN sync endpoint to be able to receive multiple service labels ([f206035](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f2060354f6d27f6717bdf03ae303ae2cd9b38b8d))
* update implementation to support Unreal Engine 5.1 ([c18cf19](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c18cf19d084aaefde5380afb8a18056924be416e))


### Bug Fixes

* **logging:** change verbosity level in the logging with AccelByteReport ([fed313a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fed313a3cde430b56d994e69f1e80b9c777ec173))
* **settings:** unable to use public OAuth credentials type for Server Settings ([d559f69](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d559f697543a166cf280d35cf8f34c883af46ac4))

### [20.1.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/20.1.1%0D20.1.0) (2022-12-01)


### Features

* Add new chat service APIs ([deb61e6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/deb61e6ceb5ea9ea2405a1a8b56317482a7d34a1))
* **session:** add ENDED and UNKNOWN status in EAccelByteV2GameSessionDsStatus ([2a3cbb9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2a3cbb92f9b76b2bf44a8bdb9e18b694344f8c66))


### Bug Fixes

* **session:** remove TextChat field from FAccelByteModelsV2GameSessionBaseRequest. ([f1c5016](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f1c50167f8379bd9be2d27771f6f08c71d84850a))

## [20.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/20.1.0%0D20.0.0) (2022-11-21)


### Features

* E-Commerce Loot box ([59a9de6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/59a9de6cabb097145319cc1d1892888751e0c256))
* expose get all stats from specified user and Get user's specified stat items ([9c874ae](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9c874ae1879d7976934d3ab6a4fbb6917c867956))
* Expose Get Store List for DS ([e834c2a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e834c2ab064085dd770fd395a37fcb9cc0d80621))
* **iam:** expose the Admin Get User's Bans endpoint ([6fddbb3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6fddbb3b45ad6a36b7e59d65cda7c103c33a6f30))
* **session:** add missing to avoid breaking changes ([975666e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/975666e798e286e91397d1b8605757606fd758f4))

## [20.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/20.0.0%0D19.0.0) (2022-11-07)


### ⚠ BREAKING CHANGES

* **Session:** Rename FAccelByteModelsV2PartyMembersChangedEvent field from Party to Session
* **session:** model list
-  FAccelByteModelsV2DSStatusChangedNotif.GameServer -> moved to inside FAccelByteModelsV2DSStatusChangedNotif.Session.DSInformation
-  FAccelByteModelsV2PartyMembersChangedEvent.PartyID -> moved inside FAccelByteModelsV2PartyMembersChangedEvent.Party.ID
-  FAccelByteModelsV2PartyMembersChangedEvent.Members -> moved inside FAccelByteModelsV2PartyMembersChangedEvent.Party.Members
- FAccelByteModelsV2GameSessionMembersChangedEvent.SessionID -> moved inside FAccelByteModelsV2GameSessionMembersChangedEvent.Session.ID
- FAccelByteModelsV2GameSessionMembersChangedEvent.Members-> moved inside FAccelByteModelsV2GameSessionMembersChangedEvent.Session.Members

### Features

* add match pool field to game session create and update request models ([acb7a2f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/acb7a2fdec8f03223183d7a8cf015ce5745984fb))
* added username field in the FBaseUserInfo function, used when call public get bulk user info API ([300cb95](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/300cb95853b6f6b77a76f6d9182df1c06650dd54))
* implement get credential turn server ([f1cbae9](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f1cbae9e7261257847bf6cab50492edbf8405638))
* OPTIONBOX on EAccelByteEntitlementClass ([3e1a0fd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3e1a0fd9649131d572f65d381cbec4397e9f3157))
* **Session:** add DISCONNECTED and TERMINATED member status enum ([3e57137](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3e571371193d8bde7a05f662ed60b2c9cb90dc51))
* **session:** add missing to avoid breaking changes ([0045cc1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0045cc10ff7638467dd75196d55f3503121c2eb0))
* **session:** Add session and party info in Game and Party session member changed notif and DS status change notif ([649674a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/649674a6d76df03877132ff98299513fed36e665))
* **Session:** add TIMEOUT in session member status enum ([622e63b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/622e63b2410af380eb91b3287682663d0a526d50))


### Bug Fixes

* crash when static casting session model ([0a05535](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0a055355997bd7ba99b4228a1a49f64e80be9bfc))
* **Session:** Rename FAccelByteModelsV2PartyMembersChangedEvent field from Party to Session ([a11c1a0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a11c1a0b0cd04d32798da75edf2a71059b557d48))

## [19.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/19.0.0%0D18.0.0) (2022-10-24)


### ⚠ BREAKING CHANGES

* renamed some variable in item info model

### Features

* **ABUtilities:** add function to get JsonString ([d5b4933](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d5b49333326ef29601d323083a61904ed74026fe))
* Bulk get user season progression API ([2e4adcd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2e4adcda4d06f80e4935b1f89735def68e5b9f75))
* command line args switch parser ([398952d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/398952da6cfb9d43a18b3b4f21fbcea81c96d60a))
* expose update channel public endpoint ([cbdaf3a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/cbdaf3aae1658b1c026ed0f70f8a170c1a7f0054))
* implement get credential turn server ([c84b33f](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c84b33fba54c54912752dbf210ab5cb0278987fe))
* **qos:** re-enable udpecho on android ([3084896](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3084896b169945f069d8c30f63db285cf97a6691))
* **Session:** add TIMEOUT in session member status enum ([7574d07](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/7574d074d96e594a1c2361fd073ebf668ca212cc))


### Bug Fixes

* add some missing headers ([5ca5fec](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5ca5fec1ca6b4471952493a2476debb4ab4324da))


### Reverts

* Revert "feat(session)!: Add session and party info in Game and Party session member changed notif and DS status change notif" ([86d4a06](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/86d4a06838436e0db4d8177ac2c9535869a0c905))


* Merge remote-tracking branch 'remotes/origin/hotfix/17.0.0' into hotfix/resolve-conflict-17.0.0-master ([256353b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/256353bfa2cf774233ded2e0b818a244bf2ba7c6))

## [18.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/18.0.0%0D17.0.2) (2022-10-10)


### ⚠ BREAKING CHANGES

* Renamed some variables in item info model
* renamed FPresenceStatus to FAccelBytePresenceStatus to resolve conflict

### Features

* adjustment for improve entitlement track log ([e496799](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/e496799a9f6dccd39d4f470f7bba2581802cc411))
* banning by device ID (PS, XBOX) ([270b46a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/270b46a010cb3385499b7ff8b4901d51658650cf))
* Bruteforce protection upon game login ([676e9dc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/676e9dcb8142d594e29c271650ce33429632e504))
* expose Get and Update PrivateCustomAttributes ([d05434a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d05434ab03fce0a70d397973d24a29d40efa6b3e))
* **HTTP:** adding custom metadata header to HTTP request ([7189399](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/71893991bd024b0db1600fac656c0c8367ae5461))
* **lobby:** add send party notif ([9a685cd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/9a685cd08a27bb19113725a94b94d35e9c650891))
* **lobby:** support for role based matchmaking ([64e1c66](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/64e1c669492c4f76a83c19b59c7ea892e365a0a8))
* **lobby:** support rejected status on role based matchmaking ([41d704c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/41d704c35f48faa21d71a912bb30ef80f3af4062))
* update to cover isse When invoke Credentials::GetUserEmailAddress() in AccelByte SDK for UE4 after sign-in, it returns username instead of email address. ([719591d](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/719591d6ee0ec6ee4abc6b80fe8d4df810023960))


### Bug Fixes

* conflicting FPresenceStatus used in both AccelByte SDK and Vivox SDK ([8683789](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/86837896e2071d4258197309007bffdc2856e76e))
* **login:** wrong implementation on get user data on successful login ([50ddfd5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/50ddfd526066d31f7f7e4a0c1f8b8a2c65ddabf0))
* support get wallet info old workflow ([5a9b40e](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/5a9b40ee40da7a4e96ab6d2cc95d91b133350c11))
* update current user from register and unregister player to session browser ([a1e14b6](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/a1e14b618c98f1b34f587d0484bd87283d7b8bf8))


### Refactors

* change LobbyErrorMessage to static ([6051dc0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6051dc0903f2253307b6d2b7b126fce7e46fe4e2))


* Merge remote-tracking branch 'remotes/origin/hotfix/17.0.0' into hotfix/resolve-conflict-17.0.0-release-candidate ([8d2e154](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8d2e154c778c4aebf0ea2ffd66a823cb1af006b0))

### [17.0.2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/17.0.2%0D17.0.1) (2022-10-04)


### Features

* **ban:** automatic token refresh when the ban has ended ([0282a3b](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0282a3bfc90e8410c979bdc8a7c49bcdbf209c2c))


### Bug Fixes

* wrong headers included in the source ([51835a2](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/51835a20507267d11c5f33f0ba58b876444ee21b))

### [17.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/17.0.1%0D17.0.0) (2022-09-28)

## [17.0.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/17.0.0%0D16.2.1) (2022-09-27)


### ⚠ BREAKING CHANGES

* renamed FPresenceStatus to FAccelBytePresenceStatus to resolve conflict
* Wallet::GetWalletInfoByCurrencyCode returns new adjusted model

### Features

* AccelByteUe4Sdk Plug-in's Wallet::GetWalletInfoByCurrencyCode returns a partial response ([c551113](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/c55111352b3be5985156ae1371e7069a3e6ed473))
* **extension:** add new item type called extension ([0659bdd](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0659bdd5b57b2174e6edc392c2546172870d89bd))
* Improve Store Category search results to display all items from its sub-categories ([2218624](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2218624d6a71e8e659b174f595546a82ad5918d1))
* **lobby:** NOT BREAKING CHANGE: add reject consent function ([6fbcf78](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6fbcf788ca6b4209d49ea6114ad11a123ee0a6ed))
* providing mac address & platform name methods to fulfill client requirement ([0a5fef5](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0a5fef52cd979044d2a661420b4f7433687e694d))
* Temporarily cache telemetry data into the Local disk storage ([0ec3fe3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/0ec3fe30069b2cf7bc95e9a7ac02621799d93d29))
* walletapi support old walkflow and multiple platform ([18f814a](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/18f814a1906590ccd7544752eecdd683bedcec12))


### Bug Fixes

* Conflicting FPresenceStatus used in both AccelByte SDK and Vivox SDK ([068d1ed](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/068d1ed272a6a01622e5c9f17d99f30cde6e65aa))
* Fix data corruption after item deletion ([0189662](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/01896623640085f88bcb7085794b537f48802817))
* PopulatedItemInfo inherits ItemInfo model ([3b68192](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3b68192be031322fcff8f588b058b97ebc54cb42))
* support get wallet info old workflow ([fe387c8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/fe387c8393732abc52a14d5321e5f3ece44cca56))
* update current user from register and unregister player to session browser ([cdcd755](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/cdcd7555b3a97da78e242851312049fce2268c25))

### [16.2.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/16.2.1%0D16.2.0) (2022-09-15)

## [16.2.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/16.2.0%0D16.1.0) (2022-09-12)


### Features

* binary storage & refresh token caching ([dbd72a8](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/dbd72a87f786d8dc4cc391a549b21bb3f4ebcd21))
* IAM Phase 4 Statement & Substitute Implementation ([76def47](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/76def475d2a1dd8a79cac0fb20028ab342ceca3b))
* **lobby:** NOT BREAKING CHANGE: add reject consent function ([2bdc7f3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2bdc7f3efb74417c4db9ce1b246ddd497849604a))
* **MPv2:** Add multiplayer v2 APIs ([13618a1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/13618a13842719c1f3e7adcac68c7d90748fb606))
* SDK Implementation for UGC service ([f13ff02](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f13ff026fcabb3092be9e886cbcd27f6b0d78aae))


### Bug Fixes

* **module:** prevent application stopped and crash reporting when a service is not deployed yet ([67994bc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/67994bcf76f6a2ec1fe3eabb4f260d30d5419293))
* PopulatedItemInfo inherits ItemInfo model ([4d9841c](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/4d9841cf452fa7d91f70198a55ebfab7ef7fcf25))

## [16.1.0](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/16.1.0%0D16.0.1) (2022-08-29)


### Bug Fixes

* **Lobby:** fix crash on connecting to lobby ([d6d02e3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d6d02e3795d11cdf969c5ede29b19c92321adc48))

### [15.1.3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/15.1.3%0D16.0.0) (2022-08-18)


### Features

* **entitlement:** add platform iap sync for single item ([ea9b7de](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/ea9b7de133a97c8f992bddc3535e5e6d1b6db237))
* iam phase 2 statement & substitute implementation ([86c0c06](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/86c0c069ebdf266809dc585656152fefa0a2e4af))
* IAM Phase 3 Statement & Substitute Implementation ([6550a69](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/6550a692f9ddfb3ddd26910077ec00148f4ad1a7))
* **lobby:** refactor LobbyMessageToJson function, handle trailing comma on array of object and an object value ([44ea8d7](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/44ea8d77fa846c49b5f632bcff8124b205e46f29))
* **qos:** get ping timeout from GConfig ([d00b6eb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/d00b6ebf8bd360a69d7d9bd749b739dd6b06e649))
* SDK Implementation for UGC service - Get follower count ([028a408](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/028a4087575b753233113494c62b9472946bdb1d))
* SDK Implementation for UGC service - Get list content ([2a4baa4](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/2a4baa4bd25f8b66ec4e645caead3309e7c6e266))
* SDK Implementation for UGC service - Query by Tags ([3ea6b16](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3ea6b167d78f25f23bb03d094de19c027a3cb837))


### Bug Fixes

* Cannot remember "device_token" if comes from request header, rename header device_token to device-token ([b4128eb](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/b4128eb608e50dca9a1139c1d49be0d2bea39573))
* **settings:** URL didn't change when switching environment ([3801b40](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3801b40345c19282dfe827dd3257f13d108f66a5))
* SortBy in GetItemByCriteria and UrlEncoded in Query Param Array ([8fd45dc](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/8fd45dc70f98fcc1e1aa20d39d411ca3c345ce15))

### [16.0.1](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/16.0.1%0D16.0.0) (2022-08-18)


### Bug Fixes

* **settings:** URL didn't change when switching environment ([f767246](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/f7672467afe2f1af8fb77dd14c45f069281a4e88))

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

### [15.1.3](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/branches/compare/15.1.3%0D15.1.2) (2022-08-18)


### Bug Fixes

* **settings:** URL didn't change when switching environment ([3801b40](https://bitbucket.org/accelbyte/justice-unreal-sdk-plugin/commits/3801b40345c19282dfe827dd3257f13d108f66a5))

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
