# Changelog

All notable changes to this project will be documented in this file. See [standard-version] for commit guidelines.

### [27.1.1] (2025-05-30)


### Bug Fixes

* **HttpRetryScheduler:** unassigned timeout in HttpRequest and use the timeout in HttpRetryScheduler instead 

## [27.1.0] (2025-05-21)


### Features

* add thread lock in messaging system 
* **cache:** isolate cache copying for Editory-only 
* **Credentials:** adding safeguard to improve the thread-safety 
* **DataStorage:** improve thread-safety by using CriticalSection 
* move the default storage cache from ProjectSaved to ProjectContent directory 
* **threadsafety:** change the way to use FRWScopeLock, avoid copy 
* **wallet:** Add server APIs for managing both platform and non-platform wallets 


### Bug Fixes

* change FString to TCHAR for better portability between different platforms 
* change the behaviour of HttpRetryScheduler 
* **chat:** add forceCleanup flag to forcefully close the WebSocket connection 
* **credentials:** separate lock for credentials information and delegate 
* **device id:** re-use instance index for device id instead making a new one 
* **gameTelemetry:** missing scopelock on OnLogoutSuccess, potentially cause a crash 
* improve thread safety of FAccelByteInstance 
* make ABInstance unmovable 
* wrong behaviour on UE5.4 and above 


### Refactors

* change automatic refresh token to use V4 endpoint 
* refactor ChatUpdateUserTopicNotif to derive from ChatUpdateTopicNotif 

## [27.0.0] (2025-03-24)


### ⚠ BREAKING CHANGES

* decommissioned ApiClass reference usage in ApiClient
* decommissioned static FRegistry and FMultiregistry

### Features

* **achievement:** bulk unlock achievement API for both client & server 
* **ams:** provide AMS drain notif multicast delegate since the existing delegate is only single delegate 
* **ban:** schedule unban notification in Lobby when logging in 
* **Challenge:** add ServerChallenge::GetUserChallengeProgress() function & add additional field to response model FAccelByteModelsChallengeGoalProgress & FAccelByteModelsChallengeProgressMeta 
* **chat:** expose SenderPlatformId in chat message notification 
* **ecommerce:** add notifications for entitlement updated, wallet status changed, and wallet balance changed 
* **game profile:** add deprecation message to the game profile service 
* **iap:** Implement Steam IAP sync by transaction API methods 
* **session:** implement server API endpoint to update DS information for a game session 


### Bug Fixes

* **Session:** remove typo definition of the FAccelBtyeModelsGameSessionExcludedSession 


### Refactors

* decommissioned ApiClass reference usage in ApiClient 
* decommissioned static FRegistry and FMultiregistry 

## [26.7.0] (2025-02-28)


### Features

* **apiClient:** add GetCloudStorageApi 
* **challenge:** add ListSchedules and ListScheduleByGoal, deprecate FAccelByteModelsChallengeGoal::Schedule 
* **challenge:** add new blueprint endpoint with tags parameter 
* **challenge:** add tags parameter to filter the challenges 
* **gameStandardEvent:** add new events 
* **HTTPCache:** improvement to support actual CDN usage 
* **LobbyApi:** add new functionality to AddMessageNotifDelegate 
* **user:** replace and deprecate user data endpoints 


### Bug Fixes

* cleanup ApiRegistries and AccelByteInstance 
* compile error against Nintendo Switch 
* crash due to invalid object captured in the lambda delegates 
* **EnhancedPresence:** change ClientTimestamp format from UnixTimestamp to ISO8601 
* fixed websocket responses being labeled as requests by enhanced logging 
* issue when socket is deleted in multithread whilst in middle of a tick execution 
* move enhanced logging of requests to retry scheduler for completeness (was missing some requests before) 
* **websocket:** teardown should close the connection again to ensure the old websocket connection is closed 

### [26.6.1] (2025-01-24)


### Bug Fixes

* adding missing header file that cause compile error on Linux server build 

## [26.6.0] (2025-01-22)


### Features

* **GameTelemetry:** add functionalities to set flag to enable or disable caching 


### Bug Fixes

* **websocket:** teardown should close the connection again to ensure the old websocket connection is closed 


### Refactors

* connecting to AMS will not use credentials 

## [26.5.0] (2025-01-16)


### Features

* **access:** change APIs to WeakPtr 
* **access:** change user and agreement blueprints, to use ApiWeakPtr 
* **commerce:** change APIs to WeakPtr 
* **commerce:** change blueprints, to use ApiWeakPtr 
* **engagement:** change APIs to WeakPtr 
* **HttpRetryScheduler:** change HttpRetryScheduler object to shared object 
* Implement AccelByteInstance 
* **multiplayer:** change APIs to WeakPtr 
* **presenceBroadcast:** change APIs to WeakPtr 
* put service logging on console var (off by default but compiled in) 
* **Session:** add team id variable to session creation and history 
* **session:** exclude past session when create a match ticket & introduce party session storage 
* **session:** implement notification handling for first-party platform session sync 
* **storage:** change APIs to WeakPtr 
* support Unreal 5.5 


### Bug Fixes

* AccelByteInstance not connecting to AMS 
* adjust report validation by the category 
* **HttpRetryTask:** add validation to unbind request complete if request is valid 
* **model:** correct the FAccelBtyeModelsGameSessionExcludedSession mistype and introducing typedef to avoid breaking change 
* prevent connecting to the Lobby and Chat WebSocket server when Credentials is NOT Valid 
* **websocket:** move the state correctly when Disconnect the connection 
* **websocket:** stuck in the reconnection state when got disconnect notification (4041 & ban notif) 


### Refactors

* change BearerAuthRejected and BearerAuthRefreshed to multicast delegates 

## [26.4.0] (2024-12-04)


### Features

* add support for retrieving platform friends using platform handler and restructure platform handler files 
* **challenge:** add new goal progression id variable 
* **cloud save:** add get bulk user records by keys 
* **cloudsave:** add bulk get and replace user record for server 
* **entitlement:** add get user entitlement by ids 
* **friends:** add a new error code when requester is blocked by requestee 
* implement delete record ttl config on server cloud save and binary cloud save. 
* implement delete record ttl config on server cloud save and binary cloud save. 
* **platform handler:** add some functions to support metaQuest IAP 
* server partial backfill acceptance adjustment 
* **server:** allow server to accept backfill proposal partially 
* **ServerCloudSave:** add TTLConfig on AdminGameRecord endpoints 
* **session:** exclude past session when create a match ticket & introduce party session storage 


### Bug Fixes

* **AMS:** always set bServerUseAMS as true in ServerSettings 
* **AMS:** make heartbeat as optional and change error message for AMS parameters 
* check url validity on process request 
* **qos:** add delay when try to resolve server ip, change to SharedPtr, pass WeakPtr instead of this on lambda capture 
* Remove scheduled unban notification if ban lifted earlier 
* **websocket:** stuck in the reconnection state when got disconnect notification (4041 & ban notif) 

### [26.3.1] (2024-11-07)


### Bug Fixes

* **AMS:** make heartbeat as optional and change error message for AMS parameters 
* **challenge:** add missing enum member CUSTOM in EAccelByteModelsChallengeAssignmentRule 
* **gameTelemetry:** change TSharedPtr<FAccelByteModelsTelemetryBody> to TSharedPtr<FAccelByteModelsTelemetryBody, ESPMode::ThreadSafe> 
* **lobby, chat:** move to sharedPtr 

## [26.3.0] (2024-10-30)


### Features

* add a new flag called IsHiddenPublic 
* add Challenge::GetChallengeProgress with optional GoalCode 
* add trace cpu profiler on some poller functions 
* **challenge:** Add 'get previous challenge progress' and challenge blueprint API 
* **cloudsave:** add 'get current user public keys' 
* **entitlement:**  implement query user subscriptions 
* **lobby:** add OnPartyCreated notification 
* **reward:** add an extra reward item to complete both type (ItemID & SKU) 
* **websocket:** ability to configure the retry reconnection through the ApiClient 


### Bug Fixes

* **Analytics:** change gameTelemetry object passed on lambda as sharedPtr instead of object references 
* **analytics:** remove TSharedFromThis on PredefinedEvent and GameStandardEvent because SharedPtr being null on 5.4 
* change messaging topic type to lobby in qos destructor 
* **entitlement:** QueryEntitlement's entitlementName parameter encoded twice 
* **gameTelemetry:** change AddRaw to AddThreadSafeSP, make sure object accessible when needed 
* **gameTelemetry:** fix compile error on 4.27, change TWeakPtr<GameTelemetry> to TWeakPtr<GameTelemetry,ESPMode::ThreadSafe> 
* Item::GetItemMappings copy OnSuccess by value instead of ref 
* linux compile error in UE 5.4 
* remove accelbyte time manager constructor 
* serialize json content with the expected value 
* supporting to fetch equals type of command line to get the value from there 
* use TEXT macro to prevent deprecation warning on JsonObject operations 

### [26.2.1] (2024-09-26)


### Features

* add GetResponseHeader in HttpRetryTask 
* add lockable queue 
* add new fields in FAccelByteModelsV2MatchmakingGetTicketDetailsResponse 
* implement LockableQueue in lobby notification 


### Bug Fixes

* change from flush into shutdown 
* cleanup on pre exit 


### Refactors

* strong typed lobby sequenceID and Number 

## [26.2.0] (2024-09-19)


### Features

* Add TicketID in matchmaking v2 canceled notification model 
* **agreement:** add array of tags inside public policy model 
* **DSHub:** add X-Ab-Custom header when connecting DSHub 
* **inventory:** add bulk endpoints and add new fields to inventory item response 
* **serviceLabel:** add additional optional parameter to allow serviceLabel for authentication 
* **session history:** add a new endpoint to get game session history 
* **session:** add API to kick user from a game session 
* **user:** add API for update password, get mfa status, and setup mfa 
* **user:** add support for user account creation with mandatory email verification 


### Bug Fixes

* change game telemetry `flightId` case to PascalCase `FlightId` 
* **commerce:** change the default value of AutoAck on FAccelByteModelsPlatformSyncMobileGoogle to false 
* **lobby:** missing message type field 
* **settings:** fix load ams server settings 


### Refactors

* move QoS GetServerLatencies from after user logged in, to SDK Module OnGameInstanceCreated 

### [26.1.1] (2024-08-21)


### Features

* Add TicketID in matchmaking v2 canceled notification model 


### Bug Fixes

* access violation when accessing MessagingSystem that has been destroyed 
* lobby notification buffer SortBuffer method consider SequenceId instead of SentAt 

## [26.1.0] (2024-08-12)


### Features

* add BulkGetUserByOtherPlatformUserIdsV4 
* **AMS:** add capability to set and reset session timeout from dedicated server 
* **chatv2:** add API to get and set user chat configuration 
* **ecommerce:** add StatisticCodePredicate as PredicateType enum, add `code` field on ItemPredicate model 
* ServerUserAPI to Get Bulk User Information 
* **serviceLabel:** add additional optional parameter to allow serviceLabel for authentication 
* **user:** new endpoints for BulkGetUserPresenceV2 and BulkGetPublicUserProfileInfosV2 


### Bug Fixes

* **analytics:** update client time stamp using server utc time 
* change AsWeak() usage to AsShared() for UE 4.27 
* **ServerTime:** call get server time after game instance created and fetch the response time from http retry task. 
* **SyncEntitlement:** sync platform purchase with simultaneous platform 
* **telemetry:** change `deviceType` to use PascalCase 
* **Wallet:** duplicate the correct expiration information model field from ExpireAt into ExpiredAt 

### [26.0.2] (2024-07-18)


### Bug Fixes

* access violation issue in refresh token delegate 
* **entitlement:** fix remove empty field from request JSON on SyncMobilePlatformPurchaseApple 
* **qos:** add empty address guard, check whether ip already valid before resolve it 

### [26.0.1] (2024-07-10)


### Features

* **Agreement:** add change policy preferences 


### Bug Fixes

* **challenge:** update data model and add request validation 

## [26.0.0] (2024-07-05)


### ⚠ BREAKING CHANGES

