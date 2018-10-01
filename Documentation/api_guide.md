\page api_guide API Guide

We have blueprint version for each C++ Function on each services.

\section using_the_sdk_with_blueprints Using the SDK with Blueprints

The blueprint function is available in two flavors.

1. As an Async Action Base single node with multiple output pin, whether it succeeds or fails
2. As a Function Library using delegate node to handle callback

### Identity Service
This is the first required service before we can proceed to platform service.

### UserLogin ###

This blueprint is similar to `JusticeIdentity::UserLogin` function. You can use this blueprint function to login with email and password. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask User Login](images/bp_asynctask_userlogin.png "Blueprint AsyncTask User Login")

#### Function Library flavor ####

![Blueprint Function Library User Login](images/bp_functionlibrary_userlogin.png "Blueprint Function Library User Login")

### Register New Player ###

This blueprint is similar to `JusticeIdentity::RegisterNewPlayer` function. You can use this blueprint to register new player using email and password. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask Register New Player](images/bp_asynctask_registernewplayer.png "Blueprint AsyncTask Register New Player")

#### Function Library flavor ####
![Blueprint Function Library Register New Player](images/bp_functionlibrary_registernewplayer.png "Blueprint Function Library Register New Player")

### ReissueVerificationCode ###

This blueprint is similar to `JusticeIdentity::ReissueVerificationCode` function. You can use this blueprint function to send user verification code to their email. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Reissue Verification Code](images/bp_asynctask_reissueverificationcode.png "Blueprint AsyncTask Reissue Verification Code")

### VerifyNewPlayer ###

This blueprint is similar to `JusticeIdentity::VerifyNewPlayer` function. You can use this blueprint function to verify newly created account with verification code that user receives in their email. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask Verify New Player](images/bp_asynctask_verifynewplayer.png "Blueprint AsyncTask Verify New Player")

#### Function Library flavor ####

![Blueprint Function Library Verify New Player](images/bp_functionlibrary_verifynewplayer.png "Blueprint Function Library Verify New Player")

### Device Login ###

This blueprint is similar to `JusticeIdentity::DeviceLogin` function. You can use this blueprint function to login with device Id. We use value from `FGenericPlatformMisc::GetDeviceId()` as Device Id. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Device Login](images/bp_asynctask_devicelogin.png "Blueprint AsyncTask Device Login ")

### Auth Code Login ###

This blueprint is similar to `JusticeIdentity::AuthCodeLogin` function. You can use this blueprint function to login from AccelByte's Launcher. We send authentication code from launcher, in the SDK the authentication code is grabbed for login. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Auth Code Login](images/bp_asynctask_authcodelogin.png "Blueprint AsyncTask Auth Code Login")

### UpgradeHeadlessAccount ### 

This blueprint is similar to `JusticeIdentity::UpgradeHeadlessAccount` function. You can use this blueprint function to upgrade headless accounts (such as from Steam, Facebook and Google) into account using email. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Upgrade Headless Account](images/bp_asynctask_upgradeheadlessaccount.png "Blueprint AsyncTask Upgrade Headless Account")

### ResetPassword ###

This blueprint is similar to `JusticeIdentity::ResetPassword` function. You can use this blueprint function to reset user’s password. User would receive verification code to reset their password. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask Reset Password](images/bp_asynctask_resetpassword.png "Blueprint AsyncTask Reset Password")

#### Function Library flavor ####

![Blueprint Function Library Reset Password](images/bp_functionlibrary_resetpassword.png "Blueprint AsyncTask Reset Password")

### LinkPlatform ###

This blueprint is similar to `JusticeIdentity::LinkPlatform` function. You can use this blueprint function to link your account into any platform (e.g. Steam, Google, Facebook). It comes only in Async Action Base flavor.

#### Async Action Base flavor ####
![Blueprint AsyncTask Link Platform](images/bp_asynctask_linkplatform.png "Blueprint AsyncTask Link Platform")

### UnlinkPlatform ###

This blueprint is similar to `JusticeIdentity::UnlinkPlatform` function. You can use this blueprint function to unlink your account from any platform (e.g. Steam, Google, Facebook). It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Unlink Platform](images/bp_asynctask_unlinkplatform.png "Blueprint AsyncTask Unlink Platform")

