# Blueprints #

## Table of Contents ##

- [Table of Contents](#table-of-contents)
- [Overview](#overview)
- [Identity Service](#identity-service)
- [Platform Service](#platform-service)
    - [Catalog](#catalog)
    - [Purchases](#purchases)
    - [User Profile](#user-profile)

## Overview ##

We have blueprint version for each C++ Function on each services. The blueprint function is available in two flavors.

1. As an Async Action Base single node with multiple output pin, whether it succeeds or fails
1. As a Function Library using delegate node to handle callback

## IDENTITY SERVICE ##

This is the first required service before we can proceed to platform service.
<!--language lang-cs -->

### UserLogin ###

This blueprint is similar to `JusticeIdentity::UserLogin` function. You can use this blueprint function to login with email and password. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask User Login](images/bp_asynctask_userlogin.PNG?raw=true "Blueprint AsyncTask User Login")

#### Function Library flavor ####

![Blueprint Function Library User Login](images/bp_functionlibrary_userlogin.PNG?raw=true "Blueprint Function Library User Login")

### Register New Player ###

This blueprint is similar to `JusticeIdentity::RegisterNewPlayer` function. You can use this blueprint to register new player using email and password. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask Register New Player](images/bp_asynctask_registernewplayer.PNG?raw=true "Blueprint AsyncTask Register New Player")

#### Function Library flavor ####
![Blueprint Function Library Register New Player](images/bp_functionlibrary_registernewplayer.PNG?raw=true "Blueprint Function Library Register New Player")

### ReissueVerificationCode ###

This blueprint is similar to `JusticeIdentity::ReissueVerificationCode` function. You can use this blueprint function to send user verification code to their email. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Reissue Verification Code](images/bp_asynctask_reissueverificationcode.PNG?raw=true "Blueprint AsyncTask Reissue Verification Code")

### VerifyNewPlayer ###

This blueprint is similar to `JusticeIdentity::VerifyNewPlayer` function. You can use this blueprint function to verify newly created account with verification code that user receives in their email. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask Verify New Player](images/bp_asynctask_verifynewplayer.PNG?raw=true "Blueprint AsyncTask Verify New Player")

#### Function Library flavor ####

![Blueprint Function Library Verify New Player](images/bp_functionlibrary_verifynewplayer.PNG?raw=true "Blueprint Function Library Verify New Player")

### Device Login ###

This blueprint is similar to `JusticeIdentity::DeviceLogin` function. You can use this blueprint function to login with device ID. We use value from `FGenericPlatformMisc::GetDeviceId()` as Device ID. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Device Login](images/bp_asynctask_devicelogin.PNG?raw=true "Blueprint AsyncTask Device Login ")

### Auth Code Login ###

This blueprint is similar to `JusticeIdentity::AuthCodeLogin` function. You can use this blueprint function to login from AccelByte's Launcher. We send authentication code from launcher, in the SDK the authentication code is grabbed for login. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Auth Code Login](images/bp_asynctask_authcodelogin.PNG?raw=true "Blueprint AsyncTask Auth Code Login")

### UpgradeHeadlessAccount ### 

This blueprint is similar to `JusticeIdentity::UpgradeHeadlessAccount` function. You can use this blueprint function to upgrade headless accounts (such as from Steam, Facebook and Google) into account using email. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Upgrade Headless Account](images/bp_asynctask_upgradeheadlessaccount.PNG?raw=true "Blueprint AsyncTask Upgrade Headless Account")

### ResetPassword ###

This blueprint is similar to `JusticeIdentity::ResetPassword` function. You can use this blueprint function to reset user’s password. User would receive verification code to reset their password. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask Reset Password](images/bp_asynctask_resetpassword.PNG?raw=true "Blueprint AsyncTask Reset Password")

#### Function Library flavor ####

![Blueprint Function Library Reset Password](images/bp_functionlibrary_resetpassword.PNG?raw=true "Blueprint AsyncTask Reset Password")

### LinkPlatform ###

This blueprint is similar to `JusticeIdentity::LinkPlatform` function. You can use this blueprint function to link your account into any platform (e.g. Steam, Google, Facebook). It comes only in Async Action Base flavor.

#### Async Action Base flavor ####
![Blueprint AsyncTask Link Platform](images/bp_asynctask_linkplatform.PNG?raw=true "Blueprint AsyncTask Link Platform")

### UnlinkPlatform ###

This blueprint is similar to `JusticeIdentity::UnlinkPlatform` function. You can use this blueprint function to unlink your account from any platform (e.g. Steam, Google, Facebook). It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Unlink Platform](images/bp_asynctask_unlinkplatform.PNG?raw=true "Blueprint AsyncTask Unlink Platform")

### GetLinkedPlatform ###

This blueprint is similar to `JusticeIdentity::GetLinkedPlatform` function. You can use this blueprint function to get list of platforms (e.g. Steam, Google, Facebook) linked to your account. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Get Linked Platform](images/bp_asynctask_getlinkedplatform.PNG?raw=true "Blueprint AsyncTask Get Linked Platform")

### User Logout ###

This blueprint is similar to `JusticeIdentity::UserLogout` function. You can use this blueprint function to logout from current user session. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask User Logout](images/bp_asynctask_userlogout.PNG?raw=true "Blueprint AsyncTask User Logout")

#### Function Library flavor ####

![Blueprint Function Library User Logout](images/bp_functionlibrary_userlogout.PNG?raw=true "Blueprint Function Library User Logout")