* **Inventory:** this changes remove Quantity param on GetUserInventoryAllItems, and remove some enums on EAccelByteUserItemsSortBy
* **Challenge:** enum EAccelByteModelsChallengeGoalRequirementPredicateParameterType remove USER_ACCOUNT, added USERACCOUNT

### Features

* add IAP config error code 
* add Reason and PartyID fields in MatchmakingCanceledNotif 
* add storage field in create game session request 
* add storage in create matchmaking ticket and ticket information model 
* Added v1 of service logging (for Visual Debugger) 
* Added v1 of service logging (for Visual Debugger) 
* admin inventory endpoints 
* **Challenge:** Implement server challenge get and claim rewards endpoints 
* **ecommerce:** add server bulk get items by id 
* **Inventory:** remove quantity on GetUserInventoryAllItems 
* **sessionv2:** add cancel game session or party invite 
* **TurnServerLatency:** add new endpoint to get turn server list and add function to get turn server latencies 


### Bug Fixes

* buffered notifications dispatched when access token is not valid 
* **Challenge:** add NOT_STARTED enum to the EAccelByteModelsChallengeGoalProgressStatus to ensure JSON deserialization 
* GetItemMapping with platform NONE will query all platform 
* **HttpRetryTask:** add member's default value & not use the constructor initializer list 
* IAP item mapping EPICGames and add STADIA 
* incorrect include filename 
* prevent breaking changes in GetItemMappings by intercepting and translating EAccelBytePlatformMapping 
* remove deprecate log on ReadChat function 
* **UEnum:** reorder EAccelByteStatisticUpdateStrategy enum value and add additional entry to allow other enums appear completely in blueprint 

### [25.5.2] (2024-06-30)


### Features

* **IAM:** support google login 


### Bug Fixes

* missing IAM service URL in ServerOAuth API 

### [25.5.1] (2024-06-05)


### Features

* add new request header to identify DS based on its ID during Server Authentication 
* **NewProfanityFilter:** add new endpoint for validate user input 
* **user:** add logout api and deprecate revoke api 


### Bug Fixes

* add missing Flight ID and additional information to the request headers for Login APIs 
* **challenge:** update goal progress requirement model 
* fix unreachable code on shipping build 
* skip http flush if shutdown called in game thread and engine exit is not requested 

## [25.5.0] (2024-05-22)


### Features

* add roles to the player session attribute 
* changes to support unreal 5.4 
* **chat:** add new endpoint to get a chat configuration 
* discount code redemption and order preview API 
* Enable rate limit via AB config injection 
* **MMv2:** MM start notification for cross play 
* **User:** add a new SDK function to GetUserPublicInfoByUserId while deprecate the old function (User::GetUserByUserId) 


### Bug Fixes

* **chat:** fix chat disconnect on normal disclosure state 
* **entitlement:** fix remove empty field from JSON on SyncMobilePlatformPurchaseGooglePlay 
* rename DiscountCodeInfo field 
* **User:** change revoke token success handler 


### Refactors

* Credentials in ApiBase changed to SharedRef 

### [25.4.1] (2024-05-16)


### Refactors

* Credentials in ApiBase changed to SharedRef 

## [25.4.0] (2024-05-13)


### Features

* add login queue cancel error code 
* **analytics:** user segmentation data quality check 
* **entitlement:** expose public get user entitlement history 


### Bug Fixes

* **achievement:** change 'DateTime' field type to 'FString' as 'FDateTime' does not format to ISO 8601 in JSON 

### [25.3.2] (2024-05-04)


### Features

* configurable websocket total timeout for DSHub and AMS 


### Bug Fixes

* **qos:** rework qos to resolve addresses synchronously for platforms that do not support async resolution 
* **websocket:** check if engine is still active before calling websocket close 

### [25.3.1] (2024-05-01)


### Features

* **entitlement:** expose get user entitlement history 
* **notification:** add a listener & handler when a Party or GameSession invitation for invitee is timeout. And add ExpiredAt info to the FAccelByteModelsV2PartyInvitedEvent & FAccelByteModelsV2PartyInvitedEvent 

## [25.3.0] (2024-04-26)


### Features

* **achievement:** implement method to call psn event sync endpoint from servers 
* **cache:** migrate old cache and separate telemetry cache from other caches (deviceID, refresh token, etc). into each cache files 
* **inventory:** add new item inventory endpoints 
* **lobby:** add oneTimeCodeLinked notification 
* **Oauth:** add PollingTime field in models 
* **session:** add get recent team player endpoint 
* **storage:** improve the storing format for telemetry by reduce the amount of serialization 
* **sync:** add endpoint for syncing blocked players from third party platform 
* **telemetry:** accumulating the serialization partially when sending to cache 


### Bug Fixes

* **achievement:** deprecate old QueryUserAchievement and create new method with proper global sort by enum 
* **challenge:** use ConverChallengeSortByToString for query params 
* clang error due to ordering of conditionals 
* fix buffer overflow in UdpPing 
* **telemetry:** caching telemetry without a need to do excessive JSON operation 
* **telemetry:** move the JSON operation & file IO into background thread 
* **websocket:** remove ticker after on closed triggered 

### [25.2.1] (2024-04-26)


### Bug Fixes

* **Config:** fix get config to not need auth token for request 

## [25.2.0] (2024-04-05)


### Features

* **DSMC:** Implement RegisterServerToDSM with server info on success 
* **ecommerce:** add API for retrieving IAP items mapping 
* **ecommerce:** Add DebitUserWalletV2 and PaymentWithUserWallet 
* **lobby:** add entitlement token generation and check to lobby connection 
* **session:** add 'previousStatus' field to session member model 


### Bug Fixes

* **Core:** add challenge api on registry 
* **FormatUrl:** fix format api url flow by using sdk setting if the session is not valid 
* **http:** fix construct key implementation by using hash key to prevent the fname character limitation 

### [25.1.1] (2024-03-28)


### Features

* **login:** add simultaneous user info to credentials object 
* **oauth:** add additional simultaneous login info on token model 


### Bug Fixes

* add platformUserId as cachedTokenKey on ClaimAccessToken 

## [25.1.0] (2024-03-24)


### Features

* **mpv2:** add support for friends only joinability in game and party session 
* **ttl:** expose ttl config on cloud save api 


### Bug Fixes

* **chore:** change DefaultSection name to DefaultSectionPath to prevent linux shadow declaration error 
* **launcher:** get the exchange code and call generate game token if using exchange code. 
* **MessagingSystem:** fix issue when messageSystem is not the same for api and registry when create a default apiClient 
* **setting:** update override setting using AccelByte config injection 

## [25.0.0] (2024-03-08)


### ⚠ BREAKING CHANGES

* **CommerceBalance:** balance type on wallet is using int64 now

### Features

* add enablesecret field to response model in sdk 
* **backfill:** add new notification for backfill ticket expired 
* **challenge:** add client and server API methods for the challenge service 


### Bug Fixes

* **CommerceBalance:** fix wallet balance using int64 type 
* **ecommerce:** add missing field DisplayOrder in FAccelByteModelsSectionInfo 

## [24.11.0] (2024-02-21)


### Features

* add common config API 
* add new autoCalcEstimatedPrice Param 
* add TicketIDs field in game session model 
* Implement notification sender 
* **lobby:** add friend REST API 
* **lobby:** add handler for retrieving missing notifications 
* **UGC:** add some public api that can be used without login in some environment 


### Bug Fixes

* **account:** account profile revamp 
* add validation to User API 
* incorrect request body serialized in GetUserOtherPlatformBasicPublicInfo 
* remove validation for platform information request 
* **session:** remove empty fields for storePlayerAttributes's request 
* **support:** add Ext field for FAccelByteModelsSectionInfo model 
* **support:** add SEASONAL enum type on EAccelByteCycle 
* **support:** repositioning the parameter on Region and Language inside ListActiveSectionContents function 


### Refactors

* **PlatformHandler:** add dictionary as extra attributes for FetchPlatfromToken method 


### Tests

* add define guard to avoid parallel tests to override stored value 

## [24.10.0] (2024-02-05)


### Features

* add AMS fleet command API 
* add endpoint to get other user 3rd party platform information 
* add new endpoint of UGC staging content 
* **namespace:** encode namespace 
* **namespace:** REVERT namespace encode 
* **platformHandler:** add OnPresenceChanged delegate to notify sdk if there any presence changed from platform 


### Bug Fixes

* add missing FlightId header in Login with DeviceId functionality 
* change achievement blueprint header 
* **lobby:** store notification message delegate handle, replace instead of add new delegate 
* move DisconnectFromExternalReconnect outside of the boundary 
* remove validation for platform information request 
* **settings:** ClientSecret is still using the old value when the key is not defined in the .ini file 

## [24.9.0] (2024-01-18)


### Features

* add messaging system for communicating between api and automatically send user region on lobby connected 
* **HTTP:** automatically retry the request if the backend returns 429 response (too many request) 
* implement message envelope in lobby 
* Implement network conditioner in chat and lobby apis 
* Implement NetworkConditioner class 
* **Login:** simultaneous login IAM function 
* sdk adjustment to support returning explicit 3rd party platform info 
* send new access token to chat every auth token set and remove chat refresh token handlers 
* send new auth token to lobby every time it's set 
* **session:** Session Service Player Validation 
* **session:** Session Service Player Validation 
* SHA1 generator, update and set to lower case by default 


### Bug Fixes

* add missing include header in AccelByteChatApi.cpp 
* add more description on platform user id 
* add platform user id on login refresh token to cached when try to re-login 
* change queue to shared pointer and add delay in PingRegionsSetLatencies to avoid intermittent enqueue error in messaging system 
* change the parameter of platform user id position 
* **DeviceID:** Device ID randomizer not completely random due to uninitialize seed number 
* fix LogPython same name warning 
* GEngine check cause compile errors in UE 5.2 
* redo removing logphyton warning on editor startup 
* remove ApiClient Credentials intializer 
* remove the new function and add new platform user id optional parameter on the existing function 
* separate SendQosLatenciesMessage from UdpPing success delegate 
* **utility:** ambiguous FMath::Pow 


### Refactors

* remove lobby token refresh handlers 

## [24.8.0] (2024-01-09)


### Features

* add game session recent player endpoint 
* expose cloud save game binary records endpoints in server SDK 
* **gameTelemetry:** add feature to enable/disable game telemetry cache from Engine.ini setup 
* **IAM:** refresh native platform token 
* new lobby notification delegate for UE OSS 
* **oauth:** add namespaceRoles on Oauth2Token model 
* **ServerTime:** return back caculated ServerTime and sync ServerTime with the service every one hour. 


### Bug Fixes

* GetPublicUserProfileInfo headers attached as queryParam 
* **presenceBroadcast:** mark StartHeartbeat and StopHeartbeat as deprecated since it not supposed to be used outside of the class 

### [24.7.1] (2024-01-02)


### Bug Fixes

* hotfix get api client from oss 
* remove unnecessary return for a better code 

## [24.7.0] (2023-12-14)


### Features

* Add SDK version in the connection headers of all the Websocket API. 
* adjust concurrent record endpoint behavior 
* **analytics:** refactor predefinedEvent, adding base analytics api 
* **chatv2:** add more information in FSystemMessageNotifMessage 
* expose public bulk get content by sharecode 
* expose public endpoint for updating content share code 
* Expose UGC ModifyContent and DeleteContent by shareCode 
* Exposed RetrieveUserThirdPartyPlatformToken 

## [24.6.0] (2023-11-28)


### Features

* add deprecated message for UpdateUserV3 
* add several predefined events for storage scope 
* expose new search hidden content endpoints 
* **IAM:** generate game namespace code for publisher usage 
* **OAuth:** add CreateHeadless for LoginWithDeviceId 


### Bug Fixes

* fixed cache token for windows platform 
* **predefinedEvent:** change event namespace to game namespace 
* **Switch:** compile error on switch platform 

## [24.5.0] (2023-11-13)


### Features