### GetLinkedPlatform ###

This blueprint is similar to `JusticeIdentity::GetLinkedPlatform` function. You can use this blueprint function to get list of platforms (e.g. Steam, Google, Facebook) linked to your account. It comes only in Async Action Base flavor.

#### Async Action Base flavor ####

![Blueprint AsyncTask Get Linked Platform](images/bp_asynctask_getlinkedplatform.png "Blueprint AsyncTask Get Linked Platform")

### User Logout ###

This blueprint is similar to `JusticeIdentity::UserLogout` function. You can use this blueprint function to logout from current user session. It comes with two flavors: Async Action Base and Function Library.

#### Async Action Base flavor ####

![Blueprint AsyncTask User Logout](images/bp_asynctask_userlogout.png "Blueprint AsyncTask User Logout")

#### Function Library flavor ####

![Blueprint Function Library User Logout](images/bp_functionlibrary_userlogout.png "Blueprint Function Library User Logout")

\section using_the_sdk_with_cpp Using the SDK with C++

See JusticeIdentity, JusticePlatform, Catalog, JusticePurchase, and JusticeWallet.

\section usage_examples Usage examples

\subsection justice_identity Justice Identity

* `RegisterNewPlayer`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString LoginId = "johnDoe@example.com";
            FString Password = "password";
            FString DisplayName = "John_Doe";
            FUserAuthTypeJustice AuthType = Email;
            JusticeIdentity::RegisterNewPlayer(LoginId, Password, DisplayName, AuthType,
                FRegisterPlayerCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, UserCreateResponse* Response)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Register User Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("User's registered display name is %s"), *Response->DisplayName)
                        UE_LOG(LogTmp, Log, TEXT("User's registered UserId is %s"), *Response->UserId)
                    }
                );
            );
        }
```

* `ReissueVerificationCode`
```cpp

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString UserId = "1223334444";
            FString LoginId = "johnDoe@example.com";
            FUserAuthTypeJustice AuthType = Email;
            JusticeIdentity::ReissueVerificationCode(UserId, LoginId,
                FDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Issue Verification Code Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    }
                );
            );
        }
```

* `VerifyNewPlayer`
```cpp

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString UserId = "1223334444";
            FString VerificationCode = "13579";
            FUserAuthTypeJustice AuthType = Email;
            JusticeIdentity::VerifyNewPlayer(UserId, VerificationCode, AuthType,
                FDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Verify Registration Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    }
                );
            );
        }
```

* `UserLogin`
```cpp

        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString LoginId = "johnDoe@example.com";
            FString Password = "password";
            JusticeIdentity::UserLogin(LoginId, Password,
                FUserLoginCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, *OAuthTokenJustice Token)
                    {
                        UE_LOG(LogTmp, Log, TEXT("User Login with Email Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("User's Display Name is %s"), *Token->DisplayName);
                    }
                );
            );
        }
```

* `DeviceLogin`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            JusticeIdentity::DeviceLogin(FUserLoginCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr, *OAuthTokenJustice Token)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Login with Device: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    UE_LOG(LogTmp, Log, TEXT("User's Display Name is %s"), *Token->DisplayName);
                }
            ););
        }
```

* `AuthCodeLogin`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString AuthCode = "0123456789";
            FString RedirectURI = "https://example.com";
            JusticeIdentity::AuthCodeLogin(AuthCode, RedirectURI, FUserLoginCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr, *OAuthTokenJustice Token)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Login with Authorization Code (Launcher) Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    UE_LOG(LogTmp, Log, TEXT("User's Display Name is %s"), *Token->DisplayName);
                }
            ););
        }
```

* `UpgradeHeadlessAccount`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login with device first if you want to upgrade it with email and password

            FString Email = "johnDoe@example.com";
            FString Password = "password";
            JusticeIdentity::UpgradeHeadlessAccount(Email, Password, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Upgrade Headless Account Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }
```

* `ForgotPassword`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString LoginId = "johnDoe@example.com";
            JusticeIdentity::ForgotPassword(LoginId, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Request to Reset Password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }
