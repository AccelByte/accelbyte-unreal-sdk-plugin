
## TABLE OF CONTENT ##
- [Table of Content](#table-of-content)
- [Overview](#overview)
- [Identity Service](#identity-service)
- [Platform Service](#platform-service)
    - [Catalog](#catalog)
    - [Purchases](#purchases)
    - [User Profile](#user-profile)

## OVERVIEW  ##
We have blueprint version for each C++ Function on each services. The blueprint function available in two flavor.

1. As an Async Action Base single node with multiple ouput pin wheater is success or failed
1. As an Function Library using delegate node to handle callback

## IDENTITY SERVICE ##
This is the first service that required before we can proceed to platform service. Construct an IdentityService object to interact with services.
<!--language lang-cs -->

### UserLogin ###
This blueprint is similar to `JusticeIdentity::UserLogin` function. You can use this blueprint function to login with email and password. It come with two flavor: Async Action Base and Function Library.

#### Async Action Base flavor ####
![Blueprint AsyncTask User Login](images/bp_asynctask_userlogin.PNG?raw=true "Blueprint AsyncTask User Login")

#### Function Library flavor ####
![Blueprint Function Library User Login](images/bp_functionlibrary_userlogin.PNG?raw=true "Blueprint Function Library User Login")


### Register New Player 
This blueprint is similar to `JusticeIdentity::RegisterNewPlayer` function. You can use this blueprint to register new player using email and password. It come with two flavor: Async Action Base and Function Library.

#### Async Action Base flavor ####
![Blueprint AsyncTask Register New Player](images/bp_asynctask_registernewplayer.PNG?raw=true "Blueprint AsyncTask Register New Player")

#### Function Library flavor ####
![Blueprint Function Library Register New Player](images/bp_functionlibrary_registernewplayer.PNG?raw=true "Blueprint Function Library Register New Player")


### ReissueVerificationCode
This blueprint is similar to `JusticeIdentity::ReissueVerificationCode` function. You can use this blueprint function to send user verification code to their email. It come only in Async Action Base flavor.

#### Async Action Base flavor ####
![Blueprint AsyncTask Reissue Verification Code](images/bp_asynctask_reissueverificationcode.PNG?raw=true "Blueprint AsyncTask Reissue Verification Code")



### VerifyNewPlayer
This blueprint is similar to `JusticeIdentity::VerifyNewPlayer` function. You can use this blueprint function to verify newly created account with verification code that user receive in their email. It come with two flavor: Async Action Base and Function Library.

#### Async Action Base flavor ####
![Blueprint AsyncTask Verify New Player](images/bp_asynctask_verifynewplayer.PNG?raw=true "Blueprint AsyncTask Verify New Player")

#### Function Library flavor ####
![Blueprint Function Library Verify New Player](images/bp_functionlibrary_verifynewplayer.PNG?raw=true "Blueprint Function Library Verify New Player")

### Device Login ###
This blueprint is similar to `JusticeIdentity::DeviceLogin` function. You can use this blueprint function to login with device ID. We use value from `FGenericPlatformMisc::GetDeviceId()` as Device ID. It come only in Async Action Base flavor.

#### Async Action Base flavor ####
![Blueprint AsyncTask Device Login](images/bp_asynctask_devicelogin.PNG?raw=true "Blueprint AsyncTask Device Login ")

### Auth Code Login ###
This blueprint is similar to `JusticeIdentity::AuthCodeLogin` function. You can use this blueprint function to login from Accelbyte's Launcher. We send auth code from launcher, and on the sdk get grab the Auth Code to login with it. It come only in Async Action Base flavor.

#### Async Action Base flavor ####
![Blueprint AsyncTask Auth Code Login](images/bp_asynctask_authcodelogin.PNG?raw=true "Blueprint AsyncTask Auth Code Login")

### UpgradeHeadlessAccount ### 
This blueprint is similar to `JusticeIdentity::UpgradeHeadlessAccount` function. You can use this blueprint function to upgrade from headless account (such as steam, facebook, google) into email account. It come only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Upgrade Headless Account](images/bp_asynctask_upgradeheadlessaccount.PNG?raw=true "Blueprint AsyncTask Upgrade Headless Account")

### ResetPassword
This blueprint is similar to `JusticeIdentity::ResetPassword` function. You can use this blueprint function to reset user password, user will receive verification code to reset their password. It come with two flavor: Async Action Base and Function Library.

#### Async Action Base flavor ####
![Blueprint AsyncTask Reset Password](images/bp_asynctask_resetpassword.PNG?raw=true "Blueprint AsyncTask Reset Password")

#### Function Library flavor ####
![Blueprint Function Library Reset Password](images/bp_functionlibrary_resetpassword.PNG?raw=true "Blueprint AsyncTask Reset Password")


### LinkPlatform ###
This blueprint is similar to `JusticeIdentity::LinkPlatform` function. You can use this blueprint function to link your account into any platform(steam, google, facebook). It come only in Async Action Base flavor.

#### Async Action Base flavor ####
![Blueprint AsyncTask Link Platform](images/bp_asynctask_linkplatform.PNG?raw=true "Blueprint AsyncTask Link Platform")

### UnlinkPlatform ###
This blueprint is similar to `JusticeIdentity::UnlinkPlatform` function. You can use this blueprint function to your unlink account from any platform(steam, google, facebook). It come only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Unlink Platform](images/bp_asynctask_unlinkplatform.PNG?raw=true "Blueprint AsyncTask Unlink Platform")

### GetLinkedPlatform ###
This blueprint is similar to `JusticeIdentity::GetLinkedPlatform` function. You can use this blueprint function to get list of platform(steam, google, facebook) that you're linked to. It come only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Get Linked Platform](images/bp_asynctask_getlinkedplatform.PNG?raw=true "Blueprint AsyncTask Get Linked Platform")


### User Logout ###
This blueprint is similar to `JusticeIdentity::UserLogout` function. You can use this blueprint function to logout from current user session. It come with two flavor: Async Action Base and Function Library.

#### Async Action Base flavor ####
![Blueprint AsyncTask User Logout](images/bp_asynctask_userlogout.PNG?raw=true "Blueprint AsyncTask User Logout")

#### Function Library flavor ####
![Blueprint Function Library User Logout](images/bp_functionlibrary_userlogout.PNG?raw=true "Blueprint Function Library User Logout")
