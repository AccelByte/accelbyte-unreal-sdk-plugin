
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
    - [Presence](#presence)
    - [Notification](#notification)
    - [Matchmaking](#matchmaking)
    - [Friends](#friends)
  - [Game Profile](#game-profile)
  - [Statistic](#statistic)
  - [Cloud Storage](#cloud-storage)


## User Management and Authorization

See `AccelByte::Api::User`, `AccelByte::Api::Oauth2`, `AccelByte::Credentials`,

### Login with Client Credentials
This is to get access token from `client_credentials` grant type, then store the access token in memory. 
You shouldn't use this if your application is a client. It is intended for machine-to-machine (server).
It is "required" for user management (create new user, reset password, etc). See AccelByte IAM documentation for how this OAuth2 token endpoint works.

```cpp
bool bClientLoggedIn = false;

FRegistry::User.LoginWithClientCredentials(FVoidHandler::CreateLambda([&bClientLoggedIn]()
    {
        UE_LOG(LogAccelByteUserTest, Display, TEXT("Success."));
        bClientLoggedIn = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```

See `AccelByte::Api::User::LoginWithClientCredentials()`.

### Login with Username
Log in with email/phone number account.

```cpp
const FString OriginalEmail = TEXT("originalEmail@example.com");
const FString Password = TEXT("password");
bool bUserLoggedIn = false;

FRegistry::User.LoginWithUsername(OriginalEmail, Password, FVoidHandler::CreateLambda([&bUserLoggedIn]()
    {
        UE_LOG(LogAccelByteUserTest, Display, TEXT("Success."));
        bUserLoggedIn = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::LoginWithUsername()`.

### Login with Third Party Platform

This is to get access token by using a third party platform account e.g. Steam, Google, Facebook, Twitch, etc. See AccelByte IAM documentation for how this OAuth2 token endpoint works.

```cpp
bool bSteamLoginSuccessful1 = false;

FRegistry::User.LoginWithOtherPlatform(EAccelBytePlatformType::Steam, GetSteamTicket(), FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bSteamLoginSuccessful1 = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::LoginWithOtherPlatform()`.

### Login with Device Id

Register / login with device id is very similar to a login with a third party platform. The user account created from this process is also a headless account.

```cpp
bool bDeviceLoginSuccessful1 = false;

FRegistry::User.LoginWithDeviceId(FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bDeviceLoginSuccessful1 = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::LoginWithDeviceId()`.

### Login with Launcher
If you use the AccelByte Launcher, the game will login into the same user as the user that is logged in to the Launcher.

```cpp
bool bLauncherLoginSuccessful1 = true;

FRegistry::User.LoginWithLauncher(FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bLauncherLoginSuccessful1 = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::LoginWithLauncher()`.

### Logout
Logout will remove the user credentials from memory. User needs to re-login after logout.

```cpp
FRegistry::User.ForgetAllCredentials();
```
See `AccelByte::Api::User::ForgetAllCredentials()`.

### Register
This function will register a new user with email-based account.

```cpp
const FString OriginalEmail = TEXT("originalEmail@example.com");
const FString Password = TEXT("password");
const FString DisplayName = TEXT("testName");
const FString Country = TEXT("US");
const FString DateOfBirth = TEXT("2000-12-20");
bool bUserAccountCreated = false;

FRegistry::User.Register(OriginalEmail, Password, DisplayName, Country, DateOfBirth, THandler<FUserData>::CreateLambda([&bUserAccountCreated](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Display, TEXT("Success."));
        bUserAccountCreated = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::Register()`.

### Get Data
This function will get data of currently logged in user.

```cpp
bool bGetDataSuccessful1 = false;
FUserData GetDataResult;
FRegistry::User.GetData(THandler<FUserData>::CreateLambda([&](const FUserData& Result) 
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bGetDataSuccessful = true;
        GetDataResult = Result;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::GetData()`.

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

FRegistry::User.Update(UpdateRequest, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Display, TEXT("Success."));
        bUserUpdated = true;
        UpdateResult = Result;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::Update()`.

### Upgrade
This function will upgrade user's headless account. You may call SendVerificationCode afterwards.
Headless account is an account that doesn't have an email and password. If a user logs in with a device/platform and they cannot login with email and password, their account is considered as a headless account.
Therefore, the function requests user’s Username and Password for parameters.

```cpp
FString Email = TEXT("testSDK@game.test");
FString Password = TEXT("password");
bool bUpgradeSuccessful = false;

FRegistry::User.Upgrade(Email, Password, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bUpgradeSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::Upgrade()`.

### Send Verification Code 
Verify user's email. User should login with email and password first to get access token.

```cpp
bool bSendSuccessful = false;

FRegistry::User.SendVerificationCode(FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bSendSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::SendVerificationCode()`.

### Verify
This function will verify the registered email **after** user receives verification code sent with ::SendVerificationCode() to their email.

```cpp
bool bGetVerificationCode = false;
FString VerificationCode = GetVerificationCodeFromUserId(FRegistry::Credentials.GetUserId());

FRegistry::User.Verify(VerificationCode, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bGetVerificationCode = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::Verify()`.

### Send Reset Password Code
For a password reset, verification code is sent to the email.

```cpp
FString LoginId = TEXT("testeraccelbyte@game.test");
bool bForgotPaswordSuccessful = false;

FRegistry::User.SendResetPasswordCode(LoginId, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bForgotPaswordSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::SendResetPasswordCode()`.

### Reset Password
Reset user's password with sent verification code.

```cpp
FString LoginId = TEXT("testeraccelbyte@game.test");
FString Password = "new_password";
FString VerificationCode = GetVerificationCode(LoginId);
bool bResetPasswordSuccessful = false;

FRegistry::User.ResetPassword(VerificationCode, LoginId, Password, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bResetPasswordSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::ResetPassword()`.

### Send Upgrade Verification Code
This function should be done before user upgrade their headless account. After this function successfully called, obtain the verification code from the submitted email. Then call UpgradeHeadlessAccountWithVerificationCode function afterwards.

```cpp
FString Email = TEXT("upgradeAndVerify@example.com");
bool bSendUserUpgradeVerificationCode = false;

FRegistry::User.SendUpgradeVerificationCode(Email, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bSendUserUpgradeVerificationCode = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::SendUpgradeVerificationCode()`.

### Upgrade and Verify
This function should be called after you call SendUserUpgradeVerificationCode and obtain verification code.

```cpp
FString Email = TEXT("upgradeAndVerify@example.com");
FString Password = TEXT("password");
bool bGetVerificationCode = false;
FString VerificationCode = GetVerificationCodeFromUserId(FRegistry::Credentials.GetUserId());

FRegistry::User.UpgradeAndVerify(Email, Password, VerificationCode, THandler<FUserData>::CreateLambda([&](const FUserData& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bGetVerificationCode = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::UpgradeAndVerify()`.

### Get Third Party Platform Links
This function gets user's platform accounts linked to user’s account.

```cpp
bool bGetPlatformLinksSuccessful = false;

FRegistry::User.GetPlatformLinks(FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bGetPlatformLinksSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::GetPlatformLinks()`.

### Link Third Party Platform
This function links user's current account to their other account in other platform.
Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch platform).
The browser will redirect the URL to a site with a code in form of parameter URL.

```cpp
bool bLinkOtherPlatformSuccessful = false;
FString PlatformId = TEXT("PlatformId");
FString Ticket = TEXT("Ticket");

FRegistry::User.LinkOtherPlatform(PlatformId, Ticket, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bLinkOtherPlatformSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::LinkOtherPlatform()`.

### Unlink Third Party Platform
This function links user's current account to their other account in other platform
Ticket for each platform (PlatformToken) can be obtained from browser with platform linking URL (e.g. Facebook, Google, Twitch).
The browser will redirect the URL to a site with a code in form of parameter URL.

```cpp
bool bUnlinkOtherPlatformSuccessful = false;
FString PlatformId = TEXT("PlatformId");

FRegistry::User.UnlinkOtherPlatform(PlatformId, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bLinkOtherPlatformSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::User::UnlinkOtherPlatform()`.

---

## User Profile
API to create, update, and get user's profile.
See `AccelByte::Api::UserProfile`,

### Get User Profile
Get a user's profile information. If it doesn't exist, that will be an error.

```cpp
bool bGetProfileSuccessful1 = false;

FRegistry::UserProfile.GetUserProfile(THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
        bGetProfileSuccessful1 = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::UserProfile::GetUserProfile()`.

### Update User Profile
Update user's current profile information. If it doesn't exist, that will be an error.

```cpp
FAccelByteModelsUserProfileUpdateRequest ProfileUpdate;
    ProfileUpdate.Language = "en";
    ProfileUpdate.Timezone = "Etc/UTC";
    ProfileUpdate.DateOfBirth = "2000-01-01";
bool bUpdateProfileSuccessful = false;

FRegistry::UserProfile.UpdateUserProfile(ProfileUpdate, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("Success"));
        bUpdateProfileSuccessful = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteUserTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::UserProfile::UpdateUserProfile()`.

### Create User Profile
Create complete player profile. If it already exists, that will be an error.

```cpp
bool bCreateProfileSuccessful1 = false;
FAccelByteModelsUserProfileCreateRequest ProfileCreate;
    ProfileCreate.Language = "en";
    ProfileCreate.Timezone = "Etc/UTC";
    ProfileCreate.DateOfBirth = "1970-01-01";

FRegistry::UserProfile.CreateUserProfile(ProfileCreate, THandler<FAccelByteModelsUserProfileInfo>::CreateLambda([&](const FAccelByteModelsUserProfileInfo& Result)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("    Success"));
        bCreateProfileSuccessful1 = true;
    }), FErrorHandler::CreateLambda([&](int32 Code, FString Message)
    {
        UE_LOG(LogAccelByteUserTest, Log, TEXT("    Fail: %d %s"), Code, *Message);
        bCreateProfileSuccessful1 = false;
    }));
```
See `AccelByte::Api::UserProfile::CreateUserProfile()`.

---

## Ecommerce
API related to Ecommerce.
See `AccelByte::Api::Category`, `AccelByte::Api::Item`, `AccelByte::Api::Order`, `AccelByte::Api::Wallet`,

### Categories
Category API for buying things from the online store.
The category has [tree data structure](https://en.wikipedia.org/wiki/Tree_(data_structure)). Each category has path, for example "/equipments/armor/legs". Each category has items inside it. You can get a list of items by criteria or by its ID.
See `AccelByte::Api::Category`,

#### Get Root Categories
This function gets root categories that exist in the specified namespace.
This is equivalent of getting categories in "/". This will yield, for example, "/equipments", "/consumables", "/pay2win", "/boosts".

```cpp
FString Language = TEXT("en");
bool bGetRootCategoriesSuccess = false;
bool bExpectedRootCategoryFound = false;

FRegistry::Category.GetRootCategories(Language, Category::FGetRootCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
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
See `AccelByte::Api::Category::GetRootCategories()`.

#### Get Category
This function gets the category from a store in the specified namespace, for example, "/equipments/armor/torso".

```cpp
FString CategoryPath = TEXT("/equipments/armor/torso");
FString Language = TEXT("en");
bool bGetCategorySuccess = false;
    
FRegistry::Category.GetCategory(CategoryPath, Language, Category::FGetCategorySuccess::CreateLambda([&](FAccelByteModelsFullCategoryInfo Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetCategorySuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::Category::GetCategory()`.

#### Get Child Categories
This function gets the subcategories from a category in the specified namespace.

```cpp
FString Language = TEXT("en");
FString CategoryPath = TEXT("/equipments/pay2win/super_badass_armor/chest");
bool bGetChildCategoriesSuccess = false;
bool bExpectedChildCategoryFound = false;

FRegistry::Category.GetChildCategories(Language, CategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
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
See `AccelByte::Api::Category::GetChildCategories()`.

#### Get Descendant Categories
This will get every subcategory from a category in the specified namespace.

```cpp
FString Language = TEXT("en");
FString CategoryPath = TEXT("/equipments/pay2win/super_badass_armor/chest");
bool bGetDescendantCategoriesSuccess = false;
bool bExpectedDescendantCategoryFound1 = false;
bool bExpectedDescendantCategoryFound2 = false;
    
FRegistry::Category.GetDescendantCategories(Language, CategoryPath, Category::FGetChildCategoriesSuccess::CreateLambda([&](TArray<FAccelByteModelsFullCategoryInfo> Results)
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
See `AccelByte::Api::Category::GetDescendantCategories()`.

### Items
Item API for buying things from the online store. An item represents a single product sold in the online store. Each category has items inside it. You can get a list of items by criteria or by its ID.
See `AccelByte::Api::Item`,

#### Get Item by ID
Get an items information from an online store.

```cpp
FString ItemId = TEXT("YourItemId");
FString Language = TEXT("en");
FString Region = TEXT("US");
bool bGetItemByIdSuccess = false;
    
FRegistry::Item.GetItemById(ItemId, Language, Region, Item::FGetItemByIdSuccess::CreateLambda([&](const FAccelByteModelsItemInfo& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetItemByIdSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::Item::GetItemById()`.

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
    
FRegistry::Item.GetItemsByCriteria(Language, Region, CategoryPath, Type, Status, 0, 20, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
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
See `AccelByte::Api::Item::GetItemsByCriteria()`.

#### Search Items
Search items by keyword in title, description and long description from a published store. Language constrained. If the item does not exist in the specified region, the default region item will be returned.

```cpp
FString Language = TEXT("en");
FString Region = TEXT("US");
int PageNumber = 0;
int PageSize = 10;
FString SearchedItem = TEXT("Elixir");
bool bSearchItemSuccess = false;
bool bSearchedItemFound = false;
UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("SearchItem"));
FRegistry::Item.SearchItem(Language, SearchedItem, PageNumber, PageSize, Region, THandler<FAccelByteModelsItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsItemPagingSlicedResult& Result)
    {
        for (int i = 0; i < Result.Data.Num(); i++)
        {
            if (Result.Data[i].Title == SearchedItem)
            {
                UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Searched item is found!"));
                bSearchedItemFound = true;
            }
        }
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("    Finished"));
        bSearchItemSuccess = true;
	}), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));

```

### Order
Order is used to purchase something from the online store.
See `AccelByte::Api::Order`

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
    
FRegistry::Order.CreateNewOrder(OrderCreate, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](FAccelByteModelsOrderInfo Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bCreateNewOrderSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::Order::CreateNewOrder()`.

#### Get User Order
Get a single order with order number.

```cpp
FString OrderNo = TEXT("YourOrderNo");
bool bGetUserOrderSuccess = false;
    
FRegistry::Order.GetUserOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetUserOrderSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::Order::GetUserOrder()`.

#### Get User Orders
Get all of a user's orders that have been created, with paging.

```cpp
const int32 page = 0;
const int32 size = 20;
bool bGetUserOrdersSuccess = false;
    
FRegistry::Order.GetUserOrders(page, size, THandler<FAccelByteModelsOrderInfoPaging>::CreateLambda([&](const FAccelByteModelsOrderInfoPaging& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetUserOrdersSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::Order::GetUserOrders()`.

#### Get Order History
Get the order history of a specified user.

```cpp
FString OrderNo = TEXT("YourOrderNo");
bool bGetUserOrderHistorySuccess = false;
    
FRegistry::Order.GetUserOrderHistory(OrderNo, THandler<TArray<FAccelByteModelsOrderHistoryInfo>>::CreateLambda([&](const TArray<FAccelByteModelsOrderHistoryInfo>& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetUserOrderHistorySuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::Order::GetUserOrderHistory()`.

#### Fulfill order
Fulfill an order if the order is charged but the fulfillment fail.

```cpp
FString OrderNo = TEXT("YourOrderNo");
bool bFulfillOrderSuccess = false;
    
FRegistry::Order.FulfillOrder(OrderNo, THandler<FAccelByteModelsOrderInfo>::CreateLambda([&](const FAccelByteModelsOrderInfo& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bFulfillOrderSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Api::Order::FulfillOrder()`.

### Wallet
Wallet API for buying things from the online store; a wallet can be a virtual or real currency.
See `AccelByte::Api::Wallet`

#### Get Wallet Info by Currency Code
Get user's wallet information for a specific currency code.

```cpp
FString ExpectedCurrencyCode = TEXT("CurrencyCode");
bool bGetWalletSuccess = false;
    
FRegistry::Wallet.GetWalletInfoByCurrencyCode(ExpectedCurrencyCode, THandler<FAccelByteModelsWalletInfo>::CreateLambda([&](const FAccelByteModelsWalletInfo& Result)
    {
        UE_LOG(LogAccelByteEcommerceTest, Log, TEXT("Success"));
        bGetWalletSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, const FString& ErrorMessage)
    {
        UE_LOG(LogAccelByteEcommerceTest, Fatal, TEXT("Error. Code: %d, Reason: %s"), ErrorCode, *ErrorMessage)
    }));
```
See `AccelByte::Wallet::GetWalletInfoByCurrencyCode()`.

### Entitlement
Entitlement API for checking a users’s entitlements. User can query a list of item that belongs to him/her.
See `AccelByte::Api::Entitlement`

#### Query User Entitlement
Get a list of entitlements that belong to the user.
```cpp
bool bQueryEntitlementSuccess = false;
bool bQueryResultTrue = false;
FString entitlementName = TEXT("");
FString itemId = TEXT("");
int32 page = 0;
int32 size = 20;
EAccelByteEntitlementClass entitlementClass = EAccelByteEntitlementClass::NONE;
EAccelByteAppType appType = EAccelByteAppType::NONE; 

FRegistry::Entitlement.QueryUserEntitlement(entitlementName, itemId, page, size,
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
See `AccelByte::Api::Entitlement::QueryUserEntitlement()`.

---

## Lobby
Lobby is for chatting and party management. Unlike other servers which use HTTP, Lobby server uses WebSocket ([RFC 6455](https://tools.ietf.org/html/rfc6455)).

![lobby-api](http://www.plantuml.com/plantuml/png/TP3DgeCm44RtynI3Uz_WSj5xIw7q1y6bk8ZZL8CnAN5i4V7TAxLGA6GvdCF7S991b9UnDo1Q3EF9LTK4GCQDDSxmEl4dhjt3nbXagpjXplwkYSjl-jyg2SBCyy2ME2ZilMPR3q5-SQAlcfHePuFIMGcrlUgqRJxE0d1JrAc6CKg9sFnDmfZlZX7EK6mBQNOeSLlvVKTO85aVMjCy0G00)

Lobby is a collection of services that are connected together through a websocket connection. Those services are:

1. Party Service
2. Chat Service
3. Friends Service
4. Presence Service
5. Notification Service
6. Matchmaking Service

### Lobby Protocol

AccelByte Lobby Protocol closely follows RPC model and is described by its message format, which is a subset of YAML. The message is divided into two parts: header and payload. Header fields are type, id, and code (optional), while payload fields depends on the type.

Request Example:

```text
type: someRequest\n
id: id123\n
payloadFieldBool1: true\n
payloadFieldDouble2: 2.0\n
payloadFieldInt3: 3\n
payloadFieldStr4: some text message
```

Response code 0 means the request returned an OK response, while other code means the request returned an error response. Request and Response come in pair, so that a pair of request and response have the same id, while an Notification doesn't have an id.

Response OK Example:

```text
type: someResponse\n
id: id123\n
code: 0\n
payloadFieldBool1: true\n
payloadFieldDouble2: 2.0\n
payloadFieldInt3: 3\n
payloadFieldStr4: some text message\n
payloadFieldStrArray5: [item1,item2,item3,item4]
```

Response Error Example:

```text
type: someResponse\n
id: id123\n
code: 14777
```

Notification/Event Example:

```text
type: someNotif\n
payloadFieldBool1: true\n
payloadFieldStrArray5: [item1]
```
### Connect to server
You must connect to the server before you can start sending/receiving. Also make sure you have logged in first as this operation requires access token.

```cpp
AccelByte::FRegistry::Lobby.Connect();
```

### Disconnect from server
Disconnect from server if and only if the you have connected to server. If not currently connected, then this does nothing.
```cpp
AccelByte::FRegistry::Lobby.Disconnect();
```

### Check if connected
Check whether the connection has been established with the server.
```cpp
if (AccelByte::FRegistry::Lobby.IsConnected())
{
    UE_LOG(LogTemp, Log, TEXT("Yay."));
}
else
{
    UE_LOG(LogTemp, Log, TEXT("Nay."));
}
```

### Sending ping
You should send ping every some time (for example, every 4 seconds) so that the server doesn't close the connection.

```cpp
AccelByte::FRegistry::Lobby.SendPing();
```

### Bind delegates
 You must bind delegates/callbacks first to handle the events. For example when a user received a private message or a response to create party request. A delegate which ends with `Notice` means that it's like a notification, while one which ends with `Response` means it's like a response to a request. The delegates can be `nullptr` if you want to not bind the callback. All delegates have one parameter `Result` with different types.
```cpp
    //CONNECTION delegates
    AccelByte::FRegistry::Lobby.SetConnectSuccessDelegate(
        AccelByte::Api::Lobby::FConnectSuccess::CreateLambda([]()
        {
            UE_LOG(LogTemp, Log, TEXT("Connected to lobby!"));
        }));

    AccelByte::FRegistry::Lobby.SetConnectFailedDelegate(
        AccelByte::FErrorHandler::CreateLambda([](int32 Code, const FString& ErrorMessage)
        {
            UE_LOG(LogTemp, Log, TEXT("Failed to establish lobby connection!\nCause: %s\nCode:%d"), *ErrorMessage, Code);
        }));

    AccelByte::FRegistry::Lobby.SetConnectionClosedDelegate(
        AccelByte::Api::Lobby::FConnectionClosed::CreateLambda([](int32 StatusCode, const FString& Reason, bool WasClean)
        {
            UE_LOG(LogTemp, Log, TEXT("Lobby connection closed!\nReason: %s\nCode:%d"), *Reason, StatusCode);
        }));

    AccelByte::FRegistry::Lobby.SetParsingErrorDelegate(
        AccelByte::FErrorHandler::CreateLambda([](int32 Code, const FString& ErrorMessage)
        {
            UE_LOG(LogTemp, Log, TEXT("Failed to parsing message!\nCause: %s\nCode:%d"), *ErrorMessage, Code);
        }));

    //PARTY NOTIFICATION delegates
    AccelByte::FRegistry::Lobby.SetPartyLeaveNotifDelegate(
        AccelByte::Api::Lobby::FPartyLeaveNotif::CreateLambda([](const FAccelByteModelsLeavePartyNotice& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s is leaving your party!"), *Result.UserID);
        }));

    AccelByte::FRegistry::Lobby.SetPartyInviteNotifDelegate(
        AccelByte::Api::Lobby::FPartyInviteNotif::CreateLambda([](const FAccelByteModelsInvitationNotice& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s invites %s to join a party!"), *Result.InviterID, *Result.InviteeID);
        }));

    AccelByte::FRegistry::Lobby.SetPartyGetInvitedNotifDelegate(
        AccelByte::Api::Lobby::FPartyGetInvitedNotif::CreateLambda([](const FAccelByteModelsPartyGetInvitedNotice& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s invites you to join a party!"), *Result.From);
            UE_LOG(LogTemp, Log, TEXT("Party ID = %s"), *Result.PartyId);
            UE_LOG(LogTemp, Log, TEXT("Invitation Token = %s"), *Result.InvitationToken);
        }));

    AccelByte::FRegistry::Lobby.SetPartyJoinNotifDelegate(
        AccelByte::Api::Lobby::FPartyJoinNotif::CreateLambda([](const FAccelByteModelsPartyJoinNotice& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s joins to your party!"), *Result.UserId);
        }));

    AccelByte::FRegistry::Lobby.SetPartyKickNotifDelegate(
        AccelByte::Api::Lobby::FPartyKickNotif::CreateLambda([](const FAccelByteModelsGotKickedFromPartyNotice& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s kick %s from this party (%s)!"), *Result.LeaderId, *Result.UserId, *Result.PartyId);
        }));

    //PARTY RESPONSE delegates
    AccelByte::FRegistry::Lobby.SetInfoPartyResponseDelegate(
        AccelByte::Api::Lobby::FPartyInfoResponse::CreateLambda([](const FAccelByteModelsInfoPartyResponse& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("Here is your current party information."));
            UE_LOG(LogTemp, Log, TEXT("Party ID: %s"), *Result.PartyId);
            UE_LOG(LogTemp, Log, TEXT("Leader's user ID: %s"), *Result.LeaderId);
            for (int i = 0; i < Result.Members.Num(); i++)
            {
                UE_LOG(LogTemp, Log, TEXT("Member %d user ID = %s"), i, *Result.Members[i]);
            }
            UE_LOG(LogTemp, Log, TEXT("Party's invitation token: %s"), *Result.InvitationToken);
        }));

    AccelByte::FRegistry::Lobby.SetCreatePartyResponseDelegate(
        AccelByte::Api::Lobby::FPartyCreateResponse::CreateLambda([](const FAccelByteModelsCreatePartyResponse& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("Party created!"));
            UE_LOG(LogTemp, Log, TEXT("Party ID: %s"), *Result.PartyId);
            UE_LOG(LogTemp, Log, TEXT("Party leader's user ID: %s"), *Result.LeaderId);
            for (int i = 0; i < Result.Members.Num(); i++)
            {
                UE_LOG(LogTemp, Log, TEXT("Member %d user ID = %s"), i, *Result.Members[i]);
            }
            UE_LOG(LogTemp, Log, TEXT("Party's invitation token: %s"), *Result.InvitationToken);
        }));

    AccelByte::FRegistry::Lobby.SetLeavePartyResponseDelegate(
        AccelByte::Api::Lobby::FPartyLeaveResponse::CreateLambda([](const FAccelByteModelsLeavePartyResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully leave party!")); }
            else 
            { UE_LOG(LogTemp, Log, TEXT("Failed to leave party! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetInvitePartyResponseDelegate(
        AccelByte::Api::Lobby::FPartyInviteResponse::CreateLambda([](const FAccelByteModelsPartyInviteResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully invite your friend to your party!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to invite your friend to your party! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetInvitePartyJoinResponseDelegate(
        AccelByte::Api::Lobby::FPartyJoinResponse::CreateLambda([](const FAccelByteModelsPartyJoinReponse& Result)
        {
            if (Result.Code != "0") { UE_LOG(LogTemp, Log, TEXT("Failed to accept a party invitation! Code: %s"), *Result.Code); }
            else
            { 
                UE_LOG(LogTemp, Log, TEXT("Successfully accept a party invitation!"));
                UE_LOG(LogTemp, Log, TEXT("Party ID: %s"), *Result.PartyId);
                UE_LOG(LogTemp, Log, TEXT("Party leader's user ID: %s"), *Result.LeaderId);
                for (int i = 0; i < Result.Members.Num(); i++)
                {
                    UE_LOG(LogTemp, Log, TEXT("Member %d user ID = %s"), i, *Result.Members[i]);
                }
            }
        }));

    AccelByte::FRegistry::Lobby.SetInvitePartyKickMemberResponseDelegate(
        AccelByte::Api::Lobby::FPartyKickResponse::CreateLambda([](const FAccelByteModelsKickPartyMemberResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully kick a member from current party!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to kick a member from current party! Code: %s"), *Result.Code); }
        }));

    //CHAT NOTIFICATION delegates
    AccelByte::FRegistry::Lobby.SetPartyChatNotifDelegate(
        AccelByte::Api::Lobby::FPartyChatNotif::CreateLambda([](const FAccelByteModelsPartyMessageNotice& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s send a party message to this party (%s)!"), *Result.From, *Result.To);
            UE_LOG(LogTemp, Log, TEXT("Message:\n%s"), *Result.Payload);
        }));

    AccelByte::FRegistry::Lobby.SetPrivateMessageNotifDelegate(
        AccelByte::Api::Lobby::FPersonalChatNotif::CreateLambda([](const FAccelByteModelsPersonalMessageNotice& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s send a private message to you (%s)!"), *Result.From, *Result.To);
            UE_LOG(LogTemp, Log, TEXT("Message:\n%s"), *Result.Payload);
        }));

    //CHAT RESPONSE delegates
    AccelByte::FRegistry::Lobby.SetPrivateMessageResponseDelegate(
        AccelByte::Api::Lobby::FPersonalChatResponse::CreateLambda([](const FAccelByteModelsPersonalMessageResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully send a private message!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to send a private message! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetPartyMessageResponseDelegate(
        AccelByte::Api::Lobby::FPartyChatResponse::CreateLambda([](const FAccelByteModelsPartyMessageResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully send a party message!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to send a party message! Code: %s"), *Result.Code); }
        }));

    //PRESENCE NOTIFICATION delegates
    AccelByte::FRegistry::Lobby.SetUserPresenceNotifDelegate(
        AccelByte::Api::Lobby::FFriendStatusNotif::CreateLambda([](const FAccelByteModelsUsersPresenceNotice& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s is %s right now!"), *Result.UserID, *Result.Availability);
            UE_LOG(LogTemp, Log, TEXT("Activity: %s"), *Result.Activity);
        }));

    //PRESENCE RESPONSE delegates
    AccelByte::FRegistry::Lobby.SetUserPresenceResponseDelegate(
        AccelByte::Api::Lobby::FSetUserPresenceResponse::CreateLambda([](const FAccelByteModelsSetOnlineUsersResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully set your presence!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to set your presence! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetGetAllUserPresenceResponseDelegate(
        AccelByte::Api::Lobby::FGetAllFriendsStatusResponse::CreateLambda([](const FAccelByteModelsGetOnlineUsersResponse& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("Here is your friends' current statuses."));
            for (int i = 0 ; i < Result.friendsId.Num() ; i++)
            {
                UE_LOG(LogTemp, Log, TEXT("%s availability is %s & the activity is %s."), *Result.friendsId[i], *Result.availability[i], *Result.activity[i]);
                UE_LOG(LogTemp, Log, TEXT("%s last seen at %s."), *Result.friendsId[i], *Result.lastSeenAt[i]);
            }   
        }));

    //NOTIFICATION delegates
    AccelByte::FRegistry::Lobby.SetMessageNotifDelegate(
        AccelByte::Api::Lobby::FMessageNotif::CreateLambda([](const FAccelByteModelsNotificationMessage& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("There is an incoming notification."));
            UE_LOG(LogTemp, Log, TEXT("From: %s \nTo: %s\nTopic: %s"), *Result.From, *Result.To, *Result.Topic);
            UE_LOG(LogTemp, Log, TEXT("Notification: %s"), *Result.Payload);
        }));

    //MATCHMAKING NOTIFICATION delegates
    AccelByte::FRegistry::Lobby.SetMatchmakingNotifDelegate(
        AccelByte::Api::Lobby::FMatchmakingNotif::CreateLambda([](const FAccelByteModelsMatchmakingNotice& Result)
        {
            const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EAccelByteMatchmakingStatus"), true);

            UE_LOG(LogTemp, Log, TEXT("Here is the detail of your matchmaking result!"));
            UE_LOG(LogTemp, Log, TEXT("MatchId: %s"), *Result.MatchId);
            UE_LOG(LogTemp, Log, TEXT("Status: %s"), *(EnumPtr->GetNameByValue((int8)Result.Status)).ToString());
            for (int i = 0; i < Result.PartyMember.Num(); i++)
            {
                UE_LOG(LogTemp, Log, TEXT("Party member %d user ID = %s"), i, *Result.PartyMember[i]);
            }
            for (int i = 0; i < Result.CounterPartyMember.Num(); i++)
            {
                UE_LOG(LogTemp, Log, TEXT("Enemy member %d user ID = %s"), i, *Result.CounterPartyMember[i]);
            }
        }));

    //MATCHMAKING RESPONSE delegates
    AccelByte::FRegistry::Lobby.SetStartMatchmakingResponseDelegate(
        AccelByte::Api::Lobby::FMatchmakingResponse::CreateLambda([](const FAccelByteModelsMatchmakingResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully start a matchmaking!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to start a matchmaking! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetCancelMatchmakingResponseDelegate(
        AccelByte::Api::Lobby::FMatchmakingResponse::CreateLambda([](const FAccelByteModelsMatchmakingResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully cancel a matchmaking!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to cancel a matchmaking! Code: %s"), *Result.Code); }
        }));

    //FRIEND NOTIFICATION delegates
    AccelByte::FRegistry::Lobby.SetOnFriendRequestAcceptedNotifDelegate(
        AccelByte::Api::Lobby::FAcceptFriendsNotif::CreateLambda([](const FAccelByteModelsAcceptFriendsNotif& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s accepted your friend request!"), *Result.friendId);
        }));

    AccelByte::FRegistry::Lobby.SetOnIncomingRequestFriendsNotifDelegate(
        AccelByte::Api::Lobby::FRequestFriendsNotif::CreateLambda([](const FAccelByteModelsRequestFriendsNotif& Result)
        {
            UE_LOG(LogTemp, Log, TEXT("%s send you a friend request!"), *Result.friendId);
        }));

    //FRIEND RESPONSE delegates
    AccelByte::FRegistry::Lobby.SetRequestFriendsResponseDelegate(
        AccelByte::Api::Lobby::FRequestFriendsResponse::CreateLambda([](const FAccelByteModelsRequestFriendsResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully send a friend request!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to send a friend request! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetUnfriendResponseDelegate(
        AccelByte::Api::Lobby::FUnfriendResponse::CreateLambda([](const FAccelByteModelsUnfriendResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully unfriend a friend!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to unfriend a friend! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetListOutgoingFriendsResponseDelegate(
        AccelByte::Api::Lobby::FListOutgoingFriendsResponse::CreateLambda([](const FAccelByteModelsListOutgoingFriendsResponse& Result)
        {
            if (Result.Code != "0") { UE_LOG(LogTemp, Log, TEXT("Cannot retrieve the list of outgoing/pending friend request!")); }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Here is the list of outgoing/pending friend request.")); 
                for (int i = 0 ; i < Result.friendsId.Num() ; i++ )
                {
                    UE_LOG(LogTemp, Log, TEXT("Friend user ID = %s"), *Result.friendsId[i]); 
                }
            }
        }));

    AccelByte::FRegistry::Lobby.SetCancelFriendsResponseDelegate(
        AccelByte::Api::Lobby::FCancelFriendsResponse::CreateLambda([](const FAccelByteModelsCancelFriendsResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully cancel an outgoing friend request!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to cancel an outgoing friend request! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetListIncomingFriendsResponseDelegate(
        AccelByte::Api::Lobby::FListIncomingFriendsResponse::CreateLambda([](const FAccelByteModelsListIncomingFriendsResponse& Result)
        {
            if (Result.Code != "0") { UE_LOG(LogTemp, Log, TEXT("Cannot retrieve the list of incoming friend request!")); }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Here is the list of incoming friend request.")); 
                for (int i = 0 ; i < Result.friendsId.Num() ; i++ )
                {
                    UE_LOG(LogTemp, Log, TEXT("Friend user ID = %s"), *Result.friendsId[i]); 
                }
            }
        }));

    AccelByte::FRegistry::Lobby.SetAcceptFriendsResponseDelegate(
        AccelByte::Api::Lobby::FAcceptFriendsResponse::CreateLambda([](const FAccelByteModelsAcceptFriendsResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully accept a friend request!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to accept a friend request! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetRejectFriendsResponseDelegate(
        AccelByte::Api::Lobby::FRejectFriendsResponse::CreateLambda([](const FAccelByteModelsRejectFriendsResponse& Result)
        {
            if (Result.Code == "0") { UE_LOG(LogTemp, Log, TEXT("Successfully reject a friend request!")); }
            else
            { UE_LOG(LogTemp, Log, TEXT("Failed to reject a friend request! Code: %s"), *Result.Code); }
        }));

    AccelByte::FRegistry::Lobby.SetLoadFriendListResponseDelegate(
        AccelByte::Api::Lobby::FLoadFriendListResponse::CreateLambda([](const FAccelByteModelsLoadFriendListResponse& Result)
        {
            if (Result.Code != "0") { UE_LOG(LogTemp, Log, TEXT("Cannot retrieve the your friend list!")); }
            else
            {
                UE_LOG(LogTemp, Log, TEXT("Here is the list of your friend.")); 
                for (int i = 0 ; i < Result.friendsId.Num() ; i++ )
                {
                    UE_LOG(LogTemp, Log, TEXT("Friend user ID = %s"), *Result.friendsId[i]); 
                }
            }
        }));

    AccelByte::FRegistry::Lobby.SetGetFriendshipStatusResponseDelegate(
        AccelByte::Api::Lobby::FGetFriendshipStatusResponse::CreateLambda([](const FAccelByteModelsGetFriendshipStatusResponse& Result)
        {
            if (Result.Code != "0") { UE_LOG(LogTemp, Log, TEXT("Cannot get your friendship status!")); }
            else
            {
                const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ERelationshipStatusCode"), true);
                UE_LOG(LogTemp, Log, TEXT("Your friendship status is %s"), *(EnumPtr->GetNameByValue((int8)Result.friendshipStatus)).ToString());
            }
        }));
```

### Unbind delegates
Unbind all callbacks.
```cpp
AccelByte::FRegistry::Lobby.UnbindDelegates();
```

### Chat

#### Party chat
Send a party chat.
```cpp
AccelByte::FRegistry::Lobby.SendPartyMessage(TEXT("Hello, my party."));
```

#### Private message (PM)
Send a private message. Note that the `UserId` format in IAM is just a GUID without hyphens. The `UserId` used here is just for examples.
```cpp
AccelByte::FRegistry::Lobby.SendPrivateMessage(TEXT("Bahamut"), TEXT("Hello, Bahamut."));
```

### Party

#### Get party information
Get information about current party.
```cpp
AccelByte::FRegistry::Lobby.SendInfoPartyRequest();
```

#### Create a party
Create a party. You can't create a party when you're already in one.
```cpp
AccelByte::FRegistry::Lobby.SendCreatePartyRequest();
```

#### Leave current party
Leave current a party. You can't leave party when you're not in one.
```cpp
AccelByte::FRegistry::Lobby.SendLeavePartyRequest();
```

#### Invite someone to party
Invite someone to party.
```cpp
AccelByte::FRegistry::Lobby.SendInviteToPartyRequest(TEXT("Bahamut"));
```

#### Accept a party invitation
Invite someone to party. `PartyId` and `InvitationToken` are from the invitation notice.
```cpp
AccelByte::FRegistry::Lobby.SendAcceptInvitationRequest(TEXT("Bahamut's Party"), TEXT("Random text from the invitation notice"));
```

#### Kick a party member
Only party leader can kick a party member.
```cpp
AccelByte::FRegistry::Lobby.SendKickPartyMemberRequest(TEXT("Behemoth"));
```

### Presence

#### Get all online users
Get a list of all online users in the Lobby server.
```cpp
AccelByte::FRegistry::Lobby.SendGetOnlineUsersRequest();
```

#### Set my presence
Set user presence (availability and activity). 
```cpp
AccelByte::FRegistry::Lobby.SendSetPresenceStatus(Availability::Offline, TEXT("Away"));
```

### Notification

#### Fetch all pending notification
Get all pending notification(s) that is sent to user when user is not connected to lobby. Please call this function after user connected to lobby.
```cpp
AccelByte::FRegistry::Lobby.GetAllAsyncNotification();
```

### Matchmaking

#### Start matchmaking
Start the matchmaking with specified game mode. Please register a matchmaking channel first before send a matchmaking request.
Channel is just a string that registered to matchmaking server and the game developer that implement SDK should know the channel that already registered.
In this example, the game request a "4vs4" matchmaking. And the developer should configure or register the "4vs4" rule before that.
```cpp
AccelByte::FRegistry::Lobby.SendStartMatchmaking(TEXT("4vs4"));
```

#### Cancel matchmaking
Cancel the matchmaking request that already created.
```cpp
AccelByte::FRegistry::Lobby.SendCancelMatchmaking(TEXT("4vs4"));
```

### Friends

#### Friend request
Send a friend request to a user.
```cpp
AccelByte::FRegistry::Lobby.RequestFriend(FString UserId);
```

#### Unfriend
Unfriend a user.
```cpp
AccelByte::FRegistry::Lobby.Unfriend(FString UserId);
```

#### List outgoing friend request
Get all friend request that already send and has not accepted yet.
```cpp
AccelByte::FRegistry::Lobby.ListOutgoingFriends();
```

#### Cancel friend request
Cancel the friend request that already sent.
```cpp
AccelByte::FRegistry::Lobby.CancelFriendRequest(FString UserId);
```
#### List incoming friend request
Get all friend request that is sent to user.
```cpp
AccelByte::FRegistry::Lobby.ListIncomingFriends();
```

#### Accept friend request
Accepting an incoming friend request.
```cpp
AccelByte::FRegistry::Lobby.AcceptFriend(FString UserId);
```

#### Reject friend request
Reject an incoming friend request.
```cpp
AccelByte::FRegistry::Lobby.RejectFriend(FString UserId);
```

#### Load friend list
Get all friend request that is sent to user.
```cpp
AccelByte::FRegistry::Lobby.LoadFriendsList();
```

#### Get friendship status
Get friendship status with another user.
```cpp
AccelByte::FRegistry::Lobby.GetFriendshipStatus(FString UserId);
```

## Game Profile
GameProfile API to manage user's in-game profiles (character).
See `AccelByte::Api::GameProfile`,

### Batch Get Public Game Profiles
Get public game profiles from the specified user id(s).
```cpp
TArray<FAccelByteModelsPublicGameProfile> GetBatchPublicGameProfilesResult;
bool bBatchGetPublicGameProfiles = false;
TArray<FString> ArrayOfUserId;
ArrayOfUserId.Add(TEXT("userId12345"));
ArrayOfUserId.Add(TEXT("userId23456"));

FRegistry::GameProfile.BatchGetPublicGameProfiles(ArrayOfUserId, THandler<TArray<FAccelByteModelsPublicGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsPublicGameProfile>& Result)
    {
        bBatchGetPublicGameProfiles = true;
        GetBatchPublicGameProfilesResult = Result;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::GameProfile::BatchGetPublicGameProfiles()`.

### Get All Game Profiles
Get all user's game profiles.
```cpp
TArray<FAccelByteModelsGameProfile> GetAllGameProfileResult;
bool bGetAllGameProfileSuccess = false;

FRegistry::GameProfile.GetAllGameProfiles(THandler<TArray<FAccelByteModelsGameProfile>>::CreateLambda([&](const TArray<FAccelByteModelsGameProfile>& Result){
            UE_LOG(LogAccelByteGameProfileTest, Log, TEXT("\t\tsuccess"));
            GetAllGameProfileResult = Result;
            bGetAllGameProfileSuccess = true;
        }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::GameProfile::GetAllGameProfiles()`.

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

FRegistry::GameProfile.CreateGameProfile(Request, THandler<FAccelByteModelsGameProfile>::CreateLambda([&ActualResult, &bCreateGameProfileSuccess](const FAccelByteModelsGameProfile& Result){
        ActualResult = Result;
        bCreateGameProfileSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::GameProfile::CreateGameProfile()`.

### Get Game Profile
Get a specific game profile.
```cpp
FAccelByteModelsGameProfile GetResult;
bool bGetGameProfileSuccess = false;
FString profileId;

FRegistry::GameProfile.GetGameProfile(profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGetGameProfileSuccess, &GetResult](const FAccelByteModelsGameProfile& Result){
        GetResult = Result;
        bGetGameProfileSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::GameProfile::GetGameProfile()`.

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
FString profileId;

FRegistry::GameProfile.UpdateGameProfile(profileId, UpdateRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bUpdateGameProfileSuccess, &UpdateResult](const FAccelByteModelsGameProfile& Result){
        UpdateResult = Result;
        bUpdateGameProfileSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::GameProfile::UpdateGameProfile()`.

### Delete Game Profile
Delete a specific game profile.
```cpp
bool bDeleteGameProfileSuccess = false;
FString profileId;

FRegistry::GameProfile.DeleteGameProfile(profileId, FVoidHandler::CreateLambda([&bDeleteGameProfileSuccess]() {
        bDeleteGameProfileSuccess = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::GameProfile::DeleteGameProfile()`.

### Get Game Profile Attribute
Get an attribute value from a game profile.
```cpp
bool bGameProfileDoesntExist = false;
FString profileId = "profileid12345";

FRegistry::GameProfile.GetGameProfile(profileId, THandler<FAccelByteModelsGameProfile>::CreateLambda([&bGameProfileDoesntExist](const FAccelByteModelsGameProfile& Result) {
        bGameProfileDoesntExist = Result.profileId.IsEmpty();
    }), FErrorHandler::CreateLambda([&bGameProfileDoesntExist](int32 Code, FString Message) {
        bGameProfileDoesntExist = Code == EHttpResponseCodes::NotFound;
    }));
```
See `AccelByte::Api::GameProfile::GetGameProfileAttribute()`.

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
FString profileId;

FRegistry::GameProfile.UpdateGameProfileAttribute(profileId, AttributeRequest, THandler<FAccelByteModelsGameProfile>::CreateLambda([&](const FAccelByteModelsGameProfile& Result)
    {
        bUpdateGameProfileAttributeSuccess = true;
        UpdateResult = Result;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteGameProfileTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::GameProfile::UpdateGameProfileAttribute()`.

## Statistic
Statistic API manages user's statistics, e.g. : TOTAL_KILLS, TOTAL_DEATHS, TOTAL_ASSISTS, MVP, etc.
See `AccelByte::Api::Statistic`,
### Get All Stat Items
This function gets all stat items from the specified user id and profile id.
```cpp
bool bGetAllStatItemsResult = false;
FAccelByteModelsUserStatItemPagingSlicedResult GetResult;
const FString ProfileId = "profileid12345";

FRegistry::Statistic.GetAllStatItems(ProfileId, THandler<FAccelByteModelsUserStatItemPagingSlicedResult>::CreateLambda([&](const FAccelByteModelsUserStatItemPagingSlicedResult& Result)
	{
		bGetAllStatItemsResult = true;
		GetResult = Result;
	}), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::Statistic::GetAllStatItems()`.

### Get Stat Items By Stat Codes
This function gets stat item(s) from the specified user id and profile id by the stat code(s).
```cpp
bool bGetStatItemsByStatCodesResult = false;
const FString ProfileId = "profileid12345";
TArray<FString> StatCodes;
StatCodes.Add(TEXT("TOTAL_KILLS");
StatCodes.Add(TEXT("TOTAL_DEATHS");
TArray<FAccelByteModelsUserStatItemInfo> GetResult;

FRegistry::Statistic.GetStatItemsByStatCodes(ProfileId, StatCodes, const THandler<TArray<FAccelByteModelsUserStatItemInfo>>::CreateLambda([&](const TArray<FAccelByteModelsUserStatItemInfo>& Result)
	{
		bGetStatItemsByStatCodesResult = true;
		GetResult = Result;
	}), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteStatisticTest, Fatal, TEXT("Error code: %d\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::Statistic::GetStatItemsByStatCodes()`.

---

## Cloud Storage
Cloud Storage API for storing binary data on the cloud.
See `AccelByte::Api::CloudStorage`,

### Get All Slots
This function gets a list of slot(s) owned by the player.
```cpp
bool bGetAllSlotsResult = false;
TArray<FAccelByteModelsSlot> Results;
FRegistry::CloudStorage.GetAllSlots(THandler<TArray<FAccelByteModelsSlot>>::CreateLambda([&](const TArray<FAccelByteModelsSlot>& Slots)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Get All Slots Success"));
        Results = Slots;
        bGetAllSlotsResult = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::CloudStorage::GetAllSlots()`.

### Create Slot
This function creates a slot for uploaded binary data to be stored.
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

FRegistry::CloudStorage.CreateSlot(Payload, fileName, tags, label, customAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Create Slot Success"));
        CreatedSlot = Result;
        bSlotCreatedResult = true;
    }), nullptr, FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::CloudStorage::CreateSlot()`.

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

FRegistry::CloudStorage.UpdateSlot(SlotId, Payload, newFileName, newTag, newLabel, customAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Update Slot Success"));
        bSlotUpdatedResult = true;
    }), nullptr, FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::CloudStorage::UpdateSlot()`.

### Update Slot Metadata
This function updates a stored slot's metadata.
```cpp
FString SlotId = "1234567890" // example of slot ID that owned by the user;
FString UpdateFileName = "metadataUpdate.txt";
TArray<FString> UpdateTags;
UpdateTags.Add("tagOne");
UpdateTags.Add("tagTwo");
FString UpdateLabel = "metadataLabel";
FString UpdateCustomAttribute = TEXT("This is a custom attribute");

FRegistry::CloudStorage.UpdateSlotMetadata(CreatedSlot.SlotId, UpdateFileName, UpdateTags, UpdateLabel, UpdateCustomAttribute, THandler<FAccelByteModelsSlot>::CreateLambda([&](const FAccelByteModelsSlot& Result)
{
	UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Update Metadata Success"));
	bMetadataUpdatedResult = true;
	}),  nullptr, FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
	{
		UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
	}));
```

### Get Slot
This function gets the data that is stored in the slot.
```cpp
bool bGetSlotResult = false;
FString SlotId = "1234567890" // example of slot ID that owned by the user;

FRegistry::CloudStorage.GetSlot(SlotId, CloudStorage::FGetSlotSuccess::CreateLambda([&](const TArray<uint8>& Data)
    {
        bGetSlotResult = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::CloudStorage::GetSlot()`.

### Delete Slot
This function deletes the specified slot.
```cpp
bool bDeleteSlotResult = false;
FString SlotId = "1234567890" // example of slot ID that owned by the user;

FRegistry::CloudStorage.DeleteSlot(SlotId, FVoidHandler::CreateLambda([&]()
    {
        UE_LOG(LogAccelByteCloudStorageTest, Log, TEXT("Delete Slot Success"));
        bDeleteSlotResult = true;
    }), FErrorHandler::CreateLambda([](int32 ErrorCode, FString ErrorMessage)
    {
        UE_LOG(LogAccelByteCloudStorageTest, Fatal, TEXT("Error code: %s\nError message:%s"), ErrorCode, *ErrorMessage);
    }));
```
See `AccelByte::Api::CloudStorage::DeleteSlot()`.