*  Expose CheckUserAvailability 
* add message field in send chat response model 
* adjust logic RemoveEmptyFieldsFromJson to allows removal of a fields with a value of 0 
* **CloudSave:** add cloud save admin game record API 
* making param guard for ServerEcommerce::FulfillUserItem 
* Parity model for FulfillUserItem 
* **predefinedEvent:** add events for chatv2 scope 
* **predefinedEvent:** add some models on Engagement Leaderboard, Seasonpass, and Reward scope 
* **predefinedEvent:** disabling predefinedEventApi 
* **qosm:** allow the game client to specify the qosm URL change to AMS-qosm using DefaultEngine.ini 
* **UGC:** new V2 UGC endpoints 


### Bug Fixes

* compile error on PlayStation build 
* **Ecommerce:** add some validations inside the grant user entitlements 
* fixed cache token 
* fixed cache token 
* move the function from OSS to the utilities function 
* **predefinedEvent:** change event namespace to game namespace 
* remove delegate capture by reference in lambda capture 
* **serverEcommerce:** remove availableDate param when the value is default 

### [24.4.1] (2023-11-13)


### Bug Fixes

* add allowed user id list for official user id 

## [24.4.0] (2023-10-30)


### Features

* add Flight ID across API and Web Socket calls 
* add upd ping pong async task 
* **lobby:** add FriendId in friend interaction response 
* **MPv2:** add API to send DS session ready to session service 
* **predefinedEvent:** add some models on Engagement Achievement scope 
* **predefinedEvent:** add some models on Engagement Group scope 
* **predefinedEvent:** disabling predefinedEventApi 
* **predefinedEvent:** update and add some models on Play scope 
* **qosm:** allow the game client to specify the qosm URL change to AMS-qosm using DefaultEngine.ini 
* **QoS:** use udp ping pong to get latencies 


### Bug Fixes

* change presence broadcast event namespace from publisher namespace to session namespace 
* compile error on PlayStation build 
* enable update session with empty teams 
* fix clang error in linux build 
* fix preprocessor checks for TEMPORARY_ENABLE_COMPAT_CHECK macro 
* given new/proper name for Sellable in FAccelByteModelsItemInfo model 
* Incorrect include paths 
* **predefinedEvent:** change event namespace to game namespace 


### Refactors

* **compatibilityMap:** temporarily disable compatibility map checking 
* validate user id before calling API 

## [24.3.0] (2023-10-12)


### Features

* add flight id to additional login data 
* exposed latest query user orders 
* Flexible bundle pricing 
* platform wrapper interface 
* **predefinedEvent:** add CampaignCodeRedeemedPayload model, add payment payload 
* **predefinedEvent:** add some event on social scope 
* **predefinedEvent:** update and add some models on Play scope 


### Bug Fixes

* capitalization for itemId in FAccelByteModelsPriceDetail 
* compile error on UE5.1 
* compile error on UE5.1 
* compile warnings when using monolithic header 
* custom json policy not working on UE5 
* custom json policy not working on UE5 
* incorrect signature for the parameters used in the lambda function that causes compile errors 
* **lobby:** add missing platform field in FAccelByteModelsUserStatusNotif and FAccelByteModelsGetOnlineUsersResponse 
* **lobby:** ignore empty friendsId for BulkFriendRequest and don't send the request 
* missing Base64 header in the Lobby Api 
* **mpv2:** add missing notification handler for OnSessionEnded 
* **predefinedEvent:** remove UPROPERTY's params so that it won't shown on blueprint 
* **UE:** allow compilation on UE5.2 
* **UE:** allow compilation on UE5.2 
* **UGC:** sync the variables for UGC response 

### [24.2.1] (2023-09-27)


### Bug Fixes

* command line parser will have wrong value when input key contains other param key 

## [24.2.0] (2023-09-25)


### Features

* **QoS:** send multiple ping and calculate average round trip 
* **telemetry:** bringback caching improvement 
* **Telemetry:** make some function protected to allow access by child class 


### Bug Fixes

* **lobby:** add missing platform field in FAccelByteModelsUserStatusNotif and FAccelByteModelsGetOnlineUsersResponse 
* **predefinedEvent:** adjust predefinedEvent models on storage scope 

### [24.1.1] (2023-09-15)


### Bug Fixes

* DefaultGame.ini is altered after build 

## [24.1.0] (2023-09-11)


### Features

* add predefinedEvent api 
* **BinaryCloudSave:** added public binary cloud save APIs 
* check user id validity before calling API 
* **MMv1:** add Region in MMv1 matchmaking notice 
* **MPv2:** add new create matchmaking ticket with more info on error and deprecate the old one 
* **predefinedEvent:** add bool to enable/disable caching, add event timestamp as parameter 
* **predefinedEvent:** add payload with TelemetryBody, add send method with telemetryBody 
* **predefinedEvent:** change to clientTimestamp 
* **Session:** add update session storage methods 


### Bug Fixes

* **AMS:** default AMS config is empty 
* deprecation warning for IWYU support in Unreal Engine 5.2 
* missing Category in UProperty causing an explicit category specifier issue 
* **predefinedEvent:** change UserAgreement_Rejected to UserAgreement_NotAccepted, change UserProfile payload from TArray<FString> to FJsonObjectWrapper 
* **Statistic:** Adjust newest flow to expose which statistic is being updated 

### [24.0.3] (2023-09-08)


### Bug Fixes

* deprecation warning for IWYU support in Unreal Engine 5.2 

### [24.0.2] (2023-09-06)


### Bug Fixes

* **AMS:** DS not connecting to amssim when launched using editor command line 
* **AMS:** settings are not loaded on module startup 

### [24.0.1] (2023-09-04)


### Bug Fixes

* unable to connect to lobby when logged in via 3rd party platform 

## [24.0.0] (2023-08-28)


### ⚠ BREAKING CHANGES

* parsing behavior has been improved and handle quotation mark + backslash

### Features

* add lobby error code for max friend limit reached 
* check user id validity before calling API 
* Expose Fulfill Rewards 
* exposing missing psn sync entitlement multiple service 
* **lobby:** send platform name to lobby when connecting 
* **MMv1:** add Region in MMv1 matchmaking notice 
* SetUserPresenceStatus support JSON 
* **telemetry:** add clientTimestamp 
* **UGC:** add a new sort by value and devide liked content endpoint 


### Bug Fixes

* change I64d to lli 
* **Leaderboard:** Sync the leftover variables for get user rank endpoint 
* lobby not triggering ConnectionClosed after user disconnected because of login ban. 
* Prebuild script failed when path contain spaces 
* remove 'using namespace' in public header 
* **Statistic:** Adjust newest flow to expose which statistic is being updated 
* **switch:** unable to read/write cache file 


### Refactors

* move IsValidUrl to AccelByte Net Utilities 

### [23.7.1] (2023-08-23)


### Bug Fixes

* Prebuild script failed when project path contain spaces 

## [23.7.0] (2023-08-14)


### Features

* bring back oculus iap sync 
* **config:** omit server ClientId and ClientSecret from configuration file on shipping build 
* **launcher:** read command line arguments for Justice Auth Code 
* **telemetry:** add clientTimestamp 
* **UGC:** add a new sort by value and devide liked content endpoint 


### Bug Fixes

* **core:** add scopelock on availableToken to handle multi thread requests 
* lobby not triggering ConnectionClosed after user disconnected because of login ban. 
* Prebuild script failed when path contain spaces 
* removed encoding process in ConvertItemSortByToString utilites fucntion 

### [23.6.1] (2023-08-03)


### Features

* **MPv2:** add teams setting model and auto join flag 

## [23.6.0] (2023-07-31)


### Features

* Add GetMyMatchTickets API 
* add raw platform user id support 
* add UnlinkOtherPlatform and UnlinkAllPlatform into blueprint 
* bring back oculus iap sync 
* Expose Oculus IAP Sync Endpoint 
* **Lobby:** Add incoming and outgoing friend request with timestamp 
* **MPv2:** add accept backfill proposal API with game session info and deprecate the old one 
* revert oculus sync iap changes, will be a hotfix at justice 3.52 


### Bug Fixes

* initialize USTRUCT member to default value 
* **Inventory:** Revert all inventory endpoints since BE isn't ready 
* trigger on connection close delegate when failed reconnecting to websocket 
* **user:** add missing fields in FAccountUserData 

### [23.5.2] (2023-07-25)


### Features

* **launcher:** read command line arguments for Justice Auth Code 

### [23.5.1] (2023-07-20)


### Bug Fixes

* **AMS:** launch param cannot be read properly 
* **chatV2:** update websocket chat session id after connected 
* removed oculus iap sync API function due to incompatibility with AB service. 

## [23.5.0] (2023-07-17)


### Features

* add custom print policy to fix locale issue 
* add raw platform user id support 
* add UnlinkOtherPlatform and UnlinkAllPlatform into blueprint 
* **Error:** add sessionJoinFull error code 
* redirect readme to docs portal 


### Bug Fixes

* initialize USTRUCT member to default value 

### [23.4.2] (2023-07-14)


### Bug Fixes

* added sync IAP & DLC for oculus platform 

### [23.4.1] (2023-07-05)


### Bug Fixes

* error when building Linux Editor 
* Separate UnlinkOtherPlatform API functions for improved functionality and bug fixes 

## [23.4.0] (2023-07-03)


### Features

* add GDPR Api to SDK 
* Add missing fields in the DSM models 
* add missing header in LinkOtherPlatformId 
* add session code management for DS 
* Add Username in the Credentials class 
* Add validity flag in the QoS API class to ensure the object is still valid when OnLoginSuccess triggered 
* **DSHub:** add session member changed notif delegate 
* extend Environment settings by adding sandbox, integration and qa 
* implement presence boardcast event 
* Logs squelching to hide some information in the HTTP request and response by adding the new HTTP header 


### Bug Fixes

* change cache qos get latencies to get active latencies 
* **httpclient:** Fix undefined Content-Type and Accept headers in the HTTP request 
* inconsistent string name on GDPRApi Header 


### Refactors

* **httpcache:** Change CachedItems initialization process as lazy initialization 


### Performance

* Use reference to iterate switches in the ServerDSM API class 

### [23.3.1] (2023-06-23)


### Features

* Add game session code management endpoints 


### Refactors

* Implement ApiRequest with TSharedRef<FJsonObject> json parameter 

## [23.3.0] (2023-06-19)


### Features

* add AccelByte-specific storage file used in some features 
* add missing header in LinkOtherPlatformId 
* cache generated device id 
* **chat:** add event for muted and unmuted 
* **chat:** add group chat moderator api 
* **credentials:** change refresh token retry flow 
* **MPv2:** add error code for party and game session not found 
* new statistic endpoints 
* OIDC Unlink Features 
* **session:** add promote game session leader API for client and DS 


### Bug Fixes

* **Chat:** add missing header for Chat API 
* **log:** fix string format 


### Refactors

* **AMS:** change ServerWatchdog to ServerAMS 
* **httpclient:** change query params to be TMultiMap 

### [23.2.1] (2023-06-05)


### Features

* **UGC:** add a new optional channel name parameter to query a specific channel 


### Refactors

* **HttpClient:** use Request instead of ApiRequest method for specified API function without authorization header check 

## [23.2.0] (2023-06-05)


### Features

* **DS:** add DSM heartbeat API 
* **session:** add query game sessions API for dedicated server 

### [23.1.4] (2023-05-26)


### Features

* **DS:** add DSM heartbeat API 

### [23.1.3] (2023-05-22)

### [23.1.2] (2023-05-17)


### Bug Fixes

* **UGC:** change the content update boolean to false by default 

### [23.1.1] (2023-05-09)


### Bug Fixes

* missing include headers when the project doesn't use shared precompiled headers 

## [23.1.0] (2023-05-08)


### Features

* **HttpClient:** change regex port to have 5 digits 
* leaderboard revamp 
* **lobby:** Add Reconnecting delegate 
* **presence:** limiting bulk User presence to 40 

### [23.0.1] (2023-05-04)


### Bug Fixes

* **lobby:** add missing data members in FAccelByteModelsUsersPresenceNotice 

## [23.0.0] (2023-04-26)


### ⚠ BREAKING CHANGES

* namespace restructurization

### Features