```

* `ResetPassword`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString VerificationCode = "123456";
            FString UserId = "johnDoe@example.com";
            FString NewPassword = "freshPassword";
            JusticeIdentity::ResetPassword(UserId, VerificationCode, NewPassword, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Reset Password Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }
```
* `LinkPlatform`
```cpp
    
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login user first

            FString PlatformID = "facebook";
            FString Ticket = "0123456789";
            JusticeIdentity::LinkPlatform(PlatformID, Ticket, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Linking Account Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }
```
* `UnlinkPlatform`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login user first

            FString PlatformID = "facebook";
            JusticeIdentity::UnlinkPlatform(PlatformID, FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Unlinking Account Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }
```

* `GetLinkedUserPlatform`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login user first

            JusticeIdentity::GetLinkedPlatform(FGetLinkedPlatformCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr, TArray<LinkedPlatform> LinkedPlatforms)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Get Linked Account Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    UE_LOG(LogTmp, Log, TEXT("Found Linked Account: %d"), LinkedPlatforms.Num());
                }
            ););
        }
```
* `Logout`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            // Login user first

            JusticeIdentity::UserLogout(FDefaultCompleteDelegate::CreateLambda(
                [](bool bSuccessful, FString ErrorStr)
                {
                    UE_LOG(LogTmp, Log, TEXT("User Logout Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                }
            ););
        }
```

\subsection platform_service Platform Service

\subsubsection justice_catalog Justice Catalog

* `GetCategory`
```cpp
    
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString ParentPath = "/game/weapon/example";
            FString Language = "en";
            Catalog::GetCategory(ParentPath, Language,
                FCategoryDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<Category> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Category Found : %d"), Result.Num());                        
                    }
                );
            );
        }
```
* `GetRootCategory`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Language = "en";
            Catalog::GetRootCategory(Language,
                FCategoryDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<Category> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Root Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Root Category Found : %d"), Result.Num());                        
                    }
                );
            );
        }
```

* `GetChildCategory`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Language = "en";
            FString CategoryPath = "/game/weapon";
            Catalog::GetChildCategory(Language, CategoryPath,
                FCategoryDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<Category> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Child Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Child Category Found : %d"), Result.Num());                        
                    }
                );
            );
        }
```

* `GetDescendantCategory`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Language = "en";
            FString CategoryPath = "/game/potion";
            Catalog::GetDescendantCategory(Language, CategoryPath,
                FCategoryDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<Category> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Descendant Category Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Descendant Category Found: %d"), Result.Num());                        
                    }
                );
            );
        }
```

* `GetItem`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString ItemId = "123456789";
            FString Region = "US";
            FString Language = "en";
            Catalog::GetItem(ItemId, Region, Language,
                FGetItemCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, ItemInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Item Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Item name: %s"), *Result->Title;
                    }
                );
            );
        }
```

* `GetItemByQuery`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Language = "en";
            FString Region = "US";
            FStri / CategoryPath parameter and option/armour";
            FString ItemType = "";
            FString Status = "ACTIVE";
            FString Page = "0";
            FString Size = "5";
            Catalog::GetItemByQuery(Language, Region, CategoryPath, ItemType, Status, Page, Size,
                FItemCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<ItemInfo> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Item by Query Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Item found: %d"), Result.Num();
                    }
                );
            );
        }
```

\subsection justice_purchase Justice Purchase

* `CreateNewOrder`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString ItemId = "0123456789";
            int32 Price = 10;
            int32 DiscountedPrice = "8";
            FString Currency = "USD";
            JusticePurchase::CreateNewOrder(ItemId, Price, DiscountedPrice, Currency,
                FOrderInfoCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, OrderInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Create Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Order Number: %s"), *Result->OrderNo;
                    }
                );
            );
        }
```

* `GetUserOrder`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString OrderNo = "1223334444";
            JusticePurchase::GetUserOrder(OrderNo, 
                FOrderInfoCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, OrderInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get User Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Order's Item Id: %s"), *Result->ItemId;
                    }
                );
            );
        }
```

* `GetUserOrders`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            int32 Page = "0";
            int32 Size = "20";
            JusticePurchase::GetUserOrders(Page, Size,
                FOrderInfoCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<OrderInfo> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get User Orders Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("User's order found : %d"), Result.Num();
                    }
                );
            );
        }
