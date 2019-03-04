
# AccelByte SDK Tutorial
- [AccelByte SDK Tutorial](#accelbyte-sdk-tutorial)
  - [User Management and Authorization](#user-management-and-authorization)
  - [User Profile](#user-profile)
  - [Ecommerce](#ecommerce)
    - [Categories](#categories)
    - [Items](#items)
    - [Order](#order)
    - [Wallet](#wallet)
    - [Entitlement](#entitlement)
  - [Lobby](#lobby)
    - [Bind Delegates](#bind-delegates)
    - [Unbind Delegates](#unbind-delegates)
    - [Chat](#chat)
    - [Party](#party)
    - [Notification](#notification)
    - [Matchmaking](#matchmaking)
    - [Friends](#friends)
  - [Game Profile](#game-profile)
  - [Cloud Storage](#cloud-storage)


## User Management and Authorization
See AccelByte::Api::User, AccelByte::Api::Oauth2, AccelByte::Credentials,

### Login with Client Credentials
This is to get access token from `client_credentials` grant type, then store the access token in memory. 
You shouldn't use this if your application is a client. It is intended for machine-to-machine (server).
It is "required" for user management (create new user, reset password, etc). See AccelByte IAM documentation for how this OAuth2 token endpoint works.

```cpp
bool bClientLoggedIn = false;

User::LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoggedIn]()
    {
        UE_LOG(LogAccelByteUserTest, Display, TEXT("Success."));
        bClientLoggedIn = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```

See AccelByte::Api::User::LoginWithClientCredentials().

### Login with Username
Log in with email/phone number account.

```cpp
const FString OriginalEmail = TEXT("originalEmail@example.com");
const FString Password = TEXT("password");
bool bUserLoggedIn = false;

User::LoginWithUsername(OriginalEmail, Password, FVoidHandler::CreateLambda([&bUserLoggedIn]()
    {
        UE_LOG(LogAccelByteUserTest, Display, TEXT("Success."));
        bUserLoggedIn = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::LoginWithUsername().

### Login with Other Platform
This is to get access token with another platform account e.g. Steam, Google, Facebook, Twitch, etc. See AccelByte IAM documentation for how this OAuth2 token endpoint works.

```cpp
bool bSteamLoginSuccessful1 = false;

User::LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bSteamLoginSuccessful1 = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::LoginWithOtherPlatform().

### Login with Device ID
Log in with device ID (anonymous log in).

```cpp
bool bDeviceLoginSuccessful1 = false;

User::LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bDeviceLoginSuccessful1 = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::LoginWithDeviceId().

### Login with Launcher
Login from Accelbyte Launcher

```cpp
bool bLauncherLoginSuccessful1 = true;

User::LoginWithLauncher(FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bLauncherLoginSuccessful1 = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::LoginWithLauncher().

### Logout
This simply remove the stored access tokens and user ID from memory.

```cpp
AccelByte::Api::User::ForgetAllCredentials();
```
See AccelByte::Api::User::ForgetAllCredentials().

### Register
This function will register a new user with email-based account.

```cpp
const FString OriginalEmail = TEXT("originalEmail@example.com");
const FString Password = TEXT("password");
const FString DisplayName = TEXT("testName");
bool bUserAccountCreated = false;

User::Register(OriginalEmail, Password, DisplayName, THandler<FUserData>::CreateLambda([&bUserAccountCreated](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Display, TEXT("Success."));
        bUserAccountCreated = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::Register().

### Get Data
This function will get data of currently logged in user.

```cpp
bool bGetDataSuccessful1 = false;
FUserData GetDataResult;
User::GetData(THandler<FUserData>::CreateLambda([&](const FUserData& Result) 
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bGetDataSuccessful = true;
        GetDataResult = Result;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::GetData().

### Update
This function will update user's account.

```cpp
FUserUpdateRequest UpdateRequest
    {
        TEXT("US"),
        TEXT("UpdateName"),
        FString(UpdatedEmail),
        FString()
    };
FUserData UpdateResult;
bool bUserUpdated = false;

User::Update(UpdateRequest, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Display, TEXT("Success."));
        bUserUpdated = true;
        UpdateResult = Result;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::Update().

### Upgrade
This function will upgrade user's headless account. You may call SendUserAccountVerificationCode afterwards.
Headless account is an account that doesn't have an email and password.
If user logs in with a device/platform and they cannot login with email-and-password, their account is considered as a headless account.
Therefore, the function requests user’s Username and Password for parameters.

```cpp
FString Email = TEXT("testSDK@game.test");
FString Password = TEXT("password");
bool bUpgradeSuccessful = false;

User::Upgrade(Email, Password, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bUpgradeSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::Upgrade().

### Send Verification Code 
Verify user's email. User should login with email and password first to get access token.

```cpp
bool bSendSuccessful = false;

User::SendVerificationCode(FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bSendSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::SendVerificationCode().

### Verify
This function will verify the registered email **after** user receives verification code sent with ::SendVerificationCode() to their email.

```cpp
bool bGetVerificationCode = false;
FString VerificationCode = GetVerificationCodeFromUserId(FRegistry::Credentials.GetUserId());

User::Verify(VerificationCode, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bGetVerificationCode = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::Verify().

### Send Reset Password Code
For a password reset, verification code is sent to the email.

```cpp
FString LoginId = TEXT("testeraccelbyte@game.test");
bool bForgotPaswordSuccessful = false;

User::SendResetPasswordCode(LoginId, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bForgotPaswordSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::SendResetPasswordCode().

### Reset Password
Reset user's password with sent verification code.

```cpp
FString LoginId = TEXT("testeraccelbyte@game.test");
FString Password = "new_password";
FString VerificationCode = GetVerificationCode(LoginId);
bool bResetPasswordSuccessful = false;

User::ResetPassword(VerificationCode, LoginId, Password, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bResetPasswordSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::ResetPassword().

### Send Upgrade Verification Code
This function should be done before user upgrade their headless account. After this function successfully called, obtain the verification code from the submitted email. Then call UpgradeHeadlessAccountWithVerificationCode function afterwards.

```cpp
FString Email = TEXT("upgradeAndVerify@example.com");
bool bSendUserUpgradeVerificationCode = false;

User::SendUpgradeVerificationCode(Email, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bSendUserUpgradeVerificationCode = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::SendUpgradeVerificationCode().

### Upgrade and Verify
This function should be called after you call SendUserUpgradeVerificationCode and obtain verification code.

```cpp
FString Email = TEXT("upgradeAndVerify@example.com");
FString Password = TEXT("password");
bool bGetVerificationCode = false;
FString VerificationCode = GetVerificationCodeFromUserId(FRegistry::Credentials.GetUserId());

User::UpgradeAndVerify(Email, Password, VerificationCode, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bGetVerificationCode = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::UpgradeAndVerify().

### Get Platform Links
This function gets user's platform accounts linked to user’s account.

```cpp
bool bGetPlatformLinksSuccessful = false;

User::GetPlatformLinks(FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bGetPlatformLinksSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::GetPlatformLinks().

### Link Other Platform
This function links user's current account to their other account in other platform.
Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch platform).
The browser will redirect the URL to a site with a code in form of parameter URL.

```cpp
bool bLinkOtherPlatformSuccessful = false;
FString PlatformId = TEXT("PlatformId");
FString Ticket = TEXT("Ticket");

User::LinkOtherPlatform(PlatformId, Ticket, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bLinkOtherPlatformSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::LinkOtherPlatform().

### Unlink Other Platform
This function links user's current account to their other account in other platform
Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch).
The browser will redirect the URL to a site with a code in form of parameter URL.

```cpp
bool bUnlinkOtherPlatformSuccessful = false;
FString PlatformId = TEXT("PlatformId");

User::UnlinkOtherPlatform(PlatformId, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bLinkOtherPlatformSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::User::UnlinkOtherPlatform().

---

## User Profile
API to create, update, and get user's profile.
See AccelByte::Api::UserProfile,

### Get User Profile
Get user's profile information. If it doesn't exist, that will be an error.

```cpp
bool bGetProfileSuccessful1 = false;

UserProfile::GetUserProfile(THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
        bGetProfileSuccessful1 = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::UserProfile::GetUserProfile().

### Update User Profile
Update user's current profile information. If it doesn't exist, that will be an error.

```cpp
FAccelByteModelsUserProfileUpdateRequest ProfileUpdate;
    ProfileUpdate.Language = "en";
    ProfileUpdate.Timezone = "Etc/UTC";
    ProfileUpdate.DateOfBirth = "2000-01-01";
bool bUpdateProfileSuccessful = false;

UserProfile::UpdateUserProfile(ProfileUpdate, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bUpdateProfileSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::UserProfile::UpdateUserProfile().

### Create User Profile
Create complete player profile. If it already exist, that will be an error.

```cpp
bool bCreateProfileSuccessful1 = false;
FAccelByteModelsUserProfileCreateRequest ProfileCreate;
    ProfileCreate.Language = "en";
    ProfileCreate.Timezone = "Etc/UTC";
    ProfileCreate.DateOfBirth = "1970-01-01";

UserProfile::CreateUserProfile(ProfileCreate, THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
        bCreateProfileSuccessful1 = true;
    }), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
    {
        if (Code != 2271)
        {
            UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
            bCreateProfileSuccessful1 = false;
        }
        else
        {
            UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
            bCreateProfileSuccessful1 = true;
        }
    }));
```
See AccelByte::Api::UserProfile::CreateUserProfile().

---

## Ecommerce
API related to Ecommerce.
See AccelByte::Api::Category, AccelByte::Api::Item, AccelByte::Api::Order, AccelByte::Api::Wallet,

### Categories
Category API for buying things from the online store.
The category has [tree data structure](https://en.wikipedia.org/wiki/Tree_(data_structure)). Each category has path, for example "/equipments/armor/legs". Each category has items inside it. You can get a list of items by criteria or by its ID.
See AccelByte::Api::Category,

#### Get Root Categories
This function gets root categories that exist in the specified namespace.
This is equivalent of getting categories in "/". This will yield, for example, "/equipments", "/consumables", "/pay2win", "/boosts".

```cpp
FString Language = TEXT("en");
bool bGetRootCategoriesSuccess = false;
bool bExpectedRootCategoryFound = false;

Category::GetRootCategories(Language, Category::FGetRootCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        for (int i = 0; i < Results.Num(); i++)
        {
            if (Results[i].CategoryPath == ExpectedRootCategoryPath)
            {
                bExpectedRootCategoryFound = true;
            }
        }
        bGetRootCategoriesSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Category::GetRootCategories().

#### Get Category
This function gets the category from a store in the specified namespace, for example, "/equipments/armor/torso".

```cpp
FString CategoryPath = TEXT("/equipments/armor/torso");
FString Language = TEXT("en");
bool bGetCategorySuccess = false;
    
Category::GetCategory(CategoryPath, Language, Category::FGetCategorySuccess::CreateLambda([&](FAccelByteModelsFullCategoryInfo Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetCategorySuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Category::GetCategory().

#### Get Child Categories
This function gets the subcategories from a category in the specified namespace.

```cpp
FString Language = TEXT("en");
FString CategoryPath = TEXT("/equipments/pay2win/super_badass_armor/chest");
bool bGetChildCategoriesSuccess = false;
bool bExpectedChildCategoryFound = false;

Category::GetChildCategories(Language, CategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        for (int i = 0; i < Results.Num(); i++)
        {
            if (Results[i].CategoryPath == ExpectedChildCategoryPath)
            {
                bExpectedChildCategoryFound = true;
            }
        }
        bGetChildCategoriesSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Category::GetChildCategories().

#### Get Descendant Categories
This is to get every subcategories from a category in the specified namespace.

```cpp
FString Language = TEXT("en");
FString CategoryPath = TEXT("/equipments/pay2win/super_badass_armor/chest");
bool bGetDescendantCategoriesSuccess = false;
bool bExpectedDescendantCategoryFound1 = false;
bool bExpectedDescendantCategoryFound2 = false;
    
Category::GetDescendantCategories(Language, CategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        for (int i = 0; i < Results.Num(); i++)
        {
            if (Results[i].CategoryPath == ExpectedChildCategoryPath)
            {
                bExpectedDescendantCategoryFound1 = true;
            }
            else if (Results[i].CategoryPath == ExpectedGrandChildCategoryPath)
            {
                bExpectedDescendantCategoryFound2 = true;
            }
        }
        bGetDescendantCategoriesSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Category::GetDescendantCategories().

### Items
Item API for buying things from the online store. An item represents a single product sold in the online store. Each category has items inside it. You can get a list of items by criteria or by its ID.
See AccelByte::Api::Item,

#### Get Item by ID
Get one item information from an online store.

```cpp
FString ItemId = TEXT("YourItemId");
FString Language = TEXT("en");
FString Region = TEXT("US");
bool bGetItemByIdSuccess = false;
    
Item::GetItemById(ItemId, Language, Region, Item::FGetItemByIdSuccess::CreateLambda([&](const FAccelByteModelsItemInfo& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetItemByIdSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Item::GetItemById().

#### Get Items by Criteria
Get an array of items with specific criteria/filter from online store. You can think of it like a SQL query.

```cpp
FString Language = TEXT("en");
FString Region = TEXT("US");
FString CategoryPath = TEXT("/equipments/pay2win/super_badass_armor/chest");
EAccelByteItemStatus Status = EAccelByteItemStatus::ACTIVE;
EAccelByteItemType Type = EAccelByteItemType::INGAMEITEM;
bool bGetItemByCriteriaSuccess = false;
bool bExpectedRootItemFound = false;
    
Item::GetItemsByCriteria(Language, Region, CategoryPath, Type, Status, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        for (int i = 0; i < Result.Data.Num(); i++)
        {
            if (Result.Data[i].Title == ExpectedRootItemTitle)
            {
                UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("        Found expected item"));
                ItemId = Result.Data[i].ItemId;
                bExpectedRootItemFound = true;
            }
        }
        bGetItemByCriteriaSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Item::GetItemsByCriteria().

### Order
Order is used to purchase something from the online store.
See AccelByte::Api::Order

#### Create Order
Create order to purchase something from the store. 

```cpp
const int32 Quantity = 1;
FAccelByteModelsOrderCreate OrderCreate;
    OrderCreate.CurrencyCode = Item.RegionData[0].CurrencyCode;
    OrderCreate.DiscountedPrice = Item.RegionData[0].DiscountedPrice * Quantity;
    OrderCreate.Price = Item.RegionData[0].Price * Quantity;
    OrderCreate.Quantity = 1;
    OrderCreate.ReturnUrl = TEXT("https://sdk.example.com");
    OrderCreate.ItemId = Item.ItemId;

bool bCreateNewOrderSuccess = false;
    
Order::CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](FAccelByteModelsOrderInfo Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bCreateNewOrderSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Order::CreateNewOrder().

#### Get User Order
Get a single order with order number.

```cpp
FString OrderNo = TEXT("YourOrderNo");
bool bGetUserOrderSuccess = false;
    
Order::GetUserOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetUserOrderSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Order::GetUserOrder().

#### Get User Orders
Get all of user's orders that have been created with paging.

```cpp
const int32 page = 0;
const int32 size = 20;
bool bGetUserOrdersSuccess = false;
    
Order::GetUserOrders(page, size, THandler<FAccelByteModelsOrderInfoPaging>::CreateLambda([&](const FAccelByteModelsOrderInfoPaging& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetUserOrdersSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Order::GetUserOrders().

#### Get Order History
Get the history of the created orders.

```cpp
FString OrderNo = TEXT("YourOrderNo");
bool bGetUserOrderHistorySuccess = false;
    
Order::GetUserOrderHistory(OrderNo, THandler<TArray<FAccelByteModelsOrderHistoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsOrderHistoryInfo>& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetUserOrderHistorySuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Order::GetUserOrderHistory().

#### Fulfill order
Fulfill an order if the order is charged but the fulfillment fail.

```cpp
FString OrderNo = TEXT("YourOrderNo");
bool bFulfillOrderSuccess = false;
    
Order::FulfillOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bFulfillOrderSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Api::Order::FulfillOrder().

### Wallet
Wallet API for buying things from the online store; a wallet can be a virtual or real currency.
See AccelByte::Api::Wallet

#### Get Wallet Info by Currency Code
Get user's wallet information for a specific currency code.

```cpp
FString ExpectedCurrencyCode = TEXT("CurrencyCode");
bool bGetWalletSuccess = false;
    
Wallet::GetWalletInfoByCurrencyCode(ExpectedCurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&](const FAccelByteModelsWalletInfo& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetWalletSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See AccelByte::Wallet::GetWalletInfoByCurrencyCode().

### Entitlement
Entitlement API for checking user's ownership. User can query a list of item that belongs to him/her.
See AccelByte::Api::Entitlement

#### Query User Entitlement
Get list of ownership(s) that belongs to the user.
```cpp
bool bQueryEntitlementSuccess = false;
bool bQueryResultTrue = false;
FString entitlementName = TEXT("");
FString itemId = TEXT("");
int32 page = 0;
int32 size = 20;
EAccelByteEntitlementClass entitlementClass = EAccelByteEntitlementClass::NONE;
EAccelByteAppType appType = EAccelByteAppType::NONE; 

Entitlement::QueryUserEntitlement(entitlementName, itemId, page, size,
THandler<FAccelByteModelsEntitlementPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsEntitlementPagingSlicedResult& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bQueryEntitlementSuccess = true;
        bQueryResultTrue = (Result.Data.Num() > 0);
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }), entitlementClass, appType);
```
See AccelByte::Api::Entitlement::QueryUserEntitlement().

---

## Lobby
Lobby is for chatting and party management. Unlike other servers which use HTTP, Lobby server uses WebSocket ([RFC 6455](https://tools.ietf.org/html/rfc6455)).
See AccelByte::Api::Lobby

### Connect to server
You must connect to the server before you can start sending/receiving. Also make sure you have logged in first as this operation requires access token.

```cpp
FRegistry::Lobby.Connect();
```
See AccelByte::Api::Lobby::Connect().

### Disconnect from server
Disconnect from server if and only if the you have connected to server. If not currently connected, then this does nothing.

```cpp
FRegistry::Lobby.Disconnect();
```
See AccelByte::Api::Lobby::Disconnect().

### Check If Connected
Check whether the websocket is currently connected to the Lobby server.

```cpp
if (FRegistry::Lobby.IsConnected())
{
    UE_LOG(LogTemp, Log, TEXT("Yay."));
}
else
{
    UE_LOG(LogTemp, Log, TEXT("Nay."));
}
```
See AccelByte::Api::Lobby::IsConnected().

### Sending Ping
You should send ping every some time (for example, every 4 seconds) so that the server doesn't close the connection.

```cpp
FRegistry::Lobby.SendPing();
```
See AccelByte::Api::Lobby::SendPing().

### Bind Delegates
 You must bind delegates/callbacks first to handle the events. For example when a user received a private message or a response to create party request. You can bind the delegate by using set method on every delegate you want to use. The delegates can be `nullptr` if you want to not bind the callback. All delegates have one parameter `Result` with different types.

```cpp
    void SetConnectSuccessDelegate(const FConnectSuccess& OnConnectSuccess)
    void SetConnectFailedDelegate(const FErrorHandler& OnConnectError)
    void SetConnectionClosedDelegate(const FConnectionClosed& OnConnectionClosed)
    void SetPartyLeaveNotifDelegate(const FPartyLeaveNotif& OnLeavePartyNotice)
    void SetPartyInviteNotifDelegate(const FPartyInviteNotif& OnPartyInviteNotif)
    void SetPartyGetInvitedNotifDelegate(const FPartyGetInvitedNotif& OnInvitePartyGetInvitedNotice)
    void SetPartyJoinNotifDelegate(const FPartyJoinNotif& OnInvitePartyJoinNotice)
    void SetPartyKickNotifDelegate(const FPartyKickNotif& OnInvitePartyKickedNotice)
    void SetPrivateMessageNotifDelegate(FPersonalChatNotif OnPersonalChatNotif)
    void SetPartyChatNotifDelegate(FPartyChatNotif OnPersonalChatNotif)
    void SetUserPresenceNotifDelegate(FFriendStatusNotif OnUserPresenceNotif)
    void SetMessageNotifDelegate(const FMessageNotif& OnNotificationMessage)
    void SetOnFriendRequestAcceptedNotifDelegate(const FAcceptFriendsNotif& OnAcceptFriendsNotif) 
    void SetOnIncomingRequestFriendsNotifDelegate(const FRequestFriendsNotif& OnRequestFriendsNotif)
    void SetParsingErrorDelegate(const FErrorHandler& OnParsingError)
    ...
```

Usage :
```cpp
FRegistry::Lobby.SetConnectSuccessDelegate(Api::Lobby::FConnectSuccess::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    User 0 connected!"))
        UsersConnected = true;
        UsersConnectionResponded = true;
    }));

FRegistry::Lobby.SetConnectFailedDelegate(FErrorHandler::CreateLambda([&](int32 Code, FString Message)
    {
        UE_LOG(LogAccelByteLobbyTest, Log, TEXT("    User 0 failed to connect!"))
        UsersConnected = false;
        UsersConnectionResponded = true;
    }));
Lobby.Connect();
```
See AccelByte::Api::Lobby.

### Unbind Delegates
Unbind all callbacks.
```cpp
FRegistry::Lobby.UnbindEvent();
```
See AccelByte::Api::Lobby::UnbindEvent().

### Chat

#### Party Chat
Send a message to other party members.

```cpp
FString Message = TEXT("Message");

FRegistry::Lobby.SendPartyMessage(Message);
```
See AccelByte::Api::Lobby::SendPartyMessage(). 

#### Private Message (PM)
Send a private message to another user.

```cpp
FString UserId = TEXT("FriendUserId");
FString Message = TEXT("Message");

FRegistry::Lobby.SendPrivateMessage(UserId, Message);
```
See AccelByte::Api::Lobby::SendPrivateMessage().

### Party

#### Get Party Information
Get information about current party.
```cpp
FRegistry::Lobby.SendInfoPartyRequest();
```
See AccelByte::Api::Lobby::SendInfoPartyRequest().

#### Create a Party
Create a party. You can't create a party when you're already in one.
```cpp
FRegistry::Lobby.SendCreatePartyRequest();
```
See AccelByte::Api::Lobby::SendCreatePartyRequest().

#### Leave Current party
Leave current a party. You can't leave party when you're not in one.
```cpp
FRegistry::Lobby.SendLeavePartyRequest();
```
See AccelByte::Api::Lobby::SendLeavePartyRequest().

#### Invite Someone to Party
Invite someone to party.
```cpp
FString UserId = TEXT("UserId");

FRegistry::Lobby.SendInviteToPartyRequest(UserId);
```
See AccelByte::Api::SendInviteToPartyRequest().

#### Set Presence Status
Set presence status on lobby service
```cpp
Availability availability = Availabe;
FString activity = TEXT("Active");

FRegistry::Lobby.SendSetPresenceStatus(availability, activity);
```
See AccelByte::Api::Lobby::SendSetPresenceStatus().

#### Accept a Party Invitation
Accept a party invitation. `PartyId` and `InvitationToken` are from the invitation notice.
```cpp
FString partyId;
FString invitationToken;

FRegistry::Lobby.SendAcceptInvitationRequest(partyId, invitationToken);
```
See AccelByte::Api::Lobby::SendAcceptInvitationRequest().

#### Kick a Party Member
Only party leader can kick a party member.
```cpp
FString userId;

FRegistry::Lobby.SendKickPartyMemberRequest(userId);
```
See AccelByte::Api::Lobby::SendKickPartyMemberRequest().

#### Get All Online Users
Get a list of all online users in the Lobby server.
```cpp
FRegistry::Lobby.SendGetOnlineUsersRequest();
```
See AccelByte::Api::Lobby::SendGetOnlineUsersRequest();

### Notification

#### Get All Asynchronous Notifications
Get all pending notification(s) that is sent to user when user is not connected to lobby. Please call this function after user connected to lobby.
```cpp
FRegistry::Lobby.GetAllAsyncNotification();
```
See AccelByte::Api::Lobby::GetAllAsyncNotification().

### Matchmaking

#### Send Start Matchmaking
Start the matchmaking.
```cpp
FString gameMode;

FRegistry::Lobby.SendStartMatchmaking(gameMode);
```
See AccelByte::Api::Lobby::SendStartMatchmaking().

#### Send Cancel Matchmaking
Cancel the currently running matchmaking process.
```cpp
FString gameMode;

FRegistry::Lobby.SendCancelMatchmaking(gameMode);
```
See AccelByte::Api::Lobby::SendCancelMatchmaking().

### Friends

#### Request Friend
Send request friend request.
```cpp
FString userId;

FRegistry::Lobby.RequestFriend(userId);
```
See AccelByte::Api::Lobby::RequestFriend().

#### Unfriend
Send unfriend request.
```cpp
FString userId;

FRegistry::Lobby.Unfriend(userId);
```
See AccelByte::Api::Lobby::Unfriend().

#### List Outgoing Friends
Send list of outgoing friends request.
```cpp
FRegistry::Lobby.ListOutgoingFriends();
```
See AccelByte::Api::Lobby::ListOutgoingFriends().

#### Cancel Friend Request
Send cancel friend request.
```cpp
FString userId;

FRegistry::Lobby.CancelFriendRequest(userId);
```
See AccelByte::Api::Lobby::CancelFriendRequest().

#### List Incoming Friends
Send list of incoming friends request.
```cpp
FRegistry::Lobby.ListIncomingFriends();
```
See AccelByte::Api::Lobby::ListIncomingFriends().

#### Accept Friend
Send accept friend request.
```cpp
FString userId;

FRegistry::Lobby.AcceptFriend(userId);
```
See AccelByte::Api::Lobby::AcceptFriend().

#### Reject Friend
Send reject friend request.
```cpp
FString userId;

FRegistry::Lobby.RejectFriend(userId);
```
See AccelByte::Api::Lobby::RejectFriend().

#### Load Friends List
Send load friends list request.
```cpp
FRegistry::Lobby.LoadFriendsList();
```
See AccelByte::Api::Lobby::LoadFriendsList().

#### Get Friendship Status
Send get friendship status request.
```cpp
FString userId;

FRegistry::Lobby.GetFriendshipStatus(userId);
```
See AccelByte::Api::Lobby::GetFriendshipStatus().

## Game Profile
GameProfile API to manage user's in-game profiles (character).
See AccelByte::Api::GameProfile,

### Batch Get Public Game Profiles
Get public game profiles from the specified user id(s).
```cpp
TArray<FAccelByteModelsPublicGameProfile> GetBatchPublicGameProfilesResult;
bool bBatchGetPublicGameProfiles = false;
Credentials GameProfileCred;
AccelByte::Api::GameProfile gameProfile(GameProfileCred, FRegistry::Settings);
TArray<FString> ArrayOfUserId;
ArrayOfUserId.Add(TEXT("userId12345"));
ArrayOfUserId.Add(TEXT("userId23456"));

gameProfile.BatchGetPublicGameProfiles(ArrayOfUserId, THandler<TArray<FAccelByteModelsPublicGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsPublicGameProfile>& Result)
    {
        bBatchGetPublicGameProfiles = true;
        GetBatchPublicGameProfilesResult = Result;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::GameProfile::BatchGetPublicGameProfiles().

### Get All Game Profiles
Get all user's game profiles.
```cpp
TArray<FAccelByteModelsGameProfile> GetAllGameProfileResult;
bool bGetAllGameProfileSuccess = false;
Credentials GameProfileCred;
AccelByte::Api::GameProfile gameProfile(GameProfileCred, FRegistry::Settings);

gameProfile.GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsGameProfile>& Result){
            UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\tsuccess"));
            GetAllGameProfileResult = Result;
            bGetAllGameProfileSuccess = true;
        }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::GameProfile::GetAllGameProfiles().

### Create Game Profile
Create a game profile.
```cpp
int32 attributeLength = 5;
FString attributeKeyPrefix = TEXT("key");
FString attributeValuePrefix = TEXT("val");
int32 tagsLength = 3;
FString tagPrefix = "tag";
FString avatarUrl = TEXT("http://example.com/");
FString label = TEXT("DefaultLabel");
FString profileName = TEXT("DefaultName");

FAccelByteModelsGameProfileRequest Request = GenerateGameProfileRequest(attributeLength, attributeKeyPrefix, attributeValuePrefix, tagsLength, tagPrefix, avatarUrl, label, profileName);
FAccelByteModelsGameProfile ActualResult;
bool bCreateGameProfileSuccess = false;
Credentials GameProfileCred;
AccelByte::Api::GameProfile gameProfile(GameProfileCred, FRegistry::Settings);

gameProfile.CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&ActualResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result){
        ActualResult = Result;
        bCreateGameProfileSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::GameProfile::CreateGameProfile().

### Get Game Profile
Get a specific game profile.
```cpp
FAccelByteModelsGameProfile GetResult;
bool bGetGameProfileSuccess = false;
Credentials GameProfileCred;
AccelByte::Api::GameProfile gameProfile(GameProfileCred, FRegistry::Settings);
FString profileId;

gameProfile.GetGameProfile(profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGetGameProfileSuccess, &GetResult](const FAccelByteModelsGameProfile& Result){
        GetResult = Result;
        bGetGameProfileSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::GameProfile::GetGameProfile().

### Update Game Profile
Update a specific game profile.
```cpp
int32 oldAttributeLength = 5;
FString newAttributeKeyPrefix = TEXT("newKey");
FString newAttributeValuePrefix = TEXT("newVal");
int32 oldTagsLength = 3;
FString newTagPrefix = "newTag";
FString newAvatarUrl = TEXT("http://example.com/new");
FString newLabel = TEXT("NewLabel");
FString newProfileName = TEXT("NewName");
FAccelByteModelsGameProfileRequest UpdateRequest = GenerateGameProfileRequest(oldAttributeLength+1, newAttributeKeyPrefix, newAttributeValuePrefix, oldTagsLength+1, newTagPrefix, newAvatarUrl, newLabel, newProfileName);
bool bUpdateGameProfileSuccess = false;
FAccelByteModelsGameProfile UpdateResult;
Credentials GameProfileCred;
AccelByte::Api::GameProfile gameProfile(GameProfileCred, FRegistry::Settings);
FString profileId;

gameProfile.UpdateGameProfile(profileId, UpdateRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bUpdateGameProfileSuccess, &UpdateResult](const FAccelByteModelsGameProfile& Result){
        UpdateResult = Result;
        bUpdateGameProfileSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::GameProfile::UpdateGameProfile().

### Delete Game Profile
Delete a specific game profile.
```cpp
bool bDeleteGameProfileSuccess = false;
Credentials GameProfileCred;
AccelByte::Api::GameProfile gameProfile(GameProfileCred, FRegistry::Settings);
FString profileId;

gameProfile.DeleteGameProfile(profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]() {
        bDeleteGameProfileSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::GameProfile::DeleteGameProfile().

### Get Game Profile Attribute
Get an attribute value from a game profile.
```cpp
bool bGameProfileDoesntExist = false;
Credentials GameProfileCred;
AccelByte::Api::GameProfile gameProfile(GameProfileCred, FRegistry::Settings);
FString profileId;

gameProfile.GetGameProfile(profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGameProfileDoesntExist](const FAccelByteModelsGameProfile& Result) {
        bGameProfileDoesntExist = Result.profileId.IsEmpty();
    }), FErrorHandler::CreateLambda([&bGameProfileDoesntExist](int32 Code, FString Message) {
        bGameProfileDoesntExist = Code == EHttpResponseCodes::NotFound;
    }));
```
See AccelByte::Api::GameProfile::GetGameProfileAttribute().

### Update Game Profile Attribute
Update an attribute from a game profile.
```cpp
int32 attributeLength = 5;
FString attributeKeyPrefix = TEXT("key");
FString attributeValuePrefix = TEXT("val");
int32 tagsLength = 3;
FString tagPrefix = "tag";
FString avatarUrl = TEXT("http://example.com/");
FString label = TEXT("DefaultLabel");
FString profileName = TEXT("DefaultName");

FAccelByteModelsGameProfileRequest Request = GenerateGameProfileRequest(attributeLength, attributeKeyPrefix, attributeValuePrefix, tagsLength, tagPrefix, avatarUrl, label, profileName);
TArray<FString> AttributeKeys;
Request.attributes.GetKeys(AttributeKeys);
FString TestAttributeKey = AttributeKeys[0];
FString UpdateAttributeValue = "updateVALUE";
FAccelByteModelsGameProfile UpdateResult;
FAccelByteModelsGameProfileAttribute AttributeRequest{ TestAttributeKey , UpdateAttributeValue };
bool bUpdateGameProfileAttributeSuccess = false;
Credentials GameProfileCred;
AccelByte::Api::GameProfile gameProfile(GameProfileCred, FRegistry::Settings);
FString profileId;

gameProfile.UpdateGameProfileAttribute(profileId, AttributeRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result)
    {
        bUpdateGameProfileAttributeSuccess = true;
        UpdateResult = Result;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::GameProfile::UpdateGameProfileAttribute().

## Cloud Storage
Cloud Storage API for storing binary data on the cloud.
See AccelByte::Api::CloudStorage,

### Get All Slots
This function gets list of slot(s) those owned by the player.
```cpp
bool bGetAllSlotsResult = false;
TArray<FAccelByteModelsSlot> Results;
CloudStorage::GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([&](const TArray<FAccelByteModelsSlot>& Slots)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Get All Slots Success"));
        Results = Slots;
        bGetAllSlotsResult = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::CloudStorage::GetAllSlots().

### Create Slot
This function creates a slot for an uploaded binary data.
```cpp
FString PayloadString = TEXT("PayloadOne");
FString fileName = TEXT("create.txt");
TArray<FString> tags = {TEXT("tag")};
FString label = TEXT("label");
FString customAttribute = TEXT("level=5;str=3;int=5;agi=4");
bool bSlotCreatedResult = false;

TArray<uint8> Payload;
Payload.AddUninitialized(PayloadString.Len());
StringToBytes(PayloadString, Payload.GetData(), PayloadString.Len());

CloudStorage::CreateSlot(Payload, fileName, tags, label, customAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Create Slot Success"));
        CreatedSlot = Result;
        bSlotCreatedResult = true;
    }), nullptr, FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::CloudStorage::CreateSlot().

### Update Slot
This function updates a stored slot.
```cpp
bool bSlotUpdatedResult = false;
FString SlotId = "1234567890" // example of slot ID that owned by the user;
FString PayloadString = TEXT("PayloadOne");
FString newFileName = TEXT("update.txt");
FString newTag = TEXT("newTag");
FString newLabel = TEXT("newLabel");
FString customAttribute = TEXT("level=8;str=6;int=9;agi=7");

TArray<uint8> Payload;
Payload.AddUninitialized(PayloadString.Len());
StringToBytes(PayloadString, Payload.GetData(), PayloadString.Len());

CloudStorage::UpdateSlot(SlotId, Payload, newFileName, newTag, newLabel, customAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Update Slot Success"));
        bSlotUpdatedResult = true;
    }), nullptr, FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::CloudStorage::UpdateSlot().

### Update Slot Metadata
This function updates stored slot's metadata.
```cpp
FString SlotId = "1234567890" // example of slot ID that owned by the user;
FString UpdateFileName = "metadataUpdate.txt";
TArray<FString> UpdateTags;
UpdateTags.Add("tagOne");
UpdateTags.Add("tagTwo");
FString UpdateLabel = "metadataLabel";
FString UpdateCustomAttribute = TEXT("This is a custom attribute");

CloudStorage::UpdateSlotMetadata(CreatedSlot.SlotId, UpdateFileName, UpdateTags, UpdateLabel, UpdateCustomAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
{
	UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Update Metadata Success"));
	bMetadataUpdatedResult = true;
	}),  nullptr, FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
	}));
```

### Get Slot
This function gets the data that stored in the slot.
```cpp
bool bGetSlotResult = false;
FString SlotId = "1234567890" // example of slot ID that owned by the user;

CloudStorage::GetSlot(SlotId, CloudStorage::FGetSlotSuccess::CreateLambda([&](const TArray<uint8>& Data)
    {
        bGetSlotResult = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::CloudStorage::GetSlot().

### Delete Slot
This function delete the specified slot.
```cpp
bool bDeleteSlotResult = false;
FString SlotId = "1234567890" // example of slot ID that owned by the user;

CloudStorage::DeleteSlot(SlotId, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Delete Slot Success"));
        bDeleteSlotResult = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See AccelByte::Api::CloudStorage::DeleteSlot().