* add new field 
* add SIGTERM catch for kill signal 
* add uparam macro so the parameter will be treated as input instead of output. 
* **MMv2:** add get matchmaking metrics endpoint and queueTime in create match ticket response 
* support build without PCHs 


### Bug Fixes

* **chat:** add missing header file when compiling oss 
* remove using namespace accelbyte from header files 
* resolve conflict 22.0.2 to master branch 


### Refactors

* **MMv2:** change error for invalid MM request to be ErrorCodes::InvalidRequest instaed of 400 

### [22.0.4] (2023-04-10)


### Bug Fixes

* resolve conflict 22.0.2 to release candidate branch 

### [22.0.3] (2023-04-03)


### Bug Fixes

* **openssl:** missing include in openSSL 
* **statistic:** deprecation warning for ANY_PACKAGE in Unreal Engine 5.1 

### [22.0.2] (2023-03-29)

### [22.0.1] (2023-03-29)


### Features

* add public statistic cycle endpoint 
* **chat:** add get system message stats api 
* **DS:** sdk emits StatsD metrics 


### Bug Fixes

* **blueprint:** add mising login & credential function 
* **lobby:** keep lobby connected if refresh token failed and disconnect when token expired 
* **UGC:** UGC missing CustomAttribute field assignment 

## [22.0.0] (2023-03-27)


### ⚠ BREAKING CHANGES

* **chat:** Optional bool enum members changed
* **session:** changed OnSuccess result of ServerSession::QueryPartySessions from FAccelByteModelsV2QueryPartiesResponse to FAccelByteModelsV2PaginatedPartyQueryResult

### Features

* **lobby:** lobby errorNotif will trigger response delegate when there is a request associated with it. 
* **session:** use paginated model in server query party sessions 
* **UserProfile:** add bulk get user profile with array param 


### Bug Fixes

* add missing blueprint function for lobby, party and chat v1 
* **chat:** refactor optional bool model to avoid compile error in mac 
* **HttpCache:** configurable HTTP caching mode (MEMORY & STORAGE) 
* **HTTPCache:** move the free cache to the SDK module shutdown 
* **session:** keep previous ServerSession::QueryPartySessions to avoid breaking change 

### [21.3.2] (2023-03-17)


### Features

* **error:** add WS error code list based on RFC 

### [21.3.1] (2023-03-15)


### Bug Fixes

* add missing blueprint function for lobby, party and chat v1 

## [21.3.0] (2023-03-13)


### Features

* **chat:** implement system inbox message apis 
* **deviceID:** encode the mac address & override in development phase 
* **MPv2:** add error codes for MMv2 ticket conflict and not found 
* **UserProfile:** always remove empty field from update request to prevent validation issue if the field left empty 


### Bug Fixes

* DS crash when sending message to watchdog 
* **MPv2:** fix joinType not removed from request when EMPTY, and fix request capitalization changes by unreal engine. 

## [21.2.0] (2023-02-28)


### Features

* **DS:** add watchdog API 
* **MMv2:** add API calls to manually queue for backfill 
* query user achievement by user id 

### [21.1.1] (2023-02-16)


### Features

* **MMv2:** add API calls to manually queue for backfill 

## [21.1.0] (2023-02-13)


### Features

* add limit and offset parameter to GetAllUserStatItems and GetUserStatItems 
* get game token by publisher token 
* **Group:** Add Group V2 
* Remove some Stadia enumeration variable, because Google is shutting down Stadia. 
* **ugc:** additional custom attributes 


### Bug Fixes

* **MMv2:** fix validation for ticket ID and match pool not aborting requests 

### [21.0.3] (2023-02-06)


### Bug Fixes

* **LRUCache:** remove cache cleanup & it duplicates with the one from HTTP retry scheduler 

### [21.0.2] (2023-02-01)


### Bug Fixes

* **SessionBrowser:** set default create game session current_player to 0 

### [21.0.1] (2023-01-31)


### Bug Fixes

* missing AES_BLOCK_SIZE when compiled in UE 5.1 

## [21.0.0] (2023-01-30)


### ⚠ BREAKING CHANGES

* **OAuth:** Error handler signature changed and IamUrl changed as optional
* The existing var type (TMap<FString, FJsonObjectWrapper> CustomAttributes{};) in a model (FAccelByteModelsPublicUserProfileInfo) is not compatible with server response, we need to adjust it (to FJsonObjectWrapper CustomAttributes{};) to appropriate with server response and also uniformity JsonObject SDK parsing model.
* adjusted JsonObject variable type in FAccelByteModelsPublicUserProfileInfo model to able give json value and uniformity
* adjusted variable type in a model to able give json value and  uniformity

### Features

* add response validation for Login APIs 
* **engagement:** add blueprint function 
* **engagement:** implement new endpoint bulk get user records and game records 
* group api v2 
* handle Third Party Login and Banned Users and update other error login method related 
* Implement get party info from the session service 
* implement HTTP Requests guard to prevent DDoS-ing 
* **module:** Change ABCredentials class to public 
* provided CustomError for LoginWithDeviceId 
* **session:** add parties field in FAccelByteModelsV2GameSessionTeam 
* **storage:** data storage for shipping build windows 
* **turnmanager:** add api to send metric to turn manager 


### Bug Fixes

* error when parsing unidentified enum in http result handler 
* fix missing includes 
* Fixing build errors from merge with 5c5adf8 - feat: add ApiRequest methods with Headers in FHttpClient 
* Json case sensitive error 
* missing includes cause compile errors when unity build is disabled 
* missing includes in OAuth2 API which causes compile errors when unity build is set to false 
* updating HttpClient.ApiReqest to HttpClient.Request 
* updating HttpClient.Request calls to HttpClient.ApiRequest 


### Documentations

* Changed parameter comment mistake 


* Merge branch 'master' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-8327-logindeviceid-customerror 
* Merge branch 'master' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-8327-logindeviceid-customerror 
* Merge branch 'master' into feature/OS-8327-logindeviceid-customerror 


### Refactors

* **OAuth:** Change CustomErrorHandler with OAuthErrorHandler to handle OAuth error specifically 

## [20.5.0] (2023-01-16)


### Features

* achievement query using tag and new get public tag endpoint 
* add ApiRequest methods with Headers in FHttpClient 
* Add validation in the HTTP API calls to check empty auth token and invalid url 
* **cache:** HTTP threadsafe 
* **HTTP:** result handler parsing request/response from cache 
* **lobby:** update all Lobby responses to inherit LobbyBaseResponse class 
* **Reporting:** add chat reporting 
* **session:** add player and DS status V2 


### Bug Fixes

* empty values in required configurations only shown as warnings 
* failed GetCountryFromIP API because empty Bearer authorization header 
* **SessionBrowser:** add missing query param in ServerSessionBrowser::GetGameSessions 

## [20.4.0] (2023-01-03)


### Features

* add handler for unidentified enum values in Json 
* add userId to addToTopic and removeFromTopic event 
* ecommerce display & section (Rotation Shop Items) 
* facilitate pagination param (offset and limit) for GetAllUserStatItems 
* **lobby:** add away availability in lobby presence 
* **lobby:** use custom json object string converter to handle unidentified enum 
* **matchmaking:** implement MMR Rebalance endpoint in server matchmaking 
* **Statistic:** Add get globalstatitembystatcode 
* **Statistic:** Add new struct for globalstat 
* **storage:** specify log directory as data storage destination for non-windows platform 
* **utilities:** bring back and add advance query 
* **utilities:** bring back and add advance query 
* **utilities:** bring back and add advance query 
* **utilities:** bring back and add advance query 
* **utilities:** bring back and add advance query 
* **utilities:** bring back and add advance query 
* **utilities:** bring back and add advance query 
* **utilities:** bring back and add advance query 
* validate HTTP responses when expecting JSON 


### Bug Fixes

* **advanced query:** change data type to receive different data type 
* **advanced query:** change data type to receive different data type 
* **advanced query:** change data type to receive different data type 
* **advanced query:** change data type to receive different data type 
* **advanced query:** change data type to receive different data type 
* **advanced query:** change data type to receive different data type 
* **advanced query:** change data type to receive different data type 
* **advanced query:** change data type to receive different data type 
* **advanced query:** change error log to warning 
* **advanced query:** change error log to warning 
* **advanced query:** change error log to warning 
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool 
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool 
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool 
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool 
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool 
* **advanced query:** change function name and change 'And' and 'Or' functions to return bool 
* **advanced query:** change implementation flow and move it to new files 
* **advanced query:** change implementation flow and move it to new files 
* **advanced query:** change implementation flow and move it to new files 
* **advanced query:** change implementation flow and move it to new files 
* **advanced query:** change implementation flow and move it to new files 
* **advanced query:** change implementation flow and move it to new files 
* **advanced query:** change implementation flow and move it to new files 
* **advanced query:** change implementation flow and move it to new files 
* **advanced query:** change start function using sharedRef 
* **advanced query:** change start function using sharedRef 
* **advanced query:** change start function using sharedRef 
* **advanced query:** change start function using sharedRef 
* **advanced query:** change start function using sharedRef 
* **advanced query:** change start function using sharedRef 
* **advanced query:** change start function using sharedRef 
* **advanced query:** change start function using sharedRef 
* **advanced query:** clean up after rebasing from master 
* **advanced query:** clean up after rebasing from master 
* **advanced query:** clean up after rebasing from master 
* **advanced query:** clean up after rebasing from master 
* **advanced query:** clean up after rebasing from master 
* **advanced query:** rebasing from master again 
* **crash:** fix crash on shutdown due to task graph and tickers being destroyed 
* **entitlement:** unable to query user entitlements with multiple itemIds 
* **heartbeat:** fix compile errors with FRegistry::HeartBeat on Linux 
* remove unused namespace declare 
* **Statistic:** Initialize FDateTime With 0 


### Refactors

* **Statistic:** Change variable name 

## [20.3.0] (2022-12-19)


### Features

* Account linking with one-time code 
* Add new chat service APIs 
* BulkResetMultipleUserStatItemsValue to support OSS 
* expose to support OSS, Reset stats from a specified user 
* **session:** add ENDED and UNKNOWN status in EAccelByteV2GameSessionDsStatus 
* **settings:** sanitize base url on load 
* **Utilities:** disable jwttoken for nintendo switch 


### Bug Fixes

* disable HeartBeat in DS 
* **heartbeat:** fix compile errors with FRegistry::HeartBeat on Linux 
* **session:** remove TextChat field from FAccelByteModelsV2GameSessionBaseRequest. 


### Documentations

* add initial documentation files 
* implement script for automated build 

### [20.2.2] (2022-12-14)


### Bug Fixes

* incorrect IAM url used during refresh token 

### [20.2.1] (2022-12-09)


### Features

* **MPv2:** add new server calls to create game sessions and send invites to game sessions 

## [20.2.0] (2022-12-05)


### Features

* add heartbeat service 
* add missing header 
* Exposing Get Item By Criteria in DS 
* Make the PSN sync endpoint to be able to receive multiple service labels 
* update implementation to support Unreal Engine 5.1 


### Bug Fixes

* **logging:** change verbosity level in the logging with AccelByteReport 
* **settings:** unable to use public OAuth credentials type for Server Settings 

### [20.1.1] (2022-12-01)


### Features

* Add new chat service APIs 
* **session:** add ENDED and UNKNOWN status in EAccelByteV2GameSessionDsStatus 


### Bug Fixes

* **session:** remove TextChat field from FAccelByteModelsV2GameSessionBaseRequest. 

## [20.1.0] (2022-11-21)


### Features

* E-Commerce Loot box 
* expose get all stats from specified user and Get user's specified stat items 
* Expose Get Store List for DS 
* **iam:** expose the Admin Get User's Bans endpoint 
* **session:** add missing to avoid breaking changes 

## [20.0.0] (2022-11-07)


### ⚠ BREAKING CHANGES