```

* `GetUserOrderHistory`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString OrderNo = "1223334444";
            JusticePurchase::GetUserOrderHistory(OrderNo, 
                FGetUserOrderHistoryCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, TArray<OrderHistoryInfo> Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get User Order History Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Found %d order history."), Result.Num();
                    }
                );
            );
        }
```

* `FulfillOrder`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString OrderNo = "1223334444";
            JusticePurchase::FulfillOrder(OrderNo, 
                FOrderInfoCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, OrderInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Fulfill User Order Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Item Id that fulfilled is %s."), *Result->ItemId;
                    }
                );
            );
        }
```

\subsubsection justice_wallet Justice Wallet

* `GetWalletBalance`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString CurrencyCode = "USD";
            JusticeWallet::GetWalletBalance(CurrencyCode, 
                FGetWalletBalanceCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, int32 Balance)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get Wallet Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("Wallet balance is %d."), *Balance;
                    }
                );
            );
        }
```
\subsubsection user_profile User Profile

* `RequestCurrentPlayerProfile`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            JusticePlatform::RequestCurrentPlayerProfile( 
                FReqestCurrentPlayerProfileCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr, UserProfileInfo Result)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Get User's Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                        UE_LOG(LogTmp, Log, TEXT("User's display name is %s."), *Result->DisplayName;
                    }
                );
            );
        }
```

* `CreateDefaultPlayerProfile`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FString Email = "johndoe@example.com";
            FString DisplayName = "JohnD03";
            JusticePlatform::CreateDefaultPlayerProfile(Email, DisplayName, 
                FDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Create Default Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    }
                );
            );
        }
```

* `UpdatePlayerProfile`
```cpp
        void AJusticeSDKDemoGameModeBase::BeginPlay()
        {
            FAccelByteModelsUserProfileInfoUpdate NewUserProfile;
            NewUserProfile.Email = "johndoe@example.com";
            NewUserProfile.DisplayName = "JohnDoe";
            NewUserProfile.AvatarLargeUrl = "https://example.com/large.jpg";
            NewUserProfile.AvatarUrl = "https://example.com/medium.jpg";
            NewUserProfile.AvatarSmallUrl = "https://example.com/small.jpg";
            NewUserProfile.FirstName = "John";
            NewUserProfile.LastName = "Doe";
            NewUserProfile.Language = "en";
            NewUserProfile.Timezone = "Asia/Shanghai";
            NewUserProfile.DateOfBirth = "2002-02-02";
            
            JusticePlatform::UpdatePlayerProfile(NewUserProfile, 
                FDefaultCompleteDelegate::CreateLambda(
                    [](bool bSuccessful, FString ErrorStr)
                    {
                        UE_LOG(LogTmp, Log, TEXT("Update Player Profile Result: %s"), bSuccessful ? TEXT("Success") : TEXT("Failed"));
                    }
                );
            );
        }
```

\section models Models

## FBalanceInfo

|Field|Type|Description|
|---|---|---|
Id              |FString    |User Id that owns this balance.
WalletID        |FString    |Wallet Id from the balance.
CurrencyCode    |FString    |Currency code from the balance (e.g. USD)
Balance         |int32      |Amount of the balance.
BalanceSource   |FString    |Balance's source.
CreatedAt       |FString    |Balance creation time.
UpdatedAt       |FString    |Latest balance update time.
Status          |FString    |Status of the balance.

## FCategory

|Field|Type|Description|
|---|---|---|
Namespace           |FString    |Category's namespace.
ParentCategoryPath  |FString    |Hold category's parent's path.
CategoryPath        |FString    |Current category's path.
DisplayName         |FString    |Category's display name.
CreatedAt           |FString    |Category creation time.
UpdatedAt           |FString    |Category latest update time.
Root                |bool       |Is this category is a root category?

## FCurrencySummary

|Field|Type|Description|
|---|---|---|
CurrencyCode    |FString|The code of the currency
CurrencySymbol  |FString|The symbol of the currency.
CurrencyType    |FString|The type of the currency ('REAL', 'VIRTUAL')
Namespace       |FString|The namespace of the currency.
Decimals        |int32  |The decimals number of the currency.

## FItemInfo