* **Session:** Rename FAccelByteModelsV2PartyMembersChangedEvent field from Party to Session
* **session:** model list
-  FAccelByteModelsV2DSStatusChangedNotif.GameServer -> moved to inside FAccelByteModelsV2DSStatusChangedNotif.Session.DSInformation
-  FAccelByteModelsV2PartyMembersChangedEvent.PartyID -> moved inside FAccelByteModelsV2PartyMembersChangedEvent.Party.ID
-  FAccelByteModelsV2PartyMembersChangedEvent.Members -> moved inside FAccelByteModelsV2PartyMembersChangedEvent.Party.Members
- FAccelByteModelsV2GameSessionMembersChangedEvent.SessionID -> moved inside FAccelByteModelsV2GameSessionMembersChangedEvent.Session.ID
- FAccelByteModelsV2GameSessionMembersChangedEvent.Members-> moved inside FAccelByteModelsV2GameSessionMembersChangedEvent.Session.Members

### Features

* add match pool field to game session create and update request models 
* added username field in the FBaseUserInfo function, used when call public get bulk user info API 
* implement get credential turn server 
* OPTIONBOX on EAccelByteEntitlementClass 
* **Session:** add DISCONNECTED and TERMINATED member status enum 
* **session:** add missing to avoid breaking changes 
* **session:** Add session and party info in Game and Party session member changed notif and DS status change notif 
* **Session:** add TIMEOUT in session member status enum 


### Bug Fixes

* crash when static casting session model 
* **Session:** Rename FAccelByteModelsV2PartyMembersChangedEvent field from Party to Session 

## [19.0.0] (2022-10-24)


### ⚠ BREAKING CHANGES

* renamed some variable in item info model

### Features

* **ABUtilities:** add function to get JsonString 
* Bulk get user season progression API 
* command line args switch parser 
* expose update channel public endpoint 
* implement get credential turn server 
* **qos:** re-enable udpecho on android 
* **Session:** add TIMEOUT in session member status enum 


### Bug Fixes

* add some missing headers 


### Reverts

* Revert "feat(session)!: Add session and party info in Game and Party session member changed notif and DS status change notif" 


* Merge remote-tracking branch 'remotes/origin/hotfix/17.0.0' into hotfix/resolve-conflict-17.0.0-master 

## [18.0.0] (2022-10-10)


### ⚠ BREAKING CHANGES

* Renamed some variables in item info model
* renamed FPresenceStatus to FAccelBytePresenceStatus to resolve conflict

### Features

* adjustment for improve entitlement track log 
* banning by device ID (PS, XBOX) 
* Bruteforce protection upon game login 
* expose Get and Update PrivateCustomAttributes 
* **HTTP:** adding custom metadata header to HTTP request 
* **lobby:** add send party notif 
* **lobby:** support for role based matchmaking 
* **lobby:** support rejected status on role based matchmaking 
* update to cover isse When invoke Credentials::GetUserEmailAddress() in AccelByte SDK for UE4 after sign-in, it returns username instead of email address. 


### Bug Fixes

* conflicting FPresenceStatus used in both AccelByte SDK and Vivox SDK 
* **login:** wrong implementation on get user data on successful login 
* support get wallet info old workflow 
* update current user from register and unregister player to session browser 


### Refactors

* change LobbyErrorMessage to static 


* Merge remote-tracking branch 'remotes/origin/hotfix/17.0.0' into hotfix/resolve-conflict-17.0.0-release-candidate 

### [17.0.2] (2022-10-04)


### Features

* **ban:** automatic token refresh when the ban has ended 


### Bug Fixes

* wrong headers included in the source 

### [17.0.1] (2022-09-28)

## [17.0.0] (2022-09-27)


### ⚠ BREAKING CHANGES

* renamed FPresenceStatus to FAccelBytePresenceStatus to resolve conflict
* Wallet::GetWalletInfoByCurrencyCode returns new adjusted model

### Features

* AccelByteUe4Sdk Plug-in's Wallet::GetWalletInfoByCurrencyCode returns a partial response 
* **extension:** add new item type called extension 
* Improve Store Category search results to display all items from its sub-categories 
* **lobby:** NOT BREAKING CHANGE: add reject consent function 
* providing mac address & platform name methods to fulfill client requirement 
* Temporarily cache telemetry data into the Local disk storage 
* walletapi support old walkflow and multiple platform 


### Bug Fixes

* Conflicting FPresenceStatus used in both AccelByte SDK and Vivox SDK 
* Fix data corruption after item deletion 
* PopulatedItemInfo inherits ItemInfo model 
* support get wallet info old workflow 
* update current user from register and unregister player to session browser 

### [16.2.1] (2022-09-15)

## [16.2.0] (2022-09-12)


### Features

* binary storage & refresh token caching 
* IAM Phase 4 Statement & Substitute Implementation 
* **lobby:** NOT BREAKING CHANGE: add reject consent function 
* **MPv2:** Add multiplayer v2 APIs 
* SDK Implementation for UGC service 


### Bug Fixes

* **module:** prevent application stopped and crash reporting when a service is not deployed yet 
* PopulatedItemInfo inherits ItemInfo model 

## [16.1.0] (2022-08-29)


### Bug Fixes

* **Lobby:** fix crash on connecting to lobby 

### [15.1.3] (2022-08-18)


### Features

* **entitlement:** add platform iap sync for single item 
* iam phase 2 statement & substitute implementation 
* IAM Phase 3 Statement & Substitute Implementation 
* **lobby:** refactor LobbyMessageToJson function, handle trailing comma on array of object and an object value 
* **qos:** get ping timeout from GConfig 
* SDK Implementation for UGC service - Get follower count 
* SDK Implementation for UGC service - Get list content 
* SDK Implementation for UGC service - Query by Tags 


### Bug Fixes

* Cannot remember "device_token" if comes from request header, rename header device_token to device-token 
* **settings:** URL didn't change when switching environment 
* SortBy in GetItemByCriteria and UrlEncoded in Query Param Array 

### [16.0.1] (2022-08-18)


### Bug Fixes

* **settings:** URL didn't change when switching environment 

## [16.0.0] (2022-08-15)


### ⚠ BREAKING CHANGES

* group API models change affect function signature

### Features

* adjust code to following new method 
* adjust the code and bring back the code that made some test error 
* **blueprints:** add utilities class 
* **blueprints:** add utilities to get and modify fjsonobjectwrapper 
* changed a value on struct related statistic end point to floating number type 
* expose statistic v2 endpoints 
* expose store Id and list public store 
* fixed typo and comments, wrong param and some struct 
* **iam:** EA Origin Authentication Integration 
* **item:** get Item Dynamic Data 
* New item type for get items like method, option box config. New API call method, get entitlement ownership by item ids. OPTIONBOX ItemType. 
* **party:** add party connect, and disconnect notification 
* Purchasing Requirements 
* **qos:** get ping timeout from GConfig 
* **qos:** reduce udpecho timeout from 10s to 0.6s 
* SDK Implementation for UGC service - "like' a content 
* **sessionbrowser:** add get game sessions by user ids 
* support searching entitlement by feature 


### Bug Fixes

* Group blueprint function, JSON request creation, Group Member Roles model 
* **party:** change all all function, delegate or variable by using PartyMember. Also adding some description on the function. 
* **party:** fix typo on partyMemberLeaveNotice and FPartyMemberDisconnectNotif 
* **party:** Set Deprecation Variable, Function, and Delegate for PartyLeaveNotif. 
* SortBy in GetItemByCriteria and UrlEncoded in Query Param Array 
* **statistic:** wrong query params in GetUserStatItems 

### [15.1.3] (2022-08-18)


### Bug Fixes

* **settings:** URL didn't change when switching environment 

### [15.1.2] (2022-08-05)


### Bug Fixes

* **lobby:** unbind delegates on Lobby destructor 
* **party:** Party leave notif is not triggered 

### [15.1.1] (2022-08-04)

## [15.1.0] (2022-08-01)


### Features

* **party:** add party connect, and disconnect notification 


### Bug Fixes

* **party:** change all all function, delegate or variable by using PartyMember. Also adding some description on the function. 
* **party:** fix typo on partyMemberLeaveNotice and FPartyMemberDisconnectNotif 
* **party:** Set Deprecation Variable, Function, and Delegate for PartyLeaveNotif. 

## [15.0.0] (2022-07-18)


### ⚠ BREAKING CHANGES

* **reporting:** Changed AdditionalInfo datatype from dictionary to a struct / JsonObject later

### Features

* add setby from server response function GetGameRecord 
* Adding SortBy parameter of AccelByteModelsItemCriteria to call GetItemsByCriteria blueprint function 
* **basic:** added category (optional) param in upload generate URL for user content 
* **basic:** Expose generate upload URL 
* **ecommerce:** Support to sync Durable items from Epic Game Store 
* Implement query game record method for ServerCloudSave 
* **item:** get Item Dynamic Data 
* Purchasing Requirements 
* **reporting:** Reporting Signature Adjustment 
* sdk adjustment for avoiding breaking changes and revisioning/reverting of phase 2 cleanup legacy Avatar/User Profile implementation 
* Set the currencyType for Currency.GetCurrencyList 
* **User:** add country and DoB on FAccountUserData 


### Bug Fixes

* add UMETA to the Environment enum class to fix compile errors 
* **blueprints:** add missing category for any class exposed to Blueprints which use UPROPERTY 
* **blueprints:** wrong function name for Fulfillment to redeem code 
* **item:** wrong query parameters creation on GetItemsByCriteria 

## [14.0.0] (2022-07-04)


### ⚠ BREAKING CHANGES

* **Lobby:** - Remove EAccelByteGeneralUserStatus
- Renamed Availability to EAvailability
- Renamed Availability::Availabe to EAvailability::Online
* **iam:** Refactor/removal AvatarURL/User Profile related Implementation

### Features

* **auth:** Add SnapChatAuth Support 
* **iam:** Login with OIDC in SDK 
* **iam:** Refactor Avatar/User Profile Implementation 
* **iam:** SDK Changes for BE Refactor/removal AvatarURL/User Profile related Implementation 
* **lobby:** add Leader field in lobby models 
* **repoting:** add new enum of USER for reporting player or user 
* sdk adjustment for avoiding breaking changes and revisioning/reverting of phase 2 cleanup legacy Avatar/User Profile implementation 
* **Tests:** remove tests from module directory 
* **user:** fix query search users encode twice 


### Bug Fixes

* create ticker alias to support both UE4 FTicker and UE5 FTSTicker 
* wrong implementation causing errors 


### Tests

* add default value to test models and remove unused variables 


### Refactors

* **Lobby:** Change availability type in presence model 

## [13.0.0] (2022-06-20)


### ⚠ BREAKING CHANGES

* new param Source and tags in GrantExpToUser method

### Features

* **credentials:** add function to get IsComply 
* **precompileHeader:** Resolve compile when set BuildConfiguration UsePchFiles as false 
* **seasonpass:** make new params (source & tags) as optional. 
* **user:** add login with refresh token param 
* **user:** fix query search users encode twice 
* **user:** fix query search users encode twice 


### Bug Fixes

* create ticker alias to support both UE4 FTicker and UE5 FTSTicker 
* wrong implementation causing errors 


### seasonpass

* Granted Season Pass EXP Tracking 


### Tests

* **reporting:** add missing object type when submitting report 

### [12.2.1] (2022-06-14)


### Bug Fixes

* OAuth client credentials not set when ApiClient is created without the MultiRegistry 

## [12.2.0] (2022-06-06)


### Features

* **iam:** Account linking handling 
* **iam:** Include platformUserIds field when bulk gets users' basic info 
* **iam:** Need an adjustment to the SDK for the updated Public Search User V3 API, adding offset and limit. 
* **lobby:** implement add friend with public id 
* **lobby:** implement set party size limit 


### Bug Fixes

* **blueprint:** missing Group blueprint object initialization in the ApiClient Blueprint 
* **blueprints:** resolve conflict for hotfix 12.1.1 
* Fix ambiguous function overloads 
* remove pragma once from cpp files 
* **test:** add wait 60s for lobby to refresh mm game mode cache 
* **websocket:** properly clean up websocket object on destroy 


### Tests

* **lobby:** commented lobby channel chat tests 
* **reporting:** make exception if objectId was empty and use dummy objectId following the UUID v4 without hyphen format 

### [12.1.2] (2022-05-31)


### Features

* **DSM:** Add register game session in DSM 
* **entitlement:** add SyncMobilePlatformPurchaseGooglePlay blueprint 


### Bug Fixes

* change the source of truth for OAuth client Id and Secret to Credentials class 
* compile errors when targeting Linux platform due to incorrect code implementation 


### Tests

* update test cases to handle the changes on source of truth for OAuth client credentials 


### Refactors

* rename parameters in the constructor to use different name with the class fields 

### [12.1.1] (2022-05-23)


### Bug Fixes

* **blueprints:** remove const from functions, change blueprintreadonly to blueprintreadwrite 

## [12.1.0] (2022-05-23)


### Features

* **iam:** Expose the Admin Ban User endpoint 


### Bug Fixes

* **lobby:** fix sdk still retry update party storage after retry limit is reached. 
* **test:** add wait 60s for lobby to refresh mm game mode cache 
* **websocket:** properly clean up websocket object on destroy 


### Tests

* **lobby:** commented lobby channel chat tests 

## [12.0.0] (2022-05-09)


### ⚠ BREAKING CHANGES

* **ecommerce:** To get total  balance could not be retrieved from credit wallet response (call via admin end point call) anymore because it’s be a current wallet balance. User should call again get use wallet balance public interface to get total balance.
* Game and user record struct utiization

### Features

* **blueprints:** add Group API blueprint to the API Client blueprint 
* **Credentials:** add method to set clientid and secret directly from gconfig 
* **dsm:** Add custom attribute parameter in register DS and in DSNotice model. 
* **ecommerce:** add BulkGetItemsBySkus API 
* **ecommerce:** add initializer on the model, add integration test when sku is invalid 
* **ecommerce:** Update Unit Test Code for New Behavior Cross Platform Wallet 
* **lobby:** enable trigger error/response callback when failed to deserialize lobby responses. 
* **lobby:** Skip fetching lobby error messages in shipping build 
* **models:** adding ItemSku & Currency into FAccelByteModelsSeasonPassReward 
* **serverecommerse:** fixed params query user entitlement in unit test to make it passes 


### Bug Fixes

* change server refresh token to not pointed to FRegistry 
* **models:** add missing header file 


### Tests

* **lobby:** commented lobby multithread test 
* **lobby:** re-enable channel chat test 


* Refactor Redundancy of Game Record and User Record Cloud Save on Api and Blueprint class 

## [11.0.0] (2022-04-25)


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

* **AccelByteUe4SdkModule:** add environment changed delegate 
* **cloudsave:** Change field Metadata in RecordRequest Input and game record behaviour 
* **cloudsave:** Change field Metadata in RecordRequest Input and game record behaviour 
* **cloudsave:** Change field Metadata in RecordRequest Input and game record behaviour 
* **cloudsave:** updated signature couldsave record in BP 
* **credentials:** add method to check if session is valid 
* **lobby:** add websocket reconnect to lobby if receive close code 4000 
* **MultiRegistry:** Change RemoveApi to bool. NOT BREAKING CHANGE: new API 
* **serverecommerse:** fixed params query user entitlement in unit test to make it passes 
* **serverecommerse:** fixed Proper Query Params for ServerEcommerce::QueryUserEntitlements Url 
* **ServerSettings:** multiple environment support on server settings 
* **Settings:** change way to retrieve settings, from GetDefaut to GConfig 
* **Wallet:** add ListWalletTransactionsByCurrencyCode 


### Bug Fixes

* **lobby:** Fix typo on model name 
* **lobby:** fixed empty session upgrade headers on reconnect 
* **lobby:** remove refresh token delegate when conenction closed. 
* **multiregistry:** Implement input from Ben (SIMS). NOT BREAKING CHANGE: no implementation breaking api 
* **qos:** calculate latencies data even when the scheduler is activated 
* **qos:** wrong ping regions on success mechanism 
* **server lobby:** fix wrong field name in FAccelByteModelsDataPartyResponse 
* **setting:** remove set min latency polling in setting. 
* **user:** change logout to revoke user token instead 


* Merge branch 'master' into feature/OS-6137-Signature-Record-CloudSave-on-BP 
* Merge branch 'master' into feature/OS-6137-Signature-Record-CloudSave-on-BP 
* Merge branch 'feature/OS-6137-Metadata-Field-and-Game-Record' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-6137-Metadata-Field-and-Game-Record 
* Merge branch 'master' into feature/OS-6137-Metadata-Field-and-Game-Record 
* Merge branch 'feature/OS-6137-Metadata-Field-and-Game-Record' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-6137-Metadata-Field-and-Game-Record 
* Merge branch 'feature/OS-6137-Metadata-Field-and-Game-Record' of bitbucket.org:accelbyte/justice-unreal-sdk-plugin into feature/OS-6137-Metadata-Field-and-Game-Record 


### Tests

* disabled test because of major changes in the lobby backend 
* **qos:** Commented GetActiveLatencies test 

### [10.0.1] (2022-04-20)

## [10.0.0] (2022-04-11)


### ⚠ BREAKING CHANGES

* **oauth:** Get DeviceId from FPlatformMisc, FCustomErrorHandler

### Features

* **dbAdapter:** add db adapter interface and sqlite adapter implementation 
* IAM Input validation 
* **lua:** lobby missing blueprint API added along with the functional test 
* **oauth:** bring back old login flow 
* **oauth:** handle verify 2FA code when user enable 2FA from player portal 
* **plugin:** change cisqlite3 plugin to false as default 
* **test:** create agreement admin blueprint 


### Bug Fixes

* disabling getItembyAppId test 
* **lobby:** Fix lobby reconnecting on multiple user connected. 
* missing header #include "JsonObjectWrapper.h" 
* **TokenGenerator:** fix header dir 


### Refactors

* **dbadapter:** Refactor DB adapter and SQLite3 adapter 


### Tests

* **lobby:** fix concurrent test to be more reliable when failing. 
* **qos:** Commented GetActiveLatencies test 

### [9.1.2] (2022-04-07)


### Features

* **qos:** Add get active qos server and latencies 

### [9.1.1] (2022-04-06)


### Bug Fixes

* **blueprint:** update GetApiClient method to be static so it can be used in the Blueprint 
* disabling getItembyAppId test 
* incorrect header path on some files 

## [9.1.0] (2022-03-28)


### Features

* **entitlement:** add get entitlement ownership token & token only function 
* **httpretry:** add functionality to specify custom handlers when receiving certain response code 
* **JwtWrapper:** Create JWT wrapper class 
* **lobby:** add lobby connect using Token Generator 
* server validation for cloud save 
* **serverApiClient:** Add shared server credential 
* **serverApiClient:** Add shared shared server credential 
* **serverApiClient:** Add shared shared server credential. Resolve inline copyright year 
* **serverApiClient:** Rebase to master branch 
* **TokenGenerator:** add EntitlementTokenGenerator 


### Bug Fixes

* add Purchasable field on ItemInfo 
* **Error:** provide correct ErrorCodes to handle user relate Error 
* missing EmailAddress field on FAccountUserData model 


### Tests

* **httpretry:** update tests implementation to use automation spec 

### [9.0.2] (2022-03-23)


### Bug Fixes

* missing header file in Commerce models 

### [9.0.1] (2022-03-22)


### Bug Fixes

* **commerce:** change Ext and LocalExt data types to FJsonObjectWrapper 

## [9.0.0] (2022-03-14)


### ⚠ BREAKING CHANGES

* **user:** change EVerificationContext::UpgradeHeadlessAccount to
EVerificationContext::upgradeHeadlessAccount

### Features

* **cloudsave:** Server Validation for cloud save 
* **HttpCache:** add mechanism to cache http request and response 
* **lua:** add lobby Presence blueprint API for lua test purpose 
* **qos:** Implement Qos Scheduler & Init Trigger 
* **telemetry:** add EventTimestamp automatically inside Send function. 
* **user:** upgrade and verify headless account 


### Tests

* **lobby:** add integration test case for lobby usage in multithread scenario 
* **matchmaking:** refactored create matchmaking channel to setup step. 

## [8.6.0] (2022-03-01)


### Features

* **item:** get item by sku 
* **sessionbrowser:** adding get session by session id on server API. 
* sync twitch drop entitlement api function and model 
* **user:** Login using password grant V3 


### Bug Fixes

* **lobby:** fix order of Lobby::RequestDS parameter to be the same as in header 
* **oauth:** deprecate RevokeToken function and create new RevokeUserToken 

## [8.5.0] (2022-02-14)


### Features

* **entitlement:** add new item sync with google play item function 
* **group:** BP Group Member (individuals) 
* **group:** BP group member (individuals), WIP 
* **group:** BP Group Roles (permissions) 
* **group:** BP multi member actions 
* **group:** Implement group mgmt 
* **group:** Vanilla BP, WIP 
* **httpclient:** adding cancellation token mechanism 
* **lobby:** Add blocked & unblocked user ID on respective response 
* **lobby:** add more fields to MatchmakingNotif 
* **lobby:** Add support for parsing array of json object field type in lobby ws parser. 
* **lobby:** Add User Id in Invite party and kick response models 
* UE4 Group Mgmt (WIP); Unity parity 

## [8.4.0] (2022-01-31)


### Features

* **lobby:** Add blocked & unblocked user ID on respective response 
* **lobby:** add more fields to MatchmakingNotif 
* **lobby:** Add support for parsing array of json object field type in lobby ws parser. 
* **lobby:** Add User Id in Invite party and kick response models 
* **lua:** Add Server Statistic blueprint API 
* **lua:** server Achievement blueprint API 
* **ServerSessionBrowser:** add Blueprint class from ServerSessionBrowser API 
* **UGC:** add UGC API Blueprint from UGC API 


### Bug Fixes

* **ABServerSessionBrowser:** fix build error due to delegates name already used in other class 
* failed compile on PS5 
* missing headers that was not detected when unity build is enabled 
* **missing include:** add missing include on test classes to avoid compile error 
* remove unnecessary code that causes compile error 


### Refactors

* **AccelByteBPApiClient.cpp:** add missing line break before the end of a function 
* **CloudSaveTest.cpp:** remove duplicate "include" 


### Tests

* **leaderboard:** add delay to make sure leaderboard is active when stat code item is updated 
* **seasonpass:** change season type setup to be durable according to BE validation changes. 

## [8.3.0] (2022-01-17)


### Features

* **Achievement:** add Achievement API Blueprint from Achievement API 
* **entitlement:** bringback Sync DLC item 
* **Leaderboard:** add Leaderboard API Blueprint from Leaderboard API 
* **Leaderboard:** add Leaderboard API Blueprint from Leaderboard API 
* **SessionBrowser:** add SessionBrowser blueprint from SessionBrowser api 
* **Statistic:** add Statistic blueprint from Statistic api 


### Bug Fixes

* **lobby:** Remove redundant reconnect call on lobby OnClosed function. 


### Tests

* **joinable test:** add more delay for waiting for dsNotif 
* **lobby:** fix cleanup and makes some tests more isolated. 


### Refactors

* **AccelByteBPApiClient.cpp:** add missing line break to a function to match the format of the rest 
* **SessionBrowser:** fix typo on ufunction macro category 
* **SessionBrowser:** removed testing purpose code 
* **settings:** move environment settings enumeration to appropriate file 

## [8.2.0] (2022-01-04)


### Features