|Field|Type|Description|
|---|---|---|
Title               |FString     |Item's title
Description         |FString     |Item's description
LongDescription     |FString     |Item's long description
Images              |TArray<FJusticeImage>    |Item's image(s)
ThumbnailImage      |FJusticeImage     |Item's thumbnail
ItemId              |FString     |Item's Id
AppId               |FString     |App Id for the item.
AppType             |FString     |App type for the item.
SKU                 |FString     |Item's SKU.
Namespace           |FString     |Item's namespace.
EntitlementName     |FString     |Item's entitlement name.
EntitlementType     |FString     |Item's entitlement type.
UseCount            |int32       |Item's usage count.
CategoryPath        |FString     |Category path containing the item.
Status              |FString     |Item's status.
ItemType            |FString     |Item's type.
CreatedAt           |FString     |Item's creation time.
UpdatedAt           |FString   |Item latest update time.
TargetCurrencyCode  |FString     |Item's target currency code.
TargetNamespace     |FString     |Item's target namespace.
RegionData          |TArray<FAccelByteModelsRegionData>|Region data of this item.
ItemIds             |TArray< FString>   |Array of item IDs if this item is a bundle item.
Tags                |TArray< FString>   |Item's tag(s).

## FItemPagingSlicedResult

|Field|Type|Description|
|---|---|---|
Data    |TArray<FItemInfo>|Array of items in the result page.
Paging  |FPaging|Paging system from the result.

## FJusticeImage

|Field|Type|Description|
|---|---|---|
Height          |int32  |Image's height in pixel.
Width           |int32  |Image's width in pixel.
ImageURL        |FString|Image's URL.
SmallImageURL   |FString|Small image's URL.

## FAccelByteModelsLinkedPlatform

|Field|Type|Description|
|---|---|---|
PlatformID      |FString|Id of the platform linked to user.
PlatformUserID  |FString|User Id that links the user to platform.
Namespace       |FString|Namespace of the linked account.
UserId          |FString|User Id that is linked to platform.

## FOAuthTokenJustice

|Field|Type|Description|
|---|---|---|
AccessToken     |FString|Client's access token.
UserRefreshToken|FString|Client's refresh token.
TokenType       |FString|Type of token.
ExpiresIn       |double|Token expiration time.
Permissions     |TArray<FAccelByteModelsPermission>|Permission(s) of the client.
Roles           |TArray< FString>|Role(s) of the client.
Bans            |TArray< FString>|Ban(s) of the client.
UserId          |FString|Id of the client.
DisplayName     |FString|Display name of the client.
Namespace       |FString|Namespace of the client.
LastTokenRefreshUtc|FDateTime|Last token refresh in UTC.
NextTokenRefreshUtc|FDateTime|Next token refresh in UTC.
TokenRefreshBackoff|FTimespan|Refresh token backoff.
JFlags          |int32|Flags

## FOrderCreate

|Field|Type|Description|
|---|---|---|
ItemId          |FString|Id of item ordered.
Quantity        |FString|Quantity of item ordered.
Price           |int32  |Total price of item ordered.
DiscountedPrice |int32  |Total price of item ordered after discount.
CurrencyCode    |FString|The currency code of item ordered.
ReturnUrl       |FString|URL that is opened after order is fulfilled.

## FOrderHistoryInfo

|Field|Type|Description|
|---|---|---|
OrderNo     |FString     |Order's number.
Operator    |FString     |Operator of the order.
Action      |FString     |Order's current action.
Reason      |FString     |Order's reason.
UserId      |FString     |Id of user dealing with this order.
CreatedAt   |FString     |Order's creation time.
UpdatedAt   |FString     |Order latest update time.

## FOrderInfo

|Field|Type|Description|
|---|---|---|
OrderNo             |FString    |Order's number.
UserId              |FString    |Id of user that creates this order.
ItemId              |FString    |Item's Id included in this order.
bSandBox            |bool       |Is this order sandbox mode?
Quantity            |int32      |The amount of item ordered.
Price               |int32      |Item's price.
DiscountedPrice     |int32      |Item's discounted price.
Vat                 |int32      |Order's VAT.
SalesTax            |int32      |Order's sales tax.
PaymentProviderFee  |int32      |Payment provider fee for this order.
PaymentMethodFee    |int32      |Payment method fee for this order.
Currency            |FCurrencySummary|Currency used in this order.
PaymentUrl          |FPaymentURL|Order's payment information.
PaymentStationURL   |FString    |URL used to pay this order.
Transactions        |TArray<FAccelByteModelsTransaction>|Order's transaction step.
EntitlementIds      |TArray< FString>   |Entitlement Id from this order.
Status              |FString    |Order's status.
StatusReason        |FString    |Reason for order's status.
Namespace           |FString    |Namespace of this order.
CreatedTime         |FString    |Order creation time.
ChargedTime         |FString    |Order charging time.
FulfilledTime       |FString    |Order fulfilling time.
RefundedTime        |FString    |Order refunding time.
CreatedAt           |FString    |Order creation time.
UpdatedAt           |FString    |Order latest activity.

## FOrderInfoPaging

|Field|Type|Description|
|---|---|---|
Data    |TArray<FOrderInfo>|Array of orders' informations in the result page.
PagingInfo  |FPaging|Paging system from the result.

## FPaging

|Field|Type|Description|
|---|---|---|
Previous    |FString|Previous page.
Next        |FString|Next page.

## FPaymentURL

|Field|Type|Description|
|---|---|---|
PaymentProvider |FString |Provider of this payment.
PaymentUrl      |FString |URL to continue the payment.
PaymentToken    |FString |Payment's token.
ReturnUrl       |FString |URL returned from OrderCreate
PaymentType     |FString |Payment's type.

## FAccelByteModelsPermission

|Field|Type|Description|
|---|---|---|
Resource    |FString|Resource of the permission.
Action      |int32  |Code number for the possible action.

## FAccelByteModelsPrice

|Field|Type|Description|
|---|---|---|
Value       |int32   |Price value.
CurrencyCode|FString |Code of currency for this price.
CurrencyType|FString |Type of the currency.
Namespace   |FString |Namespace of the currency.

## FAccelByteModelsRegionData

|Field|Type|Description|
|---|---|---|
Price                       |int32      |Price of item in this region.
DiscountPercentage          |int32      |Percentage of the discount for item in this region.
DiscountAmount              |int32      |Amount of discount for item in this region.
DiscountedPrice             |int32      |Price of item after discounted in this region.
CurrencyCode                |FString    |Code of currency used for the item in this region.
CurrencyType                |FString    |Type of currency for item in this region.
CurrencyNamespace           |FString    |Namespace of currency used.
PurchaseAt                  |FString    |Item purchase time.
ExpireAt                    |FString    |Item expiration time.
TotalNum                    |int32      |Item amount.
TotalNumPerAccount          |int32      |Amount of item per user.
DiscountPurchaseAt          |FString    |Discount purchasing time.
DiscountExpireAt            |FString    |Discount expiration time.
DiscountTotalNum            |int32      |Total discount amount.
DiscountTotalNumPerAccount  |int32      |Total discount amount per user.

## FAccelByteModelsResetPasswordRequest

|Field|Type|Description|
|---|---|---|
Code        |FString |Verification code to reset old password.
LoginId     |FString |LoginId (e.g. email) of the user that forgets the password.
NewPassword |FString |New password to replace the old password.

## FAccelByteModelsTransaction

|Field|Type|Description|
|---|---|---|
TransactionId           |FString |Transaction Id.
Amount                  |int32   |Transaction's amount.
Vat                     |int32   |Transcation's VAT.
SalesTax                |int32   |Transaction's sales tax.
Currency                |FCurrencySummary|Transaction currency information.
Type                    |FString |Transaction type ('CHARGE', 'REFUND').
Status                  |FString |Transaction status ('INIT', 'FINISHED', 'FAILED').
Provider                |FString |Transaction's provider (e.g. [XSOLLA](https://xsolla.com/)).
PaymentProviderFee      |int32   |Transaction's fee for provider.
PaymentMethod           |FString |Transaction's payment method.
PaymentMethodFee        |int32   |Transaction's fee for payment method used.
MerchantId              |FString |Transaction merchant's Id
ExternalTransactionId   |FString |External transaction's Id.
ExternalStatusCode      |FString |External transaction status code.
ExternalMessage         |FString |External transaction message.
TransactionStartTime    |FString |Transaction's starting time.
TransactionEndTime      |FString |Transaction's ending time.