* **blueprint:** agreement blueprint api , closes [#142]
* **cloudsave:** added cloudsave blueprint from cloudsave api 
* **lobby:** add sub_game_mode in optional param struct for lobby SendStartMatchmaking. 
* **reward:** add reward blueprint from reward api 
* **SeasonPass:** add SeasonPass blueprint from SeasonPass api 
* **serverDSM:** Add the blueprint API along with the Server BP multiregistry 
* **serverLobby:** add blueprint for serverLobby API 
* **ServerMatchmaking:** add blueprint class and function for Dedicated Server Matchmaking API 
* **serverOauth:** add blueprint class for ServerOAuth2 API 


### Bug Fixes

* **cloudsave blueprint:** fixed FJsonObjectWrapper not properly converted to FJsonObject 
* **deprecation:** change deprecation indicator from macro to log warning 
* **lobby:** fix lobby violation error when destroying lobby while websocket still have events in queue. 
* **reporting:** missing #include header file 
* **reward blueprint:** add missing macro class specifier 


### Reverts

* **agreement:** re-add agreement blueprint 


### Refactors

* **cloudsave blueprint:** rename FDJsonObject to FDPayloadJsonObject to avoid confusion 


### Tests

* **lobby:** add set general lobby rate limit in lobby test setup. 
* **user:** add additional admin function to create user for testing purpose 

## [8.1.0] (2021-12-20)


### Features

* **blueprint:** catalog, order, entitlement, currency, & wallet blueprint API 
* **shadowban:** Add Delegate Handler for task refresh function and delete it to prevent memory leak 
* **shadowban:** Add Trim and Ignore case on Authorization check, to make sure catch various Bearer typing possibilities. 
* **shadowban:** exclude shadowban trigger from server build 
* **shadowban:** Fix Linux error compile, constructor has wrong order variable definition 
* **shadowban:** switch order of bearer token refresh handle definition, make sure its assigned before refresh token triggered 


### Bug Fixes

* **lobby:** add missing field to several models, allow lobby MessageNotification handle incoming JSON object (curly bracket) 
* Merge back UTF-8 BOM removal and UGC UploadTo 
* Merge tag 8.0.0 for clean cutoff release 


### Tests

* **lobby:** fix InviteRejected test case not setting leave party delegate 
* **serverlobby:** make sure lobby connected before requesting leave party, add more wait time for writing party storage concurrently 

### [8.0.1] (2021-12-10)


### Bug Fixes

* **blueprint:** rename blueprint classes to have AccelByte prefix to avoid conflicts with other classes 

## [8.0.0] (2021-12-06)


### ⚠ BREAKING CHANGES

* **user:** User Blueprint API modified
* **serverDSM)!: Revert "feat(serverDSM:** Revert "feat(serverDSM): add support for DSMC Multi Allocation Server Registration."

### Features

* **friends:** add lobby's friend blueprint API 
* **Jwt:** Add Utilities functions to Validate JWT using JWK 
* **ServerUserApi:** Game Server api to provide DS searchUserOtherPlatform 
* **SyncIOSandApple:** sync purchased item from mobile platform Apple/Google 
* **user:** complete user blueprint API & fix missing UENUM 


### Bug Fixes

* change encoding of uplugin file to UTF-8 without BOM 
* **httpscheduler:** cancel request will trigger callback 
* **httpscheduler:** check empty response when request status succeeded 
* **httpscheduler:** wrong break placement when getting denied status 
* **lobby:** fix Uninitialized NewSessionOnly value. 
* **lobby:** Handling ws message parsing if an array value have empty string as element 
* **UGC:** fix adjustment for backend behavior changes 


### Reverts

* Revert "chore(release): 8.0.0 - commited and tagged by Jenkins" 


### Tests

* **httpscheduler:** update http retry tests to check cancelled state 
* **lobby:** add wait ds notif to 60 sec 
* **lobby:** LobbyMatchmakingAllParam case cleanup add wait 10 sec for mmNotif to account for transport delay. 


### Refactors

* **serverDSM)!: Revert "feat(serverDSM:** add support for DSMC Multi Allocation Server Registration." 

### [7.2.1] (2021-11-24)


### Bug Fixes

* **telemetry:** add method to flush pending events without waiting for the next batch in GameTelemetry 
* **telemetry:** add method to flush pending events without waiting for the next batch in ServerGameTelemetry 

## [7.2.0] (2021-11-22)


### Features

* **Lobby BP:** Add non-static Lobby blueprint 
* **Lobby:** add start matchmaking with optional param struct, also add NewSessionOnly in matchmaking optional param 
* **Matchmaking BP:** Add matchmaking blueprints API 
* **serverDSM:** add support for DSMC Multi Allocation Server Registration. 
* **User:** get bulk userinfo 


### Bug Fixes

* **lobby:** fix Uninitialized NewSessionOnly value. 
* **lobby:** Handling ws message parsing if an array value have empty string as element 


### Refactors

* **Lobby blueprint:** rename functions to resemble their message types 
* **lobby:** refactor lobby API 
* **serverDSM): Revert "feat(serverDSM:** add support for DSMC Multi Allocation Server Registration." 

### [7.1.1] (2021-11-12)


### Bug Fixes

* **websocketclient:** uninitialized variables cause undefined behavior 

## [7.1.0] (2021-11-08)


### Features

* **GetCurrentUserProgression:** add GetCurrentUserProgression 
* **lobby:** add API to request DS for custom game session 
* **sessionbrowser:** add APIs to manage custom game session 
* **utilities:** add UEnum to FString conversion and vice versa 
* **websocket:** add scheduler to make sure websocket messages execute in gamethread. 


### Bug Fixes

* **MultiRegistry:** Fix double Free calls from ApiClient when using shared credential 
* **settings:** Resolve compile warning in AccelByte Setting in ResetSettings 
* **websocket:** attempt to clear all ws event before disconnecting & clear prev ws events on connect 
* **websocket:** make sure OnConnect event is called before disconnecting. 
* **websocket:** solve IsConnected returned true before connect event is triggered 


### Refactors

* **lobby:** message id is now taken into account when triggering a esponse delegate 
* **lobby:** moving to switch from if conditionals 
* **lobby:** Update some LobbyResponse string format, add const string for "suffix" namespace 
* **websocket:** extract websocket management from lobby to it's own class 


### Tests

* **lobby:** add simple test AccelByte.Tests.Lobby.B.MessageIdCachedResponse 
* **lobby:** fix matchmaking random timeout after StartMatchmakingAllParameters Test & increase wait matchmaking notif timeout 
* **lobby:** Increase timeout for Matchmaking and ReadyConsent notif as original value is too low 
* **sessionbrowser:** add integration tests for custom game session 

### [7.0.3] (2021-10-27)


### Refactors

* **compatibility:** add flag to ignore comparing patch number in the version 
* **compatibility:** use LogWarning when version mismatch happened for less disruptive process 


### Tests

* **compatibility:** add new test cases for ignoring patch number 

### [7.0.2] (2021-10-26)


### Tests

* **lobby:** fix matchmaking random timeout after StartMatchmakingAllParameters Test & increase wait matchmaking notif timeout 


### Refactors

* **ServerDSM:** Remove fetching public IP when registering cloud server 

### [7.0.1] (2021-10-25)


### Bug Fixes

* **lobby:** return invalidRequest when UserIds empty on bulk user presence 


### Refactors

* **apiclient:** default ApiClient will use Credentials and HttpRetryScheduler instances from FRegistry 

## [7.0.0] (2021-10-11)


### ⚠ BREAKING CHANGES

* **ServerLobby:** remove GetActiveParties method

### Features

* **DSM:** Save server info when registered into variable and add Getter function 
* **ecommerce:** Add GetUserEntitlementOwnershipByItemId 
* **ecommerce:** remove returnURL on CreateNewOrder if empty 
* implement qos turn manager to get the closest TURN server 
* **reward:** Add new Reward Endpoint and Integration Test 
* **settings:** change ResetSettings param from FString to enum ESettingsEnvironment 


### Bug Fixes

* **CheckServicesCompatibility:** Only check compatibility for non shipping build 
* **lobby:** linux build failure due to werror on unreal engine > 4.25 
* **sdkModule:** remove GetDefaultUrl, change it to call from Settings instead 
* **seasonpass:** season pass tier duplicates when there are more than 1 pass codes 
* **User:** change UpdateUser verb from PUT to PATCH 


### Tests

* **ParseErrorTest:** add unit test for service error parser 


### Refactors

* **ServerLobby:** remove GetActiveParties method 

### [6.1.1] (2021-10-01)


### Tests

* **Reporting:** add missing include 

## [6.1.0] (2021-09-27)


### Features

* **ecommerce:** add epicgames entitlement on SyncPlatformPurchase 
* **ecommerce:** add Media Item type 
* **lobby:** add lobby ws request error handler and error message 
* **misc:** add GetServerCurrentTime 
* **shadowban:** Add refresh token when token revoked. Pause task and resume after new token retrieved 
* **Version:** Ensure version is compatible 


### Tests

* **agreement:** revision 

### [6.0.1] (2021-09-16)


### Bug Fixes

* **User:** change UpdateUser verb from PUT to PATCH 

## [6.0.0] (2021-09-13)


### ⚠ BREAKING CHANGES

* **httpclient:** change methods to return FAccelByteTaskPtr so that http request can be cancelled

### Features

* **ecommerce:** new currency list and cancel order endpoint 
* **ecommerce:** New CurrencyList and CancelOrder endpoint 
* **ecommerce:** New CurrencyList and CancelOrder endpoint 
* **ecommerce:** New CurrencyList and CancelOrder endpoint 
* **ecommerce:** New CurrencyList and CancelOrder endpoint 
* **FHttpClient:** make building url easier with namespace and user id token replacement with values in credentials 
* **httpclient:** change methods to return FAccelByteTaskPtr so that http request can be cancelled 
* **lobby:** add unbind event per lobby scope 
* **lobby:** add unbind event per lobby scope 
* **MultiRegistry:** Add "default" key parameter to GetClientApi static method 
* **MultiRegistry:** Add"default" key parameter to GetClientApi static method (again) 
* **reporting:** re-add reporting SDK 
* **User:** Add LoginWithRefreshToken 


### Bug Fixes

* **lobby:** fixed PartyDataUpdateNotif Leader field typo. 


### Refactors

* revert back changes that cause errors 


### Tests

* **ecommerce:** update Ecommerce Test Admin to fix some crashes during Ecommerce Setup process 
* **ecommerce:** use valid uuid format for testing item that does not exist + move ServerEntitlementFulfillInvalidFailed to integration tests 
* **lobby:** fix intermttent test error on rematchmaking 
* **serverlobby:** fix intermittent test error in get set session attribute 

## [5.0.0] (2021-08-30)


### ⚠ BREAKING CHANGES

* **Achievement:** Change the type of GoalValue of FAccelByteModelsPublicAchievement and FAccelByteModelsMultiLanguageAchievement and model from int32 into float.

### Features

* **Error:** parse oauth error response 
* **Error:** parse oauth error response 
* **FApiClient:** add GetApi method template to support API extension 
* **FHttpClient:** implement http client to make api request easier 
* **User:** Add Logout api 


### Bug Fixes

* **Achievement:** Fix the Achievement Api's Public Achievement and Multi Language Achievement model difference with the Backend response. Fix the Test Helper's Achievement Request and Achievement Response model difference with the Backend response. 
* **Subscription:** Fix subscription integration test. 


### Refactors

* **FApiClient and FRegistry:** Each API receive FHttpRetryScheduler reference from constructur instead of directly referring the singleton 


### Tests

* **agreement:** make more resistant to dirty data 
* **FHttpClient:** add core test for FHttpClient 
* **lobby:** fix LobbyTestStartMatchmakingTempPartyOfTwo_ReturnOk 

### [4.0.1] (2021-08-16)

## [4.0.0] (2021-08-02)


### ⚠ BREAKING CHANGES

* **Achievement:** Change the type of LatestValue of FAccelByteModelsUserAchievement model from int32 into float.

### Features

* add awscognito platform type 
* Add digital signature check to plugin 
* **core:** add scope lock-free mechanism on HttpRetryScheduler for thread-safety , closes [#2]
* **jenkins:** jenkinsfile - remove ue 4.20, 4.21 and add 4.26 
* **jenkins:** make build linux & windows parallel + separate build editor and game to make ci faster 
* **jenkins:** use the new justice-sdk-builder 
* **lobby:** add get & getAll session Attributes ws commands 
* **lobby:** add GetOnlineFriendPresence, and marked OnGetAllUSerPresence obsolete. 
* **lobby:** Generate party code 
* **serverloby:** add Set, Get, GetAll user session attribute 
* **session browser:** add get session data by session id 
* **ue5ea:** Jenkins - Build.ps1 - remove superficial option ue4exe 
* **ue5ea:** Jenkins - RunTests.ps1 - add alternate unreal editor exe for UE5 and UE4 
* **ue5ea:** jenkins.config.json - add ue 5.0ea 
* **unrealengine:** remove support for ue < 4.25 


### Bug Fixes

* **Achievement:** Fix the User Achievement model difference with the Backend response. 
* **files order:** Fix signature algorithm to ensure files and directories order consistent and update digital signature 
* **lobby:** Response.Code always empty 
* **Oauth2:** Fix the GetTokenWithAuthorizationCode() using v1 endpoint 
* **Telemetry:** Change GetSeconds() to GetTotalSeconds 
* **UGC:** Fix the integration test naming 
* **UGC:** Rewrite the inline documentation: fixing from copy-paste mistake, and not standard writing. Fix the copyright. 
* **uplugin:** remove unicode literal in PrebuildSteps 


### Documentations

* add version.json, changelog, and readme 


### Tests

* **AUser:** fix test regression due to invalid partial query result assertion 
* **AUser:** make STEAM_KEY environment variable optional 
* **basic:** check steam ticket before testing with steam in AUser tests 
* **general:** fix incorrect token retrieval for the admin user in test utilities , closes [#19]
* **play:** add random element in lobby user test emails 
* **play:** fixed lobby, serverlobby, joinablesession & session browser tests , closes [#375]
* **subscription:** fix error categoryPath /game 

## [3.2.0] (2021-07-07)


### Features

* **Achievement:** remove redundant test code + fix identifier naming according to ue4 guideline 
* **jenkins:** Convert scripts to powershell 
* **jenkins:** Convert scripts to powershell 
* **Json:** Remove empty string fields from FJSonObject 
* **lobby:** add errorNotif and move admin function into new file 
* **lobby:** Add Send Notification to User 
* **ugc:** Add UGC into SDK 
* **UserProfile:** add BatchGetPublicUserProfileInfos 
* **UserProfile:** Get custom attribute other user id 


### Bug Fixes

* **CloudSaveTest:** Disable concurrent record test 
* **CloudSaveTest:** Disable concurrent record test 
* **Jenkins:** Revert unfinished work to convert scripts to powershell 
* **User test:** test failed due to missing waiting 


### Reverts

* Revert "test(Achievement): make test variable declarations static to keep it local + make the names shorter" 
* Revert "test(MultiRegistry): make test variable declarations static to keep it local + make the names shorter" 
* Revert "test(GameProfile): make test variable declarations static to keep it local + make the names shorter" 
* Revert "test(UserIntegration): make test variable declarations static to keep it local + make the names shorter" 


### Tests

* **Achievement:** deduplicate QueryAchievement tests 
* **Achievement:** deduplicate QueryAchievement tests (more) 
* **Achievement:** remove unnecessary http flush 
* **Achievement:** rename identifiers according to ue4 guideline 
* **Achievement:** setup - use reference on achievements loop 
* **CloudSave & GameTelemetry:** change check macro with AB_TEST_XXXX that doesn't crash if failed 
* **eCommerce:** Change check macro to AB_TEST_XXX to not crash test automation 
* **Ecommerce:** fix sdk tests (max draft 1 be changes, remove unnecessary http flush, remove code redundancy) 
* **lobby:** fix lobby test stuck , closes [#364]
* **MultiRegistry:** use setup/teardown test users from utilities 
* **serverlobby:** fix serverlobby test cases stuck , closes [#366]
* **Social:** change check macro into AB_TEST_XXX to not crash automated test 
* **UserIntegration:** BatchGetPublicUserProfileInfos - use setup/teardown test users from utilities 
* **UserIntegration:** BatchGetPublicUserProfileInfos - use setup/teardown test users from utilities (more) 
* **UserIntegration:** clean up 
* **UserIntegration:** fix error due to missing waiting in GetUserByDisplayName, GetUserByEmailAddress, GetUserByUsername 
* **User:** Replace check macro with more descriptive TestXXX-based AB_TEST_XXX macros 
* **Utilities:** move LatenciesPredicate to its proper place 
* **Utilities:** remove redundant get required environment variable logic 

## [3.1.0] (2021-06-23)


### Features

* **apigw:** remove apigw and adjust endpoint, add Registerv3 accept legal policy when create new account 
* **FMultiRegistry,FApiClient:** allow multiple users to be signed in at one 
* **lobby:** Bulk get user presence 
* **lobby:** Invite and Join Party via Code 
* **lobby:** Promote party leader request 
* **lobby:** Reject party invitation 
* **Party:** Get party data by party id 


### Bug Fixes

* **Tests.AUserProfile:** remove redundant delete profile function 
* **Tests.AUserProfile:** remove redundant delete profile function 

## [3.0.1] (2021-06-09)


### Features

* **ecommerce:** add some entitlement endpoint to server sdk 
875da0fe172fc5120ed60ca9fc276d80302fd2cd))
* **lobby:** unfriend, cancel friend, and reject friend notification 

### Bug Fixes

* **user:** Fix login with other platform function 
* **entitlement:** change appid to string ([78592c5]

## [3.0.0] (2021-06-09)


### ⚠ BREAKING CHANGES

* **AUser.Tests:** Fix breaking change caused by removal sensitive info
* **tests.AUser:** Fix breaking change caused by removal sensitive info

### Features

* **ecommerce:** redeem code feature from VE repo 
* **lobby:** add lobbySessionId to be able to reconnect using same token 


### Bug Fixes

* **AUser.Tests:** Fix breaking change caused by removal sensitive info 
* **log:** github sync, removed nonfunctional verbosity set functions. 
* **tests.AUser:** Fix breaking change caused by removal sensitive info 
* error compile need to include AccelByteError.h on file AccelByteUtilities.h , closes [#312]

## [2.35.0] (2021-05-25)


### Features

* **dsm:** add RegisterLocalServer with public ip address & moved GetPublicIp to AccelByteUtilites , closes [#307]
* **lobby:** add SetSessionAttribute function & matchmaking with extra attribute field , closes [#303]


### Bug Fixes

* **Utilities:** fix FAccelByteModelsPubIp undefined in UE4 4.20 , closes [#310]

## [2.34.0] (2021-04-28)


### Features

* **cloudsave:** add Cloud Save APIs for Server SDK 
* **ecommerce:** add bulk entitlement lookup 
* **ecommerce:** add Debit wallet 
* **lobby:** add custom port field in DSNotice lobby model. , closes [#288]
* **lobby:** add custom port field in DSNotice lobby model. 
* **server:** Re-Login mechanism to renew token on Server SDK 


### Bug Fixes

* **ecommerce:** fix integration test for bulk query entitlement 


### Tests

* **dsmconfig:** fix missing protocol field in dsmconfig , closes [#294]

## [2.33.0] (2021-04-14)


### Features

* **entitlement:** add entitlement sync for 3rd party platform purchases , closes [#272]
* **iam:** add endpoint to retrieve JWT from particular session id on apigw 
* **server:** remove agones 


### Bug Fixes

* **http:** fixed http flush on shutdown 
* **http:** Not started http requests needs to wait for timeout before return error 
* **lobby:** create a new websocket instance when connection state error 
* **lobby:** fixed websocket instance connection error state reconnecting 

## [2.32.0] (2021-03-17)


### Features

* Add ability to set AccelByteReportLog's verbosity 
* add session browser api , closes [#270]
* add signaling p2p to lobby , closes [#263]
* **plugin:** remove platform whitelist 


### Bug Fixes

* **error:** check field before try parsing 
* AccelByteUe4SdkDemo project blueprint 
* Remove #pragma once from cpp file, which can cause compilation failures on some platforms. 
* **error:** check field before try parsing 


### Tests

* **joinable:** fix joinable test case failed because of persisting party between test session 

## [2.31.0] (2021-03-03)


### Features

* **partyStorage:** add party storage client function 
* **serverLobby:** add party storage 


### Bug Fixes

* **chore:** remove array 
* **serverLobby:** simplify baseUrl 
* fixed request content was not encoded for type x-www-form-urlencoded 

## [2.30.0] (2021-02-17)


### Features

* **config:** make service url config optional 
* **Matchmaking:** implement joinable session , closes [#258]
* **Server:** Add query party by userId in server , closes [#242]
* **user:** add login with other platform ps5 enum 


### Bug Fixes

* undefined ENGINE_MINOR_VERSION preprocessor on AccelByteUserModels.h 
* **GetDeviceId:** Suppress deprecation warning for GetDeviceId 

### [2.29.2] (2021-02-03)


### Bug Fixes

* **dsm:** fixed parse match request content length checking lead to uninvoked callback 

## [2.29.0] (2021-01-20)


### Features

* **Automation Metric:** SDET-1300 Integrate SDK Test Result to Automation Metrics , closes [#237]
* **Automation Metric:** SDET-1300 Integrate SDK Test Result to Automation Metrics 
* **core:** remove AccelByteCustomWebBrowserWidget 
* **core:** support UE 4.26 


### Bug Fixes

* **config:** add config null check 

## [2.28.0] (2021-01-06)


### Features

* **dsm:** adjust get DSM region url, get region and provider value from run args, select DSM client based on region and provider (backward compatible) 

## [2.27.0] (2020-12-10)


### Features

* **lobby:** parse object value, parse quoted array element, only show parse error for known messages 


### Bug Fixes

* **lobby:** fixed lobby crash on exit 

## [2.26.0] (2020-11-25)


### Features

* **lobby:** add party attribute option for the StartMatchmaking request 


### Bug Fixes

* **partyAttribute:** restrict the matchmaking party attribute to Map<string, string> and affect the sent request + heartbeat parsing at the server 

## [2.25.0] (2020-10-28)


### Features

* **user:** add epicgames and stadia platformType 


### Bug Fixes

* crash on RemoveTicker on game telemetry API 
* GetPlatformLinks and UnlinkOtherPlatform namespace 

## [2.24.0] (2020-10-14)


### Features

* **User:** add search filter param in SearchUser 


### Tests

* **Daily Test:** SDET-1166 Run SDK Automation Test Daily and Send report to slack channel 

## [2.23.0] (2020-10-01)


### Features

* **lobby:** implement global chat 
* **user:** add username parameter for register, upgrade, and search 

## [2.22.0] (2020-09-16)


### Features

* **Subscription:** add CheckEligibleUserCheck blueprint node 


### Bug Fixes

* **Subscription:** updated check flow to latest using check any entitlement ownership API 


### Tests

* **Achievement:** fix achievement invalid ID test to comply to UUIDv4 format 
* **ecommerce:** fix invalid case because not UUIDv4 format 
* **ecommerce:** wrong UUIDv4 format 
* **Ecommerce:** fix another invalid id error 
* **sub:** changed sub test users 
* **subscription:** make app id static in sub test 

## [2.21.0] (2020-09-02)


### Features

* **User:** added `ps4` and xbox `live` enum for 3rd party login 


### Bug Fixes

* **config:** fix DefaultEngine.ini default server setting value 

## [2.20.0] (2020-08-19)


### Features

* **Achievement:** bringback achievement from VE 


### Bug Fixes

* **test:** additional incremental Achievement test 
* fix ue 4.25 build 


### Tests

* **gameTelemetry:** fix game telemetry test 


### CI

* **jenkins:** add target parameters 

## [2.19.0] (2020-08-05)


### Features

* game telemetry scheduler 


### Bug Fixes

* **cloudstorage:** Fix CloudStorage API documentation. 
* **cloudstorage:** Mark Status field as DEPRECATED and add Size field in the FAccelByteModelsSlot model. 
* **cloudstorage:** Mark UpdateSlotMetadata function with filename  parameter as deprecated 
* **cloudstorage:** Overload UpdateSlotMetdata()  function. 
* **config:** remove leaderboard server URL from server SDK config 
* **jenkins:** add SERVER_STATISTIC_PREFIX for jenkins config 
* **statistic:** Update statistic error codes. 


### CI

* **jenkins:** temporary social service env flag 

### [2.18.6] (2020-07-22)


### Refactors

* **envar:** clean up warning and refactor envar 

### [2.18.5] (2020-07-08)

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