## FUserAuthTypeJustice

|Field|Type|Description|
|---|---|---|
Email|enum(readonly)|Email-based account.
Phone|enum(readonly)|Phone-based account.

## FAccelByteModelsUserCreateRequest

|Field|Type|Description|
|---|---|---|
AuthType    |FString|Type of authentication.
DisplayName |FString|User's display name.
LoginId     |FString|User's login Id.
Password    |FString|User's password.

## FAccelByteModelsUserCreateResponse

|Field|Type|Description|
|---|---|---|
Namespace   |FString|Namespace of the user that has been created.
UserId      |FString|Id of the user that has been created.
AuthType    |FString|Authentication type of the user that has been created.
DisplayName |FString|Display name of the user that has been created.
LoginId     |FString|Login Id (e.g. email) of the user that has been created.

## FAccelByteModelsUserProfileInfo

|Field|Type|Description|
|---|---|---|
UserId          |FString|User's Id.
Namespace       |FString|Namespace of the user.
DisplayName     |FString|User's display name.
FirstName       |FString|User's first name.
LastName        |FString|User's last name.
Country         |FString|User's country.
AvatarSmallUrl  |FString|URL of user's small avatar.
AvatarUrl       |FString|URL of user's avatar.
AvatarLargeUrl  |FString|URL of user's large avatar.
Email           |FString|User's email.
Status          |FString|User's status.
Language        |FString|User's language.
Timezone        |FString|User's timezone.
DateOfBirth     |FString|User's date of birth.
CustomAttributes|TMap< FString, FString>|User's custom attributes.

## FUserProfileInfoUpdate

|Field|Type|Description|
|---|---|---|
DisplayName     |FString|Update user's display name.
FirstName       |FString|Update user's first name.
LastName        |FString|Update user's last name.
AvatarSmallUrl  |FString|Update user's URL of their small avatar.
AvatarUrl       |FString|Update user's URL of their avatar.
AvatarLargeUrl  |FString|Update user's URL of their large avatar.
Email           |FString|Update user's email.
Language        |FString|Update user's language.
Timezone        |FString|Update user's timezone.
DateOfBirth     |FString|Update user's date of birth.
CustomAttributes|TMap< FString, FString>|User's custom attributes.
Country         |FString|Update user's country.
Status          |FString|Update user's status.

## FAccelByteModelsWalletInfo

|Field|Type|Description|
|---|---|---|
Id              |FString|Wallet's Id.
Namespace       |FString|Wallet's namespace.
UserId          |FString|Wallet's owner's Id.
CurrencyCode    |FString|Currency code for this wallet.
CurrencySymbol  |FString|Symbol of the currency code.
Balance         |int32  |Wallet's balance.
CreatedAt       |FString|Wallet creation time.
UpdatedAt       |FString|Wallet update time.
Status          |FString|Wallet's status.

\section callbacks Callbacks

### FDefaultCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.

### FRegisterPlayerCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
UserCreateResponse|**UserCreateResponse**|User's account that created.

### FUserLoginCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
OAuthTokenJustice|**OAuthTokenJustice**|User's token.

### FGetLinkedPlatformCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<LinkedPlatform>|Array of linked platform.

### FCategoryDefaultCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<Category>|Array of linked platform.

### FGetItemCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|ItemInfo|Item information.

### FItemCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<ItemInfo>|Array of item information.

### FReqestCurrentPlayerProfileCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|UserProfileInfo|User profile.

### FOrderInfoCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|OrderInfo|Order information.

### FGetUserOrdersCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<OrderInfo>|Array of order information.

### FGetUserOrderHistoryCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|TArray<OrderHistoryInfo>|Array of order history.

### FGetWalletBalanceCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|**int32**|Amount of wallet balance.

### FReqestCurrentPlayerProfileCompleteDelegate
|type|description|
|---|---|
|**bool**|Is the request successful?
|**FString**|Error message if request is unsuccessful.
|UserProfileInfo|User profile information.
